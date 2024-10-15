//==============================================================================
//
//  Main.cpp
//
//==============================================================================

//#define SHOW_DMT_STUFF

//==============================================================================
//  INCLUDES
//==============================================================================

#if defined(TARGET_PC)
#include <process.h>
#include <conio.h>
#endif

#include "Entropy.hpp"
#include "NetLib\BitStream.hpp"
#include "Globals.hpp"
#include "fe_Globals.hpp"
#include "GameServer.hpp"
#include "GameUser.hpp"
#include "GameClient.hpp"
#include "Tokenizer\Tokenizer.hpp"
#include "AudioMgr\Audio.hpp"
#include "Building\BuildingObj.hpp"
#include "Objects\Terrain\Terrain.hpp"
#include "ServerMan.hpp"
#include "PointLight\PointLight.hpp"
#include "e_NetFS.hpp"
#include "Support\Objects\Bot\Graph.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "GameMgr\GameMgr.hpp"
#include "GameMgr\LogMgr.hpp"
#include "Objects\Projectiles\Plasma.hpp"
#include "Objects\Projectiles\Bullet.hpp"
#include "Objects\Projectiles\Laser.hpp"
#include "Objects\Projectiles\Blaster.hpp"
#include "Objects\Projectiles\Bounds.hpp"
#include "Objects\Bot\Graph.hpp"
#include "StringMgr\StringMgr.hpp"

#include "data\ui\ui_strings.h"

#include "ui\ui_manager.hpp"
#include "hud\hud_manager.hpp"
#include "masterserver/masterserver.hpp"
#include "serverversion.hpp"
#include "sm_common.hpp"
#include "AADS/SpecialVersion.hpp"
#include "telnetconsole/telnetmgr.hpp"
#include "telnetconsole/chatclient.hpp"
#include "Common/e_cdfs.hpp"

#define EXPORT_MISSIONS     0   // set this to 1 to export all missions
#define TEST_PATHFIND       0

#define EXPORT_NAV_GRAPHS   0
#define VALIDATE_ASS_FILES  0
#define LIGHT_LEVELS        0   // set this to 1 to light all buildings when exporting all missions

//=========================================================================
//  Build String
//=========================================================================

char s_BuildString[] = "AADS";

//==============================================================================
//  GLOBAL STORAGE
//==============================================================================

#ifdef TARGET_PS2_CLIENT
xbool DO_SCREEN_SHOTS = TRUE;
#else
xbool DO_SCREEN_SHOTS = FALSE;
#endif

#if defined(athyssen)
volatile xbool PRINT_ENGINE_STATS = FALSE;
#else
volatile xbool PRINT_ENGINE_STATS = FALSE;
#endif

#if defined(athyssen)
volatile xbool SHOW_FRAME_RATE = FALSE;
#else
volatile xbool SHOW_FRAME_RATE = FALSE;
#endif

#if defined(athyssen) || defined(bwatson) || defined(acheng) || defined(mreed)
volatile xbool SHOW_MAIN_TIMINGS = FALSE;
#else
volatile xbool SHOW_MAIN_TIMINGS = FALSE;
#endif

#if defined(acheng) || defined(mreed)
volatile xbool SHOW_BOT_TIMINGS = TRUE;
#else
volatile xbool SHOW_BOT_TIMINGS = FALSE;
#endif

static s32 NFrames = 0;
static xbool s_PingDisplayEnabled = FALSE;

volatile xcolor SHAPE_AMBIENT_COLOR = xcolor(200,200,200,255);
volatile xcolor SHAPE_LIGHT_COLOR   = xcolor(180,180,180,255);

extern xstring* x_fopen_log;
extern graph g_Graph;
extern xbool TagPlayers;

//==============================================================================

//extern ui_manager*      pFrontEnd;
extern server_manager*  pSvrMgr;

void KillFrontEnd( void );
void InitWarriorSetup( warrior_setup& Setup, s32 iWarrior );

//==============================================================================
//  PROTOTYPES
//==============================================================================
void LoadPermanentData( void );
void UnloadPermanentData( void );
//==============================================================================
//  EVENT HANDLING FUNCTION DECLARATIONS
//==============================================================================

void event_Time     ( f32 DeltaMs );
void event_Input    ( void );
void event_Packet   ( void );

void ExportMissions( void );
//=============================================================================

//==============================================================================
//  HELPER FUNCTIONS
//==============================================================================

void UpdateEventString( char C )
{
    tgl.EventString[tgl.EventStringIndex] = C;
    tgl.EventString[(tgl.EventStringIndex+1)%32] = ' ';
    tgl.EventStringIndex = (tgl.EventStringIndex+1)%32;
};

//==============================================================================
//  EVENT PUMPS
//==============================================================================

void pump_ProcessInput( void )
{
    if( tgl.ExitApp )
        return;

    // Check if enough time has gone by to check input
    //if( (tgl.LogicTimeMs - tgl.LastInputTime) >= tgl.EventRateInput )
    {
        tgl.LastInputTime += tgl.EventRateInput;

        input_UpdateState();

//        while( input_UpdateState() )
        /*
        {
            // Make call to input users
            UpdateEventString('I');
            tgl.EventInputCount++;
        }
        */

        event_Input();

        // Short circuit if trying to shut down
        //if( tgl.DoShutdown ) return;
    }
}

//==============================================================================

s32     STAT_NServerPackets;
s32     STAT_NClientPackets;
xtimer  STAT_ServerPacketTime;
xtimer  STAT_ClientPacketTime;
xtimer  STAT_NetReceiveTime;
xtimer  STAT_MasterPacketTime;

struct chunk
{
    byte PAD[16];
} PS2_ALIGNMENT(16);

//------------------------------------------------------------------------------

void pump_ProcessPackets( void )
{
    s32 PacketType;
    xbool ok;

    if( tgl.ExitApp )
        return;

    // Check if enough time has gone by to check input
    //if( (tgl.LogicTimeMs - tgl.LastPacketTime) > tgl.EventRatePacket )
    {
        tgl.LastPacketTime = tgl.LogicTimeMs;

        UpdateEventString('P');
        tgl.EventPacketCount++;

        // Dispatch all the packets
        bitstream BitStream;
        chunk Chunk[1024/16];
        byte* Buffer = (byte*)Chunk;
        
        // Grab all server packets
        if( tgl.ServerPresent )
        {
            s32 Count=128;

            net_address ServerAddr = tgl.pServer->GetAddress();
            net_address SenderAddr;
                
            //x_DebugMsg("####### checking for server packets\n");
            while( Count-- )
            {
                STAT_NetReceiveTime.Reset();
                STAT_ServerPacketTime.Reset();
                STAT_MasterPacketTime.Reset();

                s32 BufferSize = sizeof(Chunk);

                // If we haven't been assigned an IP address yet, let's just bail
                if (!ServerAddr.IP)
                    break;
                STAT_NetReceiveTime.Start();
                xbool Result = net_Receive( ServerAddr, SenderAddr, Buffer, BufferSize );
                STAT_NetReceiveTime.Stop();
                if(!Result)
                    break;

                // Since we are not using the bitstream version of receive (EVER) then we
                // can perform the checksum test here when the packet comes in. If the
                // checksum fails, then it's most likely something for the master
                // server code and the master server code can deal with that.
                if ((u8)net_Checksum(Buffer,BufferSize-1) != (u8)Buffer[BufferSize-1])
                {
                    BitStream.Init(Buffer,BufferSize-1);
                    BitStream.ReadS32( PacketType );
                    if ((PacketType & 0xffff0000) == (CONN_PACKET_TYPE_LOGIN & 0xffff0000) ) 
                    {
                        x_DebugMsg("pump_ProcessPackets: Bad checksum came in on a bitstream\n");
                    }
                    STAT_MasterPacketTime.Start();
                    ASSERT(tgl.pMasterServer);
                    tgl.pMasterServer->ProcessPacket(Buffer,BufferSize,ServerAddr,SenderAddr);
                    STAT_MasterPacketTime.Stop();
                }
                else
                {
                    // Remove the checksum byte which will be on the end of the packet
                    BufferSize--;

                    BitStream.Init(Buffer,BufferSize);
                    BitStream.ReadS32( PacketType );
                    if ((PacketType & 0xffff0000) == (CONN_PACKET_TYPE_LOGIN & 0xffff0000) ) 
                    {
                        ok = net_Decrypt(Buffer,BufferSize,ENCRYPTION_KEY);
                        if (!ok)
                        {
                            x_DebugMsg( "****\n" );
                            x_DebugMsg( "**** PACKET DID NOT DECRYPT ON SERVER\n" );
                            x_DebugMsg( "****\n" );
                            break;
                        }
                        STAT_NServerPackets++;
                        STAT_ServerPacketTime.Start();
                        BitStream.Init(Buffer,BufferSize);
                        tgl.pServer->ProcessPacket( BitStream, SenderAddr );
                        //x_DebugMsg("********* PUMP RECEIVED PACKET\n");
                        STAT_ServerPacketTime.Stop();
                    }
                    else
                    {
                        STAT_MasterPacketTime.Start();
                        ASSERT(tgl.pMasterServer);
                        tgl.pMasterServer->ProcessPacket(Buffer,BufferSize,ServerAddr,SenderAddr);
                        STAT_MasterPacketTime.Stop();
                    }

                    #ifdef SHOW_DMT_STUFF
                    x_DebugMsg( "TIMERS -- RECEIVE(%g) PROCESS(%g) MASTER(%g)\n",
                                STAT_NetReceiveTime.ReadMs(), 
                                STAT_ServerPacketTime.ReadMs(),
                                STAT_MasterPacketTime.ReadMs() );
                    #endif
                }
            }
        }       

        // Grab all client packets
        if( tgl.ClientPresent )
        {
            s32 Count=128;

            net_address ClientAddr = tgl.pClient->GetAddress();
            net_address SenderAddr;

            while( Count-- )
            {
                s32 BufferSize = sizeof(Chunk);

                // If we haven't been assigned an IP address yet, let's just bail
                if (!ClientAddr.IP)
                    break;
                STAT_NetReceiveTime.Start();
                xbool Result = net_Receive( ClientAddr, SenderAddr, Buffer, BufferSize );
                STAT_NetReceiveTime.Stop();
                if(!Result)
                    break;

                if ((u8)net_Checksum(Buffer,BufferSize-1) != (u8)Buffer[BufferSize-1])
                {
                    BitStream.Init(Buffer,BufferSize-1);
                    BitStream.ReadS32( PacketType );
                    if ((PacketType & 0xffff0000) == (CONN_PACKET_TYPE_LOGIN & 0xffff0000) ) 
                    {
                        x_DebugMsg("pump_ProcessPackets: Bad checksum came in on a bitstream\n");
                    }
                    STAT_MasterPacketTime.Start();
                    ASSERT(tgl.pMasterServer);
                    tgl.pMasterServer->ProcessPacket(Buffer,BufferSize,ClientAddr,SenderAddr);
                    STAT_MasterPacketTime.Stop();
                }
                else
                {
                    BufferSize--;
                    BitStream.Init(Buffer,BufferSize);
                    BitStream.ReadS32( PacketType );
                    if ( (PacketType & 0xffff0000) == (CONN_PACKET_TYPE_LOGIN & 0xffff0000) ) 
                    {
                        ok = net_Decrypt(Buffer,BufferSize,ENCRYPTION_KEY);
                        if (!ok)
                        {
                            x_DebugMsg( "****\n" );
                            x_DebugMsg( "**** PACKET DID NOT DECRYPT ON CLIENT\n" );
                            x_DebugMsg( "****\n" );
                            break;
                        }
                        STAT_NClientPackets++;
                        STAT_ClientPacketTime.Start();
                        BitStream.Init(Buffer,BufferSize);
                        tgl.pClient->ProcessPacket( BitStream, SenderAddr );
                        STAT_ClientPacketTime.Stop();
                    }
                    else
                    {
                        ASSERT(tgl.pMasterServer);
                        tgl.pMasterServer->ProcessPacket(Buffer,BufferSize,ClientAddr,SenderAddr);
                    }
                }
            }
        }       
        // If we don't have a server or client, then we MUST be in the front end
        // so we make sure we pass on ANY packets on to the master server manager
        if (!tgl.ServerPresent && !tgl.ClientPresent )
        {
            xbool       Status;
            net_address Sender;
            s32         BufferSize;

            if (tgl.UserInfo.MyAddr.IP)
            {
                s32 i;
                for (i=0;i<96;i++)
                {
                    Status = net_Receive( tgl.UserInfo.MyAddr,Sender,Buffer,BufferSize);
                    if (Status)
                    {
                        tgl.pMasterServer->ProcessPacket(Buffer,BufferSize,tgl.UserInfo.MyAddr,Sender);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}

//==============================================================================

s32 STAT_NTimeLoops;
xbool SHOW_TIME_INFO = FALSE;
s32   FRAME_RATE_SAMPLES = 3 ;

//------------------------------------------------------------------------------

void pump_ProcessTime( void )
{
    static s32 Entry=0;
    static s32 EntryData[32];

    ASSERT(FRAME_RATE_SAMPLES >= 1) ;
    ASSERT(FRAME_RATE_SAMPLES <= 32) ;

    if( tgl.ExitApp )
        return;

    // Decide how much time has passed
    xtick NewActualTime     = x_GetTime();
    xtick TimeDelta         = NewActualTime - tgl.ActualTime;
    s32   TicksPerMs        = (s32)x_GetTicksPerMs();
    s32   MaxTicksPerStep   = TicksPerMs*tgl.MaxLogicStepSize;
    s32   MinTicksPerStep   = TicksPerMs*tgl.MinLogicStepSize;;
    s32   nCycles = 0;

//  x_DebugMsg( ".\n---DeltaTime:%02dms\n",  
//              TimeDelta / TicksPerMs );

    if (TimeDelta < 0)
    {
        TimeDelta = 0;
    }
    // Jump forward if we need to
    if( TimeDelta > (TicksPerMs*1000) )
    {
        TimeDelta = (TicksPerMs*100);
        tgl.ActualTime = NewActualTime - (TicksPerMs*100);
        //x_DebugMsg( "---JUMP\n" );
        //x_DebugMsg( "---DeltaTime:%02dms\n",  
        //            TimeDelta / TicksPerMs );
    }

    if( tgl.JumpTime )
    {
        TimeDelta = MinTicksPerStep+1;
        tgl.JumpTime = FALSE;
    }

    // *SB* - I changed this to only allow 1 update per game cycle or we cascade!
    //        and slow the game down even more...
    // (something maybe wrong with this code as nCycles quite regularly equals 2 even
    //  through the max time is 1 second)
    //while( TimeDelta > MinTicksPerStep )
    if ( TimeDelta > MinTicksPerStep )
    {
        nCycles++;
        s32 StepTicks = (s32)MIN(MaxTicksPerStep,TimeDelta);
        s32 StepMs    = (s32)(0.5f + ((f32)StepTicks/(f32)TicksPerMs)) ; // Round to nearest ms

//      x_DebugMsg( " - True Step:%02dms\n", StepMs );

        // Put desired in buffer
        EntryData[Entry] = StepMs;
        Entry = (Entry+1) % FRAME_RATE_SAMPLES ;

        // Compute average to step with
        StepMs = 0;
        for( s32 i=0; i < FRAME_RATE_SAMPLES; i++ )
            StepMs += EntryData[i];
        StepMs /= FRAME_RATE_SAMPLES ;

//      x_DebugMsg( " - Avg. Step:%02dms\n", StepMs );

        if( SHOW_TIME_INFO )
            x_printf("StepMs %1d\n",StepMs);

        // Update game logic time
        tgl.LogicTimeMs += StepMs;

        UpdateEventString('T');
        tgl.EventTimeCount++;
        STAT_NTimeLoops++;
        tgl.DeltaLogicTime = StepMs * 0.001f;
        event_Time( tgl.DeltaLogicTime );

//      TimeDelta -= StepTicks;
        TimeDelta -= StepMs * TicksPerMs;
        tgl.ActualTime += StepTicks;

        if( tgl.JumpTime )
        {
            TimeDelta = MinTicksPerStep-1;
            tgl.JumpTime = FALSE;
        }

//      x_DebugMsg( "---RemainingDeltaTime:%02dms\n",  
//                  TimeDelta / TicksPerMs );
    }

    //x_DebugMsg("Loops:%d\n", nCycles) ;


    if( SHOW_TIME_INFO )
    {
        for( s32 i=0; i < FRAME_RATE_SAMPLES ; i++ )
        {
            x_printfxy( (i/8)*3, (i%8)+2, "%3d", EntryData[i] );
        }
    }
}

//==============================================================================

static xtick StartTime;

void TribesCrashMessageBuilder(char *pBuff,s32 Length)
{
    (void)Length;
    // pBuff is a free format string (currently 64 byte buffer)
    // which can contain extra context information about the
    // current crash to be sent back to the exception catcher
    u32 SerialNumber = x_chksum( &StartTime, sizeof(xtick) );
    x_sprintf( pBuff, "%08X - %s", SerialNumber, sm_GetState() );
}

//==============================================================================
#include "stdio.h"
void InitEngine( void )
{
    eng_Init();

    StartTime = x_GetTime();
    x_DebugSetVersionString( xfs( "%s, %s, %s", s_BuildString, __DATE__, __TIME__ ) );
    x_DebugSetCrashFunction( TribesCrashMessageBuilder );

    audio_Init();
    net_Init();
}

//==============================================================================

void KillEngine( void )
{
    net_Kill();
    audio_Kill();
    eng_Kill();
}

//==============================================================================

void InitT2Globals( void )
{
    // Load the some string tables.
    StringMgr.LoadTable( "ui",       "data\\ui\\ui_strings.bin" );
    StringMgr.LoadTable( "Messages", "data\\ui\\Messages.bin" );
    StringMgr.LoadTable( "Pickups",  "data\\ui\\Pickups.bin" );

    //tgl.GameStage         = GAME_STAGE_INIT;

    // SB - Make sure the player defines are loaded first, since the front end will 
    //      be copying the loadouts when it gets initialized!
    player_object::LoadDefines() ;

    // Setup time
    tgl.LogicTimeMs       = 0;
    tgl.DeltaLogicTime    = 0;
    tgl.LastRenderTime    = 0;
    tgl.LastInputTime     = 0;
    tgl.LastPacketTime    = 0;
    tgl.ActualStartTime   = x_GetTime();
    tgl.ActualTime        = tgl.ActualStartTime;
    tgl.TicksPerMs        = (s32)x_GetTicksPerMs();

    // Set rates
    tgl.EventRateRender    = 0;
    tgl.EventRateInput     = 0;
    tgl.EventRatePacket    = 0;
    tgl.MinLogicStepSize   = 16;
    tgl.MaxLogicStepSize   = 68;

    // Clear counters
    tgl.EventTimeCount      = 0;
    tgl.EventInputCount     = 0;
    tgl.EventRenderCount    = 0;
    tgl.EventPacketCount    = 0;
 
    // Clear random
    tgl.Random.srand(13);
        
    tgl.HeartbeatDelay = (1.0f/2.0f);
    tgl.ServerPacketShipDelay = (1.0f / 10.0f);
    tgl.LastNetStatsResetTime = 0;
    tgl.ClientPacketShipDelay = (1.0f / 10.0f);

    tgl.Connected     = FALSE;
    tgl.ServerPresent = FALSE;
    tgl.ClientPresent = FALSE;
    tgl.UserPresent = FALSE;
    tgl.NUsers = 0;
    for( s32 i=0; i<MAX_GAME_USERS; i++ )
        tgl.pUser[i] = NULL;
    tgl.pServer = NULL;
    tgl.pClient = NULL;

    tgl.ConnManagerTimeout = 1000*15;

    tgl.UserInfo.UsingConfig = FALSE;

    // Clear stats collection
    tgl.DisplayStats = FALSE;
    tgl.LogStats = FALSE;
    x_memset(tgl.EventString,0,sizeof(tgl.EventString));
    tgl.EventStringIndex = 0;

    tgl.DisplayObjectStats = FALSE;

    tgl.DoRender = TRUE;
    tgl.RenderBuildingGrid = FALSE;

    tgl.NLocalPlayers = 0;

    // Set up the 4 views.

    tgl.UseFreeCam[0] = FALSE;
    tgl.UseFreeCam[1] = FALSE;
    
    // DEDICATED_SERVER
    tgl.UseFreeCam[0] = TRUE;
    // DEDICATED_SERVER

    tgl.FreeCam[0].SetXFOV( R_70 );
    tgl.FreeCam[0].SetPosition( vector3(423.7f, 201.9f, 178.4f) );
    tgl.FreeCam[0].SetRotation( radian3(0.2573f, -0.4316f, 0) );
    tgl.FreeCam[0].SetZLimits ( 0.1f, 1000.0f );

    tgl.FreeCam[1].SetXFOV( R_70 );
    tgl.FreeCam[1].SetPosition( vector3(423.7f, 201.9f, 178.4f) );
    tgl.FreeCam[1].SetRotation( radian3(0.2573f, -0.4316f, 0) );
    tgl.FreeCam[1].SetZLimits ( 0.1f, 1000.0f );

    tgl.PC[0].View.SetXFOV( R_70 );                               
    tgl.PC[0].View.SetPosition( vector3(423.7f, 201.9f, 178.4f) );
    tgl.PC[0].View.SetRotation( radian3(0.2573f, -0.4316f, 0) );  
    tgl.PC[0].View.SetZLimits ( 0.1f, 1000.0f );                  

    tgl.PC[1].View.SetXFOV( R_70 );                               
    tgl.PC[1].View.SetPosition( vector3(423.7f, 201.9f, 178.4f) );
    tgl.PC[1].View.SetRotation( radian3(0.2573f, -0.4316f, 0) );  
    tgl.PC[1].View.SetZLimits ( 0.1f, 1000.0f );                  

    tgl.NSpawnPoints = 0;

    tgl.NRenders = 0;
    tgl.MissionName[0] = '\0';
    tgl.MissionDir [0] = '\0';

    tgl.ExitApp     = FALSE;
    tgl.Playing     = FALSE;
    tgl.GameRunning = FALSE;
    tgl.GameActive  = FALSE;

    tgl.Round = 0;

    tgl.NBots = 0;

    tgl.pUIManager  = NULL;
    tgl.pHUDManager = NULL;

    tgl.MissionLoadCompletion = 0.0f;
    tgl.MusicId         = 0;
    // Volumes must be set at a 0.02 accuracy since the slider within the
    // volume control settings are done in increments of 0.02
    tgl.MusicSampleId   = -1;
    tgl.MasterVolume    = 1.11f;
    tgl.MusicVolume     = 0.50;
    tgl.EffectsVolume   = 1.0f;
    tgl.VoiceVolume     = 1.0f;
    
    tgl.HighestCampaignMission = 6;
    // Environment default
    tgl.Environment = t2_globals::ENVIRONMENT_LUSH ;

    // Initialize the Front End Globals
    x_memset( &fegl, 0, sizeof(fegl) );

    // Init ServerSettings in fegl
    x_wstrcpy( fegl.ServerSettings.ServerName, StringMgr( "ui", IDS_TRIBES_SERVER ) );
    fegl.ServerSettings.GameType            = 0;
    fegl.ServerSettings.MapIndex            = 0;
    fegl.ServerSettings.BotsEnabled         = FALSE;
    fegl.ServerSettings.BotsNum             = 6;
    fegl.ServerSettings.uiBotsDifficulty    = 5;
    fegl.ServerSettings.BotsDifficulty      = 0.5f;
    fegl.ServerSettings.uiTeamDamage        = 50;
    fegl.ServerSettings.TeamDamage          = 0.5f;
    fegl.ServerSettings.MaxPlayers          = 8;
    fegl.ServerSettings.MaxClients          = 7;
    fegl.ServerSettings.ObserverTimeout     = 30;
    fegl.ServerSettings.TimeLimit           = 30;
    fegl.ServerSettings.VotePass            = 60;
    fegl.ServerSettings.VoteTime            = 20;
    fegl.ServerSettings.WarmupTime          = 5;
    fegl.PortNumber                         = MIN_USER_PORT;
//    fegl.CurrentFilter.BuddyString[0]       = 0x0;
//    fegl.CurrentFilter.GameType             = GAME_NONE;
//    fegl.CurrentFilter.MinPlayers           = 0;
//    fegl.CurrentFilter.MaxPlayers           = 16;
    fegl.SortKey                            = 0;
    fegl.WarriorModified                    = FALSE;
    fegl.OptionsModified                    = FALSE;
    fegl.IngameModified                     = FALSE;
    fegl.FilterModified                     = FALSE;

    fegl.GameMode                           = FE_MODE_NULL;
    fegl.GameModeOnline                     = FALSE;

    g_SM.CurrentState = STATE_NULL;
    g_SM.NewState = STATE_COMMON_INIT;

    // DEDICATED_SERVER
    fegl.GameModeOnline                     = TRUE;
    g_SM.NewState = STATE_SERVER_INIT_SERVER;
    // DEDICATED_SERVER
}

//==============================================================================

void InitMissionSystems( void )
{
    x_MemAddMark( "Begin - InitMissionSystems" );

    // Initialize the building manager.
    g_BldManager.Initialize();
    
    GameMgr.SetStartupPower();

    x_MemAddMark( "End   - InitMissionSystems" );
    
    tgl.pHUDManager->Initialize();
}

//==============================================================================

void KillMissionSystems( void )
{
    g_BldManager.Kill();
}

//==============================================================================

void InitPermSystems( void )
{
    x_MemAddMark( "Begin - InitPermSystems" );

    bounds_Init();
    ptlight_Init();
    shape::Init() ;

    tgl.pMasterServer = new master_server;
    ASSERT(tgl.pMasterServer);
    tgl.pMasterServer->Init();
    //
    // On a wide beta version, we use the production master server
    //
    // NOTE: We need to make sure we do a version of this for the final
    // master that will select the correct server and also set the right
    // name for the directory accessed.
    tgl.pMasterServer->AddMasterServer( "tribes2.m1.sierra.com", 15101 );
    tgl.pMasterServer->AddMasterServer( "tribes2.m2.sierra.com", 15101 );
    tgl.pMasterServer->AddMasterServer( "tribes2.m3.sierra.com", 15101 );

    tgl.pMasterServer->SetPath( StringMgr( "ui", IDS_PATH_TRIBES2PS2 ) );

    // Initialize sub systems embedding within the tgl.
    tgl.ParticlePool.Init( 4096 );

    x_MemAddMark( "End   - InitPermSystems" );
}

//==============================================================================

void KillPermSystems( void )
{
    if( tgl.pServer )
    {
        delete tgl.pServer;
        tgl.pServer       = NULL;
        tgl.ServerPresent = FALSE;
    }

    if( tgl.pClient )
    {
        delete tgl.pClient;
        tgl.pClient       = NULL;
        tgl.ClientPresent = FALSE;
    }

    if( pSvrMgr )
    {
        delete pSvrMgr;
        pSvrMgr = NULL;
    }

    //----------------------------------

    tgl.ParticlePool.Kill();
    ASSERT(tgl.pMasterServer);
    tgl.pMasterServer->Kill();
    delete tgl.pMasterServer;
    tgl.pMasterServer = NULL;
    bounds_End();
    ptlight_Kill();
    audio_Kill();
    shape::Kill() ;
}

//==============================================================================

extern random SpawnRandom;

void DoOneGameCycle( void )
{
#if 0
    if( _kbhit() && (_getch() == 27) )
    {
        tgl.ExitApp = TRUE;
        return;
    }
#endif

    SpawnRandom.irand(0,100);

    extern xtimer NET_SendTime;
    extern xtimer NET_ReceiveTime;
    extern s32    NET_NSendPackets;
    extern s32    NET_NRecvPackets;
    extern s32    NET_NSendBytes;
    extern s32    NET_NRecvBytes;

    xtimer AudioTimer;
    xtimer InputTimer;
    xtimer PacketTimer;
    xtimer LogicTimer;
    xtimer RenderTimer;
    xtimer StallTimer;

    xtimer WaitTimer;
    WaitTimer.Start();
    x_WatchdogReset();

    // Update the audio channel information
    AudioTimer.Start();
    audio_Update();
    AudioTimer.Stop();

    // Process all gathered input
    InputTimer.Start();
    pump_ProcessInput();
    InputTimer.Stop();

    // Process time
    LogicTimer.Start();
    pump_ProcessTime();
    LogicTimer.Stop();

    // Process all gathered networking
    PacketTimer.Start();
    pump_ProcessPackets();
    PacketTimer.Stop();

    smem_Toggle();

    #ifdef SHOW_DMT_STUFF
    if( (g_SM.CurrentState == STATE_SERVER_INGAME) || (g_SM.CurrentState == STATE_CLIENT_INGAME) )
        x_DebugMsg( "--------------------------------------------------------------------------------\n" );
    #endif

    //x_printf("%4.1f ",WaitTimer.ReadMs());
    //if( (NFrames%6)==0 )
    //    x_printf("\n");

    StallTimer.Start();
    while( WaitTimer.ReadMs() < (f32)30 )
    {
        x_DelayThread(2);
    }        
    StallTimer.Stop();
    NFrames++;

//  x_printf( "%5.2f ", WaitTimer.ReadMs() );
//  if( (NFrames%6) == 0 )
//      x_printf( "\n" );
    
    if( SHOW_FRAME_RATE )
    {
        static f32 Highest=0;
        static s32 HighestFrame=0;
        f32 Ms = WaitTimer.ReadMs();
        
        if( Ms > Highest )
        {
            Highest = Ms;
            HighestFrame = 30*4;
        }

        x_printfxy(34,0,"%4.1f",Ms);
        HighestFrame--;
        if( HighestFrame > 0 )  x_printfxy(34,1,"%4.1f",Highest);
        else                    Highest = 0;
    }

    if( NFrames>=30*1 )
    {
        if( SHOW_MAIN_TIMINGS )
        {
            f32 PF = 1.0f;

#if 0
            x_DebugMsg("----- MAIN TIMINGS -----\n");
            x_DebugMsg("Audio:      %4.2f\n", AudioTimer.ReadMs()  * PF );
            x_DebugMsg("Input:      %4.2f\n", InputTimer.ReadMs()  * PF );
            x_DebugMsg("Packet:     %4.2f\n", PacketTimer.ReadMs() * PF );
            x_DebugMsg("  Net       %4.2f\n", STAT_NetReceiveTime.ReadMs() * PF );
            x_DebugMsg("  Server    %4.2f %1.1f\n", STAT_ServerPacketTime.ReadMs() * PF, (STAT_NServerPackets*PF) );
            x_DebugMsg("  Client    %4.2f %1.1f\n", STAT_ClientPacketTime.ReadMs() * PF, (STAT_NClientPackets*PF) );
            x_DebugMsg("Logic:      %4.2f\n", LogicTimer.ReadMs()  * PF );
            x_DebugMsg("  NLoops    %4.2f\n", STAT_NTimeLoops*PF );
            x_DebugMsg("  Server    %4.2f\n", STAT_ServerLogic.ReadMs()*PF );
            x_DebugMsg("  Client    %4.2f\n", STAT_ClientLogic.ReadMs()*PF );
            x_DebugMsg("  ObjMgr    %4.2f\n", STAT_ObjMgrLogic.ReadMs()*PF );
            x_DebugMsg("  PtLight   %4.2f\n", STAT_PtLightLogic.ReadMs()*PF );
            x_DebugMsg("  Misc      %4.2f\n", STAT_MiscLogic.ReadMs()*PF );
            x_DebugMsg("Render:     %4.2f\n", RenderTimer.ReadMs() * PF );
            x_DebugMsg("Stall:      %4.2f\n", StallTimer.ReadMs()  * PF );
#endif
            //
            // Display network stats
            //
            x_DebugMsg("NetSend T:%2.4f N:%1.1f B:%4.2f\n",
                        NET_SendTime.ReadMs()*PF,
                        NET_NSendPackets*PF,
                        NET_NSendBytes*PF);

            x_DebugMsg("NetRecv T:%2.4f N:%1.1f B:%4.2f\n",
                        NET_ReceiveTime.ReadMs()*PF,
                        NET_NRecvPackets*PF,
                        NET_NRecvBytes*PF);
        }
        NET_SendTime.Reset();
        NET_ReceiveTime.Reset();
        NET_NSendPackets = 0;
        NET_NRecvPackets = 0;
        NET_NSendBytes = 0;
        NET_NRecvBytes = 0;

        AudioTimer.Reset();
        InputTimer.Reset();
        PacketTimer.Reset();
        LogicTimer.Reset();
        RenderTimer.Reset();
        StallTimer.Reset();
        STAT_NTimeLoops = 0;

        NFrames = 0;
    }
}

//==============================================================================

void DisplayOptions( void )
{
    const char* GameTypeString[] =
    {
        "<none>",
        "Capture the Flag",
        "Capture and Hold",
        "Team Death Match",
        "Death Match",
        "Hunter", 
    };

    s32 TeamDamage = (s32)(fegl.ServerSettings.TeamDamage * 100.0f + 0.1f);

    x_printf( "\n" );
    x_printf( "Server configuration:\n" );
    x_printf( " - Server Name ...... \"%s\"\n",      (const char*)xstring( fegl.ServerSettings.ServerName    ) );
    x_printf( " - Server Password .. \"%s\"\n",      (const char*)xstring( fegl.ServerSettings.AdminPassword ) );
    x_printf( " - Game Type ........ %s\n",          GameTypeString[ fegl.ServerSettings.GameType ] );
    x_printf( " - Maximum Players .. %3d\n",         fegl.ServerSettings.MaxPlayers );
    x_printf( " - Maximum Clients .. %3d\n",         fegl.ServerSettings.MaxClients );
    x_printf( " - Team Damage ...... %3d%%\n",       TeamDamage );
    x_printf( " - Vote Pass Level .. %3d%%\n",       fegl.ServerSettings.VotePass );
    x_printf( " - Time Limit ....... %3d minutes\n", fegl.ServerSettings.TimeLimit );
    x_printf( " - Admin Username ... \"%s\"\n",      g_TelnetManager.GetAdminUser() );
    x_printf( " - Admin Password ... \"%s\"\n",      g_TelnetManager.GetAdminPass() );
    if( (x_strlen(g_TelnetManager.GetAdminUser())==0) || 
        (x_strlen(g_TelnetManager.GetAdminPass())==0) )
    {
        x_printf("WARNING: No admin user or pass set. You will not be\n"
                 "         able to remotely administer this server.\n");
    }
    x_printf( "\n" );
}

//==============================================================================

void ChangeServerName( const char* pName, xbool Telnet )
{
    s32  i, j, Len;
    char ServerName[16] = "\x1F ";
    char Buffer    [16];

    // Copy new name into buffer one character at a time.
    // Handle "special" characters as they are encountered.
    for( i=0, j=0; pName[i] && (j<15); i++ )
    {
        // Look for "special sequence".
        if( (pName[i+0] == '$') &&
            (x_strchr( "isI", pName[i+1] )) &&
            (pName[i+2] == '$') )
        {
            char Icon;
            switch( pName[i+1] )
            {
            case 'i':   Icon = (char)0x90;  break;
            case 's':   Icon = (char)0x91;  break;
            case 'I':   Icon = (char)0x92;  break;
            }

            Buffer[j++] = Icon;
            i += 2;
            continue;
        }

        // Look for "escape sequence".
        if( (pName[i+0] == '$') &&
            (x_ishex( pName[i+1] )) &&
            (x_ishex( pName[i+2] )) &&
            (pName[i+3] == '$') )
        {
            s32 Code = 0;

            if( IN_RANGE( '0', pName[i+1], '9' ) )   Code += (pName[i+1] - '0');
            if( IN_RANGE( 'a', pName[i+1], 'f' ) )   Code += (pName[i+1] - 'a') + 10;
            if( IN_RANGE( 'A', pName[i+1], 'F' ) )   Code += (pName[i+1] - 'A') + 10;

            Code *= 16;

            if( IN_RANGE( '0', pName[i+2], '9' ) )   Code += (pName[i+2] - '0');
            if( IN_RANGE( 'a', pName[i+2], 'f' ) )   Code += (pName[i+2] - 'a') + 10;
            if( IN_RANGE( 'A', pName[i+2], 'F' ) )   Code += (pName[i+2] - 'A') + 10;

            Buffer[j++] = (char)Code;
            i += 3;
            continue;
        }

        // Fix the '/' problem.
        if( pName[i] == '/' )
        {
            Buffer[j++] = '\\';
            continue;
        }

        // Look for out of range characters.
        if( !IN_RANGE( 0x20, pName[i], 0x7E ) )
            continue;

        Buffer[j++] = pName[i];
    }
    Buffer[j] = '\0';

    // Was the given name too long?
    if( (j == 15) && pName[i] )
    {
        if( Telnet )
        {
            g_TelnetManager.AddDebugText( "WARNING: Given SERVERNAME '%s' is too long.", pName );
            g_TelnetManager.AddDebugText( "         Maximum length is 15 characters." );
        }
        else
        {
            x_printf( "WARNING: Given SERVERNAME '%s' is too long.\n", pName );
            x_printf( "         Maximum length is 15 characters.\n" );
        }
    }

    // Now, copy the buffer into our local name.
    // If the name is too long, then we overwrite the "->" character.
    Len = x_strlen( Buffer );
    if( Len <= 13 )
        x_strcpy( ServerName+2, Buffer );
    else
    {
        // Name is too long.  Clobber the "->".  Also warn outside world.
        x_strcpy( ServerName+0, Buffer );

        if( Telnet )
        {
            g_TelnetManager.AddDebugText( "WARNING: Max suggested SERVERNAME length is 13." );
        }
        else
        {
            x_printf( "WARNING: Max suggested SERVERNAME length is 13.\n" );
        }
    }

    // Ship it!
    x_wstrcpy( fegl.ServerSettings.ServerName, xwstring( ServerName ) );
}

//==============================================================================

static char ServerName[16];

const char* GetServerName( void )
{
    xstring S( fegl.ServerSettings.ServerName );
    x_strncpy( ServerName, (const char*)S, 15 );
    ServerName[15] = '\0';
    return( ServerName );
}

//==============================================================================

xbool ProcessCmdLine( s32 argc, char* argv[] )
{
    token_stream DSFile;
    const char* pFilename;

    // Establish default values.
    {
        x_wstrcpy( fegl.ServerSettings.ServerName,    xwstring( "\x1F Dedicated" ) );
        x_wstrcpy( fegl.ServerSettings.AdminPassword, xwstring( ""               ) );

        fegl.nLocalPlayers                 = 0;
        fegl.GameType                      = GAME_CTF;
        fegl.ServerSettings.GameType       = GAME_CTF;
        fegl.ServerSettings.BotsDifficulty = 0.5f;
        fegl.ServerSettings.BotsEnabled    = FALSE;
        fegl.ServerSettings.BotsNum        = 0;
        fegl.ServerSettings.MaxClients     = 16;
        fegl.ServerSettings.MaxPlayers     = 16;
        fegl.ServerSettings.TeamDamage     = 0.75f;
        fegl.ServerSettings.TimeLimit      = 20;
        fegl.ServerSettings.VotePass       = 100;
        x_strcpy( fegl.MissionName, "Avalon" );
    }

    if( argc == 1 )
    {
        pFilename = "Default.settings.txt";
    }
    else
    {
        pFilename = argv[1];
    }

    if( !DSFile.OpenFile( pFilename ) )
    {
        x_printf( "Error:  Unable to open configuration file '%s'.\n", pFilename );
        return( FALSE );
    }

    x_printf( "Processing configuration file '%s'.\n", pFilename );

    char Opcode  [256];
    char Operand [256];
    char Operand2[256];

    DSFile.Read();  x_strcpy( Opcode,  DSFile.String() );

    // Process the configuration file.
    while( DSFile.Type() != token_stream::TOKEN_EOF )
    {
        //----------------------------------------------------------------------
        if( x_stricmp( Opcode, "GAMETYPE" ) == 0 )
        {
            DSFile.Read();  
            x_strcpy( Operand, DSFile.String() );

            int GameType = -1;

            if( x_stricmp( Operand, "CTF"  ) == 0 )     GameType = GAME_CTF;
            if( x_stricmp( Operand, "CNH"  ) == 0 )     GameType = GAME_CNH;
            if( x_stricmp( Operand, "TDM"  ) == 0 )     GameType = GAME_TDM;
            if( x_stricmp( Operand, "DM"   ) == 0 )     GameType = GAME_DM;
            if( x_stricmp( Operand, "HUNT" ) == 0 )     GameType = GAME_HUNTER;

            if( GameType == -1 )
            {
                x_printf( "WARNING: Unknown GAMETYPE '%s'.\n", Operand );
            }
            else
            {
                fegl.GameType                = GameType;
                fegl.ServerSettings.GameType = GameType;
            }
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "MAXPLAYERS" ) == 0 )
        {
            DSFile.Read();  
            x_strcpy( Operand, DSFile.String() );
            s32 MaxPlayers = x_atoi( Operand );
            if( !IN_RANGE( 1, MaxPlayers, 16 ) )
            {
                x_printf( "WARNING: Illegal MAXPLAYERS value (%d).\n", MaxPlayers );
                MaxPlayers = 16;
            }

            fegl.ServerSettings.MaxPlayers = MaxPlayers;
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "MAXCLIENTS" ) == 0 )
        {
            DSFile.Read();  
            x_strcpy( Operand, DSFile.String() );
            s32 MaxClients = x_atoi( Operand );
            if( !IN_RANGE( 1, MaxClients, 16 ) )
            {
                x_printf( "WARNING: Illegal MAXCLIENTS value (%d).\n", MaxClients );
                MaxClients = 16;
            }

            fegl.ServerSettings.MaxClients = MaxClients;
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "SERVERNAME" ) == 0 )
        {
            DSFile.Read();
            ChangeServerName( DSFile.String(), FALSE );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "PASSWORD" ) == 0 )
        {
            DSFile.Read();  
            x_strcpy( Operand, DSFile.String() );

            s32  Len = x_strlen( Operand );
            char Password[16];

            x_strncpy( Password, Operand, 15 );
            Password[15] = '\0';

            if( Len > 15 )
                x_printf( "WARNING: Given PASSWORD '%s' is too long.  Max characters is 15.\n", Operand );

            x_wstrcpy( fegl.ServerSettings.AdminPassword, xwstring( Password ) );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "VOTEPASS" ) == 0 )
        {
            DSFile.Read();  
            x_strcpy( Operand, DSFile.String() );

            s32 VotePass = x_atoi( Operand );
            if( !IN_RANGE( 10, VotePass, 100 ) )
            {
                x_printf( "WARNING: Illegal VOTEPASS value (%d).  Range is 10-100.\n", VotePass );
                VotePass = 100;
            }

            fegl.ServerSettings.VotePass = VotePass;
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "TEAMDAMAGE" ) == 0 )
        {
            DSFile.Read();  
            x_strcpy( Operand, DSFile.String() );

            s32 TeamDamage = x_atoi( Operand );
            if( !IN_RANGE( 0, TeamDamage, 100 ) )
            {
                x_printf( "WARNING: Illegal TEAMDAMAGE value (%d).  Range is 0-100.\n", TeamDamage );
                TeamDamage = 100;
            }

            fegl.ServerSettings.TeamDamage = TeamDamage / 100.0f;
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "TIMELIMIT" ) == 0 )
        {
            DSFile.Read();  
            x_strcpy( Operand, DSFile.String() );

            s32 TimeLimit = x_atoi( Operand );
            if( !IN_RANGE( 0, TimeLimit, 100 ) )
            {
                x_printf( "WARNING: Illegal TIMELIMIT value (%d).  Range is 10-60.\n", TimeLimit );
                TimeLimit = 100;
            }

            fegl.ServerSettings.TimeLimit = TimeLimit;
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "TELNET" ) == 0 )
        {
            // "username" and "password".
            DSFile.Read();  x_strcpy( Operand,  DSFile.String() );
            DSFile.Read();  x_strcpy( Operand2, DSFile.String() );
            g_TelnetManager.SetAdminUser( Operand  );
            g_TelnetManager.SetAdminPass( Operand2 );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "LOGGING" ) == 0 )
        {
            // ON or OFF and then "Path" for reports.
            DSFile.Read();  x_strcpy( Operand,  DSFile.String() );
            DSFile.Read();  x_strcpy( Operand2, DSFile.String() );
            LogMgr.SetLogStatus( x_stricmp( Operand, "ON" ) == 0 );
            LogMgr.SetLogDir   ( Operand2 );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "PORT" ) == 0 )
        {
            DSFile.Read();
            x_strcpy( Operand, DSFile.String() );

            s32 Port = x_atoi( Operand );
            if( (Port > 0) && (Port < 65535) )
            {
                fegl.PortNumber = Port;
            }
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "KEY" ) == 0 )
        {
            DSFile.Read();
            g_TelnetManager.SetAccessKey( DSFile.String() );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "IRC-SERVER" ) == 0 )
        {
            DSFile.Read();  

            net_address Address;
            s32 i;

            for( i = 0; i < 10; i++ )
            {
                Address.IP = net_ResolveName( DSFile.String() );
                if (Address.IP)
                    break;
            }
            if( Address.IP == 0 )
            {
                x_printf("Warning: Unable to resolve IRC server IP address '%s'\n",Operand);
            }

            DSFile.Read();
            Address.Port = x_atoi( DSFile.String() );
            if( (Address.Port <=0) || (Address.Port >=65535) )
            {
                Address.Port = 6667;
            }
            g_ChatManager.SetHost( Address );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "IRC-CHANNEL" ) == 0 )
        {
            DSFile.Read();
            g_ChatManager.SetChannel( DSFile.String() );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "IRC-NICKNAME" ) == 0 )
        {
            DSFile.Read();
            g_ChatManager.SetNick( DSFile.String() );
        }
        //----------------------------------------------------------------------
        else
        if( x_stricmp( Opcode, "TAG" ) == 0 )
        {
            TagPlayers = TRUE;
        }
        //----------------------------------------------------------------------
        else
        {
            x_printf( "WARNING:  Unknown option '%s' in configuration file.\n", Opcode );
        }
        //----------------------------------------------------------------------
        // - SKIPMAP
        // - PLAYMAP
        //----------------------------------------------------------------------
    
        DSFile.Read();  x_strcpy( Opcode,  DSFile.String() );
    }

    DSFile.CloseFile();

    // Determine first map.
    switch( fegl.GameType )
    {
        case GAME_CTF:      x_strcpy( fegl.MissionName, "Avalon"     );   break;
        case GAME_CNH:      x_strcpy( fegl.MissionName, "Abominable" );   break;
        case GAME_TDM:      x_strcpy( fegl.MissionName, "Equinox"    );   break;
        case GAME_DM:       x_strcpy( fegl.MissionName, "Equinox"    );   break;
        case GAME_HUNTER:   x_strcpy( fegl.MissionName, "Escalade"   );   break;
    }

    DisplayOptions();
    return( TRUE );
}
#include <stdio.h>
s32 main( s32 argc, char* argv[] )
{
    (void)argc;
    (void)argv;

    x_Init();

#if EXPORT_MISSIONS
    ExportMissions();
#endif

    cdfs_Init( "AADS.DAT" );
    // Initialize the engine.
    InitEngine();

    // Initialize the tgl data.
    InitT2Globals();

    // Go for the command line.
    if( !ProcessCmdLine( argc, argv ) )
        return -1;

    // Initialize various subsystems.
    InitPermSystems();

    // Load permanent data.
    LoadPermanentData();

    // Loop until ExitApp is set.  
    // (Only applies to PC, but will be used on PS2 for testing.)
    while( !tgl.ExitApp )
    {
        DoOneGameCycle();
    }

    // Unload permanent data.
    UnloadPermanentData();

    // Dump some string tables.
    StringMgr.UnloadTable( "Pickups" );
    StringMgr.UnloadTable( "Messages" );
    StringMgr.UnloadTable( "ui" );

    // Kill various subsystems.
    KillPermSystems();

    KillEngine();
    return 0;
}


//==============================================================================
#if EXPORT_MISSIONS
//==============================================================================

xbool LoadMission   ( void );
void  UnloadMission ( void );

//------------------------------------------------------------------------------

void GetMissionDefFromName( void )
{
    mission_def* pFoundMissionDef = NULL;
    mission_def* pMissionDef      = fe_Missions;

    while( pMissionDef->GameType != GAME_NONE )
    {
        if( x_strcmp( pMissionDef->Folder, fegl.MissionName ) == 0 )
        {
            pFoundMissionDef = pMissionDef;
        }

        pMissionDef++;
    }

    fegl.pMissionDef = pFoundMissionDef;
}

//------------------------------------------------------------------------------

void ExportMissions( void )
{
    char* List[] = 
    {
        "Abominable"   , 
        "Avalon"       , 
        "BeggarsRun"   , 
        "Damnation"    , 
        "DeathBirdsFly", 
        "Desiccator"   , 
        "Equinox"      , 
        "Escalade"     , 
        "Firestorm"    , 
        "Flashpoint"   , 
        "Gehenna"      , 
        "Insalubria"   , 
        "Invictus"     , 
        "JacobsLadder" , 
        "Katabatic"    , 
        "Myrkwood"     , 
        "Oasis"        , 
        "Overreach"    , 
        "Paranoia"     , 
        "Pyroclasm"    , 
        "Quagmire"     , 
        "Rasp"         , 
        "Recalescence" , 
        "Reversion"    , 
        "Rimehold"     , 
        "Sanctuary"    , 
        "Sirocco"      , 
        "Slapdash"     , 
        "SunDried"     , 
        "ThinIce"      , 
        "Tombstone"    , 
        "Underhill"    , 
        "Whiteout"     , 
        "FINISHED",
    };

    // Initialize the engine.
    InitEngine();

    // Initialize the tgl data.
    InitT2Globals();

    // Initialize various subsystems.
    InitPermSystems();

    // Load permanent data.
    LoadPermanentData();

    // Loop through missions
    s32 i=0;
    while( x_stricmp(List[i],"FINISHED") != 0 )
    {
        x_DebugMsg("*******************************\n");
        x_DebugMsg("*******************************\n");
        x_DebugMsg("*******************************\n");
        x_DebugMsg("PROCESSING %s\n",List[i]);
        x_DebugMsg("*******************************\n");
        x_DebugMsg("*******************************\n");
        x_DebugMsg("*******************************\n");

        // Init mission systems
        InitMissionSystems();

        // Setup directory
        x_strcpy( tgl.MissionName, List[i] );
        x_strcpy( fegl.MissionName, List[i] );
        x_strcpy( tgl.MissionDir, xfs("Data/Missions/%s",tgl.MissionName) );
        tgl.MissionPrefix[0] = '\0';

        // Make sure the names of all the ???-Mission.txt files are in cdfs.
        {
            X_FILE* pFile;

            pFile = x_fopen( xfs( "%s/CTF-Mission.txt", tgl.MissionDir ), "rt" );
            if( pFile )  x_fclose( pFile );

            pFile = x_fopen( xfs( "%s/CNH-Mission.txt", tgl.MissionDir ), "rt" );
            if( pFile )  x_fclose( pFile );

            pFile = x_fopen( xfs( "%s/TDM-Mission.txt", tgl.MissionDir ), "rt" );
            if( pFile )  x_fclose( pFile );

            pFile = x_fopen( xfs( "%s/DM-Mission.txt", tgl.MissionDir ), "rt" );
            if( pFile )  x_fclose( pFile );

            pFile = x_fopen( xfs( "%s/HUNT-Mission.txt", tgl.MissionDir ), "rt" );
            if( pFile )  x_fclose( pFile );
        }

        GetMissionDefFromName();        // Load mission in
        while( !LoadMission() );

        /*** NOT NEEDED IF BOTS AREN'T SUPPORTED
        // load graph
        g_Graph.Load( xfs( "%s/%s.gph", tgl.MissionDir, tgl.MissionName),FALSE );

        xfs AssFileName("%s/%s.ass", tgl.MissionDir,tgl.MissionName);

        X_FILE* fh = x_fopen( AssFileName, "rb" );
        if( fh )
        {
            x_fclose(fh);
        }
        else
        {
            x_DebugMsg( "WARNING: No ass file %s\n", (const char *)AssFileName );
        }
        ***/

        // Unload
        UnloadMission();

        KillMissionSystems();
        i++;
    }

    // Dump CD script so we can make a file.dat
    if( x_fopen_log )
    {
        x_fopen_log->SaveFile( "fs.txt" );
    }

    // Unload permanent data.
    UnloadPermanentData();

    StringMgr.UnloadTable( "ui" );

    // Kill various subsystems.
    KillPermSystems();

    // Kill the engine.
    KillEngine();

    // HACK UNTIL WE CAN SHUT DOWN PROPERLY
    _exit(0);
}

//==============================================================================
#endif // EXPORT_MISSIONS
//==============================================================================
