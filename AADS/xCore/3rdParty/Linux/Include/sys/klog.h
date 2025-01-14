/* Copyright (C) 1996, 1999 Free Software Foundation, Inc.
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

#ifndef	_SYS_KLOG_H

#define	_SYS_KLOG_H	1
#include <features.h>

__BEGIN_DECLS

/* Control the kernel's logging facility.  This corresponds exactly to
   the kernel's syslog system call, but that name is easily confused
   with the user-level syslog facility, which is something completely
   different.  */
extern int klogctl (int __type, char *__bufp, int __len) __THROW;

__END_DECLS

#endif /* _SYS_KLOG_H */
