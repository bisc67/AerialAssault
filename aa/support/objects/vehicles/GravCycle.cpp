//==============================================================================
//
//  GravCycle.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================
#include "..\Demo1\Globals.hpp"
#include "Entropy.hpp"
#include "NetLib\BitStream.hpp"
#include "ObjectMgr\Object.hpp"
#include "GravCycle.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "audiomgr/audio.hpp"
#include "vfx.hpp"
#include "StringMgr\StringMgr.hpp"
#include "Data\UI\ui_strings.h"

//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   GravCycleCreator;

obj_type_info   GravCycleClassInfo( object::TYPE_VEHICLE_WILDCAT, 
                                    "GravCycle", 
                                    GravCycleCreator, 
                                    object::ATTR_REPAIRABLE       | 
                                    object::ATTR_ELFABLE          |
                                    object::ATTR_SOLID_DYNAMIC |
                                    object::ATTR_VEHICLE |
                                    object::ATTR_DAMAGEABLE |
                                    object::ATTR_SENSED |
                                    object::ATTR_HOT |
                                    object::ATTR_LABELED );


static vfx_geom s_VFX;
static s32 s_Glow, s_Ext1, s_Ext2;

//==============================================================================

static veh_physics s_GravCyclePhysics =
{
    TRUE,           // Is Ground vehicle

    83.0f,          // Maximum speed of vehicle
    100.0f,         // Linear Acceleration
    1.2f,           // Drag coefficent
    80.0f,          // Acceleration due to gravity
    
    R_1,            // Maximum pitch rotation from player
    R_40,           // Maximum roll rotation from player
    
    0.0f,           // Pitch acceleration
    R_250 *  2.0f,  // Yaw acceleration
    R_150 *  3.0f,  // Roll acceleration
    4.5f,           // Local rotational drag
    
    R_180 * 15.0f,  // World rotational acceleration
    4.5f ,          // World rotational drag
    
    500.0f,         // Mass
    0.30f,          // Perpendicular scale
    0.99f,          // Parallel scale
    0.5f,           // Damage scale


    1.0f /  7.0f,   // EnergyRecharge   
    1.0f /  7.5f,   // BoosterConsume
    1.0f,           // WeaponRecharge
    1.00f,          // WeaponConsume 
    1.0f,           // WeaponDelay
};

//==============================================================================
//  FUNCTIONS
//==============================================================================

void GravCycle_InitFX( void )
{
    // one-time initialization of bomber effect graphics
    s_VFX.CreateFromFile( "data/vehicles/gravcycle.vd" );

    // find my glowies
    s_Glow = s_VFX.FindGeomByID( "MAINGLOW" );
    s_Ext1 = s_VFX.FindGeomByID( "EXT1" );
    s_Ext2 = s_VFX.FindGeomByID( "EXT2" );
}


object* GravCycleCreator( void )
{
    return( (object*)(new gravcycle) );
}


//==============================================================================
//  MEMBER FUNCTIONS
//==============================================================================
void gravcycle::OnAdd( void )
{
    gnd_effect::OnAdd();

    // Set the vehicle type
    m_VehicleType = TYPE_WILDCAT;
    m_ColBoxYOff  = 0.65f;

    // create the hovering smoke effect
    m_HoverSmoke.Create( &tgl.ParticlePool,
                         &tgl.ParticleLibrary,
                         PARTICLE_TYPE_HOVER_SMOKE,
                         m_WorldPos, vector3(0,-1.0f,0),
                         NULL,
                         NULL );

    // disable it
    m_HoverSmoke.SetEnabled( FALSE );

    // create the hovering smoke effect
    m_DamageSmoke.Create( &tgl.ParticlePool,
                         &tgl.ParticleLibrary,
                         PARTICLE_TYPE_DAMAGE_SMOKE,
                         m_WorldPos, vector3(0,0,0),
                         NULL,
                         NULL );

    // disable it
    m_DamageSmoke.SetEnabled( FALSE );

    //m_Exhaust3
    // start audio
    m_AudioEng =        audio_Play( SFX_VEHICLE_WILDCAT_ENGINE_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );
    m_AudioAfterBurn = -1;

    // find the exhaust points
    // Loop through all model hot points
    model* Model = m_GeomInstance.GetModel() ;
    ASSERT(Model) ;
    s32 j = 0, i;

    for (i = 0 ; i < Model->GetHotPointCount() ; i++)
    {
        hot_point* HotPoint = Model->GetHotPointByIndex(i) ;
        ASSERT(HotPoint) ;

        if ( HotPoint->GetType() == HOT_POINT_TYPE_VEHICLE_EXHAUST1 )
            m_EP1[ j++ ] = HotPoint;

        if ( HotPoint->GetType() == HOT_POINT_TYPE_VEHICLE_EXHAUST2 )
            m_EP2 = HotPoint;
    }


    // create smoke effects
    /*for ( i = 0; i < 2; i++ )
        m_Exhaust1[i].Create( &tgl.ParticlePool, 
                       &tgl.ParticleLibrary, 
                       PARTICLE_TYPE_VEHICLE_EX1, 
                       m_WorldPos, 
                       vector3(0,0,1), 
                       NULL, 
                       NULL );*/

    m_Exhaust2.Create( &tgl.ParticlePool, 
                       &tgl.ParticleLibrary, 
                       PARTICLE_TYPE_VEHICLE_EX1, 
                       m_WorldPos, 
                       vector3(0,0,1), 
                       NULL, 
                       NULL );



    // set the other 2 sounds to low volume for now
    //audio_SetVolume     ( m_AudioEng, 0.5f );

    // Set the label
    x_wstrcpy( m_Label, StringMgr( "ui", IDS_WILDCAT ) );

    m_BubbleScale( 3.5f, 3.5f, 5.0f );

    // set the audio target volume and ramp rate  MARC TWEAK RAMPRATE TO YOUR LIKING
    m_CurVol = 0.5f;
    m_TargetVol = 0.5f;
    m_RampRate = 0.2f;

}

//==============================================================================

void gravcycle::Initialize( const vector3& Pos, radian InitHdg, u32 TeamBits )
{
    gnd_effect::InitPhysics( &s_GravCyclePhysics );
    
    // call the base class initializers with info specific to gravcycle
    gnd_effect::Initialize( SHAPE_TYPE_VEHICLE_WILDCAT,         // Geometry shape
                            SHAPE_TYPE_VEHICLE_WILDCAT_COLL,    // Collision shape
                            Pos,                                // position
                            InitHdg,                            // heading
                            2.0f,                               // Seat radius
                            TeamBits );
}

//==============================================================================

void gravcycle::SetupSeat( s32 Index, seat& Seat )
{
    (void)Index;
    ASSERT( Index == 0 );
    ASSERT( Seat.Type == SEAT_TYPE_PILOT );

    // Setup character types that can get in seat
    Seat.PlayerTypes[player_object::ARMOR_TYPE_LIGHT]  = TRUE ;
    Seat.PlayerTypes[player_object::ARMOR_TYPE_MEDIUM] = FALSE ;
    Seat.PlayerTypes[player_object::ARMOR_TYPE_HEAVY]  = FALSE ;

    Seat.CanLookAndShoot = FALSE ;
    Seat.PlayerAnimType  = ANIM_TYPE_CHARACTER_PILOT_GRAV_CYCLE ;
}

//==============================================================================

void gravcycle::OnAcceptInit( const bitstream& BitStream )
{
    // Make valid object
    Initialize(vector3(0,0,0), 0, 0) ;

    // Now call base call accept init to override and setup vars
    gnd_effect::OnAcceptInit( BitStream );
}

//==============================================================================

object::update_code gravcycle::Impact( vector3& Pos, vector3& Norm )
{
    (void)Pos;
    (void)Norm;

    return UPDATE;
}

//==============================================================================

void gravcycle::Render( void )
{
    // Begin with base render...
    gnd_effect::Render() ;

//    if (m_nSeatsFree == 0)
//        x_printfxy(2, 10, "Health = %.2f", m_Health );

}

//==============================================================================

void gravcycle::RenderParticles( void )
{
    if ( m_IsVisible )
    {
        // render particles from base class
        gnd_effect::RenderParticles();

        // render our own particles
        //if( m_Exhaust2.IsCreated() )
        //{
            //m_Exhaust2.SetPosition( m_GeomInstance.GetHotPointWorldPos( m_EP2 ); );
            //m_Exhaust2.Render();
        //}

        // render glowies
        if ( m_Control.ThrustY >= 0.0f )
        {

            // render glowies
            matrix4 L2W = m_GeomInstance.GetL2W();
            s_VFX.RenderGlowyPtSpr( L2W, s_Glow, m_Control.ThrustY * x_frand(0.5f, 0.75f));

            // render horizontal extrusions
            s_VFX.RenderExtrusion(   L2W, 
                                     s_Ext1,
                                     s_Ext2, 
                                     1.5f * m_Control.ThrustY + 0.25f , 
                                     0.3f * m_Control.ThrustY, 
                                     0.0f      );
        }
    }
}

//==============================================================================

object::update_code gravcycle::OnAdvanceLogic  ( f32 DeltaTime )
{
    matrix4 EP1;

    // set the axis for exhaust
    vector3 Axis;

    // call base class first, remember return param
    object::update_code RetVal = gnd_effect::OnAdvanceLogic( DeltaTime );

    EP1 = m_GeomInstance.GetHotPointL2W( HOT_POINT_TYPE_VEHICLE_EXHAUST2 );
    Axis.Set( 0.0f, 0.0f, -1.0f );
    EP1.SetTranslation( vector3(0.0f, 0.0f, 0.0f) );
    Axis = EP1 * Axis;

    Axis.RotateX( x_frand(-R_5, R_5) );
    Axis.RotateY( x_frand(-R_5, R_5) );

    // update particle effect    
    if ( m_Exhaust2.IsCreated() )
    {
        m_Exhaust2.GetEmitter(0).SetAxis( Axis );
        m_Exhaust2.GetEmitter(0).UseAxis( TRUE );
        //m_Exhaust2.SetPosition( m_GeomInstance.GetHotPointWorldPos( m_EP2 ) ) ;
        m_Exhaust2.SetVelocity( vector3(0.0f, 0.0f, 0.0f) );
        m_Exhaust2.UpdateEffect( DeltaTime );
    }
    
    // update audio
    m_TargetVol = m_Control.ThrustY * 0.20f + 0.80f;
    f32 RampDelta = m_RampRate * DeltaTime;

    if ( m_TargetVol < m_RampRate )
    {
        m_CurVol = MIN( m_CurVol + RampDelta, m_TargetVol );
    }
    else
    {
        m_CurVol = MAX( m_CurVol - RampDelta, m_TargetVol );
    }

    audio_SetPosition( m_AudioEng, &m_WorldPos );
    audio_SetVolume  ( m_AudioEng, m_CurVol );
    audio_SetPitch   ( m_AudioEng, MIN( 1.5f, (m_Speed.Length() / m_pPhysics->MaxForwardSpeed) * 0.08f + 1.0f ) );

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


    return RetVal;
    
}

void gravcycle::OnRemove( void )
{
    audio_Stop(m_AudioEng);
    audio_Stop(m_AudioAfterBurn); 
    gnd_effect::OnRemove();
}

void gravcycle::ApplyMove ( player_object* pPlayer, s32 SeatIndex, player_object::move& Move )
{
    // Call base class to position the turret etc
    gnd_effect::ApplyMove( pPlayer, SeatIndex, Move );

    // play audio for jetting
    if ( ( Move.JetKey ) && ( m_AudioAfterBurn == -1 ) && ( m_Energy > 10.0f) )
        m_AudioAfterBurn =  audio_Play( SFX_VEHICLE_WILDCAT_AFTERBURNER_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );

}

//==============================================================================

f32 gravcycle::GetPainRadius( void ) const
{
    return( 2.0f );
}

//==============================================================================
