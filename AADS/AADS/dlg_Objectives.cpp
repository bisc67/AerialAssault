//=========================================================================
//
//  dlg_objectives.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "FrontEnd.hpp"
#include "fe_globals.hpp"

#include "GameMgr\GameMgr.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_frame.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_check.hpp"

#include "dlg_objectives.hpp"
#include "fe_colors.hpp"
#include "dlg_util_rendercontroller.hpp"

#include "HUD\hud_manager.hpp"

#include "data\ui\ui_strings.h"

//=========================================================================
//  Objectives Dialog
//=========================================================================

enum controls
{
    IDC_FRAME
};

ui_manager::control_tem ObjectivesControls[] =
{
    { IDC_FRAME,    IDS_NULL,   "frame", 22-16,  41-37, 468, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem ObjectivesDialog =
{
    IDS_OBJECTIVES_TITLE,
    1, 1,
    sizeof(ObjectivesControls)/sizeof(ui_manager::control_tem),
    &ObjectivesControls[0],
    0
};

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

void dlg_objectives_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "objectives", &ObjectivesDialog, &dlg_objectives_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_objectives_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_objectives* pDialog = new dlg_objectives;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_objectives
//=========================================================================

dlg_objectives::dlg_objectives( void )
{
}

//=========================================================================

dlg_objectives::~dlg_objectives( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_objectives::Create( s32                        UserID,
                              ui_manager*                pManager,
                              ui_manager::dialog_tem*    pDialogTem,
                              const irect&               Position,
                              ui_win*                    pParent,
                              s32                        Flags,
                              void*                      pUserData )
{
    xbool   Success = FALSE;

    (void)pUserData;

    ASSERT( pManager );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_pFrame = (ui_frame*)FindChildByID( IDC_FRAME );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_objectives::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_objectives::Render( s32 ox, s32 oy )
{
    // Exit if not visible
    if( !(m_Flags & WF_VISIBLE) )
        return;

    ui_dialog::Render( ox, oy );

    irect Rect = m_pFrame->GetPosition();
    Rect.Deflate( 4, 4 );
    Rect.Translate( 8, 8 );
    LocalToScreen( Rect );
    const irect& br = m_pManager->GetUserBounds( m_UserID );
    Rect.Translate( br.l, br.t );

    // HACK: get the current objective text
    const xwchar* pString = tgl.pHUDManager->GetObjective();

    if( pString )
        m_pManager->RenderText( 0, Rect, ui_font::v_top | ui_font::h_left, HUDCOL_TEXT_WHITE, pString );
}

//=========================================================================

void dlg_objectives::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // Call the navigation function
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_objectives::OnPadSelect( ui_win* pWin )
{
    (void)pWin;
}

//=========================================================================

void dlg_objectives::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Go back to Tabbed Dialog parent
    if( m_pParent )
    {
        ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
}

//=========================================================================

void dlg_objectives::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================
