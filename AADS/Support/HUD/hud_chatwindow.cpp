//=========================================================================
//
//  hud_chatwindow.cpp
//
//=========================================================================
/*
#include "entropy.hpp"
#include "AADS/Globals.hpp"

#include "GameMgr/GameMgr.hpp"
#include "ObjectMgr/ObjectMgr.hpp"

#include "ui/ui_manager.hpp"
#include "ui/ui_font.hpp"

#include "demo1\fe_colors.hpp"
*/
#include "hud_manager.hpp"
#include "telnetconsole/telnetmgr.hpp"

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================
/*
const s32 ChatTextHeight      =  13;
const f32 ChatLineTime        =   5.0f;
const f32 ChatFadeTime        =   0.5f;
const f32 ChatDelayTime       =   0.3f;
const f32 ChatGlassTime       =   0.1f;
const s32 ChatGlassWidth      = 170;
const s32 ChatGlassHeight     = 128;
const s32 ChatGlassRealWidth  = 256;
const s32 ChatGlassRealHeight =  73;
const s32 ChatGlassOffsetX    =   3;
const s32 ChatGlassOffsetY    =  26;
const s32 ChatTextOffsetX     =  16;
const s32 ChatTextOffsetY     =  16;

const f32 PopupTimeout[3] = { 3.0f, 3.0f, 0.1f };
const f32 PopupFadeout[3] = { 1.0f, 1.0f, 0.0f };
const s32 PopupOffsetX[3] = {    0,    0,    0 };
const s32 PopupOffsetY[3] = {  -52,   52,  -68 };

xcolor ChatOutLineColor( XCOLOR_BLACK );
xcolor ChatGlassCol    ( 255, 255, 255, 70 );
*/
//=========================================================================
//  ClearChat
//=========================================================================

void hud_manager::ClearChat( user* pUser )
{
}

//=========================================================================
//  RenderChatWindow
//=========================================================================

void hud_manager::AddChat( s32 PlayerIndex, const xwchar* pString )
{
}

//=========================================================================

void hud_manager::Popup( s32 Region, s32 PlayerIndex, const xwchar* pString )
{
}

//=========================================================================

void hud_manager::ClearPopup( s32 Region, s32 PlayerIndex )
{
}

//=========================================================================
