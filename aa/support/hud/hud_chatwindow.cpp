//=========================================================================
//
//  hud_chatwindow.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "demo1/Globals.hpp"

#include "GameMgr/GameMgr.hpp"
#include "ObjectMgr/ObjectMgr.hpp"

#include "hud_manager.hpp"
#include "ui/ui_manager.hpp"
#include "ui/ui_font.hpp"

#include "demo1\fe_colors.hpp"

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

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

//=========================================================================
//  ClearChat
//=========================================================================

void hud_manager::ClearChat( user* pUser )
{
    for( s32 i=0; i<NumChatLines; i++ )
    {
        pUser->ChatLines[i].Timer = 0.0f;
    }

    pUser->ChatGlassY    = -ChatGlassRealHeight;
    pUser->ChatIndex     = 0;
    pUser->ChatLinesUsed = 0;
}

//=========================================================================
//  RenderChatWindow
//=========================================================================

void hud_manager::RenderChat( user* pUser, const irect& wr ) const
{
    s32 X  = ((wr.l + wr.r) / 2) - ChatGlassWidth + ChatGlassOffsetX;
    s32 Y  = wr.t + (s32)pUser->ChatGlassY;
    s32 TX = X;

    #ifdef TARGET_PS2
    
    gsreg_Begin();
    gsreg_SetScissor( wr.l, wr.t, wr.r, wr.b );
    eng_PushGSContext( 1 );
    gsreg_SetScissor( wr.l, wr.t, wr.r, wr.b );
    eng_PopGSContext();
    gsreg_End();

    // Render the chat window glass
    irect LRect( X, Y, X + ChatGlassRealWidth, Y + ChatGlassHeight );
    RenderBitmap( HUD_BMP_CHAT_GLASS, LRect, ChatGlassCol );
    
    X += (ChatGlassWidth * 2) - 1;
    irect RRect( X, Y, X - ChatGlassRealWidth, Y + ChatGlassHeight );
    RenderBitmap( HUD_BMP_CHAT_GLASS, RRect, ChatGlassCol );

    // Get the dimensions of the entire screen
    irect Screen( 0, 0, 0, 0 );
    s32 XRes, YRes;    
    eng_GetRes( XRes, YRes );
    Screen.SetWidth ( XRes );
    Screen.SetHeight( YRes );

    gsreg_Begin();
    gsreg_SetScissor( Screen.l, Screen.t, Screen.r, Screen.b );
    eng_PushGSContext( 1 );
    gsreg_SetScissor( Screen.l, Screen.t, Screen.r, Screen.b );
    eng_PopGSContext();
    gsreg_End();
    
    #endif

    irect Rect( TX, wr.t, TX, wr.t );
    Rect.SetWidth ( (ChatGlassWidth * 2) - (ChatTextOffsetX * 2) );
    Rect.SetHeight( (ChatTextHeight * NumChatLines) - ChatTextOffsetY );
    Rect.Translate( ChatTextOffsetX, ChatTextOffsetY );
    
    // Split screen is limited to only 2 chat lines
    s32 NumLines = (m_Users.GetCount() == 1) ? NumChatLines : NumChatLines2P;

    // Render all chat lines
    for( s32 i=0; i<NumLines; i++ )
    {
        s32 Index = pUser->ChatIndex - i;
        
        if( Index < 0 )
            Index += NumLines;
        
        chat_line& ChatLine = pUser->ChatLines[ Index ];

        if( ChatLine.Timer > 0.0f )
        {
            // Compute timed fade value
            f32 T = 1.0f;
            
            if( ChatLine.Timer <= ChatFadeTime )
                T = ChatLine.Timer / ChatFadeTime;
            
            u8   A     = (u8)(255.0f * T);
            s32  Flags =  ui_font::v_top | ui_font::h_left;
            
            ChatOutLineColor.A = A;
            
            RenderTextOutline( Rect, Flags, ChatOutLineColor, ChatLine.Text );
            RenderText       ( Rect, Flags,                A, ChatLine.Text );

            Rect.Translate( 0, ChatTextHeight );
        }
    }
}

//=========================================================================

void hud_manager::AddChat( s32 PlayerIndex, const xwchar* pString )
{
    ASSERT( pString );

    user* pUser = GetUserFromPlayerID( PlayerIndex );

    if( pUser == NULL )
        return;

    s32 NumLines = (m_Users.GetCount() == 1) ? NumChatLines : NumChatLines2P;
    
    pUser->ChatIndex++;
    
    if( pUser->ChatIndex == NumLines )
        pUser->ChatIndex  = 0;

    chat_line& ChatLine = pUser->ChatLines[ pUser->ChatIndex ];

    // If all lines are full then dont delay the chat text
    if( pUser->ChatLinesUsed == NumLines )
        ChatLine.Timer =  ChatLineTime;
    else
        ChatLine.Timer = -ChatDelayTime;

    x_wstrncpy( ChatLine.Text, pString, 127 );
    ChatLine.Text[127] = '\0';
}

//=========================================================================

void hud_manager::UpdateChat( user* pUser, f32 DeltaTime )
{
    pUser->ChatLinesUsed = 0;
    
    s32 NumLines = (m_Users.GetCount() == 1) ? NumChatLines : NumChatLines2P;
    
    for( s32 i=0; i<NumLines; i++ )
    {
        chat_line& ChatLine = pUser->ChatLines[i];
        
        if( ChatLine.Timer < 0.0f )
        {
            // Delay the chat text appearing so glass has time to slide down
            ChatLine.Timer += DeltaTime;
            
            if( ChatLine.Timer >= 0.0f )
                ChatLine.Timer = ChatLineTime;
            
            pUser->ChatLinesUsed++;
        }
        else
        {
            ChatLine.Timer -= DeltaTime;
        
            if( ChatLine.Timer < 0.0f )
                ChatLine.Timer = 0.0f;
            else
                pUser->ChatLinesUsed++;
        }
    }

    s32 StartY = (pUser->ChatLinesUsed > 0) ? -ChatGlassRealHeight + ChatGlassOffsetY : -ChatGlassRealHeight;
    f32 DestY  = (f32)StartY + (pUser->ChatLinesUsed * ChatTextHeight);
    f32 Speed  = (DestY - pUser->ChatGlassY) / ChatGlassTime;
    
    pUser->ChatGlassY += Speed * DeltaTime;
    
    if( Speed > 0.0f )
    {
        if( pUser->ChatGlassY > DestY )
            pUser->ChatGlassY = DestY;
    }
    else
    {
        if( Speed < 0.0f )
        {
            if( pUser->ChatGlassY < DestY )
                pUser->ChatGlassY = DestY;
        }
    }
}

//=========================================================================

void hud_manager::RenderPopups( user* pUser, const irect& wr ) const
{
    s32     i;
    vector2 Center = wr.GetCenter();

    for( i = 0; i < 3; i++ )
    {
        if( pUser->Popup[i].Timer <= 0.0f )
            continue;

        s32 X = (s32)Center.X + PopupOffsetX[i];
        s32 Y = (s32)Center.Y + PopupOffsetY[i];
        s32 A = 255;

        if( pUser->Popup[i].Timer < PopupFadeout[i] )
        {
            A = (s32)(255.0f * (pUser->Popup[i].Timer / PopupFadeout[i]));
        }
        
        irect  Rect( X, Y, X, Y );
        s32    Flags = ui_font::v_top | ui_font::h_center;
        xcolor Color( 0, 0, 0, A );

        RenderTextOutline( Rect, Flags, Color, pUser->Popup[i].Text );
        RenderText       ( Rect, Flags,     A, pUser->Popup[i].Text );
    }
}

//=========================================================================

void hud_manager::Popup( s32 Region, s32 PlayerIndex, const xwchar* pString )
{
    ASSERT( pString );
    ASSERT( IN_RANGE( 0, Region, 2 ) );

    user* pUser = GetUserFromPlayerID( PlayerIndex );

    if( pUser == NULL )
        return;

    pUser->Popup[Region].Timer = PopupTimeout[Region];

    if( Region < 2 )    // Not Exchange region.
        pUser->Popup[Region].Timer += (x_wstrlen(pString) * 0.1f);

    x_wstrncpy( pUser->Popup[Region].Text, pString, 127 );
    pUser->Popup[Region].Text[127] = '\0';
}

//=========================================================================

void hud_manager::ClearPopup( s32 Region, s32 PlayerIndex )
{
    ASSERT( IN_RANGE( 0, Region, 2 ) );

    user* pUser = GetUserFromPlayerID( PlayerIndex );

    if( pUser == NULL )
        return;

    if( pUser->Popup[Region].Timer > PopupFadeout[Region] )
        pUser->Popup[Region].Timer = PopupFadeout[Region];
}

//=========================================================================

void hud_manager::UpdatePopups( user* pUser, f32 DeltaTime )
{
    pUser->Popup[0].Timer -= DeltaTime;
    pUser->Popup[1].Timer -= DeltaTime;
    pUser->Popup[2].Timer -= DeltaTime;

    if( pUser->Popup[0].Timer < 0.0f )   pUser->Popup[0].Timer = 0.0f;
    if( pUser->Popup[1].Timer < 0.0f )   pUser->Popup[1].Timer = 0.0f;
    if( pUser->Popup[2].Timer < 0.0f )   pUser->Popup[2].Timer = 0.0f;
}

//=========================================================================
