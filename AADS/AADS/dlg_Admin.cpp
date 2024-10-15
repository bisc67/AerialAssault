//=========================================================================
//
//  dlg_admin.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "FrontEnd.hpp"

#include "GameMgr\GameMgr.hpp"
#include "serverman.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_dlg_list.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_slider.hpp"

#include "dlg_message.hpp"
#include "dlg_admin.hpp"
#include "sm_common.hpp"

#include "objects\bot\botobject.hpp"

#include "data\ui\ui_strings.h"
#include "fe_colors.hpp"
#include "fe_globals.hpp"
#include "StringMgr\StringMgr.hpp"

#include "Demo1/SpecialVersion.hpp"


extern bot_object::bot_debug g_BotDebug;

//=========================================================================

extern server_manager* pSvrMgr;

//=========================================================================
//  Admin Dialog
//=========================================================================

enum controls
{
    IDC_TARGET_LOCK,
    IDC_NEXT_MAP,
    IDC_CHANGE_MAP,
    IDC_TEAMCHANGE_PLAYER,
    IDC_KICK_PLAYER,
    IDC_EXIT_GAME,

    IDC_ENABLE_BOTS,
    IDC_SL_BOT_AI,
    IDC_SL_MIN_PLAYERS,
    IDC_SL_MAX_PLAYERS,
    IDC_SL_TEAM_DAMAGE,
    IDC_SL_VOTE_PASS,
    IDC_TX_BOT_AI,
    IDC_TX_MIN_PLAYERS,
    IDC_TX_MAX_PLAYERS,
    IDC_TX_TEAM_DAMAGE,
    IDC_TX_VOTE_PASS,

    IDC_FRAME
};

ui_manager::control_tem AdminControls[] =
{
    { IDC_TARGET_LOCK,          IDS_TARGET_LOCK,        "check",   13,  10, 223,  24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_NEXT_MAP,             IDS_NEXT_MAP,           "button",  13,  37, 223,  24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CHANGE_MAP,           IDS_CHANGE_MAP,         "button",  13,  64, 223,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_TEAMCHANGE_PLAYER,    IDS_TEAMCHANGE_PLAYER,  "button",  13,  91, 223,  24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_KICK_PLAYER,          IDS_KICK_PLAYER,        "button",  13, 118, 223,  24, 0, 4, 1, 1, ui_win::WF_VISIBLE },
    { IDC_EXIT_GAME,            IDS_SHUTDOWN_SERVER,    "button",  13, 145, 223,  24, 0, 5, 1, 1, ui_win::WF_VISIBLE },

    { IDC_ENABLE_BOTS,          IDS_ENABLE_BOTS,        "check",  244,  10, 223,  24, 1, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SL_BOT_AI,            IDS_NULL,               "slider", 403,  37,  64,  24, 1, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SL_MIN_PLAYERS,       IDS_NULL,               "slider", 403,  64,  64,  24, 1, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SL_MAX_PLAYERS,       IDS_NULL,               "slider", 403,  91,  64,  24, 1, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SL_TEAM_DAMAGE,       IDS_NULL,               "slider", 403, 118,  64,  24, 1, 4, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SL_VOTE_PASS,         IDS_NULL,               "slider", 403, 145,  64,  24, 1, 5, 1, 1, ui_win::WF_VISIBLE },

    { IDC_TX_BOT_AI,            IDS_AI_LEVEL,           "text",   244,  37, 100,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_TX_MIN_PLAYERS,       IDS_NUM_BOTS,           "text",   244,  64, 100,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_TX_MAX_PLAYERS,       IDS_MAX_PLAYERS,        "text",   244,  91, 100,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_TX_TEAM_DAMAGE,       IDS_TEAM_DAMAGE,        "text",   244, 118, 100,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_TX_VOTE_PASS,         IDS_VOTE_PASS,          "text",   244, 145, 100,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE },

    { IDC_FRAME,                IDS_NULL,               "frame",    6,   4, 468, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem AdminDialog =
{
    IDS_NULL,
    2, 6,
    sizeof(AdminControls)/sizeof(ui_manager::control_tem),
    &AdminControls[0],
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

s32 g_AdminCount = 0;

//=========================================================================
//  Registration function
//=========================================================================

void dlg_admin_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "admin", &AdminDialog, &dlg_admin_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_admin_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_admin* pDialog = new dlg_admin;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_admin
//=========================================================================

dlg_admin::dlg_admin( void )
{
    g_AdminCount++;
}

//=========================================================================

dlg_admin::~dlg_admin( void )
{
    Destroy();
    g_AdminCount--;
}

//=========================================================================

xbool dlg_admin::Create( s32                        UserID,
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

    m_pTargetLock       = (ui_check *)FindChildByID( IDC_TARGET_LOCK );
    m_pNextMap          = (ui_button*)FindChildByID( IDC_NEXT_MAP );
    m_pChangeMap        = (ui_button*)FindChildByID( IDC_CHANGE_MAP );
    m_pTeamchangePlayer = (ui_button*)FindChildByID( IDC_TEAMCHANGE_PLAYER );
    m_pKickPlayer       = (ui_button*)FindChildByID( IDC_KICK_PLAYER );
    m_pShutdownServer   = (ui_button*)FindChildByID( IDC_EXIT_GAME );

    m_pEnableBots       = (ui_check *)FindChildByID( IDC_ENABLE_BOTS );
    m_pSLBotAI          = (ui_slider*)FindChildByID( IDC_SL_BOT_AI );
    m_pSLMinPlayers     = (ui_slider*)FindChildByID( IDC_SL_MIN_PLAYERS );
    m_pSLMaxPlayers     = (ui_slider*)FindChildByID( IDC_SL_MAX_PLAYERS );
    m_pSLTeamDamage     = (ui_slider*)FindChildByID( IDC_SL_TEAM_DAMAGE );
    m_pSLVotePass       = (ui_slider*)FindChildByID( IDC_SL_VOTE_PASS );
    m_pTXBotAI          = (ui_text  *)FindChildByID( IDC_TX_BOT_AI );
    m_pTXMinPlayers     = (ui_text  *)FindChildByID( IDC_TX_MIN_PLAYERS );
    m_pTXMaxPlayers     = (ui_text  *)FindChildByID( IDC_TX_MAX_PLAYERS );
    m_pTXTeamDamage     = (ui_text  *)FindChildByID( IDC_TX_TEAM_DAMAGE );
    m_pTXVotePass       = (ui_text  *)FindChildByID( IDC_TX_VOTE_PASS );

    m_pTXBotAI     ->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTXMinPlayers->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTXMaxPlayers->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTXTeamDamage->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTXVotePass  ->SetLabelFlags( ui_font::v_center|ui_font::h_left );

    m_Updating = TRUE;

    if( !fegl.GameModeOnline )
    {
        m_pShutdownServer->SetLabel( StringMgr( "ui", IDS_EXIT_GAME ) );
    }

    m_pEnableBots->SetSelected( fegl.ServerSettings.BotsEnabled );

    m_pSLBotAI     ->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );
    m_pSLMinPlayers->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );
    m_pTXBotAI     ->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );
    m_pTXMinPlayers->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );

    m_pSLBotAI->SetRange( 1, 10 );
    m_pSLBotAI->SetStep( 1, 1 );
    m_pSLBotAI->SetValue( fegl.ServerSettings.uiBotsDifficulty );

    if( (GameMgr.GetGameType() == GAME_DM) ||
        (GameMgr.GetGameType() == GAME_HUNTER) )
    {
        m_pSLMinPlayers->SetRange( 1, MIN(12,fegl.ServerSettings.MaxPlayers) );
    }
    else
    {
        m_pSLMinPlayers->SetRange( 1, MIN(6,(fegl.ServerSettings.MaxPlayers/2)) );
    }

    m_pSLMinPlayers->SetStep( 1, 1 );
    m_pSLMinPlayers->SetValue( fegl.ServerSettings.BotsNum );
    m_pSLMinPlayers->SetParametric();

#ifdef DEDICATED_SERVER
    m_pSLMaxPlayers->SetRange( fegl.nLocalPlayers, 32 );
#else
    m_pSLMaxPlayers->SetRange( fegl.nLocalPlayers, 16 );
#endif
    m_pSLMaxPlayers->SetStep( 1 );
    m_pSLMaxPlayers->SetValue( fegl.ServerSettings.MaxPlayers );

    // For team based games, there should be an even number of players.
    s32 MaxPlayers = fegl.ServerSettings.MaxPlayers;
    if( (GameMgr.GetGameType() == GAME_TDM) ||
        (GameMgr.GetGameType() == GAME_CTF) ||
        (GameMgr.GetGameType() == GAME_CNH) )
    {
        if( MaxPlayers & 0x01 )
            MaxPlayers += 1;
        m_pSLMaxPlayers->SetValue( MaxPlayers );
#ifdef DEDICATED_SERVER
	    m_pSLMaxPlayers->SetRange( 2, 32 );
#else
        m_pSLMaxPlayers->SetRange( 2, 16 );
#endif
        m_pSLMaxPlayers->SetStep( 2 );
    }

    m_pSLTeamDamage->SetRange( 0, 100 );
    m_pSLTeamDamage->SetStep( 5, 5 );
    m_pSLTeamDamage->SetValue( fegl.ServerSettings.uiTeamDamage );

    m_pSLVotePass->SetRange( 10, 100 );
    m_pSLVotePass->SetStep ( 5, 5 );
    m_pSLVotePass->SetValue( fegl.ServerSettings.VotePass );

    m_pTargetLock->SetSelected( fegl.ServerSettings.TargetLockEnabled );

    // If not online then disable some stuff
    if( fegl.State != FE_STATE_ONLINE )
    {
        m_pKickPlayer->SetFlag( WF_DISABLED, 1 );
    }

    m_iChangeMap        = -1;
    m_iTeamchangePlayer = -1;
    m_iKickPlayer       = -1;
    m_DoShutdownServer  = FALSE;
    m_DoNextMap         = FALSE;

    // Disable change team if not a team based game
    if( (!GameMgr.IsTeamBasedGame()) || (GameMgr.GetCampaignMission() != -1) )
        m_pTeamchangePlayer->SetFlag( WF_DISABLED, TRUE );

    m_Updating = FALSE;

    #ifdef DEMO_DISK_HARNESS
    m_pNextMap  ->SetFlag( WF_DISABLED, 1 );
    m_pChangeMap->SetFlag( WF_DISABLED, 1 );
    #endif

    // Return success code
    return Success;
}

//=========================================================================

void dlg_admin::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_admin::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_admin::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    ui_dialog::OnUpdate( pWin, DeltaTime );

    xbool DialogsEnded = FALSE;

    // Teamchange the player
    if( m_iTeamchangePlayer != -1 )
    {
        GameMgr.ChangeTeam( m_iTeamchangePlayer );
        m_iTeamchangePlayer = -1;
    }

    // Kick the player
    if( m_iKickPlayer != -1 )
    {
        GameMgr.KickPlayer( m_iKickPlayer );
        m_iKickPlayer = -1;
    }

    // Change the map
    if( m_iChangeMap != -1 )
    {
        s32 iMap = m_iChangeMap;
        m_iChangeMap = -1;
        DialogsEnded = TRUE;
        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        GameMgr.ChangeMap( iMap );
    }

    // Next map requested
    if( m_DoNextMap == DLG_MESSAGE_YES )
    {
        m_DoNextMap = FALSE;

        DialogsEnded = TRUE;
        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );

        //AdvanceMission();
        if( tgl.ServerPresent )
        {
            g_SM.ServerMissionEnded = TRUE;
            GameMgr.EndGame();
        }
    }

    // Shutdown server if requested
    if( m_DoShutdownServer == DLG_MESSAGE_YES )
    {
        m_DoShutdownServer = FALSE;

        DialogsEnded = TRUE;
        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );

        tgl.Playing = FALSE;
        g_SM.ServerShutdown = TRUE;
        GameMgr.EndGame();
    }

    if( !DialogsEnded )
    {
        // Dynamically disable some buttons based on vote circumstances...
        ui_win* pActiveWin = m_pManager->GetWindowUnderCursor( m_UserID );

        // Disable the Kick Player button if there are no kickable players.
        xbool Flag = GetKickMask() && tgl.ServerPresent;
        m_pKickPlayer->SetFlag( WF_DISABLED, !Flag );
        if( (!Flag) && ((ui_win*)m_pKickPlayer == pActiveWin) )
            OnPadBack( this );

        // Update Text fields
        m_pTXBotAI     ->SetLabel( xwstring(xfs("AI LEVEL %d",       fegl.ServerSettings.uiBotsDifficulty)) );
        m_pTXMaxPlayers->SetLabel( xwstring(xfs("MAX PLAYERS %d",    fegl.ServerSettings.MaxPlayers      )) );
        m_pTXTeamDamage->SetLabel( xwstring(xfs("TEAM DAMAGE %d%%",  fegl.ServerSettings.uiTeamDamage    )) );
        m_pTXVotePass  ->SetLabel( xwstring(xfs("VOTE PASS %d%%",    fegl.ServerSettings.VotePass        )) );
        if( (GameMgr.GetGameType() == GAME_DM) ||
            (GameMgr.GetGameType() == GAME_HUNTER) )
        {
            m_pTXMinPlayers->SetLabel( xwstring(xfs("MIN PLAYERS %d", fegl.ServerSettings.BotsNum)) );
        }
        else
        {
            m_pTXMinPlayers->SetLabel( xwstring(xfs("MIN TEAMSIZE %d", fegl.ServerSettings.BotsNum)) );
        }
    }
}

//=========================================================================

void dlg_admin::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // If at the top of the dialog then a move up will move back to the tabbed dialog
    if( (Code == ui_manager::NAV_UP) && (m_NavY == 0) || 
        ( (Code == ui_manager::NAV_UP) && (m_pChangeMap->GetFlags() & WF_DISABLED) && (m_NavY == 2) ) )
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

void dlg_admin::OnPadSelect( ui_win* pWin )
{
    // Next Map
    if( pWin == (ui_win*)m_pNextMap )
    {
        dlg_message* pMD = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                                 "message",
                                                                 irect(0,0,250,120),
                                                                 NULL,
                                                                 WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
        pMD->Configure( StringMgr("ui", IDS_NEXT_MAP),
                        StringMgr("ui", IDS_YES),
                        StringMgr("ui", IDS_NO),
                        StringMgr("ui", IDS_ARE_YOU_SURE),
                        HUDCOL_TEXT_WHITE,
                        &m_DoNextMap );
    }

    // Change Map
    if( pWin == (ui_win*)m_pChangeMap )
    {
        // Throw up default config selection dialog
        irect r = GetPosition();
        LocalToScreen( r );
        r.t -= 14;
        r.b = r.t + 26+18*7;
        r.l += 64;
        r.r -= 64;
        ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
        ui_listbox* pList = pListDialog->GetListBox();
        pListDialog->SetResultPtr( &m_iChangeMap );

        // Add all the maps to the list
        xwstring    MissionName;
        for( s32 i=0 ; ; i++ )
        {
            if( fe_Missions[i].GameType == GAME_NONE )
                break;

            if( fegl.GameType == fe_Missions[i].GameType )
            {
                MissionName = StringMgr( "MissionName", fe_Missions[i].DisplayNameString );
                pList->AddItem( MissionName, i );
            }
        }

        pList->SetSelection( 0 );
    }

    // Teamchange Player
    if( (pWin == (ui_win*)m_pTeamchangePlayer) )
    {
        // Throw up default config selection dialog
        irect r = GetPosition();
        LocalToScreen( r );
        r.t -= 14;
        r.b = r.t + 26+18*7;
        r.l += 64;
        r.r -= 64;
        ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
        ui_listbox* pList = pListDialog->GetListBox();
        pListDialog->SetResultPtr( &m_iTeamchangePlayer );

        const game_score& Score = GameMgr.GetScore();

        for( s32 i = 0; i < 32; i++ )
        {
            if( Score.Player[i].IsInGame && (!Score.Player[i].IsBot) )
            {
                s32 Item = pList->AddItem( Score.Player[i].Name, i );
                pList->SetItemColor( Item, HUDCOL_TEXT_BLUE );
            }
        }

        if( pList->GetItemCount() > 0 )
            pList->SetSelection( 0 );
    }

    // Kick Player
    u32 KickMask = GetKickMask();
    if( (pWin == (ui_win*)m_pKickPlayer) && KickMask )
    {
        // Throw up default config selection dialog
        irect r = GetPosition();
        LocalToScreen( r );
        r.t -= 14;
        r.b = r.t + 26+18*7;
        r.l += 64;
        r.r -= 64;
        ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
        ui_listbox* pList = pListDialog->GetListBox();
        pListDialog->SetResultPtr( &m_iKickPlayer );

        const game_score& Score = GameMgr.GetScore();

        for( s32 i = 0; i < 32; i++ )
        {
            if( KickMask & (1 << i) )
            {
                s32 Item = pList->AddItem( Score.Player[i].Name, i );
                pList->SetItemColor( Item, HUDCOL_TEXT_BLUE );
            }
        }

        if( pList->GetItemCount() > 0 )
            pList->SetSelection( 0 );
    }

    // Shutdown Server
    if( pWin == (ui_win*)m_pShutdownServer )
    {
        dlg_message* pMD = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                                 "message",
                                                                 irect(0,0,250,120),
                                                                 NULL,
                                                                 WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
        pMD->Configure( StringMgr("ui", IDS_SHUTDOWN),
                        StringMgr("ui", IDS_YES),
                        StringMgr("ui", IDS_NO),
                        StringMgr("ui", IDS_ARE_YOU_SURE),
                        HUDCOL_TEXT_WHITE,
                        &m_DoShutdownServer );
    }
}

//=========================================================================

void dlg_admin::OnPadBack( ui_win* pWin )
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

void dlg_admin::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    if( m_Updating )
        return;

    // Target Lock
    if( pSender == (ui_win*)m_pTargetLock )
    {
        fegl.ServerSettings.TargetLockEnabled = m_pTargetLock->GetSelected();
    }

    // Enable Bots
    if( pSender == (ui_win*)m_pEnableBots )
    {
        fegl.ServerSettings.BotsEnabled = m_pEnableBots->GetSelected();
        GameMgr.SetPlayerLimits( fegl.ServerSettings.MaxPlayers,
                                 fegl.ServerSettings.BotsNum,
                                 fegl.ServerSettings.BotsEnabled );

        m_pSLBotAI     ->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );
        m_pSLMinPlayers->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );
        m_pTXBotAI     ->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );
        m_pTXMinPlayers->SetFlag( WF_DISABLED, !m_pEnableBots->GetSelected() );
    }

    // Bot AI
    if( pSender == (ui_win*)m_pSLBotAI )
    {
        fegl.ServerSettings.uiBotsDifficulty = m_pSLBotAI->GetValue();
        fegl.ServerSettings.BotsDifficulty = fegl.ServerSettings.uiBotsDifficulty / 10.0f;
    }

    // Num Bots
    if( pSender == (ui_win*)m_pSLMinPlayers )
    {
        fegl.ServerSettings.BotsNum = m_pSLMinPlayers->GetValue();
        GameMgr.SetPlayerLimits( fegl.ServerSettings.MaxPlayers,
                                 fegl.ServerSettings.BotsNum,
                                 fegl.ServerSettings.BotsEnabled );
    }

    // Max Players
    if( pSender == (ui_win*)m_pSLMaxPlayers )
    {
        fegl.ServerSettings.MaxPlayers = m_pSLMaxPlayers->GetValue();

        if( (GameMgr.GetGameType() == GAME_DM) ||
            (GameMgr.GetGameType() == GAME_HUNTER) )
        {
            m_pSLMinPlayers->SetRange( 1, MAX(1,MIN(12,fegl.ServerSettings.MaxPlayers)) );
        }
        else
        {
            m_pSLMinPlayers->SetRange( 1, MAX(1,MIN(6,(fegl.ServerSettings.MaxPlayers/2))) );
        }
        fegl.ServerSettings.BotsNum = m_pSLMinPlayers->GetValue();

        GameMgr.SetPlayerLimits( fegl.ServerSettings.MaxPlayers,
                                 fegl.ServerSettings.BotsNum,
                                 fegl.ServerSettings.BotsEnabled );
    }

    // Team Damage
    if( pSender == (ui_win*)m_pSLTeamDamage )
    {
        fegl.ServerSettings.uiTeamDamage = m_pSLTeamDamage->GetValue();
        fegl.ServerSettings.TeamDamage = fegl.ServerSettings.uiTeamDamage / 100.0f;
        GameMgr.SetTeamDamage( fegl.ServerSettings.TeamDamage );
    }

    // Vote Pass
    if( pSender == (ui_win*)m_pSLVotePass )
    {
        fegl.ServerSettings.VotePass = m_pSLVotePass->GetValue();
    }

    m_pSLTeamDamage     = (ui_slider*)FindChildByID( IDC_SL_TEAM_DAMAGE );
    m_pSLVotePass       = (ui_slider*)FindChildByID( IDC_SL_VOTE_PASS );

}

//=========================================================================

u32 dlg_admin::GetKickMask( void )
{
    const game_score& Score      = GameMgr.GetScore();
    player_object*    pPlayer    = (player_object*)m_pManager->GetUserData( m_UserID );
    s32               MyIndex    = pPlayer->GetPlayerIndex();
    u32               KickMask   = 0;

    for( s32 i=0; i<32; i++ )
    {
        // To be able to kick, the victim must be:
        //  - Not yourself
        //  - In the game
        //  - Not a bot
        //  - Not on machine 0 (the server)
        if( (i != MyIndex) &&
            (Score.Player[i].IsInGame) &&
            (Score.Player[i].IsBot == FALSE) &&
            (Score.Player[i].Machine != 0) )
        {
            KickMask |= (1 << i);
        }
    }

    return( KickMask );
}

//=========================================================================
