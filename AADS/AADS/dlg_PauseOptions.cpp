//=========================================================================
//
//  dlg_pauseoptions.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_globals.hpp"
#include "FrontEnd.hpp"

#include "GameMgr\GameMgr.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_font.hpp"
#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_tabbed_dialog.hpp"

#include "dlg_pauseoptions.hpp"

#include "data\ui\ui_strings.h"

//=========================================================================
//  Score Dialog
//=========================================================================

enum controls
{
    IDC_INVERT_Y_LOOK,
    IDC_INVERT_Y_VEHICLE,
    IDC_3RD_PERSON_VEHICLE,
    IDC_VIBRATION_FUNCTION
};

ui_manager::control_tem PauseOptionsControls[] =
{
//    { "frame",    "frame",   22-16,  41-37, 468, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_INVERT_Y_LOOK,        IDS_INVERT_Y_LOOK,      "check",   6,   4, 230, 24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_INVERT_Y_VEHICLE,     IDS_INVERT_Y_VEHICLE,   "check",   6,  36, 230, 24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_3RD_PERSON_VEHICLE,   IDS_3RD_PERSON_VEHICLE, "check",   6,  68, 230, 24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_VIBRATION_FUNCTION,   IDS_VIBRATION_FUNCTION, "check",   6, 100, 230, 24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem PauseOptionsDialog =
{
    IDS_NULL,
    2, 4,
    sizeof(PauseOptionsControls)/sizeof(ui_manager::control_tem),
    &PauseOptionsControls[0],
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

void dlg_pauseoptions_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "pauseoptions", &PauseOptionsDialog, &dlg_pauseoptions_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_pauseoptions_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_pauseoptions* pDialog = new dlg_pauseoptions;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_pauseoptions
//=========================================================================

dlg_pauseoptions::dlg_pauseoptions( void )
{
}

//=========================================================================

dlg_pauseoptions::~dlg_pauseoptions( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_pauseoptions::Create( s32                        UserID,
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

    // Setup Data
    for( s32 i=0 ; i<4 ; i++ )
    {
        m_Children[i]->SetLabelFlags( ui_font::h_left|ui_font::v_center );
    }

    // Return success code
    return Success;
}

//=========================================================================

void dlg_pauseoptions::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_pauseoptions::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_pauseoptions::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // If at the top of the dialog then a move up will move back to the tabbed dialog
    if( (Code == ui_manager::NAV_UP) && (m_NavY == 0) )
    {
        if( m_pParent )
        {
            ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
        }
    }
    else
    {
        // Call the navigation function
        ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
    }
}

//=========================================================================

void dlg_pauseoptions::OnPadSelect( ui_win* pWin )
{
    (void)pWin;

    // Select on favorite combo jumps to weapons controls
/*
    if( pWin == (ui_win*)m_pFavorite )
    {
        // Activate Weapon selection controls
        GotoControl( m_pArmor );
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }
*/
}

//=========================================================================

void dlg_pauseoptions::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Go back to Tabbed Dialog parent
    if( m_pParent )
    {
        ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
    }
}

//=========================================================================

void dlg_pauseoptions::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

//    fegl.AudioSurround = m_pSurround->GetSelected();
//    audio_SetSurround(fegl.AudioSurround);
}

//=========================================================================
