/*
 * Server-side io_uring support header
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

#ifndef __WINE_SERVER_IO_URING_H
#define __WINE_SERVER_IO_URING_H

#include "object.h"

struct async;

#ifdef HAVE_IO_URING

/* io_uring availability and initialization */
extern int io_uring_available(void);
extern int io_uring_init(void);
extern void io_uring_cleanup(void);

/* io_uring operations */
extern int io_uring_queue_read(struct async *async, int fd, void *buf, size_t len, off_t offset);
extern int io_uring_queue_write(struct async *async, int fd, const void *buf, size_t len, off_t offset);

/* io_uring event processing */
extern int io_uring_process_events(void);

#else /* !HAVE_IO_URING */

/* Stub declarations when io_uring is not available */
#define io_uring_available() 0
#define io_uring_init() 0
#define io_uring_cleanup() do { } while(0)
#define io_uring_queue_read(a,b,c,d,e) (-1)
#define io_uring_queue_write(a,b,c,d,e) (-1)
#define io_uring_process_events() 0

#endif /* HAVE_IO_URING */

#endif /* __WINE_SERVER_IO_URING_H */

