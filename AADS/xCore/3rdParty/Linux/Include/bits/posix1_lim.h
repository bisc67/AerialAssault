/* Copyright (C) 1991,92,93,96,98,2000,2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/*
 *	POSIX Standard: 2.9.2 Minimum Values	Added to <limits.h>
 *
 *	Never include this file directly; use <limits.h> instead.
 */

#ifndef	_BITS_POSIX1_LIM_H
#define	_BITS_POSIX1_LIM_H	1


/* These are the standard-mandated minimum values.  */

/* Minimum number of operations in one list I/O call.  */
#define _POSIX_AIO_LISTIO_MAX	2

/* Minimal number of outstanding asynchronous I/O operations.  */
#define _POSIX_AIO_MAX		1

/* Maximum length of arguments to `execve', including environment.  */
#define	_POSIX_ARG_MAX		4096

/* Maximum simultaneous processes per real user ID.  */
#define	_POSIX_CHILD_MAX	6

/* Minimal number of timer expiration overruns.  */
#define _POSIX_DELAYTIMER_MAX	32

/* Maximum link count of a file.  */
#define	_POSIX_LINK_MAX		8

/* Number of bytes in a terminal canonical input queue.  */
#define	_POSIX_MAX_CANON	255

/* Number of bytes for which space will be
   available in a terminal input queue.  */
#define	_POSIX_MAX_INPUT	255

/* Maximum number of message queues open for a process.  */
#define _POSIX_MQ_OPEN_MAX	8

/* Maximum number of supported message priorities.  */
#define _POSIX_MQ_PRIO_MAX	32

/* Number of simultaneous supplementary group IDs per process.  */
#define	_POSIX_NGROUPS_MAX	0

/* Number of files one process can have open at once.  */
#define	_POSIX_OPEN_MAX		16

/* Number of descriptors that a process may examine with `pselect' or
   `select'.  */
#define	_POSIX_FD_SETSIZE	_POSIX_OPEN_MAX

/* Number of bytes in a filename.  */
#define	_POSIX_NAME_MAX		14

/* Number of bytes in a pathname.  */
#define	_POSIX_PATH_MAX		256

/* Number of bytes than can be written atomically to a pipe.  */
#define	_POSIX_PIPE_BUF		512

/* Minimal number of realtime signals reserved for the application.  */
#define _POSIX_RTSIG_MAX	8

/* Number of semaphores a process can have.  */
#define _POSIX_SEM_NSEMS_MAX	256

/* Maximal value of a semaphore.  */
#define _POSIX_SEM_VALUE_MAX	32767

/* Number of pending realtime signals.  */
#define _POSIX_SIGQUEUE_MAX	32

/* Largest value of a `ssize_t'.  */
#define	_POSIX_SSIZE_MAX	32767

/* Number of streams a process can have open at once.  */
#define	_POSIX_STREAM_MAX	8

/* Maximum length of a timezone name (element of `tzname').  */
#define	_POSIX_TZNAME_MAX	6

/* Maximum number of connections that can be queued on a socket.  */
#define	_POSIX_QLIMIT		1

/* Maximum number of bytes that can be buffered on a socket for send
   or receive.  */
#define	_POSIX_HIWAT		_POSIX_PIPE_BUF

/* Maximum number of elements in an `iovec' array.  */
#define	_POSIX_UIO_MAXIOV	16

/* Maximum number of characters in a tty name.  */
#define	_POSIX_TTY_NAME_MAX	9

/* Number of timer for a process.  */
#define _POSIX_TIMER_MAX	32

/* Maximum length of login name.  */
#define	_POSIX_LOGIN_NAME_MAX	9

/* Maximum clock resolution in nanoseconds.  */
#define _POSIX_CLOCKRES_MIN	20000000


/* Get the implementation-specific values for the above.  */
#include <bits/local_lim.h>


#ifndef	SSIZE_MAX
# define SSIZE_MAX	INT_MAX
#endif


/* This value is a guaranteed minimum maximum.
   The current maximum can be got from `sysconf'.  */

#ifndef	NGROUPS_MAX
# define NGROUPS_MAX	_POSIX_NGROUPS_MAX
#endif

#endif	/* bits/posix1_lim.h  */
