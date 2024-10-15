//==============================================================================
//
//  sm_client.cpp
//
//==============================================================================

#include "x_types.hpp"
#include "globals.hpp"
#include "audiomgr/audio.hpp"
#include "fe_globals.hpp"
#include "gameserver.hpp"
#include "gameclient.hpp"
#include "masterserver/masterserver.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "Objects\Bot\MAI_Manager.hpp"
#include "sm_common.hpp"
#include "FrontEnd.hpp"

void sm_SetState( s32 StateType );
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


static f32 s_KickTime;
static s32 s_KickRunning=FALSE;


//==============================================================================
//==============================================================================
//==============================================================================

static void sm_InitClient_Begin( void )
{
    ASSERT( tgl.pClient == NULL );

    if (tgl.MusicId)
    {
        audio_Stop(tgl.MusicId);
        tgl.MusicId =0;
    }

    tgl.pClient = new game_client;
    tgl.ClientPresent = TRUE;
    tgl.pClient->InitNetworking(tgl.UserInfo.MyAddr,
                                tgl.UserInfo.ServerAddr,
                                tgl.UserInfo.Server,
                                tgl.UserInfo.MyName,
								0);

    fegl.MissionName[0] = '\0';

    //tgl.GameStage = GAME_STAGE_DECIDE_MISSION;
    tgl.Connected = TRUE;
	s_KickRunning = FALSE;

}

//==============================================================================

static void sm_InitClient_Advance( f32 DeltaSec )
{
    (void)DeltaSec;

    ASSERT( tgl.pClient );

    if( (g_SM.SecInState > 20.0f) || (tgl.pClient->IsLoginRefused()) )
    {
        x_DebugMsg("LOGIN UNSUCCESSFUL\n");
        g_SM.LoginFailed = TRUE;
        EndMissionDialogs();
        sm_SetState(STATE_CLIENT_RETURN_TO_FE);
        return;
    }

    if( tgl.pClient->IsLoggedIn() )
    {
		s_KickRunning = FALSE;
        sm_SetState(STATE_CLIENT_REQUEST_MISSION);
    }
	else if ( tgl.pClient->GetPing() > 600.0f )
	{
		if (s_KickRunning)
		{
			s_KickTime -= DeltaSec;
			if (s_KickTime < 0.0f)
			{
				g_SM.ServerTooBusy = TRUE;
				g_SM.LoginFailed   = TRUE;
				EndMissionDialogs();
				sm_SetState(STATE_CLIENT_RETURN_TO_FE);
				return;
			}
		}
		else
		{
			s_KickRunning = TRUE;
			s_KickTime = 2.0f;
		}
	}
	else
	{
		s_KickRunning = FALSE;
	}
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_RequestMission_Begin( void )
{
	s_KickRunning = FALSE;
}

//==============================================================================

static void sm_RequestMission_Advance( f32 DeltaSec )
{
    (void)DeltaSec;

    if( fegl.MissionName[0] )
    {
        x_strcpy ( tgl.MissionName, fegl.MissionName );
        x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
        x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
        GameMgr.SetGameType( (game_type)fegl.GameType );
        sm_SetState( STATE_CLIENT_LOAD_MISSION );
    }
	else if ( tgl.pClient->GetPing() > 600.0f )
	{
		if (s_KickRunning)
		{
			s_KickTime -= DeltaSec;
			if (s_KickTime < 0.0f)
			{
				g_SM.ServerTooBusy = TRUE;
				g_SM.LoginFailed   = TRUE;
				EndMissionDialogs();
				sm_SetState(STATE_CLIENT_RETURN_TO_FE);
				return;
			}
		}
		else
		{
			s_KickRunning = TRUE;
			s_KickTime = 2.0f;
		}
	}
	else
	{
		s_KickRunning = FALSE;
	}

    // Send a mission request every second
    if( (tgl.NRenders%30)==0 )
        tgl.pClient->RequestMission();
}

//==============================================================================

static void sm_RequestMission_End( void )
{
    GetMissionDefFromNameAndType();
    InitMissionSystems();
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_LoadMission_Begin( void )
{
    g_SM.MissionLoading = TRUE;

    tgl.pMasterServer->SetServer(FALSE);
    pSvrMgr->SetIsInGame(TRUE);
    pSvrMgr->SetAsServer(FALSE);
	s_KickRunning = FALSE;

    // Remove Mission Load or Score front end screen
    //EndMissionDialogs();

    // Show mission load screen
    ShowMissionLoadDialog();

    InitMissionSystems();
}

//==============================================================================

static void sm_LoadMission_Advance( f32 DeltaSec )
{
    (void)DeltaSec;
    if( LoadMission() )
        sm_SetState( STATE_CLIENT_SYNC );
}

//==============================================================================

static void sm_LoadMission_End( void )
{
    g_SM.MissionLoading = FALSE;
    g_SM.MissionLoaded = TRUE;

}

//==============================================================================
//==============================================================================
//==============================================================================

static f32 SyncTimeoutSec = 32.0f;

//==============================================================================

static void sm_SyncWithServer_Begin( void )
{
	s_KickRunning = FALSE;
    x_DebugMsg("***************************\n");
    x_DebugMsg("Starting Sync with Server\n");
    x_DebugMsg("***************************\n");
}

//==============================================================================

static void sm_SyncWithServer_Advance( f32 DeltaSec )
{
	x_DebugMsg("SYNCING----\n");

    // Are we properly in sync?
    if( tgl.pClient->IsInSync() && (tgl.NLocalPlayers>0) )
    {
        sm_SetState( STATE_CLIENT_INGAME );
    }

    // Have we timed out trying to sync?
    if( (g_SM.SecInState > SyncTimeoutSec) || (!tgl.pClient->IsLoggedIn()) )
    {
        x_DebugMsg("Sync with server timed out\n");
        g_SM.Disconnected = TRUE;
        sm_SetState( STATE_CLIENT_RETURN_TO_FE );
    }

    // If the server still there?
    if( g_SM.ServerShutdown )
    {
        g_SM.Disconnected = TRUE;
        sm_SetState( STATE_CLIENT_RETURN_TO_FE );
    }

    // Still waiting so process time
    tgl.pClient->ProcessTime(DeltaSec);
}

//==============================================================================

static void sm_SyncWithServer_End( void )
{
    x_DebugMsg("***************************\n");
    x_DebugMsg("Finished Sync with Server\n");
    x_DebugMsg("Sync time %f sec\n",g_SM.SecInState);
    x_DebugMsg("***************************\n");

    // Remove Mission Load front end screen
    EndMissionDialogs();

    if( g_SM.SecInState < SyncTimeoutSec )
        GameMgr.BeginGame();
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_Ingame_Begin( void )
{
    s32 i;

	s_KickRunning = FALSE;
    g_SM.MissionRunning     = TRUE;
    g_SM.ServerMissionEnded = FALSE;
    g_SM.ServerShutdown     = FALSE;
    g_SM.Disconnected       = FALSE;
    g_SM.ClientQuit         = FALSE;
    g_SM.ClientKicked       = FALSE;
    g_SM.ClientBanned       = FALSE;
    g_SM.InvalidPassword    = FALSE;
    g_SM.ServerFull         = FALSE;

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
}

//==============================================================================

static void sm_Ingame_Advance( f32 DeltaSec )
{
    UpdateIngame( DeltaSec );

    if( tgl.pClient->IsLoggedIn() == FALSE )
    {
        g_SM.MissionRunning = FALSE;
        g_SM.Disconnected = TRUE;
    }

    if( !g_SM.MissionRunning )
    {
        if( g_SM.ServerShutdown )
        {
            ShowMissionEndDialog( TRUE );
            sm_SetState( STATE_CLIENT_RETURN_TO_FE );
        }
        else
        if( g_SM.Disconnected || g_SM.NetworkDown )
        {
            ShowMissionEndDialog( TRUE );
            sm_SetState( STATE_CLIENT_RETURN_TO_FE );
        }
        else
        if( g_SM.ClientQuit )
        {
            ShowMissionEndDialog( TRUE );
            sm_SetState( STATE_CLIENT_DISCONNECT );
        }
        else
        if( g_SM.ServerMissionEnded )
        {
            ShowMissionEndDialog( FALSE );
            sm_SetState( STATE_CLIENT_UNLOAD );
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

static void sm_Disconnect_Advance( f32 DeltaSec )
{
    (void)DeltaSec;

    // Tell server to disconnect        
    if( (!tgl.pClient->IsLoggedIn()) || ( g_SM.SecInState > 5.0f ) )
    {
        sm_SetState(STATE_CLIENT_RETURN_TO_FE);
    }
    else
    {
        tgl.pClient->Disconnect();
    }
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_Unload_Begin( void )
{
    sm_UnloadMission();
    tgl.pClient->EndMission();
    sm_SetState(STATE_CLIENT_REQUEST_MISSION);
}

//==============================================================================
//==============================================================================
//==============================================================================

static void sm_ReturnToFE_Begin( void )
{
    sm_UnloadMission();

    // Be sure game has ended on this side
    if( GameMgr.GameInProgress() )
        GameMgr.EndGame();

    // Deallocate client
    delete tgl.pClient;
    tgl.pClient = NULL;
    tgl.ClientPresent = FALSE;

    sm_SetState(STATE_COMMON_INIT_FRONTEND);
}

//==============================================================================
//==============================================================================
//==============================================================================
// STATE TABLE
//==============================================================================
//==============================================================================
//==============================================================================

sm_state s_ClientState[] =
{
    {STATE_CLIENT_INIT_CLIENT,      sm_InitClient_Begin,        sm_InitClient_Advance,      NULL },
    {STATE_CLIENT_REQUEST_MISSION,  sm_RequestMission_Begin,    sm_RequestMission_Advance,  sm_RequestMission_End },
    {STATE_CLIENT_LOAD_MISSION,     sm_LoadMission_Begin,       sm_LoadMission_Advance,     sm_LoadMission_End },
    {STATE_CLIENT_SYNC,             sm_SyncWithServer_Begin,    sm_SyncWithServer_Advance,  sm_SyncWithServer_End },
    {STATE_CLIENT_INGAME,           sm_Ingame_Begin,            sm_Ingame_Advance,          sm_Ingame_End },
    {STATE_CLIENT_UNLOAD,           sm_Unload_Begin,            NULL,                       NULL },
    {STATE_CLIENT_RETURN_TO_FE,     sm_ReturnToFE_Begin,        NULL,                       NULL },
    {STATE_CLIENT_DISCONNECT,       NULL,                       sm_Disconnect_Advance,      NULL },
    {STATE_NULL, NULL, NULL, NULL},
};
    
//==============================================================================


