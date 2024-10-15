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
#include "titles.hpp"
#include "cardmgr/cardmgr.hpp"
#include "masterserver/masterserver.hpp"
#include "savedgame.hpp"
#include "FrontEnd.hpp"
#include "ui\ui_win.hpp"
#include "ui\ui_manager.hpp"
#include "hud\hud_manager.hpp"
#include "dlg_message.hpp"
#include "dlg_WarriorSetup.hpp"
#include "dlg_MainMenu.hpp"
#include "dlg_Online.hpp"
#include "dlg_Inventory.hpp"
#include "dlg_Score.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "pointlight\pointlight.hpp"
#include "Objects\Bot\MAI_Manager.hpp"
#include "sm_common.hpp"
#include "titles.hpp"
#include "StringMgr\StringMgr.hpp"
#include "Demo1\Data\UI\ui_strings.h"

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
extern sm_state s_CommonState[];
extern sm_state s_ServerState[];
extern sm_state s_CampaignState[];
extern sm_state s_ClientState[];

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
xbool InitFrontEnd( f32 DeltaSec );
xbool UpdateFrontEnd( f32 DeltaSec );

//==============================================================================

void sm_SetState( state_enum StateType )
{
    // This resets all the timers to make sure that when we change a state,
    // all the subsequent advances occur properly after a state change.
    tgl.ActualTime = x_GetTime();
    g_SM.NewState = StateType;
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
        s_CommonState,
        s_CampaignState,
        s_ServerState,
        s_ClientState,
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
            status = net_Bind(tgl.UserInfo.MyAddr,MIN_USER_PORT);
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

#if 0

static
void UpdateIngame( f32 DeltaSec )
{
    (void)DeltaSec;
    s32 i;

    // Move stats to next entry
    if( tgl.pServer )
    {
        tgl.pServer->AdvanceStats();
    }

    // Update objects in database
    tgl.ObjMgrLogicTime.Reset();
    tgl.ObjMgrLogicTime.Start();
    GameMgr.AdvanceTime( DeltaSec );
    if ( tgl.ServerPresent )
    {
        MAI_Mgr.Update(); 
    }
    GameMgr.UpdateSensorNet();
    ObjMgr.AdvanceAllLogic( DeltaSec );
    tgl.ObjMgrLogicTime.Stop();

    for( i = 0; i < tgl.NLocalPlayers; i++ )
    {                                            
        player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( object::id( tgl.PC[i].PlayerIndex, -1 ) );
        ASSERT( pPlayer );
        {
            if( !tgl.UseFreeCam[i] && !pPlayer->GetHasInputFocus() )
            {
                tgl.FreeCam[i] = tgl.PC[i].View;
            }
            tgl.UseFreeCam[i] = !pPlayer->GetHasInputFocus();
        }
    }

    // Advance point light logic
    ptlight_AdvanceLogic( DeltaSec );
/*
    // Update server time
    tgl.ServerProcessTime.Reset();
    tgl.ServerProcessTime.Start();
    if( tgl.ServerPresent )
        tgl.pServer->ProcessTime(DeltaSec);
    tgl.ServerProcessTime.Stop();

    // Update client time
    tgl.ClientProcessTime.Reset();
    tgl.ClientProcessTime.Start();
    if( tgl.ClientPresent )
        tgl.pClient->ProcessTime(DeltaSec);
    tgl.ClientProcessTime.Stop();
*/
    // Update users
    for( i=0; i<MAX_GAME_USERS; i++ )
        if( tgl.pUser[i] )
            tgl.pUser[i]->ProcessTime(DeltaSec);

    // Update HUD
    tgl.pHUDManager->Update( DeltaSec );

    // Update UI
    tgl.pUIManager->Update( DeltaSec );

    // Process ambient sounds
    //UpdateAmbientSounds(DeltaSec);
}

#endif

//==============================================================================
extern xbool g_ForceServerListRefresh;

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
	g_ForceServerListRefresh = 1;

    // initialize the bot static scheduling stuff
    InitBotAdvance();

    fegl.MissionName[0] = 0;
}

//==============================================================================
//==============================================================================
//==============================================================================

void sm_CommonInit_Advance( f32 DeltaSec )
{
    if (InitFrontEnd(DeltaSec))
    {
        sm_SetState(STATE_COMMON_INIT_TITLES);
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
extern s32 g_ClientsConnected;
extern s32 g_ClientsDropped;

void sm_CommonInitFrontEnd_Begin( void )
{
    const xwchar* pTitle   = NULL;
    const xwchar* pMessage = NULL;

    //
    // Interpret how we got back to the frontend
    //
	if (g_SM.NetworkDown)
	{
        pTitle   = StringMgr( "ui", IDS_CONNECTION_LOST );
		pMessage = StringMgr( "ui", IDS_NETWORK_DOWN);
        GotoMainMenuDialog();
	}
    else
	if( g_SM.ClientQuit )
    {
//        pTitle   = StringMgr( "ui", IDS_CLIENT_QUIT );
//        pMessage = StringMgr( "ui", IDS_CLIENT_QUIT_GAME );
    }
    else
    if( g_SM.ServerMissionEnded && !fegl.GameHost )
    {
        if( g_SM.ClientKicked )
        {
            pTitle   = StringMgr( "ui", IDS_CLIENT_KICKED );
            pMessage = StringMgr( "ui", IDS_YOU_WERE_KICKED );
        }
        else if ( g_SM.ServerTooBusy )
		{
			pTitle	= StringMgr( "ui", IDS_CONNECTION_LOST );
			pMessage = StringMgr( "ui", IDS_CONNECTION_TOO_BUSY );
		}
		else

        {
            pTitle   = StringMgr( "ui", IDS_MISSION_ENDED );
            pMessage = StringMgr( "ui", IDS_SERVER_ENDED_MISSION );
        }
    }
    else
    if( g_SM.ServerShutdown && !fegl.GameHost )
    {
        pTitle   = StringMgr( "ui", IDS_CONNECTION_LOST );
        pMessage = StringMgr( "ui", IDS_SERVER_SHUT_DOWN );
    }
    else
    if( g_SM.Disconnected )
    {
        pTitle   = StringMgr( "ui", IDS_CONNECTION_LOST );
        pMessage = StringMgr( "ui", IDS_CONNECTION_WAS_LOST );
    }
    else
	if ( g_SM.ServerTooBusy )
	{
		pTitle	= StringMgr( "ui", IDS_CONNECTION_LOST );
		pMessage = StringMgr( "ui", IDS_CONNECTION_TOO_BUSY );
	}
	else
	if( g_SM.TooManyClients )
	{
		pTitle = StringMgr( "ui", IDS_LOGIN_FAILED );
		pMessage = StringMgr( "ui", IDS_TOO_MANY_CLIENTS );
	}
	else
    if( g_SM.LoginFailed )
    {
        if( g_SM.ClientBanned )
        {
            pTitle   = StringMgr( "ui", IDS_LOGIN_FAILED );
            pMessage = StringMgr( "ui", IDS_LOGIN_BANNED );
        }
        else if( g_SM.InvalidPassword )
        {
            pTitle   = StringMgr( "ui", IDS_LOGIN_FAILED );
            pMessage = StringMgr( "ui", IDS_LOGIN_INVALID_PASSWORD );
        }
        else if( g_SM.ServerFull )
        {
            pTitle   = StringMgr( "ui", IDS_LOGIN_FAILED );
            pMessage = StringMgr( "ui", IDS_LOGIN_SERVER_FULL );
        }
        else
        {
            pTitle   = StringMgr( "ui", IDS_LOGIN_FAILED );
            pMessage = StringMgr( "ui", IDS_LOGIN_WAS_UNSUCCESSFUL );
        }
    }
    else
    if( g_SM.CampaignMissionEnded )
    {
        // TODO: Mark mission complete and advance to next mission
    }
    else
    {
    }
	// BW 8/3/02 - Comment this in to make the server display a message should a significant number
	// of the clients that got connected to that server were eventually dropped due to network
	// connection overload.
	if ( (pTitle == NULL) && 
		 (g_ClientsConnected >= 4) && 
		 (((f32)g_ClientsDropped / (f32)g_ClientsConnected) >= 0.5f) )
	{
		pTitle	= StringMgr("ui",IDS_WARNING);
		(const xwchar*)xwstring("WARNING!");
		pMessage = StringMgr("ui",IDS_MAXCLIENTS_WARNING);
	}


    // Show message dialog
    if( pTitle )
    {
        dlg_message* pDialog = (dlg_message *)tgl.pUIManager->OpenDialog( fegl.UIUserID, "message", irect(0,0,300,200), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER );
        ASSERT( pDialog );
        pDialog->Configure( pTitle,
                            NULL,
                            StringMgr( "ui", IDS_CONTINUE ),
                            pMessage,
                            XCOLOR_WHITE );
    }

    // Clear special states
    g_SM.MissionRunning       = FALSE;
    g_SM.MissionLoading       = FALSE;
    g_SM.ClientKicked         = FALSE;
    g_SM.ClientQuit           = FALSE;
    g_SM.ServerMissionEnded   = FALSE;
    g_SM.ServerShutdown       = FALSE;
    g_SM.Disconnected         = FALSE;
    g_SM.CampaignMissionEnded = FALSE;
    g_SM.LoginFailed          = FALSE;
	g_SM.ServerTooBusy		  = FALSE;
	g_SM.TooManyClients		  = FALSE;
    g_SM.MissionLoaded        = FALSE;
	g_SM.NetworkDown		  = FALSE;
    g_SM.ClientBanned         = FALSE;
    g_SM.InvalidPassword      = FALSE;
    g_SM.ServerFull           = FALSE;

	g_ClientsDropped		  = 0;
	g_ClientsConnected		  = 0;
}

//==============================================================================

void sm_CommonInitFrontEnd_Advance( f32 DeltaSec )
{
    if( InitFrontEnd(DeltaSec) )
    {
        sm_SetState(STATE_COMMON_FRONTEND);
		tgl.pUIManager->EnableUser(fegl.UIUserID,TRUE);

        #if defined(TARGET_PS2_DEV) && defined(bwatson)
		extern void ps2_GetFreeMem(s32 *,s32*,s32 *);
		s32 Free,Largest,Fragments;

		ps2_GetFreeMem(&Free,&Largest,&Fragments);
		x_DebugMsg("sm_CommonInitFrontEnd_Advance: %d bytes free, %d largest, %d fragments\n",Free,Largest,Fragments);
        if( x_fopen_log )
        {
            x_fopen_log->SaveFile( "cdfs_perm.txt" );
            x_fopen_log->Clear();
        }
        #endif
    }
}

//==============================================================================
//==============================================================================
//==============================================================================

void sm_CommonFrontEnd_Advance( f32 DeltaSec )
{
    if (!tgl.MusicId)
    {
        tgl.MusicId = audio_Play(MUSIC_FRONTEND);
    }


    if( UpdateFrontEnd( DeltaSec ) )
    {
        tgl.Playing     = TRUE;
        tgl.NLocalPlayers = 0;

		g_ClientsConnected = 0;
		g_ClientsDropped   = 0;
        if( tgl.UserInfo.IsServer )
        {
            if( fegl.GameType == GAME_CAMPAIGN )
                sm_SetState(STATE_CAMPAIGN_INIT_CAMPAIGN);
            else
                sm_SetState(STATE_SERVER_INIT_SERVER);
        }
        else
        {
            sm_SetState(STATE_CLIENT_INIT_CLIENT);
        }
    }
}

//==============================================================================

void sm_CommonFrontEnd_End( void )
{
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
    {STATE_COMMON_INIT,             NULL,                       sm_CommonInit_Advance,          NULL},
    {STATE_COMMON_INIT_TITLES,      title_Init_Begin,           title_Init_Advance,             NULL},
    {STATE_COMMON_INIT_FRONTEND,    sm_CommonInitFrontEnd_Begin,sm_CommonInitFrontEnd_Advance,  NULL},
    {STATE_COMMON_FRONTEND,         NULL,                       sm_CommonFrontEnd_Advance,      sm_CommonFrontEnd_End},
    {STATE_NULL,                    NULL,                       NULL,                           NULL},
};

//==============================================================================
