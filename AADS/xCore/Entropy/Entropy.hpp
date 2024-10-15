//==============================================================================
//  
//  Entropy.hpp
//
//==============================================================================

#ifndef ENTROPY_HPP
#define ENTROPY_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "e_View.hpp"
#include "e_Draw.hpp"
#include "e_Text.hpp"
#include "e_VRAM.hpp"
#include "e_Input.hpp"
#include "e_ScratchMem.hpp"

#include "x_files.hpp"

#ifdef TARGET_PC
#include "D3DEngine\d3deng_Private.hpp"
#endif

#ifdef TARGET_PS2
#include "PS2\ps2_Misc.hpp"
#endif

#ifdef TARGET_LINUX
#include "Linux\Linux_Misc.hpp"
#endif

//==============================================================================
//  DEFINES
//==============================================================================

#define ENG_MAX_VIEWS   8

//==============================================================================
//  FUNCTIONS
//==============================================================================

void            eng_Init                ( void );
void            eng_Kill                ( void );

void            eng_GetRes              ( s32& XRes, s32& YRes );
void            eng_SetBackColor        ( xcolor Color );

void            eng_MaximizeViewport    ( view& View );

void            eng_SetView             ( const view& View, s32 ViewPaletteID = 0 );
void            eng_ActivateView        ( s32 ViewPaletteID );
void            eng_DeactivateView      ( s32 ViewPaletteID );

u32             eng_GetActiveViewMask   ( void );
const view*     eng_GetView             ( s32 ViewPaletteID = 0 );

s32             eng_GetNActiveViews     ( void );
const view*     eng_GetActiveView       ( s32 ActiveViewListID );

void            eng_ScreenShot          ( const char* pFileName = NULL, s32 Size = 1 );
xbool           eng_ScreenShotActive    ( void );
s32             eng_ScreenShotSize      ( void );
s32             eng_ScreenShotX         ( void );
s32             eng_ScreenShotY         ( void );

xbool           eng_Begin               ( const char* pTaskName=NULL );
void            eng_End                 ( void );
xbool           eng_InBeginEnd          ( void );

void            eng_PageFlip            ( void );
void            eng_Sync                ( void );

void            eng_SetViewport         ( const view& View );

f32             eng_GetFPS              ( void );
void            eng_PrintStats          ( void );

f32             eng_GetTVRefreshRate    ( void ) ;



//==============================================================================
#endif // ENTROPY_HPP
//==============================================================================
