/* Calls to enable and disable swapping on specified locations.  Linux version.
   Copyright (C) 1996, 1998, 1999 Free Software Foundation, Inc.
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

#ifndef _SYS_SWAP_H

#define _SYS_SWAP_H	1
#include <features.h>

/* The swap priority is encoded as:
   (prio << SWAP_FLAG_PRIO_SHIFT) & SWAP_FLAG_PRIO_MASK
*/
#define	SWAP_FLAG_PREFER	0x8000	/* Set if swap priority is specified. */
#define	SWAP_FLAG_PRIO_MASK	0x7fff
#define	SWAP_FLAG_PRIO_SHIFT	0

__BEGIN_DECLS

/* Make the block special device PATH available to the system for swapping.
   This call is restricted to the super-user.  */
extern int swapon (__const char *__path, int __flags) __THROW;

/* Stop using block special device PATH for swapping.  */
extern int swapoff (__const char *__path) __THROW;

__END_DECLS

#endif /* _SYS_SWAP_H */
