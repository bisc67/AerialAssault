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

#include "dlg_OptionsMainMenu.hpp"
#include "titles.hpp"
#include "dlg_loadsave.hpp"

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
    IDC_AUDIO,
    IDC_NETWORK,
    IDC_CREDITS,
    IDC_BACK,
};


ui_manager::control_tem OptionsMainMenuControls[] =
{
    { IDC_AUDIO,                IDS_AUDIO,              "button",  128, 156, 224, 24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_NETWORK,              IDS_NETWORK,            "button",  128, 184, 224, 24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CREDITS,              IDS_CREDITS,            "button",  128, 212, 224, 24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_BACK,                 IDS_BACK,               "button",  128, 240, 224, 24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
};



ui_manager::dialog_tem OptionsMainMenuDialog =
{
    IDS_OPTIONS,
    1, 9,
    sizeof(OptionsMainMenuControls)/sizeof(ui_manager::control_tem),
    &OptionsMainMenuControls[0],
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

void dlg_options_main_menu_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "options main menu", &OptionsMainMenuDialog, &dlg_options_main_menu_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_options_main_menu_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_options_main_menu* pDialog = new dlg_options_main_menu;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_main_menu
//=========================================================================

dlg_options_main_menu::dlg_options_main_menu( void )
{
}

//=========================================================================

dlg_options_main_menu::~dlg_options_main_menu( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_options_main_menu::Create( s32                        UserID,
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

    m_pAudio      = (ui_button*)FindChildByID( IDC_AUDIO );  
    m_pNetwork    = (ui_button*)FindChildByID( IDC_NETWORK );  
    m_pBack       = (ui_button*)FindChildByID( IDC_BACK );
    m_pCredits    = (ui_button*)FindChildByID( IDC_CREDITS );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_options_main_menu::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_options_main_menu::Render( s32 ox, s32 oy )
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

void dlg_options_main_menu::OnPadSelect( ui_win* pWin )
{

    // Options
    if ( pWin==(ui_win*)m_pAudio)
    {
        fegl.State          = FE_STATE_GOTO_AUDIO_OPTIONS;
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }

    if ( pWin==(ui_win*)m_pNetwork)
    {
        fegl.State          = FE_STATE_GOTO_NETWORK_OPTIONS;
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }

    if (pWin == (ui_win*)m_pCredits)
    {
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
        title_Credits();
    }
    // Check for BACK button
    if( pWin == (ui_win*)m_pBack )
    {
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
		if (fegl.OptionsModified)
		{
			load_save_params options(0,(load_save_mode)(SAVE_OPTIONS|ASK_FIRST),NULL);
			m_pManager->OpenDialog( m_UserID, 
								  "optionsloadsave", 
								  irect(0,0,300,150), 
								  NULL, 
								  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
								  (void *)&options );
			fegl.OptionsModified = FALSE;
		}
    }    
}

//=========================================================================

void dlg_options_main_menu::OnPadBack( ui_win* pWin )
{
    (void)pWin;
    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
	if (fegl.OptionsModified)
	{
		load_save_params options(0,(load_save_mode)(SAVE_OPTIONS|ASK_FIRST),NULL);
		m_pManager->OpenDialog( m_UserID, 
							  "optionsloadsave", 
							  irect(0,0,300,150), 
							  NULL, 
							  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
							  (void *)&options );
		fegl.OptionsModified = FALSE;
	}
}

//=========================================================================