// PlayStation 2 Demo Segment SDK Version 1.1

#ifndef _LIBSCEDEMO_H
#define _LIBSCEDEMO_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

// -- LANGUAGES --

// (NOTE: These constants are different to the SCE_xxxx_LANG used by
// libscf).

#define SCE_DEMO_LANGUAGE_ENGLISH     0
#define SCE_DEMO_LANGUAGE_FRENCH      1
#define SCE_DEMO_LANGUAGE_GERMAN      2
#define SCE_DEMO_LANGUAGE_SPANISH     3
#define SCE_DEMO_LANGUAGE_ITALIAN     4
#define SCE_DEMO_LANGUAGE_DUTCH       5
#define SCE_DEMO_LANGUAGE_PORTUGUESE  6
#define SCE_DEMO_LANGUAGE_JAPANESE    7

// -- VIDEO ASPECT RATIO --

#define SCE_DEMO_ASPECT_4_3     0
#define SCE_DEMO_ASPECT_16_9    1
#define SCE_DEMO_ASPECT_FULL    2

// -- PLAYABILITY MODE --

#define SCE_DEMO_PLAYMODE_PLAYABLE  0
#define SCE_DEMO_PLAYMODE_ATTRACT   1

// -- MEDIA TYPE

#define SCE_DEMO_MEDIATYPE_CD  0
#define SCE_DEMO_MEDIATYPE_DVD 1
 
// -- END REASON --

typedef enum
{
  SCE_DEMO_ENDREASON_ATTRACT_INTERRUPTED,
  SCE_DEMO_ENDREASON_ATTRACT_COMPLETE,
  SCE_DEMO_ENDREASON_PLAYABLE_INACTIVITY_TIMEOUT,
  SCE_DEMO_ENDREASON_PLAYABLE_GAMEPLAY_TIMEOUT,
  SCE_DEMO_ENDREASON_PLAYABLE_COMPLETE,
  SCE_DEMO_ENDREASON_PLAYABLE_QUIT
} sceDemoEndReason;


// -- PROTOTYPES --

void sceDemoStart(int            argc,
		   char           **argv,
		   unsigned short *language,
		   unsigned short *aspect,
		   unsigned short *playmode,
		   unsigned short *to_inactive,
		   unsigned short *to_gameplay,
		   unsigned short *mediatype,
		   unsigned short *masterVolumeScale,
		   unsigned short *dataDirectorySectorNumber
		   );

void sceDemoEnd(sceDemoEndReason why);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
#endif // _LIBSCEDEMO_H
