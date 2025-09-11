/*
 * Server-side io_uring support
 *
 * Copyright (C) 2024 Wine Project
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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

#ifdef HAVE_IO_URING
#include <linux/io_uring.h>

#include "ntstatus.h"
#define WIN32_NO_STATUS
#include "windef.h"
#include "winternl.h"

#include "file.h"
#include "request.h"
#include "process.h"
#include "handle.h"
#include "thread.h"

/* Forward declarations */
extern void async_terminate( struct async *async, unsigned int status );

/* io_uring syscalls */
static inline int io_uring_setup(unsigned entries, struct io_uring_params *p)
{
    return syscall(__NR_io_uring_setup, entries, p);
}

static inline int io_uring_enter(int fd, unsigned to_submit, unsigned min_complete, unsigned flags, sigset_t *sig)
{
    return syscall(__NR_io_uring_enter, fd, to_submit, min_complete, flags, sig);
}

static inline int io_uring_register(int fd, unsigned opcode, void *arg, unsigned nr_args)
{
    return syscall(__NR_io_uring_register, fd, opcode, arg, nr_args);
}

/* io_uring state */
static int io_uring_fd = -1;
static struct io_uring_params io_uring_params;
static void *sq_ring = NULL;
static void *cq_ring = NULL;
static struct io_uring_sqe *sqes = NULL;
static int io_uring_entries = 0;
static int io_uring_enabled = 0;

/* io_uring ring pointers */
static volatile unsigned *sq_head;
static volatile unsigned *sq_tail;
static volatile unsigned *sq_ring_mask;
static volatile unsigned *sq_ring_entries;
static volatile unsigned *sq_flags;
static volatile unsigned *sq_dropped;
static unsigned *sq_array;

static volatile unsigned *cq_head;
static volatile unsigned *cq_tail;
static volatile unsigned *cq_ring_mask;
static volatile unsigned *cq_ring_entries;
static volatile unsigned *cq_overflow;
static struct io_uring_cqe *cqes;

/* io_uring operation tracking */
struct wine_io_uring_op {
    struct async *async;
    __u64 user_data;
    int opcode;
    int fd;
    void *buf;
    size_t len;
    off_t offset;
};

#define MAX_IO_URING_OPS 1024
static struct wine_io_uring_op io_uring_ops[MAX_IO_URING_OPS];

/* Check if io_uring is supported by the kernel */
static int check_io_uring_support(void)
{
    struct io_uring_params params = {0};
    int fd;
    
    /* Try to create an io_uring instance */
    fd = io_uring_setup(1, &params);
    if (fd < 0) {
        if (debug_level)
            fprintf(stderr, "io_uring not supported: %s\n", strerror(errno));
        return 0;
    }
    
    close(fd);
    if (debug_level)
        fprintf(stderr, "io_uring supported\n");
    return 1;
}

/* Initialize io_uring */
static int init_io_uring(void)
{
    struct io_uring_params params = {0};
    size_t sq_size, cq_size;
    
    if (!check_io_uring_support()) {
        return 0;
    }
    
    /* Setup io_uring with reasonable defaults */
    io_uring_entries = 256; /* Start with 256 entries */
    params.flags = IORING_SETUP_COOP_TASKRUN | IORING_SETUP_SINGLE_ISSUER;
    
    io_uring_fd = io_uring_setup(io_uring_entries, &params);
    if (io_uring_fd < 0) {
        if (debug_level)
            fprintf(stderr, "io_uring_setup failed: %s\n", strerror(errno));
        return 0;
    }
    
    io_uring_params = params;
    
    /* Map submission queue */
    sq_size = params.sq_off.array + params.sq_entries * sizeof(unsigned);
    sq_ring = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
                   io_uring_fd, IORING_OFF_SQ_RING);
    if (sq_ring == MAP_FAILED) {
        if (debug_level)
            fprintf(stderr, "mmap sq_ring failed: %s\n", strerror(errno));
        close(io_uring_fd);
        io_uring_fd = -1;
        return 0;
    }
    
    /* Map completion queue */
    cq_size = params.cq_off.cqes + params.cq_entries * sizeof(struct io_uring_cqe);
    cq_ring = mmap(NULL, cq_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
                   io_uring_fd, IORING_OFF_CQ_RING);
    if (cq_ring == MAP_FAILED) {
        if (debug_level)
            fprintf(stderr, "mmap cq_ring failed: %s\n", strerror(errno));
        munmap(sq_ring, sq_size);
        close(io_uring_fd);
        io_uring_fd = -1;
        return 0;
    }
    
    /* Map submission queue entries */
    sqes = mmap(NULL, params.sq_entries * sizeof(struct io_uring_sqe),
                PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
                io_uring_fd, IORING_OFF_SQES);
    if (sqes == MAP_FAILED) {
        if (debug_level)
            fprintf(stderr, "mmap sqes failed: %s\n", strerror(errno));
        munmap(sq_ring, sq_size);
        munmap(cq_ring, cq_size);
        close(io_uring_fd);
        io_uring_fd = -1;
        return 0;
    }
    
    /* Setup ring pointers */
    sq_head = (volatile unsigned *)((char *)sq_ring + params.sq_off.head);
    sq_tail = (volatile unsigned *)((char *)sq_ring + params.sq_off.tail);
    sq_ring_mask = (volatile unsigned *)((char *)sq_ring + params.sq_off.ring_mask);
    sq_ring_entries = (volatile unsigned *)((char *)sq_ring + params.sq_off.ring_entries);
    sq_flags = (volatile unsigned *)((char *)sq_ring + params.sq_off.flags);
    sq_dropped = (volatile unsigned *)((char *)sq_ring + params.sq_off.dropped);
    sq_array = (unsigned *)((char *)sq_ring + params.sq_off.array);
    
    cq_head = (volatile unsigned *)((char *)cq_ring + params.cq_off.head);
    cq_tail = (volatile unsigned *)((char *)cq_ring + params.cq_off.tail);
    cq_ring_mask = (volatile unsigned *)((char *)cq_ring + params.cq_off.ring_mask);
    cq_ring_entries = (volatile unsigned *)((char *)cq_ring + params.cq_off.ring_entries);
    cq_overflow = (volatile unsigned *)((char *)cq_ring + params.cq_off.overflow);
    cqes = (struct io_uring_cqe *)((char *)cq_ring + params.cq_off.cqes);
    
    io_uring_enabled = 1;
    
    if (debug_level)
        fprintf(stderr, "io_uring initialized successfully (entries=%d)\n", io_uring_entries);
    
    return 1;
}

/* Cleanup io_uring */
static void cleanup_io_uring(void)
{
    if (sqes) {
        munmap(sqes, io_uring_entries * sizeof(struct io_uring_sqe));
        sqes = NULL;
    }
    if (cq_ring) {
        size_t cq_size = io_uring_params.cq_off.cqes + io_uring_entries * sizeof(struct io_uring_cqe);
        munmap(cq_ring, cq_size);
        cq_ring = NULL;
    }
    if (sq_ring) {
        size_t sq_size = io_uring_params.sq_off.array + io_uring_entries * sizeof(unsigned);
        munmap(sq_ring, sq_size);
        sq_ring = NULL;
    }
    if (io_uring_fd >= 0) {
        close(io_uring_fd);
        io_uring_fd = -1;
    }
    io_uring_enabled = 0;
}

/* Check if io_uring is available */
int io_uring_available(void)
{
    return io_uring_enabled;
}

/* Get next available submission queue entry */
static struct io_uring_sqe *get_sqe(void)
{
    unsigned head, tail, next_tail, mask;
    
    head = *sq_head;
    tail = *sq_tail;
    mask = *sq_ring_mask;
    
    next_tail = tail + 1;
    if (next_tail == head) {
        /* Queue is full */
        return NULL;
    }
    
    return &sqes[tail & mask];
}

/* Submit operations to io_uring */
static int submit_io_uring_ops(int count)
{
    unsigned tail = *sq_tail;
    int ret;
    
    if (count == 0) return 0;
    
    /* Update tail pointer */
    *sq_tail = tail + count;
    
    /* Submit to kernel */
    ret = io_uring_enter(io_uring_fd, count, 0, IORING_ENTER_GETEVENTS, NULL);
    if (ret < 0) {
        if (debug_level)
            fprintf(stderr, "io_uring_enter failed: %s\n", strerror(errno));
        return -1;
    }
    
    return ret;
}

/* Process completion queue */
static int process_completions(void)
{
    unsigned head, tail, mask;
    int processed = 0;
    
    head = *cq_head;
    tail = *cq_tail;
    mask = *cq_ring_mask;
    
    while (head != tail) {
        struct io_uring_cqe *cqe = &cqes[head & mask];
        struct wine_io_uring_op *op;
        struct async *async;
        
        /* Find the operation */
        if (cqe->user_data < MAX_IO_URING_OPS) {
            op = &io_uring_ops[cqe->user_data];
            async = op->async;
            
            if (async) {
                /* Complete the async operation */
                unsigned int status = cqe->res >= 0 ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
                async_terminate(async, status);
                
                /* Clear the operation */
                op->async = NULL;
                op->user_data = 0;
            }
        }
        
        head++;
        processed++;
    }
    
    /* Update head pointer */
    *cq_head = head;
    
    return processed;
}

/* Queue a read operation */
int io_uring_queue_read(struct async *async, int fd, void *buf, size_t len, off_t offset)
{
    struct io_uring_sqe *sqe;
    int op_index;
    
    if (!io_uring_enabled) return -1;
    
    /* Find free operation slot */
    for (op_index = 0; op_index < MAX_IO_URING_OPS; op_index++) {
        if (io_uring_ops[op_index].async == NULL) break;
    }
    if (op_index >= MAX_IO_URING_OPS) return -1;
    
    /* Get submission queue entry */
    sqe = get_sqe();
    if (!sqe) return -1;
    
    /* Setup operation */
    memset(sqe, 0, sizeof(*sqe));
    sqe->opcode = IORING_OP_READ;
    sqe->fd = fd;
    sqe->addr = (__u64)(uintptr_t)buf;
    sqe->len = len;
    sqe->off = offset;
    sqe->user_data = op_index;
    
    /* Track operation */
    io_uring_ops[op_index].async = async;
    io_uring_ops[op_index].user_data = op_index;
    io_uring_ops[op_index].opcode = IORING_OP_READ;
    io_uring_ops[op_index].fd = fd;
    io_uring_ops[op_index].buf = buf;
    io_uring_ops[op_index].len = len;
    io_uring_ops[op_index].offset = offset;
    
    /* Submit */
    return submit_io_uring_ops(1);
}

/* Queue a write operation */
int io_uring_queue_write(struct async *async, int fd, const void *buf, size_t len, off_t offset)
{
    struct io_uring_sqe *sqe;
    int op_index;
    
    if (!io_uring_enabled) return -1;
    
    /* Find free operation slot */
    for (op_index = 0; op_index < MAX_IO_URING_OPS; op_index++) {
        if (io_uring_ops[op_index].async == NULL) break;
    }
    if (op_index >= MAX_IO_URING_OPS) return -1;
    
    /* Get submission queue entry */
    sqe = get_sqe();
    if (!sqe) return -1;
    
    /* Setup operation */
    memset(sqe, 0, sizeof(*sqe));
    sqe->opcode = IORING_OP_WRITE;
    sqe->fd = fd;
    sqe->addr = (__u64)(uintptr_t)buf;
    sqe->len = len;
    sqe->off = offset;
    sqe->user_data = op_index;
    
    /* Track operation */
    io_uring_ops[op_index].async = async;
    io_uring_ops[op_index].user_data = op_index;
    io_uring_ops[op_index].opcode = IORING_OP_WRITE;
    io_uring_ops[op_index].fd = fd;
    io_uring_ops[op_index].buf = (void *)buf;
    io_uring_ops[op_index].len = len;
    io_uring_ops[op_index].offset = offset;
    
    /* Submit */
    return submit_io_uring_ops(1);
}

/* Process io_uring events (called from main loop) */
int io_uring_process_events(void)
{
    if (!io_uring_enabled) return 0;
    
    return process_completions();
}

/* Initialize io_uring support */
int io_uring_init(void)
{
    return init_io_uring();
}

/* Cleanup io_uring support */
void io_uring_cleanup(void)
{
    cleanup_io_uring();
}

#else /* !HAVE_IO_URING */

/* Stub implementations when io_uring is not available */
int io_uring_available(void) { return 0; }
int io_uring_init(void) { return 0; }
void io_uring_cleanup(void) { }
int io_uring_queue_read(struct async *async, int fd, void *buf, size_t len, off_t offset) { return -1; }
int io_uring_queue_write(struct async *async, int fd, const void *buf, size_t len, off_t offset) { return -1; }
int io_uring_process_events(void) { return 0; }

#endif /* HAVE_IO_URING */

