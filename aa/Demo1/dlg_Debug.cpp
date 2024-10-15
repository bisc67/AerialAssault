//=========================================================================
//
//  dlg_debug.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "FrontEnd.hpp"

#include "GameMgr\GameMgr.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "GameMgr\Campaign_Logic.hpp"

//#include "Objects\Player\PlayerLogic.cpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_check.hpp"

#include "dlg_debug.hpp"
#include "sm_common.hpp"

#include "objects\bot\botobject.hpp"

#include "data\ui\ui_strings.h"
#include "fe_Globals.hpp"

extern bot_object::bot_debug g_BotDebug;
extern xbool CTF_LOGIC_HUMANS_ON_SAME_TEAM;
extern xbool HUD_DebugInfo;
extern xbool HUD_HideHUD;
extern xbool UseAutoAimHelp;
extern xbool UseAutoAimHint;
extern xbool ManiacMode[2];
extern xbool g_bUnlimitedAmmo;

extern xbool g_FlickerIndoorTerrain;
extern xbool g_FlickerTerrainHoles;

extern xbool SHOW_BLENDING;
extern xbool PROJECTILE_BLENDING;

//=========================================================================
//  Debug Dialog
//=========================================================================

enum controls
{
    IDC_DEBUG_INFO,
    IDC_INVULNERABLE,
    IDC_NEXT_MISSION,
    IDC_EXIT_GAME,
    IDC_AUTOAIM_HINT,
    IDC_AUTOAIM_HELP,
    IDC_MANIAC_MODE,
    IDC_HIDE_HUD,
    IDC_CHANGE_TEAM,
    IDC_AUTO_WIN,
    IDC_AMMO,
    IDC_FLICKER_INDOOR_TERRAIN,
    IDC_FLICKER_TERRAIN_HOLE,
    IDC_DO_BLENDING,
    IDC_SHOW_BLENDING,
    IDC_FRAME
};

ui_manager::control_tem DebugControls[] =
{
    { IDC_DEBUG_INFO,   IDS_DEBUG_INFO,     "check",   10,  19, 146,  24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_INVULNERABLE, IDS_INVULNERABLE,   "check",   10,  51, 146,  24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_NEXT_MISSION, IDS_NEXT_MAP,       "button",  10,  83, 146,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_EXIT_GAME,    IDS_EXIT_GAME,      "button",  10, 115, 146,  24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_AUTO_WIN,     IDS_AUTO_WIN,       "button",  10, 147, 146,  24, 0, 4, 1, 1, ui_win::WF_VISIBLE },

    { IDC_AUTOAIM_HINT, IDS_AUTOAIM_HINT,   "check",  164,  19, 146,  24, 1, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_AUTOAIM_HELP, IDS_AUTOAIM_HELP,   "check",  164,  51, 146,  24, 1, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_MANIAC_MODE,  IDS_MANIAC_MODE,    "check",  164,  83, 146,  24, 1, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_HIDE_HUD,     IDS_HIDE_HUD,       "check",  164, 115, 146,  24, 1, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CHANGE_TEAM,  IDS_CHANGE_TEAM,    "button", 164, 147, 146,  24, 1, 4, 1, 1, ui_win::WF_VISIBLE },

    { IDC_AMMO,                     IDS_UNLIMITED_AMMO, "check",  318,  19, 146,  24, 2, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_FLICKER_INDOOR_TERRAIN,   IDS_NULL,           "check",  318,  51, 146,  24, 2, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_FLICKER_TERRAIN_HOLE,     IDS_NULL,           "check",  318,  83, 146,  24, 2, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DO_BLENDING,              IDS_NULL,           "check",  318, 115, 146,  24, 2, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SHOW_BLENDING,            IDS_NULL,           "check",  318, 147, 146,  24, 2, 4, 1, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME,        IDS_NULL,           "frame",    6,   4, 468, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem DebugDialog =
{
    IDS_NULL,
    3, 5,
    sizeof(DebugControls)/sizeof(ui_manager::control_tem),
    &DebugControls[0],
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

void dlg_debug_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "debug", &DebugDialog, &dlg_debug_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_debug_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_debug* pDialog = new dlg_debug;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_admin
//=========================================================================

dlg_debug::dlg_debug( void )
{
}

//=========================================================================

dlg_debug::~dlg_debug( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_debug::Create( s32                        UserID,
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

    m_pNextMap      = (ui_button*)FindChildByID( IDC_NEXT_MISSION );
    m_pLeaveGame    = (ui_button*)FindChildByID( IDC_EXIT_GAME );
    m_pDebugInfo    = (ui_check* )FindChildByID( IDC_DEBUG_INFO );
    m_pInvincible   = (ui_check* )FindChildByID( IDC_INVULNERABLE  );
    m_pAutoAimHint  = (ui_check* )FindChildByID( IDC_AUTOAIM_HINT );
    m_pAutoAimHelp  = (ui_check* )FindChildByID( IDC_AUTOAIM_HELP );
    m_pManiacMode   = (ui_check* )FindChildByID( IDC_MANIAC_MODE );
    m_pHideHUD      = (ui_check* )FindChildByID( IDC_HIDE_HUD );
    m_pChangeTeam   = (ui_button*)FindChildByID( IDC_CHANGE_TEAM );
    m_pAutoWin      = (ui_button*)FindChildByID( IDC_AUTO_WIN );
    m_pUnlimitedAmmo= (ui_check* )FindChildByID( IDC_AMMO );
    
    m_pFlickerIndoorTerrain = (ui_check*)FindChildByID( IDC_FLICKER_INDOOR_TERRAIN );
    m_pFlickerIndoorTerrain->SetSelected( g_FlickerIndoorTerrain );
    m_pFlickerIndoorTerrain->SetLabel( "Indoor Terrain" );

    m_pFlickerTerrainHole = (ui_check*)FindChildByID( IDC_FLICKER_TERRAIN_HOLE );
    m_pFlickerTerrainHole->SetSelected( g_FlickerTerrainHoles );
    m_pFlickerTerrainHole->SetLabel( "Terrain Hole " );
    
    m_pDebugInfo ->SetSelected( HUD_DebugInfo );
    m_pHideHUD   ->SetSelected( HUD_HideHUD );
    m_pInvincible->SetSelected( fegl.PlayerInvulnerable );
    m_pUnlimitedAmmo->SetSelected( g_bUnlimitedAmmo );

    s32 WarriorID = ((player_object*)m_pManager->GetUserData( m_UserID ))->GetWarriorID();
    if( WarriorID != -1 )
        m_pManiacMode->SetSelected( ManiacMode[WarriorID] );

    m_pAutoAimHint->SetSelected( UseAutoAimHint );
    m_pAutoAimHelp->SetSelected( UseAutoAimHelp );

    m_pDoBlending = (ui_check* )FindChildByID( IDC_DO_BLENDING );
    m_pDoBlending->SetSelected( PROJECTILE_BLENDING );
    m_pDoBlending->SetLabel( "Shot Blending" );

    m_pShowBlending = (ui_check* )FindChildByID( IDC_SHOW_BLENDING );
    m_pShowBlending->SetSelected( SHOW_BLENDING );
    m_pShowBlending->SetLabel( "Show Blending" );

    if( !tgl.ServerPresent )
        m_pNextMap->SetFlags( m_pNextMap->GetFlags() || ui_win::WF_DISABLED );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_debug::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_debug::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_debug::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // If at the top of the dialog then a move up will move back to the tabbed dialog
    if( (Code == ui_manager::NAV_UP) && (m_NavY == 0) )
    {
        if( m_pParent )
        {
            ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
            return;
        }
    }

    // Call the navigation function
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_debug::OnPadSelect( ui_win* pWin )
{
    (void)pWin;

    if( pWin == (ui_win*)m_pNextMap )
    {
        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );

        //AdvanceMission();
        if( tgl.ServerPresent )
        {
            g_SM.ServerMissionEnded = TRUE;
            GameMgr.EndGame();
        }
    }

    if( pWin == (ui_win*)m_pLeaveGame )
    {
        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );

        tgl.Playing = FALSE;
        
        if( tgl.ServerPresent )
            g_SM.ServerShutdown = TRUE;
        else
            g_SM.ClientQuit = TRUE;

        GameMgr.EndGame();
    }

    if( pWin == (ui_win*)m_pChangeTeam )
    {
        // Get Player Object & Change Team
        player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
        pPlayer->RequestChangeTeam();
    }

    if( pWin == (ui_win*) m_pAutoWin )
    {
        g_AutoWinMission = TRUE;
        tgl.HighestCampaignMission = 16;
    }
}

//=========================================================================

void dlg_debug::OnPadBack( ui_win* pWin )
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

void dlg_debug::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)Command;
    (void)pData;

    if( pSender == m_pDebugInfo )
    {
        HUD_DebugInfo= m_pDebugInfo->GetSelected();
    }

    else if( pSender == m_pHideHUD )
    {
        HUD_HideHUD = m_pHideHUD->GetSelected();
    }

    else if( pSender == m_pInvincible )
    {
        fegl.PlayerInvulnerable = m_pInvincible->GetSelected();
    }

    else if( pSender == m_pAutoAimHint )
    {
        UseAutoAimHint = m_pAutoAimHint->GetSelected();
    }
    else if( pSender == m_pAutoAimHelp )
    {
        UseAutoAimHelp = m_pAutoAimHelp->GetSelected();
    }
    else if( pSender == m_pManiacMode )
    {
        s32 WarriorID = ((player_object*)m_pManager->GetUserData( m_UserID ))->GetWarriorID();
        if( WarriorID != -1 )
            ManiacMode[WarriorID] = m_pManiacMode->GetSelected();
    }
    else if( pSender == m_pUnlimitedAmmo )
    {
        g_bUnlimitedAmmo = m_pUnlimitedAmmo->GetSelected();
    }
    else if( pSender == m_pFlickerIndoorTerrain )
    {
        g_FlickerIndoorTerrain = m_pFlickerIndoorTerrain->GetSelected();
    }
    else if( pSender == m_pFlickerTerrainHole )
    {
        g_FlickerTerrainHoles = m_pFlickerTerrainHole->GetSelected();
    }
    else if( pSender == m_pDoBlending )
    {
        PROJECTILE_BLENDING = m_pDoBlending->GetSelected();
    }
    else if( pSender == m_pShowBlending )
    {
        SHOW_BLENDING = m_pShowBlending->GetSelected();
    }
}

//=========================================================================
