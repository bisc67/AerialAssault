/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.5
 */
/* 
 *                      I/O Processor Library
 *                          Version 1.02
 *                           Shift-JIS
 *
 *      Copyright (C) 1995-2001 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         inet - in.h
 *            Internet TCP/UDP port and endian conversion
 *
 * $Id: in.h,v 1.5.2.1 2002/02/19 10:31:42 xokano Exp $
 */

#if !defined(_IN_H)
#define _IN_H

#define IPADR_BROAD		0xffffffffL	/* 255.255.255.255 */
#define IPADR_ANY		0x00000000L	/* 0.0.0.0 */

#define PORT_ANY		0
#define PORT_ECHO		7
#define PORT_DISCARD		9
#define PORT_DAYTIME		13
#define PORT_CHARGEN		19
#define PORT_FTP_DATA		20
#define PORT_FTP		21
#define PORT_TELNET		23
#define PORT_DOMAIN		53
#define PORT_BOOTPS		67
#define PORT_BOOTPC		68
#define PORT_TFTP		69
#define PORT_USER		5000
#define PORT_MAX		65535

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN

#define ntohl(v)	(v)
#define ntohs(v)	(v)
#define htonl(v)	(v)
#define htons(v)	(v)

#else

#define ntohs(sv)       (u_int)({			\
	u_int _v = (sv);				\
	((_v << 8) & 0xff00) | ((_v >> 8) & 0x00ff);	\
})  

#define ntohl(lv)       (u_int)({			\
	u_int _v = (lv);				\
	(_v << 24) | ((_v & 0xff00) << 8)		\
		| ((_v >> 8) & 0xff00) | (_v >> 24);	\
})

#define htons(sv)	ntohs(sv)
#define htonl(lv)	ntohl(lv)

#endif

#define put2byte(p, v)	({ 		\
	u_char *_p = (u_char *)(p); 	\
	u_short _v = (u_short)(v); 	\
	*(_p + 0) = _v >>  8; 		\
	*(_p + 1) = _v >>  0; 		\
	_p + 2; 			\
})

#define put4byte(p, v)	({ 		\
	u_char *_p = (u_char *)(p);	\
	u_int _v = (u_int)(v);		\
	*(_p + 0) = _v >> 24;		\
	*(_p + 1) = _v >> 16;		\
	*(_p + 2) = _v >>  8;		\
	*(_p + 3) = _v >>  0;		\
	_p + 4;				\
})

#define get2byte(p)	({		\
	u_char *_p = (u_char *)(p);	\
	(*(_p + 0) <<  8)		\
		| (*(_p + 1) <<  0);	\
})

#define get4byte(p)	({		\
	u_char *_p = (u_char *)(p);	\
	(*(_p + 0) << 24)		\
		| (*(_p + 1) << 16)	\
		| (*(_p + 2) <<  8)	\
		| (*(_p + 3) <<  0);	\
})

#define get4u(p)	({		\
	u_char *_p = (u_char *)(p);	\
	(*(_p + 0) << 0)		\
		| (*(_p + 1) <<  8)	\
		| (*(_p + 2) << 16)	\
		| (*(_p + 3) << 24);	\
})

#define put4u(p, v)	({ 		\
	u_char *_p = (u_char *)(p);	\
	u_int _v = (u_int)(v);		\
	*(_p + 0) = _v >>  0;		\
	*(_p + 1) = _v >>  8;		\
	*(_p + 2) = _v >> 16;		\
	*(_p + 3) = _v >> 24;		\
	_p + 4;				\
})

#define get4h(p)	({		\
	u_short *_p = (u_short *)(p);	\
	(*(_p + 0) << 0)		\
		| (*(_p + 1) << 16);	\
})

#define put4h(p, v)	({ 		\
	u_short *_p = (u_short *)(p);	\
	u_int _v = (u_int)(v);		\
	*(_p + 0) = _v >>  0;		\
	*(_p + 1) = _v >> 16;		\
	_p + 2;				\
})

#if defined(__cplusplus)
}
#endif

#endif	/* _IN_H */
