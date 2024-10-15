//==============================================================================
//
//  MissionIO.cpp
//
//==============================================================================

#include "entropy.hpp"
#include "common/e_cdfs.hpp"
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
#include "Objects\Player\DefaultLoadouts.hpp"
#include "Objects\Projectiles\Plasma.hpp"
#include "Objects\Projectiles\Bullet.hpp"
#include "Objects\Projectiles\Laser.hpp"
#include "Objects\Projectiles\Generator.hpp"
#include "Objects\Projectiles\Sensor.hpp"
#include "Objects\Projectiles\Turret.hpp"
#include "Objects\Projectiles\FlipFlop.hpp"
#include "Objects\Projectiles\WayPoint.hpp"
#include "Objects\Projectiles\Nexus.hpp"
#include "Objects\Projectiles\ForceField.hpp"
#include "Objects\Projectiles\Projector.hpp"
#include "Objects\Bot\BotObject.hpp"
#include "Objects\Pickups\pickups.hpp"
#include "Objects\Projectiles\Flag.hpp"
#include "Objects\Projectiles\Blaster.hpp"
#include "Objects\Projectiles\Station.hpp"
#include "Objects\vehicles\gravcycle.hpp"
#include "Objects\vehicles\shrike.hpp"
#include "Objects\vehicles\transport.hpp"
#include "Objects\vehicles\bomber.hpp"
#include "Objects\scenic\scenic.hpp"
#include "Building\BuildingOBJ.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "audiomgr/audio.hpp"
#include "ui\ui_manager.hpp"
#include "hud\hud_manager.hpp"
#include "hud\hud_Icons.hpp"
#include "StringMgr\StringMgr.hpp"
#include "specialversion.hpp"

#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "pointlight\pointlight.hpp"

#include "objects\bot\pathgenerator.hpp"

#include "Objects\Bot\MAI_Manager.hpp"

#include "Damage\Damage.hpp"

extern xstring* x_fopen_log;
extern xbool    x_logging_enabled;
extern xbool    MeteorShower;

static s32 LoadMissionStage = 0;

extern graph g_Graph;

vector3 SpawnSpherePos[16];
f32     SpawnSphereRadius[16];
s32     SpawnSphereTeam[16];
s32     NSpawnSpheres=0;

//==============================================================================

void LoadPermanentData( void )
{
    x_MemAddMark( "Begin - LoadPermanentData" );

    // Particles

    x_MemAddMark( "Effects&Music" );
    tgl.VoicePackageId   = audio_LoadContainer( "data\\audio\\voice.pkg" );
	tgl.TrainingPackageId= audio_LoadContainer( "data\\audio\\training.pkg");

    //title_SierraLogo(TRUE);
    //title_InevitableLogo(FALSE);

    tgl.EffectsPackageId = audio_LoadContainer( "data\\audio\\effects.pkg" );
    tgl.MusicPackageId   = audio_LoadContainer( "data\\audio\\music.pkg" );

    audio_SetMasterVolume(tgl.MasterVolume,tgl.MasterVolume);
    audio_SetContainerVolume(tgl.EffectsPackageId,tgl.EffectsVolume);
    audio_SetContainerVolume(tgl.MusicPackageId,tgl.MusicVolume);
    audio_SetContainerVolume(tgl.VoicePackageId,tgl.VoiceVolume);
    audio_SetContainerVolume(tgl.TrainingPackageId,tgl.VoiceVolume);


    audio_SetEarView( &tgl.PC[0].View, 6.0f, 10.0f, 600.0f, 0.2f );

    x_MemAddMark( "Particles" );
    tgl.ParticleLibrary.Create( "data/particles/particles" );
    // load vehicle special effects

    Bomber_InitFX();
    GravCycle_InitFX();
    Shrike_InitFX();
    Transport_InitFX();

    // Shapes.
    {
        x_MemAddMark( "GameShapes" );
        // Setup global shape draw features that shape instances will be initialized with
        shape::SetFogColor          ( xcolor(0,0,0,0) );
        shape::SetLightAmbientColor ( xcolor(200,200,200,255) );                         
        shape::SetLightColor        ( xcolor(180,180,180,255) );                         


        // Load binary shape files
        tgl.GameShapeManager.AddAndLoadShapes( "Data/Shapes/GameShapes.txt", 
                                               shape_manager::MODE_LOAD_BINARY );

        // Setup the damage system if damage texture is present
        texture* DamageTexture = tgl.GameShapeManager.GetTextureManager().GetTextureByName("damage.xbmp") ;
        if (DamageTexture)
            tgl.Damage.Init(DamageTexture) ;
    }

    // Bitmaps for the various projectiles.
    {
        x_MemAddMark( "Projectiles" );
        laser::Init();
        plasma::Init();
        bullet::Init();
        blaster::Init();
        force_field::Init();
    }

    // UI
    {
        x_MemAddMark( "UI" );
        ASSERT( tgl.pUIManager == NULL );
        tgl.pUIManager = new ui_manager;
        ASSERT( tgl.pUIManager );
        tgl.pUIManager->Init();
    }

    // HUD
    {
        x_MemAddMark( "HUD" );
        ASSERT( tgl.pHUDManager == NULL );
        tgl.pHUDManager = new hud_manager;
        ASSERT( tgl.pHUDManager );
        tgl.pHUDManager->Init( tgl.pUIManager );

        HudIcons.Init();
    }

    // Load String Table for mission names and objectives/notes.
    x_MemAddMark( "StringTable" );
    StringMgr.LoadTable( "MissionName", "Data\\Missions\\MissionNames.bin" );
    StringMgr.LoadTable( "Objective",   "Data\\Missions\\Objectives.bin" );

    // Wait until inevitable logo movie has completed running
    //title_InevitableLogo(TRUE);

    // Player defines
    x_MemAddMark( "Player::SetupViewAndWeaponLookupTable" );
    // BW 1/23/02 - Had to move this to after the movie play was complete
    // since there was a clash on attempting to access scratchmem from two
    // threads.
    player_object::SetupViewAndWeaponLookupTable() ;

    x_MemAddMark( "End   - LoadPermanentData" );
}

//==============================================================================

void UnloadPermanentData( void )
{   
    laser::Kill();
    plasma::Kill();
    bullet::Kill();
    blaster::Kill();
    force_field::Kill();

    audio_StopAll();
    audio_Update();
    audio_UnloadContainer(tgl.MusicPackageId);
    audio_UnloadContainer(tgl.EffectsPackageId);
	audio_UnloadContainer(tgl.TrainingPackageId);
    audio_UnloadContainer(tgl.VoicePackageId);

    tgl.Damage.Kill() ;
    tgl.GameShapeManager.DeleteAllShapes();
    tgl.ParticleLibrary.Kill();

    HudIcons.Kill();

    StringMgr.UnloadTable( "Objective" );
    StringMgr.UnloadTable( "MissionName" );
}

//==============================================================================

void KillServerPlayers( void )
{
}

//==============================================================================

void AddServerPlayers( void )
{
}

//==============================================================================

void AttemptAddSpawnPoint( const vector3& Point, u32 TeamBits, xbool CheckBoundsAndDrop )
{
    if( tgl.NSpawnPoints >= MAX_SPAWN_POINTS )
        return;

    bbox Bounds = GameMgr.GetBounds();

    if( CheckBoundsAndDrop )
    {
        //
        // Spawn points must be in bounds.
        //

        if( !Bounds.Intersect( Point ) )
        {
            // This spawn point is out of bounds.  Sorry.  Thanks for playing.
            return;
        }
    }

    //
    // Check for any solid object within the spawn bounding box.
    //

    object* pObj;
    bbox    BBox;

    BBox.Set( Point, 1.0f );

    ObjMgr.Select( object::ATTR_SOLID_DYNAMIC | object::ATTR_PERMEABLE, BBox );
    pObj = ObjMgr.GetNextSelected();
    ObjMgr.ClearSelection();

    // If we have a non-NULL pointer, then there is an object within the spawn 
    // bounding box.  Sorry.  Thanks for playing.
    if( pObj )
        return;

    //
    // See how far the spawn point is up in the air.  Don't wanna spawn and 
    // then fall.
    //

    if( CheckBoundsAndDrop )
    {
        collider Ray;
        vector3  Drop = Point;

        Drop.Y -= 2.0f;

        Ray.RaySetup( NULL, Point, Drop, -1, TRUE );
        ObjMgr.Collide( object::ATTR_SOLID, Ray );

        if( !Ray.HasCollided() )
        {
            // Nothing to stand on within a reasonable distance below the spawn
            // point.  Sorry.  Thank for playing.
            return;
        }
    }

    //
    // This spawn point is satisifactory to Spawn Point Committee.
    //

    vector3 Center = Bounds.GetCenter();
    radian  Yaw    = x_atan2( Center.X - Point.X, Center.Z - Point.Z );

    tgl.SpawnPoint[ tgl.NSpawnPoints++ ].Setup( TeamBits, 
                                                Point.X, 
                                                Point.Y + 0.1f, 
                                                Point.Z, 
                                                R_0, Yaw );
}

//==============================================================================

void EndOfMissionLoad( void )
{
    // Fill out spawn points
    if( tgl.ServerPresent )
    {
        bbox    Bounds = GameMgr.GetBounds();
        vector3 SP[512];
        s32     MaxPtsPerSphere = MAX_SPAWN_POINTS;

        if( NSpawnSpheres != 0 )
        {
            MaxPtsPerSphere /= NSpawnSpheres;
        }

        MaxPtsPerSphere = MIN( MaxPtsPerSphere, 512 );

        // Loop through spawn spheres
        for( s32 s=0; s<NSpawnSpheres; s++ )
        {
            s32 i;

            // Determine team bits
            s32 TeamBits;
            if( SpawnSphereTeam[s] == -1 )
                TeamBits = 0xFFFFFFFF;
            else
                TeamBits = 1 << SpawnSphereTeam[s];

            // Request navgraph nodes.
            s32 NSP = g_Graph.GetNGroundPointsInRadius( MaxPtsPerSphere, 
                                                        SpawnSpherePos[s], 
                                                        SpawnSphereRadius[s], 
                                                        SP );

            // Try to make a spawn point from each nav node point.
            for( i=0; i<NSP; i++ )
            {
                AttemptAddSpawnPoint( SP[i], TeamBits, TRUE );
            }

            // If we didn't get enough, then add some at random if it is not a campain game.
            if( (NSP == 0) || ((NSP < 64) && (GameMgr.GetGameType() != GAME_CAMPAIGN)) )
            {
                for( i=0; i<64; i++ )
                {
                    vector3 Point = SpawnSpherePos[s];
                    Point.Y  = Bounds.Max.Y - 1.0f;
                    Point.X += x_frand(-SpawnSphereRadius[s],SpawnSphereRadius[s]);
                    Point.Z += x_frand(-SpawnSphereRadius[s],SpawnSphereRadius[s]);

                    if( Bounds.Intersect( Point ) )
                    {
                        bbox    BBox;
                        vector3 Move( 0.0f, -4000.0f, 0.0f );

                        BBox.Min( Point.X - 1.75f, Point.Y       , Point.Z - 1.75f );
                        BBox.Max( Point.X + 1.75f, Point.Y + 3.0f, Point.Z + 1.75f );

                        collider Collider;
                        Collider.ExtSetup( NULL, BBox, Move );
                        ObjMgr.Collide( object::ATTR_SOLID_STATIC, Collider );

                        if( Collider.HasCollided() )
                        {
                            Point += Collider.GetCollisionT() * Move;
                            AttemptAddSpawnPoint( Point, TeamBits, FALSE );
                        }                    
                    }
                }
            }
        }
        MAI_Mgr.EndOfMissionLoadInit();
    }

    // Setup terrain lighting
    {
        // Get ptr to terrain object
        terrain* pTerrain;
        ObjMgr.StartTypeLoop( object::TYPE_TERRAIN );
        pTerrain = (terrain*)ObjMgr.GetNextInType();
        ObjMgr.EndTypeLoop();
        ASSERT( pTerrain );

        pTerrain->m_Terrain.BuildLightingData( tgl.Sun,
                                               128.0f,
                                               0.45f, 0.70f,
                                               0.80f, 1.00f,
                                               0.45f, 0.90f,
                                               xfs("%s/lighting.lit",tgl.MissionDir));

        // Tell fog terrain's bounds
        f32 MinY,MaxY;
        pTerrain->m_Terrain.GetMinMaxHeight( MinY, MaxY );
        tgl.Fog.SetMinMaxY( MinY, MaxY );

        // Tell tgl where terrain is
        tgl.pTerr = &pTerrain->m_Terrain;
    }

    // Test Ray collision
    if( 0 )
    {
        vector3 Start( 1268.74f, 266.523f, 1538.92f );
        vector3 End  ( 1103.6f, 204.789f, 1214.67f );
        random  Rand;
        f32     R = 0.2f;
        collider Coll;
        s32 i;

        x_DebugMsg("TESTING RAY COLLISION\n");
        
        for( i=0; i<60750; i++ )
        {
            Rand.v3(-R,R,-R,R,-R,R);
            Rand.v3(-R,R,-R,R,-R,R);
        }

        for( i=0; i<100000; i++ )
        {
            vector3 P0 = Start + Rand.v3(-R,R,-R,R,-R,R);
            vector3 P1 = End   + Rand.v3(-R,R,-R,R,-R,R);

            Coll.RaySetup( NULL, P0, P1 );
            ObjMgr.Collide( object::ATTR_SOLID_STATIC, Coll );
        }
        x_DebugMsg("FINISHED TESTING RAY COLLISION\n");
    }

    // Dump memory usage after mission is loaded

#if defined(TARGET_PS2_DEV) && defined(X_DEBUG) && defined(bwatson)
    //x_MemDump( "MissionLoaded.txt" );
#endif


    /*
    // CJG: Dump log of files loaded during initialization so I can build CD Filesystem FILES.DAT
    #if (defined(TARGET_PS2_DEV)) //&& defined(cgalley))
    x_fopen_log->SaveFile( "cdfs_script.txt" );
    delete x_fopen_log;
    x_fopen_log = NULL;
    #endif
    */

//  printf("Begin free mem size.\n");
#if defined(TARGET_PS2) && defined(X_ASSERT) && !defined(X_DEBUG)
    {
        s32 Free,Largest,Fragments;

        extern void ps2_GetFreeMem(s32 *pFree,s32 *pLargest,s32 *pFragments);

        ps2_GetFreeMem(&Free,&Largest,&Fragments);
        scePrintf("Mission %s, free=%d, largest=%d,fragments=%d\n",tgl.MissionName,Free,Largest,Fragments);
    }
#endif

    audio_SetEarView(&tgl.PC[0].View);

    g_BldManager.SetFog( &tgl.Fog );

    ASSERT( tgl.MusicId == 0 );
    ASSERT( tgl.MusicSampleId != -1 );
    
    tgl.MusicId = audio_Play( tgl.MusicSampleId );
}

//==============================================================================

static token_stream Mission;
static xbool        MissionFileOpen = FALSE;

//==============================================================================
xbool ProcessEntry( token_stream& Mission )
{
    char  Tag[256] = "";
    char* pToken   = Mission.String();
    s32   i ;

    //
    // Look for a tag.
    //
    if( x_stricmp( pToken, "#TAG" ) == 0 )
    {
        x_strcpy( Tag, Mission.ReadString() );
        Mission.Read();
        pToken = Mission.String();
    }

    //--------------------------------------------------------------------------
    //  Mission Name
    //--------------------------------------------------------------------------
    if( x_stricmp( pToken, "MISSION" ) == 0 )
    {
        Mission.ReadString();
    }

    //--------------------------------------------------------------------------
    //  Music
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "MUSIC" ) == 0 )
    {
        Mission.ReadSymbol();

        if (x_stricmp(Mission.String(),"DESERT")==0)
        {
            tgl.MusicSampleId = MUSIC_DESERT;
        }
        else if (x_stricmp(Mission.String(),"BADLANDS")==0)
        {
            tgl.MusicSampleId = MUSIC_BADLANDS;
        }
        else if (x_stricmp(Mission.String(),"ICE")==0)
        {
            tgl.MusicSampleId = MUSIC_ICE;
        }
        else if (x_stricmp(Mission.String(),"LUSH")==0)
        {
            tgl.MusicSampleId = MUSIC_LUSH;
        }
        else if (x_stricmp(Mission.String(),"VOLCANIC")==0)
        {
            tgl.MusicSampleId = MUSIC_VOLCANIC;
        }
        else
        {
            tgl.MusicSampleId = -1;
        }

    }

    //--------------------------------------------------------------------------
    //  Environment
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "ENVIRONMENT" ) == 0 )
    {

        Mission.ReadSymbol();


#if defined(TARGET_PS2_DEV) && defined(X_DEBUG) && defined(bwatson)
        // When we're doing a mission export, let's try not to duplicate
        // actual environment data. We will only have one instance on
        // the disk.
        static xbool EnvironmentExported[10]={FALSE,FALSE,FALSE,FALSE};
        s32     EnvIndex;

        if(x_stricmp(Mission.String(),"DESERT")==0)
        {
            EnvIndex = 0;
        }
        else
        if(x_stricmp(Mission.String(),"LUSH")==0)
        {
            EnvIndex = 1;
        }
        else
        if(x_stricmp(Mission.String(),"ICE")==0)
        {
            EnvIndex = 2;
        }
        else
        if(x_stricmp(Mission.String(),"LAVA")==0)
        {
            EnvIndex = 3;
        }
        else
        if(x_stricmp(Mission.String(),"BADLAND")==0)
        {
            EnvIndex = 4;
            // No shapes for badlands
        }
        else
        {
            ASSERT( FALSE );
        }

        if (EnvironmentExported[EnvIndex])
        {
            x_logging_enabled = FALSE;
        }

        EnvironmentExported[EnvIndex] = TRUE;
#endif

        MeteorShower = FALSE;

        if(x_stricmp(Mission.String(),"DESERT")==0)
        {
            tgl.Environment = t2_globals::ENVIRONMENT_DESERT ;
            tgl.EnvShapeManager.AddAndLoadShapes( "Data/Shapes/DesertShapes.txt", shape_manager::MODE_LOAD_BINARY );
        }
        else
        if(x_stricmp(Mission.String(),"LUSH")==0)
        {
            tgl.Environment = t2_globals::ENVIRONMENT_LUSH ;
            tgl.EnvShapeManager.AddAndLoadShapes( "Data/Shapes/LushShapes.txt", shape_manager::MODE_LOAD_BINARY );
        }
        else
        if(x_stricmp(Mission.String(),"ICE")==0)
        {
            tgl.Environment = t2_globals::ENVIRONMENT_ICE ;
            tgl.EnvShapeManager.AddAndLoadShapes( "Data/Shapes/IceShapes.txt", shape_manager::MODE_LOAD_BINARY );
        }
        else
        if(x_stricmp(Mission.String(),"LAVA")==0)
        {
            MeteorShower = TRUE;
            tgl.Environment = t2_globals::ENVIRONMENT_LAVA ;
            tgl.EnvShapeManager.AddAndLoadShapes( "Data/Shapes/LavaShapes.txt", shape_manager::MODE_LOAD_BINARY );
        }
        else
        if(x_stricmp(Mission.String(),"BADLAND")==0)
        {
            tgl.Environment = t2_globals::ENVIRONMENT_BADLAND ;

            // No shapes for badlands
        }
        else
        {
            ASSERT( FALSE );
        }
        // Fast forward material IDs
        s32 MaterialID         = tgl.GameShapeManager.GetNextMaterialID() ; 
        s32 MaterialDrawListID = tgl.GameShapeManager.GetNextMaterialDrawListID() ;
        tgl.EnvShapeManager.AssignMaterialIDs( MaterialID, MaterialDrawListID ) ;
#if defined(TARGET_PS2_DEV) && defined(X_DEBUG) && defined(bwatson)
        x_logging_enabled = TRUE;
#endif
    }

    //--------------------------------------------------------------------------
    //  Sun
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "SUN" ) == 0 )
    {
        tgl.Sun.X     = Mission.ReadFloat();
        tgl.Sun.Y     = Mission.ReadFloat();
        tgl.Sun.Z     = Mission.ReadFloat();

        tgl.Direct.R  = Mission.ReadInt();
        tgl.Direct.G  = Mission.ReadInt();
        tgl.Direct.B  = Mission.ReadInt();

        tgl.Ambient.R = Mission.ReadInt();
        tgl.Ambient.G = Mission.ReadInt();
        tgl.Ambient.B = Mission.ReadInt();

        shape::SetLightDirection   ( -tgl.Sun, TRUE );
        shape::SetLightColor       (  tgl.Direct    );
        shape::SetLightAmbientColor(  tgl.Ambient   );
    }

    //--------------------------------------------------------------------------
    //  Fog
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "FOG" ) == 0 )
    {
        // Check if there is a fog settings file
        X_FILE* fp = x_fopen(xfs("%s/FogSettings.txt",tgl.MissionDir),"rt");
        if( fp )
        {
            x_fclose(fp);
            tgl.Fog.LoadSettings(xfs("%s/FogSettings.txt",tgl.MissionDir));
            for( i=0; i<14; i++ )
                Mission.Read();
            s32 N = Mission.ReadInt();
            for( i=0; i<N*3; i++ )
                Mission.Read();
        }
        else
        {
            tgl.Fog.Load( Mission );
        }
    }

    //--------------------------------------------------------------------------
    //  Sky
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "SKY" ) == 0 )
    {
        tgl.Sky.Load( Mission );
        tgl.Sky.SetFog( &tgl.Fog );
        tgl.Sky.SetYaw( tgl.Sun.GetYaw() - R_65 );
    }

    //--------------------------------------------------------------------------
    //  Camera
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "CAMERA" ) == 0 )
    {
        vector3 Position;
        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        tgl.FreeCam[0].SetPosition( Position );
    }

    //--------------------------------------------------------------------------
    //  Terrain
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "TERRAIN" ) == 0 )
    {
        terrain* pTerrain = (terrain*)ObjMgr.CreateObject( object::TYPE_TERRAIN );
        pTerrain->Initialize( xfs("%s/%s",tgl.MissionDir,Mission.ReadString()), &tgl.Fog );
        ObjMgr.AddObject( pTerrain, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  Empty Squares in the Terrain
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "EMPTYSQUARES" ) == 0 )
    {
        ObjMgr.StartTypeLoop( object::TYPE_TERRAIN );
        terrain* pTerrain = (terrain*)ObjMgr.GetNextInType();
        ObjMgr.EndTypeLoop();
        if( pTerrain )
        {
            s32 N;
            while( 1 )
            {
                N = Mission.ReadInt();
                if( N == -1 ) 
                    break;
                pTerrain->m_Terrain.SetEmptySquare( N );
            }
        }
    }

    //--------------------------------------------------------------------------
    //  Building
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "BUILDING" ) == 0 )
    {

        matrix4 L2W;
        vector3 Position;
        radian3 Rotation;
        vector3 Scale;
        s32     Power;
        char    FileName[ X_MAX_PATH ];

        Power = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        Scale.X = Mission.ReadFloat();
        Scale.Y = Mission.ReadFloat();
        Scale.Z = Mission.ReadFloat();

        L2W.Identity();
        L2W.SetScale ( Scale    );
        L2W.Rotate   ( Rotation );
        L2W.Translate( Position );

        char* pFileName = Mission.ReadString();

        // Check to see if file is available
        x_sprintf( FileName, "Data/Interiors/%s.bin", pFileName );
        X_FILE* fp = x_fopen( FileName, "rb" );
        if( fp )
        {
            x_fclose(fp);
            building_obj* pBuilding = (building_obj*)ObjMgr.CreateObject( object::TYPE_BUILDING );
            pBuilding->Initialize( FileName, L2W, Power );
            ObjMgr.AddObject( pBuilding, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
        }
        else
        {
            x_DebugMsg( "******************************\n" );
            x_DebugMsg( "Could not load building %s\n", FileName );
        }   

    }

    //--------------------------------------------------------------------------
    //  Scenery
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "SCENERY" ) == 0 )
    {
        vector3 Position;
        radian3 Rotation;
        vector3 Scale;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        Scale.X = Mission.ReadFloat();
        Scale.Y = Mission.ReadFloat();
        Scale.Z = Mission.ReadFloat();

        Mission.ReadString();

        scenic* pScenic = (scenic*)ObjMgr.CreateObject( object::TYPE_SCENIC );
        pScenic->Initialize( Position, Rotation, Scale, Mission.String() );
        ObjMgr.AddObject( pScenic, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  GENERATOR
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "GENERATOR" ) == 0 )
    {
        s32     Switch;
        s32     Power;
        s32     Index;
        vector3 Position;
        radian3 Rotation;
        const xwchar* pLabel;

        Switch = Mission.ReadInt();
        Power  = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Index  = Mission.ReadInt();       
        pLabel = StringMgr( "Mission", Index );

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        generator* pGenerator = (generator*)ObjMgr.CreateObject( object::TYPE_GENERATOR );
        pGenerator->Initialize( Position, Rotation, Switch, Power, pLabel );
        ObjMgr.AddObject( pGenerator, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  Inventory Stations
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "INVEN" ) == 0 )
    {
        vector3 Position;
        radian3 Rotation;
        s32     Switch;
        s32     Power;
        s32     Index;
        const xwchar* pLabel;

        Switch = Mission.ReadInt();
        Power  = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Index  = Mission.ReadInt();       
        pLabel = StringMgr( "Mission", Index );

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        station* pStation = (station*)ObjMgr.CreateObject( object::TYPE_STATION_FIXED );
        pStation->Initialize( Position, Rotation, Power, Switch, pLabel );
        ObjMgr.AddObject( pStation, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  Vehicle Pads
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "VEHICLEPAD" ) == 0 )
    {
        vector3 Position;
        radian3 Rotation;
        s32     Switch;
        s32     Power;
        s32     Index;
        const xwchar* pLabel;

        Switch = Mission.ReadInt();
        Power  = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Index  = Mission.ReadInt();       
        pLabel = StringMgr( "Mission", Index );

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        station* pStation = (station*)ObjMgr.CreateObject( object::TYPE_STATION_VEHICLE );
        pStation->Initialize( Position, Rotation, Power, Switch, pLabel );
        ObjMgr.AddObject( pStation, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  TURRET
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "TURRET" ) == 0 )
    {
        object::type    Type = object::TYPE_NULL;
        s32             Switch;
        s32             Power;
        s32             Index;
        vector3         Position;
        radian3         Rotation;
        char*           pBarrel;
        turret::kind    Kind = turret::DEPLOYED;
        const xwchar*   pLabel;

        Switch = Mission.ReadInt();
        Power  = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Index  = Mission.ReadInt();       
        pLabel = StringMgr( "Mission", Index );

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        pBarrel = Mission.ReadSymbol();
        if( x_strcmp( pBarrel, "AA"      ) == 0 )   { Kind = turret::AA;        Type = object::TYPE_TURRET_FIXED;  }
        if( x_strcmp( pBarrel, "ELF"     ) == 0 )   { Kind = turret::ELF;       Type = object::TYPE_TURRET_FIXED;  }
        if( x_strcmp( pBarrel, "Missile" ) == 0 )   { Kind = turret::MISSILE;   Type = object::TYPE_TURRET_FIXED;  }
        if( x_strcmp( pBarrel, "Mortar"  ) == 0 )   { Kind = turret::MORTAR;    Type = object::TYPE_TURRET_FIXED;  }
        if( x_strcmp( pBarrel, "Plasma"  ) == 0 )   { Kind = turret::PLASMA;    Type = object::TYPE_TURRET_FIXED;  }
        if( x_strcmp( pBarrel, "Sentry"  ) == 0 )   { Kind = turret::SENTRY;    Type = object::TYPE_TURRET_SENTRY; }
        if( x_strcmp( pBarrel, "Spider"  ) == 0 )   { Kind = turret::DEPLOYED;  Type = object::TYPE_TURRET_CLAMP;  }

        turret* pTurret = (turret*)ObjMgr.CreateObject( Type );
        pTurret->Initialize( Position, Rotation, Switch, Power, Kind, pLabel );
        ObjMgr.AddObject( pTurret, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  SENSOR
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "SENSOR" ) == 0 )
    {
        object::type  Type = object::TYPE_NULL;
        s32           Switch;
        s32           Power;
        s32           Index;
        vector3       Position;
        radian3       Rotation;
        char*         pSize;
        const xwchar* pLabel;

        pSize = Mission.ReadSymbol();
        switch( pSize[0] )
        {
            case 'L':   Type = object::TYPE_SENSOR_LARGE;   break;
            case 'M':   Type = object::TYPE_SENSOR_MEDIUM;  break;
            default:    ASSERT( FALSE );                    break;
        }

        Switch = Mission.ReadInt();
        Power  = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Index  = Mission.ReadInt();       
        pLabel = StringMgr( "Mission", Index );

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        sensor* pSensor = (sensor*)ObjMgr.CreateObject( Type );
        pSensor->Initialize( Position, Rotation, Switch, Power, pLabel );
        ObjMgr.AddObject( pSensor, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  FORCE FIELD
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "FORCEFIELD" ) == 0 )
    {
        s32     Switch;
        s32     Power;
        vector3 Position;
        radian3 Rotation;
        vector3 Scale;

        Switch = Mission.ReadInt();
        Power  = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        Scale.X = Mission.ReadFloat();
        Scale.Y = Mission.ReadFloat();
        Scale.Z = Mission.ReadFloat();

        force_field* pForceField = (force_field*)ObjMgr.CreateObject( object::TYPE_FORCE_FIELD );
        pForceField->Initialize( Position, Rotation, Scale, Switch, Power );
        ObjMgr.AddObject( pForceField, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  SWITCHES
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "FLIPFLOP" ) == 0 )
    {
        s32     Switch;
        vector3 Position;
        radian3 Rotation;

        Switch = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        flipflop* pFlipFlop = (flipflop*)ObjMgr.CreateObject( object::TYPE_FLIPFLOP );
        pFlipFlop->Initialize( Position, Rotation, Switch );
        ObjMgr.AddObject( pFlipFlop, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  PROJECTORS
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "PROJECTOR" ) == 0 )
    {
        s32     Switch;
        vector3 Position;
        radian3 Rotation;

        Switch = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        projector* pProjector = (projector*)ObjMgr.CreateObject( object::TYPE_PROJECTOR );
        pProjector->Initialize( Position, Rotation, Switch );
        ObjMgr.AddObject( pProjector, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  NEXUS
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "NEXUS" ) == 0 )
    {
        vector3 Position;
        f32     ScaleY;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();
        ScaleY     = Mission.ReadFloat();

        nexus* pNexus = (nexus*)ObjMgr.CreateObject( object::TYPE_NEXUS );
        pNexus->Initialize( Position, ScaleY );
        ObjMgr.AddObject( pNexus, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  Waypoints
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "WAYPOINT" ) == 0 )
    {
        s32     Switch;
        s32     Index;    
        vector3 Position;
        const xwchar* pLabel;

        Switch = Mission.ReadInt();

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Index  = Mission.ReadInt();       
        pLabel = StringMgr( "Mission", Index );

        waypoint* pWaypoint = (waypoint*)ObjMgr.CreateObject( object::TYPE_WAYPOINT );
        pWaypoint->Initialize( Position, Switch, pLabel );
        ObjMgr.AddObject( pWaypoint, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  WaterMark
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "WATERMARK" ) == 0 )
    {
        if( tgl.ServerPresent )
        {
            tgl.pServer->SyncMissionLoad();
        }
    }

    //--------------------------------------------------------------------------
    //  POWER GRID
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "POWER" ) == 0 )
    {
        s32 PowerCount[16];
        for( s32 i = 0; i < 16; i++ )
            PowerCount[i] = Mission.ReadInt();
        GameMgr.InitPower( PowerCount );
    }

    //--------------------------------------------------------------------------
    //  SWITCH GRID
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "SWITCH" ) == 0 )
    {
        u32 SwitchBits[16];
        for( s32 i = 0; i < 16; i++ )
        {
            SwitchBits[i] = (u32)Mission.ReadInt();
        }
        GameMgr.InitSwitch( SwitchBits );
    }

    //--------------------------------------------------------------------------
    //  Bounds
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "BOUNDS" ) == 0 )
    {
        bbox Bounds;
        Bounds.Min.X = Mission.ReadFloat();
        Bounds.Min.Y = Mission.ReadFloat();
        Bounds.Min.Z = Mission.ReadFloat();
        Bounds.Max.X = Mission.ReadFloat();
        Bounds.Max.Y = Mission.ReadFloat();
        Bounds.Max.Z = Mission.ReadFloat();
        GameMgr.SetBounds( Bounds );
    }

    //--------------------------------------------------------------------------
    //  Spawn point
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "SPAWN" ) == 0 )
    {
        s32     Team;
        vector3 P;
        radian  Pitch = R_0;
        radian  Yaw   = R_0;
        f32     Radius;

        Team     = Mission.ReadInt();
        P.X      = Mission.ReadFloat();
        P.Y      = Mission.ReadFloat();
        P.Z      = Mission.ReadFloat();
        Radius   = Mission.ReadFloat();
        Pitch    = Mission.ReadFloat();
        Yaw      = Mission.ReadFloat();

        SpawnSpherePos   [NSpawnSpheres] = P;
        SpawnSphereRadius[NSpawnSpheres] = Radius;
        SpawnSphereTeam  [NSpawnSpheres] = Team;
        NSpawnSpheres++;
    }

    //--------------------------------------------------------------------------
    //  Bot
    //--------------------------------------------------------------------------
    else if( (x_stricmp( pToken, "BOT" ) == 0) )// && (tgl.GameStage != GAME_STAGE_INIT) )
    {
    #define GENERATE_PATH_INFO 0
    #if GENERATE_PATH_INFO
        static HaveGeneratedPathInfo = false;
        if ( !HaveGeneratedPathInfo )
        {
            path_generator::GenerateTerrainVertices( ObjMgr );
            HaveGeneratedPathInfo = true;
        }
    #endif

        s32 N = Mission.ReadInt();
        if( tgl.ServerPresent )
            tgl.NBots = N;
    }

    //--------------------------------------------------------------------------
    //  Flag
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "FLAG_HOME" ) == 0 )
    {
        vector3 Position;
        radian  Yaw;
        s32     FlagTeam;

        FlagTeam   = Mission.ReadInt();
        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();
        Yaw        = Mission.ReadFloat();

        if( pGameLogic )
            pGameLogic->RegisterFlag( Position, Yaw, FlagTeam );
    }

    //--------------------------------------------------------------------------
    //  Pickups
    //--------------------------------------------------------------------------

    else if( x_stricmp( pToken, "PICKUP" ) == 0 )
    {
        char*           pKind;
        vector3         PickupPos;
        s32             i;

        char* KindStrList[] = 
        {
            "???",                              // KIND_NONE,

            "DISC",                             // KIND_WEAPON_DISK_LAUNCHER,  
            "PLASMA",                           // KIND_WEAPON_PLASMA_GUN,  
            "SNIPERRIFLE",                      // KIND_WEAPON_SNIPER_RIFLE,  
            "???",                              // KIND_WEAPON_MORTAR_GUN,  
            "GRENADELAUNCHER",                  // KIND_WEAPON_GRENADE_LAUNCHER,  
            "CHAINGUN",                         // KIND_WEAPON_CHAIN_GUN,  
            "BLASTER",                          // KIND_WEAPON_BLASTER,  
            "ELFGUN",                           // KIND_WEAPON_ELF_GUN,  
            "???",                              // KIND_WEAPON_MISSILE_LAUNCHER,  

            "DISCAMMO",                         // KIND_AMMO_DISC                 
            "CHAINGUNAMMO",                     // KIND_AMMO_CHAIN_GUN            
            "PLASMAAMMO",                       // KIND_AMMO_PLASMA               
            "GRENADELAUNCHERAMMO",              // KIND_AMMO_GRENADE_LAUNCHER     
            "GRENADE",                          // KIND_AMMO_GRENADE_BASIC
            "MINE",                             // KIND_AMMO_MINE
            "???",                              // KIND_AMMO_BOX 

            "REPAIRPATCH",                      // KIND_HEALTH_PATCH
            "REPAIRKIT",                        // KIND_HEALTH_KIT

            "AMMOPACK",                         // KIND_ARMOR_PACK_AMMO           
            "ENERGYPACK",                       // KIND_ARMOR_PACK_ENERGY         
            "REPAIRPACK",                       // KIND_ARMOR_PACK_REPAIR         
            "???",                              // KIND_ARMOR_PACK_SENSOR_JAMMER  
            "SHIELDPACK",                       // KIND_ARMOR_PACK_SHIELD         

            "???",                              // KIND_DEPLOY_PACK_BARREL_AA     
            "???",                              // KIND_DEPLOY_PACK_BARREL_ELF    
            "???",                              // KIND_DEPLOY_PACK_BARREL_MORTAR 
            "???",                              // KIND_DEPLOY_PACK_BARREL_MISSILE
            "???",                              // KIND_DEPLOY_PACK_BARREL_PLASMA 
            "???",                              // KIND_DEPLOY_PACK_INVENT_STATION
            "???",                              // KIND_DEPLOY_PACK_PULSE_SENSOR  
            "???",                              // KIND_DEPLOY_PACK_TURRET_INDOOR 
            "???",                              // KIND_DEPLOY_PACK_TURRET_OUTDOOR
            "SATCHELCHARGE",                    // KIND_DEPLOY_PACK_SATCHEL_CHARGE
        };
                                                                       
        // read the coordinates                                        
        PickupPos.X = Mission.ReadFloat();                             
        PickupPos.Y = Mission.ReadFloat();                             
        PickupPos.Z = Mission.ReadFloat();                             
                                                                       
        // read the kind                                               
        pKind = Mission.ReadSymbol();                                  
                                                                       
        // Search for the kind string in the list.                     
        for( i = 0; i < (s32)(sizeof(KindStrList) / 4); i++ )          
        {
            if( x_strcmp( pKind, KindStrList[i] ) == 0 )
                break;
        }

        if( i != ((s32)(sizeof(KindStrList) / 4)) )
        {
            // create the pickup
            pickup* pPickup = (pickup*)ObjMgr.CreateObject( object::TYPE_PICKUP );

            pPickup->Initialize( PickupPos, vector3(0,0,0), (pickup::kind)i, 
                pickup::flags(pickup::FLAG_IMMORTAL | pickup::FLAG_RESPAWNS), 10.0f );

            // Add the object to the world
            ObjMgr.AddObject( pPickup, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
        }
    }

    //--------------------------------------------------------------------------
    //  VARIOUS VEHICLES
    //--------------------------------------------------------------------------
    /*
    else if( x_stricmp( pToken, "GRAVCYCLE" ) == 0 )
    {
        vector3 Position;
        radian  Yaw;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Yaw = Mission.ReadFloat();

        gravcycle* pGrav = (gravcycle*)ObjMgr.CreateObject( object::TYPE_VEHICLE_WILDCAT );
        pGrav->Initialize( Position, Yaw );
        ObjMgr.AddObject( pGrav, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }
    else if( x_stricmp( pToken, "SHRIKE" ) == 0 )
    {
        vector3 Position;
        radian  Yaw;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Yaw = Mission.ReadFloat();

        shrike* pShrike = (shrike*)ObjMgr.CreateObject( object::TYPE_VEHICLE_SHRIKE );
        pShrike->Initialize( Position, Yaw );
        ObjMgr.AddObject( pShrike, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }
    else if( x_stricmp( pToken, "ASSTANK" ) == 0 )
    {
        vector3 Position;
        radian  Yaw;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Yaw = Mission.ReadFloat();

        asstank* pTank = (asstank*)ObjMgr.CreateObject( object::TYPE_VEHICLE_BEOWULF );
        pTank->Initialize( Position, Yaw );
        ObjMgr.AddObject( pTank, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }
    else if( x_stricmp( pToken, "TRANSPORT" ) == 0 )
    {
        vector3 Position;
        radian  Yaw;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Yaw = Mission.ReadFloat();

        transport* pTrans = (transport*)ObjMgr.CreateObject( object::TYPE_VEHICLE_HAVOC );
        pTrans->Initialize( Position, Yaw );
        ObjMgr.AddObject( pTrans, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }
    else if( x_stricmp( pToken, "BOMBER" ) == 0 )
    {
        vector3 Position;
        radian  Yaw;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Yaw = Mission.ReadFloat();

        bomber* pBomb = (bomber*)ObjMgr.CreateObject( object::TYPE_VEHICLE_THUNDERSWORD );
        pBomb->Initialize( Position, Yaw );
        ObjMgr.AddObject( pBomb, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }
    else if( x_stricmp( pToken, "MPB" ) == 0 )
    {
        vector3 Position;
        radian  Yaw;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Yaw = Mission.ReadFloat();

        mpb* pMPB = (mpb*)ObjMgr.CreateObject( object::TYPE_VEHICLE_JERICHO2 );
        pMPB->Initialize( Position, Yaw );
        ObjMgr.AddObject( pMPB, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }
    */
    //--------------------------------------------------------------------------
    //  PLACE HOLDER
    //--------------------------------------------------------------------------
    else if( x_stricmp( pToken, "PLACE_HOLDER" ) == 0 )
    {
        vector3 Position;
        radian3 Rotation;
        char*   pLabel;

        Position.X = Mission.ReadFloat();
        Position.Y = Mission.ReadFloat();
        Position.Z = Mission.ReadFloat();

        Rotation.Pitch = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Yaw   = DEG_TO_RAD( Mission.ReadFloat() );
        Rotation.Roll  = DEG_TO_RAD( Mission.ReadFloat() );

        pLabel = Mission.ReadString();

        place_holder* pPlaceHolder = (place_holder*)ObjMgr.CreateObject( object::TYPE_PLACE_HOLDER );
        pPlaceHolder->Initialize( Position, Rotation, pLabel );
        ObjMgr.AddObject( pPlaceHolder, obj_mgr::AVAILABLE_SERVER_SLOT, Tag );
    }

    //--------------------------------------------------------------------------
    //  What the f..?  Um.  Oh well, unknown token.
    //--------------------------------------------------------------------------
    else
    {
#ifndef RELEASE_CANDIDATE
        x_printf( "Unexpected token found while reading mission file: \"%s\"\n", pToken );
#endif
    }

    return( FALSE );
}

//==============================================================================

void StartOfMissionLoad( void )
{
    //x_MemDump( "BeforeMissionLoad.txt" );

    NSpawnSpheres = 0;
    tgl.NSpawnPoints = 0;
    tgl.NRenders     = 0;

    if (tgl.ServerPresent)
    // Load nav graph for bots
    //if( fegl.ServerSettings.BotsEnabled )
    {
        // Check if file is available
        X_FILE* fp = x_fopen(xfs("%s/%s.gph",tgl.MissionDir,tgl.MissionName),"rb");
        if( fp ) 
        {
            x_fclose(fp);
            PathManager_LoadGraph( xfs("%s/%s.gph",tgl.MissionDir,tgl.MissionName) );
            // load graph
            g_Graph.Load( xfs( "%s/%s.gph", tgl.MissionDir, tgl.MissionName), TRUE );
        }
    }
}

//==============================================================================

xbool LoadMission( void )
{
    // Update mission stage
    LoadMissionStage++;

    // If the mission file has not yet been opened, open it!
    if( !MissionFileOpen )
    {
#if 0
	#if defined(TARGET_PS2_DEV) && defined(bwatson)
		char str[32];
		static s32 lp=0;

		if (x_fopen_log)
		{
			x_fopen_log->Clear();
		}

		x_sprintf(str,"memdump_%02d.txt",lp);
		lp++;
		x_MemDump(str);
	#endif
#endif
        x_MemAddMark( "Begin Loading Mission" );

        // Load the strings for this mission.
        StringMgr.LoadTable( "Mission", xfs( "%s/Strings.bin", tgl.MissionDir ) );
        if( fegl.GameType == GAME_CAMPAIGN )
            StringMgr.LoadTable( "Campaign", "Data\\Missions\\Campaign.bin" );

        VERIFY( Mission.OpenFile( xfs( "%s/%sMission.txt", tgl.MissionDir, tgl.MissionPrefix ) ) );
        MissionFileOpen = TRUE;

        StartOfMissionLoad();
    }

    // Read the next token from the mission file.  Must be a symbol or EOF.
    Mission.Read();
    if( (Mission.Type() == token_stream::TOKEN_EOF) ||
        ((x_stricmp( Mission.String(), "WATERMARK" ) == 0) && (tgl.ClientPresent)))
    {
        Mission.CloseFile();
        MissionFileOpen = FALSE;

        x_MemAddMark( "Done Loading Mission" );

        // All mission objects are loaded.  Do last minute stuff
        EndOfMissionLoad();

        return( TRUE );
    };

    x_MemAddMark( Mission.String() );

    // Set the correct mission dir for the building loader
    g_BldManager.SetMissionDir( tgl.MissionDir );

    // Process this object
    ProcessEntry( Mission );

    // Set Parametric MissionLoadCompletion
    if( Mission.GetFileSize() > 0 )
        tgl.MissionLoadCompletion = (f32)Mission.GetCursor() / (f32)Mission.GetFileSize();

    return( FALSE );
}

//==============================================================================

void EndLoadMission( void )
{
    if( MissionFileOpen )
    {
        Mission.CloseFile();
        MissionFileOpen = FALSE;
        x_MemAddMark( "******** Done Loading Mission ********" );
    }

}

//==============================================================================

void UnloadMission( void )
{
    audio_Stop(tgl.MusicId);
    tgl.MusicId = 0;

    ObjMgr.Clear();
    
    if( tgl.ServerPresent ) 
        tgl.pServer->UnloadMission();
    
    if( tgl.ClientPresent )
        tgl.pClient->UnloadMission();

    tgl.Sky.Kill();
    tgl.Fog.Unload();

    // Dump EnvShapeManager
    tgl.EnvShapeManager.DeleteAllShapes();

    // Dump navgraph
    g_Graph.Unload();

    // Lose strings.
    if( fegl.GameType == GAME_CAMPAIGN )
        StringMgr.UnloadTable( "Campaign" );
    StringMgr.UnloadTable( "Mission" );

    // Shut down master AI

    // Clear mission in tgl and fegl
    if( tgl.ClientPresent )
        x_strcpy( fegl.MissionName, "" );
    x_strcpy( tgl.MissionName, "" );
    x_strcpy( tgl.MissionDir, "" );
}

//==============================================================================
