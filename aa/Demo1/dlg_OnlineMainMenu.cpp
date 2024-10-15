//=========================================================================
//
//  dlg_main_menu.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_globals.hpp"

#include "AudioMgr\audio.hpp"
#include "LabelSets\Tribes2Types.hpp"

#include "ui\ui_font.hpp"
#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"

#include "dlg_OnlineMainMenu.hpp"
#include "titles.hpp"

#include "serverman.hpp"

#include "data\ui\ui_strings.h"

#include "Demo1\DemoVersion.hpp"

//=========================================================================
//  Switches
//=========================================================================

//#define INCLUDE_SOUND_TEST

//=========================================================================
//  Main Menu Dialog
//=========================================================================

enum controls
{
    IDC_ONE_PLAYER,
    IDC_TWO_PLAYER,
    IDC_BACK,
};


ui_manager::control_tem OnlineMainMenuControls[] =
{
    { IDC_ONE_PLAYER,           IDS_ONE_PLAYER,         "button",  128, 156, 224, 24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_TWO_PLAYER,           IDS_TWO_PLAYER,         "button",  128, 184, 224, 24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_BACK,                 IDS_BACK,               "button",  128, 212, 224, 24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem OnlineMainMenuDialog =
{
    IDS_ONLINE,
    1, 9,
    sizeof(OnlineMainMenuControls)/sizeof(ui_manager::control_tem),
    &OnlineMainMenuControls[0],
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

void dlg_online_main_menu_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "online main menu", &OnlineMainMenuDialog, &dlg_online_main_menu_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_online_main_menu_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_online_main_menu* pDialog = new dlg_online_main_menu;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_main_menu
//=========================================================================

dlg_online_main_menu::dlg_online_main_menu( void )
{
}

//=========================================================================

dlg_online_main_menu::~dlg_online_main_menu( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_online_main_menu::Create( s32                        UserID,
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

    m_pOneOnline  = (ui_button*)FindChildByID( IDC_ONE_PLAYER );
    m_pTwoOnline  = (ui_button*)FindChildByID( IDC_TWO_PLAYER );
    m_pBack       = (ui_button*)FindChildByID( IDC_BACK );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_online_main_menu::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_online_main_menu::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );

    irect r;
    s32 Width, Height;
    eng_GetRes(Width, Height);

    r.Set( 0, m_Position.b-16-12, Width, m_Position.b-16 );
    m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_BLACK, 
                            "(C)2002 Sierra Entertainment, Inc.\n"
                            "Tribes is a trademark of Sierra Entertainment, Inc." );
    r.Translate( -1, -1 );
    m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_WHITE, 
                            "(C)2002 Sierra Entertainment, Inc.\n"
                            "Tribes is a trademark of Sierra Entertainment, Inc." );
}

//=========================================================================

void dlg_online_main_menu::OnPadSelect( ui_win* pWin )
{
    // Single player online
    if( pWin == (ui_win*)m_pOneOnline )
    {
        tgl.NRequestedPlayers = 1;
        fegl.nLocalPlayers    = 1;
        fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
        fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
        fegl.iWarriorSetup    = 0;

        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }

    // Two player online
    if( pWin == (ui_win*)m_pTwoOnline )
    {
        tgl.NRequestedPlayers = 2;
        fegl.nLocalPlayers    = 2;
        fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
        fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
        fegl.iWarriorSetup    = 0;

        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }
    // Check for BACK button
    if( pWin == (ui_win*)m_pBack )
    {
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
    }
}

//=========================================================================

void dlg_online_main_menu::OnPadBack( ui_win* pWin )
{
    (void)pWin;
    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
}

//=========================================================================