/*
 * direct.h
 *
 * Functions for manipulating paths and directories (included from io.h)
 * plus functions for setting the current drive.
 *
 * This file is part of the Mingw32 package.
 *
 * Contributors:
 *  Created by Colin Peters <colin@bird.fu.is.saga-u.ac.jp>
 *
 *  THIS SOFTWARE IS NOT COPYRIGHTED
 *
 *  This source code is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This code is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAMED. This includes but is not limited to warranties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Revision: 1.2 $
 * $Author: khan $
 * $Date: 1999/01/04 23:21:12 $
 *
 */

#ifndef __STRICT_ANSI__

#ifndef	_DIRECT_H_
#define	_DIRECT_H_

/* All the headers include this file. */
#include <_mingw.h>

#define __need_wchar_t
#ifndef RC_INVOKED
#include <stddef.h>
#endif	/* Not RC_INVOKED */

#include <io.h>

#ifndef RC_INVOKED

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _DISKFREE_T_DEFINED
/* needed by _getdiskfree (also in dos.h) */
struct _diskfree_t {
	unsigned total_clusters;
	unsigned avail_clusters;
	unsigned sectors_per_cluster;
	unsigned bytes_per_sector;
};
#define _DISKFREE_T_DEFINED
#endif  

/*
 * You really shouldn't be using these. Use the Win32 API functions instead.
 * However, it does make it easier to port older code.
 */
int	_getdrive ();
unsigned long _getdrives();
int	_chdrive (int nDrive);
char*	_getdcwd (int nDrive, char* caBuffer, int nBufLen);
unsigned _getdiskfree (unsigned, struct _diskfree_t *);

#ifndef	_NO_OLDNAMES
# define diskfree_t _diskfree_t
#endif

#ifndef _WDIRECT_DEFINED

/* wide character versions. Also in wchar.h */

int _wchdir(const wchar_t*);
wchar_t* _wgetcwd(wchar_t*, int);
wchar_t* _wgetdcwd(int, wchar_t*, int);
int _wmkdir(const wchar_t*);
int _wrmdir(const wchar_t*);

#define _WDIRECT_DEFINED
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _DIRECT_H_ */

#endif	/* Not __STRICT_ANSI__ */

