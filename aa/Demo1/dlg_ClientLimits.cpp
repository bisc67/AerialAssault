//=========================================================================
//
//  dlg_clientlimits.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_font.hpp"

#include "dlg_Message.hpp"

#include "fe_colors.hpp"
#include "dlg_clientlimits.hpp"

#include "data\ui\ui_strings.h"
#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"
#include "specialversion.hpp"

//==============================================================================
// ClientLimits Dialog
//==============================================================================

extern ui_manager::dialog_tem MessageDialog;

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Registration function
//=========================================================================

void dlg_clientlimits_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "clientlimits", &MessageDialog, &dlg_clientlimits_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_clientlimits_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_clientlimits* pDialog = new dlg_clientlimits;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_clientlimits
//=========================================================================

dlg_clientlimits::dlg_clientlimits( void )
{
}

//=========================================================================

dlg_clientlimits::~dlg_clientlimits( void )
{
    Destroy();
}

//=========================================================================

const char* Str1[] =
{
#if defined(PAL_RELEASE)
    "64K DSL/ISDN",
#else
    "Analog modem",
#endif
    "Cable modem",
    "256K DSL",
    "384K or faster DSL, or T1"
};

const char* Str2[] =
{
    "2",
    "6",
    "12",
    "15"
};

s32 g_CJ1 = 80;
s32 g_CJ2 = 16;
s32 g_CJ3 = 32;
xcolor g_CJ4(255,255,255,64);
s32 g_CJ5 = 40;

void dlg_clientlimits::Render( s32 ox, s32 oy )
{
    s32 i;

    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        // Fade out background
//        const irect& ru = m_pManager->GetUserBounds( m_UserID );
//        m_pManager->RenderRect( ru, xcolor(0,0,0,128), FALSE );

        // Get window rectangle
        irect   r;
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        irect   rb = r;
        rb.Deflate( 1, 1 );

        // Render frame
        if( m_Flags & WF_BORDER )
        {
            // Render background color
            if( m_BackgroundColor.A > 0 )
            {
                m_pManager->RenderRect( rb, m_BackgroundColor, FALSE );
            }

            // Render Title Bar Gradient
            rb.SetHeight( 40 );
            xcolor c1 = FECOL_TITLE1;
            xcolor c2 = FECOL_TITLE2;
            m_pManager->RenderGouraudRect( rb, c1, c1, c2, c2, FALSE );

            // Render the Frame
            m_pManager->RenderElement( m_iElement, r, 0 );

            // Render Title
            rb.Deflate( 16, 1 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_BLACK, m_Label );
            rb.Translate( -1, -1 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_WHITE, m_Label );
        }

        // Render Message
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        r.Deflate( g_CJ2, 52 );
        r.SetHeight( 15 );

        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_top, m_MessageColor, "If you want to host a game, please read\npages 12 to 14 of the instruction manual." );
        r.Translate( 0, g_CJ5 );

        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_top, m_MessageColor, "We recommend these settings for\noptimal game performance:" );
        r.Translate( 0, g_CJ5 );

        irect rl = r;
        irect rr = r;
        rr.l = r.r - g_CJ1;

        m_pManager->RenderText( 0, rl, ui_font::h_left|ui_font::v_center, m_MessageColor, "Connection" );
        m_pManager->RenderText( 0, rr, ui_font::h_left|ui_font::v_center, m_MessageColor, "Max Clients" );

        draw_Begin( DRAW_LINES, DRAW_2D|DRAW_USE_ALPHA|DRAW_NO_ZBUFFER );
        draw_Color( g_CJ4 );
        r.l -= 4;
        r.r += 4;
        draw_Vertex( vector3( (f32)rl.l-4, (f32)rr.b+1, 0.0f) );
        draw_Vertex( vector3( (f32)rr.r+4, (f32)rr.b+1, 0.0f) );
        draw_End();


        rl.Translate( 0, 18 );
        rr.Translate( 0, 18 );

        for( i=0 ; i<4 ; i++ )
        {
            m_pManager->RenderText( 0, rl, ui_font::h_left|ui_font::v_center, m_MessageColor, Str1[i] );
            m_pManager->RenderText( 0, rr, ui_font::h_center|ui_font::v_center, m_MessageColor, Str2[i] );
            rl.Translate( 0, 15 );
            rr.Translate( 0, 15 );
        }

/*
        rl.Translate( 0, g_CJ3 );
        rr.Translate( 0, g_CJ3 );
        r.t = rl.t;
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_top, m_MessageColor, "If you want to host a game, please read\npages 12 to 14 of the manual." );
*/

        // Render children
        for( i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}

//=========================================================================

//void dlg_clientlimits::OnPadBack           ( ui_win* pWin )
//{
//    eng_ScreenShot();
//}

//=========================================================================
