/* Interface to GNU libc specific functions for version information.
   Copyright (C) 1998, 1999 Free Software Foundation, Inc.
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

#ifndef _GNU_LIBC_VERSION_H
#define	_GNU_LIBC_VERSION_H	1

#include <features.h>

__BEGIN_DECLS

/* Return string describing release status of currently running GNU libc.  */
extern const char *__gnu_get_libc_release (void) __THROW;
extern const char *gnu_get_libc_release (void) __THROW;

/* Return string describing version of currently running GNU libc.  */
extern const char *__gnu_get_libc_version (void) __THROW;
extern const char *gnu_get_libc_version (void) __THROW;

__END_DECLS

#endif	/* gnu/libc-version.h */
