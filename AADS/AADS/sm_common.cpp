//==============================================================================
//
//  sm_common.cpp
//
//==============================================================================

#include "entropy.hpp"
#include "netlib\netlib.hpp"
#include "netlib\bitstream.hpp"
#include "tokenizer\tokenizer.hpp"
#include "globals.hpp"
#include "fe_globals.hpp"
#include "gameserver.hpp"
#include "gameclient.hpp"
#include "gameuser.hpp"
#include "Building\BuildingOBJ.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "audiomgr/audio.hpp"
#include "masterserver/masterserver.hpp"
#include "savedgame.hpp"
#include "ui\ui_manager.hpp"
#include "hud\hud_manager.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "pointlight\pointlight.hpp"
#include "Objects\Bot\MAI_Manager.hpp"
#include "sm_common.hpp"
#include "StringMgr\StringMgr.hpp"
#include "AADS\Data\UI\ui_strings.h"

//==============================================================================

extern void GotoMainMenuDialog( void );

void  UnloadMission( void );
void  EndLoadMission( void );
void  AddServerPlayers( void );
void  KillServerPlayers( void );
void  InitMissionSystems( void );
void  KillMissionSystems( void );
void  InitBotAdvance( void );
static void  UpdateNetworkConnection( f32 DeltaSec );

//==============================================================================
//  LOCAL STORAGE
//==============================================================================

xbool RumbleOn = FALSE;

extern server_manager* pSvrMgr;
extern xstring* x_fopen_log;
extern sm_state s_ServerState[];

sm_common g_SM;

char* MissionPrefix[] = 
{
    "",
    "CTF-",
    "CNH-",
    "TDM-",
    "DM-",
    "HUNT-",
    "SOLO-",
};

//==============================================================================

void sm_AdvanceServer( f32 DeltaSec );
xbool UpdateFrontEnd( f32 DeltaSec );

//==============================================================================

void sm_SetState( state_enum StateType )
{
    // This resets all the timers to make sure that when we change a state,
    // all the subsequent advances occur properly after a state change.
    tgl.ActualTime = x_GetTime();
    g_SM.NewState  = StateType;
}

//==============================================================================

const char* s_StateName[] = 
{
    "STATE_NULL",
    "STATE_COMMON_INIT",
    "STATE_COMMON_INIT_TITLES",
    "STATE_COMMON_INIT_FRONTEND",
    "STATE_COMMON_FRONTEND",    
    "STATE_SERVER_INIT_SERVER",
    "STATE_SERVER_LOAD_MISSION",
    "STATE_SERVER_INGAME",
    "STATE_SERVER_COOL_DOWN",
    "STATE_SERVER_SHUTDOWN",    
    "STATE_CAMPAIGN_INIT_CAMPAIGN",
    "STATE_CAMPAIGN_LOAD_MISSION",
    "STATE_CAMPAIGN_INGAME",
    "STATE_CAMPAIGN_COOL_DOWN",
    "STATE_CAMPAIGN_SHUTDOWN",    
    "STATE_CLIENT_INIT_CLIENT",
    "STATE_CLIENT_REQUEST_MISSION",
    "STATE_CLIENT_LOAD_MISSION",
    "STATE_CLIENT_SYNC",
    "STATE_CLIENT_INGAME",
    "STATE_CLIENT_UNLOAD",
    "STATE_CLIENT_RETURN_TO_FE",
    "STATE_CLIENT_DISCONNECT",
};

const char* sm_GetState( void )
{
    return( s_StateName[ g_SM.CurrentState ] );
}   

//==============================================================================

sm_common::sm_common( void )
{
    MissionRunning       = FALSE;
    MissionLoading       = FALSE;
    ClientKicked         = FALSE;
    ClientQuit           = FALSE;
    ServerMissionEnded   = FALSE;
    ServerShutdown       = FALSE;
    Disconnected         = FALSE;
    CampaignMissionEnded = FALSE;
    LoginFailed          = FALSE;
	ServerTooBusy		 = FALSE;
	TooManyClients		 = FALSE;
    MissionLoaded        = FALSE;
	NetworkDown		     = FALSE;
    ClientBanned         = FALSE;
    InvalidPassword      = FALSE;
    ServerFull           = FALSE;
}

//==============================================================================

void sm_Time( f32 DeltaSec )
{
    if( RumbleOn )
    {
        input_Feedback(1.0f,1.0f,0);
    }
    else
    {
        input_Feedback(0.0f,0.0f,0);
    }

    // Watch network connection 
    UpdateNetworkConnection( DeltaSec );


    // Find state entries
    sm_state* pNew = NULL;
    sm_state* pCurrent = NULL;

    sm_state* pState[] = 
    {
        s_ServerState,
        NULL,
    };

    s32 j=0;
    while( 1 )
    {
        if( pState[j] == NULL ) break;

        s32 i=0;
        while(1)
        {
            if( pState[j][i].StateType == STATE_NULL )
                break;

            if( pState[j][i].StateType == g_SM.NewState )
                pNew = &pState[j][i];

            if( pState[j][i].StateType == g_SM.CurrentState )
                pCurrent = &pState[j][i];

            i++;
        }

        j++;
    }

    // Switch states if necessary
    if( g_SM.NewState != g_SM.CurrentState )
    {
        x_DebugMsg( "STATE CHANGE FROM %1d(%s)(%1.3f) to %1d(%s)\n", 
                    g_SM.CurrentState, 
                    s_StateName[ g_SM.CurrentState ],
                    g_SM.SecInState,
                    g_SM.NewState,
                    s_StateName[ g_SM.NewState ] );

        g_SM.CurrentState = g_SM.NewState;

        if( pCurrent && pCurrent->pEndFunc )
            pCurrent->pEndFunc();

        if( pNew && pNew->pBeginFunc )
            pNew->pBeginFunc();

        pCurrent = pNew;
        g_SM.SecInState = 0;
    }

    // Advance current state
    if( pCurrent && pCurrent->pAdvanceFunc )
        pCurrent->pAdvanceFunc( DeltaSec );

    // Increment time we've been in this state
    g_SM.SecInState += DeltaSec;
}

//==============================================================================

void UpdateNetworkConnection( f32 DeltaSec )
{
    interface_info info;
    s32 status;

    // Fire up server broadcast manager and refresh
    if( pSvrMgr == NULL )
    {
        pSvrMgr = new server_manager;
    }
    pSvrMgr->Refresh(DeltaSec);


    // Check on interface
    net_GetInterfaceInfo(-1,info);
    if (info.Address)
    {
        if (!tgl.UserInfo.MyAddr.IP)
        {
            status = net_Bind(tgl.UserInfo.MyAddr,fegl.PortNumber);
            if (status)
            {
                pSvrMgr->SetLocalAddr(tgl.UserInfo.MyAddr);
                tgl.pMasterServer->SetAddress(tgl.UserInfo.MyAddr);
            }
        }
    }
    else
    {
        if (tgl.UserInfo.MyAddr.IP)
        {
            net_Unbind(tgl.UserInfo.MyAddr);
            tgl.UserInfo.MyAddr.Clear();
            pSvrMgr->SetLocalAddr(tgl.UserInfo.MyAddr);
            tgl.pMasterServer->ClearAddress();
        }
    }

    // Update stats
    if( (tgl.LogicTimeMs - tgl.LastNetStatsResetTime) > 10000 )
    {
        tgl.LastNetStatsResetTime = tgl.LogicTimeMs;
        net_ClearStats();
    }

    if( tgl.ClientPresent )
        tgl.pClient->ProcessHeartbeat(DeltaSec);

    tgl.pMasterServer->Update(DeltaSec);

    if( tgl.pClient )
        tgl.pClient->ProcessTime(DeltaSec);

    if( tgl.pServer )
        tgl.pServer->ProcessTime(DeltaSec);
}

//==============================================================================


void sm_UnloadMission( void )
{
    x_strcpy( fegl.MissionName, "" );
    x_strcpy( tgl.MissionName, "" );
    x_strcpy( tgl.MissionDir, "" );

    if( !g_SM.MissionLoaded )
        return;

    MAI_Mgr.UnloadMission();
    UnloadMission();
    KillMissionSystems();

    tgl.pMasterServer->SetServer(FALSE);
    pSvrMgr->SetIsInGame(FALSE);
    pSvrMgr->SetAsServer(FALSE);

    // initialize the bot static scheduling stuff
    InitBotAdvance();

    fegl.MissionName[0] = 0;
}

//==============================================================================
//==============================================================================
//==============================================================================
// STATE TABLE
//==============================================================================
//==============================================================================
//==============================================================================

sm_state s_CommonState[] =
{
    {STATE_NULL,                    NULL,                       NULL,                           NULL},
};

//==============================================================================
