/* Copyright (C) 1999 Free Software Foundation, Inc.
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

#ifndef _UNISTD_H
# error "Never include this file directly.  Use <unistd.h> instead"
#endif

#include <bits/wordsize.h>

/* This header should define the following symbols under the described
   situations.  A value `1' means that the model is always supported,
   `-1' means it is never supported.  Undefined means it cannot be
   statically decided.

   _XBS5_ILP32_OFF32	32bit int, long, pointers, and off_t type
   _XBS5_ILP32_OFFBIG	32bit int, long, and pointers and larger off_t type

   _XBS5_LP64_OFF32	64bit long and pointers and 32bit off_t type
   _XBS5_LPBIG_OFFBIG	64bit long and pointers and large off_t type
*/

#if __WORDSIZE == 64

/* We can never provide environments with 32-bit wide pointers.  */
# define _XBS5_ILP32_OFF32	-1
# define _XBS5_ILP32_OFFBIG	-1

/* By default we have 64-bit wide `long int', pointers and `off_t'.  */
# define _XBS5_LP64_OFF32	1
# define _XBS5_LPBIG_OFFBIG	1

#else /* __WORDSIZE == 32 */

/* By default we have 32-bit wide `int', `long int', pointers and `off_t'.  */
# define _XBS5_ILP32_OFF32	1

/* We optionally provide an environment with the above size but an 64-bit
   side `off_t'.  Therefore we don't define _XBS5_ILP32_OFFBIG.  */

/* We can never provide environments with 64-bit wide pointers.  */
# define _XBS5_LP64_OFF64	-1
# define _XBS5_LPBIG_OFFBIG	-1

#endif /* __WORDSIZE == 32 */
