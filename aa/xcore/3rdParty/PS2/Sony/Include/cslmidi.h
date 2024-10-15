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
 *                            cslmidi.h
 *                Component Sound Library MIDI Stream
 *
 *     Version   Date          Design     Log
 *  --------------------------------------------------------------------
 *     0.60      Oct.12.1999   katayama   first checked in.
 */
#ifndef _cslmidi_h_
#define _cslmidi_h_
typedef struct {
	u_int	buffsize;	// include Header size
	u_int	validsize;	// valid data size
	u_char	data[0];	// data max is data[buffsize]
} sceCslMidiStream;
/**** MIDI Special Messages ****/
#define sceCslMidiHsStatusEx1		0xf9
#define sceCslMidiHsStatusEx2		0xfd
#define sceCslMidiHsCmdExpression	0x00
#define sceCslMidiHsCmdPanpot		0x01
#define sceCslMidiHsCmdPitchBend	0x02
#define sceCslMidiHsCmdNoteCtrl		0x10
#define sceCslMidiHsCtrlAllKey		(1<<4)
#define sceCslMidiHsKIdAll			0x7f
#endif //!_cslmidi_h_
