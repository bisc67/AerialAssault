//=========================================================================
//
//  dlg_online.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_globals.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"
#include "serverman.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"

#include "dlg_Online.hpp"
#include "dlg_OnlineHost.hpp"
#include "dlg_clientlimits.hpp"
#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"
#include "StringMgr\StringMgr.hpp"

#include "SpecialVersion.hpp"

extern server_manager*  pSvrMgr;

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

void dlg_online_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "online", NULL, &dlg_online_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_online_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_online* pDialog = new dlg_online;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_online
//=========================================================================

dlg_online::dlg_online( void )
{
    m_pJoin = NULL;
    m_pHost = NULL;
}

//=========================================================================

dlg_online::~dlg_online( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_online::Create( s32                        UserID,
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

    xbool IsNetworked = (pSvrMgr->GetLocalAddr().IP != 0);
    
    if(fegl.State == FE_STATE_ONLINE)
        IsNetworked = 1;
    else
        IsNetworked = 0;

    // Set title regarding online / offline
    if( IsNetworked ) 
        SetLabel( "ONLINE" );
    else
	{
        SetLabel( "BOT MATCH" );
	}

    // Create Tabbed Dialog
    irect r( 4, 48, 512-32-4, 448-32-4 );
    m_pTabbed = (ui_tabbed_dialog*)m_pManager->OpenDialog( m_UserID, "ui_tabbed_dialog", r, this, ui_win::WF_VISIBLE|ui_win::WF_BORDER );
    ASSERT( m_pTabbed );
    m_pTabbed->SetTabWidth( 100 );

    // Create Child Dialogs
    r.Set( 4, 24, r.GetWidth()-8, r.GetHeight() );

    // Create the HOST tab.
    m_pHost = m_pManager->OpenDialog( m_UserID, "onlinehost", r, m_pTabbed, ui_win::WF_VISIBLE|ui_win::WF_TAB );
    ASSERT( m_pHost );

    // Take care of "online" tabs.
    if( IsNetworked )
    {
        // Create and add the JOIN tab.
        m_pJoin = m_pManager->OpenDialog( m_UserID, "onlinejoin", r, m_pTabbed, ui_win::WF_VISIBLE|ui_win::WF_TAB );
        ASSERT( m_pJoin );
        m_pTabbed->AddTab( "JOIN", m_pJoin );

        // Add the HOST tab.
        m_pTabbed->AddTab( "HOST", m_pHost );
    }
    else
    {
        // Add the HOST tab with the label "GAME" (since HOST doesn't make 
        // sense when offline).
        m_pTabbed->AddTab( "GAME", m_pHost );
    }

    m_pTabbed->ActivateTab( 0 );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_online::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_online::Render( s32 ox, s32 oy )
{
    xwstring Online;
    xwstring Players;

    if( fegl.nLocalPlayers == 1 )
        Players = xwstring("ONE PLAYER");
    else
        Players = xwstring("TWO PLAYER");

    if( fegl.State == FE_STATE_ONLINE )
        Online = xwstring(" ONLINE GAME");
    else
        Online = xwstring(" OFFLINE GAME");

    m_Label = Players + Online;

    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_online::OnPadSelect( ui_win* pWin )
{
    (void)pWin;
}

//=========================================================================

void dlg_online::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
}

//=========================================================================

void dlg_online::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    // Selected TAB changed
    if( pSender == m_pTabbed )
    {
        if( m_pTabbed->GetTabLabel( m_pTabbed->GetActiveTab() ) == xwstring("HOST") )
        {
            // Open warning dialog
            dlg_message* pMD = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                                     "clientlimits",
                                                                     irect(0,0,320,260),
                                                                     NULL,
                                                                     WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
            pMD->Configure( xwstring("ALERT!"),
                            NULL,
                            StringMgr("ui", IDS_CONTINUE),
                            xwstring(""),
                            HUDCOL_TEXT_WHITE,
                            NULL );
        }
    }
}

//=========================================================================
