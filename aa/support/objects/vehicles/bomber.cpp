//==============================================================================
//
//  Bomber.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================
#include "../Demo1/Globals.hpp"
#include "Entropy.hpp"
#include "NetLib/BitStream.hpp"
#include "ObjectMgr/Object.hpp"
#include "Bomber.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Audiomgr/audio.hpp"
#include "Objects/Projectiles/bombershot.hpp"
#include "Objects/Projectiles/Bomb.hpp"
#include "Objects/Projectiles/Aimer.hpp"
#include "contrail.hpp"
#include "vfx.hpp"
#include "StringMgr\StringMgr.hpp"
#include "Data\UI\ui_strings.h"

//==============================================================================
//  STATIC
//==============================================================================

//static s32      s_TargetSpot = -1;

//==============================================================================
//  DEFINES
//==============================================================================

//#define ADDITIVE_ANIM_ID_PITCH  0
//#define ADDITIVE_ANIM_ID_YAW    1        

//#define FIRE_GUN_DELAY          0.25f    // Time between firing turret (secs)
//#define SHOT_ENERGY             0.15f

//#define SIM_TIME_INC            0.5f    // elapsed time per iteration
//#define BOMB_GRAVITY            25.0f   // gravity constant used for the arcing bomb
//#define TARG_RAD                5.0f    // target radius

#define DROP_BOMB_DELAY         0.5f    // Time between dropping bombs (secs)
#define BOMB_ENERGY             0.35f

#define CONTRAIL_VEL            15.0f
#define CONTRAIL_VEL_SQ         ( CONTRAIL_VEL * CONTRAIL_VEL )

//#define MIN_BOMB_TARGET_DIST    ( 40.0f * 40.0f )
//#define TARGET_START_FADE_DIST  ( 60.0f * 60.0f )

//==============================================================================

static veh_physics s_BomberPhysics =
{
    FALSE,          // Is ground vehicle

    62.0f,          // Maximum speed of vehicle
    75.0f,          // Linear Acceleration
    1.2f,           // Drag coefficent
    7.5f,           // Acceleration due to gravity

    R_45,           // Maximum pitch rotation from player
    R_30,           // Maximum roll rotation from player
    
    R_230,          // Pitch acceleration
    R_230,          // Yaw acceleration
    R_230,          // Roll acceleration
    6.0f,           // Local rotational drag
    
    0.0f,           // World rotational acceleration
    0.0f,           // World rotational drag

    10000.0f,       // Mass
    0.15f,          // Perpendicular scale
    0.99f,          // Parallel scale
    0.3f,           // Damage scale

    1.0f / 16.0f,   // EnergyRecharge   
    1.0f /  6.0f,   // BoosterConsume
    1.0f /  4.0f,   // WeaponRecharge  ????????????????????????????
    0.35f,          // WeaponConsume 
    0.5f,           // WeaponDelay
};

//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   BomberCreator;

obj_type_info   BomberClassInfo( object::TYPE_VEHICLE_THUNDERSWORD, 
                                    "Bomber", 
                                    BomberCreator, 
                                    object::ATTR_REPAIRABLE       | 
                                    object::ATTR_ELFABLE          |
                                    object::ATTR_SOLID_DYNAMIC |
                                    object::ATTR_VEHICLE |
                                    object::ATTR_DAMAGEABLE |
                                    object::ATTR_SENSED |
                                    object::ATTR_HOT |
                                    object::ATTR_LABELED );

static vector3 s_BomberBombingViewOffset                (0.0f, -1.5f, -1.4f) ;

static vector3 s_BomberTurretBaseOffset                 (0.0f, -0.75f, -1.4f) ;

static vector3 s_BomberTurret1stPersonViewPivotOffset   (0.0f, 0.0f,  0.0f) ;
static vector3 s_BomberTurret1stPersonViewPivotDist     (0.0f, -0.08f, -0.1f) ;

static vector3 s_BomberTurret3rdPersonViewPivotOffset   (0.0f, 0.0f,  0.0f) ;
static vector3 s_BomberTurret3rdPersonViewPivotDist     (0.0f, -0.2f, -8.0f) ;

//static vector3 BomberPilotEyeOffset_3rd( 0.0f, 6.0f, -25.0f );
//static radian3 BomberPilotEyeAngle_3rd( 0, 0, 0);
static vector3 BomberPilotEyeOffset_3rd( 0, 9, -18 );
static radian3 BomberPilotEyeAngle_3rd( 0.5f, 0, 0);


static vfx_geom s_VFX;
static s32 s_GlowL1, s_GlowL2, s_GlowL3, s_GlowL4, s_GlowL5, s_GlowL6 ;
static s32 s_GlowR1, s_GlowR2, s_GlowR3, s_GlowR4, s_GlowR5, s_GlowR6 ;
static s32 s_LeftExt1, s_LeftExt2;
static s32 s_RightExt1, s_RightExt2;
static s32 s_CenExt1, s_CenExt2;
static s32 s_BotLeftExt1, s_BotLeftExt2;
static s32 s_BotRightExt1, s_BotRightExt2;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void Bomber_InitFX( void )
{
    // one-time initialization of bomber effect graphics
    s_VFX.CreateFromFile( "data/vehicles/bomber.vd" );

    // find my glowies
    s_GlowL1 = s_VFX.FindGeomByID( "LEFTGLOW1" );
    s_GlowL2 = s_VFX.FindGeomByID( "LEFTGLOW2" );
    s_GlowL3 = s_VFX.FindGeomByID( "LEFTGLOW3" );
    s_GlowL4 = s_VFX.FindGeomByID( "LEFTGLOW4" );
    s_GlowL5 = s_VFX.FindGeomByID( "LEFTGLOW5" );
    s_GlowL6 = s_VFX.FindGeomByID( "LEFTGLOW6" );
    s_GlowR1 = s_VFX.FindGeomByID( "RIGHTGLOW1" );
    s_GlowR2 = s_VFX.FindGeomByID( "RIGHTGLOW2" );
    s_GlowR3 = s_VFX.FindGeomByID( "RIGHTGLOW3" );
    s_GlowR4 = s_VFX.FindGeomByID( "RIGHTGLOW4" );
    s_GlowR5 = s_VFX.FindGeomByID( "RIGHTGLOW5" );
    s_GlowR6 = s_VFX.FindGeomByID( "RIGHTGLOW6" );

    s_LeftExt1 = s_VFX.FindGeomByID( "LEFTEXT1" );
    s_LeftExt2 = s_VFX.FindGeomByID( "LEFTEXT2" );
    s_RightExt1 = s_VFX.FindGeomByID( "RIGHTEXT1" );
    s_RightExt2 = s_VFX.FindGeomByID( "RIGHTEXT2" );

    s_CenExt1 = s_VFX.FindGeomByID( "CENEXT1" );
    s_CenExt2 = s_VFX.FindGeomByID( "CENEXT2" );

    s_BotLeftExt1 = s_VFX.FindGeomByID( "BOTLEFTEXT1" );
    s_BotLeftExt2 = s_VFX.FindGeomByID( "BOTLEFTEXT2" );
    s_BotRightExt1 = s_VFX.FindGeomByID( "BOTRIGHTEXT1" );
    s_BotRightExt2 = s_VFX.FindGeomByID( "BOTRIGHTEXT2" );
}


object* BomberCreator( void )
{
    return( (object*)(new bomber) );
}


//==============================================================================
//  MEMBER FUNCTIONS
//==============================================================================

void bomber::OnAdd( void )
{
    s32 i, j;

    gnd_effect::OnAdd();

    // Set the vehicle type
    m_VehicleType = TYPE_THUNDERSWORD;
    m_ColBoxYOff  = 1.5f;

    // create the hovering smoke effect
    m_HoverSmoke.Create( &tgl.ParticlePool,
                         &tgl.ParticleLibrary,
                         PARTICLE_TYPE_HOVER_SMOKE2,
                         m_WorldPos, vector3(0,-1.0f,0),
                         NULL,
                         NULL );

    // disable it
    m_HoverSmoke.SetEnabled( FALSE );

    // create the hovering smoke effect
    m_DamageSmoke.Create( &tgl.ParticlePool,
                         &tgl.ParticleLibrary,
                         PARTICLE_TYPE_DAMAGE_SMOKE2,
                         m_WorldPos, vector3(0,0,0),
                         NULL,
                         NULL );

    // disable it
    m_DamageSmoke.SetEnabled( FALSE );

    // find the wingtips
    j = 0;
    m_LeftHP = NULL;
    m_RightHP = NULL;

    // find the hotpoints for the contrails
    for ( i = 0 ; i < m_GeomInstance.GetModel()->GetHotPointCount() ; i++)
    {
        hot_point* HotPoint = m_GeomInstance.GetModel()->GetHotPointByIndex(i) ;
        ASSERT(HotPoint) ;

        // Found the wing tips?
        if ( HotPoint->GetType() == HOT_POINT_TYPE_VEHICLE_EXHAUST3 )
        {
            if ( j == 0 )
                m_LeftHP = HotPoint;
            else
            {
                m_RightHP = HotPoint;
                break;
            }

            j++;
        }
    }

    ASSERT(m_LeftHP);
    ASSERT(m_RightHP);

    j = 0;

    // find the hotpoints for the primary exhaust

    // ...

    // start audio
    m_AudioEng =        audio_Play( SFX_VEHICLE_THUNDERSWORD_ENGINE_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );
    //m_AudioAfterBurn =  audio_Play( SFX_VEHICLE_THUNDERSWORD_AFTERBURNER_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );
    m_AudioAfterBurn = -1;

    // set the other 2 sounds to low volume for now
    //audio_SetVolume     ( m_AudioEng, 0.5f );

    // Set the label
    x_wstrcpy( m_Label, StringMgr( "ui", IDS_THUNDERSWORD ) );

    m_BubbleScale( 10, 10, 15 );

    // set the audio target volume and ramp rate  MARC TWEAK RAMPRATE TO YOUR LIKING
    m_CurVol = 0.5f;
    m_TargetVol = 0.5f;
    m_RampRate = 0.2f;
}

//==============================================================================

void bomber::Initialize( const vector3& Pos, radian InitHdg, u32 TeamBits )
{
    gnd_effect::InitPhysics( &s_BomberPhysics );

    // call the base class initializers with info specific to bomber
    gnd_effect::Initialize( SHAPE_TYPE_VEHICLE_THUNDERSWORD,        // Geometry shape
                            SHAPE_TYPE_VEHICLE_THUNDERSWORD_COLL,   // Collision shape
                            Pos,                                    // position
                            InitHdg,                                // heading
                            2.0f, TeamBits );                          // Seat radius
/*
    // Setup turret base
    m_TurretBaseInstance.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_BELLY_BASE ) ) ;
    m_TurretBaseInstance.SetAnimByType(ANIM_TYPE_TURRET_IDLE) ;        

    // Make the geometry use frame 0 reflection map
    m_TurretBaseInstance.SetTextureAnimFPS(0) ;
    m_TurretBaseInstance.SetTextureFrame(0) ;

    // Add pitch anim
    anim_state* PitchAnimState = m_TurretBaseInstance.AddAdditiveAnim(ADDITIVE_ANIM_ID_PITCH) ;
    ASSERT(PitchAnimState) ;
    PitchAnimState->SetSpeed(0) ;
    m_TurretBaseInstance.SetAdditiveAnimByType(ADDITIVE_ANIM_ID_PITCH, ANIM_TYPE_TURRET_PITCH) ;

    // Add yaw anim
    anim_state* YawAnimState = m_TurretBaseInstance.AddAdditiveAnim(ADDITIVE_ANIM_ID_YAW) ;
    ASSERT(YawAnimState) ;
    YawAnimState->SetSpeed(0) ;
    m_TurretBaseInstance.SetAdditiveAnimByType(ADDITIVE_ANIM_ID_YAW, ANIM_TYPE_TURRET_YAW) ;

    // Setup turret barrels
    //m_TurretBarrelInstance[0].SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_BELLY_BARREL_LEFT ) ) ;
    m_TurretBarrelInstance[0].SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_BELLY_BARREL_RIGHT ) ) ;
    m_TurretBarrelInstance[1].SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_BELLY_BARREL_RIGHT ) ) ;

    // Lookup turret barrel position hot points
    m_TurretBarrelHotPoint[0] = NULL ;
    m_TurretBarrelHotPoint[1] = NULL ;
    model*  BaseModel = m_TurretBaseInstance.GetModel() ;
    ASSERT(BaseModel) ;
    for (i = 0 ; i < BaseModel->GetHotPointCount() ; i++)
    {
        hot_point* HotPoint = BaseModel->GetHotPointByIndex(i) ;
        ASSERT(HotPoint) ;

        if (HotPoint->GetType() == HOT_POINT_TYPE_TURRET_BARREL_MOUNT)
            m_TurretBarrelHotPoint[i] = HotPoint ;
    }
    ASSERT(m_TurretBarrelHotPoint[0]) ;
    ASSERT(m_TurretBarrelHotPoint[0]) ;

    // Setup barrel anims
    for (i = 0 ; i < 2 ; i++)
    {
        // Get barrel
        shape_instance& Barrel = m_TurretBarrelInstance[i] ;

        // Put barrel to idle
        Barrel.SetAnimByType(ANIM_TYPE_TURRET_IDLE) ;
    }

    // Default weapon
    m_BomberWeaponType = TURRET ;            // Current weapon
    m_TurretBarrelFireSide = 0 ;

    // Setup turret vars
    m_bHasTurret        = TRUE ;
    m_TurretSpeedScaler = 0.5f ;            // Controls input speed
    m_TurretMinPitch    = 0 ;               // Min pitch of turret (looking up)
    m_TurretMaxPitch    = R_89 ;            // Max pitch of turret (looking down)  (keep at 89 for quaternion::GetRotation() to work!)
*/
    m_bHasTurret        = FALSE ;
}

//==============================================================================

void bomber::SetupSeat( s32 Index, seat& Seat )
{
    (void)Index;
    ASSERT( Index == 0 );
    ASSERT( Seat.Type == SEAT_TYPE_PILOT );

    Seat.PlayerTypes[player_object::ARMOR_TYPE_LIGHT]  = TRUE ;
    Seat.PlayerTypes[player_object::ARMOR_TYPE_MEDIUM] = TRUE ;
    Seat.PlayerTypes[player_object::ARMOR_TYPE_HEAVY]  = FALSE ;

    Seat.CanLookAndShoot = FALSE ;
    Seat.PlayerAnimType  = ANIM_TYPE_CHARACTER_SITTING ;
}

//==============================================================================

void bomber::OnAcceptInit( const bitstream& BitStream )
{
    // Make valid object
    Initialize(vector3(0,0,0), 0, 0 );

    // Now call base call accept init to override and setup vars
    gnd_effect::OnAcceptInit( BitStream );
}

//==============================================================================

object::update_code bomber::Impact( vector3& Pos, vector3& Norm )
{
    (void)Pos;
    (void)Norm;

    return UPDATE;
}

//==============================================================================
/*
void bomber::UpdateInstances( void )
{
    s32     i ;

    // Call base class
    vehicle::UpdateInstances() ;

    // Get turret rot
    radian3 TurretRot = GetDrawTurretRot() ;

    // Calc turret rot
    quaternion VehicleRot      = quaternion(m_GeomInstance.GetRot()) ;
    quaternion TurretBarrelRot = quaternion(radian3(TurretRot.Pitch, TurretRot.Yaw, 0)) ;

    // Get parametric pitch and yaw for additive anims
    f32 Pitch = (TurretRot.Pitch - m_TurretMinPitch) / (m_TurretMaxPitch - m_TurretMinPitch) ;
    f32 Yaw   = ( x_ModAngle2(R_180 + TurretRot.Yaw) + R_180) / R_360 ;

    // Update yaw anim
    anim_state* YawAnimState = m_TurretBaseInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_YAW) ;
    ASSERT(YawAnimState) ;
    YawAnimState->SetFrameParametric(Yaw) ;

    // Update pitch anim
    anim_state* PitchAnimState = m_TurretBaseInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_PITCH) ;
    ASSERT(PitchAnimState) ;
    PitchAnimState->SetFrameParametric(Pitch) ;

    // Dirty the turret base instance so additive anims get in sync 
    m_TurretBaseInstance.Advance(0) ;

    // Update turret base
    m_TurretBaseInstance.SetRot(m_GeomInstance.GetRot()) ;
    vector3 Offset = s_BomberTurretBaseOffset ;
    Offset.Rotate(m_GeomInstance.GetRot()) ;
    m_TurretBaseInstance.SetPos(Offset + m_GeomInstance.GetPos()) ;

    // Update barrels
    for (i = 0 ; i < 2 ; i++)
    {
        m_TurretBarrelInstance[i].SetPos(m_TurretBaseInstance.GetHotPointWorldPos(m_TurretBarrelHotPoint[i])) ;
        m_TurretBarrelInstance[i].SetRot((VehicleRot * TurretBarrelRot).GetRotation()) ;
    }
}
*/

//==============================================================================

void bomber::Render( void )
{
    // Begin with base render...
    gnd_effect::Render() ;

    if (!m_IsVisible)
        return ;

/*
    draw_Marker( GetDrawPos(),       XCOLOR_GREEN );
    draw_Marker( GetBlendFirePos(0), XCOLOR_WHITE );
*/

    // render the contrails with the current position of the wing
    xbool Connect = ( m_Vel.LengthSquared() > CONTRAIL_VEL_SQ );

    m_LTrail.Render( m_GeomInstance.GetHotPointWorldPos( m_LeftHP ), Connect  );
    m_RTrail.Render( m_GeomInstance.GetHotPointWorldPos( m_RightHP ), Connect );

/*
    // Default to drawing turret base
    xbool   DrawTurretBase = TRUE ;

    // Don't render the base in 1st person!
    player_object* pPlayer = GetSeatPlayer(1) ;     // Get bomber player
    if ((pPlayer) && (pPlayer->GetProcessInput()))  // Drawing on players machine?
    {
        // Don't draw turret base if drawing in 1st person
        if (pPlayer->GetViewType() == player_object::VIEW_TYPE_1ST_PERSON)
            DrawTurretBase = FALSE ;
    }

    // Draw turret base
    if (DrawTurretBase)
    {
        m_TurretBaseInstance.SetFogColor( m_GeomInstance.GetFogColor() ) ;
        m_TurretBaseInstance.SetColor( m_GeomInstance.GetColor() ) ;
        m_TurretBaseInstance.Draw( m_DrawFlags, NULL, -1, m_RenderMaterialFunc, (void*)this );
    }

    // Draw barrel
    for (i = 0 ; i < 2 ; i++)
    {
        m_TurretBarrelInstance[i].SetFogColor( m_GeomInstance.GetFogColor() ) ;
        m_TurretBarrelInstance[i].SetColor( m_GeomInstance.GetColor() ) ;
        m_TurretBarrelInstance[i].Draw( m_DrawFlags, NULL, -1, m_RenderMaterialFunc, (void*)this );
    }
*/
}

//==============================================================================
/*
void bomber::RenderBombTarget( void )
{
    vector3 CalcPos;
    player_object *pPlayer;

    pPlayer = GetSeatPlayer(1);

    if ( ( pPlayer ) && ( m_BomberWeaponType == BOMB ) )
    {
        // only render this for the guy in the bomber seat
        if ( tgl.PC[tgl.WC].PlayerIndex == pPlayer->GetObjectID().Slot )
        {
            CalcTargetPos( CalcPos );

            // check the distance
            f32 LenSq = (m_WorldPos - CalcPos).LengthSquared();

            if ( LenSq > MIN_BOMB_TARGET_DIST )
            {
                xcolor Color = XCOLOR_WHITE;

                f32 AlphaVal = ( TARGET_START_FADE_DIST - LenSq ) / ( TARGET_START_FADE_DIST - MIN_BOMB_TARGET_DIST );

                if ( AlphaVal > 0.0f )
                    Color.A = (u8)(255 * (1.0f - AlphaVal));
                else
                    Color.A = 255;

                // find the texture if we haven't already
                if ( s_TargetSpot == -1 )
                {
                    s_TargetSpot = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("bullseye");
                }

                draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
            #ifdef TARGET_PS2
                gsreg_Begin();
                gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
                gsreg_SetZBufferUpdate(FALSE);
                gsreg_SetZBufferTest( ZBUFFER_TEST_ALWAYS );
                gsreg_SetClamping( FALSE );
                gsreg_SetMipEquation( TRUE, 
                                      0, 
                                      0, 
                                      MIP_MAG_BILINEAR, 
                                      MIP_MIN_INTERP_MIPMAP_BILINEAR) ;
                gsreg_End();
            #endif

                // set the texture
                draw_SetTexture ( tgl.GameShapeManager.GetTextureManager().GetTexture(s_TargetSpot).GetXBitmap() );
    
                // draw the quad
                draw_Color( Color );
                draw_UV( 0,0 );
                draw_Vertex( CalcPos.X - TARG_RAD, CalcPos.Y, CalcPos.Z - TARG_RAD );
                draw_UV( 1,0 );
                draw_Vertex( CalcPos.X + TARG_RAD, CalcPos.Y, CalcPos.Z - TARG_RAD );
                draw_UV( 1,1 );
                draw_Vertex( CalcPos.X + TARG_RAD, CalcPos.Y, CalcPos.Z + TARG_RAD );

                draw_UV( 1,1 );
                draw_Vertex( CalcPos.X + TARG_RAD, CalcPos.Y, CalcPos.Z + TARG_RAD );
                draw_UV( 0,1 );
                draw_Vertex( CalcPos.X - TARG_RAD, CalcPos.Y, CalcPos.Z + TARG_RAD );
                draw_UV( 0,0 );
                draw_Vertex( CalcPos.X - TARG_RAD, CalcPos.Y, CalcPos.Z - TARG_RAD );
                draw_End();

    #ifdef TARGET_PS2
                gsreg_Begin();
                gsreg_SetZBufferUpdate(TRUE);
                gsreg_SetZBufferTest( ZBUFFER_TEST_GEQUAL );
                gsreg_End();
    #endif
            }
        }
    }
}
*/
//==============================================================================

void bomber::RenderParticles( void )
{
    if ( m_IsVisible )
    {

        // render particles for the base class
        gnd_effect::RenderParticles();

        /*
        // Left Side
        for ( i = 0; i < EXHAUST_PTS_PER_SIDE; i++ )
        {
            Pos = m_GeomInstance.GetHotPointWorldPos( m_LThrust[i] );
            m_Exhaust1[i].SetPosition( Pos );
            m_Exhaust1[i].Render();
        }

        // Right Side
        for ( i = 0; i < EXHAUST_PTS_PER_SIDE; i++ )
        {
            Pos = m_GeomInstance.GetHotPointWorldPos( m_RThrust[i] ) ;
            m_Exhaust1[i + EXHAUST_PTS_PER_SIDE].SetPosition( Pos );
            m_Exhaust1[i + EXHAUST_PTS_PER_SIDE].Render();
        }*/

        // render thrust effects
        if ( m_Control.ThrustY >= 0.0f )
        {

            // render glowies
            matrix4 L2W = m_GeomInstance.GetL2W();
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL1, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL2, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL3, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL4, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL5, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL6, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR1, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR2, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f) );
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR3, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f) );
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR4, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f) );
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR5, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f) );
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR6, (m_Control.ThrustY * 0.5f) * x_frand(0.5f, 0.75f) );

            f32 Mul = (m_Control.Boost > 0.0f) ? 2.0f : 1.0f;

            // render horizontal extrusions
            s_VFX.RenderExtrusion(   L2W, 
                                     s_LeftExt1,
                                     s_LeftExt2, 
                                     (0.5f * m_Control.ThrustY + 0.25f) * Mul, 
                                     0.1f * m_Control.ThrustY, 
                                     0.0f      );

            s_VFX.RenderExtrusion(   L2W, 
                                     s_RightExt1,
                                     s_RightExt2, 
                                     0.5f * m_Control.ThrustY + 0.25f , 
                                     0.1f * m_Control.ThrustY, 
                                     0.0f      );

            s_VFX.RenderExtrusion(   L2W, 
                                     s_CenExt1,
                                     s_CenExt2, 
                                     0.5f * m_Control.ThrustY + 0.25f , 
                                     0.1f * m_Control.ThrustY, 
                                     0.0f      );

            if ( m_Control.Boost > 0.0f )
            {
                // render vertical extrusions
                s_VFX.RenderExtrusion(   L2W, 
                                         s_BotLeftExt1,
                                         s_BotLeftExt2, 
                                         1.5f * (1.0f - m_Control.ThrustY) + 0.25f , 
                                         0.1f * (1.0f - m_Control.ThrustY), 
                                         0.0f      );

                s_VFX.RenderExtrusion(   L2W, 
                                         s_BotRightExt1,
                                         s_BotRightExt2, 
                                         1.5f * (1.0f - m_Control.ThrustY) + 0.25f , 
                                         0.1f * (1.0f - m_Control.ThrustY), 
                                         0.0f      );
            }
        }
    }
}

//==============================================================================

object::update_code bomber::OnAdvanceLogic  ( f32 DeltaTime )
{
    matrix4 EP1;

    // update the contrails
    m_LTrail.Update( DeltaTime );
    m_RTrail.Update( DeltaTime );

    if ( m_Vel.LengthSquared() > CONTRAIL_VEL_SQ )
    {
        f32 Alpha = m_Vel.LengthSquared() / ( m_pPhysics->MaxForwardSpeed * m_pPhysics->MaxForwardSpeed );
        m_LTrail.AddNode( m_GeomInstance.GetHotPointWorldPos( m_LeftHP), Alpha );
        m_RTrail.AddNode( m_GeomInstance.GetHotPointWorldPos( m_RightHP), Alpha );
    }

    // set the axis for exhaust
    vector3 Axis;

    // call base class first, remember return param
    object::update_code RetVal = gnd_effect::OnAdvanceLogic( DeltaTime );

    EP1 = m_GeomInstance.GetHotPointL2W( HOT_POINT_TYPE_VEHICLE_EXHAUST1 );
    
    // turn the hotpoints around if turboing
    if ( m_Control.Boost > 0.0f )
        Axis.Set( 0.0f, -1.0f, 0.0f );
    else
        Axis.Set( 0.0f, 1.0f, 0.0f );

    EP1.SetTranslation( vector3(0.0f, 0.0f, 0.0f) );
    Axis = EP1 * Axis;

    Axis.RotateX( x_frand(-R_5, R_5) );
    Axis.RotateY( x_frand(-R_5, R_5) );

    // update particle effect
    //for ( s32 i = 0; i < NUM_EXHAUST_PTS; i++ )
    //{
        //m_Exhaust1[i].GetEmitter(0).SetAxis( Axis );
        //m_Exhaust1[i].GetEmitter(0).UseAxis( TRUE );
        //m_Exhaust1[i].SetVelocity( vector3(0.0f, 0.0f, 0.0f) );
        //m_Exhaust1[i].Update( DeltaTime );
    //}
    
    // update audio
    m_TargetVol = m_Control.ThrustY * 0.25f + 0.75f;
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
    audio_SetPitch   ( m_AudioEng, (m_Speed.Length() / m_pPhysics->MaxForwardSpeed) * 0.08f + 1.0f );
    
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
/*
    // Update turret animations
    if (m_State == STATE_MOVING)
    {
        m_TurretBaseInstance.Advance(DeltaTime) ;
        m_TurretBarrelInstance[0].Advance(DeltaTime) ;
        m_TurretBarrelInstance[1].Advance(DeltaTime) ;
    }

    // Activate/deactivate turret when someone is in the bomber seat
    if ( GetSeatPlayer(1) )
        m_TurretBaseInstance.SetAnimByType(ANIM_TYPE_TURRET_ACTIVATE, 0.25f) ;  // Blend fast
    else
        m_TurretBaseInstance.SetAnimByType(ANIM_TYPE_TURRET_IDLE, 2.0f) ;       // Blend slowly

    // Return the turret back to normal if no player is in it
    if (GetSeatPlayer(1) == NULL)
        ReturnTurretToRest(DeltaTime) ;
*/
    return RetVal;
}

//==============================================================================

void bomber::OnRemove( void )
{
    audio_Stop(m_AudioEng);
    audio_Stop(m_AudioAfterBurn); 
    gnd_effect::OnRemove();
}       

//==============================================================================

void bomber::Get3rdPersonView( s32 SeatIndex, vector3& Pos, radian3& Rot )
{
    (void)SeatIndex;
//  vector3 PivotPos, PivotDist ;

    // Special case yaw
    radian3 FreeLookYaw = Rot ;

    // Make sure shape instances are in sync with actual object
    UpdateInstances() ;

    // Different seats have different views
/*
    const seat& Seat = GetSeat(SeatIndex) ;
    switch(Seat.Type)
    {
        case SEAT_TYPE_BOMBER:
            ASSERT(0) ;
            break ;

        default:
            // Special 3rd person view
            ASSERT( m_GeomInstance.GetHotPointByType(HOT_POINT_TYPE_VEHICLE_EYE_POS) ) ;
            Pos = BomberPilotEyeOffset_3rd;
            Pos.Rotate( m_Rot );
            Pos.Rotate( FreeLookYaw );
            Pos += m_GeomInstance.GetHotPointWorldPos(HOT_POINT_TYPE_VEHICLE_EYE_POS);
            Rot += BomberPilotEyeAngle_3rd + m_Rot ;

            break ;
    }
*/

    ASSERT( m_GeomInstance.GetHotPointByType(HOT_POINT_TYPE_VEHICLE_EYE_POS) ) ;
    Pos = BomberPilotEyeOffset_3rd;
    Pos.Rotate( m_Rot );
    Pos.Rotate( FreeLookYaw );
    Pos += m_GeomInstance.GetHotPointWorldPos(HOT_POINT_TYPE_VEHICLE_EYE_POS);
    Rot += BomberPilotEyeAngle_3rd + m_Rot ;
}

//==============================================================================
/*
void bomber::Get1stPersonView( s32 SeatIndex, vector3& Pos, radian3& Rot )
{
//    vector3 PivotPos, PivotDist;

    // Make sure shape instances are in sync with actual object
    UpdateInstances() ;

    // Different seats have different views

    const seat& Seat = GetSeat(SeatIndex) ;
    switch(Seat.Type)
    {
        case SEAT_TYPE_BOMBER:

            // Setup pivot pos
            PivotPos = s_BomberTurret1stPersonViewPivotOffset ;
            PivotPos.Rotate(m_TurretBaseInstance.GetRot()) ;
            PivotPos += (m_TurretBarrelInstance[0].GetPos() + m_TurretBarrelInstance[1].GetPos()) * 0.5f ;

            // Setup pivot dist
            PivotDist = s_BomberTurret1stPersonViewPivotDist ;
            PivotDist.Rotate(m_TurretBarrelInstance[0].GetRot()) ;

            // Setup view rot and pos
            Rot = m_TurretBarrelInstance[0].GetRot() ;
            Pos = PivotPos + PivotDist ;
            break ;

        default:
            // Call base class
            vehicle::Get1stPersonView( SeatIndex, Pos, Rot) ;
            break ;
    }

    vehicle::Get1stPersonView( SeatIndex, Pos, Rot) ;
}
*/
//==============================================================================

vector3 bomber::GetBlendFirePos( s32 Index )
{
    (void)Index;
    ASSERT( Index == 0 );
    vector3 Pos;
    GetBombPoint( Pos );
    return( Pos );
}

//==============================================================================

void bomber::ApplyMove ( player_object* pPlayer, s32 SeatIndex, player_object::move& Move )
{
    // Call base class to position the turret etc
    gnd_effect::ApplyMove( pPlayer, SeatIndex, Move );

    // play audio for jetting
    if ( ( Move.JetKey ) && ( m_AudioAfterBurn == -1 ) && ( m_Energy > 10.0f) )
        m_AudioAfterBurn =  audio_Play( SFX_VEHICLE_THUNDERSWORD_AFTERBURNER_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );

    // Update weapons?
    if ((pPlayer) && (tgl.ServerPresent))
    {
        // Update fire delay
        m_Fire1Delay -= Move.DeltaTime ;
        if (m_Fire1Delay < 0)
            m_Fire1Delay = 0 ;
    
        /*
        // Toggle weapon?
        if ((Move.PrevWeaponKey) || (Move.NextWeaponKey))
        {
            // Toggle (go through function so that sfx on server are played)
            switch(m_BomberWeaponType)
            {
                case TURRET:    SetBomberWeaponType(BOMB) ;     break ;
                case BOMB:      SetBomberWeaponType(TURRET) ;   break ;
            }

            // Clear keys
            Move.PrevWeaponKey = FALSE ;
            Move.NextWeaponKey = FALSE ;
        }

        // Player can only fire bombs now                
        SetBomberWeaponType( BOMB );
        */

        // Only create on the server
        if ( (Move.FireKey) && (m_Fire1Delay == 0) && ( m_WeaponEnergy > m_pPhysics->WeaponConsume ) )
        {
            // Throw bomb!
            bomb* pBomb = (bomb*)ObjMgr.CreateObject( object::TYPE_BOMB );
            pBomb->Initialize( GetBlendFirePos(0), m_ObjectID, m_ObjectID, pPlayer->GetTeamBits() );
            pBomb->SetScoreID( pPlayer->GetObjectID() );
            ObjMgr.AddObject( pBomb, obj_mgr::AVAILABLE_SERVER_SLOT );

            // Wait before firing again
            m_Fire1Delay = m_pPhysics->WeaponDelay;

            // decrement weapon energy
            AddWeaponEnergy( -m_pPhysics->WeaponConsume );

            // Tell all clients to do fire anim
            //m_DirtyBits |= VEHICLE_DIRTY_BIT_FIRE ;
        }
    }
}

//==============================================================================

void bomber::WriteDirtyBitsData  ( bitstream& BitStream, u32& DirtyBits, f32 Priority, xbool MoveUpdate )
{
    // Fire?
    //if (DirtyBits & VEHICLE_DIRTY_BIT_FIRE)
    //{
        // This is all we have to do...
        //DirtyBits &= ~VEHICLE_DIRTY_BIT_FIRE ;
    //}

    // Call base class
    gnd_effect::WriteDirtyBitsData(BitStream, DirtyBits, Priority, MoveUpdate) ;
}

//==============================================================================

xbool bomber::ReadDirtyBitsData   ( const bitstream& BitStream, f32 SecInPast )
{
    // Call base class
    return gnd_effect::ReadDirtyBitsData(BitStream, SecInPast) ;
}

//==============================================================================
/*
void bomber::SetBomberWeaponType ( weapon_type Type )
{
    // Must be within range
    ASSERT(Type >= TURRET) ;
    ASSERT(Type <= LASER) ;

    // Switching?
    if (Type != m_BomberWeaponType)
    {
        // Controlled by a player?
        player_object* pPlayer = GetSeatPlayer(1) ;
        if (pPlayer)
        {
            // Lookup activate sfx for current barrel
            s32 ActivateSfx = 0 ;
            switch(Type)
            {
                case TURRET:
                    ActivateSfx = SFX_WEAPONS_BLASTER_ACTIVATE ;
                    break ;

                case BOMB:
                    ActivateSfx = SFX_WEAPONS_GRENADELAUNCHER_ACTIVATE ;
                    break ;
            }
       
            // Play full volume sfx?
            if (pPlayer->GetProcessInput())
                audio_Play(ActivateSfx) ;               // Full vol
            else
                audio_Play(ActivateSfx, &m_WorldPos) ;  // Normal vol
        }

        // Finally, set new weapon
        m_BomberWeaponType = Type ;
    }
}
*/
//==============================================================================
/*
s32 bomber::GetWeaponType( void ) const
{
    return m_BomberWeaponType;
}
*/
//==============================================================================
/*
void bomber::CalcTargetPos( vector3& Spot )
{
    // simulate dropping the bomb right this instant
    vector3 Offset = s_BomberBombingViewOffset ;
    vector3 Pos = Offset + GetDrawPos(); //m_GeomInstance.GetPos() ;
    object::id DummyObjID;

    CalculateArcingImpactPos(
        Pos
        , m_Speed
        , Spot
        , DummyObjID
        , this
        , m_ObjectID.GetAsS32()
        , BOMB_GRAVITY );
}
*/
//==============================================================================

f32 bomber::GetPainRadius( void ) const
{
    return( 5.0f );
}

//==============================================================================

void bomber::GetBombPoint( vector3& Pos )
{
    vector3 Offset = s_BomberBombingViewOffset ;
    Offset.Rotate(m_GeomInstance.GetRot()) ;
    Pos = Offset + GetDrawPos();
}

//==============================================================================

