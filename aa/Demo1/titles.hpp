#ifndef _TITLES_HPP
#define _TITLES_HPP

#include "savedgame.hpp"

#define NETWORK_OPTIONS		(1<<0)
#define AUDIO_OPTIONS		(1<<1)
#define CAMPAIGN_OPTIONS	(1<<2)
#define ALL_OPTIONS			(-1)

void title_InitialCheck     ( void );
void title_InevitableLogo   (xbool WaitForEnd);
void title_SierraLogo       (xbool WaitForEnd);
void title_IntroMovie       (void);
void title_Credits          (void);
void title_ClearLogo        (void);
void title_TribesLogo       (void);
void title_SetOptions       (saved_options *pSavedData,s32 SetMask);
void title_GetOptions       (saved_options *pSavedData,s32 GetMask);

void title_Init_Begin       (void);
void title_Init_Advance     (f32 DeltaTime);

#endif // _TITLES_HPP