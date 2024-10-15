//=========================================================================
//
//  dlg_controls.cpp
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

#include "dlg_controls.hpp"
#include "fe_colors.hpp"
#include "dlg_util_rendercontroller.hpp"

#include "data\ui\ui_strings.h"

//=========================================================================
//  Controls Dialog
//=========================================================================

enum controls
{
    IDC_FRAME
};

ui_manager::control_tem ControlsControls[] =
{
    { IDC_FRAME,    IDS_NULL,   "frame", 22-16,  41-37, 468, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem ControlsDialog =
{
    IDS_CONTROLS_TITLE,
    1, 4,
    sizeof(ControlsControls)/sizeof(ui_manager::control_tem),
    &ControlsControls[0],
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

void dlg_controls_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "controls", &ControlsDialog, &dlg_controls_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_controls_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_controls* pDialog = new dlg_controls;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_controls
//=========================================================================

dlg_controls::dlg_controls( void )
{
}

//=========================================================================

dlg_controls::~dlg_controls( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_controls::Create( s32                        UserID,
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

void dlg_controls::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_controls::Render( s32 ox, s32 oy )
{
    // Exit if not visible
    if( !(m_Flags & WF_VISIBLE) )
        return;

    ui_dialog::Render( ox, oy );

    // Get Warrior Setup pointer
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );
    s32 WarriorID = pPlayer->GetWarriorID();
    ASSERT( (WarriorID == 0) || (WarriorID == 1) || (WarriorID == 2) );
    warrior_setup* pWarriorSetup = &fegl.WarriorSetups[WarriorID];

    // Get Bounding rectangle for controller
    irect Rect = m_pFrame->GetPosition();
    Rect.Deflate( 4, 4 );
    Rect.Translate( 0, 10 );
    LocalToScreen( Rect );
    const irect& br = m_pManager->GetUserBounds( m_UserID );
    Rect.Translate( br.l, br.t );

    RenderController( Rect, pWarriorSetup );
}

//=========================================================================

void dlg_controls::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // Call the navigation function
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_controls::OnPadSelect( ui_win* pWin )
{
    (void)pWin;
}

//=========================================================================

void dlg_controls::OnPadBack( ui_win* pWin )
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

void dlg_controls::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================
