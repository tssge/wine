/*
 * Server-side io_uring file operations
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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "ntstatus.h"
#define WIN32_NO_STATUS
#include "windef.h"
#include "winternl.h"

#include "file.h"
#include "handle.h"
#include "thread.h"
#include "request.h"
#include "process.h"
#include "security.h"

#ifdef HAVE_IO_URING
#include "io_uring.h"

/* Forward declarations */
struct async;

/* Enhanced file operations using io_uring when available */
static void io_uring_file_queue_async( struct fd *fd, struct async *async, int type, int count )
{
    struct file *file = get_fd_user( fd );
    int unix_fd;
    int ret;
    
    /* Check if we should use io_uring for this file */
    if (!file || !should_use_io_uring_for_file(file) || !io_uring_available()) {
        /* Fall back to default behavior */
        default_fd_queue_async( fd, async, type, count );
        return;
    }
    
    unix_fd = get_unix_fd( fd );
    if (unix_fd < 0) {
        /* Fall back to default behavior */
        default_fd_queue_async( fd, async, type, count );
        return;
    }
    
    /* Try to queue the operation with io_uring */
    switch (type) {
    case ASYNC_TYPE_READ:
        /* For now, we can't access async->data fields directly, so fall back */
        /* TODO: Need proper access to async data structure */
        default_fd_queue_async( fd, async, type, count );
        break;
        
    case ASYNC_TYPE_WRITE:
        /* For now, we can't access async->data fields directly, so fall back */
        /* TODO: Need proper access to async data structure */
        default_fd_queue_async( fd, async, type, count );
        break;
        
    default:
        /* Fall back to default behavior */
        default_fd_queue_async( fd, async, type, count );
        break;
    }
}

/* io_uring enhanced fd operations */
static const struct fd_ops io_uring_file_fd_ops =
{
    default_fd_get_poll_events,   /* get_poll_events */
    default_fd_poll_event,        /* poll_event */
    no_fd_flush,                  /* flush */
    default_fd_get_fd_type,       /* get_fd_type */
    default_fd_ioctl,             /* ioctl */
    io_uring_file_queue_async,    /* queue_async - enhanced with io_uring */
    default_fd_reselect_async,    /* reselect_async */
    default_fd_cancel_async       /* cancel_async */
};

/* Create an io_uring-enhanced file object */
struct file *create_io_uring_file_for_fd( int fd, unsigned int access, unsigned int sharing )
{
    /* For now, just create a regular file */
    /* TODO: Implement io_uring enhanced file creation */
    return create_file_for_fd( fd, access, sharing );
}

/* Check if a file should use io_uring */
int should_use_io_uring_for_file( struct file *file )
{
    /* Only use io_uring for regular files and block devices */
    /* This avoids issues with special files, sockets, pipes, etc. */
    if (!file || !file->fd) return 0;
    
    /* Check if it's a regular file or block device */
    if (S_ISREG(file->mode) || S_ISBLK(file->mode)) {
        return 1;
    }
    
    /* Don't use io_uring for special files, sockets, etc. */
    return 0;
}

/* Initialize io_uring file support */
int io_uring_file_init(void)
{
    /* This function can be used for any io_uring file-specific initialization */
    return io_uring_available() ? 1 : 0;
}

#else /* !HAVE_IO_URING */

/* Stub implementations when io_uring is not available */
struct file *create_io_uring_file_for_fd( int fd, unsigned int access, unsigned int sharing )
{
    return create_file_for_fd( fd, access, sharing );
}

int should_use_io_uring_for_file( struct file *file )
{
    return 0;
}

int io_uring_file_init(void)
{
    return 0;
}

#endif /* HAVE_IO_URING */