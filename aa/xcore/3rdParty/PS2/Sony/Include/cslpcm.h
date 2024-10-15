/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.4
 */
/*
 *                     I/O Processor Library
 *                          Version 0.60
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                            cslpcm.h
 *                Component Sound Library PCM Stream
 *
 *     Version   Date          Design     Log
 *  --------------------------------------------------------------------
 *     0.60      Oct.12.1999   katayama   first checked in.
 */
#ifndef _cslpcm_h_
#define _cslpcm_h_
typedef struct {
	u_int	pcmbuf_size;		// real pcm buffer size, NOT include header size
	u_int	validsize;
	void	*pcmbuf;
	u_int	pad;
} sceCslPcmStream;
#endif //!_cslpcm_h_
