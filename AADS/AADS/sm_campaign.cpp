//==============================================================================
//
//  sm_campaign.cpp
//
//==============================================================================

#include "x_types.hpp"
#include "globals.hpp"
#include "audiomgr/audio.hpp"
#include "fe_globals.hpp"
#include "gameserver.hpp"
#include "masterserver/masterserver.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "Objects\Bot\MAI_Manager.hpp"
#include "sm_common.hpp"
#include "FrontEnd.hpp"

void  sm_SetState( s32 StateType );
xbool InitFrontEnd( f32 DeltaSec );
void  GetMissionDefFromNameAndType( void );
void  ShowMissionLoadDialog( void );
void  ShowMissionEndDialog( xbool AllowContinue );
void  EndMissionDialogs( void );
void  KillFrontEnd( void );
xbool UpdateFrontEnd( f32 DeltaSec );
xbool LoadMission( void );
void  UnloadMission( void );
void  EndLoadMission( void );
void  AddServerPlayers( void );
void  KillServerPlayers( void );
void  InitMissionSystems( void );
void  KillMissionSystems( void );
void  InitBotAdvance( void );

void UpdateIngame( f32 DeltaSec );

extern server_manager* pSvrMgr;




//==============================================================================
//==============================================================================
//==============================================================================

static void sm_InitCampaign_Begin( void )
{
    ASSERT( fegl.GameType == GAME_CAMPAIGN );

    if (tgl.MusicId)
    {
        audio_Stop(tgl.MusicId);
        tgl.MusicId = 0;
    }

    tgl.pServer = new game_server;
    tgl.ServerPresent = TRUE;
    tgl.pServer->InitNetworking(tgl.UserInfo.MyAddr, tgl.UserInfo.Server);

    //tgl.GameStage = GAME_STAGE_DECIDE_MISSION;
    tgl.Connected = TRUE;

    //
    // DECIDE MISSION
    //
    x_strcpy ( tgl.MissionName, fegl.MissionName );
    x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
    x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
    GameMgr.SetGameType( (game_type)fegl.GameType, fegl.CampaignMission );
    AddServerPlayers();

    GetMissionDefFromNameAndType();
    InitMissionSystems();

    sm_SetState( STATE_CAMPAIGN_LOAD_MISSION );
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_LoadMission_Begin( void )
{
    g_SM.MissionLoading = TRUE;

    // Remove Mission Load or Score front end screen
    EndMissionDialogs();

    // Show mission load screen
    ShowMissionLoadDialog();

    // Initially, there is no score limit.  May get one from mission file.
    GameMgr.SetScoreLimit( 0 );
}

//==============================================================================

static void sm_LoadMission_Advance( f32 DeltaSec )
{
    (void)DeltaSec;
    if( LoadMission() )
        sm_SetState( STATE_CAMPAIGN_INGAME );
}

//==============================================================================

static void sm_LoadMission_End( void )
{
    g_SM.MissionLoading = FALSE;
    g_SM.MissionLoaded  = TRUE;

    // Remove Mission Load front end screen
    EndMissionDialogs();

    ASSERT( fegl.GameType == GAME_CAMPAIGN );

    GameMgr.SetTeamDamage( 0.75f ); //fegl.ServerSettings.TeamDamage );
    GameMgr.SetTimeLimit( 30 );     //fegl.ServerSettings.TimeLimit );

    GameMgr.BeginGame();

    MAI_Mgr.InitMission();
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_Ingame_Begin( void )
{
    g_SM.MissionRunning     = TRUE;
    g_SM.ServerMissionEnded = FALSE;
    g_SM.ServerShutdown     = FALSE;
}

//==============================================================================

static void sm_Ingame_Advance( f32 DeltaSec )
{
    UpdateIngame( DeltaSec );

    if( !g_SM.MissionRunning )
    {
        if( g_SM.ServerShutdown )
        {
            ShowMissionEndDialog( TRUE );
            sm_SetState( STATE_CAMPAIGN_SHUTDOWN );
        }
        else
        if( g_SM.CampaignMissionEnded )
        {
            ShowMissionEndDialog( TRUE );
            sm_SetState( STATE_CAMPAIGN_SHUTDOWN );
        }
        else
        {
            // I don't know how game was ended
            ASSERT( FALSE );
        }
    }
}

//==============================================================================

static void sm_Ingame_End( void )
{
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_CoolDown_Advance( f32 DeltaSec )
{
    (void)DeltaSec;

    sm_UnloadMission();

    // Decide on new mission
    AdvanceMission();
    x_strcpy ( tgl.MissionName, fegl.MissionName );
    x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
    x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
    GameMgr.SetGameType( (game_type)fegl.GameType, fegl.CampaignMission );

    sm_SetState( STATE_CAMPAIGN_LOAD_MISSION );
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_Shutdown_Advance( f32 DeltaSec )
{
    (void)DeltaSec;

    KillServerPlayers();
    sm_UnloadMission();
        
    // Deallocate server
    delete tgl.pServer;
    tgl.pServer = NULL;
    tgl.ServerPresent = FALSE;

    sm_SetState( STATE_COMMON_INIT_FRONTEND );
}

//==============================================================================
//==============================================================================
//==============================================================================
// STATE TABLE
//==============================================================================
//==============================================================================
//==============================================================================

sm_state s_CampaignState[] =
{
    {STATE_CAMPAIGN_INIT_CAMPAIGN,      sm_InitCampaign_Begin,    NULL,                       NULL },
    {STATE_CAMPAIGN_LOAD_MISSION,     sm_LoadMission_Begin,   sm_LoadMission_Advance,     sm_LoadMission_End},
    {STATE_CAMPAIGN_INGAME,           sm_Ingame_Begin,        sm_Ingame_Advance,          sm_Ingame_End },
    {STATE_CAMPAIGN_COOL_DOWN,        NULL,      sm_CoolDown_Advance,        NULL },
    {STATE_CAMPAIGN_SHUTDOWN,         NULL,      sm_Shutdown_Advance,        NULL },
    {STATE_NULL, NULL, NULL, NULL},
};

//==============================================================================


