//==============================================================================
//
//  MPB.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "MPB.hpp"
#include "..\Demo1\Globals.hpp"
#include "Entropy.hpp"
#include "ObjectMgr\Object.hpp"
#include "Audiomgr/Audio.hpp"
#include "NetLib\BitStream.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Objects/Projectiles/Mortar.hpp"
#include "Objects/Projectiles/Bullet.hpp"
#include "Objects/Projectiles/Turret.hpp"
#include "Objects/Projectiles/Station.hpp"
#include "Objects/Projectiles/ParticleObject.hpp"
#include "GameMgr/GameMgr.hpp"
#include "vfx.hpp"
#include "HUD\hud_Icons.hpp"

//==============================================================================
//  DEFINES
//==============================================================================

// Thrust defines
#define     NORMAL_THRUST       100.0f
#define     DEPLOYED_THRUST     30.0f

// Deploy defines
#define     TIME_TO_DEPLOY      6.0f
#define     DEPLOY_BLDG_RAD     50.0f
#define     DEPLOY_PLAYER_RAD   3.0f

// Camera defines
static vector3 MPBPilotEyeOffset_3rd( 0.0f, 9.0f, -30.0f );

// Collision model indices defines
#define COLL_MODEL_CLOSED       0   // Fully closed
#define COLL_MODEL_TRANSITION   1   // In middle of opening/closing
#define COLL_MODEL_DEPLOYED     2   // Fully open




//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   MPBCreator;

obj_type_info   MPBClassInfo( object::TYPE_VEHICLE_JERICHO2, 
                                    "MPB", 
                                    MPBCreator, 
                                    object::ATTR_REPAIRABLE       | 
                                    object::ATTR_ELFABLE          |
                                    object::ATTR_SOLID_DYNAMIC |
                                    object::ATTR_VEHICLE |
                                    object::ATTR_DAMAGEABLE |
                                    object::ATTR_SENSED |
                                    object::ATTR_HOT |
                                    object::ATTR_LABELED );


static s32      s_GlowL, s_GlowR, s_Ext1, s_Ext2;
static vfx_geom s_VFX;

//==============================================================================

// IMPORTANT NOTE: You need to set these values!

static veh_physics s_MPBPhysics;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void MPB_InitFX( void )
{
    s_VFX.CreateFromFile( "data/vehicles/MPB.vd" );

    // find my glowies
    s_GlowL = s_VFX.FindGeomByID( "LEFTGLOWPT" );
    s_GlowR = s_VFX.FindGeomByID( "RIGHTGLOWPT" );
    s_Ext1 = s_VFX.FindGeomByID( "BOTEXT1" );
    s_Ext2 = s_VFX.FindGeomByID( "BOTEXT2" );
}

object* MPBCreator( void )
{
    return( (object*)(new mpb) );
}  

//==============================================================================
//  MEMBER FUNCTIONS
//==============================================================================

mpb::mpb( void )
{
    m_AssetsCreated = FALSE;
}

//==============================================================================

void mpb::OnAdd( void )
{
    gnd_effect::OnAdd();

    // Set the vehicle type
    m_VehicleType = TYPE_JERICHO;

    // not yet deployed
    m_DeployedState = NOT_DEPLOYED;
    m_DeployTime    = 0 ;
    m_ImpulseTimer  = 0.0f;
    m_DeploySfx     = -1;
    m_StationID     = obj_mgr::NullID;
    m_TurretID      = obj_mgr::NullID;
    m_ColBoxYOff    = 4.5f;

    m_ClientProcessedState = FALSE;

    // create the hovering smoke effect
    m_HoverSmoke.Create( &tgl.ParticlePool,
                         &tgl.ParticleLibrary,
                         PARTICLE_TYPE_HOVER_SMOKE3,
                         m_WorldPos, vector3(0,-1.0f,0),
                         NULL,
                         NULL );

    // disable it
    m_HoverSmoke.SetEnabled( FALSE );

    // create the hovering smoke effect
    m_DamageSmoke.Create( &tgl.ParticlePool,
                         &tgl.ParticleLibrary,
                         PARTICLE_TYPE_DAMAGE_SMOKE3,
                         m_WorldPos, vector3(0,0,0),
                         NULL,
                         NULL );

    // disable it
    m_DamageSmoke.SetEnabled( FALSE );

    // start audio
    m_AudioEng =        audio_Play( SFX_VEHICLE_WILDCAT_ENGINE_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );
    m_AudioAfterBurn = -1;

    // set the other 2 sounds to low volume for now
    //audio_SetVolume     ( m_AudioEng, 0.5f );

    // Set the label
    {
        xwstring Name( "Jericho" );
        x_wstrcpy( m_Label, (const xwchar*)Name );
    }

    m_BubbleScale( 18, 12, 20 );

    //**** DEPLOY ****
    m_GeomInstance.SetAnimByType( ANIM_TYPE_VEHICLE_DEPLOY );
}

//==============================================================================

void mpb::WriteDirtyBitsData  ( bitstream& BitStream, u32& DirtyBits, f32 Priority, xbool MoveUpdate )
{
    if ( BitStream.WriteFlag( DirtyBits & VEHICLE_DIRTY_BIT_STATE ) )
    {
        // Current deploy state
        BitStream.WriteRangedS32( m_DeployedState, NOT_DEPLOYED, UNDEPLOYING );

        // write the object ID's of the assets
        BitStream.WriteObjectID( m_TurretID );
        BitStream.WriteObjectID( m_StationID );
    }

    // Call base class
    gnd_effect::WriteDirtyBitsData(BitStream, DirtyBits, Priority, MoveUpdate) ;
}

//==============================================================================

xbool mpb::ReadDirtyBitsData   ( const bitstream& BitStream, f32 SecInPast )
{
    // read state into tmp first, to make sure we can use it
    s32   Int;
    state tmpstate;

    if ( BitStream.ReadFlag() )
    {
        state OldState = m_DeployedState;
    
        // read the state
        BitStream.ReadRangedS32( Int, NOT_DEPLOYED, UNDEPLOYING );
        tmpstate = (state)Int;

        // read the object ID's
        BitStream.ReadObjectID( m_TurretID );
        BitStream.ReadObjectID( m_StationID );

        // Let the clients arrive at these 2 states on its own
        if( (tmpstate != DEPLOYED) && (tmpstate != NOT_DEPLOYED) )
        {
            // prevent server from taking us backwards in states
            if ( (!((m_DeployedState == NOT_DEPLOYED) && (tmpstate == UNDEPLOYING))) &&
                 (!((m_DeployedState == DEPLOYED) && (tmpstate == DEPLOYING))) )
            {
                m_DeployedState = tmpstate;
        
                // make sure we are dealing with a new state before clearing the flag
                if( OldState != m_DeployedState )
                    m_ClientProcessedState = FALSE;
            }
        }
    }

    // Call base class
    return gnd_effect::ReadDirtyBitsData(BitStream, SecInPast) ;
}

//==============================================================================


void mpb::Initialize( const vector3& Pos, radian InitHdg, u32 TeamBits )
{
    gnd_effect::InitPhysics( &s_MPBPhysics );
    
    // call the base class initializers with info specific to mpb
    gnd_effect::Initialize( SHAPE_TYPE_VEHICLE_JERICHO2,        // Geometry shape
                            SHAPE_TYPE_VEHICLE_JERICHO2_COLL,   // Collision shape
                            Pos,                                // position
                            InitHdg,                            // heading
                            2.0f,                               // Seat radius 
                            TeamBits );

    m_DeployedState = NOT_DEPLOYED;
}

//==============================================================================

void mpb::SetupSeat( s32 Index, seat& Seat )
{
    (void)Index ;

/*
    // Setup character types that can get in seat
    Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_LIGHT]  = TRUE ;
    Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_MEDIUM] = TRUE ;
    Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_HEAVY]  = FALSE ;

    Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_LIGHT]    = TRUE ;
    Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_MEDIUM]   = TRUE ;
    Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_HEAVY]    = FALSE ;

    Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_LIGHT]     = TRUE ;
    Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_MEDIUM]    = TRUE ;
    Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_HEAVY]     = FALSE ;
*/
    Seat.CanLookAndShoot = FALSE ;
    Seat.PlayerAnimType  = ANIM_TYPE_CHARACTER_SITTING ;
}

//==============================================================================

void mpb::OnAcceptInit( const bitstream& BitStream )
{
    // Make valid object
    Initialize( vector3(0,0,0), 0, 0 );

    // Now call base call accept init to override and setup vars
    gnd_effect::OnAcceptInit( BitStream );
}

//==============================================================================

object::update_code mpb::Impact( vector3& Pos, vector3& Norm )
{
    (void)Pos;
    (void)Norm;

    return UPDATE;
}

//==============================================================================

void mpb::UpdateInstances( void )
{
    // Control anim?
    if (m_State != STATE_SPAWN)
    {
        // Control deploy anim using deploy time
        m_GeomInstance.SetAnimByType(ANIM_TYPE_VEHICLE_DEPLOY) ;
        m_GeomInstance.GetCurrentAnimState().SetSpeed(0) ;
        m_GeomInstance.GetCurrentAnimState().SetFrameParametric(m_DeployTime / TIME_TO_DEPLOY) ;

        // update the MPB position as it deploys
        if ( m_DeployTime > 0.0f )
            SetMPBPos();
    }

    // Set collision model
    /*
    if (m_DeployTime == TIME_TO_DEPLOY) // Fully deployed?
        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_DEPLOYED) ;
    else
    if (m_DeployTime == 0)              // Fully closed?
        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_CLOSED) ;
    else    
        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_TRANSITION) ;    // In middle of deployed/closed
    */

    // Call base class
    vehicle::UpdateInstances() ;
}

//==============================================================================

void mpb::Render( void )
{
    // Begin with base render...
    gnd_effect::Render();

    // Render a marker on the MPB for the team.
    if( m_DeployedState == DEPLOYED )
    {
        u32 ContextBits = 0x00;
        object::id PlayerID( tgl.PC[tgl.WC].PlayerIndex, -1 );

        // Get the rendering context team bits.
        player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
        if( pPlayer )
        {    
            ContextBits = pPlayer->GetTeamBits();
            if( ContextBits & m_TeamBits )
            {
                HudIcons.Add( hud_icons::BEACON_MARK, m_WorldPos );
            }
        }
    }
//  m_CollInstance.DrawCollisionModel();
}

//==============================================================================

void mpb::RenderParticles( void )
{
    if( !m_IsVisible )
        return;

    // render the base class particles
    gnd_effect::RenderParticles();

    if( m_Exhaust1.IsCreated() )
    {
        m_Exhaust1.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST1 ) ) ;
        m_Exhaust1.RenderEffect();
    }

    if( m_Exhaust2.IsCreated() )
    {
        m_Exhaust2.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST2 ) ) ;
        m_Exhaust2.RenderEffect();
    }

    // render glowies
    matrix4 L2W = m_GeomInstance.GetL2W();

    // render glowies
    if ( m_Control.ThrustY >= 0.0f )
    {

        // render glowies
        s_VFX.RenderGlowyPtSpr( L2W, s_GlowL, m_Control.ThrustY * x_frand(0.5f, 0.75f));
        s_VFX.RenderGlowyPtSpr( L2W, s_GlowR, m_Control.ThrustY * x_frand(0.5f, 0.75f));
    }

    // render vertical extrusion
    s_VFX.RenderExtrusion(   L2W, 
                             s_Ext1,
                             s_Ext2, 
                             1.0f , 
                             0.1f * x_frand(0.0f, 1.0f), 
                             0.0f      );

}

//==============================================================================

void mpb::Get3rdPersonView( s32 SeatIndex, vector3& Pos, radian3& Rot )
{
    (void)SeatIndex;

    // Special case yaw
    radian3 FreeLookYaw = Rot ;

    // Make sure shape instances are in sync with actual object
    UpdateInstances();

    ASSERT( m_GeomInstance.GetHotPointByType(HOT_POINT_TYPE_VEHICLE_EYE_POS) ) ;
    Pos = MPBPilotEyeOffset_3rd;
    Pos.Rotate( m_Rot );
    Pos.Rotate( FreeLookYaw );
    Pos += m_GeomInstance.GetHotPointWorldPos(HOT_POINT_TYPE_VEHICLE_EYE_POS); 
    Rot += m_Rot ;
}

//==============================================================================

void mpb::Get1stPersonView( s32 SeatIndex, vector3& Pos, radian3& Rot )
{
    // Make sure shape instances are in sync with actual object
    UpdateInstances() ;

    // Call base class
    vehicle::Get1stPersonView( SeatIndex, Pos, Rot) ;
}

//==============================================================================

void mpb::ApplyMove ( player_object* pPlayer, s32 SeatIndex, player_object::move& Move )
{
    // Call base class to position the turret etc
    gnd_effect::ApplyMove( pPlayer, SeatIndex, Move );

    // play audio for jetting
    if ( ( Move.JetKey ) && ( m_AudioAfterBurn == -1 ) && ( m_Energy > 10.0f) )
        m_AudioAfterBurn =  audio_Play( SFX_VEHICLE_JERICHO_AFTERBURNER_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );
}

//==============================================================================

object::update_code mpb::OnAdvanceLogic  ( f32 DeltaTime )
{
    object::update_code RetVal = UPDATE;
    matrix4 EP1;
    // set the axis for exhaust
    vector3 Axis;

    // Wait for spawn to complete
    if (m_State == STATE_SPAWN)
    {
        RetVal = gnd_effect::OnAdvanceLogic( DeltaTime );
    }
    else
    {
        // reset the audio variable when finished playing
        if ( !audio_IsPlaying( m_DeploySfx ) )
            m_DeploySfx = -1;

        // only call the base class if not deployed
        if ( tgl.ServerPresent )
        {
            // keep track of if there was a state change, so that the dirty bit can be set
            state OldState = m_DeployedState;

            switch( m_DeployedState )
            {
                case NOT_DEPLOYED:
                    //=========================================================
                    if( GetSeatPlayer(0) == NULL )
                    {
                        m_DeployedState = ATTEMPTING_TO_DEPLOY;
                    }
                    break;

                case ATTEMPTING_TO_DEPLOY:
                    //=========================================================
                    // First, make sure driver still MIA
                    if ( GetSeatPlayer(0) != NULL )
                    {
                        // he's back, so you're not attempting any more
                        m_DeployedState = NOT_DEPLOYED;
                        break;
                    }

                    // can't deploy near bldgs, so no need checking further if we're near a bldg or too steep
                    if ( (!CheckForBldgs()) && CheckForValidSlope() )
                    {
                        // if someone is near the turret area, attempt to clear them out
                        if ( CheckTurretArea() )
                        {
                            // spawn impulse forces to clear anyone out of the area
                            m_ImpulseTimer += DeltaTime;
                            if ( m_ImpulseTimer > 0.1f )
                            {
                                vector3 Pos = m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_TURRET_MOUNT );
                                SpawnExplosion( pain::MISC_FLIP_FLOP_FORCE, Pos, vector3(0,1,0), 
                                                this->GetObjectID(), obj_mgr::NullID, 1.0f );
                                m_ImpulseTimer = 0.0f;
                            }
                        }
                        else
                        {
                            // ok, turret area clear...all is good...create assets and deploy

                            // remember where we are
                            m_PreDeployPos = m_WorldPos;

                            // calculate how far to sink
                            CalcSink( );

                            m_DeployedState = DEPLOYING;
                            m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_TRANSITION) ;
                            CreateAssets();
                            EnableAssets(FALSE);
                            
                            // no more movement
                            m_IsImmobile = TRUE;

                            // play the deploy sound
                            audio_Play( SFX_VEHICLE_MOUNT, &m_WorldPos ); // Changed SFX call
                        }
                    }
                    break;

                case DEPLOYING:
                    //=========================================================
                    // Open up
                    m_DeployTime += DeltaTime;
                    if ( m_DeployTime > TIME_TO_DEPLOY )
                    {
                        // Done
                        m_DeployTime = TIME_TO_DEPLOY ;

                        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_DEPLOYED) ;

                        m_DeployedState = DEPLOYED;
                        EnableAssets( TRUE );
                    
                        pGameLogic->ItemDeployed( m_ObjectID, obj_mgr::NullID );
                    }
                    break;

                case DEPLOYED:
                    //=========================================================
                    // watch the driver's seat to see if we should attempt to undeploy
                    if ( GetSeatPlayer(0) != NULL )
                    {
                        m_DeployedState = ATTEMPTING_TO_UNDEPLOY;
                    }
                    break;

                case ATTEMPTING_TO_UNDEPLOY:
                    //=========================================================
                    // First, make sure there's still someone in the seat
                    if ( GetSeatPlayer(0) == NULL )
                    {
                        m_DeployedState = DEPLOYED;
                        break;
                    }

                    // Otherwise, check the inven area to make sure it's clear before closing up
                    if ( CheckInvenArea() )
                    {
                        // inven area is NOT clear...pulse the bastards outta there.
                        m_ImpulseTimer += DeltaTime;
                        if ( m_ImpulseTimer > 0.1f )
                        {
                            vector3 Pos = m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_INVENT_MOUNT );
                            SpawnExplosion( pain::MISC_FLIP_FLOP_FORCE, Pos, vector3(0,1,0), 
                                            this->GetObjectID(), obj_mgr::NullID, 1.0f );
                            m_ImpulseTimer = 0.0f;
                        }
                    }
                    else
                    {
                        // Inven area is now clear, we can make final preparations to pack up

                        // next state
                        m_DeployedState = UNDEPLOYING;                        
                        // set the appropriate collision model
                        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_CLOSED) ;
                        // Hold your fire!
                        EnableAssets( FALSE );
                        // Play the audio
                        audio_Play( SFX_VEHICLE_MOUNT, &m_WorldPos ); // changed SFX call
                    }
                    break;

                case UNDEPLOYING:
                    //=========================================================
                    // Pack it up, we're outta here
                    m_DeployTime -= DeltaTime;

                    if ( m_DeployTime < 0.0f )
                    {
                        // clamp the value
                        m_DeployTime = 0.0f;

                        // Done
                        m_IsImmobile = FALSE;
                        m_DeployTime = 0.0f ;

                        // set back to default state
                        m_DeployedState = NOT_DEPLOYED;

                        // get rid of the assets
                        DestroyAssets();
                    }

                    break;

                default:
                    ASSERT(0);
            }

            // if there was a change in state, set the dirty bit
            if ( m_DeployedState != OldState )
                m_DirtyBits |= VEHICLE_DIRTY_BIT_STATE;
        }
        else
        {
            // for initial state processing
            if ( m_ClientProcessedState == FALSE )
            {
                switch ( m_DeployedState )
                {
                    case NOT_DEPLOYED:
                        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_CLOSED) ;
                        m_IsImmobile = FALSE;
                        break;
                    
                    case ATTEMPTING_TO_DEPLOY:
                        break;
                    
                    case DEPLOYING:
                        m_IsImmobile = TRUE;
                        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_TRANSITION) ;
                        audio_Play( SFX_VEHICLE_MOUNT, &m_WorldPos ); // Changed SFX call
                        m_DeployTime = 0.0f;
                        break;
                    
                    case DEPLOYED:
                        m_IsImmobile = TRUE;
                        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_DEPLOYED) ;
                        break;
                    
                    case ATTEMPTING_TO_UNDEPLOY:
                        break;

                    case UNDEPLOYING:
                        m_CollInstance.SetCollisionModelByIndex(COLL_MODEL_TRANSITION) ;
                        audio_Play( SFX_VEHICLE_MOUNT, &m_WorldPos ); // Changed SFX call
                        m_DeployTime = TIME_TO_DEPLOY;
                        break;
                    
                    default:
                        ASSERT(0);
                }

                //x_printf("Processed %d\n", m_DeployedState );
    
                // set the flag
                m_ClientProcessedState = TRUE;
            }
            // for subsequent state processing
            else
            {
                switch ( m_DeployedState )
                {
                    case NOT_DEPLOYED:
                        break;
                    
                    case ATTEMPTING_TO_DEPLOY:
                        break;
                    
                    case DEPLOYING:
                        // Open up
                        m_DeployTime += DeltaTime;
                        if (m_DeployTime > TIME_TO_DEPLOY)
                        {
                            // Done
                            m_DeployTime = TIME_TO_DEPLOY ;
                            // the only client-side "logic"
                            m_DeployedState = DEPLOYED;     
                            m_ClientProcessedState = FALSE;
                        }
                        break;
                    
                    case DEPLOYED:
                        break;

                    case ATTEMPTING_TO_UNDEPLOY:
                        break;

                    case UNDEPLOYING:
                        // continue trying to put everything away in case we changed state too soon
                        m_DeployTime -= DeltaTime;
                        if ( m_DeployTime < 0.0f )
                        {
                            // Done
                            m_DeployTime = 0.0f ;
                            // the only client-side "logic"
                            m_DeployedState = NOT_DEPLOYED;
                            m_ClientProcessedState = FALSE;
                        }
                        break;

                    default:
                        ASSERT(0);
                }
            }
        }


        // call base class logic
        RetVal = gnd_effect::OnAdvanceLogic( DeltaTime );

    }

    // Update exhaust
    EP1 = m_GeomInstance.GetHotPointL2W( HOT_POINT_TYPE_VEHICLE_EXHAUST1 );
    Axis.Set( 0.0f, 0.0f, -1.0f );
    EP1.SetTranslation( vector3(0.0f, 0.0f, 0.0f) );
    Axis = EP1 * Axis;

    Axis.RotateX( x_frand(-R_5, R_5) );
    Axis.RotateY( x_frand(-R_5, R_5) );

    // update particle effect
    if ( m_Exhaust1.IsCreated() )
    {
        m_Exhaust1.GetEmitter(0).SetAxis( Axis );
        m_Exhaust1.GetEmitter(0).UseAxis( TRUE );
        m_Exhaust1.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST2 ) ) ;
        m_Exhaust1.SetVelocity( vector3(0.0f, 0.0f, 0.0f) );
        //m_Exhaust1.SetVelocity( (EP1.GetTranslation() - m_Exhaust1.GetPosition()) / DeltaTime );
        m_Exhaust1.UpdateEffect( DeltaTime );
    }
    
    // update particle effect
    if ( m_Exhaust2.IsCreated() )
    {
        m_Exhaust2.GetEmitter(0).SetAxis( Axis );
        m_Exhaust2.GetEmitter(0).UseAxis( TRUE );
        m_Exhaust2.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST1 ) ) ;
        m_Exhaust2.SetVelocity( vector3(0.0f, 0.0f, 0.0f) );
        //m_Exhaust2.SetVelocity( (EP1.GetTranslation() - m_Exhaust2.GetPosition()) / DeltaTime );
        m_Exhaust2.UpdateEffect( DeltaTime );
    }
    
    // update audio
    audio_SetPosition   ( m_AudioEng, &m_WorldPos );
    audio_SetVolume     ( m_AudioEng, m_Control.ThrustY * 0.20f + 0.80f );
    
    if ( m_AudioAfterBurn != -1 )
    {
        if ( ( m_Control.Boost > 0.0f ) && (m_Energy > 10.0f) )
        {
            audio_SetVolume (m_AudioAfterBurn, 1.0f );
            audio_SetPosition   ( m_AudioAfterBurn, &m_WorldPos );
        }
        else
        {
            audio_Stop( m_AudioAfterBurn );
            m_AudioAfterBurn = -1;
        }
    }

    // Update the assets' position.  
    // TO DO: Should only be needed during DEPLOYING and PACKING (when asset is moving).
    // if( m_AssetsCreated )
    if ( m_StationID != obj_mgr::NullID )
    {
        // make sure hotpoints are up to date
        UpdateInstances();

        asset* pAsset;

        pAsset = (asset*)ObjMgr.GetObjectFromID( m_TurretID );
        if ( pAsset )
        {
            if( m_TurretID.Seq == -1 )
                m_TurretID.Seq = pAsset->GetObjectID().Seq;

            pAsset->MPBSetL2W( m_GeomInstance.GetHotPointL2W( HOT_POINT_TYPE_TURRET_MOUNT ) );
        }

        pAsset = (asset*)ObjMgr.GetObjectFromID( m_StationID );

        if ( pAsset )
        {
            if( m_StationID.Seq == -1 )
                m_StationID.Seq = pAsset->GetObjectID().Seq;

            pAsset->MPBSetL2W( m_GeomInstance.GetHotPointL2W( HOT_POINT_TYPE_INVENT_MOUNT ) );
        }
    }

    return( RetVal );
}

//==============================================================================

void mpb::OnRemove( void )
{

    if ( m_AssetsCreated )
    {
        DestroyAssets();
    }

    audio_Stop(m_AudioEng);
    audio_Stop(m_AudioAfterBurn); 
    
    if ( m_DeploySfx != -1 )
        audio_Stop( m_DeploySfx );

    gnd_effect::OnRemove();
}

//==============================================================================

f32 mpb::GetPainRadius( void ) const
{
    return( 5.0f );
}

//==============================================================================

void mpb::CreateAssets( void )
{
    // Turret.

    // make sure hotpoints are up to date
    UpdateInstances();

    {
        matrix4 L2W;
        turret* pTurret;

        L2W = m_GeomInstance.GetHotPointL2W( HOT_POINT_TYPE_TURRET_MOUNT );

        pTurret = (turret*)ObjMgr.CreateObject( object::TYPE_TURRET_FIXED );
        pTurret->MPBInitialize( L2W, m_TeamBits );
        ObjMgr.AddObject( pTurret, obj_mgr::AVAILABLE_SERVER_SLOT );
        m_TurretID = pTurret->GetObjectID();
    }

    // Inventory station.
    {
        matrix4  L2W;
        station* pStation;

        L2W = m_GeomInstance.GetHotPointL2W( HOT_POINT_TYPE_INVENT_MOUNT );

        pStation = (station*)ObjMgr.CreateObject( object::TYPE_STATION_FIXED );
        pStation->MPBInitialize( L2W, m_TeamBits );
        ObjMgr.AddObject( pStation, obj_mgr::AVAILABLE_SERVER_SLOT );
        m_StationID = pStation->GetObjectID();
    }

    m_AssetsCreated = TRUE;
}

//==============================================================================

void mpb::DestroyAssets( void )
{
    ObjMgr.DestroyObject( m_TurretID ); 
    ObjMgr.DestroyObject( m_StationID );

    m_StationID = obj_mgr::NullID;
    m_TurretID = obj_mgr::NullID;

    m_AssetsCreated = FALSE;
}

//==============================================================================

void mpb::EnableAssets( xbool Enable )
{
    object::id  ID;
    asset*      pTurret  = (asset*)ObjMgr.GetObjectFromID( m_TurretID  );
    asset*      pStation = (asset*)ObjMgr.GetObjectFromID( m_StationID );

    if( Enable )
    {
        if( pTurret )   pTurret ->Enabled( ID );
        if( pStation )  pStation->Enabled( ID );
    }
    else
    {
        if( pTurret )   pTurret ->Disabled( ID );
        if( pStation )  pStation->Disabled( ID );
    }
}

//==============================================================================

xbool mpb::CheckTurretArea( void )
{
    // search for objects near turret
    object* pObject;
    object* pTurret = ObjMgr.GetObjectFromID( m_TurretID );

    vector3 Pos = m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_TURRET_MOUNT );

    ObjMgr.Select( object::ATTR_ALL, 
                   Pos,
                   DEPLOY_PLAYER_RAD );
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        if( (pObject != this) && (pObject != pTurret) )
            break;
    }
    ObjMgr.ClearSelection();

    return( pObject != NULL );
}

//==============================================================================

xbool mpb::CheckInvenArea( void )
{
    // search for objects near inven
    object* pObject;
    object* pInven = ObjMgr.GetObjectFromID( m_StationID );

    vector3 Pos = m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_INVENT_MOUNT );

    ObjMgr.Select( ATTR_ALL, 
                   Pos,
                   DEPLOY_PLAYER_RAD );
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        if( (pObject != this) && (pObject != pInven) && (pObject->GetObjectID() != m_TurretID) )
            break;
    }
    ObjMgr.ClearSelection();

    return( pObject != NULL );
}

//==============================================================================

xbool mpb::CheckForBldgs( void )
{
    object* pObject;

    // search for nearby bases
    ObjMgr.Select( object::ATTR_BUILDING, 
                   m_WorldPos,
                   DEPLOY_BLDG_RAD );
    pObject = ObjMgr.GetNextSelected();
    ObjMgr.ClearSelection();
    return( pObject != NULL );
}

//==============================================================================

xbool mpb::CheckForValidSlope( void )
{
    // can't deploy unless on the ground
    if ( m_OnGround == FALSE )
        return FALSE;

    vector3 vec( 0, 1, 0 );

    // rotate the vector
    vec.Rotate( m_Rot ) ;

    radian ang = v3_AngleBetween( vec, vector3(0,1,0) );

    return ang < R_15 ;
}

//==============================================================================

void mpb::CalcSink( void )
{
    collider    ColRay;
    vector3     StartPts[2];
    vector3     EndPts[2];
    f32         BestTime = 1.0f;
    vector3     BestDelta;

    // calculate how far down we go when we deploy

    // first, get ourself 2 "down" vectors and orient them to the MPB
    StartPts[0].Set( 0, 1.0f, 1.0f );
    StartPts[0].Rotate( m_Rot );

    StartPts[1].Set( 0, 1.0f, -1.0f );
    StartPts[1].Rotate( m_Rot );

    EndPts[0].Set( 0, 10.0f, 1.0f );
    EndPts[0].Rotate( m_Rot );

    EndPts[1].Set( 0, -10.0f, -1.0f );
    EndPts[1].Rotate( m_Rot );

    // make sure hotpoints are up to date
    UpdateInstances();

    // get the station hotpoint
    vector3 station = m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_INVENT_MOUNT );

    for ( s32 i = 0; i < 2; i++ )
    {
        // now calc dist to terrain
        ColRay.RaySetup( this, station + StartPts[i], station + EndPts[i], this->GetObjectID().GetAsS32() );

        ObjMgr.Collide( ATTR_SOLID_STATIC, ColRay );

        // get the collision point, and transform it to be in relation to m_WorldPos
        if ( ColRay.HasCollided() )
        {
            collider::collision ColInfo;

            ColRay.GetCollision( ColInfo );

            if ( ColInfo.T < BestTime )
            {
                BestDelta = ColInfo.Point - ( station + StartPts[i] );
                BestTime = ColInfo.T ;
            }
        }
        else
        {
            // This should never happen, but handle it gracefully in release if it does (no movement)
            ASSERT( FALSE );
            m_PostDeployPos = m_WorldPos;
            return;
        }
    }

    // use the best point to calculate a deploy depth (then raise it up a little)
    // also subtract out the extra meter (see the StartPts definition)
    f32 Len = BestDelta.Length();

    Len -= 1.0f;        // for the extra meter (see the StartPts definition)
    Len -= 0.2f;        // for a little buffer

    BestDelta.Normalize();
    BestDelta *= Len;

    m_PostDeployPos = m_WorldPos + BestDelta;
}

//==============================================================================

void mpb::SetMPBPos( void )
{
    // set the position of the MPB relative to the deploy time (only on the server)
    if ( tgl.ServerPresent )
    {
        vector3 OldPos = m_WorldPos;

        m_WorldPos = m_PreDeployPos + ( ( m_PostDeployPos - m_PreDeployPos ) * m_DeployTime / TIME_TO_DEPLOY) ;

        if ( OldPos != m_WorldPos )
            m_DirtyBits |= VEHICLE_DIRTY_BIT_POS;
    }
}