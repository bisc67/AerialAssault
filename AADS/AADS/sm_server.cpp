//==============================================================================
//
//  sm_server.cpp
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
#include "HUD/hud_manager.hpp"
#include "ClientServer.hpp"
#include "telnetconsole/telnetmgr.hpp"

void  GetMissionDefFromNameAndType( void );
xbool LoadMission( void );
void  UnloadMission( void );
void  AddServerPlayers( void );
void  KillServerPlayers( void );
void  InitMissionSystems( void );
void  InitBotAdvance( void );

void UpdateIngame( f32 DeltaSec );

extern server_manager* pSvrMgr;

void AdvanceMission( void );

//==============================================================================
//==============================================================================
//==============================================================================

//==============================================================================

void GetMissionDefFromNameAndType( void )
{
    mission_def*    pFoundMissionDef = NULL;
    mission_def*    pMissionDef      = fe_Missions;

    while( pMissionDef->GameType != GAME_NONE )
    {
        if( (pMissionDef->GameType == fegl.GameType) &&
            (x_strcmp( pMissionDef->Folder, fegl.MissionName ) == 0) )
        {
            pFoundMissionDef = pMissionDef;
        }

        pMissionDef++;
    }

    fegl.pMissionDef = pFoundMissionDef;
}

//==============================================================================

static void sm_InitServer_Begin( void )
{
    pSvrMgr->SetName( fegl.ServerSettings.ServerName );

    tgl.pServer = new game_server;
    tgl.ServerPresent = TRUE;
    tgl.pServer->InitNetworking(tgl.UserInfo.MyAddr, tgl.UserInfo.Server);

    tgl.Connected = TRUE;

    //
    // DECIDE MISSION
    //
    x_strcpy ( tgl.MissionName, fegl.MissionName );
    x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
    x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
    GameMgr.SetGameType( (game_type)fegl.GameType, fegl.CampaignMission );

    GetMissionDefFromNameAndType();

    sm_SetState( STATE_SERVER_LOAD_MISSION );
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_LoadMission_Begin( void )
{
    g_SM.MissionLoading = TRUE;

    // Register with master server once we have completed server initialization
    tgl.pMasterServer->SetName(fegl.ServerSettings.ServerName);
	char ver[64];

	x_sprintf(ver,"%d",SERVER_VERSION);
	tgl.pMasterServer->SetAttribute("V",ver);
    pSvrMgr->SetIsInGame(TRUE);
    // Don't register with master server or respond to local lookups if we're
    // on a campaign game type
    if( fegl.GameModeOnline )
    {
        pSvrMgr->SetAsServer(TRUE);
        tgl.pMasterServer->SetServer(TRUE);
    }
    else
    {
        pSvrMgr->SetAsServer(FALSE);
        tgl.pMasterServer->SetServer(FALSE);
    }

    // Initially, there is no score limit.  May get one from mission file.
    GameMgr.SetScoreLimit( 0 );

    if( fegl.GameType != GAME_CAMPAIGN )
    {
        GameMgr.SetPlayerLimits( fegl.ServerSettings.MaxPlayers,
                                 fegl.ServerSettings.BotsNum,
                                 FALSE );
    }

    InitMissionSystems();
}

//==============================================================================

static void sm_LoadMission_Advance( f32 DeltaSec )
{
    (void)DeltaSec;
    if( LoadMission() )
        sm_SetState( STATE_SERVER_INGAME );
}

//==============================================================================

static void sm_LoadMission_End( void )
{
    g_SM.MissionLoading = FALSE;
    g_SM.MissionLoaded = TRUE;

    xbool EnableBots = fegl.ServerSettings.BotsEnabled && 
                       PathManager_IsGraphLoaded();

    if( fegl.GameType != GAME_CAMPAIGN )
    {
        GameMgr.SetPlayerLimits( fegl.ServerSettings.MaxPlayers,
                                 fegl.ServerSettings.BotsNum,
                                 EnableBots );
    }

    GameMgr.SetTeamDamage( fegl.ServerSettings.TeamDamage );

#ifdef DEBUG_SHORT_GAMES
    GameMgr.SetTimeLimit ( 2 );
#else
    GameMgr.SetTimeLimit ( fegl.ServerSettings.TimeLimit );
#endif

    GameMgr.BeginGame();

    MAI_Mgr.InitMission();
    
    tgl.pHUDManager->Initialize();
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_Ingame_Begin( void )
{
    g_SM.MissionRunning     = TRUE;
    g_SM.ServerMissionEnded = FALSE;
    g_SM.ServerShutdown     = FALSE;

    /* DEDICATED_SERVER
    s32 i;

    ASSERT(tgl.NLocalPlayers);
    for (i=0;i<tgl.NLocalPlayers;i++)
    {
        if (i==0)
        {
            audio_SetEarView(&tgl.PC[i].View);
        }
        else
        {
            audio_AddEarView(&tgl.PC[i].View);
        }
    }
    DEDICATED_SERVER */
}

//==============================================================================

static void sm_Ingame_Advance( f32 DeltaSec )
{
    UpdateIngame( DeltaSec );

    if( !g_SM.MissionRunning )
    {
        if( g_SM.ServerShutdown || g_SM.NetworkDown)
        {
            sm_SetState( STATE_SERVER_SHUTDOWN );
        }
        else
        if( g_SM.ServerMissionEnded )
        {
            sm_SetState( STATE_SERVER_COOL_DOWN );
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
    audio_StopAll();
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_CoolDown_Begin( void )
{
}

//==============================================================================

static void sm_CoolDown_Advance( f32 DeltaSec )
{
    (void)DeltaSec;

    if( tgl.pServer->CoolDown() )
    {
        sm_UnloadMission();

        // Decide on new mission
        AdvanceMission();
        x_strcpy ( tgl.MissionName, fegl.MissionName );
        x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
        x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
        GameMgr.SetGameType( (game_type)fegl.GameType, fegl.CampaignMission );


        sm_SetState( STATE_SERVER_LOAD_MISSION );
    }
}

//==============================================================================

static void sm_CoolDown_End( void )
{
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_Shutdown_Begin( void )
{
}

//==============================================================================

static void sm_Shutdown_Advance( f32 DeltaSec )
{
    (void)DeltaSec;

    if( tgl.pServer->DisconnectEveryone() )
    {
        KillServerPlayers();
        sm_UnloadMission();
        
        // Deallocate server
        delete tgl.pServer;
        tgl.pServer = NULL;
        tgl.ServerPresent = FALSE;

        tgl.ExitApp = TRUE;
    }
}

//==============================================================================

static void sm_Shutdown_End( void )
{
}

//==============================================================================
//==============================================================================
//==============================================================================
// STATE TABLE
//==============================================================================
//==============================================================================
//==============================================================================

sm_state s_ServerState[] =
{
    {STATE_SERVER_INIT_SERVER,      sm_InitServer_Begin,    NULL,                       NULL },
    {STATE_SERVER_LOAD_MISSION,     sm_LoadMission_Begin,   sm_LoadMission_Advance,     sm_LoadMission_End},
    {STATE_SERVER_INGAME,           sm_Ingame_Begin,        sm_Ingame_Advance,          sm_Ingame_End },
    {STATE_SERVER_COOL_DOWN,        sm_CoolDown_Begin,      sm_CoolDown_Advance,        sm_CoolDown_End },
    {STATE_SERVER_SHUTDOWN,         sm_Shutdown_Begin,      sm_Shutdown_Advance,        sm_Shutdown_End },
    {STATE_NULL, NULL, NULL, NULL},
};

//==============================================================================


