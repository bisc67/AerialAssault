//=========================================================================
//
//  dlg_online_host.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_Globals.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "Demo1/SpecialVersion.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_button.hpp"

#include "dlg_OnlineHost.hpp"
#include "dlg_loadsave.hpp"
#include "dlg_MissionLoad.hpp"

#include "data\ui\ui_strings.h"
#include "StringMgr\StringMgr.hpp"

//==============================================================================

extern server_manager* pSvrMgr;
extern s16 s_RenderCalls;
static xbool           s_Dirty = FALSE;

//==============================================================================
// Online Host Dialog
//==============================================================================

enum controls
{
    IDC_GAME_TYPE,
    IDC_MISSION,
    IDC_SERVER_NAME_TEXT,
    IDC_SERVER_NAME_EDIT,
    IDC_PASSWORD_TEXT,
    IDC_PASSWORD_EDIT,
    IDC_MAX_PLAYERS_TEXT,
    IDC_MAX_PLAYERS_SLIDER,
    IDC_MAX_CLIENTS_TEXT,
    IDC_MAX_CLIENTS_SLIDER,
    IDC_TEAM_DAMAGE_TEXT,
    IDC_TEAM_DAMAGE_SLIDER,
    IDC_TIME_LIMIT_TEXT,
    IDC_TIME_LIMIT_SLIDER,
    IDC_VOTE_PASS_TEXT,
    IDC_VOTE_PASS_SLIDER,
    IDC_ENABLE_TARGET_LOCK,
    IDC_ENABLE_BOTS,
    IDC_FRAME,
    IDC_NUM_BOTS_TEXT,
    IDC_NUM_BOTS_SLIDER,
    IDC_AI_LEVEL_TEXT,
    IDC_AI_LEVEL_SLIDER,
    IDC_BACK,
    IDC_HOST,
//    IDC_LOAD,
//    IDC_SAVE
};

ui_manager::control_tem OnlineHostControls[] =
{
    { IDC_GAME_TYPE,            IDS_GAME_TYPE,          "combo",     4,   8, 224,  19, 0,  0, 2, 1, ui_win::WF_VISIBLE },

    { IDC_MISSION,              IDS_NULL,               "listbox",   4,  36, 224, 266, 0,  1, 2,10, ui_win::WF_VISIBLE },

    { IDC_SERVER_NAME_TEXT,     IDS_NAME,               "text",    236,   4,  48,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_SERVER_NAME_EDIT,     IDS_VK_SERVER_NAME,     "edit",    284,   4, 176,  24, 2,  0, 2, 1, ui_win::WF_VISIBLE },

    { IDC_PASSWORD_TEXT,        IDS_PASSWORD,           "text",    236,  32,  48,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_PASSWORD_EDIT,        IDS_VK_SERVER_PASSWORD, "edit",    284,  32, 176,  24, 2,  1, 2, 1, ui_win::WF_VISIBLE },

    { IDC_VOTE_PASS_TEXT,       IDS_VOTE_PASS,          "text",    236,  60,  60,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_VOTE_PASS_SLIDER,     IDS_NULL,               "slider",  388,  60,  72,  24, 2,  2, 2, 1, ui_win::WF_VISIBLE },

    { IDC_MAX_PLAYERS_TEXT,     IDS_MAX_PLAYERS,        "text",    236,  86,  60,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_MAX_PLAYERS_SLIDER,   IDS_NULL,               "slider",  388,  86,  72,  24, 2,  3, 2, 1, ui_win::WF_VISIBLE },

    { IDC_MAX_CLIENTS_TEXT,     IDS_MAX_CLIENTS,        "text",    236, 112,  60,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_MAX_CLIENTS_SLIDER,   IDS_NULL,               "slider",  388, 112,  72,  24, 2,  4, 2, 1, ui_win::WF_VISIBLE },

    { IDC_TEAM_DAMAGE_TEXT,     IDS_TEAM_DAMAGE,        "text",    236, 138,  60,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_TEAM_DAMAGE_SLIDER,   IDS_NULL,               "slider",  388, 138,  72,  24, 2,  5, 2, 1, ui_win::WF_VISIBLE },

    { IDC_TIME_LIMIT_TEXT,      IDS_TIME_LIMIT,         "text",    236, 164,  60,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_TIME_LIMIT_SLIDER,    IDS_NULL,               "slider",  388, 164,  72,  24, 2,  6, 2, 1, ui_win::WF_VISIBLE },

    { IDC_ENABLE_TARGET_LOCK,   IDS_ENABLE_TARGET_LOCK, "check",   236, 190, 224,  24, 2,  7, 2, 1, ui_win::WF_VISIBLE },

    { IDC_ENABLE_BOTS,          IDS_ENABLE_BOTS,        "check",   236, 216, 224,  24, 2,  8, 2, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME,                IDS_NULL,               "frame",   236, 244, 224,  58, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    { IDC_NUM_BOTS_TEXT,        IDS_NUM_BOTS,           "text",    244, 250, 104,  22, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_NUM_BOTS_SLIDER,      IDS_NULL,               "slider",  382, 250,  72,  22, 2,  9, 2, 1, ui_win::WF_VISIBLE },

    { IDC_AI_LEVEL_TEXT,        IDS_AI_LEVEL,           "text",    244, 276, 104,  22, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_AI_LEVEL_SLIDER,      IDS_NULL,               "slider",  382, 276,  72,  22, 2, 10, 2, 1, ui_win::WF_VISIBLE },

    { IDC_BACK,                 IDS_BACK,               "button",    4, 308,  96,  24, 0, 11, 2, 1, ui_win::WF_VISIBLE },
//    { IDC_LOAD,                 IDS_LOAD,               "button",  124, 308,  96,  24, 1, 10, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_SAVE,                 IDS_SAVE,               "button",  244, 308,  96,  24, 2, 10, 1, 1, ui_win::WF_VISIBLE },

    { IDC_HOST,                 IDS_LAUNCH,             "button",  364, 308,  96,  24, 2, 11, 2, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem OnlineHostDialog =
{
    IDS_NULL,
    4, 12,
    sizeof(OnlineHostControls)/sizeof(ui_manager::control_tem),
    &OnlineHostControls[0],
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

void dlg_online_host_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "onlinehost", &OnlineHostDialog, &dlg_online_host_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_online_host_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_online_host* pDialog;

    pDialog = new dlg_online_host;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_online_host
//=========================================================================

dlg_online_host::dlg_online_host( void )
{
}

//=========================================================================

dlg_online_host::~dlg_online_host( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_online_host::Create( s32                        UserID,
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

    m_pGameType         = (ui_combo*)  FindChildByID( IDC_GAME_TYPE );
    m_pMissions         = (ui_listbox*)FindChildByID( IDC_MISSION );
    m_pTextName         = (ui_text*)   FindChildByID( IDC_SERVER_NAME_TEXT );
    m_pServerName       = (ui_edit*)   FindChildByID( IDC_SERVER_NAME_EDIT );
    m_pTextPass         = (ui_text*)   FindChildByID( IDC_PASSWORD_TEXT );
    m_pServerPassword   = (ui_edit*)   FindChildByID( IDC_PASSWORD_EDIT );
    m_pTextPlayers      = (ui_text*)   FindChildByID( IDC_MAX_PLAYERS_TEXT );
    m_pMaxPlayers       = (ui_slider*) FindChildByID( IDC_MAX_PLAYERS_SLIDER );
    m_pTextClients      = (ui_text*)   FindChildByID( IDC_MAX_CLIENTS_TEXT );
    m_pMaxClients       = (ui_slider*) FindChildByID( IDC_MAX_CLIENTS_SLIDER );
    m_pTeamDamage       = (ui_text*)   FindChildByID( IDC_TEAM_DAMAGE_TEXT );
    m_pTeamDamageSlider = (ui_slider*) FindChildByID( IDC_TEAM_DAMAGE_SLIDER );
    m_pTextTimeLimit    = (ui_text*)   FindChildByID( IDC_TIME_LIMIT_TEXT );
    m_pTimeLimitSlider  = (ui_slider*) FindChildByID( IDC_TIME_LIMIT_SLIDER );
    m_pTextVotePass     = (ui_text*)   FindChildByID( IDC_VOTE_PASS_TEXT );
    m_pVotePassSlider   = (ui_slider*) FindChildByID( IDC_VOTE_PASS_SLIDER );

    m_pEnableTargetLock = (ui_check*)  FindChildByID( IDC_ENABLE_TARGET_LOCK );

    m_pEnableBots       = (ui_check*)  FindChildByID( IDC_ENABLE_BOTS );
    m_pBack             = (ui_button*) FindChildByID( IDC_BACK );
//    m_pLoad             = (ui_button*) FindChildByID( IDC_LOAD );
//    m_pSave             = (ui_button*) FindChildByID( IDC_SAVE );
    m_pHost             = (ui_button*) FindChildByID( IDC_HOST );
    m_pNumBotsText      = (ui_text*)   FindChildByID( IDC_NUM_BOTS_TEXT );
    m_pNumBotsSlider    = (ui_slider*) FindChildByID( IDC_NUM_BOTS_SLIDER );
    m_pBotsAIText       = (ui_text*)   FindChildByID( IDC_AI_LEVEL_TEXT );
    m_pBotsAISlider     = (ui_slider*) FindChildByID( IDC_AI_LEVEL_SLIDER );

    m_DisableUpdate = 1;

    if( fegl.GotoGameState == FE_STATE_GOTO_OFFLINE )
        m_pHost->SetLabel( StringMgr( "ui", IDS_PLAY ) );


    if( fegl.State == FE_STATE_OFFLINE )
    {
        m_pVotePassSlider->SetFlag( WF_DISABLED, 1 );
        m_pServerName    ->SetFlag( WF_DISABLED, 1 );
        m_pServerPassword->SetFlag( WF_DISABLED, 1 );
        m_pTextVotePass  ->SetFlag( WF_DISABLED, 1 );
        m_pTextName      ->SetFlag( WF_DISABLED, 1 );
        m_pTextPass      ->SetFlag( WF_DISABLED, 1 );
        m_pMaxClients    ->SetFlag( WF_DISABLED, 1 );
        m_pTextClients   ->SetFlag( WF_DISABLED, 1 );

        fegl.ServerSettings.BotsEnabled = TRUE;
//        m_pGameType->SetNavPos( irect( 0, 2, 1, 1 ) );
//        m_pMissions->SetNavPos( irect( 0, 3, 1, 6 ) );

        // Fixup navgraph for offline use
        m_NavGraph[ 2 + 0*m_NavW ] = m_pGameType;
        m_NavGraph[ 2 + 1*m_NavW ] = m_pMaxPlayers;
        m_NavGraph[ 2 + 2*m_NavW ] = m_pMaxPlayers;
    }

	ReadOptions();

    m_pTextPass->SetLabel( xwstring("\026") );

    m_pTextName     ->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTextPass     ->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTextPlayers  ->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTextClients  ->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTeamDamage   ->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTextTimeLimit->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTextVotePass ->SetLabelFlags( ui_font::v_center|ui_font::h_left );

    m_pServerName->SetMaxCharacters( FE_MAX_SERVER_NAME-1 );
    m_pServerPassword->SetMaxCharacters( FE_MAX_ADMIN_PASS-1 );

    #ifdef DEMO_DISK_HARNESS
    m_pTextPass       ->SetFlag( WF_DISABLED, 1 );
    m_pTextTimeLimit  ->SetFlag( WF_DISABLED, 1 );
    m_pServerPassword ->SetFlag( WF_DISABLED, 1 );
    m_pTimeLimitSlider->SetFlag( WF_DISABLED, 1 );
    m_pTimeLimitSlider->SetValue( 10 );
    #endif

#ifdef DEDICATED_SERVER
    m_pMaxPlayers->SetRange( fegl.nLocalPlayers, 32 );
#else
    m_pMaxPlayers->SetRange( fegl.nLocalPlayers, 16 );
#endif

    #ifdef E3_SERVER // SPECIAL_VERSION
    m_pMaxPlayers->SetRange( fegl.nLocalPlayers, 12 );
    #endif

    m_pMaxPlayers->SetStep( 1, 1 );


#ifdef DEDICATED_SERVER
    m_pMaxClients->SetRange( 0, 31 );
#else
    m_pMaxClients->SetRange( 0, 15 );
#endif
    m_pMaxClients->SetStep( 1, 1 );
    m_pMaxClients->SetParametric();
    m_pMaxClients->SetRange( 0, MAX(1,m_pMaxPlayers->GetValue()-1) );
    m_pMaxClients->SetValue( fegl.ServerSettings.MaxClients );



    m_pTeamDamageSlider->SetRange( 0, 100 );
    m_pTeamDamageSlider->SetStep( 5, 5 );

    m_pTimeLimitSlider->SetRange( 10, 60 );
    m_pTimeLimitSlider->SetStep ( 5, 5 );

    m_pVotePassSlider->SetRange( 10, 100 );
    m_pVotePassSlider->SetStep ( 5, 5 );

    #ifdef E3_SERVER // SPECIAL_VERSION
    m_pNumBotsSlider->SetRange( 1,  8 );
    #else
    m_pNumBotsSlider->SetRange( 1, 12 );
    #endif
    m_pNumBotsSlider->SetStep( 1, 1 );
    m_pNumBotsSlider->SetParametric();

    m_pBotsAISlider ->SetRange( 1, 10 );
    m_pBotsAISlider ->SetStep( 1, 1 );

    m_pNumBotsText->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pBotsAIText ->SetLabelFlags( ui_font::v_center|ui_font::h_left );

    // SPECIAL_VERSION
    m_pGameType->AddItem( "CAPTURE THE FLAG", GAME_CTF    );
    m_pGameType->AddItem( "CAPTURE AND HOLD", GAME_CNH    );
    m_pGameType->AddItem( "DEATHMATCH",       GAME_DM     );
    m_pGameType->AddItem( "TEAM DEATHMATCH",  GAME_TDM    );
    m_pGameType->AddItem( "HUNTERS",          GAME_HUNTER );
    
    m_pGameType->SetSelection( fegl.ServerSettings.GameType );
    FillMissionsList();
    m_pMissions->SetSelection( fegl.ServerSettings.MapIndex );


    if( (m_pGameType->GetSelectedItemData() == GAME_DM) ||
        (m_pGameType->GetSelectedItemData() == GAME_HUNTER) )
    {
        s32 MinBots = 1;
        s32 MaxBots = MIN(m_pMaxPlayers->GetValue(),12);
        m_pNumBotsSlider->SetRange( MinBots, MaxBots );
        m_pNumBotsText  ->SetLabel( xwstring(xfs("MIN PLAYERS %d",    m_pNumBotsSlider   ->GetValue())) );
        m_pNumBotsSlider->SetValue( fegl.ServerSettings.BotsNum );
    }
    else
    {
        s32 MinBots = 1;
        s32 MaxBots = MIN((m_pMaxPlayers->GetValue()/2),6);
        m_pNumBotsSlider->SetRange( MinBots, MaxBots );
        m_pNumBotsText  ->SetLabel( xwstring(xfs("MIN TEAMSIZE %d",   m_pNumBotsSlider   ->GetValue())) );
        m_pNumBotsSlider->SetValue( fegl.ServerSettings.BotsNum );
    }


    m_WaitingOnSave  = FALSE;
    m_WaitingOnClose = FALSE;

    m_DisableUpdate = 0;
//	WriteOptions();

    UpdateControls();

    // Return success code
    return Success;
}

//=========================================================================

void dlg_online_host::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_online_host::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_online_host::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // If at top of dialog then go back to parent tabbed dialog
    if( (Code == ui_manager::NAV_UP) && (m_NavY == 0) || 
        ((Code == ui_manager::NAV_UP) && (m_NavY == 2) && (m_pServerName->GetFlags() & WF_DISABLED)) )
    {
        if( m_pParent )
        {
            ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        }
    }
    else
    {
        ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
    }
}

//=========================================================================

void dlg_online_host::OnPadSelect( ui_win* pWin )
{
    // Check for BACK button
    if( pWin == (ui_win*)m_pBack )
    {
        // Close dialog and step back
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }

/*
    // Check for LOAD button
    else if( pWin == (ui_win*)m_pLoad )
    {
        load_save_params options(0,(load_save_mode)(LOAD_CAMPAIGN_OPTIONS),this);
        m_pManager->OpenDialog( m_UserID, 
                              "warriorloadsave", 
                              irect(0,0,300,264), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
		ReadOptions();
        UpdateControls();
        s_Dirty = FALSE;
    }

    // Check for SAVE button
    else if( pWin == (ui_win*)m_pSave )
    {
		WriteOptions();
        load_save_params options(0,(load_save_mode)(SAVE_CAMPAIGN_OPTIONS),this);
        m_pManager->OpenDialog( m_UserID, 
                              "warriorloadsave", 
                              irect(0,0,300,264), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options);
        
        s_Dirty = FALSE;
    }
*/
    // Check for HOST button
    else if( pWin == (ui_win*)m_pHost )
    {
        fegl.GameHost = TRUE;
		WriteOptions();
		if( s_Dirty )
        {
            m_WaitingOnSave = TRUE;
			WriteOptions();
            load_save_params options(0,(load_save_mode)(SAVE_CAMPAIGN_OPTIONS|ASK_FIRST),this);
		    m_pManager->OpenDialog( m_UserID, 
							      "optionsloadsave", 
							      irect(0,0,300,150), 
							      NULL, 
							      ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
							      (void *)&options );
            s_Dirty = FALSE;
        }
        else
        {
            UpdateControls();

            // Mission Selection within Range?
            if( (m_pMissions->GetSelection() >= 0) && (m_pMissions->GetSelection() < m_pMissions->GetItemCount()) )
            {
                if( fegl.State == FE_STATE_OFFLINE )
                {
                    // Disconnect the player.
#ifndef TARGET_PS2_CLIENT
					fegl.DeliberateDisconnect = TRUE;
					net_ActivateConfig(FALSE);
#endif
                }

                // Get Pointer to mission definition
                fegl.pMissionDef = (mission_def*)m_pMissions->GetSelectedItemData();
                ASSERT( fegl.pMissionDef );
                fegl.pNextMissionDef = NULL;

                // Set Name and Game Type
                x_strcpy( fegl.MissionName, fegl.pMissionDef->Folder );
                fegl.GameType = fegl.pMissionDef->GameType;

                pSvrMgr->SetName( m_pServerName->GetLabel() );
                tgl.UserInfo.MyName[0] = '\0';
                x_wstrcpy( tgl.UserInfo.Server, pSvrMgr->GetName() );

                tgl.UserInfo.IsServer    = TRUE;
                tgl.UserInfo.MyAddr      = pSvrMgr->GetLocalAddr();
                tgl.UserInfo.UsingConfig = TRUE;

                audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );

                // Setup Mission Loading Dialog
                ui_dialog* pDialog = tgl.pUIManager->OpenDialog( fegl.UIUserID, "mission load", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
                (void)pDialog;
                ASSERT( pDialog );
            }

        }

    }
/*
    else if( pWin == (ui_win*)m_pAdvanced )
    {
        dlg_advanced_host_options* pAdvanced = (dlg_advanced_host_options*)m_pManager->OpenDialog( m_UserID, "advanced host options", irect(0,0,300,200), NULL, WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
        ASSERT(pAdvanced );
        (void)pAdvanced;
    }
*/
}

//=========================================================================

void dlg_online_host::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    UpdateControls();

    // Go back to Tabbed Dialog parent
    if( m_pParent )
    {
        ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
}

//=========================================================================

void dlg_online_host::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    #ifdef DEMO_DISK_HARNESS

    if( (pSender == (ui_win*)m_pGameType) && 
        (m_pGameType->GetSelectedItemData() != GAME_CTF) )
    {
        m_pHost            ->SetFlag( WF_DISABLED, 1 );
        m_pMissions        ->SetFlag( WF_DISABLED, 1 );
        m_pTextName        ->SetFlag( WF_DISABLED, 1 );
        m_pServerName      ->SetFlag( WF_DISABLED, 1 );
        m_pTextPlayers     ->SetFlag( WF_DISABLED, 1 );
        m_pMaxPlayers      ->SetFlag( WF_DISABLED, 1 );
        m_pTextClients     ->SetFlag( WF_DISABLED, 1 );
        m_pMaxClients      ->SetFlag( WF_DISABLED, 1 );
        m_pTeamDamage      ->SetFlag( WF_DISABLED, 1 );
        m_pTeamDamageSlider->SetFlag( WF_DISABLED, 1 );
        m_pTextVotePass    ->SetFlag( WF_DISABLED, 1 );
        m_pVotePassSlider  ->SetFlag( WF_DISABLED, 1 );
        m_pEnableTargetLock->SetFlag( WF_DISABLED, 1 );
        m_pEnableBots      ->SetFlag( WF_DISABLED, 1 );
        m_pHost            ->SetFlag( WF_DISABLED, 1 );
        m_pNumBotsText     ->SetFlag( WF_DISABLED, 1 );
        m_pNumBotsSlider   ->SetFlag( WF_DISABLED, 1 );
        m_pBotsAIText      ->SetFlag( WF_DISABLED, 1 );
        m_pBotsAISlider    ->SetFlag( WF_DISABLED, 1 );
    }
    
    if( (pSender == (ui_win*)m_pGameType) && 
        (m_pGameType->GetSelectedItemData() == GAME_CTF) )
    {
        s32 Offline = (fegl.State == FE_STATE_OFFLINE);
        s32 NoBots  = !(m_pEnableBots->GetFlags() & WF_SELECTED);

        m_pHost            ->SetFlag( WF_DISABLED, 0 );
        m_pMissions        ->SetFlag( WF_DISABLED, 0 );
        m_pTextName        ->SetFlag( WF_DISABLED, Offline );
        m_pServerName      ->SetFlag( WF_DISABLED, Offline );
        m_pTextPlayers     ->SetFlag( WF_DISABLED, 0 );
        m_pMaxPlayers      ->SetFlag( WF_DISABLED, 0 );
        m_pTextClients     ->SetFlag( WF_DISABLED, Offline );
        m_pMaxClients      ->SetFlag( WF_DISABLED, Offline );
        m_pTeamDamage      ->SetFlag( WF_DISABLED, 0 );
        m_pTeamDamageSlider->SetFlag( WF_DISABLED, 0 );
        m_pTextVotePass    ->SetFlag( WF_DISABLED, Offline );
        m_pVotePassSlider  ->SetFlag( WF_DISABLED, Offline );
        m_pEnableTargetLock->SetFlag( WF_DISABLED, 0 );
        m_pEnableBots      ->SetFlag( WF_DISABLED, 0 );
        m_pHost            ->SetFlag( WF_DISABLED, 0 );
        m_pNumBotsText     ->SetFlag( WF_DISABLED, NoBots );
        m_pNumBotsSlider   ->SetFlag( WF_DISABLED, NoBots );
        m_pBotsAIText      ->SetFlag( WF_DISABLED, NoBots );
        m_pBotsAISlider    ->SetFlag( WF_DISABLED, NoBots );

    }

    #endif

    if( pSender == (ui_win*)m_pGameType )
        FillMissionsList();

    // If X pressed in Mission List goto the host control
    if( (pSender == (ui_win*)m_pMissions) && (Command == WN_LIST_ACCEPTED) )
    {
        GotoControl( (ui_control*)m_pHost );
    }

    if( Command == WN_USER )
    {
		ReadOptions();
        if( m_WaitingOnSave )
        {
            // This way we will close the save dialog first before launching the mission load dialog.
            m_WaitingOnClose = TRUE;
            
            m_WaitingOnSave = FALSE;
        }
		s_Dirty=FALSE;
    }
    UpdateControls();
}

//=========================================================================
void dlg_online_host::OnUpdate ( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;

    if( m_WaitingOnClose )
    {
        // Mission Selection within Range?
        if( (m_pMissions->GetSelection() >= 0) && (m_pMissions->GetSelection() < m_pMissions->GetItemCount()) )
        {
            if( fegl.State == FE_STATE_OFFLINE )
            {
                // Disconnect the player.
#ifndef TARGET_PS2_CLIENT
				fegl.DeliberateDisconnect = TRUE;
				net_ActivateConfig(FALSE);
#endif
            }

            // Get Pointer to mission definition
            fegl.pMissionDef = (mission_def*)m_pMissions->GetSelectedItemData();
            ASSERT( fegl.pMissionDef );
            fegl.pNextMissionDef = NULL;

            // Set Name and Game Type
            x_strcpy( fegl.MissionName, fegl.pMissionDef->Folder );
            fegl.GameType = fegl.pMissionDef->GameType;

            pSvrMgr->SetName( m_pServerName->GetLabel() );
            tgl.UserInfo.MyName[0] = '\0';
            x_wstrcpy( tgl.UserInfo.Server, pSvrMgr->GetName() );

            tgl.UserInfo.IsServer    = TRUE;
            tgl.UserInfo.MyAddr      = pSvrMgr->GetLocalAddr();
            tgl.UserInfo.UsingConfig = TRUE;

            audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );

            // Setup Mission Loading Dialog
            ui_dialog* pDialog = tgl.pUIManager->OpenDialog( fegl.UIUserID, "mission load", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
            (void)pDialog;
            ASSERT( pDialog );

            tgl.JumpTime = TRUE;
            s_RenderCalls = 0;
        }
        
        m_WaitingOnClose = FALSE;
    }
}

//=========================================================================

void dlg_online_host::UpdateControls( void )
{
//    s32 Flags;

    if( !m_DisableUpdate )
    {
        m_DisableUpdate++;

        s32 MaxPlayers = m_pMaxPlayers->GetValue();
        s32 oldnumclients = m_pMaxClients->GetValue();

        // For team based games, there should be an even number of players.
        if( (m_pGameType->GetSelectedItemData() == GAME_TDM) ||
            (m_pGameType->GetSelectedItemData() == GAME_CTF) ||
            (m_pGameType->GetSelectedItemData() == GAME_CNH) )
        {
            if( MaxPlayers & 0x01 )
                MaxPlayers += 1;
            m_pMaxPlayers->SetValue( MaxPlayers );
            m_pMaxPlayers->SetStep( 2 );
        }
        else
        {
            m_pMaxPlayers->SetStep( 1 );
        }

        m_pMaxClients->SetRange( 0, MaxPlayers-1 );

        m_pTextPlayers  ->SetLabel( xwstring(xfs("MAX PLAYERS %d",    m_pMaxPlayers      ->GetValue())) );
        m_pTextClients  ->SetLabel( xwstring(xfs("MAX CLIENTS %d",    m_pMaxClients      ->GetValue())) );
        m_pTextTimeLimit->SetLabel( xwstring(xfs("TIME LIMIT %d min", m_pTimeLimitSlider ->GetValue())) );
        m_pTextVotePass ->SetLabel( xwstring(xfs("VOTE PASS %d%%",    m_pVotePassSlider  ->GetValue())) );
        m_pTeamDamage   ->SetLabel( xwstring(xfs("TEAM DAMAGE %d%%",  m_pTeamDamageSlider->GetValue())) );

		s32 oldnumbots = m_pNumBotsSlider->GetValue();
		s32 oldbotdiff = m_pBotsAISlider->GetValue();

        if( (m_pGameType->GetSelectedItemData() == GAME_DM) ||
            (m_pGameType->GetSelectedItemData() == GAME_HUNTER) )
        {
            s32 MinBots = 1;
            s32 MaxBots = MIN(MaxPlayers,12);
            m_pNumBotsSlider->SetRange( MinBots, MaxBots );
            m_pNumBotsText  ->SetLabel( xwstring(xfs("MIN PLAYERS %d",    m_pNumBotsSlider   ->GetValue())) );
        }
        else
        {
            s32 MinBots = 1;
            s32 MaxBots = MIN((MaxPlayers/2),6);
            m_pNumBotsSlider->SetRange( MinBots, MaxBots );
            m_pNumBotsText  ->SetLabel( xwstring(xfs("MIN TEAMSIZE %d",   m_pNumBotsSlider   ->GetValue())) );
        }

        m_pBotsAIText   ->SetLabel( xwstring(xfs("AI LEVEL %d",    m_pBotsAISlider    ->GetValue())) );

        s32 NoBots = 1;

        if( m_pEnableBots->GetFlags() & WF_SELECTED )
            NoBots = 0;

        #ifdef DEMO_DISK_HARNESS
        if( m_pGameType->GetSelectedItemData() != GAME_CTF )
            NoBots = 1;
        #endif

        m_pNumBotsText  ->SetFlag( WF_DISABLED, NoBots );
        m_pNumBotsSlider->SetFlag( WF_DISABLED, NoBots );
        m_pBotsAIText   ->SetFlag( WF_DISABLED, NoBots );
        m_pBotsAISlider ->SetFlag( WF_DISABLED, NoBots );

        /*
        if( m_pEnableBots->GetFlags() & WF_SELECTED )
            Flags = 0;
        else
            Flags = WF_DISABLED;

        m_pNumBotsText  ->SetFlags( (m_pNumBotsText  ->GetFlags() & ~WF_DISABLED) | Flags );
        m_pNumBotsSlider->SetFlags( (m_pNumBotsSlider->GetFlags() & ~WF_DISABLED) | Flags );
        m_pBotsAIText   ->SetFlags( (m_pBotsAIText   ->GetFlags() & ~WF_DISABLED) | Flags );
        m_pBotsAISlider ->SetFlags( (m_pBotsAISlider ->GetFlags() & ~WF_DISABLED) | Flags );
        */

		// This is to override the changed warning should the values have to be changed
		// because the slider limit ranges have been modified.
		if (oldnumbots != m_pNumBotsSlider->GetValue())
		{
			fegl.ServerSettings.BotsNum = m_pNumBotsSlider->GetValue();
		}

		if (oldnumclients != m_pMaxClients->GetValue())
		{
			fegl.ServerSettings.MaxClients = m_pMaxClients->GetValue();
		}

		if (oldbotdiff != m_pBotsAISlider->GetValue())
		{
			fegl.ServerSettings.BotsDifficulty = m_pBotsAISlider->GetValue() / 10.0f;
		}

//		WriteOptions();


        m_DisableUpdate--;
    }
}

//=========================================================================

void dlg_online_host::FillMissionsList( void )
{
    s32         iSel        = m_pMissions->GetSelection();
    s32         GameType    = m_pGameType->GetSelectedItemData();
    s32         i;

    // Fill Listbox with missions
    m_pMissions->DeleteAllItems();
    for( i=0 ; fe_Missions[i].GameType != GAME_NONE ; i++ )
    {
        if( fe_Missions[i].GameType == GameType )
        {
            s32 Item;
            Item = m_pMissions->AddItem( StringMgr( "MissionName", fe_Missions[i].DisplayNameString ), (s32)&fe_Missions[i] );

            #ifdef DEMO_DISK_HARNESS
            if( fe_Missions[i].DisplayNameString == 34 )
                iSel = Item;
            else
                m_pMissions->EnableItem( Item, FALSE );
            #endif
        }
    }

    // Limit Selection
    if( iSel <  0 ) iSel = 0;
    if( iSel >= m_pMissions->GetItemCount() ) iSel = m_pMissions->GetItemCount()-1;

    // Set Selection
    m_pMissions->SetSelection( iSel );
}

//=========================================================================
void dlg_online_host::ReadOptions(void)
{
    if( fegl.GameModeOnline )
    {
	    m_pServerName		->SetLabel( fegl.ServerSettings.ServerName );
	    m_pServerPassword	->SetLabel( fegl.ServerSettings.AdminPassword );
        m_pVotePassSlider	->SetValue( fegl.ServerSettings.VotePass );
    }
    else
    {
	    m_pServerName		->SetLabel( "Tribes" );
	    m_pServerPassword	->SetLabel( "" );
        m_pVotePassSlider	->SetValue( 0 );
    }

    m_pMaxPlayers		->SetValue( fegl.ServerSettings.MaxPlayers );
    m_pMaxClients		->SetValue( fegl.ServerSettings.MaxClients );
    m_pTeamDamageSlider	->SetValue( (s32)(fegl.ServerSettings.uiTeamDamage) );
    m_pTimeLimitSlider	->SetValue( fegl.ServerSettings.TimeLimit );
    m_pEnableTargetLock ->SetSelected( fegl.ServerSettings.TargetLockEnabled );
    m_pEnableBots		->SetSelected( fegl.ServerSettings.BotsEnabled );
    m_pNumBotsSlider	->SetValue( fegl.ServerSettings.BotsNum );
    m_pBotsAISlider		->SetValue( (s32)(fegl.ServerSettings.uiBotsDifficulty) );
    
    fegl.ServerSettings.BotsDifficulty = fegl.ServerSettings.uiBotsDifficulty / 10.0f;
}

s32 tmp_wstrcmp(const xwchar* str1, const xwchar* str2)
{
	while (*str1)
	{
		if (*str1 > *str2)
		{
			return 1;
		}

		if (*str1 < *str2)
		{
			return -1;
		}

		if (*str2==0)
			return 1;

		str1++;
		str2++;
	}

	if (*str2)
		return -1;
	return 0;
}

void dlg_online_host::WriteOptions(void)
{
	s32 newmaxplayers;
	s32 newmaxclients;
	s32 newtimelimit;
	s32 newvotepass;
	s32 newbotsnum;
    s32 newgametype;
    s32 newmapindex;

	xbool newlockenabled;
	xbool newbotsenabled;

	s32 newteamdamage;
	s32 newbotsdifficulty;

    newmaxplayers          = m_pMaxPlayers->GetValue();
    newmaxclients          = m_pMaxClients->GetValue();
    newtimelimit           = m_pTimeLimitSlider->GetValue();
    newvotepass            = m_pVotePassSlider->GetValue();
    newteamdamage          = m_pTeamDamageSlider->GetValue();
    newlockenabled		   = (m_pEnableTargetLock->GetFlags() & WF_SELECTED) ? TRUE : FALSE;
    newbotsenabled         = (m_pEnableBots->GetFlags() & WF_SELECTED) ? TRUE : FALSE;
    newbotsnum             = m_pNumBotsSlider->GetValue();
    newbotsdifficulty      = m_pBotsAISlider->GetValue();
    newgametype            = m_pGameType->GetSelection();
    newmapindex            = m_pMissions->GetSelection();

	if (newmaxplayers != fegl.ServerSettings.MaxPlayers)
	{
		fegl.ServerSettings.MaxPlayers          = newmaxplayers;
		s_Dirty = TRUE;
	}
	if (newmaxclients != fegl.ServerSettings.MaxClients)
	{
		fegl.ServerSettings.MaxClients          = newmaxclients;
		s_Dirty = TRUE;
	}
	if (newtimelimit != fegl.ServerSettings.TimeLimit)
	{
		fegl.ServerSettings.TimeLimit           = newtimelimit;
		s_Dirty = TRUE;
	}
	if (newvotepass != fegl.ServerSettings.VotePass)
	{
        if( fegl.GameModeOnline )
        {
		    fegl.ServerSettings.VotePass            = newvotepass;
		    s_Dirty = TRUE;
        }
	}
	if (newteamdamage != fegl.ServerSettings.uiTeamDamage)
	{
		fegl.ServerSettings.uiTeamDamage        = newteamdamage;
		fegl.ServerSettings.TeamDamage          = newteamdamage / 100.0f;
		s_Dirty = TRUE;
	}
	if (newlockenabled != fegl.ServerSettings.TargetLockEnabled)
	{
		fegl.ServerSettings.TargetLockEnabled   = newlockenabled;
		s_Dirty = TRUE;
	}
	if (newbotsenabled != fegl.ServerSettings.BotsEnabled)
	{
		fegl.ServerSettings.BotsEnabled         = newbotsenabled;
		s_Dirty = TRUE;
	}
	if (newbotsnum != fegl.ServerSettings.BotsNum)
	{
		fegl.ServerSettings.BotsNum             = newbotsnum;
		s_Dirty = TRUE;
	}
	if (newbotsdifficulty != fegl.ServerSettings.uiBotsDifficulty)
	{
		fegl.ServerSettings.uiBotsDifficulty    = newbotsdifficulty;
		fegl.ServerSettings.BotsDifficulty      = newbotsdifficulty / 10.0f;
		s_Dirty = TRUE;
	}
    if( tmp_wstrcmp(fegl.ServerSettings.ServerName,m_pServerName->GetLabel())!=0 )
    {
        if( fegl.GameModeOnline )
        {
            x_wstrncpy( fegl.ServerSettings.ServerName, m_pServerName->GetLabel(), FE_MAX_SERVER_NAME );
            fegl.ServerSettings.ServerName[FE_MAX_SERVER_NAME-1] = 0;
            s_Dirty = TRUE;
        }
    }
    if( tmp_wstrcmp(fegl.ServerSettings.AdminPassword, m_pServerPassword->GetLabel())!=0 )
    {
        if( fegl.GameModeOnline )
        {
            x_wstrncpy( fegl.ServerSettings.AdminPassword, m_pServerPassword->GetLabel(), FE_MAX_ADMIN_PASS );
            fegl.ServerSettings.AdminPassword[FE_MAX_ADMIN_PASS-1] = 0;
            s_Dirty = TRUE;
        }
    }
    if( newgametype != fegl.ServerSettings.GameType )
    {
        fegl.ServerSettings.GameType = newgametype;
        s_Dirty = TRUE;
    }
    if( newmapindex != fegl.ServerSettings.MapIndex )
    {
        fegl.ServerSettings.MapIndex = newmapindex;
        s_Dirty = TRUE;
    }
}
