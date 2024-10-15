//==============================================================================
//
//  event_Time.cpp
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
#include "Sky\sky.hpp"
#include "ObjectMgr\ObjectMgr.hpp"
#include "Objects\PlaceHolder\PlaceHolder.hpp"
#include "Objects\Terrain\Terrain.hpp"
#include "Objects\Player\PlayerObject.hpp"
#include "Objects\Projectiles\Plasma.hpp"
#include "Objects\Projectiles\Bullet.hpp"
#include "Objects\Projectiles\Laser.hpp"
#include "Objects\Pickups\pickups.hpp"
#include "Objects\Projectiles\Flag.hpp"
#include "Objects\Projectiles\Blaster.hpp"
#include "Objects\Projectiles\Station.hpp"
#include "Objects\Projectiles\Bounds.hpp"
#include "Objects\vehicles\gravcycle.hpp"
#include "Objects\vehicles\shrike.hpp"
#include "Objects\vehicles\asstank.hpp"
#include "Objects\vehicles\transport.hpp"
#include "Objects\vehicles\bomber.hpp"
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
#include "GameMgr\MsgMgr.hpp"
#include "pointlight\pointlight.hpp"

#include "Objects\Bot\MAI_Manager.hpp"
#include "sm_common.hpp"

extern xstring* x_fopen_log;


//#define EVENT_TIMERS

//==============================================================================
//  LOCAL STORAGE
//==============================================================================

server_manager* pSvrMgr = NULL;

//==============================================================================
// PROTOTYPES
//==============================================================================

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

//==============================================================================

xbool InitServer( f32 DeltaSec )
{
    (void)DeltaSec;
    tgl.pServer = new game_server;
    tgl.ServerPresent = TRUE;
    tgl.pServer->InitNetworking(tgl.UserInfo.MyAddr, tgl.UserInfo.Server);
    return TRUE;
}

//==============================================================================

xbool InitClient( f32 DeltaSec, const s32 ClientIndex )
{
    if( tgl.pClient == NULL )
    {
        tgl.pClient = new game_client;
        tgl.ClientPresent = TRUE;
        tgl.pClient->InitNetworking(tgl.UserInfo.MyAddr,
                                    tgl.UserInfo.ServerAddr,
                                    tgl.UserInfo.Server,
                                    tgl.UserInfo.MyName,
									ClientIndex);
    }

    tgl.pClient->ProcessTime(DeltaSec);
    fegl.MissionName[0] = '\0';

    return TRUE;
}

//==============================================================================

xbool KillServer( f32 DeltaSec )
{
    // Tell all clients server is shutting down
    
    // Deallocate server
    delete tgl.pServer;

    tgl.pServer = NULL;
    tgl.ServerPresent = FALSE;

    (void)DeltaSec;
    return TRUE;
}

//==============================================================================

xbool KillClient( f32 DeltaSec )
{
    // Tell server we are disconnecting
    
    // Deallocate client
    delete tgl.pClient;

    tgl.pClient = NULL;
    tgl.ClientPresent = FALSE;

    (void)DeltaSec;
    return TRUE;
}

//==============================================================================
/*
void UpdateAmbientSounds(f32 DeltaSec)
{
    static f32 AmbientDelay = x_frand(25.0f,120.0f);
    s32 EffectId;
    s32 index;
    f32 volume;
    f32 pitch;
    
    s32     GustIdTable[6]=
    {
        SFX_AMBIENT_WIND_GUST01         ,
        SFX_AMBIENT_WIND_GUST02         ,
        SFX_AMBIENT_WIND_GUST03         ,
        SFX_AMBIENT_WIND_GUST04         ,
        SFX_AMBIENT_WIND_GUST05         ,
        SFX_AMBIENT_WIND_GUST06         ,
    };

    AmbientDelay -=DeltaSec;
    if (AmbientDelay < 0.0f)
    {
        AmbientDelay = x_frand(20.0f,60.0f);
        index = x_irand(0,5);
        volume = x_frand(0.29f,0.50f);
        pitch  = x_frand(0.6f,1.4f);
        EffectId = audio_Play( GustIdTable[ index ] );
        audio_SetVolume( EffectId, volume);
        audio_SetPitch ( EffectId, pitch);
//#ifdef mschaefgen
//        x_printf ("Wind # %d,delay=%.2f, vol=%.2f,pitch=%.2f\n", index,AmbientDelay,volume,pitch);
//#endif
    }                                       
}
*/
//==============================================================================
extern void ProcessPlayerPackets( xbool Flush );

// Make sure players are in sync with vehicles
void UpdateVehiclePlayers( void )
{
    s32             i,j ;
    vehicle*        pVehicle ;
    player_object*  pPlayer ;

    for (i = object::TYPE_VEHICLE_START ; i <= object::TYPE_VEHICLE_END ; i++)
    {
        ObjMgr.StartTypeLoop((object::type)i) ;
        while((pVehicle = (vehicle*)ObjMgr.GetNextInType()))
        {
            ASSERT(pVehicle->GetAttrBits() & object::ATTR_VEHICLE) ;
            pVehicle->UpdateInstances() ;
            for (j = 1 ; j < pVehicle->GetNumberOfSeats() ; j++)
            {
                pPlayer = pVehicle->GetSeatPlayer(j) ;
                if (pPlayer)
                    pPlayer->VehicleMoved(pVehicle) ;
            }
        }
        ObjMgr.EndTypeLoop() ;
    }
}





#ifdef EVENT_TIMERS
xtimer STAT_PtLightLogic;
xtimer STAT_ServerLogic;
xtimer STAT_ClientLogic;
xtimer STAT_MiscLogic;
xtimer STAT_ObjMgrLogic;
xtimer STAT_MAI;
xbool SHOW_MASTERAI_TIME=FALSE;
#endif

void UpdateIngame( f32 DeltaSec )
{
    s32 i;

    // Move stats to next entry
    if( tgl.pServer )
    {
        tgl.pServer->AdvanceStats();
    }

#ifdef EVENT_TIMERS
    xtimer MAI;
#endif

    // Update objects in database
#ifdef EVENT_TIMERS
    tgl.ObjMgrLogicTime.Reset();
    tgl.ObjMgrLogicTime.Start();
    STAT_ObjMgrLogic.Start();
#endif

#ifdef EVENT_TIMERS
    STAT_MAI.Start();
    MAI.Start();
#endif
    if ( tgl.ServerPresent )
    {
        MAI_Mgr.Update(); 
    }
#ifdef EVENT_TIMERS
    MAI.Stop();
    STAT_MAI.Stop();
#endif

    bounds_Update( DeltaSec );

    // Network logic - must be before game logic so that players are in sync with vehicles!

    ProcessPlayerPackets( FALSE );

    // Update users
    for( i=0; i<MAX_GAME_USERS; i++ )
        if( tgl.pUser[i] )
            tgl.pUser[i]->ProcessTime(DeltaSec);

    // Process receieved moves that have been queued
    if (tgl.ServerPresent)
        ProcessReceivedMoves() ;


    GameMgr.UpdateSensorNet();

    ObjMgr.AdvanceAllLogic( DeltaSec );


#ifdef EVENT_TIMERS
    STAT_ObjMgrLogic.Stop();
    tgl.ObjMgrLogicTime.Stop();
#endif

#ifdef EVENT_TIMERS
    if( SHOW_MASTERAI_TIME )
    {
        x_DebugMsg("MAI: %5.2f\n",MAI.ReadMs());
    }
#endif

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
#ifdef EVENT_TIMERS
    STAT_PtLightLogic.Start();
#endif
    ptlight_AdvanceLogic( DeltaSec );
#ifdef EVENT_TIMERS
    STAT_PtLightLogic.Stop();
#endif

    // Update server time
/*
    tgl.ServerProcessTime.Reset();
    tgl.ServerProcessTime.Start();
    STAT_ServerLogic.Start();
    if( tgl.ServerPresent )
        tgl.pServer->ProcessTime(DeltaSec);
    STAT_ServerLogic.Stop();
    tgl.ServerProcessTime.Stop();

    // Update client time
    tgl.ClientProcessTime.Reset();
    tgl.ClientProcessTime.Start();
    STAT_ClientLogic.Start();
    if( tgl.ClientPresent )
        tgl.pClient->ProcessTime(DeltaSec);
    STAT_ClientLogic.Stop();
    tgl.ClientProcessTime.Stop();
*/
#ifdef EVENT_TIMERS
    STAT_MiscLogic.Start();
#endif

    // Make sure players are in sync with vehicles
    UpdateVehiclePlayers() ;

    // Update HUD
    tgl.pHUDManager->Update( DeltaSec );

    // Update UI
    tgl.pUIManager->Update( DeltaSec );

    // Process ambient sounds
    //UpdateAmbientSounds(DeltaSec);

    MsgMgr.AdvanceLogic( DeltaSec );
    GameMgr.AdvanceTime( DeltaSec );

#ifdef EVENT_TIMERS
    STAT_MiscLogic.Stop();
#endif
}

//==============================================================================
/*
static
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
}
*/
//==============================================================================
/*
static char* MissionPrefix[] = 
{
    "",
    "CTF-",
    "CNH-",
    "TDM-",
    "DM-",
    "HUNT-",
    "SOLO-",
};
*/
//==============================================================================

xbool DecideMission( f32 DeltaSec )
{
    (void)DeltaSec;

    if( tgl.ServerPresent )
    {           
        x_strcpy ( tgl.MissionName, fegl.MissionName );
        x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
        x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
        GameMgr.SetGameType( (game_type)fegl.GameType, fegl.CampaignMission );
        AddServerPlayers();
        return TRUE;
    }

    if( tgl.ClientPresent )
    {
        if( fegl.MissionName[0] )
        {
            x_strcpy ( tgl.MissionName, fegl.MissionName );
            x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
            x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
            GameMgr.SetGameType( (game_type)fegl.GameType );
            return TRUE;
        }

        // Send a mission request every second
        if( (tgl.NRenders%30)==0 )
            tgl.pClient->RequestMission();
    }

    return FALSE;
}

//==============================================================================

xbool NextMission( f32 DeltaSec )
{
    (void)DeltaSec;

    if( tgl.ServerPresent )
    {           
        AdvanceMission();
        x_strcpy ( tgl.MissionName, fegl.MissionName );
        x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
        x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
        GameMgr.SetGameType( (game_type)fegl.GameType, fegl.CampaignMission );
        return TRUE;
    }

    if( tgl.ClientPresent )
    {
        if( fegl.MissionName[0] )
        {
            x_strcpy ( tgl.MissionName, fegl.MissionName );
            x_sprintf( tgl.MissionDir, "Data/Missions/%s", fegl.MissionName );
            x_strcpy ( tgl.MissionPrefix, MissionPrefix[ fegl.GameType ] );
            GameMgr.SetGameType( (game_type)fegl.GameType );
            return TRUE;
        }

        // Send a mission request every second
        if( (tgl.NRenders%30)==0 )
            tgl.pClient->RequestMission();
    }

    return FALSE;
}

//==============================================================================

xbool SyncWithServer( f32 DeltaSec )
{
    if( tgl.ClientPresent )
    {
        //x_DebugMsg("SyncWithServer %f\n",DeltaSec);

        if( tgl.pClient->IsInSync() )
            return TRUE;

        //tgl.pClient->ProcessHeartbeat( DeltaSec );
        tgl.pClient->ProcessTime(DeltaSec);
    }

    (void)DeltaSec;
    return FALSE;
}

//==============================================================================

void event_Time( f32 DeltaSec )
{
    // Freeze time if doing a multi-part screen shot
    if (eng_ScreenShotActive())
        return ;

    sm_Time( DeltaSec );
}

//==============================================================================

