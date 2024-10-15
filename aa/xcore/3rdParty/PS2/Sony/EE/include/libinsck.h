/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.4.3
 */
/*
 *                      Emotion Engine Library
 *                          Version 1.00
 *                           Shift-JIS
 *
 *      Copyright (C) 2001 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       <libinsck - libinsck.h>
 *                      <libinsck general header>
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           May,17,2001     komaki      first version
 */

#ifndef	_LIBINSCK_H_
#define	_LIBINSCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libinsck/netdb.h>
#include <libinsck/sys/socket.h>
#include <libinsck/netinet/in.h>
#include <libinsck/netinet/tcp.h>
#include <libinsck/arpa/inet.h>

int *__sceInsockErrnoLoc(void);
int *__sceInsockHErrnoLoc(void);

#define	sceInsockErrno		(*__sceInsockErrnoLoc())
#define	sceInsockHErrno		(*__sceInsockHErrnoLoc())

int sceInsockSetSifMBindRpcValue(u_int buffersize, u_int stacksize,
	int priority);

int sceInsockSetRecvTimeout(int s, int ms);

int sceInsockSetSendTimeout(int s, int ms);

int sceInsockAbort(int s, int flags);

int sceInsockTerminate(int thread_id);

#ifdef __cplusplus
}
#endif

#endif	/* _LIBINSCK_H_ */

