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
static int use_odirect = 0;  /* O_DIRECT support for unbuffered I/O */

/* Pipe pool for splice operations */
#define PIPE_POOL_SIZE 32
static int pipe_pool[PIPE_POOL_SIZE][2];  /* [read_fd, write_fd] pairs */
static int pipe_pool_initialized = 0;
static int pipe_pool_available[PIPE_POOL_SIZE];

/* Splice operation context */
struct splice_context
{
    struct uring_async_data base;
    int pipe_fds[2];  /* temporary pipe for splice */
    int pipe_index;   /* index in pipe pool, or -1 if not from pool */
    int stage;        /* 0 = first splice, 1 = second splice */
};

/* Initialize pipe pool for splice operations */
static int init_pipe_pool(void)
{
    int i;
    
    if (pipe_pool_initialized) return 1;
    
    for (i = 0; i < PIPE_POOL_SIZE; i++)
    {
        if (pipe2(pipe_pool[i], O_CLOEXEC) != 0)
        {
            /* Free allocated pipes on failure */
            while (--i >= 0)
            {
                close(pipe_pool[i][0]);
                close(pipe_pool[i][1]);
                pipe_pool[i][0] = pipe_pool[i][1] = -1;
            }
            return 0;
        }
        pipe_pool_available[i] = 1;
    }
    
    pipe_pool_initialized = 1;
    return 1;
}

/* Allocate pipe from pool */
static int alloc_pipe_from_pool(int fds[2])
{
    int i;
    
    if (!pipe_pool_initialized && !init_pipe_pool())
        return -1;
    
    for (i = 0; i < PIPE_POOL_SIZE; i++)
    {
        if (pipe_pool_available[i])
        {
            fds[0] = pipe_pool[i][0];
            fds[1] = pipe_pool[i][1];
            pipe_pool_available[i] = 0;  /* Mark as used */
            return i;
        }
    }
    
    /* Pool exhausted, create a new pipe */
    if (pipe2(fds, O_CLOEXEC) == 0)
        return -1;  /* Not from pool */
    
    return -2;  /* Failed to create pipe */
}

/* Free pipe back to pool */
static void free_pipe_to_pool(int fds[2], int index)
{
    if (index >= 0 && index < PIPE_POOL_SIZE)
    {
        pipe_pool_available[index] = 1;
    }
    else if (fds[0] >= 0 && fds[1] >= 0)
    {
        close(fds[0]);
        close(fds[1]);
    }
}

/* Submit splice operation for zero-copy I/O */
static int submit_splice_operation(struct async *async, struct fd *fd, int is_read,
                                  ULONG count, LARGE_INTEGER offset, int client_socket_fd)
{
    struct splice_context *splice_ctx;
    struct io_uring_sqe *sqe1, *sqe2;
    int unix_fd;
    int pipe_index;
    
    if (!has_splice) return 0;  /* Splice not supported */
    
    /* Get the Unix file descriptor */
    unix_fd = get_unix_fd(fd);
    if (unix_fd < 0) return 0;
    
    /* Allocate splice context */
    splice_ctx = calloc(1, sizeof(*splice_ctx));
    if (!splice_ctx) return 0;
    
    /* Allocate pipe from pool */
    pipe_index = alloc_pipe_from_pool(splice_ctx->pipe_fds);
    if (pipe_index == -2)
    {
        free(splice_ctx);
        return 0;  /* Failed to create pipe */
    }
    
    splice_ctx->base.async = async;
    splice_ctx->base.fd = fd;
    splice_ctx->base.buffer = NULL;  /* No buffer needed for splice */
    splice_ctx->base.buffer_index = -1;
    splice_ctx->base.count = count;
    splice_ctx->base.offset = offset;
    splice_ctx->base.is_read = is_read;
    splice_ctx->pipe_index = pipe_index;
    splice_ctx->stage = 0;
    
    /* Get two SQEs for the splice chain */
    sqe1 = io_uring_get_sqe(&ring);
    sqe2 = io_uring_get_sqe(&ring);
    if (!sqe1 || !sqe2)
    {
        free_pipe_to_pool(splice_ctx->pipe_fds, pipe_index);
        free(splice_ctx);
        return 0;
    }
    
    if (is_read)
    {
        /* Read path: Disk FD -> Pipe -> Client Socket */
        /* First splice: disk -> pipe write end */
        io_uring_prep_splice(sqe1, unix_fd, offset.QuadPart,
                             splice_ctx->pipe_fds[1], -1, count, 0);
        io_uring_sqe_set_flags(sqe1, IOSQE_IO_LINK);
        io_uring_sqe_set_data(sqe1, splice_ctx);
        
        /* Second splice: pipe read end -> client socket */
        io_uring_prep_splice(sqe2, splice_ctx->pipe_fds[0], -1,
                             client_socket_fd, -1, count, 0);
        io_uring_sqe_set_data(sqe2, splice_ctx);
    }
    else
    {
        /* Write path: Client Socket -> Pipe -> Disk FD */
        /* First splice: client socket -> pipe write end */
        io_uring_prep_splice(sqe1, client_socket_fd, -1,
                             splice_ctx->pipe_fds[1], -1, count, 0);
        io_uring_sqe_set_flags(sqe1, IOSQE_IO_LINK);
        io_uring_sqe_set_data(sqe1, splice_ctx);
        
        /* Second splice: pipe read end -> disk */
        io_uring_prep_splice(sqe2, splice_ctx->pipe_fds[0], -1,
                             unix_fd, offset.QuadPart, count, 0);
        io_uring_sqe_set_data(sqe2, splice_ctx);
    }
    
    /* Submit the chain */
    if (io_uring_submit(&ring) < 0)
    {
        free_pipe_to_pool(splice_ctx->pipe_fds, pipe_index);
        free(splice_ctx);
        return 0;
    }
    
    return 1;  /* Success - splice operation submitted */
}

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
    struct splice_context *splice_ctx;
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
            
            /* Check if this is a splice context */
            splice_ctx = (struct splice_context *)data;
            int is_splice = (data->buffer == NULL && splice_ctx->pipe_fds[0] > 0);
            
            if (cqe->res < 0)
            {
                /* Convert errno to NTSTATUS */
                switch (-cqe->res)
                {
                    case EAGAIN:
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
                
                /* For splice operations, clean up pipes */
                if (is_splice)
                {
                    free_pipe_to_pool(splice_ctx->pipe_fds, splice_ctx->pipe_index);
                }
            }
            else
            {
                status = STATUS_SUCCESS;
                information = cqe->res;
                
                if (is_splice)
                {
                    /* For splice operations, handle chain completion */
                    if (splice_ctx->stage == 0)
                    {
                        /* First stage completed, wait for second stage */
                        splice_ctx->stage = 1;
                        io_uring_cqe_seen(&ring, cqe);
                        continue;  /* Don't complete async yet */
                    }
                    else
                    {
                        /* Both stages completed, clean up pipes */
                        free_pipe_to_pool(splice_ctx->pipe_fds, splice_ctx->pipe_index);
                    }
                }
                else
                {
                    /* For read operations, copy data from buffer to client if needed */
                    if (data->is_read && data->buffer && information > 0)
                    {
                        /* TODO: Copy data to client via IPC - this is the single copy in optimized mode */
                        /* For now, this is a placeholder for the actual IPC mechanism */
                    }
                }
            }
            
            /* Free buffer back to pool for non-splice operations */
            if (!is_splice && data->buffer)
                free_buffer_to_pool(data->buffer, data->buffer_index);
            
            /* Complete the async operation */
            async_terminate(data->async, status);
            free(data);
        }
        
        io_uring_cqe_seen(&ring, cqe);
    }
}

/* Cancel io_uring operation */
int wineio_cancel_async(struct async *async)
{
    struct io_uring_sqe *sqe;
    
    if (!uring_supported) return 0;
    
    /* Get submission queue entry for cancellation */
    sqe = io_uring_get_sqe(&ring);
    if (!sqe) return 0;
    
    /* Prepare async cancel operation */
    io_uring_prep_cancel(sqe, async, 0);
    io_uring_sqe_set_data(sqe, NULL);  /* No completion data needed */
    
    /* Submit the cancellation */
    if (io_uring_submit(&ring) < 0)
        return 0;
    
    return 1;  /* Cancellation submitted */
}
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
    
    /* Try splice first for true zero-copy (if we had client socket FD) */
    /* For now, we'll implement this as a placeholder since we need IPC integration */
    /* TODO: Implement splice when client socket FD is available
    if (has_splice && client_socket_fd >= 0) {
        if (submit_splice_operation(async, fd, is_read, count, offset, client_socket_fd))
            return 1;
    }
    */
    
    /* Fall back to optimized single-copy approach */
    
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
    
    /* Clean up pipe pool */
    if (pipe_pool_initialized)
    {
        for (i = 0; i < PIPE_POOL_SIZE; i++)
        {
            if (pipe_pool[i][0] >= 0)
            {
                close(pipe_pool[i][0]);
                close(pipe_pool[i][1]);
                pipe_pool[i][0] = pipe_pool[i][1] = -1;
            }
        }
        pipe_pool_initialized = 0;
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