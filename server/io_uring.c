/*
 * Server io_uring support
 *
 * Copyright (C) 2024 Wine contributors
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "config.h"

#ifdef HAVE_LIBURING_H

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <liburing.h>

#include "ntstatus.h"
#define WIN32_NO_STATUS
#include "windef.h"
#include "winternl.h"

#include "object.h"
#include "file.h"
#include "request.h"
#include "process.h"
#include "handle.h"

/* io_uring context */
static struct io_uring ring;
static int ring_fd = -1;
static int eventfd_fd = -1;
static int uring_supported = 0;

/* Feature flags */
static int has_splice = 0;
static int has_read_fixed = 0;
static int has_write_fixed = 0;
static int has_register_buffers = 0;

/* Buffer pool for optimized single-copy operations */
#define BUFFER_POOL_SIZE 64
#define BUFFER_SIZE (64 * 1024)  /* 64KB buffers */
static void *buffer_pool[BUFFER_POOL_SIZE];
static int buffer_pool_initialized = 0;

struct uring_async_data
{
    struct async *async;
    struct fd *fd;
    void *buffer;
    int buffer_index;  /* Index in buffer pool, or -1 if not from pool */
    ULONG count;
    LARGE_INTEGER offset;
    int is_read;
};

/* Initialize buffer pool for zero-copy operations */
static int init_buffer_pool(void)
{
    int i;
    
    if (buffer_pool_initialized) return 1;
    
    for (i = 0; i < BUFFER_POOL_SIZE; i++)
    {
        /* Allocate aligned buffers for O_DIRECT support */
        if (posix_memalign(&buffer_pool[i], 4096, BUFFER_SIZE) != 0)
        {
            /* Free allocated buffers on failure */
            while (--i >= 0)
            {
                free(buffer_pool[i]);
                buffer_pool[i] = NULL;
            }
            return 0;
        }
    }
    
    /* Register buffers with io_uring if supported */
    if (has_register_buffers)
    {
        struct iovec iovecs[BUFFER_POOL_SIZE];
        for (i = 0; i < BUFFER_POOL_SIZE; i++)
        {
            iovecs[i].iov_base = buffer_pool[i];
            iovecs[i].iov_len = BUFFER_SIZE;
        }
        
        if (io_uring_register_buffers(&ring, iovecs, BUFFER_POOL_SIZE) < 0)
        {
            /* Registration failed, but we can still use regular buffers */
            has_register_buffers = 0;
        }
    }
    
    buffer_pool_initialized = 1;
    return 1;
}

/* Allocate buffer from pool */
static void *alloc_buffer_from_pool(int *index)
{
    int i;
    
    if (!buffer_pool_initialized && !init_buffer_pool())
        return NULL;
    
    for (i = 0; i < BUFFER_POOL_SIZE; i++)
    {
        if (buffer_pool[i])
        {
            void *buffer = buffer_pool[i];
            buffer_pool[i] = NULL;  /* Mark as used */
            *index = i;
            return buffer;
        }
    }
    
    *index = -1;
    return NULL;
}

/* Free buffer back to pool */
static void free_buffer_to_pool(void *buffer, int index)
{
    if (index >= 0 && index < BUFFER_POOL_SIZE)
    {
        buffer_pool[index] = buffer;
    }
    else if (buffer)
    {
        free(buffer);
    }
}

/* Initialize io_uring engine */
int wineio_init(void)
{
    struct io_uring_params params;
    struct io_uring_probe *probe;
    int ret;
    
    if (uring_supported) return 1;  /* Already initialized */
    
    memset(&params, 0, sizeof(params));
    
    /* Initialize io_uring with reasonable queue depth */
    ret = io_uring_queue_init_params(256, &ring, &params);
    if (ret < 0)
    {
        fprintf(stderr, "wineserver: io_uring_queue_init failed: %s\n", strerror(-ret));
        return 0;
    }
    
    ring_fd = ring.ring_fd;
    
    /* Create eventfd for integration with main event loop */
    eventfd_fd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (eventfd_fd < 0)
    {
        io_uring_queue_exit(&ring);
        ring_fd = -1;
        return 0;
    }
    
    /* Register eventfd with io_uring */
    ret = io_uring_register_eventfd(&ring, eventfd_fd);
    if (ret < 0)
    {
        close(eventfd_fd);
        io_uring_queue_exit(&ring);
        ring_fd = eventfd_fd = -1;
        return 0;
    }
    
    /* Probe for supported operations */
    probe = io_uring_get_probe_ring(&ring);
    if (probe)
    {
        has_splice = io_uring_opcode_supported(probe, IORING_OP_SPLICE);
        has_read_fixed = io_uring_opcode_supported(probe, IORING_OP_READ_FIXED);
        has_write_fixed = io_uring_opcode_supported(probe, IORING_OP_WRITE_FIXED);
        
        /* Check if buffer registration is supported */
        has_register_buffers = (ring.features & IORING_FEAT_SINGLE_MMAP) != 0;
        
        free(probe);
    }
    
    uring_supported = 1;
    
    printf("wineserver: io_uring initialized with features: splice=%d, read_fixed=%d, write_fixed=%d, register_buffers=%d\n",
           has_splice, has_read_fixed, has_write_fixed, has_register_buffers);
    
    return 1;
}

/* Get eventfd for main event loop integration */
int wineio_get_eventfd(void)
{
    return eventfd_fd;
}

/* Process io_uring completions */
void wineio_process_completions(void)
{
    struct io_uring_cqe *cqe;
    struct uring_async_data *data;
    uint64_t eventfd_value;
    
    if (!uring_supported) return;
    
    /* Clear the eventfd */
    read(eventfd_fd, &eventfd_value, sizeof(eventfd_value));
    
    /* Process all available completions */
    while (io_uring_peek_cqe(&ring, &cqe) == 0)
    {
        data = (struct uring_async_data *)cqe->user_data;
        
        if (data)
        {
            NTSTATUS status;
            ULONG_PTR information = 0;
            
            if (cqe->res < 0)
            {
                /* Convert errno to NTSTATUS */
                switch (-cqe->res)
                {
                    case EAGAIN:
                    case EWOULDBLOCK:
                        status = STATUS_PENDING;
                        break;
                    case EBADF:
                        status = STATUS_INVALID_HANDLE;
                        break;
                    case EINVAL:
                        status = STATUS_INVALID_PARAMETER;
                        break;
                    case ENOMEM:
                        status = STATUS_NO_MEMORY;
                        break;
                    case ENOSPC:
                        status = STATUS_DISK_FULL;
                        break;
                    case EACCES:
                        status = STATUS_ACCESS_DENIED;
                        break;
                    default:
                        status = STATUS_UNSUCCESSFUL;
                        break;
                }
            }
            else
            {
                status = STATUS_SUCCESS;
                information = cqe->res;
                
                /* For read operations, copy data from buffer to client if needed */
                if (data->is_read && data->buffer && information > 0)
                {
                    /* TODO: Copy data to client via IPC - this is the single copy in optimized mode */
                    /* For now, this is a placeholder for the actual IPC mechanism */
                }
            }
            
            /* Free buffer back to pool */
            if (data->buffer)
                free_buffer_to_pool(data->buffer, data->buffer_index);
            
            /* Complete the async operation */
            async_terminate(data->async, status, information);
            free(data);
        }
        
        io_uring_cqe_seen(&ring, cqe);
    }
}

/* Submit I/O request via io_uring */
int wineio_submit_io(struct async *async, struct fd *fd, int is_read, 
                     ULONG count, LARGE_INTEGER offset)
{
    struct uring_async_data *data;
    struct io_uring_sqe *sqe;
    void *buffer;
    int buffer_index;
    int unix_fd;
    
    if (!uring_supported) return 0;  /* Fall back to traditional I/O */
    
    /* Get the Unix file descriptor */
    unix_fd = get_unix_fd(fd);
    if (unix_fd < 0) return 0;
    
    /* Allocate operation data */
    data = calloc(1, sizeof(*data));
    if (!data) return 0;
    
    /* Allocate buffer from pool for optimized single-copy */
    buffer = alloc_buffer_from_pool(&buffer_index);
    if (!buffer)
    {
        /* Fall back to regular malloc */
        buffer = malloc(count);
        if (!buffer)
        {
            free(data);
            return 0;
        }
        buffer_index = -1;
    }
    
    data->async = async;
    data->fd = fd;
    data->buffer = buffer;
    data->buffer_index = buffer_index;
    data->count = count;
    data->offset = offset;
    data->is_read = is_read;
    
    /* Get submission queue entry */
    sqe = io_uring_get_sqe(&ring);
    if (!sqe)
    {
        free_buffer_to_pool(buffer, buffer_index);
        free(data);
        return 0;
    }
    
    /* Prepare operation - use fixed buffers if available */
    if (buffer_index >= 0 && has_register_buffers)
    {
        if (is_read)
        {
            if (has_read_fixed)
                io_uring_prep_read_fixed(sqe, unix_fd, buffer, count, offset.QuadPart, buffer_index);
            else
                io_uring_prep_read(sqe, unix_fd, buffer, count, offset.QuadPart);
        }
        else
        {
            if (has_write_fixed)
                io_uring_prep_write_fixed(sqe, unix_fd, buffer, count, offset.QuadPart, buffer_index);
            else
                io_uring_prep_write(sqe, unix_fd, buffer, count, offset.QuadPart);
        }
    }
    else
    {
        /* Use regular operations */
        if (is_read)
            io_uring_prep_read(sqe, unix_fd, buffer, count, offset.QuadPart);
        else
            io_uring_prep_write(sqe, unix_fd, buffer, count, offset.QuadPart);
    }
    
    /* Set user data for completion handling */
    io_uring_sqe_set_data(sqe, data);
    
    /* Submit the operation */
    if (io_uring_submit(&ring) < 0)
    {
        free_buffer_to_pool(buffer, buffer_index);
        free(data);
        return 0;
    }
    
    return 1;  /* Success - operation submitted */
}

/* Cleanup io_uring engine */
void wineio_cleanup(void)
{
    int i;
    
    if (!uring_supported) return;
    
    /* Unregister buffers if they were registered */
    if (has_register_buffers && buffer_pool_initialized)
        io_uring_unregister_buffers(&ring);
    
    /* Free buffer pool */
    if (buffer_pool_initialized)
    {
        for (i = 0; i < BUFFER_POOL_SIZE; i++)
        {
            if (buffer_pool[i])
            {
                free(buffer_pool[i]);
                buffer_pool[i] = NULL;
            }
        }
        buffer_pool_initialized = 0;
    }
    
    /* Cleanup io_uring */
    if (eventfd_fd >= 0)
    {
        close(eventfd_fd);
        eventfd_fd = -1;
    }
    
    if (ring_fd >= 0)
    {
        io_uring_queue_exit(&ring);
        ring_fd = -1;
    }
    
    uring_supported = 0;
}

#else /* !HAVE_LIBURING_H */

/* Stub implementations when io_uring is not available */
int wineio_init(void) { return 0; }
int wineio_get_eventfd(void) { return -1; }
void wineio_process_completions(void) { }
int wineio_submit_io(struct async *async, struct fd *fd, int is_read, ULONG count, LARGE_INTEGER offset) { return 0; }
void wineio_cleanup(void) { }

#endif /* HAVE_LIBURING_H */