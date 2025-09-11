/*
 * io_uring enhanced file operations header
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

#ifndef __WINE_SERVER_IO_URING_FILE_H
#define __WINE_SERVER_IO_URING_FILE_H

#include "object.h"

struct file;

#ifdef HAVE_IO_URING

/* io_uring enhanced file operations */
extern struct file *create_io_uring_file_for_fd( int fd, unsigned int access, unsigned int sharing );
extern int should_use_io_uring_for_file( struct file *file );
extern int io_uring_file_init(void);

#else /* !HAVE_IO_URING */

/* Stub declarations when io_uring is not available */
#define create_io_uring_file_for_fd(fd, access, sharing) create_file_for_fd(fd, access, sharing)
#define should_use_io_uring_for_file(file) 0
#define io_uring_file_init() 0

#endif /* HAVE_IO_URING */

#endif /* __WINE_SERVER_IO_URING_FILE_H */

