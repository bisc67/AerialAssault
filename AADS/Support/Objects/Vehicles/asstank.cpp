//==============================================================================
//
//  AssTank.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================
#include "..\Demo1\Globals.hpp"
#include "Entropy.hpp"
#include "NetLib\BitStream.hpp"
#include "ObjectMgr\Object.hpp"
#include "AssTank.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Audiomgr/Audio.hpp"
#include "Objects/Projectiles/Mortar.hpp"
#include "Objects/Projectiles/bullet.hpp"
#include "Damage/Damage.hpp"
#include "vfx.hpp"



//==============================================================================
//  DEFINES
//==============================================================================

#define FIRE_MORTAR_DELAY       1.0f                // Time between firing mortar
#define FIRE_CHAIN_DELAY        (1.0f / 6.0f)       // Time between firing turret
#define FIRE_CHAIN_FX_DELAY     (1.0f / 10.0f)      // Time between muzzle flashes (matched sfx)

#define CHAINGUN_RAND_ANGLE     (R_1 * 1.1459f)     // Bullet spray angle
#define CHAINGUN_FLASH_ANGLE    (R_8 * 1.1459f)

#define BULLET_ENERGY           0.05f
#define MORTAR_ENERGY           0.50f


//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   AssTankCreator;

obj_type_info   AssTankClassInfo( object::TYPE_VEHICLE_BEOWULF, 
                                    "AssTank", 
                                    AssTankCreator, 
                                    object::ATTR_REPAIRABLE       | 
                                    object::ATTR_ELFABLE          |
                                    object::ATTR_SOLID_DYNAMIC |
                                    object::ATTR_VEHICLE |
                                    object::ATTR_DAMAGEABLE |
                                    object::ATTR_SENSED |
                                    object::ATTR_HOT |
                                    object::ATTR_LABELED );

static vector3 s_AssTankTurretBaseOffset                (0.0f, 1.87f, -1.6f) ;

static vector3 s_AssTankTurret1stPersonViewPivotOffset  (0.0f, 0.2f,  0.0f) ;
static vector3 s_AssTankTurret1stPersonViewPivotDist    (0.0f, 1.0f, -0.8f) ;

static vector3 s_AssTankTurret3rdPersonViewPivotOffset  (0.0f, 2.0f,  -3.0f) ;
static vector3 s_AssTankTurret3rdPersonViewPivotDist    (0.0f, 0.0f,  -3.0f) ;
 
static vector3 AssTankPilotEyeOffset_3rd( 0.0f, 5.0f, -22.0f );

static s32 s_FlashTexture = -1;

static s32          s_GlowL1, s_GlowL2, s_GlowR1, s_GlowR2;
static vfx_geom     s_VFX;

//==============================================================================

// IMPORTANT NOTE: You need to set these values!

static veh_physics s_AssTankPhysics;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void AssTank_InitFX( void )
{
    // one-time initialization of bomber effect graphics
    s_VFX.CreateFromFile( "data/vehicles/asstank.vd" );

    // find my glowies
    s_GlowL1 = s_VFX.FindGeomByID( "LEFTGLOW1" );
    s_GlowL2 = s_VFX.FindGeomByID( "LEFTGLOW2" );
    s_GlowR1 = s_VFX.FindGeomByID( "RIGHTGLOW1" );
    s_GlowR2 = s_VFX.FindGeomByID( "RIGHTGLOW2" );
}

object* AssTankCreator( void )
{
    return( (object*)(new asstank) );
}


//==============================================================================
//  MEMBER FUNCTIONS
//==============================================================================
void asstank::OnAdd( void )
{
    gnd_effect::OnAdd();

    // Set the vehicle type
    m_VehicleType = TYPE_BEOWULF;
    m_ColBoxYOff  = 2.0f;

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

    // start audio
    m_AudioEng       = audio_Play( SFX_VEHICLE_BEOWULF_ENGINE_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );
    m_AudioAfterBurn = -1 ;
    audio_InitLooping(m_AudioChainGunFire) ;

    // ChainGun fire flags
    m_ChainGunFxDelay  = 0 ;
    m_ChainGunHasFired = FALSE ;
    m_ChainGunFiring   = FALSE ;

    // Set the label
    {
        xwstring Name( "Beowulf" );
        x_wstrcpy( m_Label, (const xwchar*)Name );
    }

    m_BubbleScale( 12, 8, 15 );
}

//==============================================================================

void asstank::Initialize( const vector3& Pos, radian InitHdg, u32 TeamBits )
{
    gnd_effect::InitPhysics( &s_AssTankPhysics );
    
    // call the base class initializers with info specific to asstank
    gnd_effect::Initialize( SHAPE_TYPE_VEHICLE_BEOWULF,         // Geometry shape
                            SHAPE_TYPE_VEHICLE_BEOWULF_COLL,    // Collision shape
                            Pos,                                // position
                            InitHdg,                            // heading
                            2.0f,                               // Seat radius 
                            TeamBits );

    // Setup turret instances
    m_TurretBaseInstance.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_TANK_BASE ) ) ;
    m_TurretBarrelMortarInstance.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_TANK_BARREL_MORTAR ) ) ;
    m_TurretBarrelChainInstance.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_TANK_BARREL_CHAIN ) ) ;

    // Make the geometry use frame 0 reflection map
    m_TurretBaseInstance.SetTextureAnimFPS(0) ;
    m_TurretBaseInstance.SetTextureFrame(0) ;
    
    m_TurretBarrelMortarInstance.SetTextureAnimFPS(0) ;
    m_TurretBarrelMortarInstance.SetTextureFrame(0) ;

    m_TurretBarrelChainInstance.SetTextureAnimFPS(0) ;
    m_TurretBarrelChainInstance.SetTextureFrame(0) ;

    // Lookup turret barrel position hot points
    m_TurretBarrelMortarHotPoint = NULL ;
    m_TurretBarrelChainHotPoint  = NULL ;
    model*  BaseModel = m_TurretBaseInstance.GetModel() ;
    ASSERT(BaseModel) ;
    for (s32 i = 0 ; i < BaseModel->GetHotPointCount() ; i++)
    {
        hot_point* HotPoint = BaseModel->GetHotPointByIndex(i) ;
        ASSERT(HotPoint) ;

        if (HotPoint->GetType() == HOT_POINT_TYPE_TURRET_BARREL_MOUNT)
        {
            if (i == 1)
                m_TurretBarrelMortarHotPoint = HotPoint ;
            else
                m_TurretBarrelChainHotPoint = HotPoint ;
        }
    }
    ASSERT(m_TurretBarrelMortarHotPoint) ;
    ASSERT(m_TurretBarrelChainHotPoint) ;

    // Put barrels into idle anim
    m_TurretBarrelMortarInstance.SetAnimByType(ANIM_TYPE_TURRET_IDLE) ;
    m_TurretBarrelChainInstance.SetAnimByType(ANIM_TYPE_TURRET_IDLE) ;

    // Setup turret vars
    m_bHasTurret = TRUE ;
    m_TurretSpeedScaler = 0.5f ;            // Controls input speed
    m_TurretMinPitch    = -R_89 ;           // Min pitch of turret (looking up)
    m_TurretMaxPitch    = R_10 ;            // Max pitch of turret (looking down)

    m_TurretCurrentBarrel = 0 ;             // Current barrel firing out of

}

//==============================================================================

void asstank::SetupSeat( s32 Index, seat& Seat )
{
    (void)Index ;
    (void)Seat;
/*
    switch(Seat.Type)
    {
        case SEAT_TYPE_PILOT:

            Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_LIGHT]  = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_MEDIUM] = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_HEAVY]  = FALSE ;

            Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_LIGHT]    = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_MEDIUM]   = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_HEAVY]    = FALSE ;

            Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_LIGHT]     = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_MEDIUM]    = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_HEAVY]     = FALSE ;

            Seat.CanLookAndShoot = FALSE ;
            Seat.PlayerAnimType  = ANIM_TYPE_CHARACTER_SITTING ;
            break ;

        default:
        case SEAT_TYPE_GUNNER:
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_LIGHT]  = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_MEDIUM] = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_FEMALE][player_object::ARMOR_TYPE_HEAVY]  = FALSE ;

            Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_LIGHT]    = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_MEDIUM]   = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_MALE][player_object::ARMOR_TYPE_HEAVY]    = FALSE ;

            Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_LIGHT]     = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_MEDIUM]    = TRUE ;
            Seat.PlayerTypes[player_object::CHARACTER_TYPE_BIO][player_object::ARMOR_TYPE_HEAVY]     = FALSE ;

            Seat.CanLookAndShoot = FALSE ;
            Seat.PlayerAnimType  = ANIM_TYPE_CHARACTER_SITTING ;

            // Extend seat bbox above turrets
            Seat.LocalBBox.Max.Y += 1.0f ;
            break ;
    }
*/
}

//==============================================================================

void asstank::OnAcceptInit( const bitstream& BitStream )
{
    // Make valid object
    Initialize(vector3(0,0,0), 0, 0) ;

    // Now call base call accept init to override and setup vars
    gnd_effect::OnAcceptInit( BitStream );
}

//==============================================================================

object::update_code asstank::Impact( vector3& Pos, vector3& Norm )
{
    (void)Pos;
    (void)Norm;

    return UPDATE;
}

//==============================================================================

void asstank::UpdateInstances( void )
{
    // Call base class
    vehicle::UpdateInstances() ;

    // Get turret rot
    radian3 TurretRot = GetDrawTurretRot() ;

    // Calc turret rot
    quaternion VehicleRot      = quaternion(m_GeomInstance.GetRot()) ;
    quaternion TurretBaseRot   = quaternion(radian3(0, TurretRot.Yaw, 0)) ;
    quaternion TurretBarrelRot = quaternion(radian3(TurretRot.Pitch, TurretRot.Yaw, 0)) ;

    // Update turret base
    m_TurretBaseInstance.SetRot((VehicleRot * TurretBaseRot).GetRotation()) ;
    m_TurretBaseInstance.SetPos(m_GeomInstance.GetHotPointWorldPos(HOT_POINT_TYPE_TURRET_MOUNT)) ;

    // Update mortar barrel
    m_TurretBarrelMortarInstance.SetPos(m_TurretBaseInstance.GetHotPointWorldPos(m_TurretBarrelMortarHotPoint)) ;
    m_TurretBarrelMortarInstance.SetRot((VehicleRot * TurretBarrelRot).GetRotation()) ;

    // Update chain barrel
    m_TurretBarrelChainInstance.SetPos(m_TurretBaseInstance.GetHotPointWorldPos(m_TurretBarrelChainHotPoint)) ;
    m_TurretBarrelChainInstance.SetRot((VehicleRot * TurretBarrelRot).GetRotation()) ;
}

//==============================================================================

void asstank::Render( void )
{
    // Begin with base render...
    gnd_effect::Render() ;
    if (!m_IsVisible)
        return ;

    // Default to drawing turret base
    xbool   DrawTurretBase = TRUE ;

    // Don't render the base in 1st person!
    player_object* pPlayer = GetSeatPlayer(1) ;     // Get gunner player
    if ((pPlayer) && (pPlayer->GetProcessInput()))  // Drawing on players machine?
    {
        // Don't draw turret base if drawing in 1st person
        if (pPlayer->GetVehicleViewType() == player_object::VIEW_TYPE_1ST_PERSON)
            DrawTurretBase = FALSE ;
    }

    // Draw base?
    if (DrawTurretBase)
    {
        m_TurretBaseInstance.SetFogColor( m_GeomInstance.GetFogColor() ) ;
        m_TurretBaseInstance.SetColor( m_GeomInstance.GetColor() ) ;
        m_TurretBaseInstance.Draw( m_DrawFlags, m_DamageTexture, m_DamageClutHandle, m_RenderMaterialFunc, (void*)this );
    }

    // Draw mortar barrel
    m_TurretBarrelMortarInstance.SetFogColor( m_GeomInstance.GetFogColor() ) ;
    m_TurretBarrelMortarInstance.SetColor( m_GeomInstance.GetColor() ) ;
    m_TurretBarrelMortarInstance.Draw( m_DrawFlags, m_DamageTexture, m_DamageClutHandle, m_RenderMaterialFunc, (void*)this );

    // Draw chain barrel
    m_TurretBarrelChainInstance.SetFogColor( m_GeomInstance.GetFogColor() ) ;
    m_TurretBarrelChainInstance.SetColor( m_GeomInstance.GetColor() ) ;
    m_TurretBarrelChainInstance.Draw( m_DrawFlags, m_DamageTexture, m_DamageClutHandle, m_RenderMaterialFunc, (void*)this );
}

//==============================================================================

void asstank::RenderParticles( void )
{
    if ( m_IsVisible )
    {
        // render the base class particles
        gnd_effect::RenderParticles();

        if( m_Exhaust1.IsCreated() )
        {
            m_Exhaust1.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST1 ) );
            m_Exhaust1.RenderEffect();
        }

        if( m_Exhaust2.IsCreated() )
        {
            m_Exhaust2.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST2 ) );
            m_Exhaust2.RenderEffect();
        }

        // now render the muzzle flash if necessary
        if ( m_ChainGunHasFired )
        {
            // Make sure shape instances are in sync with actual object
            UpdateInstances() ;

		    // If this asserts, you need to add a fire weapon hot point info to the max
		    // weapon model file and re-export the .skel
            ASSERT(m_TurretBarrelChainInstance.GetHotPointByType(HOT_POINT_TYPE_FIRE_WEAPON)) ;

            matrix4 L2W ;
            L2W.SetTranslation(m_TurretBarrelChainInstance.GetHotPointWorldPos(HOT_POINT_TYPE_FIRE_WEAPON)) ;

            // Add in some random rotation
            radian3 TurretRot   = m_TurretBarrelChainInstance.GetRot() ;
		    L2W.SetRotation(radian3(TurretRot.Pitch + x_frand( -CHAINGUN_FLASH_ANGLE, CHAINGUN_FLASH_ANGLE ),
                                    TurretRot.Yaw + x_frand( -CHAINGUN_FLASH_ANGLE, CHAINGUN_FLASH_ANGLE ), TurretRot.Roll)) ;

            // start point
            vector3 StartPos = L2W.GetTranslation();

            // end point of the flame
            vector3 EndPos( 0.0f, 0.0f, x_frand(0.6f, 1.2f) );
            EndPos.Rotate( L2W.GetRotation() );

            EndPos += StartPos;

            // start drawing
            draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
            #ifdef TARGET_PS2
                gsreg_Begin();
                gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
                gsreg_SetZBufferUpdate(FALSE);
                gsreg_SetClamping( FALSE );
                gsreg_End();
            #endif

            // find the texture
            if ( s_FlashTexture == -1 )
            {
                s_FlashTexture = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]Flash") ;
            }

            // activate the texture
            draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_FlashTexture).GetXBitmap() );

            // draw the oriented quad
            draw_OrientedQuad( StartPos, EndPos, vector2(0,0), vector2(1,1), XCOLOR_WHITE, XCOLOR_WHITE, 0.15f );

            // finished
            draw_End();

            #ifdef TARGET_PS2
                gsreg_Begin();
                gsreg_SetZBufferUpdate(TRUE);
                gsreg_End();
            #endif
        }

/*
        // render effects
        if ( m_Thrust.Y >= 0.0f )
        {
            // render glowies
            matrix4 L2W = m_GeomInstance.GetL2W();
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL1, (m_Thrust.Y * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowL2, (m_Thrust.Y * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR1, (m_Thrust.Y * 0.5f) * x_frand(0.5f, 0.75f));
            s_VFX.RenderGlowyPtSpr( L2W, s_GlowR2, (m_Thrust.Y * 0.5f) * x_frand(0.5f, 0.75f) );
        }
*/    
    }
}

//==============================================================================

void asstank::Get3rdPersonView( s32 SeatIndex, vector3& Pos, radian3& Rot )
{
    (void)SeatIndex;
    (void)Pos;
    (void)Rot;
/*
    vector3 PivotPos, PivotDist ;

    // Special case yaw
    radian3 FreeLookYaw = Rot ;

    // Make sure shape instances are in sync with actual object
    UpdateInstances() ;

    // Different seats have different views
    const seat& Seat = GetSeat(SeatIndex) ;
    switch(Seat.Type)
    {
        case SEAT_TYPE_GUNNER:

            // Setup pivot pos
            PivotPos = s_AssTankTurret3rdPersonViewPivotOffset ;
            PivotPos.Rotate(m_TurretBaseInstance.GetRot()) ;
            PivotPos.Rotate(FreeLookYaw) ;
            PivotPos += (m_TurretBarrelMortarInstance.GetPos() + m_TurretBarrelChainInstance.GetPos()) * 0.5f ;
            //PivotPos += m_TurretBarrelMortarInstance.GetPos() ;

            // Setup pivot dist
            PivotDist = s_AssTankTurret3rdPersonViewPivotDist ;
            PivotDist.Rotate(m_TurretBarrelMortarInstance.GetRot()) ;
            PivotDist.Rotate(FreeLookYaw) ;

            // Setup view rot and pos
            Rot = m_TurretBarrelMortarInstance.GetRot() + FreeLookYaw ;
            Pos = PivotPos + PivotDist ;
            break ;

        default:

            // Special 3rd person view
            ASSERT( m_GeomInstance.GetHotPointByType(HOT_POINT_TYPE_VEHICLE_EYE_POS) ) ;
            Pos = AssTankPilotEyeOffset_3rd;
            Pos.Rotate( m_Rot );
            Pos.Rotate( FreeLookYaw );
            Pos += m_GeomInstance.GetHotPointWorldPos(HOT_POINT_TYPE_VEHICLE_EYE_POS) ;
            Rot += m_Rot ;
            break ;
    }
*/
}

//==============================================================================

void asstank::Get1stPersonView( s32 SeatIndex, vector3& Pos, radian3& Rot )
{
    (void)SeatIndex;
    (void)Pos;
    (void)Rot;
/*
    vector3 PivotPos, PivotDist ;

    // Make sure shape instances are in sync with actual object
    UpdateInstances() ;

    // Different seats have different views
    const seat& Seat = GetSeat(SeatIndex) ;
    switch(Seat.Type)
    {
        case SEAT_TYPE_GUNNER:

            // Setup pivot pos
            PivotPos = s_AssTankTurret1stPersonViewPivotOffset ;
            PivotPos.Rotate(m_TurretBaseInstance.GetRot()) ;
            PivotPos += (m_TurretBarrelMortarInstance.GetPos() + m_TurretBarrelChainInstance.GetPos()) * 0.5f ;

            // Setup pivot dist
            PivotDist = s_AssTankTurret1stPersonViewPivotDist ;
            PivotDist.Rotate(m_TurretBarrelMortarInstance.GetRot()) ;

            // Setup view rot and pos
            Rot = m_TurretBarrelMortarInstance.GetRot() ;
            Pos = PivotPos + PivotDist ;
            break ;

        default:
            // Call base class
            vehicle::Get1stPersonView( SeatIndex, Pos, Rot) ;
            break ;
    }
*/
}

//==============================================================================

s32 asstank::GetTurretBarrel( void ) const
{
    return m_TurretCurrentBarrel;
}

//==============================================================================

void asstank::SetTurretBarrel( s32 Barrel )
{
    // Only 2
    ASSERT((Barrel == 0) || (Barrel == 1)) ;

    // Switching?
    if (Barrel != m_TurretCurrentBarrel)
    {
        // Controlled by a player?
        player_object* pPlayer = GetSeatPlayer(1) ;
        if (pPlayer)
        {
            // Lookup activate sfx for current barrel
            s32 ActivateSfx = 0 ;
            switch(Barrel)
            {
                // Mortar
                case 0:
                    ActivateSfx = SFX_WEAPONS_MORTAR_ACTIVATE ;
                    break ;

                // Chaingun
                case 1:
                    ActivateSfx = SFX_WEAPONS_CHAINGUN_ACTIVATE ;
                    break ;
            }
       
            // Play full volume sfx?
            if (pPlayer->GetProcessInput())
                audio_Play(ActivateSfx) ;               // Full vol
            else
                audio_Play(ActivateSfx, &m_WorldPos) ;  // Normal vol
        }

        // Finally, set new barrel
        m_TurretCurrentBarrel = Barrel ;
    }
}

//==============================================================================

xbool asstank::ObjectInWayOfTurret( void )
{
    bbox    BBox ;
    vector3 Expand = vector3(0.2f, 0.2f, 0.2f) ;

    // Make sure the instances are in sync
    UpdateInstances() ;

    // Check base (leave top bbox alone)
    BBox  = m_TurretBaseInstance.GetWorldBounds() ;
    BBox.Min -= Expand ;        
    BBox.Max.X += Expand.X ;        
    BBox.Max.Z += Expand.Z ;        
    if (ObjectInWayOfCollision(BBox))
        return TRUE ;

    // Check mortar barrel
    BBox  = m_TurretBarrelMortarInstance.GetWorldBounds() ;
    BBox.Min -= Expand ;        
    BBox.Max += Expand ;        
    if (ObjectInWayOfCollision(BBox))
        return TRUE ;

    // Check chain barrel
    BBox  = m_TurretBarrelChainInstance.GetWorldBounds() ;
    BBox.Min -= Expand ;        
    BBox.Max += Expand ;        
    if (ObjectInWayOfCollision(BBox))
        return TRUE ;

    // Nothing is in the way
    return FALSE ;
}

//==============================================================================

f32 asstank::GetTurretSpeedScaler( void )
{
    // If a player is in the way of the turret, don't allow movement
    if (ObjectInWayOfTurret())
        return 0.0f ;
    else
        return m_TurretSpeedScaler ;    // Allow normal movement
}

//==============================================================================

void asstank::ApplyMove ( player_object* pPlayer, s32 SeatIndex, player_object::move& Move )
{
    (void)pPlayer;
    (void)SeatIndex;
    (void)Move;
/*
    // Call base class to position the turret etc
    gnd_effect::ApplyMove( pPlayer, SeatIndex, Move );

    // play audio for jetting
    if ( ( Move.JetKey ) && ( m_AudioAfterBurn == -1 ) && ( m_Energy > 10.0f) )
        m_AudioAfterBurn =  audio_Play( SFX_VEHICLE_BEOWULF_AFTERBURNER_LOOP, &m_WorldPos, AUDFLAG_PERSISTENT );

    // For dirty bit setting
    xbool LastChainGunFiring = m_ChainGunFiring ;

    // If there is no one in seat 1 (chaingun/mortar) then there is no chaingun fire
    if ((tgl.ServerPresent) && (GetSeatPlayer(1) == NULL) || (m_TurretCurrentBarrel == 0))
        m_ChainGunFiring = FALSE ;

    // Update weapons?
    if ((pPlayer) && (tgl.ServerPresent))
    {
        // Get seat info
        const seat& Seat = GetSeat(SeatIndex) ;
        switch(Seat.Type)
        {
            // Gunner?
            case SEAT_TYPE_GUNNER:

                // Update fire delays
                m_Fire1Delay -= Move.DeltaTime ;
                if (m_Fire1Delay < 0)
                    m_Fire1Delay = 0 ;

                // Update fire delays
                m_Fire2Delay -= Move.DeltaTime ;
                if (m_Fire2Delay < 0)
                    m_Fire2Delay = 0 ;

                // Change turret?
                if ((Move.PrevWeaponKey) || (Move.NextWeaponKey))
                {
                    // Setup new barrel (call function so sfx is played on server)
                    SetTurretBarrel(m_TurretCurrentBarrel ^1) ;

                    // Clear keys
                    Move.PrevWeaponKey = FALSE ;
                    Move.NextWeaponKey = FALSE ;
                }

	            // Trying to fire mortar?
                if ( (Move.FireKey) && (m_TurretCurrentBarrel == 0) && ( m_WeaponEnergy > MORTAR_ENERGY ) )
	            {
                    // Create a particle?
                    if (m_Fire1Delay == 0)
                    {
		                // If this asserts, you need to add a fire weapon hot point info to the max
		                // weapon model file and re-export the .skel
                        ASSERT(m_TurretBarrelMortarInstance.GetHotPointByType(HOT_POINT_TYPE_FIRE_WEAPON)) ;
		                vector3 TurretPos = m_TurretBarrelMortarInstance.GetHotPointWorldPos(HOT_POINT_TYPE_FIRE_WEAPON) ;
                        radian3 TurretRot = m_TurretBarrelMortarInstance.GetRot() ;

                        // get the direction
                        matrix4 L2W ;
                        L2W.SetTranslation(TurretPos) ;
		                L2W.SetRotation(TurretRot) ;
                        vector3 Dir;
                        Dir.X = L2W(2,0);
                        Dir.Y = L2W(2,1);
                        Dir.Z = L2W(2,2);

                        // Create the morta
		                mortar* pMortar = (mortar*)ObjMgr.CreateObject( object::TYPE_MORTAR );
                        pMortar->Initialize( L2W.GetTranslation(), Dir, m_Vel, pPlayer->GetObjectID(), m_ObjectID, pPlayer->GetTeamBits(), 1 );
                        ObjMgr.AddObject( pMortar, obj_mgr::AVAILABLE_SERVER_SLOT );

                        // Wait before firing again
                        m_Fire1Delay = FIRE_MORTAR_DELAY;

                        // Subtract weapon energy
                        AddWeaponEnergy( -MORTAR_ENERGY );

                        // Do fire anim
                        m_TurretBarrelMortarInstance.SetAnimByType(ANIM_TYPE_TURRET_FIRE, 0,0,TRUE) ;

                        // Tell all clients to do fire anim
                        m_DirtyBits |= VEHICLE_DIRTY_BIT_FIRE ;
                    }
                }

	            // Trying to fire bullet?
                if ( (Move.FireKey) && (m_TurretCurrentBarrel == 1) && ( m_WeaponEnergy > BULLET_ENERGY ) )
                {
                    // Flag we are firing the chaingun
                    m_ChainGunFiring = TRUE ;

                    // Create a particle? (on server only)
                    if (m_Fire2Delay == 0)
                    {
		                // If this asserts, you need to add a fire weapon hot point info to the max
		                // weapon model file and re-export the .skel
                        ASSERT(m_TurretBarrelChainInstance.GetHotPointByType(HOT_POINT_TYPE_FIRE_WEAPON)) ;
		                matrix4 L2W ;
                        L2W.SetTranslation(m_TurretBarrelChainInstance.GetHotPointWorldPos(HOT_POINT_TYPE_FIRE_WEAPON)) ;

                        // Add in some random rotation
                        radian3 TurretRot   = m_TurretBarrelChainInstance.GetRot() ;
		                L2W.SetRotation(radian3(TurretRot.Pitch + x_frand( -CHAINGUN_RAND_ANGLE, CHAINGUN_RAND_ANGLE ),
                                                TurretRot.Yaw + x_frand( -CHAINGUN_RAND_ANGLE, CHAINGUN_RAND_ANGLE ), TurretRot.Roll)) ;

		                // Fire!
		                bullet* pBullet = (bullet*)ObjMgr.CreateObject( object::TYPE_BULLET );
		                pBullet->Initialize( L2W, m_Vel, pPlayer->GetTeamBits(), pPlayer->GetObjectID(), m_ObjectID, 1 );
                        ObjMgr.AddObject( pBullet, obj_mgr::AVAILABLE_SERVER_SLOT );

                        // Wait before firing again
                        m_Fire2Delay = FIRE_CHAIN_DELAY;

                        // Subtract weapon energy
                        AddWeaponEnergy( -BULLET_ENERGY );
                    }
                }
                else
                {
                    // Player is not trying to fire
                    m_ChainGunFiring = FALSE ;
                }
                break ;
        }
    }

    // Update firing dirty bits
    if (LastChainGunFiring != m_ChainGunFiring)
        m_DirtyBits |= VEHICLE_DIRTY_BIT_FIRE ;
*/
}

//==============================================================================

void asstank::WriteDirtyBitsData  ( bitstream& BitStream, u32& DirtyBits, f32 Priority, xbool MoveUpdate )
{
    // Turret barrel
    BitStream.WriteRangedS32(m_TurretCurrentBarrel,0,1) ;

    // Chain gun status
    BitStream.WriteFlag(m_ChainGunFiring) ;

    /*
    // Fire?
    if (BitStream.WriteFlag(DirtyBits & VEHICLE_DIRTY_BIT_FIRE))
    {
        // This is all we have to do...
        DirtyBits &= ~VEHICLE_DIRTY_BIT_FIRE ;
    }
    */

    // Call base class
    gnd_effect::WriteDirtyBitsData(BitStream, DirtyBits, Priority, MoveUpdate) ;
}

//==============================================================================

xbool asstank::ReadDirtyBitsData   ( const bitstream& BitStream, f32 SecInPast )
{
    // Turret barrel (goes through function so sfx are triggered)
    s32 Barrel ;
    BitStream.ReadRangedS32(Barrel,0,1) ;
    SetTurretBarrel(Barrel) ;

    // Chain gun fire status
    m_ChainGunFiring = BitStream.ReadFlag() ;

    /*
    // Fire?
    if (BitStream.ReadFlag())
    {
        // Which turret barrel?
        switch(Barrel)
        {
            // Mortar?
            case 0:
                // Do fire anim
                m_TurretBarrelMortarInstance.SetAnimByType(ANIM_TYPE_TURRET_FIRE, 0,0,TRUE) ;
                break ;

            // Chaingun?
            case 1:
                // Already taken care of in main loop
                break ;
        }
    }
    */

    // Call base class
    return gnd_effect::ReadDirtyBitsData(BitStream, SecInPast) ;
}

//==============================================================================

object::update_code asstank::OnAdvanceLogic  ( f32 DeltaTime )
{
    matrix4 EP1;

    // set the axis for exhaust
    vector3 Axis;

    // call base class first, remember return param
    object::update_code RetVal = gnd_effect::OnAdvanceLogic( DeltaTime );

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
        m_Exhaust1.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST2 ) );
        m_Exhaust1.SetVelocity( vector3(0.0f, 0.0f, 0.0f) );
        //m_Exhaust1.SetVelocity( (EP1.GetTranslation() - m_Exhaust1.GetPosition()) / DeltaTime );
        m_Exhaust1.UpdateEffect( DeltaTime );
    }
    
    // update particle effect
    if ( m_Exhaust2.IsCreated() )
    {
        m_Exhaust2.GetEmitter(0).SetAxis( Axis );
        m_Exhaust2.GetEmitter(0).UseAxis( TRUE );
        m_Exhaust2.SetPosition( m_GeomInstance.GetHotPointWorldPos( HOT_POINT_TYPE_VEHICLE_EXHAUST1 ) );
        m_Exhaust2.SetVelocity( vector3(0.0f, 0.0f, 0.0f) );
        //m_Exhaust2.SetVelocity( (EP1.GetTranslation() - m_Exhaust2.GetPosition()) / DeltaTime );
        m_Exhaust2.UpdateEffect( DeltaTime );
    }

    // update audio
    audio_SetPosition   ( m_AudioEng, &m_WorldPos );
    audio_SetVolume( m_AudioEng, m_Control.ThrustY * 0.20f + 0.80f );
    
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

  	// Play chain gun sound looping firing sound!
    if (m_ChainGunFiring)
        audio_PlayLooping(m_AudioChainGunFire, SFX_WEAPONS_CHAINGUN_FIRE_LOOP, &m_WorldPos );
    else
        audio_StopLooping(m_AudioChainGunFire) ;

    // Update chain gun fx vars
    if (m_ChainGunFiring)
    {
        // Decrease timer
        m_ChainGunFxDelay -= DeltaTime ;
        if (m_ChainGunFxDelay < 0)
        {
            // Set next interval
            m_ChainGunFxDelay  = FIRE_CHAIN_FX_DELAY ;
            m_ChainGunHasFired = TRUE ; // For muzzle flash effect

            // Do fire anim
            m_TurretBarrelChainInstance.SetAnimByType(ANIM_TYPE_TURRET_FIRE, 0,0,TRUE) ;
        }
        else
            m_ChainGunHasFired = FALSE ;
    }
    else
    {
        m_ChainGunFxDelay  = 0 ;
        m_ChainGunHasFired = FALSE ;
    }

    // Update turret animations
    if (m_State == STATE_MOVING)
    {
        m_TurretBaseInstance.Advance(DeltaTime) ;
        m_TurretBarrelMortarInstance.Advance(DeltaTime) ;
        m_TurretBarrelChainInstance.Advance(DeltaTime) ;
    }

    // Return the turret back to normal if no player is in it
    if (GetSeatPlayer(1) == NULL)
    {
        // Do we need to return?
        if ((m_TurretRot.Pitch != 0) || (m_TurretRot.Yaw != 0))
        {
            // Only allow movement if no one is in the way
            if (!ObjectInWayOfTurret())
            {
                // Finally return!
                ReturnTurretToRest(DeltaTime) ;
            }                
        }
    }

    return RetVal;
}

void asstank::OnRemove( void )
{
    audio_Stop(m_AudioEng);
    audio_Stop(m_AudioAfterBurn); 
    audio_StopLooping(m_AudioChainGunFire) ;

    gnd_effect::OnRemove();
}

//==============================================================================

void asstank::OnCollide( u32 AttrBits, collider& Collider )
{
    // Begin with base class for main geometry
    Collider.SetID(0) ;
    gnd_effect::OnCollide(AttrBits, Collider) ;

    // Now check collision with barrel + turrets

    // Collide with geometry or node bounds?
    object* pMovingObject = (object*)Collider.GetMovingObject() ;
    if( pMovingObject )
    {
        switch(pMovingObject->GetType())
        {
            // Collide with geometry for perfect node collision
            case object::TYPE_SATCHEL_CHARGE:

                Collider.SetID(1) ;
                m_TurretBaseInstance.ApplyCollision( Collider ) ;

                Collider.SetID(2) ;
                m_TurretBarrelMortarInstance.ApplyCollision( Collider ) ;

                Collider.SetID(3) ;
                m_TurretBarrelChainInstance.ApplyCollision( Collider ) ;
                return;
                break ;
        }
    }

    // Collide with node bounding boxes of barrel + turrets
    Collider.SetID(1) ;
    m_TurretBaseInstance.ApplyNodeBBoxCollision( Collider ) ;

    Collider.SetID(2) ;
    m_TurretBarrelMortarInstance.ApplyNodeBBoxCollision( Collider ) ;

    Collider.SetID(3) ;
    m_TurretBarrelChainInstance.ApplyNodeBBoxCollision( Collider ) ;
}

//==============================================================================

matrix4* asstank::GetNodeL2W( s32 ID, s32 NodeIndex )
{
    // Make sure instances are uptodate
    UpdateInstances() ;

    // Get vehicle render node
    render_node* RenderNodes ;
    switch(ID)
    {
        default:
            return NULL ;

        case 0:
            RenderNodes = m_GeomInstance.CalculateRenderNodes() ;
            break ;

        case 1:
            RenderNodes = m_TurretBaseInstance.CalculateRenderNodes() ;
            break ;

        case 2:
            RenderNodes = m_TurretBarrelMortarInstance.CalculateRenderNodes() ;
            break ;

        case 3:
            RenderNodes = m_TurretBarrelChainInstance.CalculateRenderNodes() ;
            break ;
    }

    // Present?
    if (RenderNodes)
        return &RenderNodes[NodeIndex].L2W ;
    else
        return NULL ;
}

//==============================================================================

f32 asstank::GetPainRadius( void ) const
{
    return( 4.0f );
}

//==============================================================================
