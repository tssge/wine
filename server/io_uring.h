/*
 * Server io_uring support header
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

#ifndef __WINE_SERVER_IO_URING_H
#define __WINE_SERVER_IO_URING_H

#include "object.h"
#include "file.h"

/* Initialize io_uring engine */
extern int wineio_init(void);

/* Get eventfd for main event loop integration */
extern int wineio_get_eventfd(void);

/* Process io_uring completions */
extern void wineio_process_completions(void);

/* Submit I/O request via io_uring */
extern int wineio_submit_io(struct async *async, struct fd *fd, int is_read, 
                           ULONG count, LARGE_INTEGER offset);

/* Cancel I/O request via io_uring */
extern int wineio_cancel_async(struct async *async);

/* Cleanup io_uring engine */
extern void wineio_cleanup(void);

#endif /* __WINE_SERVER_IO_URING_H */