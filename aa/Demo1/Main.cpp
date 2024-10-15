//==============================================================================
//
//  Main.cpp
//
//==============================================================================

//#define SHOW_DMT_STUFF

//==============================================================================
//  INCLUDES
//==============================================================================

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
#include "Objects\Projectiles\Plasma.hpp"
#include "Objects\Projectiles\Bullet.hpp"
#include "Objects\Projectiles\Laser.hpp"
#include "Objects\Projectiles\Blaster.hpp"
#include "Objects\Projectiles\Bounds.hpp"
#include "Objects\Bot\Graph.hpp"
#include "cardmgr/cardmgr.hpp"
#include "titles.hpp"
#include "poly/poly.hpp"
#include "StringMgr\StringMgr.hpp"

#include "data\ui\ui_strings.h"

#include "ui\ui_manager.hpp"
#include "ui\ui_font.hpp"
#include "hud\hud_manager.hpp"
#include "masterserver/masterserver.hpp"
#include "serverversion.hpp"
#include "sm_common.hpp"
#include "FrontEnd.hpp"
#include "Demo1/SpecialVersion.hpp"

#define EXPORT_MISSIONS		0	// set this to 1 to export all missions
#define TEST_PATHFIND       0

#if TEST_PATHFIND
#include "Objects\Bot\BotObject.hpp"
#include "Objects\Bot\BotLog.hpp"
#endif

#define EXPORT_NAV_GRAPHS   0
#define VALIDATE_ASS_FILES  0
#define LIGHT_LEVELS        0   // set this to 1 to light all buildings when exporting all missions

#if EXPORT_NAV_GRAPHS
#include "BotEditor\PathEditor.hpp"
extern path_editor s_PathEditor;
extern player_object* g_pPlayer;
#endif

#if VALIDATE_ASS_FILES  
#include "BotEditor\AssetEditor.hpp"
extern asset_editor s_AssetEditor;
#endif

//=========================================================================
//  Build String
//=========================================================================

extern xbool HUD_DebugInfo;

#define RENDER_BUILD_STRING 1
					   
// BIG NOTE: DON'T CHANGE THE LENGTH OF THIS STRING SO WE CAN MAINTAIN COMPATIBILITY
// WITH THE PAL/NTSC release (on code positions for hacking)
char s_BuildString[] = "PAL Release Cand. 04";



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

xbool  g_TweakGame = FALSE;
xbool  g_TweakMenu = FALSE;

extern xstring* x_fopen_log;
extern graph g_Graph;

void TweakMenu( void );

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
void event_Render   ( s32 ViewIndex );
void event_Packet   ( void );

//=============================================================================

/*
#ifdef TARGET_PS2_CLIENT
xbool CYEDIT_ON = FALSE;
#else
xbool CYEDIT_ON = FALSE;
#endif

void UpdateCyrusEdit( void )
{
    if( !CYEDIT_ON ) return;
    if( !g_SM.MissionRunning )
        return;

#ifdef TARGET_PS2

    if( input_WasPressed(INPUT_PS2_BTN_TRIANGLE) )
    {
        x_printf("Loading fog %s\n",tgl.MissionDir);
        tgl.Fog.LoadSettings(xfs("%s/FogSettings.txt",tgl.MissionDir));
    }

    if( input_WasPressed(INPUT_PS2_BTN_CROSS) )
    {
        x_printf("Saving fog %s\n",tgl.MissionDir);
        tgl.Fog.SaveSettings(xfs("%s/FogSettings.txt",tgl.MissionDir));
    }

    if( input_WasPressed(INPUT_PS2_BTN_CIRCLE) )
    {
        x_printf("Saving camera %s\n",tgl.MissionDir);
        X_FILE* fp = x_fopen(xfs("%s/MLCamera.txt",tgl.MissionDir),"wt");
        if( !fp )
        {
            x_printf("Could not save camera settings\n");
        }
        else
        {
            const view* pView = eng_GetActiveView(0);
            vector3 Pos = pView->GetPosition();
            f32 Pitch,Yaw;
            pView->GetPitchYaw( Pitch, Yaw );

            x_fprintf(fp,"%f %f %f %f %f\n",Pos.X,Pos.Y,Pos.Z,Pitch,Yaw);
            x_fclose(fp);

            eng_ScreenShot( xfs("%s/MLCamera.tga",tgl.MissionDir) );
        }
    }
#endif
}
*/

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
        {
            // Make call to input users
            UpdateEventString('I');
            tgl.EventInputCount++;
        }

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
            s32 Count=64;
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
            s32 Count=64;
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

s32 Skip = 0;

xbool SinglePlayerView = TRUE;
f32 VIEW_X_RATIO = 1.0f;//0.75f;
f32 VIEW_Y_RATIO = 1.0f;//0.50f;
radian VIEW_XFOV = R_60;
f32 VIEW_XFOV_RATIO = 1.0f;
f32 VIEW_XFOV_RATIO_1 = 1.0f;
f32 VIEW_XFOV_RATIO_2 = 1.25f;

#ifdef TARGET_PS2
extern f32 PS2BUILDING_GLOBALK;
extern f32 TERRAIN_BASE_WORLD_PIXEL_SIZE;
#endif

//------------------------------------------------------------------------------

void pump_ProcessRender( void )
{
    if( tgl.ExitApp )
        return;

    // Startup shape rendering system
    shape::BeginFrame();

    // Check if enough time has gone by to render again
    //if( (tgl.LogicTimeMs - tgl.LastRenderTime) >= tgl.EventRateRender )
    {
        tgl.LastRenderTime += tgl.EventRateRender;

        UpdateEventString('R');
        tgl.EventRenderCount++;

        if( tgl.NLocalPlayers == 2 )
        {
            #ifdef TARGET_PS2
            PS2BUILDING_GLOBALK = 0.7f;
            TERRAIN_BASE_WORLD_PIXEL_SIZE = 0.15f;
            #endif
            VIEW_X_RATIO = 1.00f;
            VIEW_Y_RATIO = 0.50f;
            VIEW_XFOV = R_70;
            VIEW_XFOV_RATIO = VIEW_XFOV_RATIO_2;
            event_Render(0);
            event_Render(1);
        }
        else
        {
            #ifdef TARGET_PS2
            PS2BUILDING_GLOBALK = 0.85f;
            TERRAIN_BASE_WORLD_PIXEL_SIZE = 0.25f;
            #endif
            VIEW_X_RATIO = 1.0f;
            VIEW_Y_RATIO = 1.0f;
            VIEW_XFOV = R_70;
            VIEW_XFOV_RATIO = VIEW_XFOV_RATIO_1;
            event_Render(0);
        }

        #ifdef TARGET_PS2
        {
            s32 width,height;

            eng_Begin("Scissor");
            gsreg_Begin();
            eng_GetRes(width,height);
            gsreg_SetScissor(0,0,width,height);
            eng_PushGSContext(1);
            gsreg_SetScissor(0,0,width,height);
            eng_PopGSContext();
            gsreg_End();
            eng_End();
        }
        #endif

        view v = *eng_GetView( 0 );
        eng_MaximizeViewport( v );
        eng_SetView( v, 0 );

        // Render UI and HUD
        tgl.pHUDManager->Render();
        tgl.pUIManager->Render();

        // Render Build Number

        if( HUD_DebugInfo )
        {
            eng_Begin( "Build Number" );

            static xtick OldActualTime=0;
            if( !OldActualTime ) OldActualTime = tgl.ActualTime;

            xtick DeltaTime = tgl.ActualTime - OldActualTime;
            OldActualTime = tgl.ActualTime;
            s32 m,s;
            s32 mstick,fps;


            mstick = (s32)x_TicksToMs(DeltaTime);
            if (mstick <=0 )
                mstick = 1;
            fps = (s32)((1000.0f/mstick)+0.5f);

            m = (s32)x_TicksToSec(x_GetTime()) / 60;
            s = (s32)x_TicksToSec(x_GetTime()) % 60;
            xstring Buildstring(xfs("%s - %d:%02d",s_BuildString,m,s) );


            tgl.pUIManager->RenderText( 0, irect(0, 8,512,24), ui_font::v_center|ui_font::h_center, xcolor(255,255,255,192), Buildstring );
            tgl.pUIManager->RenderText( 0, irect(0,24,512,40), ui_font::v_center|ui_font::h_center, xcolor(255,255,255,192), xfs("%d fps",fps) );

            eng_End();
        }

		if (s_PingDisplayEnabled)
		{
			f32 pingtime=0.0f;

			if (tgl.pClient)
			{
				pingtime = tgl.pClient->GetPing();
			}
			else if (tgl.pServer)
			{
				pingtime = tgl.pServer->GetAveragePing();
			}

			if (tgl.pClient || tgl.pServer)
			{
				xstring PingString(xfs("Ping : %2.2fms",pingtime) );
				eng_Begin( "Ping Time" );
				tgl.pUIManager->RenderText( 0, irect(840,840,52,24), ui_font::v_center|ui_font::h_center, xcolor(255,255,255,192), PingString );
				eng_End();
			}
		}

#ifdef TARGET_PS2
		if ( input_IsPressed(INPUT_PS2_BTN_SELECT,0) && input_WasPressed(INPUT_PS2_BTN_R2,0) )
		{
			s_PingDisplayEnabled = !s_PingDisplayEnabled;
		}
#endif

/*
if( !CYEDIT_ON  )
{
#if RENDER_BUILD_STRING

#if 0
        {
            static s32 count=0;
            static s32 Free = 0,Largest = 0,Fragments=0;
            count--;
            if (count <=0)
            {
                extern void ps2_GetFreeMem(s32 *,s32 *,s32 *);
                ps2_GetFreeMem(&Free,&Largest,&Fragments);
                count = 100;

            }
            tgl.pUIManager->RenderText( 0, 
                                        irect(0,40,512,40), 
                                        ui_font::v_center|ui_font::h_center, 
                                        xcolor(255,255,255,192), 
                                        xfs("Free: %d, Largest: %d, Fragments: %d\n",Free,Largest,Fragments) );
        }
#endif
#endif
}
*/
        // MUST be called at end of render frame!
        shape::EndFrame();

        // Flip Page
		xprof_Disable();
        eng_PageFlip();
		xprof_Enable();

        if( PRINT_ENGINE_STATS )
        {
            if( ((NFrames++)%120) == 0 )
                eng_PrintStats();
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

void InitEngine( void )
{
    #ifdef TARGET_PC
    d3deng_SetPresets( ENG_ACT_LOCK_WINDOW_SIZE );
    #endif

    eng_Init();
    StartTime = x_GetTime();
    x_DebugSetVersionString(xfs("%s, %s, %s",s_BuildString,__DATE__,__TIME__));
    x_DebugSetCrashFunction(TribesCrashMessageBuilder);

    audio_Init();
    card_Init();
    net_Init();
}

//==============================================================================

void KillEngine( void )
{
    net_Kill();
    card_Kill();
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
#ifdef T2_DESIGNER_BUILD
    tgl.HighestCampaignMission = 16;
#endif
    #ifdef DEMO_DISK_HARNESS
    tgl.HighestCampaignMission =  5;
    #endif
    // Environment default
    tgl.Environment = t2_globals::ENVIRONMENT_LUSH ;

    // Initialize the Front End Globals
    x_memset( &fegl, 0, sizeof(fegl) );

    InitWarriorSetup( fegl.WarriorSetups[0], 0 );
    SetWarriorControlDefault( &fegl.WarriorSetups[0], WARRIOR_CONTROL_CONFIG_A );
    InitWarriorSetup( fegl.WarriorSetups[1], 1 );
    SetWarriorControlDefault( &fegl.WarriorSetups[1], WARRIOR_CONTROL_CONFIG_A );
    InitWarriorSetup( fegl.WarriorSetups[2], 2 );
    SetWarriorControlDefault( &fegl.WarriorSetups[2], WARRIOR_CONTROL_CONFIG_A );

    fegl.WarriorSetups[0].DualshockEnabled = TRUE;
    fegl.WarriorSetups[0].KeyboardEnabled = 0;
    fegl.WarriorSetups[0].SensitivityX    = 5; //38;
    fegl.WarriorSetups[0].SensitivityY    = 5; //12;

    fegl.WarriorSetups[1].DualshockEnabled = TRUE;
    fegl.WarriorSetups[1].KeyboardEnabled = 0;
    fegl.WarriorSetups[1].SensitivityX    = 5; //38;
    fegl.WarriorSetups[1].SensitivityY    = 5; //12;

    fegl.WarriorSetups[2].DualshockEnabled = TRUE;
    fegl.WarriorSetups[2].KeyboardEnabled = 0;
    fegl.WarriorSetups[2].SensitivityX    = 5; //38;
    fegl.WarriorSetups[2].SensitivityY    = 5; //12;
    xwstring Name( "J.Ransom" );
    x_wstrncpy( &fegl.WarriorSetups[2].Name[0], Name, FE_MAX_WARRIOR_NAME-1 );
    fegl.WarriorSetups[2].Name[FE_MAX_WARRIOR_NAME-1] = 0;

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
//    fegl.CurrentFilter.BuddyString[0]       = 0x0;
//    fegl.CurrentFilter.GameType             = GAME_NONE;
//    fegl.CurrentFilter.MinPlayers           = 0;
//    fegl.CurrentFilter.MaxPlayers           = 16;
    fegl.SortKey                            = 0;
	fegl.WarriorModified				    = FALSE;
	fegl.OptionsModified				    = FALSE;
	fegl.IngameModified					    = FALSE;
	fegl.FilterModified					    = FALSE;

    fegl.GameMode                           = FE_MODE_NULL;
    fegl.GameModeOnline                     = FALSE;

    g_SM.CurrentState = STATE_NULL;
    g_SM.NewState = STATE_COMMON_INIT;
}

//==============================================================================

void InitMissionSystems( void )
{
    x_MemAddMark( "Begin - InitMissionSystems" );

    // Initialize the building manager.
    g_BldManager.Initialize();
    
    // Initialize the polygon soup renderer
    poly_Initialize();
    
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
#if defined(WIDE_BETA) || defined(RELEASE_CANDIDATE)// SPECIAL_VERSION
    tgl.pMasterServer->AddMasterServer( "tribes2.m1.sierra.com", 15101 );
    tgl.pMasterServer->AddMasterServer( "tribes2.m2.sierra.com", 15101 );
    tgl.pMasterServer->AddMasterServer( "tribes2.m3.sierra.com", 15101 );
#else
	//
	// On any other, we use the test master server
	//
    tgl.pMasterServer->AddMasterServer( "devtest.m1.sierra.com", 15101 );
    tgl.pMasterServer->AddMasterServer( "devtest.m2.sierra.com", 15101 );
    tgl.pMasterServer->AddMasterServer( "devtest.m3.sierra.com", 15101 );
#endif
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
        delete pSvrMgr;

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

xbool SHOW_ANDY_BOT_TIMER = FALSE;
xbool SHOW_ANDY_BOTINTTASK_TIMER = FALSE;
extern random SpawnRandom;

void DoOneGameCycle( void )
{
    SpawnRandom.irand(0,100);
/*
    // If the client and server are not yet synchronized enough
    // to let the player begin participating, then show a 
    // "Loading/Score" screen.  Otherwise, show the game proper.

    // The process of synchronizing the client with the server
    // has the effect of loading all needed objects (including
    // things like the terrain and buildings).

    // Process packets, input, and time.

    if( tgl.GameActive )
    {
        // Render game world!
    }
    else
    {
        // Show the "Loading/Score" screen.
    }
*/
/*
    extern xtimer STAT_PtLightLogic;
    extern xtimer STAT_ServerLogic;
    extern xtimer STAT_ClientLogic;
    extern xtimer STAT_MiscLogic;
    extern xtimer STAT_ObjMgrLogic;
    extern xtimer STAT_MAI;
*/
    extern xtimer NET_SendTime;
    extern xtimer NET_ReceiveTime;
    extern xtimer BotTimer;
    extern xtimer BotLogicTimer[3];
    extern xtimer BotEditorTimer;
    extern s32    NET_NSendPackets;
    extern s32    NET_NRecvPackets;
    extern s32    NET_NSendBytes;
    extern s32    NET_NRecvBytes;
/*
    extern xtimer BTimer[10];
    extern xtimer GetClosestEdgeTime[3];
    extern s32    s_BotsExecuted;
    extern xtimer BotUpdateGoToLocTimer[10];
    extern xtimer BotPursueLocTimer[10];
*/
    xtimer AudioTimer;
    xtimer InputTimer;
    xtimer PacketTimer;
    xtimer LogicTimer;
    xtimer RenderTimer;
    xtimer StallTimer;
/*
    STAT_NTimeLoops=0;
    STAT_NServerPackets=0;
    STAT_NClientPackets=0;
    STAT_ServerPacketTime.Reset();
    STAT_ClientPacketTime.Reset();
    STAT_PtLightLogic.Reset();
    STAT_ServerLogic.Reset();
    STAT_ClientLogic.Reset();
    STAT_MiscLogic.Reset();
    STAT_ObjMgrLogic.Reset();
    STAT_MAI.Reset();
*/
    BotTimer.Reset();
    BotLogicTimer[0].Reset();
    BotLogicTimer[1].Reset();
    BotLogicTimer[2].Reset();
    BotEditorTimer.Reset();

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

    // Process render
    RenderTimer.Start();
    pump_ProcessRender();
    RenderTimer.Stop();

    // Ingame Tweak Menu
    if( g_TweakMenu )
        TweakMenu();

    #ifdef SHOW_DMT_STUFF
    if( (g_SM.CurrentState == STATE_SERVER_INGAME) || (g_SM.CurrentState == STATE_CLIENT_INGAME) )
        x_DebugMsg( "--------------------------------------------------------------------------------\n" );
    #endif

    //x_printf("%4.1f ",WaitTimer.ReadMs());
    //if( (NFrames%6)==0 )
    //    x_printf("\n");

    StallTimer.Start();
    #ifdef TARGET_PC
    while( WaitTimer.ReadMs() < (f32)30 )  { }        
    #endif
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
//            f32 PF = 1.0f / (f32)NFrames;
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
if ( SHOW_BOT_TIMINGS)
{
            x_DebugMsg("   Bot:     %4.2f\n", BotTimer.ReadMs()  * PF );
            x_DebugMsg("    BotL0:  %4.2f\n", BotLogicTimer[0].ReadMs()  * PF );
            x_DebugMsg("    BotL1:  %4.2f\n", BotLogicTimer[1].ReadMs()  * PF );
            x_DebugMsg("    BotL2:  %4.2f\n", BotLogicTimer[2].ReadMs()  * PF );
            x_DebugMsg("  MstrAI    %4.2f\n", STAT_MAI.ReadMs()*PF );
            x_DebugMsg("BotEd:      %4.2f\n", BotEditorTimer.ReadMs()  * PF );
}
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
/*
        STAT_NServerPackets=0;
        STAT_NClientPackets=0;
        STAT_ServerPacketTime.Reset();
        STAT_ClientPacketTime.Reset();
        STAT_NetReceiveTime.Reset();

        STAT_PtLightLogic.Reset();
        STAT_ServerLogic.Reset();
        STAT_ClientLogic.Reset();
        STAT_MiscLogic.Reset();
        STAT_ObjMgrLogic.Reset();
        STAT_MAI.Reset();
*/
        BotTimer.Reset();
        BotLogicTimer[0].Reset();
        BotLogicTimer[1].Reset();
        BotLogicTimer[2].Reset();
        BotEditorTimer.Reset();

        NFrames = 0;
    }

//  UpdateCyrusEdit();

#if 0//defined (acheng)
    if (GetClosestEdgeTime[0].ReadMs())
        x_DebugMsg("GCET: %5.2f   %2d   %5.2f   %5.2f   %5.2f\n",
                GetClosestEdgeTime[0].ReadMs(),
                GetClosestEdgeTime[0].GetNSamples(),
                GetClosestEdgeTime[0].GetAverageMs(),
        GetClosestEdgeTime[0].Reset();
#endif
/*

    //
    // BOT TIMING OUTPUT
    //
    if( (SHOW_ANDY_BOT_TIMER) && (tgl.GameStage == GAME_STAGE_INGAME) )
    {
        //static X_FILE* fp = NULL;
        //if( !fp ) fp = x_fopen("BotMainTimes.txt","wt");
        //ASSERT(fp);

        //x_fprintf(fp,"BT(%2d): ",s_BotsExecuted);
        x_DebugMsg("BT(%2d): ",s_BotsExecuted);
        for( s32 i=0; i<=6; i++ )
        {
            //x_fprintf(fp,"%5.2f ",BTimer[i].ReadMs());
            x_DebugMsg("%5.2f ",BTimer[i].ReadMs());
            BTimer[i].Reset();
        }
        x_DebugMsg("\n");
        //x_fprintf(fp,"\n");
    }

    extern xtimer BotInitTask[13];
    extern xtimer FindPathTimer;
    if( (SHOW_ANDY_BOTINTTASK_TIMER) && (tgl.GameStage == GAME_STAGE_INGAME) )
    {
        x_DebugMsg("FPT(%5.2f,%2d)  ",FindPathTimer.ReadMs(),FindPathTimer.GetNSamples());
        FindPathTimer.Reset();
        x_DebugMsg("BIT: ",s_BotsExecuted);
        for( s32 i=0; i<=12; i++ )
        {
            //x_fprintf(fp,"%5.2f ",BTimer[i].ReadMs());
            x_DebugMsg("%5.2f ",BotInitTask[i].ReadMs());
            BotInitTask[i].Reset();
        }
        x_DebugMsg("\n");
    }

    if( 0 )
    {
        x_printf("BT2: ");
        for( s32 i=0; i<=7; i++ )
        {
            x_printf("%4.1f ",BotUpdateGoToLocTimer[i].ReadMs());
            BotUpdateGoToLocTimer[i].Reset();
        }
        x_printf("\n");
    }

    if( (0) && (tgl.GameStage == GAME_STAGE_INGAME) )
    {
        //static X_FILE* fp = NULL;
        //if( !fp ) fp = x_fopen("BotPursueLoc.txt","wt");
        //ASSERT(fp);

        for( s32 i=0; i<=8; i++ )
        {
            //x_fprintf(fp,"%5.2f ",BotPursueLocTimer[i].ReadMs());
            x_printf("%5.2f ",BotPursueLocTimer[i].ReadMs());
            BotPursueLocTimer[i].Reset();
        }
        //x_fprintf(fp,"\n");
        x_printf("\n");
    }

    if( 0 )
    {
        x_DebugMsg("GCET: %5.2f   %2d   %5.2f   %5.2f   %5.2f\n",
                GetClosestEdgeTime[0].ReadMs(),
                GetClosestEdgeTime[0].GetNSamples(),
                GetClosestEdgeTime[0].GetAverageMs(),
                GetClosestEdgeTime[1].ReadMs(),
                GetClosestEdgeTime[2].ReadMs());
        GetClosestEdgeTime[0].Reset();
        GetClosestEdgeTime[1].Reset();
        GetClosestEdgeTime[2].Reset();
    }

    extern xtimer PlayerTimer[10];
    if( (0) && (tgl.GameStage == GAME_STAGE_INGAME) )
    {
        x_DebugMsg("PR: ");
        for( s32 i=0; i<=2; i++ )
        {
            x_DebugMsg("%5.2f|%2d ",PlayerTimer[i].ReadMs(),PlayerTimer[i].GetNSamples());
            PlayerTimer[i].Reset();
        }
        x_DebugMsg("\n");
    }

    extern xtimer ObjMgrCollideTimer[2];
    extern xtimer RayBlockTimer;
    if( (0) && (tgl.GameStage == GAME_STAGE_INGAME) )
    {
        x_DebugMsg("CT(%5.2f): ",ObjMgrCollideTimer[0].ReadMs()+ObjMgrCollideTimer[1].ReadMs());
        for( s32 i=0; i<=1; i++ )
        {
            x_DebugMsg("%5.2f|%2d  ",ObjMgrCollideTimer[i].ReadMs(),ObjMgrCollideTimer[i].GetNSamples());
            ObjMgrCollideTimer[i].Reset();
        }
        x_DebugMsg("RBT:%5.2f|%2d\n",RayBlockTimer.ReadMs(),RayBlockTimer.GetNSamples());
        RayBlockTimer.Reset();
    }
*/
}

//==============================================================================
//==============================================================================
//==============================================================================
// MISSION INGAME EXPORT
//==============================================================================
//==============================================================================
//==============================================================================

xbool LoadMission( void );
void UnloadMission( void );

#if EXPORT_MISSIONS
void  GetMissionDefFromName( void )
{
    mission_def*    pFoundMissionDef = NULL;
    mission_def*    pMissionDef      = fe_Missions;

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
        "Mission01"    , 
        "Mission02"    , 
        "Mission03"    , 
        "Mission04"    , 
        "Mission05"    , 
        "Mission06"    , 
        "Mission07"    , 
        "Mission08"    , 
        "Mission10"    , 
        "Mission11"    , 
        "Mission13"    , 
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
        "Training1"    ,
        "Training2"    ,
        "Training3"    ,
        "Training4"    ,
        "Training5"    ,
        "Underhill"    , 
        "Whiteout"     , 
        "FINISHED",
    };

    // Initialize the engine.
    InitEngine();
    // Loads & Displays the inevitable logo. This will return immediately to
    // allow continuing loads while displaying something very early in startup.

    // Initialize the tgl data.
    InitT2Globals();

    // Initialize various subsystems.
    InitPermSystems();

    // Load permanent data.
    LoadPermanentData();


	{
		s32 handle;

		handle = sceOpen("host:files.dat",SCE_RDONLY);
		ASSERTS(handle<0,"Delete FILES.DAT before exporting missions.");
	}
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

        // Clear file log
        #if (defined(TARGET_PS2_DEV)) 
        if (x_fopen_log)
            x_fopen_log->Clear();
        #endif

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

            pFile = x_fopen( xfs( "%s/SOLO-Mission.txt", tgl.MissionDir ), "rt" );
            if( pFile )  x_fclose( pFile );
        }

        // Save memory dump before loading mission
        //x_MemDump(xfs("%s/MemDump0.txt",tgl.MissionDir));

        GetMissionDefFromName();        // Load mission in
        while( !LoadMission() );

        // Save memory dump after loading mission
        //x_MemDump(xfs("%s/MemDump1.txt",tgl.MissionDir));

        // load graph
        g_Graph.Load( xfs( "%s/%s.gph", tgl.MissionDir, tgl.MissionName),FALSE );

        xfs AssFileName("%s/%s.ass", tgl.MissionDir,tgl.MissionName);

        X_FILE *fh = x_fopen(AssFileName,"rb");
        if (fh)
        {
            x_fclose(fh);
        }
        else
        {
            x_DebugMsg("WARNING: No ass file %s\n",(const char *)AssFileName);
        }

        // Dump CD script so we can make a file.dat
        #if (defined(TARGET_PS2_DEV)) 
        if( x_fopen_log )
        {
            x_fopen_log->SaveFile( xfs("%s/cdfs_mission.txt",tgl.MissionDir) );
            x_fopen_log->Clear();
        }
        #endif

        #ifdef TARGET_PC
        // Process terrain lighting
        {
            // In LoadMission
        }

        // Process buildings
        {
            #if LIGHT_LEVELS        
            g_BldManager.SetLightDir( -tgl.Sun );
            LightBuildings();
            #endif
        }
        #endif

        // Unload
        UnloadMission();

        KillMissionSystems();

        // Save memory dump after unloading mission
//      x_MemDump(xfs("%s/MemDump2.txt",tgl.MissionDir));
        i++;
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

#endif

//==============================================================================
#if TEST_PATHFIND
void TestPathFinding( void )
{
    char* List[] = 
    {
        "Katabatic"    , 
//        "Abominable"   , 
//        "Avalon"       , 
//        "BeggarsRun"   , 
//        "Damnation"    , 
//        "DeathBirdsFly", 
//        "Desiccator"   , 
//        "Equinox"      , 
//        "Escalade"     , 
//        "Firestorm"    , 
//        "Flashpoint"   , 
//        "Gehenna"      , 
//        "Insalubria"   , 
//        "Invictus"     , 
//        "JacobsLadder" , 
//        "Mission01"    , 
//        "Mission02"    , 
//        "Mission03"    , 
//        "Mission04"    , 
//        "Mission05"    , 
//        "Mission06"    , 
//        "Mission07"    , 
//        "Mission08"    , 
//        "Mission10"    , 
//        "Mission11"    , 
//        "Mission13"    , 
//        "Myrkwood"     , 
//        "Oasis"        , 
//        "Overreach"    , 
//        "Paranoia"     , 
//        "Pyroclasm"    , 
//        "Quagmire"     , 
//        "Rasp"         , 
//        "Recalescence" , 
//        "Reversion"    , 
//        "Rimehold"     , 
//        "Sanctuary"    , 
//        "Sirocco"      , 
//        "Slapdash"     , 
//        "SunDried"     , 
//        "ThinIce"      , 
//        "Tombstone"    , 
//        "Training1"    ,
//        "Training2"    ,
//        "Training3"    ,
//        "Training4"    ,
//        "Training5"    ,
//        "Underhill"    , 
//        "Whiteout"     , 
//
        "FINISHED",
    };

    // Initialize the engine.
    InitEngine();
    // Loads & Displays the inevitable logo. This will return immediately to
    // allow continuing loads while displaying something very early in startup.

    // Initialize the tgl data.
    InitT2Globals();

    // Initialize various subsystems.
    InitPermSystems();

    // Load permanent data.
    LoadPermanentData();
    #if (defined(TARGET_PS2_DEV)) 
    if( x_fopen_log )
    {
        // Let's not bother dumping here since it's not going
        // to have all the file references in cdfs_perm anyway.
        x_fopen_log->Clear();
    }
    #endif

    bot_log::Clear();
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

        // Clear file log
        #if (defined(TARGET_PS2_DEV)) 
        if (x_fopen_log)
            x_fopen_log->Clear();
        #endif

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

            pFile = x_fopen( xfs( "%s/SOLO-Mission.txt", tgl.MissionDir ), "rt" );
            if( pFile )  x_fclose( pFile );
        }

        // Save memory dump before loading mission
        //x_MemDump(xfs("%s/MemDump0.txt",tgl.MissionDir));

        GetMissionDefFromName();        // Load mission in
        while( !LoadMission() );

        // Save memory dump after loading mission
        //x_MemDump(xfs("%s/MemDump1.txt",tgl.MissionDir));

        // load graph
        g_Graph.Load( xfs( "%s/%s.gph", tgl.MissionDir, tgl.MissionName),FALSE );

        xfs AssFileName("%s/%s.ass", tgl.MissionDir,tgl.MissionName);

        X_FILE *fh = x_fopen(AssFileName,"rb");
        if (fh)
        {
            x_fclose(fh);
        }
        else
        {
            x_DebugMsg("WARNING: No ass file %s\n",(const char *)AssFileName);
        }

        // Dump CD script so we can make a file.dat
        #if (defined(TARGET_PS2_DEV)) 
        if( x_fopen_log )
        {
            x_fopen_log->SaveFile( xfs("%s/cdfs_mission.txt",tgl.MissionDir) );
            x_fopen_log->Clear();
        }
        #endif

        bot_object* s_pBotObject = (bot_object*)ObjMgr.CreateObject( object::TYPE_BOT );
        s_pBotObject->Initialize( vector3(0,100,0),                             // Pos
                                  player_object::CHARACTER_TYPE_FEMALE,         // Character type
                                  player_object::SKIN_TYPE_BEAGLE,              // Skin type
                                  player_object::VOICE_TYPE_FEMALE_HEROINE,     // Voice type
                                  default_loadouts::STANDARD_BOT_HEAVY,         // Default loadout
                                  NULL,                         // Name
                                  TRUE );                                       // Created from editor or not

        s_pBotObject->SetTeamBits(2);

        ObjMgr.AddObject( s_pBotObject, obj_mgr::AVAILABLE_SERVER_SLOT );
        s_pBotObject->Respawn() ;
        s_pBotObject->Player_SetupState( player_object::PLAYER_STATE_IDLE );

        s32 nNodes = g_Graph.m_nNodes;
        vector3 Src, Dst;
        for (s32 j = 0; j < nNodes; j++)
        {
            Src = g_Graph.m_pNodeList[j].Pos;
            for (s32 k = j + 1; k < nNodes; k++)
            {
                Dst = g_Graph.m_pNodeList[k].Pos;
                x_DebugMsg("*** Testing From Node %d to Node %d ***\n", j, k);
                s_pBotObject->RunEditorPath(Src, Dst);
            }
        }

        // Unload
        UnloadMission();

        KillMissionSystems();

        i++;

        volatile xbool KillMe = 0;
        if (KillMe)
            break;
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

#endif
//==============================================================================

#if EXPORT_NAV_GRAPHS
void ExportNavGraphs( void )
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
        "Mission01"    , 
        "Mission02"    , 
        "Mission03"    , 
        "Mission04"    , 
        "Mission05"    , 
        "Mission06"    , 
        "Mission07"    , 
        "Mission08"    , 
        "Mission10"    , 
        "Mission11"    , 
        "Mission13"    , 
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
        "Training1"    ,
        "Training2"    ,
        "Training3"    ,
        "Training4"    ,
//      "Training5"    ,
        "Underhill"    , 
        "Whiteout"     , 

        "FINISHED",
    };

    // Initialize the engine.
    InitEngine();
    // Loads & Displays the inevitable logo. This will return immediately to
    // allow continuing loads while displaying something very early in startup.

    // Initialize the tgl data.
    InitT2Globals();

    // Initialize various subsystems.
    InitPermSystems();

    // Load permanent data.
    LoadPermanentData();

    // Loop through missions
    s32 i=0;
    g_pPlayer = NULL;
    while( x_stricmp(List[i],"FINISHED") != 0 )
    {
        // Init mission systems
        InitMissionSystems();

        // Setup directory
        x_strcpy( tgl.MissionName, List[i] );
        x_strcpy( tgl.MissionDir, xfs("Data/Missions/%s",tgl.MissionName) );
        tgl.MissionPrefix[0] = '\0';

        // Load mission in
        while( !LoadMission() );

        xfs PthFileName("Data/Missions/%s/%s%s", List[i], List[i], ".pth");
        xfs GphFileName("Data/Missions/%s/%s%s", List[i], List[i], ".gph");
        xfs AssFileName("Data/Missions/%s/%s%s", List[i], List[i], ".ass");

#if VALIDATE_ASS_FILES
        g_pPlayer = (player_object*)ObjMgr.CreateObject( object::TYPE_PLAYER);
        g_pPlayer->Initialize(  player_object::CHARACTER_TYPE_FEMALE,         // Character type
                                  player_object::SKIN_TYPE_BEAGLE,              // Skin type
                                  player_object::VOICE_TYPE_FEMALE_HEROINE,     // Voice type
                                  0,         // Default loadout
                                  "PEPE THE HAPPY BOT");

        ObjMgr.AddObject( g_pPlayer, obj_mgr::AVAILABLE_SERVER_SLOT );
        //g_pPlayer->Player_SetupState( player_object::PLAYER_STATE_SPAWN );

        ASSERT(g_pPlayer);
    tgl.ServerPresent = TRUE;
    x_DebugMsg("Examining %s asset file...", List[i]);
    s_AssetEditor.Load( AssFileName );
    x_DebugMsg("done.\n");
    tgl.ServerPresent = FALSE;

#else
        s_PathEditor.Load( PthFileName );
        x_DebugMsg("Exporting %s... ", List[i]);
        s_PathEditor.Export( GphFileName );
        x_DebugMsg("done.  Testing continuity...");
        if ( !g_Graph.DebugConnected() )
        {
            MessageBox( NULL, "Graph is not connected\nCheck projects\\T2\\Demo1\\BotLog.txt for node positions",
                "Error", MB_OK | MB_ICONINFORMATION );
        }
        else
            x_DebugMsg("OK\n");

        s_PathEditor.Save( PthFileName );
#endif

        // load graph
//                g_Graph.Load( xfs( "%s/%s.gph", tgl.MissionDir, tgl.MissionName),FALSE );

        // Unload
        UnloadMission();

        KillMissionSystems();

        // Save memory dump after unloading mission
//        x_MemDump(xfs("%s/MemDump2.txt",tgl.MissionDir));
        i++;
    }

    // Unload permanent data.
    UnloadPermanentData();

    StringMgr.UnloadTable( "ui" );

    // Kill various subsystems.
    KillPermSystems();

    // Kill the engine.
    KillEngine();

#ifndef TARGET_PS2
    // HACK UNTIL WE CAN SHUT DOWN PROPERLY
    _exit(0);
#endif
}


#endif
//==============================================================================
extern "C" void ps2_SetBreakpoint(void*,s32,s32);

void test(void)
{
	test();
}
void AppMain( s32 argc, char* argv[] )
{
#ifdef TARGET_PS2_DEV
	// FOR A DEVKIT! Set a breakpoint to 64K below the stack so that we can
	// force a halt should a stack overrun occur
	s32 addr;

	ps2_SetBreakpoint((void*)((s32)&addr & 0xffff0000),0xffffff00,(1<<30)|(1<<29)|(1<<21)|(1<<20));
#endif
    #if EXPORT_MISSIONS
    ExportMissions();
    #endif
    
    #if EXPORT_NAV_GRAPHS
        ExportNavGraphs();
        return;
    #endif

    (void)argc;
    (void)argv;

    // Initialize the engine.
    InitEngine();
    // Initialize the tgl data.
    InitT2Globals();
    // Must be after the InitT2Globals() since it will set some
    // global variables when it reads information back from the
    // memory card.
    title_SierraLogo(FALSE);

    // Initialize various subsystems.
    InitPermSystems();

    // Load permanent data.
    LoadPermanentData();

    #ifdef TARGET_PS2_DEV
    //x_MemDump("PreMissionLoad.txt");
    #endif

#if 0
    // Dump CD script so we can make a file.dat
    #if (defined(TARGET_PS2_DEV)) //&& defined(cgalley))
    if( x_fopen_log ) 
    {
        x_fopen_log->SaveFile( "cdfs_perm.txt" );
        x_fopen_log->Clear();
    }
    #endif
#endif

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

    //x_MemDump( "After.txt" );

    // Kill the engine.
	eng_PageFlip();
	eng_PageFlip();
    KillEngine();

  //
  // The PS2 can only legitimately exit when going to the network configuration.
  //
#ifdef TARGET_PS2
#ifdef TARGET_PS2_DVD
#define PROG_NAME "cdrom0:\\SLES_513.31;1"
#define EXEC_NAME "cdrom0:\\NETGUI\\NTGUICD.ELF;1"
#else
#define PROG_NAME "host:ps2-devkit-debug/demo1.elf"
#if defined(PAL_RELEASE)
#define EXEC_NAME "host:cdbuild-pal/netgui/ntguihost.elf"
#else
#define EXEC_NAME "host:cdbuild/netgui/ntguihost.elf"
#endif
#endif

	static char *name[]={PROG_NAME};


	LoadExecPS2(EXEC_NAME,1,name);
#endif
    // HACK UNTIL WE CAN SHUT DOWN PROPERLY
#ifndef TARGET_PS2    
    _exit(0);
#endif
}

//==============================================================================

struct tweak
{
    char         Name[24];
    pain::type   PainType;
    object::type ObjType;
};

//==============================================================================

tweak TweakValues[] =
{
    {    " Light ",     pain::HIT_BY_ARMOR_LIGHT,  object::TYPE_PLAYER                },
    {    " Medium ",    pain::HIT_BY_ARMOR_MEDIUM, object::TYPE_BOT                   },
    {    " Heavy ",     pain::HIT_BY_ARMOR_HEAVY,  object::TYPE_END_DAMAGEABLE        },
    {    " GravCycle ", pain::HIT_BY_GRAV_CYCLE,   object::TYPE_VEHICLE_WILDCAT       },
    {    " Fighter ",   pain::HIT_BY_SHRIKE,       object::TYPE_VEHICLE_SHRIKE        },
    {    " Bomber ",    pain::HIT_BY_BOMBER,       object::TYPE_VEHICLE_THUNDERSWORD  },
    {    " Transport ", pain::HIT_BY_TRANSPORT,    object::TYPE_VEHICLE_HAVOC         },
    {    " Solid ",     pain::HIT_IMMOVEABLE,      object::TYPE_BUILDING              },
};

s32 nTweakValues = sizeof( TweakValues ) / sizeof( TweakValues[0] );
s32 TweakOrigin  = 0;
s32 TweakVictim  = 0;
s32 TweakCursor  = 0;

extern f32 PainMinSpeed[];
extern f32 PainMaxSpeed[];

//==============================================================================

void TweakMenu( void )
{
    //
    // Update
    //

    f32* pValue   = NULL;
    s32  PainType = TweakValues[ TweakOrigin ].PainType;
    s32  Victim   = TweakValues[ TweakVictim ].ObjType;
    f32& Damage   = g_MetalDamageTable[ PainType ][ Victim ];
    
    switch( TweakCursor )
    {
        case 0 : pValue = &Damage;                      break;
        case 1 : pValue = &PainMinSpeed[ TweakVictim ]; break;
        case 2 : pValue = &PainMaxSpeed[ TweakVictim ]; break;
    }

    ASSERT( pValue );

    f32  Speed    = 0.01f;
    f32  MaxValue = 2.0f;

    if( TweakCursor > 0 )
    {
        Speed    =   1.0f;
        MaxValue = 200.0f;
    }

    if( input_IsPressed ( INPUT_PS2_BTN_L_LEFT  ) ) *pValue -= Speed;
    if( input_IsPressed ( INPUT_PS2_BTN_L_RIGHT ) ) *pValue += Speed;
    if( input_WasPressed( INPUT_PS2_BTN_SQUARE  ) ) *pValue -= Speed;
    if( input_WasPressed( INPUT_PS2_BTN_CIRCLE  ) ) *pValue += Speed;
    if( input_WasPressed( INPUT_PS2_BTN_L_UP    ) ) TweakCursor--;
    if( input_WasPressed( INPUT_PS2_BTN_L_DOWN  ) ) TweakCursor++;
    if( input_WasPressed( INPUT_PS2_BTN_L2      ) ) TweakVictim--;
    if( input_WasPressed( INPUT_PS2_BTN_L1      ) ) TweakVictim++;
    if( input_WasPressed( INPUT_PS2_BTN_R2      ) ) TweakOrigin--;
    if( input_WasPressed( INPUT_PS2_BTN_R1      ) ) TweakOrigin++;
    
    *pValue     = MINMAX( 0, *pValue,     MaxValue       );
    TweakCursor = MINMAX( 0, TweakCursor, 2              );
    TweakOrigin = MINMAX( 0, TweakOrigin, nTweakValues-1 );
    TweakVictim = MINMAX( 0, TweakVictim, nTweakValues-2 );
    
    f32 Scalar   = Damage;
    f32 MinSpeed = PainMinSpeed[ TweakVictim ];
    f32 MaxSpeed = PainMaxSpeed[ TweakVictim ];

    //
    // Render
    //
    
    s32 Y = 8 + TweakCursor;
    
    x_printfxy(  2,  3, " Tweak Menu " );
    
    x_printfxy(  2,  5, " Victim:  " );
    x_printfxy( 20,  5, " PainType " );
    
    x_printfxy(  2,  6, TweakValues[ TweakVictim ].Name );
    x_printfxy( 20,  6, TweakValues[ TweakOrigin ].Name );
    
    x_printfxy(  0,  Y, ">>" );
    x_printfxy(  2,  8, " Scalar   : %1.2f ", Scalar   );
    x_printfxy(  2,  9, " MinSpeed : %3.0f",  MinSpeed );
    x_printfxy(  2, 10, " MaxSpeed : %3.0f",  MaxSpeed );
}

//==============================================================================

