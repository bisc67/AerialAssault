//==============================================================================
//
//  event_Render.cpp
//
//==============================================================================

#include "Entropy.hpp"
#include "Globals.hpp"
#include "GameClient.hpp"
#include "GameServer.hpp"
#include "Objects\Terrain\Terrain.hpp"
#include "Objects\PlaceHolder\PlaceHolder.hpp"
#include "Objects\Projectiles\Flag.hpp"
#include "Objects\Projectiles\Mine.hpp"
#include "Objects\Projectiles\SatchelCharge.hpp"
#include "Objects\Projectiles\Grenade.hpp"
#include "Objects\Projectiles\Disk.hpp"
#include "Objects\Projectiles\Plasma.hpp"
#include "Objects\Projectiles\Bullet.hpp"
#include "Objects\Projectiles\Laser.hpp"
#include "Objects\Projectiles\Generator.hpp"
#include "Objects\Projectiles\Sensor.hpp"
#include "Objects\Projectiles\Turret.hpp"
#include "Objects\Projectiles\Mortar.hpp"
#include "Objects\Projectiles\Blaster.hpp"
#include "Objects\Projectiles\WayPoint.hpp"
#include "Objects\Projectiles\FlipFlop.hpp"
#include "Objects\Projectiles\Nexus.hpp"
#include "Objects\Projectiles\Mortar.hpp"
#include "Objects\Projectiles\Bomb.hpp"
#include "Objects\Projectiles\Blaster.hpp"
#include "Objects\Projectiles\ShrikeShot.hpp"
#include "Objects\Projectiles\Generic.hpp"
#include "Objects\Projectiles\ParticleObject.hpp"
#include "Objects\Projectiles\Missile.hpp"
#include "Objects\Projectiles\Beacon.hpp"
#include "Objects\Projectiles\Debris.hpp"
#include "Objects\Projectiles\Bubble.hpp"
#include "Objects\Projectiles\ForceField.hpp"
#include "Objects\Projectiles\Station.hpp"
#include "Objects\Projectiles\VehiclePad.hpp"
#include "Objects\Projectiles\Projector.hpp"
#include "Objects\Projectiles\AutoAim.hpp"
#include "Objects\Projectiles\Bounds.hpp"
#include "Particles\PostFX.hpp"
#include "Objects\Player\PlayerObject.hpp"
#include "Objects\Player\CorpseObject.hpp"
#include "Objects\Bot\BotObject.hpp"
#include "Objects\Vehicles\gravcycle.hpp"
#include "Objects\Vehicles\asstank.hpp"
#include "Objects\Vehicles\shrike.hpp"
#include "Objects\Vehicles\transport.hpp"
#include "Objects\Vehicles\bomber.hpp"
#include "Objects\Vehicles\mpb.hpp"
#include "Objects\Scenic\scenic.hpp"
#include "Building\BuildingOBJ.hpp"
#include "Sky\Sky.hpp"
#include "ObjectMgr\ColliderCannon.hpp"
#include "PointLight\Pointlight.hpp"

#include "ui\ui_manager.hpp"
#include "hud\hud_manager.hpp"
#include "HUD\hud_Icons.hpp"

#include "GameMgr\GameMgr.hpp"
#include "audiomgr\audio.hpp"
#include "sm_common.hpp"

#ifdef TARGET_PC
#include "BotEditor.hpp"
#endif

//static xbool SHOW_PLAYER_UPDATE_STATS = FALSE;
static xbool SHOW_LIFE_DELAY_DATA     = FALSE;

extern xbool DO_SCREEN_SHOTS;
extern xbool   HUD_HideHUD;
xtimer BotEditorTimer;
//void blur_Begin( f32 Alpha );

static X_FILE* pLogFile = NULL;

void LOG_STATS( const char* pFormatStr, ... )
{
    if( !pLogFile )
    {
        pLogFile = x_fopen("log.txt","wt" );
        if( !pLogFile ) return;
    }

    x_va_list   Args;
    x_va_start( Args, pFormatStr );

    xvfs XVFS( pFormatStr, Args );

    x_fprintf( pLogFile, "%s", (const char*)XVFS );
}

//==============================================================================

xbool SHOW_SPAWN_POINTS=FALSE;
extern xtimer TIMER_PrepRender;
extern xtimer TIMER_PrepRender2;
extern xtimer TIMER_PrepRender3;
extern xtimer TIMER_PrepRender4;
extern s32 LoadMissionStage;

xbool SHOW_BLENDING       = FALSE;
xbool PROJECTILE_BLENDING = TRUE;
vector3 Set1[50];
vector3 Set2[50];
s32     SetI;

//extern ui_manager *pFrontEnd;

volatile f32 VIEW_Z_NEAR = 0.1f;
volatile f32 VIEW_Z_FAR  = 1000.0f;

volatile xcolor BackgroundColor(93,100,124,255);

extern f32 VIEW_X_RATIO;
extern f32 VIEW_Y_RATIO;
extern radian VIEW_XFOV;
extern f32 VIEW_XFOV_RATIO;

static player_object* s_pBotBeingObserved = NULL;

extern void test_draw_player() ;

xbool BUILDING_DRAW_MARKER = FALSE;


xbitmap* s_pPingBitmap=NULL;
//==============================================================================

xbool HandleSpecialRenders( s32 ViewIndex )
{
    if( ViewIndex == 1 )
        return FALSE;

    if( g_SM.MissionLoading )
    {
/*
        static s32 T=0;
        s32 Y=0;

        eng_SetBackColor(xcolor(63,63,63,255));

        x_printfxy(0,Y++,"Loading mission..." );
        x_printfxy(0,Y++,"Character name: %s",tgl.UserInfo.MyName);

        if( tgl.ServerPresent )
            x_printfxy(0,Y++,"Hosting game as server (%s)",tgl.UserInfo.Server);

        if( tgl.ClientPresent )
            x_printfxy(0,Y++,"Joining game run on (%s)",tgl.UserInfo.Server);

        x_printfxy((T%8),Y++,"*" );
        T++;
*/
        return TRUE;
    }

/*
    if( tgl.pUIManager )
    {
        view    v;

        // Activate view
        eng_MaximizeViewport( v );
        v.SetZLimits ( VIEW_Z_NEAR, VIEW_Z_FAR );
//        v.SetZLimits( 0.1f, F32_MAX );
        eng_SetView( v, 0 );
        eng_ActivateView ( 0 );

        s32 X0,Y0,X1,Y1;
        v.GetViewport( X0,Y0,X1,Y1 );

#ifdef TARGET_PS2
        eng_Begin( "Scissor" );
        gsreg_Begin();
        gsreg_SetScissor(X0,Y0,X1,Y1);
        eng_PushGSContext(1);
        gsreg_SetScissor(X0,Y0,X1,Y1);
        eng_PopGSContext();
        gsreg_End();
        eng_End();
#endif

        if( tgl.GameStage == GAME_STAGE_FRONTEND )
            tgl.pUIManager->Render();
    }
*/
/*
    if( tgl.ClientPresent )
    {
        if (!s_pPingBitmap)
        {
            s_pPingBitmap = new xbitmap;
            s_pPingBitmap->Setup(xbitmap::FMT_P8_ABGR_8888,         // Format
                                 32,16,                             // Width, height
                                 TRUE,(byte*)x_malloc(32*16),       // Bitmap owned & data (will be auto freed on kill)
                                 TRUE,(byte*)x_malloc(256*4));      // CMAP owned & data   (will be auto freed on kill)
            ASSERT(s_pPingBitmap->GetPixelData());
            ASSERT(s_pPingBitmap->GetClutData());
        }
        tgl.pClient->BuildPingGraph(*s_pPingBitmap);
        vram_Register(*s_pPingBitmap);
        draw_SetTexture(*s_pPingBitmap);

        draw_Sprite(vector3(0.0f,0.0f,0.0f),vector2(32,16),XCOLOR_WHITE);
        vram_Unregister(*s_pPingBitmap);

    }
    else
    {
        if (s_pPingBitmap)
        {
            delete s_pPingBitmap;
            s_pPingBitmap = NULL;
        }
    }
*/

    return FALSE;
}

//==============================================================================

void SetupPlayerCameras( void )
{
    //////////////////////////////////////////////////////////////////////
    // This code is to cycle the camera through all the bots by hitting
    // F4. Shift-F4 returns the camera to normal
    //////////////////////////////////////////////////////////////////////
    static xbool s_F4IsStillDown = FALSE; // stop key repeating...

    if ( input_IsPressed( INPUT_KBD_F4 ) && !s_F4IsStillDown )
    {
        s_F4IsStillDown = TRUE;
        if ( input_IsPressed( INPUT_KBD_LSHIFT )
            || input_IsPressed( INPUT_KBD_RSHIFT ) )
        {
            // no longer look at bots
            s_pBotBeingObserved = NULL;
        }
        else
        {
            // set the bot being observed to the next bot
            ObjMgr.StartTypeLoop( object::TYPE_BOT );

            if ( s_pBotBeingObserved == NULL )
            {
                // we're not looking at a bot yet,
                // so just look at the first one in the list
                s_pBotBeingObserved
                    = (player_object*)ObjMgr.GetNextInType();
            }
            else
            {
                // find the bot we're looking at, and look at the next one
                player_object *pPlayer;
                xbool FoundNextPlayer = FALSE;

                while ( !FoundNextPlayer
                    && ((pPlayer = (player_object*)ObjMgr.GetNextInType()) != NULL) )
                {
                    
                    if ( s_pBotBeingObserved == pPlayer )
                    {
                        // found the one we're looking at...
                        // get the next one
                        if( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
                        {
                            s_pBotBeingObserved = pPlayer;
                            FoundNextPlayer = TRUE;
                        }
                        else
                        {
                            // ok, that was the last one, get the first one
                            ObjMgr.EndTypeLoop();
                            ObjMgr.StartTypeLoop( object::TYPE_BOT );
                            s_pBotBeingObserved
                                = (player_object*)ObjMgr.GetNextInType();
                            FoundNextPlayer = TRUE;
                        }
                    }
                }
            
                ASSERT( FoundNextPlayer );
            }
            
            ObjMgr.EndTypeLoop();
        }
        if ( s_pBotBeingObserved != NULL )
        {
            x_printf( "Observing %s\n", s_pBotBeingObserved->GetName() );
        }
    }
    else if ( !input_IsPressed( INPUT_KBD_F4 ) )
    {
        s_F4IsStillDown = FALSE;
    }

    // Make sure the controlling players setup their view before any drawing takes place
    {
		player_object *pPlayer ;
        ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
        while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
        {
            pPlayer->SetupView() ;
        }
        ObjMgr.EndTypeLoop();
    }

    if ( s_pBotBeingObserved != NULL )
    {
        vector3 ViewPos;
        radian3 ViewRot;
        f32 ViewFOV;

        s_pBotBeingObserved->Get3rdPersonView(
            ViewPos
            , ViewRot
            , ViewFOV );

        tgl.FreeCam[0].SetPosition( ViewPos );
        tgl.FreeCam[0].SetRotation( ViewRot );
        tgl.FreeCam[0].SetXFOV( ViewFOV );
    }

#ifdef nmreed 
#define FIND_BOT
#endif
#ifdef nacheng
#define FIND_BOT
#endif

#ifdef FIND_BOT
    // Find the Bot and follow him
    bot_object *pBot ;
    ObjMgr.StartTypeLoop( object::TYPE_BOT );
    if ( pBot = (bot_object*)ObjMgr.GetNextInType() )
    {
        tgl.FreeCam[0].OrbitPoint(
            pBot->GetDrawPos() + vector3( 0.0f, 5.0f, 0.0f )
            , 10.0f
            , -R_10
            , pBot->GetDrawRot().Yaw + R_180 );
    }
    ObjMgr.EndTypeLoop();
#endif

}

//==============================================================================

volatile xbool RENDER_TERRAIN         = TRUE;
         xbool g_FlickerIndoorTerrain = FALSE;
extern   xbool g_FlickerTerrainHoles;


void RenderSkyFogTerrain( void )
{
    // Update fog with eye position
    {
        tgl.FogSetupCPUTime.Start();
        const view* pView = eng_GetActiveView(0);
        tgl.Fog.SetEyePos( pView->GetPosition() );
        tgl.FogSetupCPUTime.Stop();
    }

    // Render sky
    if( g_BldManager.IsZoneZeroVisible( 0 ) )
    {
        eng_Begin( "Sky" );
        tgl.RenderSkyCPUTime.Start();
        tgl.Sky.Render();
        tgl.RenderSkyCPUTime.Stop();
        eng_End();
    }

    if( !RENDER_TERRAIN )
        return;

    xbool RenderTerrain = TRUE;

    if( !g_BldManager.IsZoneZeroVisible( 0 ) )
    {
        // Normally, we do not render terrain if zone 0 isn't visible.
        // But, for debugging, we may ignore this.

        RenderTerrain = FALSE;

        if( g_FlickerIndoorTerrain && (x_rand() & 0x01) )
            RenderTerrain = TRUE;

        if( g_FlickerTerrainHoles )
            RenderTerrain = TRUE;
    }

    // Render terrain.
    if( RenderTerrain )
    {
        tgl.RenderTerrainCPUTime.Start();
        eng_Begin( "Terrain" );
        {
            terrain* pTerrain;
            ObjMgr.StartTypeLoop( object::TYPE_TERRAIN );
            while( (pTerrain = (terrain*)ObjMgr.GetNextInType()) )
            {
                pTerrain->m_Terrain.Render( TRUE, TRUE );
            }
            ObjMgr.EndTypeLoop();
        }
        eng_End();
        tgl.RenderTerrainCPUTime.Stop();
    }
}

//==============================================================================

static object::type BaseShapeRenderTypes[] = 
{
    //object::TYPE_PLAYER,  // Player is now rendered separately (so we can draw him last if needs be)
    object::TYPE_BOT,
    object::TYPE_CORPSE,
    object::TYPE_VEHICLE_WILDCAT,
    object::TYPE_VEHICLE_BEOWULF,
    object::TYPE_VEHICLE_SHRIKE,
    object::TYPE_VEHICLE_HAVOC,
    object::TYPE_VEHICLE_THUNDERSWORD,
    object::TYPE_VEHICLE_JERICHO2,
    object::TYPE_FLAG,
    object::TYPE_PICKUP,
    object::TYPE_MINE,
    object::TYPE_SATCHEL_CHARGE,
    object::TYPE_GRENADE,
    object::TYPE_MISSILE,
    object::TYPE_MORTAR,
    object::TYPE_BOMB,
    object::TYPE_DEBRIS,
    object::TYPE_STATION_FIXED,
    object::TYPE_STATION_VEHICLE,
    object::TYPE_STATION_DEPLOYED,
    object::TYPE_VPAD,
    object::TYPE_SENSOR_LARGE,
    object::TYPE_SENSOR_MEDIUM,
    object::TYPE_SENSOR_REMOTE,
    object::TYPE_TURRET_FIXED,
    object::TYPE_TURRET_SENTRY,
    object::TYPE_TURRET_CLAMP,
    object::TYPE_GENERATOR,
    object::TYPE_BEACON,
    object::TYPE_FLIPFLOP,
    object::TYPE_NEXUS,
    object::TYPE_PROJECTOR,
    object::TYPE_SCENIC,
};

static s32 NBaseShapeRenderTypes = sizeof( BaseShapeRenderTypes ) / 4;

//------------------------------------------------------------------------------

extern xtimer TerrainOccTimer;
extern s32    TerrainOccNCalls;
extern s32    TerrainOccNOccluded;
extern s32    TerrainOccNRays;
extern s32    TerrainOccNRaysFound;
extern xbool g_InGame;

//------------------------------------------------------------------------------

void event_Render( s32 ViewIndex )
{
    tgl.NRenders++;
    tgl.WC = ViewIndex;

    //TerrainOccTimer.Reset();
    //TerrainOccNCalls = 0;
    //TerrainOccNOccluded = 0;
    //TerrainOccNRays = 0;
    //TerrainOccNRaysFound = 0;

    // A pointer to the player to be rendered with transparency
    player_object* pAlphaPlayer = NULL;

    // Handle special screens
    if( HandleSpecialRenders( ViewIndex ) )
        return;

    if( !g_SM.MissionRunning )
    {
        eng_SetBackColor( XCOLOR_BLACK );
        return;
    }

    // Allow camera to cycle through bots
    SetupPlayerCameras();

    // Start up timers
    tgl.RenderCPUTime.Reset();
    tgl.RenderCPUTime.Start();
    tgl.RenderBuildingsPrepCPUTime.Reset();
    tgl.RenderTerrainCPUTime.Reset();
    tgl.RenderBuildingsCPUTime.Reset();
    tgl.RenderProjectilesCPUTime.Reset();
    tgl.RenderSkyCPUTime.Reset();
    tgl.RenderParticlesCPUTime.Reset();
    tgl.RenderShapesCPUTime.Reset();
    tgl.FogSetupCPUTime.Reset();

//    eng_SetBackColor( tgl.Fog.GetHazeColor() );//(const xcolor&)BackgroundColor);
    eng_SetBackColor( XCOLOR_BLACK );//(const xcolor&)BackgroundColor);

	// Setup camera viewport
//  if( tgl.UseFreeCam[0] )
    {
        s32 X0,X1,Y0,Y1;
        view& View = tgl.GetView(ViewIndex);

        // Make sure big screen shot is in place so 2d icons are in projected correclty!
        View.SetSubShot(eng_ScreenShotX(), eng_ScreenShotY(), eng_ScreenShotSize()) ;

        View.SetZLimits ( VIEW_Z_NEAR, VIEW_Z_FAR );
        eng_MaximizeViewport( View );
        View.GetViewport(X0,Y0,X1,Y1);

        if( ViewIndex == 0 )
        {
            X0 = (s32)(0);
            X1 = (s32)(VIEW_X_RATIO * X1);
            Y0 = (s32)(0);
            Y1 = (s32)(VIEW_Y_RATIO * Y1);
        }
        else
        {
            X0 = (s32)(0);
            X1 = (s32)(VIEW_X_RATIO * X1);
            Y0 = (s32)((1.0f-VIEW_Y_RATIO) * Y1);
            Y1 = (s32)(Y1);
        }

        View.SetViewport(X0,Y0,X1,Y1);

        View.SetXFOV( View.GetXFOV() * VIEW_XFOV_RATIO );
        eng_SetView( View, 0 );
        eng_ActivateView( 0 );

        #ifdef TARGET_PS2
        eng_Begin( "Scissor" );
        gsreg_Begin();
        gsreg_SetScissor(X0,Y0,X1,Y1);
        eng_PushGSContext(1);
        gsreg_SetScissor(X0,Y0,X1,Y1);
        eng_PopGSContext();
        gsreg_End();
        eng_End();
        #endif
    }

    /*
    // SB - TEMP TO TEST MULTI SCREEN SHOT!
    // render the head first
    eng_Begin("test") ;
    draw_ClearL2W() ;
    draw_Begin( DRAW_SPRITES, 0 );
    vector3 ViewPos  = vector3(0,0,10) ;
    vector3 WorldPos = eng_GetActiveView(0)->ConvertV2W(ViewPos) ;
    draw_SpriteUV( WorldPos, vector2(2.0f,2.0f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, 0 ) ;
    draw_End() ;
    eng_End() ;
    */

    //
    // Prep The buildings
    //
    eng_Begin( "PrepBuildings" );
    {
        g_BldManager.ClearZoneInfo();
        s32 NBuildings=0;
        tgl.RenderBuildingsPrepCPUTime.Start();
        // Prepare to render all buildings
		building_obj* pBuilding ;
        ObjMgr.StartTypeLoop( object::TYPE_BUILDING );
        while( (pBuilding = (building_obj*)ObjMgr.GetNextInType()) )
        {
            pBuilding->m_Instance.PrepRender() ;
            if( BUILDING_DRAW_MARKER )
            {
                draw_Marker( pBuilding->GetPosition(), XCOLOR_RED );
                draw_BBox( pBuilding->GetBBox(), XCOLOR_RED );
            }
            NBuildings++;

        }
        ObjMgr.EndTypeLoop();
        tgl.RenderBuildingsPrepCPUTime.Stop();

        if( BUILDING_DRAW_MARKER )
            x_printfxy(0,2,"NBuildings %1d",NBuildings);
    }
    eng_End();

    //x_printfxy(0,0,"ZoneZero %1d",g_BldManager.IsZoneZeroVisible( 0 ));

    //
    // Render the Sky, Terrain, Setup FOG
    //
    RenderSkyFogTerrain();

    //
    // Render the actual buildings
    //
    if( 1 )
    {
        eng_Begin( "Buildings" );

        tgl.RenderBuildingsCPUTime.Start();
		g_BldManager.Render() ;
        tgl.RenderBuildingsCPUTime.Stop();

        if( tgl.RenderBuildingGrid )
        {
    		building_obj* pBuilding;
            ObjMgr.StartTypeLoop( object::TYPE_BUILDING );
            while( (pBuilding = (building_obj*)ObjMgr.GetNextInType()) )
                pBuilding->RenderGrid();
            ObjMgr.EndTypeLoop();
        }

        eng_End();
    }

	// Render player shadows etc
    eng_Begin( "PlayerPre" );
    {
        tgl.RenderShapesCPUTime.Start();

        // Players
		player_object* pPlayer;
        ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
        while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
            pPlayer->PreRender();
        ObjMgr.EndTypeLoop();

        // Bots
        bot_object *pBot;
        ObjMgr.StartTypeLoop( object::TYPE_BOT );
        while( (pBot = (bot_object*)ObjMgr.GetNextInType()) )
            pBot->PreRender();
        ObjMgr.EndTypeLoop();

        tgl.RenderShapesCPUTime.Stop();
    }
    eng_End();
	
	// Render objects that use the shape library
    if( TRUE )
    {
        tgl.RenderShapesCPUTime.Start();
        eng_Begin( "Shapes" );	

		shape::BeginDraw();

        // Render all shape based objects which only need to call their 
        // virtual Render() function.
        {
            s32     i;
            object* pObject;
            player_object* pPlayer;
            
            view& View = tgl.GetView( ViewIndex );
            
            // Render all player shapes
            ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
            
            // Find the player with this view and check if he needs to be rendered with transparency
            while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
            {
                xbool DoRender = TRUE;
            
                // If player needs transparency due to a close-in camera, then dont render him now.
                // We will render him at the end of the render loop.
                if( (pPlayer->GetView()           == &View)                               &&
                    (pPlayer->GetPlayerViewType() == player_object::VIEW_TYPE_3RD_PERSON) &&
                    (pPlayer->GetCamDistanceT()   <  1.0f) )
                {
                    ASSERT( pAlphaPlayer == NULL );
                    pAlphaPlayer = pPlayer;
                    DoRender     = FALSE;
                }
                
                if( DoRender == TRUE )
                    pPlayer->Render();
            }
            ObjMgr.EndTypeLoop();
            
            // Render all other shapes
            for( i = 0; i < NBaseShapeRenderTypes; i++ )
            {   
                object::type Type = BaseShapeRenderTypes[i];

                ObjMgr.StartTypeLoop( Type );
                while( (pObject = ObjMgr.GetNextInType()) )
                    pObject->Render();
                ObjMgr.EndTypeLoop();
            }
        }

        // Render disks
        {
            disk* pDisk;
            ObjMgr.StartTypeLoop( object::TYPE_DISK );
            while( (pDisk = (disk*)ObjMgr.GetNextInType()) )
            {
                pDisk->RenderCore();
                pDisk->RenderEffects();
            }
            ObjMgr.EndTypeLoop();
        }

		shape::EndDraw();

        // Render boxes around players and flags.
        if( FALSE )
        {
            flag* pFlag;
            ObjMgr.StartTypeLoop( object::TYPE_FLAG );
            while( (pFlag = (flag*)ObjMgr.GetNextInType()) )
            {
                draw_BBox( pFlag->GetBBox(), XCOLOR_YELLOW );
            }
            ObjMgr.EndTypeLoop();

			player_object *pPlayer;
            ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
            while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
            {
                draw_BBox( pPlayer->GetBBox(), XCOLOR_RED );
            }
            ObjMgr.EndTypeLoop();
        }

        // Render ray collision information from buildings.
        if( FALSE )
        {
			building_obj *pBuilding;
            ObjMgr.StartTypeLoop( object::TYPE_BUILDING );
            while( (pBuilding = (building_obj*)ObjMgr.GetNextInType()) )
            {
                pBuilding->RenderRayNGons();
            }
            ObjMgr.EndTypeLoop();
        }

        eng_End();
        tgl.RenderShapesCPUTime.Stop(); 
    }

	// Render player particles etc
    eng_Begin( "Particles Etc" );
    {
        tgl.RenderShapesCPUTime.Start();

        // Player
		player_object *pPlayer ;
        ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
        while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
        {
            pPlayer->PostRender();
        }
        ObjMgr.EndTypeLoop();

        // Bot
		bot_object *pBot ;
        ObjMgr.StartTypeLoop( object::TYPE_BOT );
        while( (pBot = (bot_object*)ObjMgr.GetNextInType()) )
        {
            pBot->PostRender();
        }
        ObjMgr.EndTypeLoop();

        tgl.RenderShapesCPUTime.Stop();
    }

    // Turret
    {
        turret* pTurret;
        ObjMgr.StartTypeLoop( object::TYPE_TURRET_FIXED );
        while( (pTurret = (turret*)ObjMgr.GetNextInType()) )
        {
            pTurret->RenderEffects();
        }
        ObjMgr.EndTypeLoop();
    }

    // Render plasma
    {
        plasma* pPlasma;

        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            plasma::BeginRender();

            ObjMgr.StartTypeLoop( object::TYPE_PLASMA );
            while( (pPlasma = (plasma*)ObjMgr.GetNextInType()) )
            {
                pPlasma->Render();
            }
            ObjMgr.EndTypeLoop();

            plasma::EndRender();
        }
    }

    // render flare grenades
    {
        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            grenade* pGrenade;
            ObjMgr.StartTypeLoop( object::TYPE_GRENADE );
            while( (pGrenade = (grenade*)ObjMgr.GetNextInType()) )
            {
                if ( pGrenade->GetType() == grenade::TYPE_FLARE )
                    pGrenade->RenderFlare();
            }
            ObjMgr.EndTypeLoop();
        
        }
    }

    // Render bullet
    {
        bullet* pBullet;

        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            s32 i;

            for ( i = 0; i < 2; i++ )
            {
                bullet::BeginRender( i );

                ObjMgr.StartTypeLoop( object::TYPE_BULLET );
                while( (pBullet = (bullet*)ObjMgr.GetNextInType()) )
                {
                    pBullet->Render( i );
                }
                ObjMgr.EndTypeLoop();

                bullet::EndRender();
            }

        }
    }

    // Render sniper laser fire
    {
        laser* pLaser;

        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            laser::BeginRender();

            ObjMgr.StartTypeLoop( object::TYPE_LASER );
            while( (pLaser = (laser*)ObjMgr.GetNextInType()) )
            {
                pLaser->Render();
            }
            ObjMgr.EndTypeLoop();

            laser::EndRender();
        }
    }

    // Render blaser fire
    {
        blaster* pBlaster;

        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            blaster::BeginRender();

            ObjMgr.StartTypeLoop( object::TYPE_BLASTER );
            while( (pBlaster = (blaster*)ObjMgr.GetNextInType()) )
            {
                pBlaster->Render();
            }
            ObjMgr.EndTypeLoop();

            blaster::EndRender();
        }
    }

    // Render shrike fire
    {
        shrike_shot* pShot;

        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            shrike_shot::BeginRender();

            ObjMgr.StartTypeLoop( object::TYPE_SHRIKESHOT );
            while( (pShot = (shrike_shot*)ObjMgr.GetNextInType()) )
            {
                pShot->Render();
            }
            ObjMgr.EndTypeLoop();

            shrike_shot::EndRender();
        }
    }
/*
    // Render bomber fire
    {
        bomber_shot* pShot;

        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            bomber_shot::BeginRender();

            ObjMgr.StartTypeLoop( object::TYPE_BOMBERSHOT );
            while( (pShot = (bomber_shot*)ObjMgr.GetNextInType()) )
            {
                pShot->Render();
            }
            ObjMgr.EndTypeLoop();

            bomber_shot::EndRender();
        }
    }
*/
    // Render generic fire
    {
        generic_shot* pShot;

        //if ( tgl.GameStage == GAME_STAGE_INGAME )
        {
            ////////////////////////////
            // render AA Turret fire
            generic_shot::BeginRender( generic_shot::AA_TURRET );
            ObjMgr.StartTypeLoop( object::TYPE_GENERICSHOT );
            while( (pShot = (generic_shot*)ObjMgr.GetNextInType()) )
            {
                // check for appropriate kind
                if ( pShot->GetKind() == generic_shot::AA_TURRET )
                    pShot->Render();
            }
            ObjMgr.EndTypeLoop();
            generic_shot::EndRender();

            ////////////////////////////
            // render Plasma Turret fire
            generic_shot::BeginRender( generic_shot::PLASMA_TURRET );
            ObjMgr.StartTypeLoop( object::TYPE_GENERICSHOT );
            while( (pShot = (generic_shot*)ObjMgr.GetNextInType()) )
            {
                // check for appropriate kind
                if ( pShot->GetKind() == generic_shot::PLASMA_TURRET )
                    pShot->Render();
            }
            ObjMgr.EndTypeLoop();
            generic_shot::EndRender();

            ////////////////////////////
            // render Clamp Turret fire
            generic_shot::BeginRender( generic_shot::CLAMP_TURRET );
            ObjMgr.StartTypeLoop( object::TYPE_GENERICSHOT );
            while( (pShot = (generic_shot*)ObjMgr.GetNextInType()) )
            {
                // check for appropriate kind
                if ( pShot->GetKind() == generic_shot::CLAMP_TURRET )
                    pShot->Render();
            }
            ObjMgr.EndTypeLoop();
            generic_shot::EndRender();

        }
    }

    // Render particles
    {
        tgl.RenderParticlesCPUTime.Start();
        shape::BeginDraw();
        particle_object* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_PARTICLE_EFFECT );
        while( (pObject = (particle_object*)ObjMgr.GetNextInType()) )
            pObject->Render();
        ObjMgr.EndTypeLoop();
        shape::EndDraw();
        tgl.RenderParticlesCPUTime.Stop();
    }

    // render grenade smoke particles
    {
        grenade* pGrenade;
        ObjMgr.StartTypeLoop( object::TYPE_GRENADE );
        while( (pGrenade = (grenade*)ObjMgr.GetNextInType()) )
        {
            pGrenade->RenderParticles();
        }
        ObjMgr.EndTypeLoop();
    }

    // render missile smoke particles
    {
        missile* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_MISSILE );
        while( (pObject = (missile*)ObjMgr.GetNextInType()) )
        {
            pObject->RenderParticles();
        }
        ObjMgr.EndTypeLoop();
    }

    // render mortar smoke particles
    {
        mortar* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_MORTAR );
        while( (pObject = (mortar*)ObjMgr.GetNextInType()) )
        {
            pObject->RenderParticles();
        }
        ObjMgr.EndTypeLoop();
    }

    // render generic shot particles
    {
        generic_shot* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_GENERICSHOT );
        while( (pObject = (generic_shot*)ObjMgr.GetNextInType()) )
        {
            pObject->RenderParticles();
        }
        ObjMgr.EndTypeLoop();
    }

    // render debris particles
    {
        debris* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_DEBRIS );
        while( (pObject = (debris*)ObjMgr.GetNextInType()) )
        {
            pObject->RenderParticles();
        }
        ObjMgr.EndTypeLoop();
    }


    // Render gravcycle particles
    {
        gravcycle* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VEHICLE_WILDCAT );
        while( (pObject = (gravcycle*)ObjMgr.GetNextInType()) )
            pObject->RenderParticles();
        ObjMgr.EndTypeLoop();
    }

    // Render shrike particles
    {
        shrike* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VEHICLE_SHRIKE );
        while( (pObject = (shrike*)ObjMgr.GetNextInType()) )
            pObject->RenderParticles();
        ObjMgr.EndTypeLoop();
    }

    // Render bomber particles
    {
        bomber* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VEHICLE_THUNDERSWORD );
        while( (pObject = (bomber*)ObjMgr.GetNextInType()) )
            pObject->RenderParticles();
        ObjMgr.EndTypeLoop();
    }

    // Render asstank particles
    {
        asstank* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VEHICLE_BEOWULF );
        while( (pObject = (asstank*)ObjMgr.GetNextInType()) )
            pObject->RenderParticles();
        ObjMgr.EndTypeLoop();
    }

    // Render MPB particles
    {
        mpb* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VEHICLE_JERICHO2 );
        while( (pObject = (mpb*)ObjMgr.GetNextInType()) )
            pObject->RenderParticles();
        ObjMgr.EndTypeLoop();
    }

    // Render transport particles
    {
        transport* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VEHICLE_HAVOC );
        while( (pObject = (transport*)ObjMgr.GetNextInType()) )
            pObject->RenderParticles();
        ObjMgr.EndTypeLoop();
    }

    // Render Bomber particles
    {
        bomber* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VEHICLE_THUNDERSWORD );
        while( (pObject = (bomber*)ObjMgr.GetNextInType()) )
            pObject->RenderParticles();
        ObjMgr.EndTypeLoop();
    }

    // Render vehicle spawn effects
    {
        vehicle_pad* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_VPAD );
        while( (pObject = (vehicle_pad*)ObjMgr.GetNextInType()) )
            pObject->RenderSpawnFX();
        ObjMgr.EndTypeLoop();
    }

    // END of Particles Etc
    eng_End();

    // Render point lights
    eng_Begin( "PointLights" );
    {
        ptlight_Render();
    }
    eng_End();

    eng_Begin( "Shadows" );
    {
        //shadow_Render( -tgl.Sun );
    }
    eng_End();

    // Render bubble effects
    eng_Begin( "Bubble" );
    shape::BeginDraw();
    {
        object* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_BUBBLE );
        while( (pObject = ObjMgr.GetNextInType()) )
            pObject->Render();
        ObjMgr.EndTypeLoop();
    }
    shape::EndDraw();
    eng_End();

    // Render force fields
    eng_Begin( "Force Field" );
    {
        object* pObject;
        ObjMgr.StartTypeLoop( object::TYPE_FORCE_FIELD );
        while( (pObject = ObjMgr.GetNextInType()) )
            pObject->Render();
        ObjMgr.EndTypeLoop();
    }
    eng_End();

    // Bounds
    eng_Begin( "GameBounds" );
    {
        bounds_Render( GameMgr.GetBounds() );
    }
    eng_End();

    // Process way points.
    eng_Begin( "WayPoints" );
    if( !HUD_HideHUD && !eng_ScreenShotActive() )
    {
        waypoint* pWayPoint;
        ObjMgr.StartTypeLoop( object::TYPE_WAYPOINT );
        while( (pWayPoint = (waypoint*)ObjMgr.GetNextInType()) )
            pWayPoint->Render();
        ObjMgr.EndTypeLoop();
    }
    eng_End();

    // Render place holders.
    eng_Begin( "PlaceHolders" );
    {
        place_holder* pPlaceHolder;
        ObjMgr.StartTypeLoop( object::TYPE_PLACE_HOLDER );
        while( (pPlaceHolder = (place_holder*)ObjMgr.GetNextInType()) )
            pPlaceHolder->Render();
        ObjMgr.EndTypeLoop();
    }
    eng_End();

    // Let the game logic render anything it needs to.
    pGameLogic->Render();

    // Almost last, but certainly not least, render post FX (glowy spots)
    eng_Begin( "Glowy Effects" );
    {
        tgl.PostFX.Render();
        tgl.PostFX.Reset();
    }
    eng_End();

    // Render aim hint and target lock.
    eng_Begin( "AutoAim" );
    RenderAutoAimHint();
    eng_End();

    // Render the player with transparency
    if( pAlphaPlayer != NULL )
    {
        eng_Begin( "AlphaPlayer" );
        shape::BeginDraw();
        pAlphaPlayer->Render();
        shape::EndDraw();
        eng_End();
    }

    // Render hud icons.
    if( !tgl.UseFreeCam[ViewIndex] && !eng_ScreenShotActive() )
        HudIcons.Render();
    else
        HudIcons.Clear();

    // Render HUD
    //tgl.pHUDManager->Render();

    // Render UI
    //tgl.pUIManager->Render();

    /*
    eng_Begin();
    {
        s32 NViews = eng_GetNActiveViews();
        for( s32 i=0; i<NViews; i++ )
        {
            s32 X0,Y0,X1,Y1;
            f32 MX,MY,S=4;
            const view* pView = eng_GetActiveView(i);
            pView->GetViewport(X0,Y0,X1,Y1);
            MX = (X0+X1)*0.5f;
            MY = (Y0+Y1)*0.5f;

            draw_Begin(DRAW_LINES, DRAW_2D);
            draw_Color(XCOLOR_RED);
            draw_Vertex(MX+0,MY-S,0);   draw_Vertex(MX-S,MY+0,0);
            draw_Vertex(MX-S,MY+0,0);   draw_Vertex(MX+0,MY+S,0);
            draw_Vertex(MX+0,MY+S,0);   draw_Vertex(MX+S,MY+0,0);
            draw_Vertex(MX+S,MY+0,0);   draw_Vertex(MX+0,MY-S,0);
            draw_End();
        }
    }
    eng_End();
    */
/*
    if( input_IsPressed(INPUT_PS2_BTN_TRIANGLE) )
    {
        eng_Begin();
        blur_Begin(1.0f);
        eng_End();
    }
*/
    // Show the collider cannon.
    if( tgl.ShowColliderCannon )
    {
        eng_Begin( "CollideCannon" );
        cc_Render();
        eng_End();
    }

    // Render volumes for debugging
    eng_Begin( "Volumes" );
    GameMgr.RenderVolume();
    eng_End();

    // Building debug rendering
    eng_Begin( "BuildingDebug" );
    RenderBuildingDebug();
    eng_End();

    tgl.RenderCPUTime.Stop();

    //
    // Give a chance to the editor to insert in rendering stuff 
    //

#ifdef TARGET_PC
    BotEditorTimer.Start();
    BOTEDIT_ProcessLoop();
    BotEditorTimer.Stop();
#endif

    //#ifdef mtraub
    GameMgr.DebugRender();
    //#endif

    if( tgl.DisplayObjectStats )
    {
        ObjMgr.PrintStats();
        
        //x_printfxy(20,20,"CHN:%d",audio_GetChannelsInUse());
        
        if((tgl.NRenders%(30*4))==0)
        {
            if( tgl.pServer )
            {
                tgl.pServer->DumpStats();
            }
        }        
    }

    if( SHOW_LIFE_DELAY_DATA && tgl.ServerPresent )
    {
        tgl.pServer->ShowLifeDelay();
    }

    if( tgl.LogStats && ((tgl.NRenders%(60*4))==0))
    {
        LOG_STATS("--------------------------------------------------\n" );
        LOG_STATS("ObjMgrLogic    %5.2f\n",tgl.ObjMgrLogicTime.ReadMs());
        LOG_STATS("ServerProcess  %5.2f\n",tgl.ServerProcessTime.ReadMs());
        LOG_STATS("ClientProcess  %5.2f\n",tgl.ClientProcessTime.ReadMs());
        LOG_STATS("FogSetupCPU    %5.2f\n",tgl.FogSetupCPUTime.ReadMs());
        LOG_STATS("RenderSky      %5.2f\n",tgl.RenderSkyCPUTime.ReadMs());
        LOG_STATS("RenderTerrain  %5.2f\n",tgl.RenderTerrainCPUTime.ReadMs());
        LOG_STATS("PrepBuilding   %5.2f\n",tgl.RenderBuildingsPrepCPUTime.ReadMs());
        LOG_STATS("RenderBuilding %5.2f\n",tgl.RenderBuildingsCPUTime.ReadMs());
        LOG_STATS("RenderShapes   %5.2f\n",tgl.RenderShapesCPUTime.ReadMs());
        LOG_STATS("RenderProjects %5.2f\n",tgl.RenderProjectilesCPUTime.ReadMs());
        LOG_STATS("RenderParticle %5.2f\n",tgl.RenderParticlesCPUTime.ReadMs());

        f32 TotalRenderMs = 
            tgl.FogSetupCPUTime.ReadMs() + 
            tgl.RenderSkyCPUTime.ReadMs() + 
            tgl.RenderTerrainCPUTime.ReadMs() + 
            tgl.RenderBuildingsPrepCPUTime.ReadMs() +
            tgl.RenderBuildingsCPUTime.ReadMs() + 
            tgl.RenderShapesCPUTime.ReadMs() + 
            tgl.RenderProjectilesCPUTime.ReadMs() + 
            tgl.RenderParticlesCPUTime.ReadMs();

        LOG_STATS("RenderCPUTime  %5.2f  %5.2f\n",TotalRenderMs, tgl.RenderCPUTime.ReadMs());


        LOG_STATS("Rays %3d %3d %3d\n",COLL_STAT_NRaySetups,COLL_STAT_NRayApplyNGons,COLL_STAT_NRayApplyNGonsSolve);
        LOG_STATS("Exts %3d %3d %3d\n",COLL_STAT_NExtSetups,COLL_STAT_NExtApplyNGons,COLL_STAT_NExtApplyNGonsSolve);

        {
            s32 PS,BS,PR,BR,NAB;
            f32 PerSec = 1000.0f / (tgl.LogicTimeMs - tgl.LastNetStatsResetTime);
            f32 ST,RT;
            net_GetStats(PS,BS,PR,BR,NAB,ST,RT);
            LOG_STATS("PS:%1d BS:%1d PR:%1d BR:%1d\n",PS,BS,PR,BR);
            LOG_STATS("PSS:%1d BSS:%1d PRS:%1d BRS:%1d\n",
                (s32)(PS*PerSec),
                (s32)(BS*PerSec),
                (s32)(PR*PerSec),
                (s32)(BR*PerSec));
            LOG_STATS("ST:%1.1f RT:%1.1f\n", ST*PerSec, RT*PerSec );

        }

        // Count number of players in world
        {
            s32 N=0;
		    player_object *pPlayer ;
            ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
            while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
                N++;
            ObjMgr.EndTypeLoop();
            LOG_STATS("NPlayers  %1d\n",N);
        }

        ObjMgr.PrintStats( pLogFile );

        if( tgl.pServer )
        {
            tgl.pServer->DumpStats( pLogFile );
        }
    }

    {
        COLL_STAT_NRaySetups = 0;
        COLL_STAT_NExtSetups = 0;
        COLL_STAT_NExtApplyNGons = 0;
        COLL_STAT_NExtApplyNGonsSolve = 0;
        COLL_STAT_NRayApplyNGons = 0;
        COLL_STAT_NRayApplyNGonsSolve = 0;
    }

    if( tgl.DisplayStats )
    {
        s32 X=0;
        s32 Y=16;
        if( tgl.pClient )
        {
            x_printfxy(X,Y++,"Client ping to server %1f",tgl.pClient->GetPing());
        }

        if( tgl.pServer )
        {
            tgl.pServer->DisplayPings(Y);
        }
    }

#ifdef TARGET_PC
    if( input_IsPressed( INPUT_KBD_F2 ) )
    {
        vector3 Pos;
        radian Pitch;
        radian Yaw;
        view& View = tgl.GetView(0);

        // Get eye position and orientation
        Pos = View.GetPosition();
        View.GetPitchYaw(Pitch,Yaw);

        // Fire ray straight down
        vector3 Dir(0,-1000,0);
        collider  Ray;
        Ray.RaySetup( NULL, Pos, Pos+Dir );
        ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
        collider::collision Coll;
        vector3 Pos2 = Pos + Ray.GetCollisionT()*Dir;

        x_printfxy(20,0,"POS (%1.2f,%1.2f,%1.2f)",Pos.X,Pos.Y,Pos.Z);
        x_printfxy(20,1,"ROT (%1.4f,%1.4f)",Pitch,Yaw);
        x_printfxy(20,2,"GPS (%1.2f,%1.2f,%1.2f)",Pos2.X,Pos2.Y,Pos2.Z);
        
        // write out a pickup file
        if( input_IsPressed( INPUT_KBD_1 ) )
        {
            X_FILE *fp;

            fp = x_fopen("pickups.txt","at" );
            x_fprintf(fp, "PICKUP HEALTH\t%-3.4f\t%-3.4f\t%-3.4f\t30.0\n", Pos.X, Pos.Y + 0.1f, Pos.Z);
            x_fclose(fp);
            Sleep(1000);
            x_printf("Health Pickup Created.\n" );
        }

        if( input_IsPressed( INPUT_KBD_2 ) )
        {
            X_FILE *fp;

            fp = x_fopen("pickups.txt","at" );
            x_fprintf(fp, "PICKUP AMMO\t%-3.4f\t%-3.4f\t%-3.4f\t15.0\n", Pos.X, Pos.Y + 0.1f, Pos.Z);
            x_fclose(fp);
            Sleep(1000);
            x_printf("Ammo Pickup Created.\n" );
        }
    }        
#endif

#if defined(mtraub) || defined(sbroumley)
    if( 1 )
    {
        eng_Begin( "Fogmap" );
        tgl.Fog.RenderFogMap();
/*
        vector3 P(1457.4893f,169.3864f,1191.2009f);//P(1016.0114f,63.6457f,1119.9697f);
        draw_Marker(P,XCOLOR_RED);

        xcolor FC = tgl.Fog.ComputeFog(P);
        f32 U,V;
        tgl.Fog.ComputeUVs(P,U,V);
        x_printfxy(0,3,"FCOLOR %1d %1d %1d %1d",FC.R,FC.G,FC.B,FC.A);
        x_printfxy(0,4,"FUV %f %f",U,V);
*/
        eng_End();
    }
#endif

/*
    extern xtimer UpdatePlayerTimer;
    if( SHOW_PLAYER_UPDATE_STATS )
    {
        static f32 MS[32]={0};
        static s32 N[32]={0};
        static s32 I=0;

        MS[I] = UpdatePlayerTimer.ReadMs();
        N[I] = UpdatePlayerTimer.GetNSamples();
        I = (I+1)%32;

        for( s32 i=0; i<16; i++ )
        {
            x_printfxy(0,i,"%5.2f %2d",MS[i],N[i]);
        }
        UpdatePlayerTimer.Reset();
    }
*/

/*
    x_printfxy(0,5,"PrepBuilding    %5.2f",tgl.RenderBuildingsPrepCPUTime.ReadMs());
    x_printfxy(0,6,"PrepBuilding1   %5.2f",TIMER_PrepRender.ReadMs());
    x_printfxy(0,7,"PrepBuilding2   %5.2f",TIMER_PrepRender2.ReadMs());
    x_printfxy(0,8,"PrepBuilding2   %5.2f",TIMER_PrepRender3.ReadMs());
    x_printfxy(0,9,"PrepBuilding2   %5.2f",TIMER_PrepRender4.ReadMs());
    TIMER_PrepRender.Reset();
    TIMER_PrepRender2.Reset();
    TIMER_PrepRender3.Reset();
    TIMER_PrepRender4.Reset();
*/
    // Slowly clear printf buffer
//#ifndef mtraub
    {
        static s32 NLoops = 0;
        if( ((NLoops++)%30) == 0 )
        {
            x_printf( "\n" );
        }
    }
//#endif

    // Temp
    //eng_Begin( "test_draw_player" );
    //test_draw_player() ;
    //eng_End();

    // Render SPAWN points
    if( SHOW_SPAWN_POINTS )
    {
        s32 i;
        extern vector3 SpawnSpherePos[100];
        extern f32     SpawnSphereRadius[100];
        extern s32     NSpawnSpheres;

        eng_Begin( "SPAWNPOINTS" );

        // Draw points
        for( i=0; i<tgl.NSpawnPoints; i++ )
        {
            xcolor Color = XCOLOR_BLUE;
            if( tgl.SpawnPoint[i].TeamBits == 0x00000001 )
                Color = XCOLOR_GREEN;
            if( tgl.SpawnPoint[i].TeamBits == 0x00000002 )
                Color = XCOLOR_RED;

            {
                f32 MinD = F32_MAX;
                object* pObj ;

                // Check for ANY solid objects inside the players bounding box
                bbox BBox;
                BBox.Set( tgl.SpawnPoint[i].Pos, 1.0f );
                
                ObjMgr.Select( object::ATTR_SOLID_DYNAMIC | object::ATTR_PERMEABLE, BBox );
                while( (pObj = ObjMgr.GetNextSelected()) )
                {
                    {
                        MinD = 0.0f;
                        break;
                    }
                }
                ObjMgr.ClearSelection();

                /*
                collider  Ray;
                Ray.RaySetup( NULL, tgl.SpawnPoint[i].Pos, tgl.SpawnPoint[i].Pos+vector3(0,-2,0), -1, TRUE );
                ObjMgr.Collide( object::ATTR_SOLID, Ray );
                if( !Ray.HasCollided() )
                {
                    MinD = 0.0f;
                }
                */

                if( MinD <= 10.0f )
                {
                    Color = XCOLOR_BLACK;
                }  
            }

            draw_Point( tgl.SpawnPoint[i].Pos, Color );            
        }

        /*
        // Draw bboxes
        // BBox of biggest player (see Demo1/Game/Data/Characters/DefaultHeavyDefines.txt)
        vector3 BiggestSize(1.63f, 2.6f, 1.63f) ;
        vector3 BBoxMin, BBoxMax ;
        BBoxMin.X = -BiggestSize.X * 0.5f ;
        BBoxMin.Y = 0 ;
        BBoxMin.Z = -BiggestSize.Z * 0.5f ;
        BBoxMax.X = BiggestSize.X * 0.5f ;
        BBoxMax.Y = BiggestSize.Y ;
        BBoxMax.Z = BiggestSize.Z * 0.5f ;
        for( i=0; i<tgl.NSpawnPoints; i++ )
        {
            bbox BBox(BBoxMin, BBoxMax) ;
            BBox.Translate(tgl.SpawnPoint[i].Pos) ;
            draw_BBox(   BBox, XCOLOR_YELLOW );
            draw_Sphere( SpawnSpherePos[i], SpawnSphereRadius[i], XCOLOR_RED );
        }
        */

        // Draw spheres
        for( i=0; i<NSpawnSpheres; i++ )
        {
            draw_Sphere( SpawnSpherePos[i], SpawnSphereRadius[i], XCOLOR_BLUE );
        }

        eng_End();
    }

    if( FALSE )
    {
        //x_printfxy(0,10,"OTim %1.3f",TerrainOccTimer.ReadMs());
        //x_printfxy(0,11,"ONCl %1d",TerrainOccNCalls);
        //x_printfxy(0,12,"ONOc %1d",TerrainOccNOccluded);
        //x_printfxy(0,13,"OcRs %1d",TerrainOccNRays);
        //x_printfxy(0,14,"OcRF %1d",TerrainOccNRaysFound);
    }

    // SCREEN SHOT

    if( DO_SCREEN_SHOTS && input_WasPressed( INPUT_PS2_BTN_SELECT, 0 ) )
    {
        eng_ScreenShot( NULL, 4 );
    }

    /*
    {
        vector3 Pos[] = 
        {
            vector3(922.50391,  96.33669,  1203.54785),
            vector3(874.95770,  77.51126,  1223.16882),
            vector3(886.75165,  77.33988,  1236.04419),
            vector3(874.95770,  77.51126,  1223.16882),
            vector3(918.18951,  94.86086,  1203.02686),
            vector3(922.59747,  94.84761,  1199.32825),
            vector3(917.73132,  94.87281,  1206.78699),
            vector3(959.80261,  77.03635,  1225.86182),
            vector3(874.95770,  77.51126,  1223.16882),
            vector3(960.50043,  76.71902,  1227.69983),
            vector3(927.16528,  94.84622,  1199.43005)
        };
        eng_Begin();
        for( s32 i=0; i<(s32)(sizeof(Pos)/sizeof(vector3)); i++ )
        {
            if( i==0 )
                draw_Marker( Pos[i], XCOLOR_GREEN );
            else
                draw_Marker( Pos[i], XCOLOR_RED );
        }
        eng_End();
    }
    */

    /*
    {
        player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( 0 );
        vehicle*       pVehicle;
        if( pPlayer )
        {
            if( (pVehicle = pPlayer->IsAttachedToVehicle()) )
            {
                x_printfxy( 10, 0, "%10.4f", pVehicle->GetVelocity().Length() );
            }
            else
            {
                x_printfxy( 10, 0, "%10.4f", pPlayer->GetVelocity().Length() );
            }
        }
    }
    */

    // Render latest AttemptDeploy volume (requires g_ShowVolume = TRUE)
    eng_Begin();
    GameMgr.DebugRender();
    eng_End();

    if( SHOW_BLENDING )
    {
        eng_Begin();
        {
            s32 i;
            for( i = 0; i < SetI; i++ )
            {
                draw_Line( Set1[i], Set2[i], XCOLOR_YELLOW );
            }
        }
        eng_End();
    }

/*
    eng_Begin();
    draw_BBox( GameMgr.GetBounds(), XCOLOR_YELLOW );
    draw_Marker( GameMgr.GetBounds().GetCenter(), XCOLOR_YELLOW );
    draw_Line( GameMgr.GetBounds().GetCenter(), GameMgr.GetBounds().GetCenter() + vector3(0,1000,0), XCOLOR_RED );
    eng_End();
*/
}

//==============================================================================
