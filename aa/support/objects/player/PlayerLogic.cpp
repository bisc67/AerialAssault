//==============================================================================
//
//  PlayerLogic.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "CorpseObject.hpp"
#include "Entropy.hpp"
#include "NetLib/bitstream.hpp"
#include "Demo1/globals.hpp"
#include "Demo1/fe_colors.hpp"
#include "Demo1/fe_globals.hpp"
#include "Demo1/GameServer.hpp"
#include "PointLight/PointLight.hpp"
#include "AudioMgr/Audio.hpp"
#include "GameMgr/GameMgr.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Objects/Projectiles/Flag.hpp"
#include "Objects/Projectiles/Mine.hpp"
#include "Objects/Projectiles/SatchelCharge.hpp"
#include "Objects/Projectiles/Disk.hpp"
#include "Objects/Projectiles/Plasma.hpp"
#include "Objects/Projectiles/Bullet.hpp"
#include "Objects/Projectiles/Blaster.hpp"
#include "Objects/Projectiles/Laser.hpp"
#include "Objects/Projectiles/Mortar.hpp"
#include "Objects/Projectiles/Grenade.hpp"
#include "Objects/Projectiles/FlipFlop.hpp"
#include "Objects/Projectiles/Beacon.hpp"
#include "Objects/Projectiles/Turret.hpp"
#include "Objects/Projectiles/Sensor.hpp"
#include "Objects/Projectiles/Station.hpp"
#include "Objects/Projectiles/AutoAim.hpp"
#include "Objects/Scenic/Scenic.hpp"
#include "Objects/Pickups/Pickups.hpp"
#include "Objects/terrain/terrain.hpp"
#include "Objects/Vehicles/Vehicle.hpp"
#include "Objects/Vehicles/Bomber.hpp"
#include "Objects/Bot/BotObject.hpp"
#include "UI/ui_manager.hpp"
#include "UI/ui_tabbed_dialog.hpp"
#include "HUD/hud_manager.hpp"
#include "HUD/hud_voice_ids.hpp"
#include "DefaultLoadouts.hpp"
#include "Demo1\Data\UI\Messages.h"

//#define DEBUG_TURN_STATS_ON
#include "Shape/DebugUtils.hpp"

#include "Demo1/SpecialVersion.hpp"

//=========================================================================
// DEBUG COLLISION DEFINES
//=========================================================================

//#define DEBUG_COLLISION             // Draws ngon points (setup by player collision)
//#define PLAYER_DEBUG_CREASE_COLL    // Turns debug prints on for crease collision checks
//#define PLAYER_DEBUG_STEP_COLL      // Turns debug prints on for step collision checks
//#define PLAYER_DEBUG_LIP_COLL      // Turns debug prints on for lip collision checks
//#define DRAW_VIEW_AND_WEAPON_FIRE_POS   // Draws view and weapon fire pos
//#define DEBUG_BITSTREAM                 // Check what data is getting sent

xbool g_bUnlimitedAmmo = FALSE;

#ifdef DEBUG_COLLISION

extern vector3 CollPoints[] ;
extern s32     NCollPoints ;

extern vector3 PlayerCollPoints0[] ;
extern s32     NPlayerCollPoints0 ;

extern vector3 PlayerCollPoints1[] ;
extern s32     NPlayerCollPoints1 ;

extern vector3 PlayerCollPoints2[] ;
extern s32     NPlayerCollPoints2 ;

#endif

#ifdef DRAW_VIEW_AND_WEAPON_FIRE_POS

extern bbox    PlayerWeaponStartBBox ;
extern bbox    PlayerWeaponEndBBox ;

#endif

extern xbool   g_TweakGame;
extern xbool   g_TweakMenu;

extern s32      g_AdminCount;

//=========================================================================

f32     BreakLockTime       = 1.5f;

//=========================================================================
// SPRING CAMERA DEFINES
//=========================================================================

xbool   UseSpringCam       = TRUE;

f32     PlayerCamRotKs     = 8.0f;
f32     PlayerCamRotKd     = 1.0f;

f32     VehicleCamRotKs    = 8.0f;
f32     VehicleCamRotKd    = 1.0f;

f32     PlayerCamTiltKs    = 4.0f;
f32     PlayerCamTiltKd    = 1.0f;

f32     VehicleCamTiltKs   = 1.0f;
f32     VehicleCamTiltKd   = 0.1f;

radian  VehicleMaxYawAng   = R_15;
radian  VehicleMaxPitchAng = R_10;

radian  PlayerMaxYawAng    = R_15;
radian  PlayerMaxPitchAng  = R_1;

radian  PlayerFadeAng      = R_45;

static f32 MIN_PIXEL_SIZE_UPDATE_LIGHTING   = 10 ;


//=========================================================================
// KEY PRESS DEFINES
//=========================================================================

f32 player_object::GRENADE_THROW_DELAY_TIME = 1.0f ;    // Min time between throwing grenades
f32 player_object::GRENADE_THROW_MIN_TIME   = 0.0f ;    // Min time throw key must be held down
f32 player_object::GRENADE_THROW_MAX_TIME   = 2.0f ;    // Max time for max distance 

f32 player_object::MINE_THROW_DELAY_TIME    = 1.0f ;    // Min time between throwing mines
f32 player_object::MINE_THROW_MIN_TIME      = 0.0f ;    // Min time throw key must be held down
f32 player_object::MINE_THROW_MAX_TIME      = 2.0f ;    // Max time for max distance 

f32 player_object::SPAM_MIN_TIME      = 2.0f ;    // Min time between taunting
f32 player_object::SPAM_MAX_TIME      = 4.0f ;    // Max time between taunting
f32 player_object::SPAM_DELAY_TIME    = 0.5f ;    // Time penality if spam timer is not 0 and taunt is pressed

//=========================================================================
// DEBUG DEFINES
//=========================================================================
f32     player_object::PLAYER_DEBUG_SPEED         = 1.0f ;   // Run speed scaler
xbool   player_object::PLAYER_DEBUG_GROUND_TRACK  = TRUE ;   // Enables ground tracking
xbool   player_object::PLAYER_DEBUG_3RD_PERSON    = FALSE ;  // Override to 3rd person view



//=========================================================================
// MOVEMENT DEFINES
//=========================================================================

f32 player_object::PLAYER_SEND_MOVE_INTERVAL = (1.0f / 60.0f) ;         // Send move interval in secs
f32 player_object::SPAWN_TIME                = 5.0f ;                   // Total spawn time
f32 player_object::MOVING_EPSILON            = 0.001f ;			        // Velocity must be bigger than this to move the player
f32 player_object::MAX_STEP_HEIGHT           = 0.6f ;                   // Max height of step that a player can get over
f32 player_object::VERTICAL_STEP_DOT         = 0.173f ;  // 80
f32 player_object::MIN_FACE_DISTANCE         = 0.01f ;
f32 player_object::TRACTION_DISTANCE         = 0.03f ;
f32 player_object::NORMAL_ELASTICITY         = 0.01f ;
s32 player_object::MOVE_RETRY_COUNT          = 4 ;//5
f32 player_object::FALLING_THRESHOLD         = -10 ;
f32 player_object::JUMP_SKIP_CONTACTS_MAX    = (8.0f / 30.0f) ;
f32 player_object::DEPLOY_DIST               = 8.0f ;
f32 player_object::BOT_DEPLOY_DIST           = 16.0f ;
f32 player_object::PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX = player_object::JUMP_SKIP_CONTACTS_MAX ;
f32 player_object::PLAYER_LAND_RECOVER_TIME_MAX = 4.0f ;
f32 player_object::PLAYER_LIP_TEST_DIST      = 0.25f ;  // Dist below plane for lip test
f32 player_object::PLAYER_STEP_CLEAR_DIST    = 0.5f ;   // Dist that must be clear infront of step
f32 player_object::PLAYER_CONTACT_THRESHOLD  = 0.01f ;  // T threshold for comparing contact points


// Ground tracking defines
f32 player_object::GROUND_TRACKING_MAX_DIST = 1.5f ;	// Max distance to snap to ground
f32 player_object::GROUND_TRACKING_DOT      = -0.087f ;	// Determines when player is running into ground -0.087 = cos(95)

// Contact info defines...
f32 player_object::PLAYER_CONTACT_DIST       = (player_object::TRACTION_DISTANCE * 2.0f) ; // Dist for contact to take place
f32 player_object::PLAYER_HIT_BY_PLAYER_DIST = 0.2f ;                     // Dist to trigger hitting objects
f32 player_object::PLAYER_SURFACE_CHECK_DIST = 2.0f ;                     // Dist to record DistToSurface
                                
                                        
// Energy/health tweakables
f32 player_object::SHIELD_PACK_DRAIN_SCALER  = 1.25f ;
f32 player_object::HEALTH_INCREASE_PER_SEC   = 100.0f / 4.0f ;



//=========================================================================
// COLLISION ATTRIBUTE DEFINES
//=========================================================================

// Objects that player can land and walk on
u32 player_object::PLAYER_LANDABLE_ATTRS = (object::ATTR_SOLID) ;

// Objects that player collides with
u32 player_object::PLAYER_COLLIDE_ATTRS = (object::ATTR_SOLID) ;

// Objects that player weapon collides with
u32 player_object::WEAPON_COLLIDE_ATTRS = (object::ATTR_SOLID) ;
    
// Objects that player can pickup
u32 player_object::PLAYER_PICKUPS_ATTRS = (object::ATTR_PERMEABLE) ;


//=========================================================================
// PICKUP AMMO DATA
//=========================================================================

// List of invent objects that a ammo pack/box pickup can contain
struct pickup_ammo_info
{
    player_object::invent_type  InventType ;    // Ammo pickup can hold
    s32                         Max ;           // Max ammount pickup can hold
} ;

pickup_ammo_info  PickupAmmoInfoList[] =
{
    {player_object::INVENT_TYPE_WEAPON_AMMO_DISK,       15},
    {player_object::INVENT_TYPE_WEAPON_AMMO_BULLET,     150},
    {player_object::INVENT_TYPE_WEAPON_AMMO_GRENADE,    15},
    {player_object::INVENT_TYPE_WEAPON_AMMO_PLASMA,     30},
    {player_object::INVENT_TYPE_WEAPON_AMMO_MORTAR,     10},
    {player_object::INVENT_TYPE_WEAPON_AMMO_MISSILE,    4},
    {player_object::INVENT_TYPE_GRENADE_BASIC,          10},
    {player_object::INVENT_TYPE_GRENADE_FLASH,          10},
    {player_object::INVENT_TYPE_GRENADE_CONCUSSION,     10},
    {player_object::INVENT_TYPE_GRENADE_FLARE,          10},
    {player_object::INVENT_TYPE_MINE,                   5},
    {player_object::INVENT_TYPE_BELT_GEAR_HEALTH_KIT,   1},
} ;

#define PICKUP_AMMO_INFO_LIST_SIZE        ((s32)(sizeof(PickupAmmoInfoList) / sizeof(pickup_ammo_info)))


//=========================================================================
// MISC DEFINES
//=========================================================================

#ifdef T2_DESIGNER_BUILD
    extern xbool g_DeployMine;
#endif
extern bot_object::bot_debug g_BotDebug;

static const f32 BOT_CHEAT_ACCEL_PERCENT = 0.75f ;


//==============================================================================
// FUNCTIONS
//==============================================================================

object::update_code player_object::AdvanceLogic( f32 DeltaTime )
{
    //x_printfxy(5,5,"State: %s", Player_GetStateString(m_PlayerState)) ;

    // Start logging of timer stats
    DEBUG_BEGIN_STATS(1,0)

    // Add in pending time that was not processed last time
    f32 MoveDeltaTime = DeltaTime + m_PendingDeltaTime ;

    // player must belong to some client
    if( !m_ProcessInput && tgl.ServerPresent )
	{
        // Moves are applied when recieved from client.
        // Blending is performed before drawing
	}
    else
    // This is a ghost on some client
    if( !m_ProcessInput && !tgl.ServerPresent )
	{
        // Ghost players are always blendeding to the
        // latest predicted server position

        // Any prediction time left?
        if ((m_PredictionTime >= DeltaTime) || (m_PlayerState == PLAYER_STATE_TAUNT))
        {
            // Update prediction time
            m_PredictionTime -= DeltaTime ;

            // Keep predicting with the same move...
            m_Move.DeltaTime = DeltaTime ;
            ApplyMove() ;
            ApplyPhysics() ;
        }
        else
        {
            // Update weapon logic
	        Weapon_AdvanceState(DeltaTime) ;

            // Update player logic
	        Player_AdvanceState(DeltaTime) ;
        }
    }
    else
    // On client. Create move for server and apply to self
    // No dirty bits needed
    if( m_ProcessInput && !tgl.ServerPresent )
    {
        // Accumulate input (delta time is used incase front end needs it)
	    CollectInput(DeltaTime) ;

        // Create the move from the accumulated input
        CreateMoveFromInput(MoveDeltaTime) ;

        // Since delta time is truncated to the nearest frame,
        // we store the remaining time ready for next time...
        m_PendingDeltaTime = MoveDeltaTime - m_Move.DeltaTime ;
        if (m_PendingDeltaTime < 0)            
            m_PendingDeltaTime = 0;

        // Keep copy of move ready for sending, since the state code may clear
        // button presses etc.
        player_object::move  MoveToSend = m_Move ;

        // Apply move to self
        ApplyMove();
        ApplyPhysics() ;

        // Send move now it's ready to go!
        if (m_pMM)
            m_pMM->SendMove( m_ObjectID.Slot, MoveToSend) ;
    }
    else
    // On server.  Create and apply move directly
    // Update dirty bits
    if( m_ProcessInput && tgl.ServerPresent )
    {
        // Accumulate input (delta time is used incase front end needs it)
		CollectInput(DeltaTime) ;

        // Create the move from the accumulated input
        CreateMoveFromInput(MoveDeltaTime) ;

        // Since delta time is truncated to the nearest frame,
        // we store the remaining time ready for next time...
        m_PendingDeltaTime = MoveDeltaTime - m_Move.DeltaTime ;

        // Collect net stats
        #ifdef DEBUG_BITSTREAM
            move    MoveToSend = m_Move ;
        #endif    

        // Apply move
        ApplyMove() ;
        ApplyPhysics();

        // Collect net stats
        #ifdef DEBUG_BITSTREAM
        {
            u32         DirtyBits ;
            f32         Priority ;
            bitstream   BitStream ;
            s32         x=0,y=4, Bits, Bytes ;

            x_printfxy(x,y++, "LogicDeltaTime     :%f ", DeltaTime) ;
            x_printfxy(x,y++, "m_PendingDeltaTime :%f ", m_PendingDeltaTime) ;
            x_printfxy(x,y++, "MoveDeltaTime      :%f ", MoveDeltaTime) ;
            x_printfxy(x,y++, "m_Move.DeltaTime   :%f ", m_Move.DeltaTime) ;
            x_printfxy(x,y++, "m_Move.DelteVBlanks:%d", m_Move.DeltaVBlanks) ;
            
            y++ ;
            BitStream.SetCursor(0) ;
            MoveToSend.Write(BitStream) ;
            Bits = BitStream.GetCursor() ;
            Bytes = (Bits+7)/8 ;
            x_printfxy(x,y++, "REG move:write Bits:%d Bytes:%d", Bits, Bytes) ;
        
            y++ ;
            DirtyBits = m_DirtyBits ;
            Priority  = 1 ;
            BitStream.SetCursor(0) ;
            OnProvideUpdate(BitStream, DirtyBits, Priority) ;
            Bits = BitStream.GetCursor() ;
            Bytes = (Bits+7)/8 ;
            x_printfxy(x,y++, "REG Update:Pri0 Bits:%d Bytes:%d", Bits, Bytes) ;
            ASSERT(DirtyBits == 0) ;

            DirtyBits = 0xFFFFFFFF ;
            Priority  = 1 ;
            BitStream.SetCursor(0) ;
            OnProvideUpdate(BitStream, DirtyBits, Priority) ;
            Bits = BitStream.GetCursor() ;
            Bytes = (Bits+7)/8 ;
            x_printfxy(x,y++, "MAX Update:Pri0 Bits:%d Bytes:%d", Bits, Bytes) ;
            ASSERT(DirtyBits == 0) ;

            y++ ;
            DirtyBits = m_DirtyBits ;
            Priority  = 1 ;
            BitStream.SetCursor(0) ;
            OnProvideMoveUpdate(BitStream, DirtyBits) ;
            Bits = BitStream.GetCursor() ;
            Bytes = (Bits+7)/8 ;
            x_printfxy(x,y++, "REG MoveUpdate:Pri0 Bits:%d Bytes:%d", Bits, Bytes) ;
            ASSERT(DirtyBits == 0) ;

            DirtyBits = 0xFFFFFFFF ;
            Priority  = 1 ;
            BitStream.SetCursor(0) ;
            OnProvideMoveUpdate(BitStream, DirtyBits) ;
            Bits = BitStream.GetCursor() ;
            Bytes = (Bits+7)/8 ;
            x_printfxy(x,y++, "MAX MoveUpdate:Pri0 Bits:%d Bytes:%d", Bits, Bytes) ;
            ASSERT(DirtyBits == 0) ;

            //m_DirtyBits = 0 ;
        }
        #endif    

    }

	// Player house keeping and state updates
    DEBUG_BEGIN_TIMER(AdvanceInst)
	Advance(DeltaTime) ;    // advance animation
    DEBUG_END_TIMER(AdvanceInst)

    // End logging of timer stats
    DEBUG_END_STATS

    return( UPDATE );
}

//==============================================================================

void player_object::SetPlayerShape(shape *PlayerShape, s32 SkinTexture)
{
    s32     i ;
    vector3 Scale, InvScale ;

    ASSERT(PlayerShape) ;
    m_PlayerInstance.SetShape(PlayerShape) ;

    // Lookup jet hot points
    model* pModel = m_PlayerInstance.GetModel() ;
    ASSERT(pModel) ;
    m_nJets = 0 ;
    for (i = 0 ; i < pModel->GetHotPointCount() ; i++)
    {
        // Get hot point
        hot_point* pHotPoint = pModel->GetHotPointByIndex(i) ;
        ASSERT(pHotPoint) ;

        switch(pHotPoint->GetType())
        {
            // Is hits a jet emitter hot point?
            case HOT_POINT_TYPE_BACK_JETPACK_EMITTER:
            case HOT_POINT_TYPE_LEFT_FOOT_JETPACK_EMITTER:
            case HOT_POINT_TYPE_RIGHT_FOOT_JETPACK_EMITTER:

                // Add to list
                m_JetHotPoint[m_nJets++] = pHotPoint ;
                ASSERTS(m_nJets <= MAX_JETS, "Increase MAX_JETS!") ;
                break ;
        }
    }

    // Add layered anims
    anim_state *BodyUD   = m_PlayerInstance.AddAdditiveAnim(ADDITIVE_ANIM_ID_BODY_LOOK_UD) ;
    anim_state *HeadLR   = m_PlayerInstance.AddAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_LR) ;
    anim_state *HeadUD   = m_PlayerInstance.AddAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_UD) ;
   
    ASSERT(BodyUD) ;
    ASSERT(HeadLR) ;
    ASSERT(HeadUD) ;

    BodyUD->SetSpeed(0.0f) ;
    HeadLR->SetSpeed(0.0f) ;
    HeadUD->SetSpeed(0.0f) ;

    m_PlayerInstance.SetAdditiveAnimByType(ADDITIVE_ANIM_ID_BODY_LOOK_UD, ANIM_TYPE_CHARACTER_LOOK_SOUTH_NORTH) ;
    ASSERT(BodyUD->GetAnim()) ;

    m_PlayerInstance.SetAdditiveAnimByType(ADDITIVE_ANIM_ID_HEAD_LOOK_LR, ANIM_TYPE_CHARACTER_HEAD_LEFT_RIGHT) ;
    ASSERT(HeadLR->GetAnim()) ;

    m_PlayerInstance.SetAdditiveAnimByType(ADDITIVE_ANIM_ID_HEAD_LOOK_UD, ANIM_TYPE_CHARACTER_HEAD_UP_DOWN) ;
    ASSERT(HeadUD->GetAnim()) ;

    // temp
    static xbool AddBodyUD=TRUE ;
    static xbool AddHeadLR=TRUE ;
    static xbool AddHeadUD=TRUE ;
    if (!AddBodyUD)
        m_PlayerInstance.DeleteAdditiveAnim(ADDITIVE_ANIM_ID_BODY_LOOK_UD) ;

    if (!AddHeadLR)
        m_PlayerInstance.DeleteAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_LR) ;

    if (!AddHeadUD)
        m_PlayerInstance.DeleteAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_UD) ;

    // Set skin
    m_PlayerInstance.SetTextureAnimFPS(0) ; // No texture anim
    m_PlayerInstance.SetTextureFrame((f32)SkinTexture) ;

    // Set the player scale
    Scale = m_MoveInfo->SCALE ;
    m_PlayerInstance.SetScale(m_MoveInfo->SCALE) ;

    // Since attached instances inherit parent transform, we need to remove the scale
    InvScale.X = 1.0f / Scale.X ;
    InvScale.Y = 1.0f / Scale.Y ;
    InvScale.Z = 1.0f / Scale.Z ;
    m_WeaponInstance.SetScale(InvScale) ;
    m_BackpackInstance.SetScale(InvScale) ;
    m_FlagInstance.SetScale(InvScale) ;
}

//==============================================================================

void player_object::SetWeaponShape(shape *WeaponShape)
{
    m_WeaponInstance.SetShape(WeaponShape) ;

    // Make weapon relative to player mount position
    //if ((WeaponShape) && (m_PlayerInstance.GetShape()))
    //{
        //hot_point *MountHotPoint = m_WeaponInstance.GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
        //if (MountHotPoint)
        //{
            //m_WeaponInstance.SetParentInstance(&m_PlayerInstance, HOT_POINT_TYPE_WEAPON_MOUNT) ;
            //m_WeaponInstance.SetPos(-MountHotPoint->GetPos()) ;
        //}
    //}
}

//==============================================================================

// Sets backpack shape
void player_object::SetBackpackShape(shape *BackpackShape)
{
    m_BackpackInstance.SetShape(BackpackShape) ;
}

//==============================================================================

void player_object::SetCharacter(character_type	CharType,
								 armor_type		ArmorType,
                                 skin_type      SkinType,
                                 voice_type     VoiceType)
{
    ASSERT(CharType >= CHARACTER_TYPE_START) ;
    ASSERT(CharType <= CHARACTER_TYPE_END) ;

    ASSERT(ArmorType >= ARMOR_TYPE_START) ;
    ASSERT(ArmorType <= ARMOR_TYPE_END) ;

    ASSERT(SkinType >= SKIN_TYPE_START) ;
    ASSERT(SkinType <= SKIN_TYPE_END) ;

    ASSERT(VoiceType >= VOICE_TYPE_START) ;
    ASSERT(VoiceType <= VOICE_TYPE_END) ;

	// Character vars
	m_CharacterType	= CharType ;
	m_ArmorType		= ArmorType ;
    m_SkinType      = SkinType ;
	m_VoiceType		= VoiceType ;
    
    // Setup fast lookup info...
    m_CharacterInfo = &s_CharacterInfo[m_CharacterType][m_ArmorType] ;
    m_MoveInfo      = &s_MoveInfo[m_CharacterType][m_ArmorType] ;

    // Setup player shapes
    SetPlayerShape	(tgl.GameShapeManager.GetShapeByType(m_CharacterInfo->ShapeType), (s32)SkinType) ;

    // Flag dirty bits
    m_DirtyBits |= PLAYER_DIRTY_BIT_CHARACTER ;
}

//==============================================================================

player_object::character_type player_object::GetCharacterType( void ) const
{
    return m_CharacterType;
}

//==============================================================================

player_object::armor_type player_object::GetArmorType( void ) const
{
    return m_ArmorType;
}

//==============================================================================

s32 player_object::GetVoiceSfxBase( void ) const
{
    return( GetVoiceSfxBase( m_VoiceType ) );
}

//==============================================================================

s32 player_object::GetVoiceSfxBase( voice_type VoiceType )
{
    s32 SfxBase = SFX_VOICE_FEMALE_1;

    switch( VoiceType )
    {
        case VOICE_TYPE_FEMALE_HEROINE:         SfxBase = SFX_VOICE_FEMALE_1 ; break ;
        case VOICE_TYPE_FEMALE_PROFESSIONAL:    SfxBase = SFX_VOICE_FEMALE_2 ; break ;
        case VOICE_TYPE_FEMALE_CADET:           SfxBase = SFX_VOICE_FEMALE_3 ; break ;
        case VOICE_TYPE_FEMALE_VETERAN:         SfxBase = SFX_VOICE_FEMALE_4 ; break ;
        case VOICE_TYPE_FEMALE_AMAZON:          SfxBase = SFX_VOICE_FEMALE_5 ; break ;

        case VOICE_TYPE_MALE_HERO:              SfxBase = SFX_VOICE_MALE_1 ;   break ;
        case VOICE_TYPE_MALE_ICEMAN:            SfxBase = SFX_VOICE_MALE_2 ;   break ;
        case VOICE_TYPE_MALE_ROGUE:             SfxBase = SFX_VOICE_MALE_3 ;   break ;
        case VOICE_TYPE_MALE_HARDCASE:          SfxBase = SFX_VOICE_MALE_4 ;   break ;
        case VOICE_TYPE_MALE_PSYCHO:            SfxBase = SFX_VOICE_MALE_5 ;   break ;

        case VOICE_TYPE_BIO_WARRIOR:            SfxBase = SFX_VOICE_DERM_1 ;   break ;
        case VOICE_TYPE_BIO_MONSTER:            SfxBase = SFX_VOICE_DERM_2 ;   break ;
        case VOICE_TYPE_BIO_PREDATOR:           SfxBase = SFX_VOICE_DERM_3 ;   break ;
    }

    return SfxBase;
}

//==============================================================================

// Puts player as close as possible to given pos, taking collision into account
void player_object::WarpToPos( const vector3& Pos )
{
    // Calc delta movement required from current position
    vector3 Move = Pos - m_WorldPos ;

    // Fall out early due to not enough movement?
    ASSERT(MOVING_EPSILON >= collider::MIN_EXT_DIST) ;
    if (Move.LengthSquared() < (MOVING_EPSILON*MOVING_EPSILON))
        return ;

    // Setup collision types to collide with
    u32 AttrMask = PLAYER_COLLIDE_ATTRS ;

    // Make sure bounds are setup
    CalcBounds() ;

    // Do collision
	collider::collision     Collision ;
    s_Collider.ClearExcludeList() ;
	s_Collider.ExtSetup( this, m_WorldBBox, Move ) ;
    ObjMgr.Collide(AttrMask, s_Collider ) ;

    // Update distance moved if there was a collision
    s_Collider.GetCollision( Collision );
    if (Collision.Collided)
    {
        // Back off from collision plane a tad...
        Collision.T -= MIN_FACE_DISTANCE / Move.Length() ; 
        Collision.T = MAX(0, Collision.T) ;
        ASSERT(Collision.T >= 0) ;
        ASSERT(Collision.T <= 1) ;

        // Update movement to do
        Move *= Collision.T ;
    }

    // Update position and bounds
    SetPos( m_WorldPos + Move ) ;
}

//==============================================================================

// Sets new player position
void player_object::SetPos( const vector3& Pos )
{
    // Update?
    if (m_WorldPos != Pos)
    {
        // Flag it needs sending to clients
        m_DirtyBits |= PLAYER_DIRTY_BIT_POS	;

        // Set new pos
        m_WorldPos = Pos ;

        // Make sure ghost compression pos is up to date
        UpdateGhostCompPos() ;

        // Setup new bounds
        CalcBounds() ;

		// Setup contact info for move/physics
		SetupContactInfo() ;

        // Keep object manager up-to-date
        if (m_ObjectID != obj_mgr::NullID)
            ObjMgr.UpdateObject(this) ;
    }
}

//==============================================================================

// Sets player rotation
void player_object::SetRot( const radian3& Rot )
{
    // For dirty bit setting
    radian3 LastRot = m_Rot ;

    // Set new rot and truncate ready for sending to clients
    m_Rot = Rot ;
    TruncateRot(m_Rot, INPUT_PLAYER_ROT_BITS) ;

    // Update clients?
    if (m_Rot != LastRot)
        m_DirtyBits |= PLAYER_DIRTY_BIT_ROT	;
}

//==============================================================================

// Returns current blended ground rotation
radian3 player_object::GetGroundRot( void ) const
{
    // Start with player yaw
    quaternion  qWorldRot(radian3(0, m_DrawRot.Yaw, 0)) ;

    // Take into account ground rotation? (0=ident, 1=all ground)
    if (m_GroundOrientBlend != 0)
    {
        quaternion qGroundRot   = BlendToIdentity(m_GroundRot, 1-m_GroundOrientBlend) ; // 0 = ident, 1 = ground
        qWorldRot = qGroundRot * qWorldRot ;
    }

    // Extract radian3 rotation
    return qWorldRot.GetRotation() ;
}

//==============================================================================

// Gets world rotation (taking being on a vehicle into account)
void player_object::GetWorldPitchYaw( f32& Pitch, f32& Yaw )
{
    // Start with player rotation
    quaternion  qWorldRot(m_Rot) ;

    // Take into account ground rotation? (0=ident, 1=all ground)
    if (m_GroundOrientBlend != 0)
    {
        quaternion qGroundRot   = BlendToIdentity(m_GroundRot, 1-m_GroundOrientBlend) ; // 0 = ident, 1 = ground
        qWorldRot = qGroundRot * qWorldRot ;
    }

    // Get world directiom
    vector3     WorldDir = vector3(0,0,1) ;
    WorldDir = qWorldRot.Rotate(WorldDir) ;

    // Finally, extract pitch and yaw
    WorldDir.GetPitchYaw( Pitch, Yaw );
}
        
//==============================================================================

// Sets player vel
void player_object::SetVel( const vector3& Vel )
{
    // For dirty bit setting
    vector3 LastVel = m_Vel ;

    // Set new vel and truncate ready for sending to clients
    m_Vel = Vel ;
    TruncateVel(m_Vel, INPUT_PLAYER_VEL_BITS) ;

    // Update clients?
    if (m_Vel != LastVel)
        m_DirtyBits |= PLAYER_DIRTY_BIT_VEL	;
}

//==============================================================================

void player_object::OnAddVelocity( const vector3& Velocity )
{
    (void)Velocity ;

    #if 1   // SB. Removed for now since it makes the players bounce of vehicles like rubber! 

    // Skip if connecting
    if (m_PlayerState == PLAYER_STATE_CONNECTING)
        return ;

    // Skip if dead
    if (m_Health == 0)
        return ;

    // Set new vel
    SetVel(m_Vel + Velocity) ;
    #endif
}

//==============================================================================

f32 player_object::GetMass( void ) const
{
    // Start with players mass
    f32 Mass = m_MoveInfo->MASS ;

    // Add pack mass
    Mass += s_InventInfo[m_PackCurrentType].Mass ;

    // Add weapon mass
    Mass += s_InventInfo[m_WeaponCurrentType].Mass ;

/*
    // Flags are 55
    if (HasFlag())
        Mass += 55 ;
*/

    return Mass ;
}

//==============================================================================

void player_object::UpdateViewAndWeaponFirePos( void )
{
    DEBUG_BEGIN_TIMER(UpdateViewAndWeaponFirePos)

    s32 i ;

    //==========================================================================
    // Setup player and weapon rotations (takes ground/vehicle rotation into account)
    //==========================================================================
    
    DEBUG_BEGIN_TIMER(SetupPlayerAndWeaponRot)

    // Player only has yaw
    m_PlayerRot     = radian3(0, m_Rot.Yaw, 0) ;
    
    // Weapon has pitch and yaw
    m_WeaponRot     = m_Rot ;

    // If player is sat in a seat, then he cannot spin around, so disable the player draw rotation
    if (IsSatDownInVehicle())
        m_PlayerRot.Zero() ;

    // Take into account ground rotation? (0=ident, 1=all ground)
    if (m_GroundOrientBlend != 0)
    {
        quaternion qGroundRot   = BlendToIdentity(m_GroundRot, 1-m_GroundOrientBlend) ; // 0 = ident, 1 = ground

        m_PlayerRot     = (qGroundRot * quaternion(m_PlayerRot)).GetRotation() ;

        m_WeaponRot     = (qGroundRot * quaternion(m_WeaponRot)).GetRotation() ;
    }

    DEBUG_END_TIMER(SetupPlayerAndWeaponRot)

    //==========================================================================
    // Update light position
    //==========================================================================

    DEBUG_BEGIN_TIMER(SetGlowPos)

    if( m_Glow )
    {
        vector3 Position = m_DrawPos;
        Position.Y  += 1.0f;
        ptlight_SetPosition( m_PointLight, Position );
    }

    DEBUG_END_TIMER(SetGlowPos)

    //==========================================================================
	// Calculate view and weapon pos lookup info
    //==========================================================================

    DEBUG_BEGIN_TIMER(CalcViewAndWeaponInfo)

    // Lookup view index
    s32 ViewIndex = 0 ;
    if (m_Armed)
    {
        switch(m_WeaponCurrentType)
        {
            default:                                    ViewIndex = 0 ; break ;
            case INVENT_TYPE_WEAPON_SNIPER_RIFLE:       ViewIndex = 1 ; break ;
            case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:   ViewIndex = 2 ; break ;
        }
    }

    // Setup table indices
    vector3* ViewPosTable   = s_ViewPosLookup  [m_CharacterType][m_ArmorType][ViewIndex] ;
    vector3* WeaponPosTable = s_WeaponPosLookup[m_CharacterType][m_ArmorType][ViewIndex] ;

    // Calculate pitch as a value from 0 (up - VIEW_MIN_PITCH) to 1 (down - VIEW_MAX_PITCH)
    f32 Pitch = m_Rot.Pitch + (m_BlendMove.Blend * m_BlendMove.DeltaRot.Pitch) ;
    Pitch = (Pitch - s_CommonInfo.VIEW_MIN_PITCH) / (s_CommonInfo.VIEW_MAX_PITCH - s_CommonInfo.VIEW_MIN_PITCH) ;
    if (Pitch < 0.0f)
        Pitch = 0.0f ;
    else
    if (Pitch > 1.0f)
        Pitch = 1.0f ;

    // Convert to range of lookup table
    Pitch *= 31.0f ;

    // Setup integer frames to look at
    s32 A,B ;
    f32 Frac ;
    A = (s32)Pitch ;
    if (A == 31)
    {
        B    = A ;
        Frac = 0 ;
    }
    else
    {
        B = A + 1 ;
        Frac = Pitch - (f32)A ;
    }

    DEBUG_END_TIMER(CalcViewAndWeaponInfo)
    
    //==========================================================================
    // Setup view pos
    //==========================================================================

    DEBUG_BEGIN_TIMER(SetupViewPos)

    // Calculate view pos from look rot
    BlendLinear(ViewPosTable[A], ViewPosTable[B], Frac, m_ViewPos) ;
    m_ViewPos.Rotate(m_PlayerRot) ;
    m_ViewPos += m_WorldPos ;

    // Calculate a start position within the player
    vector3 Start = m_WorldPos ;
    Start.Y = m_ViewPos.Y ;
    for (i = 0 ; i < 3 ; i++)
    {
        f32 Min = m_WorldBBox.Min[i] + s_CommonInfo.VIEW_BACKOFF_DIST ;
        f32 Max = m_WorldBBox.Max[i] - s_CommonInfo.VIEW_BACKOFF_DIST ;
        if (Start[i] < Min)
            Start[i] = Min ;
        if (Start[i] > Max)
            Start[i] = Max ;
    }

    // Make sure view is valid
    SetupValidViewPos(Start, m_ViewPos) ;

    DEBUG_END_TIMER(SetupViewPos)

    //==========================================================================
    // Setup weapon fire pos info
    //==========================================================================

    DEBUG_BEGIN_TIMER(SetupFirePos)

    // Calculate weapon position
    vector3 WeaponPos ;
    BlendLinear(WeaponPosTable[A], WeaponPosTable[B], Frac, WeaponPos) ;
    WeaponPos.Rotate(m_PlayerRot) ;
    WeaponPos += m_WorldPos ;

    // Keep the weapon position (the bots use it)
    m_WeaponPos = WeaponPos ;

    // Calculate weapon fire position
    vector3 WeaponFirePos = WeaponPos ;
    model* Model = m_WeaponInstance.GetModel() ;
    if (Model)
    {
        // Lookup mount + fire hot points
        hot_point* FireHotPoint  = Model->GetHotPointByType(HOT_POINT_TYPE_FIRE_WEAPON) ;
        hot_point* MountHotPoint = Model->GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
        ASSERT(FireHotPoint) ;
        ASSERT(MountHotPoint) ;

        // Get fire offset, relative to mount pos
        vector3 FireOffset = FireHotPoint->GetPos() - MountHotPoint->GetPos() ;
        FireOffset.Rotate(m_WeaponRot) ;
       
        // Add fire offset
        WeaponFirePos += FireOffset ;
    }    

    // Keep rotation
    m_WeaponFireRot = m_WeaponRot ;
    
    //==========================================================================
    // Pull weapon back if it intersects something
    //==========================================================================

    if (Model)
    {
        // Okay - we have the ideal weapon pos - now let's do some collision to see if we can put it here
        collider::collision Collision ;

        // Lookup mount + fire hot points
        hot_point* FireHotPoint  = Model->GetHotPointByType(HOT_POINT_TYPE_FIRE_WEAPON) ;
        hot_point* MountHotPoint = Model->GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
        ASSERT(FireHotPoint) ;
        ASSERT(MountHotPoint) ;

        // The fire point may not be at the end of the weapon (eg. disk launcher)
        // so take the geometry into account and keep the longest z value
        vector3  EndPos     = FireHotPoint->GetPos() ;
        bbox     WeaponBBox = Model->GetBounds() ;
        EndPos.Z = MAX(EndPos.Z, WeaponBBox.Max.Z) ;
        EndPos -= MountHotPoint->GetPos() ;

        // Put into world space
        EndPos.Rotate(m_WeaponRot) ;
        EndPos += WeaponPos ;

        // Calc world space start pos
        vector3 StartPos = vector3(0,0,-1.3f) ;
        StartPos.Rotate(m_WeaponRot) ;
        StartPos += EndPos ;

        // Setup radius of biggest projectile the player can fire
        // (0.125f is default arcing radius for grenades, mortars etc)
        f32 MaxProjectileRadius = 0.15f ;

        // Make sure the start pos is inside the player bbox since bots have a smaller
        // bounding box and the weapon maybe already through the wall! (eg. a heavy)
        for (i = 0 ; i < 3 ; i++)
        {
            f32 Min = m_WorldBBox.Min[i] + MaxProjectileRadius ;
            f32 Max = m_WorldBBox.Max[i] - MaxProjectileRadius ;
            if (StartPos[i] < Min)
                StartPos[i] = Min ;
            if (StartPos[i] > Max)
                StartPos[i] = Max ;
        }

        // Do extrusion from start to end using biggest projectile radius
        bbox    BBox(StartPos, MaxProjectileRadius) ;
        vector3  Move = EndPos - StartPos ;
        s_Collider.ClearExcludeList() ;
        s_Collider.ExtSetup( this, BBox, Move ) ;
        ObjMgr.Collide(WEAPON_COLLIDE_ATTRS, s_Collider ) ;
        s_Collider.GetCollision( Collision );
        if (Collision.Collided)
            m_WeaponPullBackOffset = - (1 - Collision.T) * Move ;
        else
            m_WeaponPullBackOffset.Zero() ;

        // Record bbox's for player render debug collision code
        #ifdef DRAW_VIEW_AND_WEAPON_FIRE_POS
            PlayerWeaponStartBBox = BBox ;
            PlayerWeaponEndBBox   = BBox ;
            if (Collision.Collided)
                PlayerWeaponEndBBox.Translate(Move * Collision.T) ;
            else
                PlayerWeaponEndBBox.Translate(Move) ;
        #endif
    }

    DEBUG_END_TIMER(SetupFirePos)

    //==========================================================================
    // Finally, setup fire L2W matrix
    //==========================================================================
    m_WeaponFireL2W.Identity() ;
    m_WeaponFireL2W.SetTranslation(WeaponFirePos + m_WeaponPullBackOffset) ;
    m_WeaponFireL2W.SetRotation(m_WeaponRot) ;

    DEBUG_END_TIMER(UpdateViewAndWeaponFirePos)
}

//==============================================================================

const vector3& player_object::GetViewPos( void ) const
{
    return m_ViewPos ;
}

//==============================================================================

const vector3& player_object::GetWeaponPos( void ) const
{
    return m_WeaponPos ;
}

//==============================================================================

const matrix4& player_object::GetWeaponFireL2W( void ) const
{
    return m_WeaponFireL2W ;
}

//==============================================================================

const vector3 player_object::GetWeaponFirePos( void ) const
{
    return m_WeaponFireL2W.GetTranslation() ;
}

//==============================================================================

const radian3& player_object::GetWeaponFireRot( void ) const
{
    return m_WeaponFireRot ;
}

//==============================================================================

const vector3 player_object::GetWeaponFireDir( void ) const
{
    vector3 Dir ;

    // Get the Z axis direction
    Dir.X = m_WeaponFireL2W(2,0) ;
    Dir.Y = m_WeaponFireL2W(2,1) ;
    Dir.Z = m_WeaponFireL2W(2,2) ;

    return Dir ;
}

//==============================================================================

vector3 player_object::GetBlendFirePos( s32 Index )
{         
    (void)Index;
    ASSERT( Index == 0 );
    vector3 Pos = m_WeaponFireL2W.GetTranslation();
    Pos -= GetPosition();
    Pos += GetBlendPos();
    return( Pos );
}

//==============================================================================

const matrix4 player_object::GetTweakedWeaponFireL2W( void ) const
{
    vector3     ViewPos;
    radian3     ViewRot;
    f32         ViewFOV;
    radian3     Orient;
    matrix4     L2W;
    vector3     ViewZ;
    vector3     Start;
    vector3     End;

    GetView( ViewPos, ViewRot, ViewFOV );
    ViewZ.Set( ViewRot.Pitch, ViewRot.Yaw );        

    xbool GotPoint = FALSE;

    if( UseAutoAimHelp )
    {
        vector3     Dir;
        object::id  TargetID;
        GotPoint = GetAutoAimPoint( (const player_object*)this, Dir, End, TargetID );
    }
    
    // If we do not have an auto aim target (for whatever reason), then fire
    // from the tip of the weapon to where the line of sight hits something 
    // solid.
    if( !GotPoint )
    {
        End = ViewPos + (ViewZ * 1000.0f);
        
        s_Collider.ClearExcludeList() ;
        s_Collider.RaySetup  ( NULL, ViewPos, End, m_ObjectID.GetAsS32() );
        ObjMgr.Collide( ATTR_SOLID, s_Collider );
    
        // Move the end point up if we hit something.
        if( s_Collider.HasCollided() )
        {
            collider::collision Collision;
            s_Collider.GetCollision( Collision );
            End = Collision.Point;
        }
    }

    // Establish orientation based on where we are shooting.
    Start = m_WeaponFireL2W.GetTranslation();
    End  -= Start;
    End.GetPitchYaw( Orient.Pitch, Orient.Yaw );
    Orient.Roll = m_WeaponFireRot.Roll;
    
    L2W.Zero();
    L2W.SetRotation( Orient );
    L2W.SetTranslation( Start );
    
    return( L2W );
}

//==============================================================================

void player_object::SetDamageWiggle( f32 Time, f32 Radius )
{
    m_DirtyBits |= PLAYER_DIRTY_BIT_WIGGLE	;
    m_DamageTotalWiggleTime   = Time;
    m_DamageWiggleRadius      = Radius;
    m_DamageWiggleTime        = 0.0f;
}
        
//==============================================================================

// Sets up bounds of input box, given world position
void player_object::SetupBounds( const vector3& Pos, bbox& BBox )
{
    // Grab default bbox
    BBox = m_MoveInfo->BOUNDING_BOX ;

    // Always use set with and length for bots so they can pathfind easily
    if (GetType() == object::TYPE_BOT)
    {
        BBox.Min.X = BBox.Min.Z = -0.5f ;
        BBox.Max.X = BBox.Max.Z = 0.5f ;
    }

    // Put around pos
    BBox.Translate(Pos) ;
}

//==============================================================================

// Updates player bounds
void player_object::CalcBounds()
{
    SetupBounds( m_WorldPos, m_WorldBBox ) ;
}

//==============================================================================

// Returns TRUE if player is outside of the game bounds, otherwise returns FALSE
xbool player_object::IsOutOfBounds( void )
{
    bbox GameBBox = GameMgr.GetBounds() ;

    // Only check lateral bounds.  Do NOT check Y.
    return( (m_WorldPos.X < GameBBox.Min.X) ||
            (m_WorldPos.X > GameBBox.Max.X) ||
            (m_WorldPos.Z < GameBBox.Min.Z) ||
            (m_WorldPos.Z > GameBBox.Max.Z) );
}

//==============================================================================

void DisableStick( u32 DisableMask, f32& StickValue )
{
    if( DisableMask & 0x2 )
    {
        if( StickValue < 0 )
            StickValue = 0;
    }
    
    if( DisableMask & 0x1 )
    {
        if( StickValue > 0 )
            StickValue = 0;
    }
}

//==============================================================================

extern xbool DO_SCREEN_SHOTS;

// Reads input and sets up player input structure
void player_object::CollectInput( f32 DeltaTime )
{
    // Constructor clears out
    player_input    CurrentInput ;
    f32             LookSpeed = 1.0f ;

    // Lookup control info
    const player_object::control_info& ControlInfo = GetControlInfo() ;

    // Add machine specific checks here...
    #ifdef TARGET_PS2
        //xbool PS2_Connected = input_IsPressed(INPUT_PS2_QRY_PAD_PRESENT,   m_ControllerID) ;
        xbool PS2_Connected = TRUE ;
        xbool MSE_Connected = input_IsPressed(INPUT_PS2_QRY_MOUSE_PRESENT, m_ControllerID) ;
        xbool KBD_Connected = input_IsPressed(INPUT_PS2_QRY_KBD_PRESENT,   m_ControllerID) ;
    #endif

    #ifdef TARGET_PC
        // For now - check all
        xbool PS2_Connected = FALSE ;   // No PS2 joystick for the PC!
        xbool MSE_Connected = TRUE ;
        xbool KBD_Connected = TRUE ;
    #endif


    //=================================================================
    // CHARACTER TESTING INPUT
    //=================================================================

    #ifdef sbroumley
    {
        xbool NextTypePressed = FALSE ;

        // PC
        NextTypePressed |=     ((input_IsPressed( INPUT_KBD_LSHIFT, m_ControllerID )) || (input_IsPressed ( INPUT_KBD_RSHIFT, m_ControllerID ))) 
                            &&  (input_WasPressed( INPUT_KBD_C, m_ControllerID )) ; 

        // PS2
        NextTypePressed |=      (       (input_IsPressed( INPUT_PS2_BTN_L1, m_ControllerID ))
                                &&  (input_IsPressed( INPUT_PS2_BTN_L2, m_ControllerID ))
                                &&  (input_IsPressed( INPUT_PS2_BTN_R1, m_ControllerID ))
                                &&  (input_IsPressed( INPUT_PS2_BTN_R2, m_ControllerID )) )
                        &&  (       (input_WasPressed( INPUT_PS2_BTN_L1, m_ControllerID ))
                                ||  (input_WasPressed( INPUT_PS2_BTN_L2, m_ControllerID ))
                                ||  (input_WasPressed( INPUT_PS2_BTN_R1, m_ControllerID ))
                                ||  (input_WasPressed( INPUT_PS2_BTN_R2, m_ControllerID )) ) ;

        if (NextTypePressed)
        {
            static s32 s_CharType  = CHARACTER_TYPE_START ;
            static s32 s_ArmorType = ARMOR_TYPE_START ;
            static s32 s_SkinType  = SKIN_TYPE_START ;

            m_CharacterType  = (character_type)s_CharType ;
            m_ArmorType      = (armor_type)s_ArmorType ;
            m_SkinType       = (skin_type)s_SkinType ;

            // Next character for next time
            if (++s_ArmorType > ARMOR_TYPE_END)
            {
                s_ArmorType = ARMOR_TYPE_START ;
                if (++s_CharType > CHARACTER_TYPE_END)
                {
                    s_CharType = CHARACTER_TYPE_START ;
                    s_SkinType++ ;
                    if (s_SkinType > SKIN_TYPE_END)
                        s_SkinType = SKIN_TYPE_START ;
                }
            }

            SetCharacter(m_CharacterType, m_ArmorType, m_SkinType, m_VoiceType ) ;
        }        
    }
    #endif

    //=================================================================
    // CHECK PS2 CONTROLLER INPUT
    //=================================================================

    if (PS2_Connected)
    {
        xbool enabled;
        
        // Disable any force feedback if in monkey mode, or in freecam or if it's already
        // been disabled in the current warrior setup.
        enabled = (!GetManiacMode()) && 
                  (tgl.UseFreeCam[m_ControllerID]==0) && 
                  (fegl.WarriorSetups[m_WarriorID].DualshockEnabled);
        
        input_EnableFeedback(enabled ,m_ControllerID);

/*
        // Watch for monkey mode (hold select, L-stick button, and R-stick button)
        if( (input_IsPressed(INPUT_PS2_BTN_SELECT, m_ControllerID)) && 
            (input_IsPressed(INPUT_PS2_BTN_L_STICK, m_ControllerID)) &&
            (input_IsPressed(INPUT_PS2_BTN_R_STICK, m_ControllerID)) &&
            ( (input_WasPressed(INPUT_PS2_BTN_L_STICK, m_ControllerID)) || (input_WasPressed(INPUT_PS2_BTN_R_STICK, m_ControllerID)) ) )
        {
             m_InMonkeyMode = !m_InMonkeyMode;
             if (m_InMonkeyMode)
                x_printf("Monkey enabled\n\n\n\n") ;
             else
                x_printf("Monkey disabled\n\n\n\n") ;
        }
*/

        // Movement keys
        CurrentInput.FireKey        |= ControlInfo.PS2_FIRE.IsPressed(m_ControllerID) ;
        CurrentInput.JetKey         |= ControlInfo.PS2_JET.IsPressed(m_ControllerID) ;
        CurrentInput.JumpKey        |= ControlInfo.PS2_JUMP.IsPressed(m_ControllerID) ;
        CurrentInput.ZoomKey        |= ControlInfo.PS2_ZOOM.IsPressed(m_ControllerID) ;
        CurrentInput.FreeLookKey    |= ControlInfo.PS2_FREE_LOOK.IsPressed(m_ControllerID) ;
        
        // Change keys
        CurrentInput.NextWeaponKey  |= ControlInfo.PS2_NEXT_WEAPON.IsPressed(m_ControllerID) ;
        CurrentInput.ZoomInKey      |= ControlInfo.PS2_ZOOM_IN.IsPressed(m_ControllerID) ;
        CurrentInput.ZoomOutKey     |= ControlInfo.PS2_ZOOM_OUT.IsPressed(m_ControllerID) ;
        CurrentInput.ViewChangeKey  |= ControlInfo.PS2_VIEW_CHANGE.IsPressed(m_ControllerID) ;

        // Use keys
        CurrentInput.MineKey        |= ControlInfo.PS2_MINE.IsPressed(m_ControllerID) ;
        CurrentInput.GrenadeKey     |= ControlInfo.PS2_GRENADE.IsPressed(m_ControllerID) ;
        CurrentInput.PackKey        |= ControlInfo.PS2_USE_PACK.IsPressed(m_ControllerID) ;
        CurrentInput.HealthKitKey   |= ControlInfo.PS2_HEALTH_KIT.IsPressed(m_ControllerID) ;
        CurrentInput.TargetingLaserKey |= ControlInfo.PS2_TARGETING_LASER.IsPressed(m_ControllerID) ;

        // Drop keys
        CurrentInput.DropWeaponKey  |= ControlInfo.PS2_DROP_WEAPON.IsPressed(m_ControllerID) ;
        CurrentInput.DropPackKey    |= ControlInfo.PS2_DROP_PACK.IsPressed(m_ControllerID) ;
        CurrentInput.DropBeaconKey  |= ControlInfo.PS2_DROP_BEACON.IsPressed(m_ControllerID) ;
        CurrentInput.DropFlagKey    |= ControlInfo.PS2_DROP_FLAG.IsPressed(m_ControllerID) ;

        // Special keys
        CurrentInput.FocusKey       |= ControlInfo.PS2_FOCUS.IsPressed(m_ControllerID) ;
        CurrentInput.SuicideKey     |= ControlInfo.PS2_SUICIDE.IsPressed(m_ControllerID) ;
        CurrentInput.VoiceMenuKey   |= ControlInfo.PS2_VOICE_MENU.IsPressed(m_ControllerID) ;
        CurrentInput.OptionsMenuKey |= ControlInfo.PS2_OPTIONS.IsPressed(m_ControllerID) ;
        CurrentInput.TargetLockKey  |= ControlInfo.PS2_TARGET_LOCK.IsPressed(m_ControllerID) ;
        CurrentInput.ComplimentKey  |= ControlInfo.PS2_COMPLIMENT.IsPressed(m_ControllerID) ;
        CurrentInput.TauntKey       |= ControlInfo.PS2_TAUNT.IsPressed(m_ControllerID) ;
        CurrentInput.ExchangeKey    |= ControlInfo.PS2_EXCHANGE.IsPressed(m_ControllerID) ;
        CurrentInput.SpecialJumpKey |= ControlInfo.PS2_JUMP.IsPressed(m_ControllerID) ;

        // SCREEN SHOT??
        //if (ControlInfo.PS2_FREE_LOOK.IsPressed(m_ControllerID))
        //{
            //if( !DO_SCREEN_SHOTS )
            //{
                //CurrentInput.TauntKey = TRUE ;
            //}
        //}


        // Movement special
        if (ControlInfo.PS2_ZOOM.IsPressed(m_ControllerID))
        {
            f32 Scale = ControlInfo.PS2_LOOK_ZOOM_SPEED_SCALAR;
            
            vehicle* pVehicle = IsVehiclePilot();
            
            if( pVehicle )
            {
                if( pVehicle->GetType() == object::TYPE_VEHICLE_THUNDERSWORD )
                {
                    Scale = 1.0f;
                }
            }

            LookSpeed *= Scale;
        }

        f32 LookSpeedX = LookSpeed;
        f32 LookSpeedY = LookSpeed;

        // If player is NOT a pilot, then we can use the pad sensitivity settings
        if( IsVehiclePilot() == NULL )
        {
            // Scale look speed
            LookSpeedX *= ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED;
            LookSpeedY *= ControlInfo.PS2_LOOK_UP_DOWN_SPEED;
        }

        // Move left-right
        CurrentInput.MoveX -= PLAYER_DEBUG_SPEED * input_GetValue(ControlInfo.PS2_MOVE_LEFT_RIGHT.MainGadget, m_ControllerID) ;

        // Move forward-backward
        CurrentInput.MoveY += PLAYER_DEBUG_SPEED * input_GetValue(ControlInfo.PS2_MOVE_FORWARD_BACKWARD.MainGadget, m_ControllerID) ;

        // Look left-right
        CurrentInput.LookX -= PLAYER_DEBUG_SPEED * input_GetValue(ControlInfo.PS2_LOOK_LEFT_RIGHT.MainGadget, m_ControllerID) * LookSpeedX ;

        // Look up-down
        CurrentInput.LookY += PLAYER_DEBUG_SPEED * input_GetValue(ControlInfo.PS2_LOOK_UP_DOWN.MainGadget, m_ControllerID) * LookSpeedY ;
    }
 
    //=================================================================
    // CHECK KEYBOARD INPUT
    //=================================================================

    if (KBD_Connected)
    {
/*
        // Watch for monkey mode
        if( (input_IsPressed( INPUT_KBD_LCONTROL, m_ControllerID) || input_IsPressed( INPUT_KBD_RCONTROL, m_ControllerID)) &&
             input_WasPressed( INPUT_KBD_M, m_ControllerID ) )
             m_InMonkeyMode = !m_InMonkeyMode;
*/

        // Movement keys
        CurrentInput.FireKey        |= ControlInfo.KBD_FIRE.IsPressed(m_ControllerID) ;
        CurrentInput.JetKey         |= ControlInfo.KBD_JET.IsPressed(m_ControllerID) ;
        CurrentInput.JumpKey        |= ControlInfo.KBD_JUMP.IsPressed(m_ControllerID) ;
        CurrentInput.ZoomKey        |= ControlInfo.KBD_ZOOM.IsPressed(m_ControllerID) ;
        CurrentInput.FreeLookKey    |= ControlInfo.KBD_FREE_LOOK.IsPressed(m_ControllerID) ;
        
        // Change keys
        CurrentInput.NextWeaponKey  |= ControlInfo.KBD_NEXT_WEAPON.IsPressed(m_ControllerID) ;
        CurrentInput.PrevWeaponKey  |= ControlInfo.KBD_PREV_WEAPON.IsPressed(m_ControllerID) ;
        CurrentInput.ZoomInKey      |= ControlInfo.KBD_ZOOM_IN.IsPressed(m_ControllerID) ;
        CurrentInput.ZoomOutKey     |= ControlInfo.KBD_ZOOM_OUT.IsPressed(m_ControllerID) ;
        CurrentInput.ViewChangeKey  |= ControlInfo.KBD_VIEW_CHANGE.IsPressed(m_ControllerID) ;

        // Use keys
        CurrentInput.MineKey        |= ControlInfo.KBD_MINE.IsPressed(m_ControllerID) ;
        CurrentInput.GrenadeKey     |= ControlInfo.KBD_GRENADE.IsPressed(m_ControllerID) ;
        CurrentInput.PackKey        |= ControlInfo.KBD_USE_PACK.IsPressed(m_ControllerID) ;
        CurrentInput.HealthKitKey   |= ControlInfo.KBD_HEALTH_KIT.IsPressed(m_ControllerID) ;
        CurrentInput.TargetingLaserKey |= ControlInfo.KBD_TARGETING_LASER.IsPressed(m_ControllerID) ;

        // Drop keys
        CurrentInput.DropWeaponKey  |= ControlInfo.KBD_DROP_WEAPON.IsPressed(m_ControllerID) ;
        CurrentInput.DropPackKey    |= ControlInfo.KBD_DROP_PACK.IsPressed(m_ControllerID) ;
        CurrentInput.DropBeaconKey  |= ControlInfo.KBD_DROP_BEACON.IsPressed(m_ControllerID) ;
        CurrentInput.DropFlagKey    |= ControlInfo.KBD_DROP_FLAG.IsPressed(m_ControllerID) ;

        // Special keys
        CurrentInput.FocusKey       |= ControlInfo.KBD_FOCUS.IsPressed(m_ControllerID) ;
        CurrentInput.SuicideKey     |= ControlInfo.KBD_SUICIDE.IsPressed(m_ControllerID) ;
        CurrentInput.VoiceMenuKey   |= ControlInfo.KBD_VOICE_MENU.IsPressed(m_ControllerID) ;
        CurrentInput.OptionsMenuKey |= ControlInfo.KBD_OPTIONS.IsPressed(m_ControllerID) ;
        CurrentInput.TargetLockKey  |= ControlInfo.KBD_TARGET_LOCK.IsPressed(m_ControllerID) ;
        CurrentInput.ComplimentKey  |= ControlInfo.KBD_COMPLIMENT.IsPressed(m_ControllerID) ;
        CurrentInput.TauntKey       |= ControlInfo.KBD_TAUNT.IsPressed(m_ControllerID) ;
        CurrentInput.ExchangeKey    |= ControlInfo.KBD_EXCHANGE.IsPressed(m_ControllerID) ;
        CurrentInput.SpecialJumpKey |= ControlInfo.KBD_EXCHANGE.IsPressed(m_ControllerID) ;

        // Special Case: Mines are now a type of grenade
        if( GetInventCount( INVENT_TYPE_MINE ) > 0 )
        {
            if( m_InputMask.GrenadeKey == 0 )
            {
                CurrentInput.MineKey   |= ControlInfo.KBD_GRENADE.IsPressed(m_ControllerID) ; 
                CurrentInput.GrenadeKey = 0;
            }
        }

        // Movement special
        if (ControlInfo.KBD_ZOOM.IsPressed(m_ControllerID))
            LookSpeed *= ControlInfo.KBD_LOOK_ZOOM_SPEED_SCALAR ;

        // Move left-right
        if (ControlInfo.KBD_MOVE_LEFT.IsPressed(m_ControllerID))
            CurrentInput.MoveX += PLAYER_DEBUG_SPEED * 1.0f ;

        if (ControlInfo.KBD_MOVE_RIGHT.IsPressed(m_ControllerID))
            CurrentInput.MoveX -= PLAYER_DEBUG_SPEED * 1.0f ;

        // Move forward-backward
        if (ControlInfo.KBD_MOVE_FORWARD.IsPressed(m_ControllerID))
            CurrentInput.MoveY += PLAYER_DEBUG_SPEED * 1.0f ;
        
        if (ControlInfo.KBD_MOVE_BACKWARD.IsPressed(m_ControllerID))
            CurrentInput.MoveY -= PLAYER_DEBUG_SPEED * 1.0f ;

        // Look left-right
        if (ControlInfo.KBD_LOOK_LEFT.IsPressed(m_ControllerID))
            CurrentInput.LookX += ControlInfo.KBD_LOOK_LEFT_RIGHT_SPEED * LookSpeed ;

        if (ControlInfo.KBD_LOOK_RIGHT.IsPressed(m_ControllerID))
            CurrentInput.LookX -= ControlInfo.KBD_LOOK_LEFT_RIGHT_SPEED * LookSpeed ;

        // Look up-down
        if (ControlInfo.KBD_LOOK_UP.IsPressed(m_ControllerID))
            CurrentInput.LookY -= ControlInfo.KBD_LOOK_UP_DOWN_SPEED * LookSpeed ;

        if (ControlInfo.KBD_LOOK_DOWN.IsPressed(m_ControllerID))
            CurrentInput.LookY += ControlInfo.KBD_LOOK_UP_DOWN_SPEED * LookSpeed ;
    }

    //=================================================================
    // CHECK MOUSE INPUT
    //=================================================================

    // Setup the correct mouse mode
    #ifdef TARGET_PC
        if( m_HasInputFocus )
        {
            // If there dialogs, make sure the mouse is in absolute mode,
            // otherwise put into always mode for game input
            if (tgl.pUIManager->GetNumUserDialogs( m_UIUserID ) > 0)
                d3deng_SetMouseMode( MOUSE_MODE_ABSOLUTE ) ;
            else
                d3deng_SetMouseMode( MOUSE_MODE_ALWAYS ) ;
        }
        else
        {
            // Put into debug mode...
            d3deng_SetMouseMode( MOUSE_MODE_BUTTONS );
        }
    #endif

    // Only process mouse if there are no dialogs
    if ((MSE_Connected) && (tgl.pUIManager->GetNumUserDialogs( m_UIUserID ) == 0))
    {
        // Movement
        CurrentInput.FireKey |= ControlInfo.MSE_FIRE.IsPressed(m_ControllerID) ;
        CurrentInput.JetKey  |= ControlInfo.MSE_JET.IsPressed(m_ControllerID) ;

        f32 LookSpeedX = LookSpeed;
        f32 LookSpeedY = LookSpeed;
        
        // If player is NOT in a vehicle then we can use the pad sensitivity settings
        if( IsVehiclePilot() == NULL )
        {
            LookSpeedX *= ControlInfo.MSE_LOOK_LEFT_RIGHT_SPEED;
            LookSpeedY *= ControlInfo.MSE_LOOK_UP_DOWN_SPEED;
        }

        // Look left-right
        CurrentInput.LookX -= input_GetValue(ControlInfo.MSE_LOOK_LEFT_RIGHT.MainGadget, m_ControllerID) * LookSpeedX ;

        // Look up-down
        CurrentInput.LookY += input_GetValue(ControlInfo.MSE_LOOK_UP_DOWN.MainGadget, m_ControllerID) * LookSpeedY ;
    }

#ifdef T2_DESIGNER_BUILD
        // Disable Shooting
        CurrentInput.FireKey = CurrentInput.MineKey = CurrentInput.GrenadeKey = 0;
#endif


    // Special Case: Mines are now a type of grenade
    if( GetInventCount( INVENT_TYPE_MINE ) > 0 )
    {
        if( m_InputMask.GrenadeKey == 0 )
        {
            CurrentInput.MineKey   |= CurrentInput.GrenadeKey ;
            CurrentInput.GrenadeKey = 0;
        }
    }

    //=================================================================
    // Apply input mask
    //=================================================================

    // Movement keys
    CurrentInput.FireKey             = (!m_InputMask.FireKey)            && (CurrentInput.FireKey) ;
    CurrentInput.JetKey              = (!m_InputMask.JetKey)             && (CurrentInput.JetKey) ;
    CurrentInput.JumpKey             = (!m_InputMask.JumpKey)            && (CurrentInput.JumpKey) ;
    CurrentInput.ZoomKey             = (!m_InputMask.ZoomKey)            && (CurrentInput.ZoomKey) ;
    CurrentInput.FreeLookKey         = (!m_InputMask.FreeLookKey)        && (CurrentInput.FreeLookKey) ;
                                                                        
    // Change keys                                                      
    CurrentInput.NextWeaponKey       = (!m_InputMask.NextWeaponKey)      && (CurrentInput.NextWeaponKey) ;
    CurrentInput.PrevWeaponKey       = (!m_InputMask.PrevWeaponKey)      && (CurrentInput.PrevWeaponKey) ;
    CurrentInput.ZoomInKey           = (!m_InputMask.ZoomInKey)          && (CurrentInput.ZoomInKey) ;
    CurrentInput.ZoomOutKey          = (!m_InputMask.ZoomOutKey)         && (CurrentInput.ZoomOutKey) ;
    CurrentInput.ViewChangeKey       = (!m_InputMask.ViewChangeKey)      && (CurrentInput.ViewChangeKey) ;
                                                                        
    // Use keys                                                         
    CurrentInput.MineKey             = (!m_InputMask.MineKey)            && (CurrentInput.MineKey) ;
    CurrentInput.PackKey             = (!m_InputMask.PackKey)            && (CurrentInput.PackKey) ;
    CurrentInput.GrenadeKey          = (!m_InputMask.GrenadeKey)         && (CurrentInput.GrenadeKey) ;
    CurrentInput.HealthKitKey        = (!m_InputMask.HealthKitKey)       && (CurrentInput.HealthKitKey) ;
                                                                        
    // Drop keys                                                        
    CurrentInput.DropWeaponKey       = (!m_InputMask.DropWeaponKey)      && (CurrentInput.DropWeaponKey) ;
    CurrentInput.DropPackKey         = (!m_InputMask.DropPackKey)        && (CurrentInput.DropPackKey) ;
    CurrentInput.DropBeaconKey       = (!m_InputMask.DropBeaconKey)      && (CurrentInput.DropBeaconKey) ;
    CurrentInput.DropFlagKey         = (!m_InputMask.DropFlagKey)        && (CurrentInput.DropFlagKey) ;
                                        
    // Special keys                 
    CurrentInput.FocusKey            = (!m_InputMask.FocusKey)           && (CurrentInput.FocusKey) ;
    CurrentInput.SuicideKey          = (!m_InputMask.SuicideKey)         && (CurrentInput.SuicideKey) ;
    CurrentInput.VoiceMenuKey        = (!m_InputMask.VoiceMenuKey)       && (CurrentInput.VoiceMenuKey) ;
    CurrentInput.OptionsMenuKey      = (!m_InputMask.OptionsMenuKey)     && (CurrentInput.OptionsMenuKey) ;
    CurrentInput.TargetingLaserKey   = (!m_InputMask.TargetingLaserKey)  && (CurrentInput.TargetingLaserKey) ;
    CurrentInput.TargetLockKey       = (!m_InputMask.TargetLockKey)      && (CurrentInput.TargetLockKey) ;
    CurrentInput.ComplimentKey       = (!m_InputMask.ComplimentKey)      && (CurrentInput.ComplimentKey) ;
    CurrentInput.TauntKey            = (!m_InputMask.TauntKey)           && (CurrentInput.TauntKey) ;
    CurrentInput.ExchangeKey         = (!m_InputMask.ExchangeKey)        && (CurrentInput.ExchangeKey) ;

	// Sticks
    DisableStick( *(u32*)&m_InputMask.MoveX, CurrentInput.MoveX );
    DisableStick( *(u32*)&m_InputMask.MoveY, CurrentInput.MoveY );
    DisableStick( *(u32*)&m_InputMask.LookX, CurrentInput.LookX );
    DisableStick( *(u32*)&m_InputMask.LookY, CurrentInput.LookY );


    //=================================================================
    // Clear disable keys
    //=================================================================

    // Movement keys
    m_DisabledInput.FireKey             = (m_DisabledInput.FireKey)     && (CurrentInput.FireKey) ;
    m_DisabledInput.JetKey              = (m_DisabledInput.JetKey)      && (CurrentInput.JetKey) ;
    m_DisabledInput.JumpKey             = (m_DisabledInput.JumpKey)     && (CurrentInput.JumpKey) ;
    m_DisabledInput.ZoomKey             = (m_DisabledInput.ZoomKey)     && (CurrentInput.ZoomKey) ;
    m_DisabledInput.FreeLookKey         = (m_DisabledInput.FreeLookKey) && (CurrentInput.FreeLookKey) ;
                                            
    // Change keys                      
    m_DisabledInput.NextWeaponKey       = (m_DisabledInput.NextWeaponKey) && (CurrentInput.NextWeaponKey) ;
    m_DisabledInput.PrevWeaponKey       = (m_DisabledInput.PrevWeaponKey) && (CurrentInput.PrevWeaponKey) ;
    m_DisabledInput.ZoomInKey           = (m_DisabledInput.ZoomInKey)     && (CurrentInput.ZoomInKey) ;
    m_DisabledInput.ZoomOutKey          = (m_DisabledInput.ZoomOutKey)    && (CurrentInput.ZoomOutKey) ;
    m_DisabledInput.ViewChangeKey       = (m_DisabledInput.ViewChangeKey) && (CurrentInput.ViewChangeKey) ;
                                            
    // Use keys                         
    m_DisabledInput.MineKey             = (m_DisabledInput.MineKey)      && (CurrentInput.MineKey) ;
    m_DisabledInput.PackKey             = (m_DisabledInput.PackKey)      && (CurrentInput.PackKey) ;
    m_DisabledInput.GrenadeKey          = (m_DisabledInput.GrenadeKey)   && (CurrentInput.GrenadeKey) ;
    m_DisabledInput.HealthKitKey        = (m_DisabledInput.HealthKitKey) && (CurrentInput.HealthKitKey) ;
                                            
    // Drop keys                        
    m_DisabledInput.DropWeaponKey       = (m_DisabledInput.DropWeaponKey) && (CurrentInput.DropWeaponKey) ;
    m_DisabledInput.DropPackKey         = (m_DisabledInput.DropPackKey)   && (CurrentInput.DropPackKey) ;
    m_DisabledInput.DropBeaconKey       = (m_DisabledInput.DropBeaconKey) && (CurrentInput.DropBeaconKey) ;
    m_DisabledInput.DropFlagKey         = (m_DisabledInput.DropFlagKey)   && (CurrentInput.DropFlagKey) ;
                                        
    // Special keys                 
    m_DisabledInput.FocusKey            = (m_DisabledInput.FocusKey)          && (CurrentInput.FocusKey) ;
    m_DisabledInput.SuicideKey          = (m_DisabledInput.SuicideKey)        && (CurrentInput.SuicideKey) ;
    m_DisabledInput.VoiceMenuKey        = (m_DisabledInput.VoiceMenuKey)      && (CurrentInput.VoiceMenuKey) ;
    m_DisabledInput.OptionsMenuKey      = (m_DisabledInput.OptionsMenuKey)    && (CurrentInput.OptionsMenuKey) ;
    m_DisabledInput.TargetingLaserKey   = (m_DisabledInput.TargetingLaserKey) && (CurrentInput.TargetingLaserKey) ;
    m_DisabledInput.TargetLockKey       = (m_DisabledInput.TargetLockKey)     && (CurrentInput.TargetLockKey) ;
    m_DisabledInput.ComplimentKey       = (m_DisabledInput.ComplimentKey)     && (CurrentInput.ComplimentKey) ;
    m_DisabledInput.TauntKey            = (m_DisabledInput.TauntKey)          && (CurrentInput.TauntKey) ;
    m_DisabledInput.ExchangeKey         = (m_DisabledInput.ExchangeKey)       && (CurrentInput.ExchangeKey) ;
    m_DisabledInput.SpecialJumpKey      = (m_DisabledInput.SpecialJumpKey)    && (CurrentInput.SpecialJumpKey) ;

    //=================================================================
    // CHECK FOR PRESSES
    //=================================================================

// SPECIAL_VERSION
#ifdef DEBUG_OPTIONS
    // Pressed focus?
    if ((CurrentInput.FocusKey) && (!m_LastInput.FocusKey))
    {
        m_Input.FocusKey = TRUE ;
        m_HasInputFocus ^= TRUE;

		// Reload the defines?
		#ifdef sbroumley
			LoadDefines() ;
		#endif
    }
    else
        m_Input.FocusKey = FALSE ;
#endif

    // If not enough ammo or dead, do regular fire press so that out of ammo sound does not happen repeatedly!
    ASSERT(m_WeaponInfo) ;
    if ( (m_Health == 0) || (GetWeaponAmmoCount(m_WeaponCurrentType) < m_WeaponInfo->AmmoNeededToAllowFire) )
    {
        // Fire pressed?
        if ((CurrentInput.FireKey) && (!m_LastInput.FireKey))
            m_Input.FireKey = TRUE ;
    }
    else
    {
        // Pressed fire?
        m_Input.FireKey = CurrentInput.FireKey ;
    }

    // Pressed jump?
    //if ((CurrentInput.Jump != 0.0f) && (m_LastInput.Jump == 0.0f))
        //m_Input.Jump = 1.0f ;
    m_Input.JumpKey = CurrentInput.JumpKey ;  // auto jump!

    // Holding zoom?
    m_Input.ZoomKey = CurrentInput.ZoomKey ;

    // Pressed voice menu?
    if ((CurrentInput.VoiceMenuKey) && (!m_LastInput.VoiceMenuKey))
    {
        // Activate Voice Menu if Pause menu not active
        if( (tgl.pUIManager->GetNumUserDialogs( m_UIUserID ) == 0) &&
            (m_PlayerState != PLAYER_STATE_DEAD) )
        {
            tgl.pHUDManager->ActivateVoiceMenu( m_HUDUserID );
        }
    }

    // Pressed options menu key?
    if ((CurrentInput.OptionsMenuKey) && (!m_LastInput.OptionsMenuKey))
    {
        // Deactivate Voice Menu
        tgl.pHUDManager->DeactivateVoiceMenu( m_HUDUserID );

        // Check if Pause Menu already active
        if( tgl.pUIManager->GetNumUserDialogs( m_UIUserID ) > 0 )
        {
            // Close all dialogs
//            fegl.WarriorSetups[m_WarriorID].LastPauseTab = m_pPauseDialog->GetActiveTab();
            tgl.pUIManager->EndUsersDialogs( m_UIUserID );
            m_pPauseDialog = 0;
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
        }
        else
        {
#if 0
            eng_ScreenShot();
#endif
            irect R;
            s32   Flags;

            // Create the Pause Menu dialogs

            R.Set( 16,16, 512-16,216 );
            Flags = ui_win::WF_VISIBLE | ui_win::WF_BORDER;

            m_pPauseDialog = (ui_tabbed_dialog*)tgl.pUIManager->OpenDialog( m_UIUserID, "ui_tabbed_dialog", R, NULL, Flags );
            ASSERT( m_pPauseDialog );
            m_pPauseDialog->SetBackgroundColor( FECOL_DIALOG );
//            m_pPauseDialog->SetTabWidth( 76 ); //90 );

            // Setup rectangle and flags for tab window creation
            R.Set( 0,21, 512-16,200 );
            Flags = ui_win::WF_VISIBLE | ui_win::WF_TAB;

            // INVENTORY
            ui_dialog* pInventory = tgl.pUIManager->OpenDialog( m_UIUserID, "inventory", R, m_pPauseDialog, Flags );
            ASSERT( pInventory );
            m_pPauseDialog->AddTab( "Inventory",  pInventory );

            // VEHICLE
            const obj_type_info* pObjInfo = ObjMgr.GetTypeInfo( object::TYPE_VPAD );
            if( pObjInfo->InstanceCount > 0 )
            {
                ui_dialog* pVehicle   = tgl.pUIManager->OpenDialog( m_UIUserID, "vehicle",   R, m_pPauseDialog, Flags );
                ASSERT( pVehicle );
                m_pPauseDialog->AddTab( "Vehicle",    pVehicle );
            }

            // PLAYER
            ui_dialog* pPlayer    = tgl.pUIManager->OpenDialog( m_UIUserID, "player",    R, m_pPauseDialog, Flags );
            ASSERT( pPlayer );
            m_pPauseDialog->AddTab( "Player",     pPlayer );

            // HOST
            if( (fegl.GameMode != FE_MODE_CAMPAIGN) && tgl.ServerPresent && (g_AdminCount == 0) )
            {
                ui_dialog* pAdmin     = tgl.pUIManager->OpenDialog( m_UIUserID, "admin",     R, m_pPauseDialog, Flags );
                ASSERT( pAdmin );
                m_pPauseDialog->AddTab( "Host",      pAdmin );
            }

            // SCORE
            // Do not show score in "campaign" missions.
            if( GameMgr.GetGameType() != GAME_CAMPAIGN )
            {
                ui_dialog* pScore     = tgl.pUIManager->OpenDialog( m_UIUserID, "score",     R, m_pPauseDialog, Flags );
                ASSERT( pScore );
                m_pPauseDialog->AddTab( "Scores", pScore );
            }

            // CAMPAIGN
            // Only show objectives in "campaign" missions.
            if( (GameMgr.GetCampaignMission() > 0) && (GameMgr.GetCampaignMission() < 12) )
            {
                ui_dialog* pObjectives = tgl.pUIManager->OpenDialog( m_UIUserID, "objectives",     R, m_pPauseDialog, Flags );
                ASSERT( pObjectives );
                m_pPauseDialog->AddTab( "Objectives", pObjectives );
            }

            // HELP
            ui_dialog* pControls  = tgl.pUIManager->OpenDialog( m_UIUserID, "controls",  R, m_pPauseDialog, Flags );
            ASSERT( pControls );
            m_pPauseDialog->AddTab( "Help",       pControls );

            // DEBUG
// SPECIAL_VERSION
#ifdef DEBUG_OPTIONS
            ui_dialog* pDebug     = tgl.pUIManager->OpenDialog( m_UIUserID, "debug",     R, m_pPauseDialog, Flags );
            ASSERT( pDebug );
            m_pPauseDialog->AddTab( "Debug",      pDebug );
#endif

            m_pPauseDialog->FitTabs();

            // Goto last active tab & setup tab tracking
            m_pPauseDialog->ActivateTab( fegl.WarriorSetups[m_WarriorID].LastPauseTab );
            m_pPauseDialog->SetTabTracking( &fegl.WarriorSetups[m_WarriorID].LastPauseTab );

            audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
        }
    }

    // Movement keys
    m_Input.FireKey         = CurrentInput.FireKey ;
    m_Input.JetKey          = CurrentInput.JetKey ;
    m_Input.JumpKey         = CurrentInput.JumpKey ;
    m_Input.ZoomKey         = CurrentInput.ZoomKey ;
    m_Input.FreeLookKey     = (CurrentInput.FreeLookKey) && (GetPlayerViewType() == VIEW_TYPE_3RD_PERSON) ; // Only allow in 3rd person!
    
    // Change keys
    m_Input.NextWeaponKey   = (CurrentInput.NextWeaponKey)  && (!m_LastInput.NextWeaponKey) ;
    m_Input.PrevWeaponKey   = (CurrentInput.PrevWeaponKey)  && (!m_LastInput.PrevWeaponKey) ;
    m_Input.ZoomInKey       = (CurrentInput.ZoomInKey)      && (!m_LastInput.ZoomInKey) ;
    m_Input.ZoomOutKey      = (CurrentInput.ZoomOutKey)     && (!m_LastInput.ZoomOutKey) ;
    m_Input.ViewChangeKey   = (CurrentInput.ViewChangeKey)  && (!m_LastInput.ViewChangeKey) ;

    // Use keys
    m_Input.MineKey         = CurrentInput.MineKey ;
    m_Input.GrenadeKey      = CurrentInput.GrenadeKey ;
    m_Input.PackKey         = (CurrentInput.PackKey)        && (!m_LastInput.PackKey);
    m_Input.HealthKitKey    = (CurrentInput.HealthKitKey)   && (!m_LastInput.HealthKitKey);
    m_Input.TargetingLaserKey = (CurrentInput.TargetingLaserKey) && (!m_LastInput.TargetingLaserKey);

    // Drop keys
    m_Input.DropWeaponKey   = (CurrentInput.DropWeaponKey)  && (!m_LastInput.DropWeaponKey) ;
    m_Input.DropPackKey     = (CurrentInput.DropPackKey)    && (!m_LastInput.DropPackKey) ;
    m_Input.DropBeaconKey   = (CurrentInput.DropBeaconKey)  && (!m_LastInput.DropBeaconKey) ;
    m_Input.DropFlagKey     = (CurrentInput.DropFlagKey)    && (!m_LastInput.DropFlagKey) ;

    // Special keys
    m_Input.FocusKey        = (CurrentInput.FocusKey)       && (!m_LastInput.FocusKey) ;
    m_Input.SuicideKey      = (CurrentInput.SuicideKey)     && (!m_LastInput.SuicideKey) ;
    m_Input.VoiceMenuKey    = (CurrentInput.VoiceMenuKey)   && (!m_LastInput.VoiceMenuKey) ;
    m_Input.TargetLockKey   = (CurrentInput.TargetLockKey)  && (!m_LastInput.TargetLockKey) ;
    m_Input.ComplimentKey   = (CurrentInput.ComplimentKey)  && (!m_LastInput.ComplimentKey) ;
    m_Input.TauntKey        = (CurrentInput.TauntKey)       && (!m_LastInput.TauntKey) ;
    m_Input.ExchangeKey     = (CurrentInput.ExchangeKey)    && (!m_LastInput.ExchangeKey) ;
    m_Input.SpecialJumpKey  = (CurrentInput.SpecialJumpKey) && (!m_LastInput.SpecialJumpKey) ;

	// Auto jump when jetting for the first time! (taking disabled keys into account!)
	if ((m_Input.JetKey) && (!m_LastInput.JetKey))
    {
        // Dont auto jump if player is in a vehicle
        if( IsAttachedToVehicle() == NULL )
        {
    		m_Input.JumpKey = (!m_DisabledInput.JetKey) && (!m_DisabledInput.JumpKey) ;
        }
    }
    
#ifdef T2_DESIGNER_BUILD
    // Disable Shooting
    m_Input.PackKey = m_Input.FireKey = m_Input.MineKey = m_Input.GrenadeKey = 0;
    if (g_DeployMine)
    {
        // Add a mine to his inven if he doesn't have any left.
        if (!GetInventCount(INVENT_TYPE_MINE))
            AddInvent(INVENT_TYPE_MINE, 1);

        m_Input.MineKey = 1;
        g_DeployMine = FALSE;
    }
#endif

    //=================================================================
    // Clear keys that are disabled
    //=================================================================

    // Movement keys
    m_Input.FireKey             = (!m_DisabledInput.FireKey)            && (m_Input.FireKey) ;
    m_Input.JetKey              = (!m_DisabledInput.JetKey)             && (m_Input.JetKey) ;
    m_Input.JumpKey             = (!m_DisabledInput.JumpKey)            && (m_Input.JumpKey) ;
    m_Input.ZoomKey             = (!m_DisabledInput.ZoomKey)            && (m_Input.ZoomKey) ;
    m_Input.FreeLookKey         = (!m_DisabledInput.FreeLookKey)        && (m_Input.FreeLookKey) ;
                                                                        
    // Change keys                                                      
    m_Input.NextWeaponKey       = (!m_DisabledInput.NextWeaponKey)      && (m_Input.NextWeaponKey) ;
    m_Input.PrevWeaponKey       = (!m_DisabledInput.PrevWeaponKey)      && (m_Input.PrevWeaponKey) ;
    m_Input.ZoomInKey           = (!m_DisabledInput.ZoomInKey)          && (m_Input.ZoomInKey) ;
    m_Input.ZoomOutKey          = (!m_DisabledInput.ZoomOutKey)         && (m_Input.ZoomOutKey) ;
    m_Input.ViewChangeKey       = (!m_DisabledInput.ViewChangeKey)      && (m_Input.ViewChangeKey) ;
                                                                        
    // Use keys                                                         
    m_Input.MineKey             = (!m_DisabledInput.MineKey)            && (m_Input.MineKey) ;
    m_Input.PackKey             = (!m_DisabledInput.PackKey)            && (m_Input.PackKey) ;
    m_Input.GrenadeKey          = (!m_DisabledInput.GrenadeKey)         && (m_Input.GrenadeKey) ;
    m_Input.HealthKitKey        = (!m_DisabledInput.HealthKitKey)       && (m_Input.HealthKitKey) ;
                                                                        
    // Drop keys                                                        
    m_Input.DropWeaponKey       = (!m_DisabledInput.DropWeaponKey)      && (m_Input.DropWeaponKey) ;
    m_Input.DropPackKey         = (!m_DisabledInput.DropPackKey)        && (m_Input.DropPackKey) ;
    m_Input.DropBeaconKey       = (!m_DisabledInput.DropBeaconKey)      && (m_Input.DropBeaconKey) ;
    m_Input.DropFlagKey         = (!m_DisabledInput.DropFlagKey)        && (m_Input.DropFlagKey) ;
                                        
    // Special keys                 
    m_Input.FocusKey            = (!m_DisabledInput.FocusKey)           && (m_Input.FocusKey) ;
    m_Input.SuicideKey          = (!m_DisabledInput.SuicideKey)         && (m_Input.SuicideKey) ;
    m_Input.VoiceMenuKey        = (!m_DisabledInput.VoiceMenuKey)       && (m_Input.VoiceMenuKey) ;
    m_Input.OptionsMenuKey      = (!m_DisabledInput.OptionsMenuKey)     && (m_Input.OptionsMenuKey) ;
    m_Input.TargetingLaserKey   = (!m_DisabledInput.TargetingLaserKey)  && (m_Input.TargetingLaserKey) ;
    m_Input.TargetLockKey       = (!m_DisabledInput.TargetLockKey)      && (m_Input.TargetLockKey) ;
    m_Input.ComplimentKey       = (!m_DisabledInput.ComplimentKey)      && (m_Input.ComplimentKey) ;
    m_Input.TauntKey            = (!m_DisabledInput.TauntKey)           && (m_Input.TauntKey) ;
    m_Input.ExchangeKey         = (!m_DisabledInput.ExchangeKey)        && (m_Input.ExchangeKey) ;
    m_Input.SpecialJumpKey      = (!m_DisabledInput.SpecialJumpKey)     && (m_Input.SpecialJumpKey) ;

    // Keep copy for next frame so that button presses work
    m_LastInput = CurrentInput ;

    // Enable/Disable Tweak Menu
    if( g_TweakGame )
    {
        if( m_Input.ExchangeKey )
            g_TweakMenu ^= 1;
        
        if( g_TweakMenu )
        {
            m_Input.Clear();
            return;
        }
    }
	
    // Disable input while in voice menu mode
    if( tgl.pHUDManager->IsVoiceMenuActive( m_HUDUserID ) )
    {
        tgl.pHUDManager->ProcessInput( m_HUDUserID, m_ControllerID, DeltaTime );
        m_Input.Clear();
        m_DisabledInput.Set() ;
        return;
    }

    // Disable input while in pause menu mode
    if( tgl.pUIManager->GetNumUserDialogs( m_UIUserID ) > 0 )
    {
        tgl.pUIManager->ProcessInput( DeltaTime, m_UIUserID );
        m_Input.Clear() ;
        m_DisabledInput.Set() ;
        return ;
    }

	// If player doesn't have the focus, exit
	if (!m_HasInputFocus)
	{
		// Clear out input to stop the player jumping, firing etc
		m_Input.Clear() ;
        m_DisabledInput.Set() ;
		return ;
	}

    // Update movement
    m_Input.MoveX = CurrentInput.MoveX ;
    m_Input.MoveY = CurrentInput.MoveY ;

    // Update looking
	m_Input.LookX = CurrentInput.LookX ;
    m_Input.LookY = CurrentInput.LookY ;

    // Cap move and look vars
    if (m_Input.LookX > 1.0f)
        m_Input.LookX = 1.0f ;
    else
    if (m_Input.LookX < -1.0f)
        m_Input.LookX = -1.0f ;

    if (m_Input.LookY > 1.0f)
        m_Input.LookY = 1.0f ;
    else
    if (m_Input.LookY < -1.0f)
        m_Input.LookY = -1.0f ;

    if (m_Input.MoveX > 1.0f)
        m_Input.MoveX = 1.0f ;
    else
    if (m_Input.MoveX < -1.0f)
        m_Input.MoveX = -1.0f ;

    if (m_Input.MoveY > 1.0f)
        m_Input.MoveY = 1.0f ;
    else
    if (m_Input.MoveY < -1.0f)
        m_Input.MoveY = -1.0f ;

    //=================================================================
    // Process input that doesn't need to happen on the server
    //=================================================================

    // Toggle view?
    if (m_Input.ViewChangeKey)
    {
        m_Input.ViewChangeKey = FALSE ;
        switch(m_PlayerViewType)
        {
            case VIEW_TYPE_1ST_PERSON:
                m_PlayerViewType = VIEW_TYPE_3RD_PERSON ;
                break ;

            case VIEW_TYPE_3RD_PERSON:
                m_PlayerViewType = VIEW_TYPE_1ST_PERSON ;
                break ;
        }
    }

    // Overide to 3rd person view? (used when debugging collision)
    if (PLAYER_DEBUG_3RD_PERSON)
        m_PlayerViewType = VIEW_TYPE_3RD_PERSON ;

    // Holding down zoom?
    if( m_Input.ZoomKey && (IsDead() == FALSE) )
        m_ViewZoomState = VIEW_ZOOM_STATE_ON ;
    else
        m_ViewZoomState = VIEW_ZOOM_STATE_OFF ;

    // Change zoom factor?
    if (m_Input.ZoomInKey)
    {
        m_Input.ZoomInKey = FALSE ;

        if( fegl.WarriorSetups[m_WarriorID].ViewZoomFactor < 4 )
            fegl.WarriorSetups[m_WarriorID].ViewZoomFactor++;

        // Show info
        m_ViewShowZoomTime = s_CommonInfo.VIEW_SHOW_ZOOM_CHANGE_TIME ;
    }
    if (m_Input.ZoomOutKey)
    {
        m_Input.ZoomOutKey = FALSE ;

        if( fegl.WarriorSetups[m_WarriorID].ViewZoomFactor > 1 )
            fegl.WarriorSetups[m_WarriorID].ViewZoomFactor--;

        // Show info
        m_ViewShowZoomTime = s_CommonInfo.VIEW_SHOW_ZOOM_CHANGE_TIME ;
    }
}

//==============================================================================

void player_object::CreateMonkeyMove( f32 DeltaTime )
{
    //xtimer Timer;
    //Timer.Start();

    static f32 AmmoSpeed[INVENT_TYPE_WEAPON_AMMO_TOTAL] = {100.0f,425.0f,60.0f,55.0f,45.0f,10000.0f};
    (void)DeltaTime;
    xbool HasTarget = FALSE;
    vector3 TargetPos;
    vector3 TargetVel;
    s32     NTargets=0;

    m_Move.MonkeyMode = TRUE;

    // Respawn as soon as possible.
    if( IsDead() )
    {
        m_Move.FireKey = TRUE;
        return;
    }

    // Lookup control info
    const player_object::control_info& ControlInfo = GetControlInfo() ;

    // Scan for a target
    {
        vector3 BestTarget;
        vector3 BestTargetVel;
        f32     BestDist = 100000000.0f;

        object* pObj;
        ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
        while( (pObj = ObjMgr.GetNextInType()) )
        {
            if( ((player_object*)pObj)->IsDead() == FALSE )
            {
                if( !(pObj->GetTeamBits() & GetTeamBits()) )
                {
                    f32 D = (pObj->GetPosition() - m_WorldPos).LengthSquared();
                    if( D < BestDist )
                    {
                        BestTargetVel = ((player_object*)pObj)->GetVel();
                        BestTarget = ((player_object*)pObj)->GetPosition();
                        BestDist = D;
                    }

                    NTargets++;
                }
            }
        }
        ObjMgr.EndTypeLoop();

        ObjMgr.StartTypeLoop( object::TYPE_BOT );
        while( (pObj = ObjMgr.GetNextInType()) )
        {
            if( ((bot_object*)pObj)->IsDead() == FALSE )
            {
                if( !(pObj->GetTeamBits() & GetTeamBits()) )
                {
                    f32 D = (pObj->GetPosition() - m_WorldPos).LengthSquared();
                    if( D < BestDist )
                    {
                        BestTargetVel = ((bot_object*)pObj)->GetVel();
                        BestTarget = ((bot_object*)pObj)->GetPosition();
                        BestDist = D;
                    }

                    NTargets++;
                }
            }
        }
        ObjMgr.EndTypeLoop();

        BestDist = x_sqrt(BestDist);

        if( BestDist < 1000.0f )
        {
            HasTarget = TRUE;
            TargetPos = BestTarget;
            TargetVel = BestTargetVel;
        }

        //HasTarget = TRUE;
        //TargetPos = vector3(1067.61f,256.14f,1320.0f);
        //TargetVel.Zero();
    }

    // Attack
    if( HasTarget )
    {
        vector3 Dir;
        f32 Dist;
        radian Pitch,Yaw;
        f32 Speed;

        if( m_WeaponInfo->AmmoType > INVENT_TYPE_WEAPON_AMMO_MISSILE )
            Speed = 10000.0f;
        else
        if(    ( m_WeaponInfo->AmmoType >= INVENT_TYPE_WEAPON_AMMO_START )
            && ( m_WeaponInfo->AmmoType <= INVENT_TYPE_WEAPON_AMMO_END ) )
            Speed = AmmoSpeed[m_WeaponInfo->AmmoType - INVENT_TYPE_WEAPON_AMMO_START];
        else
            Speed = 10000.0f ;

        // Compute estimated travel time of ammo
        f32 TravelT = (TargetPos - m_WorldPos).Length();
            TravelT /= Speed;

        // Compute lead position
        TargetPos += (TargetVel*TravelT) - (GetVel()*TravelT*0.4f);

        Dir = TargetPos - m_WorldPos;
        Dir.GetPitchYaw( Pitch, Yaw );
        Dist = Dir.Length();

        m_Move.LookX = 0;
        m_Move.LookY = 0;

        radian PDiff = m_Rot.Pitch - Pitch;
        radian YDiff = m_Rot.Yaw - Yaw;

        while( YDiff < 0 ) YDiff += R_360;
        while( YDiff > R_360 ) YDiff -= R_360;
        if( YDiff > R_180 ) YDiff -= R_360;

        //
        // Watch for clear line of fire
        //
        if( HasTarget )
        if( (x_abs(PDiff) < R_8) &&
            (x_abs(YDiff) < R_8) )
        {
            radian3     VRot;
            f32         VFOV;
            vector3     VZ, VP, End;

            Get1stPersonView( VP, VRot, VFOV );
            VZ.Set( VRot.Pitch, VRot.Yaw );

            End = TargetPos;

            // Use faster line of sight test
            s_Collider.ClearExcludeList() ;
            s_Collider.RaySetup    ( this, VP, End, m_ObjectID.GetAsS32(), TRUE );
            ObjMgr.Collide  ( ATTR_SOLID_STATIC, s_Collider );
            if( (!s_Collider.HasCollided()) || (x_irand(0,30*10*10) < 10) )
            {
                m_Move.FireKey = TRUE;
                m_Move.MoveX = 1.0f;
            }

            // use grenades?
            if ( ( VP.Y - End.Y ) > 5.0f )
            {
                if ( ( x_abs( VP.X - End.X ) < 20.0f ) &&
                     ( x_abs( VP.Z - End.Z ) < 20.0f ) )
                {
                    m_Move.GrenadeKey = TRUE;
                }
            }
        }

        //
        // Aim in target's direction
        //
        {
            f32 T;

            if( x_abs(PDiff) > R_5 ) T = 1.0f;
            else                     T = x_abs(PDiff)/R_5;

            if( PDiff > 0 ) m_Move.LookY = -(ControlInfo.PS2_LOOK_UP_DOWN_SPEED*T);
            else            m_Move.LookY =  (ControlInfo.PS2_LOOK_UP_DOWN_SPEED*T);

            if( x_abs(YDiff) > R_5 ) T = 1.0f;
            else                     T = x_abs(YDiff)/R_5;

            if( YDiff > 0 ) m_Move.LookX = -(ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED*T);
            else            m_Move.LookX =  (ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED*T);
        }

        //
        // Decide how to move
        //
        if( (x_abs(PDiff) < R_45) &&
            (x_abs(YDiff) < R_45) )
        {
            f32 HorizDist;
            f32 VertDist;
            HorizDist = x_sqrt( Dir.X*Dir.X + Dir.Z*Dir.Z );
            VertDist  = Dir.Y;

            if( m_Energy < (5.0f * m_MoveInfo->MAX_ENERGY / 100.0f)) 
                m_ManiacState &= 0x0F;

            if( m_Energy> (95.0f * m_MoveInfo->MAX_ENERGY / 100.0f) )
            {
                m_ManiacState |= 0x10;
                m_Move.JumpKey = TRUE;
            }

            if( m_ManiacState & 0x10 )
            if( (Dist>50) || (VertDist>-20)  )
            {
                m_Move.JetKey = TRUE;
            }

            if( HorizDist > 50 )
            {
                m_Move.MoveY          = 1.0f;
                m_Move.MoveX          = 0.0f;
            }
        }
        else
        {
            // do anything to get out of vertical with other player
            m_Move.MoveX = 1.0f;
        }

        // Get some health back
        if ( GetHealth() < 0.4f )
        {
            m_Move.HealthKitKey = TRUE;
        }
        
        //
        // Should we change weapons
        //
        if( HasTarget )
        if( ((GetWeaponAmmoCount(m_WeaponCurrentType) == 0) && 
             (m_WeaponInfo->AmmoType!=INVENT_TYPE_NONE) && (x_irand(0,30*10*10) < 10) ) || 
            (x_irand(0,30*10*10) < 10) )
            m_Move.NextWeaponKey = TRUE;

        // SKip laser
        if( (m_WeaponInfo->AmmoType == INVENT_TYPE_NONE) && (x_irand(0,30*10)<10) )
            m_Move.NextWeaponKey = TRUE;

        // switch weapons when out of ammo
        if ( GetWeaponAmmoCount(m_WeaponCurrentType) < m_WeaponInfo->AmmoNeededToAllowFire )
            m_Move.NextWeaponKey = TRUE;

        //
        // Should we suicide?
        //
        if( (GetInventCount(INVENT_TYPE_WEAPON_AMMO_DISK) == 0) &&
            (GetInventCount(INVENT_TYPE_WEAPON_AMMO_BULLET) == 0) &&
            (GetInventCount(INVENT_TYPE_WEAPON_AMMO_GRENADE) == 0) &&
            (GetInventCount(INVENT_TYPE_WEAPON_AMMO_PLASMA) == 0) &&
            (GetInventCount(INVENT_TYPE_WEAPON_AMMO_MORTAR) == 0))
        {
            m_Move.SuicideKey = TRUE;
        }
    }
    else
    {
        m_Move.LookX          = ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED * 0.1f;
        m_Move.LookY          = (m_Rot.Pitch < -R_20)?(0):(-ControlInfo.PS2_LOOK_UP_DOWN_SPEED * 0.5f);
        m_Move.MoveX          = x_frand(-0.5f,0.5f) ;
        m_Move.MoveY          = x_frand(-0.5f,0.5f) ;
        m_Move.FireKey        = FALSE;
        m_Move.JetKey         = (x_irand(0,100) < 10);       
        m_Move.JumpKey        = (x_irand(0,300) < 10) ;
        m_Move.MineKey        = FALSE ;
        m_Move.GrenadeKey     = FALSE ;
        m_Move.HealthKitKey   = FALSE ;
        m_Move.FreeLookKey    = FALSE ;
        m_Move.NextWeaponKey  = (x_irand(0,30*30) < 1);
        m_Move.SuicideKey     = FALSE ;
    }

    //Timer.Stop();
    //x_printfxy(10,m_ObjectID.Slot%16,"%6.3f",Timer.ReadMs());
}

//==============================================================================

// Converts player input into a valid move
void player_object::CreateMoveFromInput( f32 DeltaTime )
{
    xbool InputActive = FALSE;

    // Clear out all values
    m_Move.Clear() ;

    // Plop in input only if the player is alive!
    if (m_Health > 0)
    {
        if( GetManiacMode() )
        {
            if (    (m_PlayerState != PLAYER_STATE_CONNECTING)
                 && (m_PlayerState != PLAYER_STATE_TAUNT) )
                CreateMonkeyMove( DeltaTime );
        }
        else
        {

            // Only allow movement if not in these states
            if (    (m_PlayerState != PLAYER_STATE_WAIT_FOR_GAME)
                 && (m_PlayerState != PLAYER_STATE_TAUNT) )
            {
                // Movement keys
                m_Move.FireKey              = m_Input.FireKey;       // Fire button
                m_Move.JetKey               = m_Input.JetKey ;        // Jetpack button
                m_Move.JumpKey              = m_Input.JumpKey ;       // Jump button

                // Change keys
                m_Move.NextWeaponKey        = m_Input.NextWeaponKey ; // Next weapon key
                m_Move.PrevWeaponKey        = m_Input.PrevWeaponKey ; // Previous weapon key

                // Use keys
                m_Move.MineKey              = m_Input.MineKey ;
                m_Move.GrenadeKey           = m_Input.GrenadeKey ;
                m_Move.PackKey              = m_Input.PackKey ;       // Use pack button
                m_Move.HealthKitKey         = m_Input.HealthKitKey ;  // Use health kit button

                // Drop keys
                m_Move.DropWeaponKey        = m_Input.DropWeaponKey ;
                m_Move.DropPackKey          = m_Input.DropPackKey ;
                m_Move.DropBeaconKey        = m_Input.DropBeaconKey ;
                m_Move.DropFlagKey          = m_Input.DropFlagKey ;

                // Special keys
                m_Move.SuicideKey           = m_Input.SuicideKey | m_RequestSuicide ; // Note - take requests also!
                m_Move.TargetingLaserKey    = m_Input.TargetingLaserKey;
                m_Move.TargetLockKey        = m_Input.TargetLockKey;
                m_Move.ComplimentKey        = m_Input.ComplimentKey;
                m_Move.TauntKey             = m_Input.TauntKey;
                m_Move.ExchangeKey          = m_Input.ExchangeKey;

                // Movement
                m_Move.MoveX                = m_Input.MoveX ; 
                m_Move.MoveY                = m_Input.MoveY ;      
            }

            // Always do these

            // Movement keys
            m_Move.FreeLookKey   = m_Input.FreeLookKey ;   // Free look key

            // Misc
            m_Move.LookX                    = m_Input.LookX ;
            m_Move.LookY                    = m_Input.LookY ;      
            m_Move.MonkeyMode               = GetManiacMode() ;
        }
    }
    else
    {
        if( GetManiacMode() )
        {
            if (    (m_PlayerState != PLAYER_STATE_CONNECTING)
                 && (m_PlayerState != PLAYER_STATE_TAUNT) )
                CreateMonkeyMove( DeltaTime );
        }
        else
        {
            // Do free look if dead...
            m_Move.FreeLookKey    = TRUE ;
            m_Move.LookX          = m_Input.LookX ;
            m_Move.LookY          = m_Input.LookY ;      
            m_Move.MonkeyMode     = GetManiacMode() ;
            m_Move.FireKey        = m_Input.FireKey       ||
                                    m_Input.JumpKey       ||
                                    m_Input.JetKey        ||
                                    m_Input.ZoomKey       ||
                                    m_Input.ZoomInKey     ||
                                    m_Input.ZoomOutKey    ||
                                    m_Input.TauntKey      ||
                                    m_Input.ComplimentKey ||
                                    m_Input.GrenadeKey    ||
                                    m_Input.PackKey       ||
                                    m_Input.NextWeaponKey ||
                                    m_Input.TargetLockKey;
        }
    }

    // Allow the following even if the player is dead!
    m_Move.NetSfxID                 = m_NetSfxID ;
    m_Move.NetSfxTeamBits           = m_NetSfxTeamBits ;
    m_Move.NetSfxTauntAnimType      = m_NetSfxTauntAnimType ;
    m_Move.InventoryLoadoutChanged  = m_InventoryLoadoutChanged ;
    m_Move.InventoryLoadout         = m_InventoryLoadout ;
    m_Move.ChangeTeam			    = m_ChangeTeam ;
    m_Move.VoteType                 = m_VoteType ;
    m_Move.VoteData                 = m_VoteData ;
    m_Move.ViewChanged              = m_ViewChanged;
    m_Move.View3rdPlayer            = (m_PlayerViewType  == VIEW_TYPE_3RD_PERSON);
    m_Move.View3rdVehicle           = (m_VehicleViewType == VIEW_TYPE_3RD_PERSON);

    // Only send net sfx once
    m_NetSfxID                  = 0 ;
    m_NetSfxTeamBits            = 0 ;
    m_NetSfxTauntAnimType       = 0 ;

    // Only send inventory once
    m_InventoryLoadoutChanged   = FALSE ;

    // Only send change team once
    m_ChangeTeam                = FALSE ;

    // Only send vote info once
    m_VoteType                  = VOTE_TYPE_NULL ;
    m_VoteData                  = 0 ;

    // Only send view change once.
    m_ViewChanged               = FALSE;

    // Is player on a bomber?
    xbool IsBomber    = FALSE;
    vehicle* pVehicle = IsVehiclePilot();
    if( pVehicle )
    {
        if( pVehicle->GetType() == object::TYPE_VEHICLE_THUNDERSWORD )
        {
            IsBomber = TRUE;
        }
    }

    // Send zoom amount too if using it
    if( (m_ViewZoomState == VIEW_ZOOM_STATE_ON) && (IsBomber == FALSE ) )
        m_Move.ViewZoomFactor = fegl.WarriorSetups[m_WarriorID].ViewZoomFactor ;
    else
        m_Move.ViewZoomFactor = 0 ;

    if( IsVehiclePilot() )
    {
        if( fegl.WarriorSetups[m_WarriorID].InvertVehicleY )
        {
            m_Move.LookY *= -1.0f;
        }
    }
    else
    {
        if( fegl.WarriorSetups[m_WarriorID].InvertPlayerY )
        {
            m_Move.LookY *= -1.0f;
        }
    }

    // How long this move applies
    m_Move.DeltaTime = DeltaTime ;

    // Truncate so that client controlled player acts exactly like server player!
    m_Move.Truncate(m_TVRefreshRate) ;

    // Clear sticky presses
    m_Input.Clear() ;

    // Clear suicide request
    m_RequestSuicide = FALSE ;

    // Was input activated? (just test sticks)
    InputActive |= m_Move.LookX != 0 ;
    InputActive |= m_Move.LookY != 0 ;
    InputActive |= m_Move.MoveX != 0 ;
    InputActive |= m_Move.MoveY != 0 ;
    InputActive |= m_Move.FireKey != 0 ;
    InputActive |= m_Move.JetKey != 0;
    InputActive |= m_Move.FreeLookKey != 0;
}


void player_test_collision(player_object* Obj, collider::collision& Collision, vector3 Pos, vector3 Move, xbool Draw)
{
    static vector3 PlanePoints[] = 
    {
        // plane0
        vector3(0.0f,          500.0f,    0.0f),
        vector3(0.0f+100.0f,   500.0f,    0.0f),
        vector3(0.0f,          500.0f,    0.0f+100.0f),

        // plane1
        vector3(0.0f,          500.0f,         0.0f),
        vector3(0.0f,          500.0f+100.0f,  0.0f+100.0f),
        vector3(0.0f+100.0f,   500.0f,         0.0f),
    } ;
#define TEST_PLANES 2

    static plane Planes[TEST_PLANES] ;


    //static AtPos=FALSE ;
    //if ((!AtPos) || (m_WorldPos.Y < 490))
    //{
        //m_WorldPos = vector3(5,505,5) ;
        //m_Vel.Zero() ;
        //AtPos = TRUE ;
    //}
    //m_Health = 100 ;

    s32 pp = 0 ;
    s32 p ;
    for (p = 0 ; p < TEST_PLANES ; p++)
    {
        Planes[p].Setup(PlanePoints[pp+2], PlanePoints[pp+1], PlanePoints[pp+0]) ;
        
        if (Draw)
        {
            draw_Grid(PlanePoints[pp], 
                      PlanePoints[pp+1] - PlanePoints[pp], 
                      PlanePoints[pp+2] - PlanePoints[pp], XCOLOR_RED, 60);
        }

        pp += 3 ;
    }

    // test
    Collision.Collided = FALSE ;
    Collision.T = 100 ;

    // check planes
    for (p = 0 ; p < TEST_PLANES ; p++)
    {
        if (Planes[p].Dot(Move) <= 0) // moving into?
        {
            f32 t ;
	        f32 dotA,dotB;

	        dotA = v3_Dot(Planes[p].Normal, Pos);
	        dotB = v3_Dot(Planes[p].Normal, Pos+Move) ;

	        if(dotB == dotA)
                t = 1 ;
            else
                t = (-Planes[p].D - dotA) / (dotB - dotA);

            if ((t >= 0) && (t <= 1) && (t < Collision.T))
            {
                Collision.Point      = Pos + (Move * t) ;
                Collision.Collided   = TRUE ;
                Collision.T          = t ;
                Collision.Plane      = Planes[p] ;
                Collision.pObjectHit = Obj ;
            }
        }
    }
}

//==============================================================================

// Setups up contact info, given a collision, and the distance below the player (HitDist) the collision occured
void player_object::SetupContactInfo( const collider::collision& Collision, f32 HitDist )
{
    s32     i ;
    vector3 Axis ;
    radian  Angle ;

    // Clear contact info
    m_ContactInfo.Clear() ;

    // Shouldn't have called if there's no collision!
    ASSERT(Collision.Collided) ;

    // Get object that was it
    object* pObject = (object*)Collision.pObjectHit ;
    ASSERT(pObject) ;

    // Record contact surface?
    xbool CheckForLip = TRUE ;
    if (HitDist <= PLAYER_HIT_BY_PLAYER_DIST)
    {
        // Keep object
        m_ContactInfo.BelowObjectType = pObject->GetType() ;

        // Keep normal
        m_ContactInfo.BelowNormal  = Collision.Plane.Normal ;

        // Setup surface type
        switch(pObject->GetType())
        {
            // Lookup terrain surface
            case object::TYPE_TERRAIN:
                switch(tgl.Environment)
                {
                    default:
                    case t2_globals::ENVIRONMENT_DESERT:
                    case t2_globals::ENVIRONMENT_LUSH:
                    case t2_globals::ENVIRONMENT_LAVA:
                    case t2_globals::ENVIRONMENT_BADLAND:
                        m_ContactInfo.BelowSurface = contact_info::SURFACE_TYPE_SOFT ;
                        break ;

                    case t2_globals::ENVIRONMENT_ICE:
                        m_ContactInfo.BelowSurface = contact_info::SURFACE_TYPE_SNOW ;
                        break ;
                }
                CheckForLip = FALSE ;   // Stops players falling through terrain
                break ;

            // Lookup building surface
            case object::TYPE_BUILDING:
                m_ContactInfo.BelowSurface = contact_info::SURFACE_TYPE_HARD ;
                break ;

            // Soft types
            case object::TYPE_SCENIC:
                m_ContactInfo.BelowSurface = contact_info::SURFACE_TYPE_SOFT ;
                CheckForLip = FALSE ;   // Stops players hanging up on trees!
                break ;

            // All other surfaces are deemed to be metal (vehicles, vehicle pads etc)
            default:
                m_ContactInfo.BelowSurface = contact_info::SURFACE_TYPE_METAL ;
                break ;
        }

        // Setup contact rotation
        vector3 Up = vector3(0,1,0) ;
        Up.GetRotationTowards( m_ContactInfo.BelowNormal, Axis, Angle );
        m_ContactInfo.BelowRotation.Setup(Axis,Angle);
    }

    // Record contact for physics?
    if (HitDist <= PLAYER_CONTACT_DIST)
    {
        // Record that we are touching a surface
        m_ContactInfo.ContactSurface = m_ContactInfo.BelowSurface ;

        // If any of the player feet bbox points are below the run plane,
        // then the player is on a lip (or tip), so just use an up vector
        // so that the player can run along the lip.

        // If this is not performed the player can get stuck on lips that
        // pertrude from the floor. eg. Recalescence at pos (1251,186,1357)

        // Start with collision plane ready for lip test...
        vector3 PlaneNormal = Collision.Plane.Normal ;

        // Perform a lip test? (only if above the highest collision point in the collider)
        if ( (CheckForLip) && (m_WorldPos.Y > s_Collider.GetHighPoint()) )
        {
            // Use bottom of bounding box (feet)
            vector3 Corner ;
            Corner.Y = m_WorldBBox.Min.Y ;

            // Check the 4 corners
            for (i = 0 ; i < 4 ; i++)
            {
                switch(i)
                {
                    case 0:
                        Corner.X = m_WorldBBox.Min.X ;
                        Corner.Z = m_WorldBBox.Min.Z ;
                        break ;
                    case 1:
                        Corner.X = m_WorldBBox.Max.X ;
                        Corner.Z = m_WorldBBox.Min.Z ;
                        break ;
                    case 2:
                        Corner.X = m_WorldBBox.Min.X ;
                        Corner.Z = m_WorldBBox.Max.Z ;
                        break ;
                    case 3:
                        Corner.X = m_WorldBBox.Max.X ;
                        Corner.Z = m_WorldBBox.Max.Z ;
                        break ;
                }

                // Is corner of bbox below the plane?
                // (a threshold is used instead of Collision.Plane.InBack() because of floating point inaccuracy -
                //  which was letting the player go through Paranoia ramps at (751,54,1051) )
                f32 Dist = Collision.Plane.Distance(Corner) ;
                if ( Dist < -PLAYER_LIP_TEST_DIST )
                {
                    // Debug info
                    #ifdef PLAYER_DEBUG_LIP_COLL
                        x_printf("Player is on a surface lip!\n") ;
                    #endif

                    // Use level normal
                    PlaneNormal = vector3(0,1,0) ;
                    break ;
                }
            }
        }

        // Record jump surface info?
        if (    (pObject->GetType() == object::TYPE_TERRAIN) || 
                (PlaneNormal.Y > x_cos(m_MoveInfo->JUMP_SURFACE_ANGLE)) )
        {
            m_ContactInfo.JumpSurface = m_ContactInfo.BelowSurface ;
            m_ContactInfo.JumpNormal  = PlaneNormal ;
        }

        // Record run surface info?
        if (    (pObject->GetType() == object::TYPE_TERRAIN) || 
                (PlaneNormal.Y > x_cos(m_MoveInfo->RUN_SURFACE_ANGLE)) )
        {
            m_ContactInfo.RunSurface = m_ContactInfo.BelowSurface ;
            m_ContactInfo.RunNormal  = PlaneNormal ;
        }

        // In crease check - if in contact with any run surface, then allow the player to run!
        // (fixes sliding again the terrain and edge of building in Underhill: 832, 95, 769)
        //x_printfxy(2,10, "NContacts:%d", s_Collider.GetNContacts()) ;
        if (s_Collider.GetNContacts() >= 2)
        {
            for (i = 0 ; (i < s_Collider.GetNContacts()) && (!m_ContactInfo.RunSurface) ; i++)
            {
                // Get contact
                const collider::contact& Contact = s_Collider.GetContact(i) ;

                // Only consider contacts that are really close to the final collision
                if (x_abs(Contact.T - Collision.T) < PLAYER_CONTACT_THRESHOLD)
                {
                    // Lookup plane
                    PlaneNormal = s_Collider.GetContact(i).Plane.Normal ;

                    // Record jump surface info?
                    if (PlaneNormal.Y > x_cos(m_MoveInfo->JUMP_SURFACE_ANGLE))
                    {
                        m_ContactInfo.JumpSurface = m_ContactInfo.BelowSurface ;
                        m_ContactInfo.JumpNormal  = PlaneNormal ;
                    }

                    // Record run surface info?
                    if (PlaneNormal.Y > x_cos(m_MoveInfo->RUN_SURFACE_ANGLE))
                    {
                        m_ContactInfo.RunSurface = m_ContactInfo.BelowSurface ;
                        m_ContactInfo.RunNormal  = PlaneNormal ;
                    }
                }
            }
        }
    }

    // Record surface dist
    m_ContactInfo.DistToSurface = HitDist ;
}

//==============================================================================

// Sets up on ground flags etc. - call once per move
void player_object::SetupContactInfo( collider::collision* pCollision /*= NULL */)
{
	collider::collision     Collision ;
    bbox                    CollBBox ;
    object*                 pObject ;

    // Clear contact info
    m_ContactInfo.Clear() ;

    // Skip if on a vehicle
    if (m_PlayerState == PLAYER_STATE_VEHICLE)
        return ;

    DEBUG_BEGIN_TIMER(ContactInfo)

	// Use local collision variable if not output collision is required
	if (pCollision == NULL)
		pCollision = &Collision ;

    // Start collision slightly above ground
    SetupBounds(m_WorldPos, CollBBox) ;
    CollBBox.Min.Y += TRACTION_DISTANCE ;
    CollBBox.Max.Y += TRACTION_DISTANCE ;

    // Calc dist to check below player
    f32 CheckDist = MAX(MAX(PLAYER_CONTACT_DIST, PLAYER_HIT_BY_PLAYER_DIST), PLAYER_SURFACE_CHECK_DIST) ;

    // Call extrusion collision to find the ground
    DEBUG_BEGIN_TIMER(ColliderExtSetup)
    vector3  Movement = vector3(0.0f, -CheckDist, 0.0f ) ; // Check below player
    s_Collider.ClearExcludeList() ;
    s_Collider.ExtSetup( this, CollBBox, Movement, -1, FALSE, TRUE ) ;  // Record contacts!
    DEBUG_END_TIMER(ColliderExtSetup)
    
    DEBUG_BEGIN_TIMER(ObjMgrCollide)
    ObjMgr.Collide(PLAYER_LANDABLE_ATTRS, s_Collider ) ;
    DEBUG_END_TIMER(ObjMgrCollide)

    // Get results
    s_Collider.GetCollision( *pCollision );

    // Any collision?
    if (!pCollision->Collided)
    {
        DEBUG_END_TIMER(ContactInfo)
        return ;
    }

    DEBUG_BEGIN_TIMER(SetupInfo)

    // Setup hit dist
    f32 HitDist = CheckDist * pCollision->T ;

    // Setup contact info
    SetupContactInfo(*pCollision, HitDist) ;

    #ifdef DEBUG_COLLISION
        // Setup set2 points (drawn in PlayerRender.cpp)
        NPlayerCollPoints2 = NCollPoints ;
        for (s32 i = 0 ; i < NCollPoints ; i++)
            PlayerCollPoints2[i] = CollPoints[i] ;
    #endif  // #ifdef DEBUG_COLLISION

    // Interact with object?
    pObject = (object*)pCollision->pObjectHit ;
    ASSERT(pObject) ;
    if ((HitDist <= PLAYER_HIT_BY_PLAYER_DIST) && (pObject) && (tgl.pServer) && (m_Health > 0))
    {
        // Hit a vehicle?
        if (pObject->GetAttrBits() & object::ATTR_VEHICLE)
        {
            // Try attach to vehicle?
            if (m_PlayerState != PLAYER_STATE_VEHICLE)
            {
                vehicle* Vehicle = (vehicle*)pObject ;

                // If attaching, grab the vehicle dirty bits!
                if (Vehicle->HitByPlayer(this) != -1)
                    m_DirtyBits |= Vehicle->GetDirtyBits() ;
                else
                {
                    // Make the player wobble off the vehicle if it's moving
                    xbool VehicleMoving = FALSE ;
                    
                    // Moving?    
                    if (Vehicle->GetVel().LengthSquared() > SQR(0.01f))
                        VehicleMoving = TRUE ;

                    // Rotating?
                    //if (    (ABS(Vehicle->GetRotVel().Pitch) > R_1)
                            //(ABS(Vehicle->GetRotVel().Yaw)   > R_1)
                            //(ABS(Vehicle->GetRotVel().Roll)  > R_1) )
                        //VehicleMoving = TRUE ;
               
                    // Wobble player off
                    if (VehicleMoving)
                    {
                        vector3 Vel = GetBBox().GetCenter() - Vehicle->GetBBox().GetCenter() ;
                        Vel.Normalize() ;
                        Vel *= 2.0f ;
                        SetVel(m_Vel + Vel) ;
                    }
                }
            }
        }
    }

    // Blend to new ground rotation (if any) - skip players so corpse do not orientate!
    if (        (m_ContactInfo.BelowSurface)
            &&  (!(pObject->GetAttrBits() & object::ATTR_PLAYER)) )
    {
        // Blend to new ground rot to avoid snapping
        m_GroundRot = Blend( m_GroundRot, m_ContactInfo.BelowRotation, 0.2f ) ;
        m_GroundRot.Normalize() ;
    }

    DEBUG_END_TIMER(SetupInfo)

    DEBUG_END_TIMER(ContactInfo)
}

//==============================================================================

void player_object::ApplyMove()
{
    // Flag logic has happened this frame
    m_AttrBits |= object::ATTR_LOGIC_APPLIED ;

    // NULL move?
    if (m_Move.DeltaTime <= 0)
        return ;

    DEBUG_BEGIN_TIMER(ApplyMove)

    // Make sure server and client are in sync!
    ASSERT(m_CharacterInfo = &s_CharacterInfo[m_CharacterType][m_ArmorType]) ;
    ASSERT(m_MoveInfo      = &s_MoveInfo[m_CharacterType][m_ArmorType]) ;

    // Record info for setting the dirty bits
    vector3     LastPos             = m_WorldPos ;
    vector3     LastVel             = m_Vel ;
    radian3     LastRot             = m_Rot ;
    radian      LastViewFreeLookYaw = m_ViewFreeLookYaw ;
    f32         LastEnergy          = m_Energy ;

    // Validate input ranges
    f32 DeltaTime = m_Move.DeltaTime ;
    ASSERT(m_Move.MoveX >= -1) ;
    ASSERT(m_Move.MoveX <=  1) ;
    ASSERT(m_Move.MoveY >= -1) ;
    ASSERT(m_Move.MoveY <=  1) ;
    ASSERT(m_Move.LookX >= -1) ;
    ASSERT(m_Move.LookX <=  1) ;
    ASSERT(m_Move.LookY >= -1) ;
    ASSERT(m_Move.LookY <=  1) ;

    //==========================================================================
    // Server special input (suicide, voice menu etc)
	//==========================================================================
    if (tgl.pServer)
    {
        // Suicide?
        if ((m_Move.SuicideKey) && (m_Health > 0))
        {
            m_Move.SuicideKey   = FALSE ;
            CommitSuicide() ;
        }

        // Play net sound?
        if (m_Move.NetSfxID)
        {
            ASSERT( FALSE );
            //tgl.pServer->SendAudio(m_Move.NetSfxID, m_Move.NetSfxTeamBits) ;
        }

        // Update taunt spam timer
        m_SpamTimer = MAX(0, m_SpamTimer - m_Move.DeltaTime) ;

        // Taunt or compliment?
        if ( ((m_Move.TauntKey) || (m_Move.ComplimentKey)) && (m_Health > 0) )
        {
            // Only show a message if the spam timer is at zero
            // (ie. taunt/compliment hasn't been pressed for a while)
            if (m_SpamTimer == 0)
            {
                // Show message
                if (m_Move.TauntKey)
                    MsgMgr.Message( MSG_NEGATIVE, m_ObjectID.Slot, ARG_WARRIOR, m_ObjectID.Slot );
                else
                    MsgMgr.Message( MSG_POSITIVE, m_ObjectID.Slot, ARG_WARRIOR, m_ObjectID.Slot );

                // Set the spam timer so player can't taunt/compliment for a while
                m_SpamTimer = SPAM_MIN_TIME ;
            }
            else
            {
                // Trying to spam eh? Well penalize the player and make the spam interval longer
                m_SpamTimer += SPAM_DELAY_TIME ;

                // Cap it, otherwise we'll really piss off the player
                if (m_SpamTimer > SPAM_MAX_TIME)
                    m_SpamTimer = SPAM_MAX_TIME ;
            }
        }

        /*
        // Play taunt anim?
        if ((m_Move.NetSfxTauntAnimType != ANIM_TYPE_NONE) && (m_Health > 0) && (m_PlayerState != PLAYER_STATE_VEHICLE))
        {
            // Put into taunt state
            Player_SetupState(PLAYER_STATE_TAUNT) ;
        
            // Start the anim
            m_PlayerInstance.SetAnimByType(m_Move.NetSfxTauntAnimType, 0.25f) ;
        }
        */

        // New inventory loadout?
        if (m_Move.InventoryLoadoutChanged)
            SetInventoryLoadout( m_Move.InventoryLoadout ) ;

        // New view settings?
        if( m_Move.ViewChanged )
        {
            SetPlayerViewType ( m_Move.View3rdPlayer  ? VIEW_TYPE_3RD_PERSON : VIEW_TYPE_1ST_PERSON );
            SetVehicleViewType( m_Move.View3rdVehicle ? VIEW_TYPE_3RD_PERSON : VIEW_TYPE_1ST_PERSON );
        }

		// Change teams?
		if (m_Move.ChangeTeam)
		{
			GameMgr.ChangeTeam( m_ObjectID.Slot, FALSE );
		}

        // Is player participating in a vote?
        switch(m_Move.VoteType)
        {
            // No voting going on here...nothing to do
            case VOTE_TYPE_NULL:
                break ;

            // Request a change of map
            case VOTE_TYPE_CHANGE_MAP:
                GameMgr.InitiateMapVote( m_Move.VoteData );
                GameMgr.CastVote( m_ObjectID.Slot, TRUE );
                break ;

            // Request that a player be kicked out of the game
            case VOTE_TYPE_KICK_PLAYER:
                GameMgr.InitiateKickVote( m_Move.VoteData );
                GameMgr.CastVote( m_ObjectID.Slot, TRUE );
                break ;

            // Vote "No" on current vote
            case VOTE_TYPE_NO:
                GameMgr.CastVote( m_ObjectID.Slot, FALSE );
                break ;

            // Vote "Yes" on current vote
            case VOTE_TYPE_YES:
                GameMgr.CastVote( m_ObjectID.Slot, TRUE );
                break ;
        }

        // Has target lock key been pressed?
        if( m_Move.TargetLockKey )
        {
            if( IsAttachedToVehicle() )
            {
                // Can't lock.
                // Setting the dirty bit will cause the sound on the client.
                m_DirtyBits |= PLAYER_DIRTY_BIT_TARGET_LOCK;

                // Audio for server.
                if( m_ProcessInput )
                    audio_Play( SFX_TARGET_LOCK_DENIED );
            }
            else
            if( m_TargetIsLocked )
            {
                // Unlock!
                m_TargetIsLocked = FALSE;
                m_LockedTarget   = obj_mgr::NullID;
                m_DirtyBits     |= PLAYER_DIRTY_BIT_TARGET_LOCK;

                // Audio for server.
                if( m_ProcessInput )
                    audio_Play( SFX_TARGET_LOCK_LOST );
            }
            else
            {
                // Attempt to lock.
                vector3     TargetPos;
                vector3     TargetVec;
                object::id  TargetID;
                
                if( (fegl.ServerSettings.TargetLockEnabled || GameMgr.IsCampaignMission()) && 
                    GetAutoAimPoint( this, TargetVec, TargetPos, TargetID ) )
                {
                    // Lock!
                    m_TargetIsLocked = TRUE;
                    m_LockedTarget   = TargetID;
                    m_DirtyBits     |= PLAYER_DIRTY_BIT_TARGET_LOCK;
                    pGameLogic->ItemAutoLocked( TargetID, this->GetObjectID() );

                    // Audio for server.
                    if( m_ProcessInput )
                        audio_Play( SFX_TARGET_LOCK_AQUIRED );
                }
                else
                {
                    // Can't lock.
                    // Setting the dirty bit will cause the sound on the client.
                    m_DirtyBits |= PLAYER_DIRTY_BIT_TARGET_LOCK;

                    // Audio for server.
                    if( m_ProcessInput )
                        audio_Play( SFX_TARGET_LOCK_DENIED );
                }
            }
        }
    }

    //==========================================================================
    // Recharge energy
    //==========================================================================

    // Only recharge if not dead
    if (!IsDead())
    {
        f32 EnergyRecharge ;

        // If player has energy pack, increase the rate
        if ((m_PackCurrentType == INVENT_TYPE_ARMOR_PACK_ENERGY) && (m_PackActive))
            EnergyRecharge = m_MoveInfo->RECHARGE_RATE_WITH_ENERGY_PACK ;
        else
            EnergyRecharge = m_MoveInfo->RECHARGE_RATE ;
        EnergyRecharge *= DeltaTime ;

        AddEnergy(EnergyRecharge) ;
    }

    //==========================================================================
    // Setup ignore input flag (add all states that do not require movement here
	//==========================================================================

    xbool bIgnoreMoveInput = (m_PlayerState == PLAYER_STATE_AT_INVENT)      ||
                             (m_PlayerState == PLAYER_STATE_WAIT_FOR_GAME)  ||
                             (m_PlayerState < PLAYER_STATE_MOVE_START)      ||
                             (m_PlayerState > PLAYER_STATE_MOVE_END)        ||
                             (m_Health == 0) ;

    xbool bIgnoreLookInput = (m_PlayerState == PLAYER_STATE_WAIT_FOR_GAME) ;

    // These maybe over ridden if player rotation is actually a vehicles turret rotation
    f32     LookSpeedScaler = 1.0f ;
    radian  LookMinPitch    = s_CommonInfo.VIEW_MIN_PITCH ;
    radian  LookMaxPitch    = s_CommonInfo.VIEW_MAX_PITCH ;

    //==========================================================================
    // Apply movement to vehicle if pilot-ing one
	//==========================================================================

    // Piloting a vehicle?
    vehicle* Vehicle = IsVehiclePilot() ;
    if (Vehicle)
    {
        // Hand move over to vehicle?
        if ((m_ProcessInput) || (tgl.pServer))
        {
            // Move
            Vehicle->ApplyMove(this, m_VehicleSeat, m_Move) ;

            // Put vehicle dirty bits into the player
            m_DirtyBits |= Vehicle->GetDirtyBits() ;
        }

        // Ignore move input
        bIgnoreMoveInput = TRUE ;

        // Ignore rot input
        m_Rot.Zero() ;
        bIgnoreLookInput = TRUE ;
    }

    // Passenger on a vehicle?
    Vehicle = IsVehiclePassenger() ;
    if (Vehicle)
    {
        // Ignore movement input
        bIgnoreMoveInput = TRUE ;
/*
        // Check for using vehicle weapon rot
        const vehicle::seat& Seat = Vehicle->GetSeat(m_VehicleSeat) ;
        switch(Seat.Type)
        {
            case vehicle::SEAT_TYPE_PASSENGER:
                
                // Stop look?
                if (!Seat.CanLookAndShoot)
                {
                    // Clear rotation and stop input
                    m_Rot.Zero() ;
                    bIgnoreLookInput = TRUE ;
                }
                break ;
          
            case vehicle::SEAT_TYPE_GUNNER:
            case vehicle::SEAT_TYPE_BOMBER:

                // Lookup turret speed scalers
                LookSpeedScaler = Vehicle->GetTurretSpeedScaler() ;
                LookMinPitch    = Vehicle->GetTurretMinPitch() ;
                LookMaxPitch    = Vehicle->GetTurretMaxPitch() ;
                break ;
        }
*/
    }
    
    // Clear auto aim target
    m_AutoAimTargetID = obj_mgr::NullID ;

    // Look 
    if( m_TargetIsLocked )
    {
        if( !m_NetMoveUpdate )
            m_LockAimSolved = FALSE;

        object* pTarget = ObjMgr.GetObjectFromID( m_LockedTarget );

        // If target is a player in a vehicle, then shoot at the vehicle itself.
        if( pTarget && (pTarget->GetAttrBits() & object::ATTR_PLAYER) )
        {
            player_object* pTargetPlayer = (player_object*)pTarget;
            vehicle*       pVehicle      = pTargetPlayer->IsAttachedToVehicle();

            if( pVehicle )  
                pTarget = (object*)pVehicle;
        }

        if( pTarget )
        {
            radian  Pitch;
            radian  Yaw;

            if( ComputeAutoAim( this, pTarget, TRUE, m_AutoAimScore, m_AutoAimTargetCenter, m_AutoAimDir, m_AutoAimPos, m_AutoAimArcPivot ) )
            {
                bIgnoreLookInput = TRUE ;

                // Keep ID of target for hud drawing
                m_AutoAimTargetID = pTarget->GetObjectID() ;

                // Make relative to ground orientation
                // (which will be the vehicle rotation if the player is in one)
                if (m_GroundOrientBlend != 0)
                {
                    // Get inverse of ground rotation
                    quaternion qInvGroundRot = BlendToIdentity(m_GroundRot, 1-m_GroundOrientBlend) ; // 0 = ident, 1 = ground
                    qInvGroundRot.Invert() ;

                    m_AutoAimDir = qInvGroundRot.Rotate(m_AutoAimDir) ;
                }

                m_AutoAimDir.GetPitchYaw( Pitch, Yaw );

                if( ((m_WorldPos - m_AutoAimTargetCenter).LengthSquared() < 25.0f) || 
                    (!IN_RANGE( -R_85, Pitch, R_85 )) )
                {
                    // Angle too extreme or target too close.  Break the target lock.
                    m_TargetIsLocked = FALSE;
                    m_LockedTarget   = obj_mgr::NullID;
                    m_DirtyBits     |= PLAYER_DIRTY_BIT_TARGET_LOCK;
                           
                    // Audio for server.
                    if( m_ProcessInput )
                        audio_Play( SFX_TARGET_LOCK_LOST );
                }
                else
                {   
                    m_Rot.Pitch = Pitch;
                    m_Rot.Yaw   = Yaw;

                    if( !m_NetMoveUpdate )
                        m_LockAimSolved = TRUE;
                }
            }
        }
    }

    //==========================================================================
    // TRIBES2 PC PHYSICS
	//==========================================================================

    f32 mWaterCoverage = 0 ;
    f32 mDrag=0;
    f32 mBuoyancy=0 ;
    f32 mGravityMod=0 ;

    // Get head rot
    matrix4 yRot ;
    yRot.Setup(radian3(0, m_Rot.Yaw, 0)) ;

    vector3 Col0, Col1, Col2 ;
    yRot.GetColumns(Col0, Col1, Col2) ;

    // Desired move direction & speed
    vector3 moveVec;
    f32 moveSpeed;
    if (!bIgnoreMoveInput)
    {
        moveVec = Col0 ;
        moveVec *= m_Move.MoveX ;
        vector3 tv;
        tv = Col2 ;
        moveVec += tv * m_Move.MoveY ;

        if (m_Move.MoveY > 0)
        {
            if( mWaterCoverage >= 0.9 )
            {
                moveSpeed = MAX(m_MoveInfo->MAX_UNDERWATER_FORWARD_SPEED * m_Move.MoveY,
                              m_MoveInfo->MAX_UNDERWATER_SIDE_SPEED * ABS(m_Move.MoveX));
            }
            else
            {
                moveSpeed = MAX(m_MoveInfo->MAX_FORWARD_SPEED * m_Move.MoveY,
                                 m_MoveInfo->MAX_SIDE_SPEED * ABS(m_Move.MoveX));
            }
        }
        else
        {
            if( mWaterCoverage >= 0.9 )
            {
                moveSpeed = MAX(m_MoveInfo->MAX_UNDERWATER_BACKWARD_SPEED * ABS(m_Move.MoveY),
                                 m_MoveInfo->MAX_UNDERWATER_SIDE_SPEED * ABS(m_Move.MoveX));
            }
            else
            {
                moveSpeed = MAX(m_MoveInfo->MAX_BACKWARD_SPEED * ABS(m_Move.MoveY),
                                 m_MoveInfo->MAX_SIDE_SPEED * ABS(m_Move.MoveX));
            }
        }
    }
    else
    {
      moveVec.Set(0,0,0);
      moveSpeed = 0;
    }

    // Acceleration due to gravity
    vector3 acc(0, s_CommonInfo.GRAVITY * DeltaTime, 0) ;

    // Acceleration on run surface
    if (m_ContactInfo.RunSurface)
    {
        m_LastContactTime = 0;

        // Remove gravity if on a surface that can be run upon!
        if (m_ContactInfo.RunSurface)
        {
            // Remove acc into contact surface (should only be gravity)
            // Clear out floating point acc errors, this will allow
            // the player to "rest" on the ground.
            f32 vd = -v3_Dot(acc, m_ContactInfo.RunNormal);
            if (vd > 0)
            {
                vector3 dv = m_ContactInfo.RunNormal * (vd + 0.002f);
                acc += dv;
                if (acc.Length() < 0.0001f)
                    acc.Set(0,0,0);
            }
        }

        // Adjust the players's requested dir. to be parallel
        // to the contact surface.
        vector3 pv = moveVec ;
        f32 pvl = pv.Length() ;
        if (pvl > 0)
        {
            vector3 nn;
            nn = v3_Cross(pv, vector3(0,1,0)) ;
            nn *= 1.0f / pvl;
            vector3 cv = m_ContactInfo.RunNormal ;
            cv -= nn * v3_Dot(nn,cv);
            pv -= cv * v3_Dot(pv,cv);
            pvl = pv.Length();
        }

        // Convert to acceleration
        if (pvl > 0)
            pv *= moveSpeed / pvl;
        
        vector3 runAcc   = pv - (m_Vel + acc) ;
        f32     runSpeed = runAcc.Length();

        // Clamp acceleratin, player also accelerates faster when
        // in his hard landing recover state.
        f32 maxAcc = (m_MoveInfo->RUN_FORCE / GetMass()) * DeltaTime ;
        if (m_PlayerState == PLAYER_STATE_LAND)
            maxAcc *= m_MoveInfo->RECOVER_RUN_FORCE_SCALE ;
        if (runSpeed > maxAcc)
            runAcc *= maxAcc / runSpeed;
        acc += runAcc;
    }
    else
        m_LastContactTime += DeltaTime ;

    // Acceleration from Jetting
    m_Jetting = (m_Move.JetKey)
        && ((m_Energy >= m_MoveInfo->MIN_JET_ENERGY)
            || (GetType() == object::TYPE_BOT))
        && (!bIgnoreMoveInput) ;
    
    if (m_Jetting)
    {
        f32 JetEnergyDrain ;

        if( mWaterCoverage >= 0.9 )
             JetEnergyDrain = m_MoveInfo->UNDERWATER_JET_ENERGY_DRAIN * 30 ;
        else
             JetEnergyDrain = m_MoveInfo->JET_ENERGY_DRAIN ;

        JetEnergyDrain *= DeltaTime ;

        // Drain the energy
        AddEnergy(-JetEnergyDrain) ;

        // adjust this players heat signature
        SetHeat(GetHeat() + (m_MoveInfo->HEAT_INCREASE_PER_SEC * DeltaTime)) ;

        // Desired jet direction
        vector3 jv = moveVec;
        vector3 yv = Col1;

        f32 jetForce = m_MoveInfo->JET_FORCE;

        if( mWaterCoverage >= 0.9 )
        {
            jetForce = m_MoveInfo->UNDERWATER_JET_FORCE;
        }

        // Build jet velocity to always have an upward component, if
        // we are at max velocity, the jet thrusts straight up.
        f32 jvl = jv.Length();
        if ((jvl > 0.0f) && (m_JumpSurfaceLastContactTime >= JUMP_SKIP_CONTACTS_MAX))
        {
            // PS2 BUGFIX - Keep the analog input - only normalize if bigger than one!!!
            f32 hacc = jvl ;
            if (jvl > 1.0f)
            {
                jv *= 1.0f / jvl;
                hacc = 1.0f ;
            }

            // If going max speed forward, then turn into vertical thrust
            f32 dot  = v3_Dot( m_Vel, jv );
            if( dot > 0.0f )
                if (dot > m_MoveInfo->MAX_JET_FORWARD_SPEED)
                   hacc = 0.0f;
                else
                   hacc -= hacc * (dot / m_MoveInfo->MAX_JET_FORWARD_SPEED);  // PS2 BUGFIX - Keep analog!
                   //hacc = 1.0f - (dot / m_MoveInfo->MAX_JET_FORWARD_SPEED);

            if (hacc > m_MoveInfo->MAX_JET_HORIZONTAL_PERCENTAGE)
                hacc = m_MoveInfo->MAX_JET_HORIZONTAL_PERCENTAGE;

            jv *= hacc * jetForce;
            jv += yv * (1.0f - hacc) * jetForce;
            if ( GetType() == player_object::TYPE_BOT )
            {
                jv.Y = jetForce;
            }
        }
        else
            jv.Set( 0.0f, jetForce, 0.0f );


         // Get drag
        f32 sWaterDensity=0;
        f32 sWaterViscosity=0 ;
        f32 sWaterCoverage=0 ;

        // Apply drag
        if (mWaterCoverage >= 0.1f)
        {
            mDrag     = m_MoveInfo->DRAG * sWaterViscosity * sWaterCoverage;
            mBuoyancy = (sWaterDensity / m_MoveInfo->DENSITY) * sWaterCoverage;
        }

        if (mDrag != 0.0f && mWaterCoverage > 0.25f)
        {
            // If we are submerged, then we get a bit of extra boost, just enough to counteract
            //  most of the vertical component of drag...
            //f32 jfRatio = m_MoveInfo->JET_FORCE / m_MoveInfo->UNDERWATER_JET_FORCE * m_MoveInfo->UNDERWATER_VERT_JET_FACTOR;
            //vector3 vertDragVel(1, jfRatio, 1);
            //jv *= vertDragVel;
        }

      // Update velocity
      acc += jv * (DeltaTime / GetMass());
   }
   
   // if player is submerged in water, release all heat
   if( mWaterCoverage > 0.5 )
      SetHeat( 0 ) ;
   
   // Acceleration from Jumping?
   if (     (m_Move.JumpKey) &&
            (!bIgnoreMoveInput) &&
            (m_JumpSurfaceLastContactTime < JUMP_SKIP_CONTACTS_MAX) &&
            (m_LandRecoverTime == 0) )
   {
        // Scale the jump impulse base on maxJumpSpeed
        f32 ySpeedScale = m_Vel.Y;
        if (ySpeedScale <= m_MoveInfo->MAX_JUMP_SPEED)
        {
            m_Jumping = TRUE ;

            ySpeedScale = (ySpeedScale <= m_MoveInfo->MIN_JUMP_SPEED)? 1:
                            1 - (ySpeedScale - m_MoveInfo->MIN_JUMP_SPEED) /
                            (m_MoveInfo->MAX_JUMP_SPEED - m_MoveInfo->MIN_JUMP_SPEED);

            // Desired jump direction
            vector3 pv = moveVec;
            f32 len = pv.Length();
            if (len > 0)
                pv *= 1 / len;

            // If we are facing into the surface jump up, otherwise
            // jump away from surface.
            f32 dot = v3_Dot(pv, m_ContactInfo.JumpNormal) ;
            f32 impulse = m_MoveInfo->JUMP_FORCE / GetMass();
            if (dot <= 0)
                acc.Y += m_ContactInfo.JumpNormal.Y * impulse * ySpeedScale;
            else
            {
                acc.X += pv.X * impulse * dot;
                acc.Y += m_ContactInfo.JumpNormal.Y * impulse * ySpeedScale;
                acc.Z += pv.Z * impulse * dot;
            }

            /*
            f32      speed = m_Vel.Length();
            xbool    doAction = TRUE;
            u32      jumpAction;
            xbool      mCanSki=FALSE ;
            if (mCanSki)
            {
                if (speed < sJumpingThreshold)
                {
                    mCanSki = FALSE;
                }
                else
                {
                    // S32   impactSound;
                    if (speed < sMinSkiSpeed)
                    {
                        // Just use run anim which will happen in pickActionThread()
                        doAction = FALSE;
                        mSkiTimer = 0;
                        // impactSound = PlayerData::ImpactNormal;
                    }
                    else
                    {
                        mSkiTimer = sSkiDuration;
                        jumpAction = m_MoveInfo->skiAction;
                        if (isServerObject())
                        {
                            mImpactSound = PlayerData::ImpactSki;
                            setMaskBits(ImpactMask);
                        }
                    }
                }
            }
            else 
                mCanSki = TRUE;

            if (doAction)
            setActionThread(jumpAction, TRUE, FALSE, TRUE);
            mJumpSurfaceLastContact = JumpSkipContactsMax;
            */
            
            // Stop jumping next time...
            m_JumpSurfaceLastContactTime = JUMP_SKIP_CONTACTS_MAX ;
        }
    }
    else
    // Update jump contact time
    if (m_ContactInfo.JumpSurface)
    {
        m_JumpSurfaceLastContactTime = 0 ;
        m_Jumping                    = FALSE ;
    }        
    else
    {
        m_JumpSurfaceLastContactTime += DeltaTime ;
        if (m_JumpSurfaceLastContactTime > JUMP_SKIP_CONTACTS_MAX)
            m_JumpSurfaceLastContactTime = JUMP_SKIP_CONTACTS_MAX ;
    }


   //if (mJumpDelay > 0)
      //mJumpDelay--;
   //else if (m_ContactInfo.Jump && !move->trigger[2])
      //mCanSki = FALSE;
      
   //if (mSkiTimer > 0)
      //mSkiTimer--;
   
   // Add in force from physical zones...
   //acc += (mAppliedForce / GetMass()) * TickSec;

    // Adjust velocity with all the move & gravity acceleration
    // TG: I forgot why doesn't the TickSec multiply happen here...
    m_Vel += acc;

    // apply horizontal air resistance
    f32 hvel = x_sqrt((m_Vel.X * m_Vel.X) + (m_Vel.Z * m_Vel.Z));
   
    if(hvel > m_MoveInfo->HORIZ_RESIST_SPEED)
    {
        f32 speedCap = hvel;
        if(speedCap > m_MoveInfo->HORIZ_MAX_SPEED)
            speedCap = m_MoveInfo->HORIZ_MAX_SPEED;
        speedCap -= m_MoveInfo->HORIZ_RESIST_FACTOR * DeltaTime * (speedCap - m_MoveInfo->HORIZ_RESIST_SPEED);
        f32 scale = speedCap / hvel;
        m_Vel.X *= scale;
        m_Vel.Z *= scale;
    }
    if(m_Vel.Y > m_MoveInfo->UP_RESIST_SPEED)
    {
        if(m_Vel.Y > m_MoveInfo->UP_MAX_SPEED)
            m_Vel.Y = m_MoveInfo->UP_MAX_SPEED;
        m_Vel.Y -= m_MoveInfo->UP_RESIST_FACTOR * DeltaTime * (m_Vel.Y - m_MoveInfo->UP_RESIST_SPEED);
    }
   
    // Container buoyancy & drag
    if (mBuoyancy != 0) 
    {     // Applying buoyancy when standing still causing some jitters- 
        if (mBuoyancy > 1.0 || !m_Vel.LengthSquared() == 0 || !m_ContactInfo.RunSurface)
        m_Vel.Y -= mBuoyancy * s_CommonInfo.GRAVITY * mGravityMod * DeltaTime;
    }
    m_Vel   -= m_Vel * mDrag * DeltaTime;

    //if (bIgnoreMoveInput)
    //{
        //m_Vel.X = 0.0f;
        //m_Vel.Z = 0.0f;
    //}

    // If we are not touching anything and have sufficient -z vel,
    // we are falling.
    if (m_ContactInfo.RunSurface)
        m_Falling = FALSE;
    else
    {
        //vector3 vel;
        //mWorldToObj.mulV(m_Vel,&vel);
        m_Falling = m_Vel.Y < FALLING_THRESHOLD ;
    }
       
   /*
   if (!isGhost()) {
      // Vehicle Dismount
      if(move->trigger[2] && isMounted())
         Con::executef(mDataBlock,2,"doDismount",scriptThis());
    
      if(!inLiquid && mWaterCoverage != 0.0f) {
         Con::executef(mDataBlock,4,"onEnterLiquid",scriptThis(), Con::getFloatArg(mWaterCoverage), Con::getIntArg(mLiquidType));
         inLiquid = TRUE;
      }
      else if(inLiquid && mWaterCoverage == 0.0f) {
         Con::executef(mDataBlock,3,"onLeaveLiquid",scriptThis(), Con::getIntArg(mLiquidType));
         inLiquid = FALSE;
      }
   }
   else {
      if(!inLiquid && mWaterCoverage >= 1.0f) {
      
         inLiquid = TRUE;
      }   
      else if(inLiquid && mWaterCoverage < 0.8f) {
         if(getVelocity().Length() >= m_MoveInfo->exitSplashSoundVel && !isMounted())
            alxPlay(m_MoveInfo->sound[PlayerData::ExitWater], &getTransform());
         inLiquid = FALSE;
      }
   }
    */

    // Update recover time
    m_LandRecoverTime -= DeltaTime ;
    if (m_LandRecoverTime < 0)
        m_LandRecoverTime = 0 ;

    // Decay the heat on the player
    if( !m_Jetting )
    {
        f32 CurrentHeat = GetHeat();
        if (CurrentHeat > 0)
        {
            CurrentHeat -= m_MoveInfo->HEAT_DECAY_PER_SEC * DeltaTime ;
            if (CurrentHeat < 0.0)
                CurrentHeat = 0.0 ;
            SetHeat(CurrentHeat) ;
        }
    }   
   
    // Choose appropriate moving state
    if (!bIgnoreMoveInput)
        Player_ChooseMovingState( DeltaTime ) ;

    //==========================================================================
	// Apply look input
	//==========================================================================

    f32 ZoomFactor = GetCurrentZoomFactor() ;

    if( Vehicle )
    {
        if( Vehicle->GetType() == object::TYPE_VEHICLE_THUNDERSWORD )
        {
            ZoomFactor = 1.0f;
        }
    }

	// Apply look - to free look?
	if (m_Move.FreeLookKey)
	{
        // Allow complete rotation just like normal play
		m_ViewFreeLookYaw  += m_Move.LookX * R_360 * DeltaTime ;
	}		
	else
    {
        // Remove free look
		if (m_ViewFreeLookYaw > 0.0f)
		{
			m_ViewFreeLookYaw -= s_CommonInfo.VIEW_BLEND_SPEED * DeltaTime ;
			if (m_ViewFreeLookYaw < 0.0f)
				m_ViewFreeLookYaw = 0.0f ;
		}
		else
		if (m_ViewFreeLookYaw < 0.0f)
		{
			m_ViewFreeLookYaw += s_CommonInfo.VIEW_BLEND_SPEED * DeltaTime ;
			if (m_ViewFreeLookYaw > 0.0f)
				m_ViewFreeLookYaw = 0.0f ;
		}

        // Look left/right
        if (!bIgnoreLookInput)
	    {
		    // Rotate player
            if (m_Move.LookX != 0)
            {
                if( ZoomFactor == 1 )
                    m_Rot.Yaw += m_Move.LookX * R_360 * LookSpeedScaler * DeltaTime;
                else
                    m_Rot.Yaw += m_Move.LookX * R_360 * LookSpeedScaler * DeltaTime / ((ZoomFactor+1)/3);
            }

	    }
    }

    // Look?
    if (!bIgnoreLookInput)
    {
	    // Look up and down
        if (m_Move.LookY != 0)
        {
            if( ZoomFactor == 1 )
        	    m_Rot.Pitch += m_Move.LookY * R_360 * LookSpeedScaler * DeltaTime;
            else
        	    m_Rot.Pitch += m_Move.LookY * R_360 * LookSpeedScaler * DeltaTime / ((ZoomFactor+1)/3)  ;
        }

        if( IsDead() )
            LookMinPitch = R_30;

        // Keep within limits
	    if (m_Rot.Pitch > LookMaxPitch)
		    m_Rot.Pitch = LookMaxPitch ;
	    else
	    if (m_Rot.Pitch < LookMinPitch)
		    m_Rot.Pitch = LookMinPitch ;

        // Add anim motion
        m_Rot += m_AnimMotionDeltaRot ;
    }

    //==========================================================================
    // Update vehicle turret rot if controlling one
    //==========================================================================
    Vehicle = IsVehiclePassenger() ;
    if (Vehicle)
    {
/*        
        // Check for using vehicle weapon rot
        const vehicle::seat& Seat = Vehicle->GetSeat(m_VehicleSeat) ;

        switch(Seat.Type)
        {
            case vehicle::SEAT_TYPE_GUNNER:
            case vehicle::SEAT_TYPE_BOMBER:

                // Plop players rotation into the vehicles turret rot!
                if ((tgl.pServer) || (m_ProcessInput))
                    Vehicle->SetTurretRot(m_Rot) ;
                break ;
        }
*/

        // Hand move over to vehicle? (checks for firing turrets, dropping bombs etc)
        if ((tgl.pServer) || (m_ProcessInput))
            Vehicle->ApplyMove(this, m_VehicleSeat, m_Move) ;
    }

    //==========================================================================
    // Truncate so server and client controller players act exactly the same
    //==========================================================================
    Truncate() ;

    //==========================================================================
    // Set dirty bits
    //==========================================================================
    if (m_WorldPos != LastPos)
        m_DirtyBits |= PLAYER_DIRTY_BIT_POS	;

    if (m_Vel != LastVel)
        m_DirtyBits |= PLAYER_DIRTY_BIT_VEL	;
    
    if (m_Rot != LastRot)
        m_DirtyBits |= PLAYER_DIRTY_BIT_ROT ;

    if (m_ViewFreeLookYaw != LastViewFreeLookYaw)
        m_DirtyBits |= PLAYER_DIRTY_BIT_ROT ;

    if (m_Energy != LastEnergy)
        m_DirtyBits |= PLAYER_DIRTY_BIT_ENERGY ;

    DEBUG_END_TIMER(ApplyMove)
}

//==============================================================================

static f32 BaseMine  =  5.0f;
static f32 MoreMine  = 50.0f;

static f32 BaseFlare = 25.0f;

// Checks for throwing and using weapons, grenades, packs etc
void player_object::CheckForThrowingAndFiring( f32 DeltaTime )
{
    // Do exchange pickup logic
    CheckForExchangePickup() ;

    //==========================================================================
    // Throw shit and fire weapon
	//==========================================================================

    // Only do if on server and player is alive
    if ((tgl.ServerPresent) && (m_Health > 0))
    {
    	//======================================================================
        // Get world rotation (which takes into account being on vehicle)
    	//======================================================================

        f32 WorldPitch, WorldYaw ;
        GetWorldPitchYaw( WorldPitch, WorldYaw );

    	//======================================================================
        // Setup useful flags
    	//======================================================================
        xbool CanFire  = (IsDead() == FALSE) ;  // Fire weapon
        xbool CanDrop  = TRUE ;                 // Drop weapons/packs etc
        xbool CanThrow = TRUE ;                 // Throw mines+grenades
        
        vehicle* Vehicle = IsAttachedToVehicle() ;
        
        // Disable the ability to drop grenades when at inven stations
        if( GetArmed() == FALSE )
        {
            CanFire = CanDrop = CanThrow = FALSE;
        }
        
        // Attached to a vehicle?
        if (Vehicle)
        {
            // Lookup disabling of firing/dropping/throwing from seat info
            const vehicle::seat& Seat = Vehicle->GetSeat(m_VehicleSeat) ;
            CanFire = CanDrop = CanThrow = Seat.CanLookAndShoot ;
        
            // Allow player to throw flares from vehicle
            if( GetInventCount( INVENT_TYPE_GRENADE_FLARE ) > 0 )
                CanThrow = TRUE;
        }

        // SB - Bug fixed:
        // If player cannot throw, then reset grenade and mine throw times to
        // avoid them being throw straight away, when they can throw again.
        // eg. when jumping out of pilotting a vehicle, at invent station etc.
        if (!CanThrow)
        {
            m_GrenadeKeyTime = 0 ;
            m_MineKeyTime    = 0 ;
        }

    	//======================================================================
        // Check for throwing grenades
        //======================================================================

        // Update grenade key held down time
        if (m_Move.GrenadeKey)
            m_GrenadeKeyTime += DeltaTime ;

        // Update grenade delay time
        if (m_GrenadeDelayTime > 0)
        {
            m_GrenadeDelayTime -= DeltaTime ;
            if (m_GrenadeDelayTime < 0)
                m_GrenadeDelayTime = 0 ;
        }

        // Flag that we want to throw a grenade?
        if ( (!m_Move.GrenadeKey) && (m_GrenadeKeyTime > 0) && (CanThrow) )
        {
            // Only record the request if the time delay since the last grenade has elapsed
            // (comment this line out if you want to queue the last grenade request)
            if (m_GrenadeDelayTime == 0)
            {
                // Flag that we want to throw a grenade
                m_GrenadeThrowKeyTime = m_GrenadeKeyTime ;
            }

            // Clear time so user has to press again to throw
            m_GrenadeKeyTime = 0 ;
        }

        // Throw a grenade?
        if ( (m_GrenadeThrowKeyTime > GRENADE_THROW_MIN_TIME) && (m_GrenadeDelayTime == 0) && (CanThrow) )
        {
            // Delay before player can throw again
            m_GrenadeDelayTime = GRENADE_THROW_DELAY_TIME ;

            // Throw which type of grenade?
            grenade::type Type = grenade::TYPE_UNDEFINED;
                 if (GetInventCount(INVENT_TYPE_GRENADE_BASIC) > 0)      Type = grenade::TYPE_BASIC;
            else if (GetInventCount(INVENT_TYPE_GRENADE_FLASH) > 0)      Type = grenade::TYPE_FLASH;
            else if (GetInventCount(INVENT_TYPE_GRENADE_CONCUSSION) > 0) Type = grenade::TYPE_CONCUSSION;
            else if (GetInventCount(INVENT_TYPE_GRENADE_FLARE) > 0)      Type = grenade::TYPE_FLARE;

            // Can throw flares 4 times faster than other types.
            if( Type == grenade::TYPE_FLARE )
                m_GrenadeDelayTime *= 0.25f;

            // Throw it?
            if ( Type != grenade::TYPE_UNDEFINED )
            {
                f32 BaseSpeed = 30.0f;

                // Calc hold ratio (0=min time, 1=max time)
                f32 HoldRatio = (m_GrenadeThrowKeyTime - GRENADE_THROW_MIN_TIME) / (GRENADE_THROW_MAX_TIME - GRENADE_THROW_MIN_TIME) ;
                if (HoldRatio > 1)
                    HoldRatio = 1 ;

                vector3 Position = m_WorldPos;
                Position.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.65f;

                vector3 WorldDir( WorldPitch, WorldYaw );
                vector3 Inherit;

                if( Vehicle )   Inherit = Vehicle->GetVel();
                else            Inherit = m_Vel;

                vector3 MoveDir( Inherit );
                MoveDir.Normalize();

                vector3 Direction;

                if( Type == grenade::TYPE_FLARE )
                {
                    BaseSpeed = BaseFlare;
                    Direction.Set( WorldPitch - R_15, WorldYaw + x_frand( -R_15, R_15 ) );
                }
                else
                {
                    f32 Dot = v3_Dot( MoveDir, WorldDir );
                    if( Dot < 0.0f )
                        Dot = 0.0f;
                    Inherit *= Dot;
                    Direction.Set( WorldPitch - R_10, WorldYaw );
                }

                // Update invent
                switch( Type )
                {
                    case grenade::TYPE_BASIC:       AddInvent(INVENT_TYPE_GRENADE_BASIC, -1) ;      break;                        
                    case grenade::TYPE_FLASH:       AddInvent(INVENT_TYPE_GRENADE_FLASH, -1) ;      break;
                    case grenade::TYPE_CONCUSSION:  AddInvent(INVENT_TYPE_GRENADE_CONCUSSION, -1) ; break;
                    case grenade::TYPE_FLARE:       AddInvent(INVENT_TYPE_GRENADE_FLARE, -1) ;      break;

                    default:
                        ASSERT(0);
                        break;
                }

		        // Throw grenade!

		        grenade* pGrenade = (grenade*)ObjMgr.CreateObject( object::TYPE_GRENADE );
		        pGrenade->Initialize( Position, Direction, Inherit, m_ObjectID, 
                                      m_TeamBits, BaseSpeed + (HoldRatio * 20.0f), Type );
		        ObjMgr.AddObject( pGrenade, obj_mgr::AVAILABLE_SERVER_SLOT );
            }

            // Clear time so user has to press again to throw
            m_GrenadeThrowKeyTime = 0 ;
        }

    	//======================================================================
        // Check for throwing mines
        //======================================================================

        // Update mine key held down time
        if (m_Move.MineKey)
            m_MineKeyTime += DeltaTime ;

        // Update mine delay time
        if (m_MineDelayTime > 0)
        {
            m_MineDelayTime -= DeltaTime ;
            if (m_MineDelayTime < 0)
                m_MineDelayTime = 0 ;
        }

        // Flag that we want to throw a mine?
        if ( (!m_Move.MineKey) && (m_MineKeyTime > 0) && (CanThrow) )
        {
            // Only record the request if the time delay since the last mine has elapsed
            // (comment this line out if you want to queue the last mine request)
            if (m_MineDelayTime == 0)
            {
                // Flag that we want to throw a mine
                m_MineThrowKeyTime = m_MineKeyTime ;
            }

            // Clear time so user has to press again to throw
            m_MineKeyTime = 0 ;
        }

        // Throw a mine?
        if ( (m_MineThrowKeyTime > MINE_THROW_MIN_TIME) && (m_MineDelayTime == 0) && (CanThrow) )
        {
            // Delay before player can throw again
            m_MineDelayTime = MINE_THROW_DELAY_TIME ;

            // Does player have any mines?
            if (GetInventCount(INVENT_TYPE_MINE) > 0)
            {
                // Calc hold ratio (0=min time, 1=max time)
                f32 HoldRatio = (m_MineThrowKeyTime - MINE_THROW_MIN_TIME) / (MINE_THROW_MAX_TIME - MINE_THROW_MIN_TIME) ;
                if (HoldRatio > 1)
                    HoldRatio = 1 ;

                // Calc dir and pos for throwing objects
                vector3 Direction( WorldPitch - R_10, WorldYaw );
                vector3 Position = m_WorldPos;
                Position.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.75f ;

                // Update invent
                AddInvent(INVENT_TYPE_MINE, -1) ;

                // Throw mine
                vector3 WorldDir( WorldPitch, WorldYaw );
                vector3 MoveDir( m_Vel );
                MoveDir.Normalize();
                vector3 Inherit = m_Vel * v3_Dot( MoveDir, WorldDir );
		        mine* pMine = (mine*)ObjMgr.CreateObject( object::TYPE_MINE );
		        pMine->Initialize( Position, Direction, Inherit, m_ObjectID, m_TeamBits, BaseMine + HoldRatio * MoreMine );
		        ObjMgr.AddObject( pMine, obj_mgr::AVAILABLE_SERVER_SLOT );
            }

            // Clear time so user has to press again to throw
            m_MineThrowKeyTime = 0 ;
        }

    	//======================================================================
        // Check for deploying beacons
        //======================================================================
        if( (m_Move.DropBeaconKey) && (CanThrow) )             
        {
            vector3      Point;
            vector3      Normal;
            object::type ObjType;

            if( GetDeployAttemptData( Point, Normal, ObjType ) )
            {
                if( ObjType == object::TYPE_BEACON )
                {  
                    // Attempt toggle.  
                    // Don't need a beacon in current inventory for this.
                    GameMgr.AttemptDeployBeacon( m_ObjectID,
                                                 Point,
                                                 Normal,
                                                 ObjType );
                }
                else
                if(  GetInventCount( INVENT_TYPE_BELT_GEAR_BEACON ) > 0 )
                {
                    // Attempt deploy.
                    // Must have a beacon in inventory.
                    if( GameMgr.AttemptDeployBeacon( m_ObjectID,
                                                     Point,
                                                     Normal,
                                                     ObjType ) )
                    {
                        AddInvent( INVENT_TYPE_BELT_GEAR_BEACON, -1 );
                    }
                }
            } 

            // clear it
            m_Move.DropBeaconKey = 0;
        }

    	//======================================================================
        // Check for switching to targeting laser
    	//======================================================================
        if ( m_Move.TargetingLaserKey )
        {
            m_Move.TargetingLaserKey = 0;
            m_WeaponRequestedType = INVENT_TYPE_WEAPON_TARGETING_LASER;
        }

    	//======================================================================
        // Drop weapon?
    	//======================================================================
        if ( (m_Move.DropWeaponKey) && (m_Armed) && (CanDrop) )
        {
            // Stop weapon selection
            m_Move.NextWeaponKey = FALSE ;
            m_Move.PrevWeaponKey = FALSE ;

            // Holding a weapon, and one that can be dropped?
            if (        (m_WeaponCurrentType != INVENT_TYPE_NONE)
                    &&  (GetInventCount(m_WeaponCurrentType) > 0) )
            {
                // Throw from shoulders
                vector3 Pos = m_WorldPos;
                Pos.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.75f ;

                // Choose random xz direction, always move up
                vector3 Vel(0, 0, 10) ;                 // Vel
                Vel.RotateX(WorldPitch - R_45) ;    // Throw up
                Vel.RotateY(WorldYaw) ;             // Throw forward
                Vel += m_Vel ;                          // Add on players velocity

                // Create the pickup
                CreatePickup( Pos, 
                              Vel,
                              s_InventInfo[m_WeaponCurrentType].PickupKind,
                              pickup::flags(pickup::FLAG_ANIMATED), 
                              20 ) ;

                // Remove from inventory
                AddInvent(m_WeaponCurrentType, -1) ;

                // Request the weapon in the same slot since it may have been 
                // updated to "no weapon"..
                m_WeaponRequestedType = m_Loadout.Weapons[m_WeaponCurrentSlot] ;
            }
        }

    	//======================================================================
        // Drop flag?
    	//======================================================================
        if ( (m_Move.DropFlagKey) && (CanDrop) )
        {
            pGameLogic->ThrowFlags( m_ObjectID );
        }

    	//======================================================================
        // Drop pack?
    	//======================================================================
        if ( (m_Move.DropPackKey) && (CanDrop) )
        {
            // Cancel use of pack
            m_Move.PackKey = FALSE ;

            // Holding a pack?
            if (m_PackCurrentType != INVENT_TYPE_NONE)
            {
                // Throw from shoulders
                vector3 Pos = m_WorldPos;
                Pos.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.75f ;

                // Choose random xz direction, always move up
                vector3 Vel(0, 0, 10) ;                 // Vel
                Vel.RotateX(WorldPitch - R_45) ;    // Throw up
                Vel.RotateY(WorldYaw) ;             // Throw forward
                Vel += m_Vel ;                          // Add on players velocity

                // Create the pickup
                CreatePickup( Pos, 
                              Vel,
                              s_InventInfo[m_PackCurrentType].PickupKind,
                              pickup::flags(pickup::FLAG_ANIMATED), 
                              20 ) ;

                // Remove from inventory
                AddInvent(m_PackCurrentType, -1) ;

                // No longer use a pack (call this function so sounds are turned off etc)
				SetPackCurrentType( INVENT_TYPE_NONE ) ;
            }
        }

    	//======================================================================
        // Use health kit?
    	//======================================================================
        if (        (m_Move.HealthKitKey)
                 && (GetInventCount(INVENT_TYPE_BELT_GEAR_HEALTH_KIT) > 0)
                 && (m_Health < 100) )
        {
            // Update invent
            AddInvent(INVENT_TYPE_BELT_GEAR_HEALTH_KIT, -1) ;

            // Increase health over time
            m_HealthIncrease = 50 ;
        }

    	//======================================================================
        // Check for updating / firing weapon
        //======================================================================

        // Player armed?
        if (m_Armed)
        {
            // If player is currenly holding the laser rifle and does not have the energy pack, disable fire
            
            // JP: removed restriction that says you must equip an energy pack when using the sniper rifle
            //if ((m_WeaponCurrentType == INVENT_TYPE_WEAPON_SNIPER_RIFLE) && (m_PackCurrentType != INVENT_TYPE_ARMOR_PACK_ENERGY))
            //    CanFire = FALSE ;

            // Next/Prev weapon pressed?
            if ((m_Move.NextWeaponKey) || (m_Move.PrevWeaponKey))
            {
                // Get direction
                s32 Dir = m_Move.NextWeaponKey - m_Move.PrevWeaponKey ;

                // Clear press
                m_Move.NextWeaponKey = FALSE ;
                m_Move.PrevWeaponKey = FALSE ;

                // Clear fire press so player doesn't fire if moving off chaingun
                m_Move.FireKey = FALSE ;

                // Search all weapon slots in requested (next or prev) order
                s32         Tries  = m_Loadout.NumWeapons ;
                invent_type Weapon = INVENT_TYPE_NONE ;
                do
                {
                    // Advance to next weapon in list, wrapping if end of list is reached
                    m_WeaponCurrentSlot += Dir ;
                    if (m_WeaponCurrentSlot >= m_Loadout.NumWeapons)
                        m_WeaponCurrentSlot = 0 ;
                    else
                    if (m_WeaponCurrentSlot < 0)
                        m_WeaponCurrentSlot = m_Loadout.NumWeapons-1 ;

                    // Lookup weapon
                    Weapon = m_Loadout.Weapons[m_WeaponCurrentSlot] ;

                    // Get info
                    ASSERT(Weapon >= INVENT_TYPE_WEAPON_START) ;
                    ASSERT(Weapon <= INVENT_TYPE_WEAPON_END) ;

                    // Stop searching for ever!
                    Tries-- ;
                }
                while((Tries > 0) && (Weapon == INVENT_TYPE_NONE)) ;   // Keep going until found a weapon or looped

                // Swap to new weapon?
                m_WeaponRequestedType = m_Loadout.Weapons[m_WeaponCurrentSlot];
                ASSERT( (m_WeaponRequestedType >= INVENT_TYPE_WEAPON_START) || (m_WeaponRequestedType <= INVENT_TYPE_WEAPON_END) );

                // Get rid of current weapon
                Weapon_SetupState(WEAPON_STATE_DEACTIVATE) ;

                // Update shape ready for new weapon
                Weapon_CheckForUpdatingShape() ;
            }

            // Fire weapon?
            if ( (m_Armed) && (m_Move.FireKey) && (m_WeaponFireDelay == 0) && (CanFire) )
            {
		        // Fire that weapon
                Weapon_SetupState(WEAPON_STATE_FIRE) ;
            }
        }
    }
}

//==============================================================================

// Checks for any pickup that the player can exchange (weapons + packs) with.
void player_object::CheckForExchangePickup( void )
{
    s32 i ;

	// Only search for exchangable pickup on the server
	if( tgl.ServerPresent && (!IsDead()) )
	{
		// Update the pickup exchange kind
        pickup* pBestPickup = NULL ;
        f32		BestDistSqr = F32_MAX ;

        // Search for the closest valid pickup within the players bounding box
		pickup* pPickup ;
		f32		DistSqr ;
		ObjMgr.Select( ATTR_PICKUP, m_WorldBBox );
		while( (pPickup = (pickup*)ObjMgr.GetNextSelected()) )
		{
            // Pickup must be pickuppable
            if (!pPickup->IsPickuppable())
                continue ;

            // Must be closer than the current best
            DistSqr = (pPickup->GetPosition() - m_WorldPos).LengthSquared() ;
            if (DistSqr >= BestDistSqr)
                continue ;

            // Lookup pickup info
            const pickup::info& Info       = pPickup->GetInfo() ;
            invent_type         InventType = (invent_type)Info.InventType ;

            // Player must be able to carry this item
            if (!GetInventMaxCount(InventType))
                continue ;

            // Player must not already be holding this item
            if (GetInventCount(InventType))
                continue ;

            // Okey dokey - we have a possible candidate...

            // Is this a weapon pickup?
            if ( (InventType >= INVENT_TYPE_WEAPON_START) && (InventType <= INVENT_TYPE_WEAPON_END) )
            {
                // Check to see if any weapon slots are empty
                for (i = 0 ; i < m_Loadout.NumWeapons ; i++)
                {
                    // Empty?
                    if (m_Loadout.Weapons[i] == INVENT_TYPE_NONE)
                        break ;
                }
                
                // If all slots are full, then this is a valid exchange pickup
                if (i == m_Loadout.NumWeapons)
                {
                    // Record
                    pBestPickup = pPickup ;
                    BestDistSqr = DistSqr ;
                }
            }

            // Is this a pack pickup?
            if ( (InventType >= INVENT_TYPE_PACK_START) && (InventType <= INVENT_TYPE_PACK_END) )
            {
                // If player already has a pack, then this is a valid exchange pickup
                if (m_PackCurrentType != INVENT_TYPE_NONE)
                {
                    // Record
                    pBestPickup = pPickup ;
                    BestDistSqr = DistSqr ;
                }
            }
        }
		ObjMgr.ClearSelection();

        // Set?
        if (pBestPickup)
        {
            // Has type changed?
            if (pBestPickup->GetKind() != m_ExchangePickupKind)
            {
                // Update and send to client
                m_ExchangePickupKind = pBestPickup->GetKind() ;
                m_DirtyBits |= PLAYER_DIRTY_BIT_EXCHANGE_PICKUP ;
            }
        }
        else
        {
            // Clear - has type changed?
            if (m_ExchangePickupKind != pickup::KIND_NONE)
            {
                // Update and send to client
                m_ExchangePickupKind = pickup::KIND_NONE ;
                m_DirtyBits |= PLAYER_DIRTY_BIT_EXCHANGE_PICKUP ;
            }
        }
	}

    // Exchange pickup available?
    if( m_ExchangePickupKind != pickup::KIND_NONE )
    {
        // Get the pickup info
        const pickup::info& Info = pickup::GetInfo(m_ExchangePickupKind) ;
        
        // Show message if this is a controling player
        if (m_ProcessInput)
        {
            if( !m_InputMask.ExchangeKey )
                MsgMgr.Message( MSG_PICKUP_EXCHANGE_AVAILABLE, m_ObjectID.Slot, ARG_PICKUP, m_ExchangePickupKind );
        }

        // Exchange?
        if( (tgl.ServerPresent) && (m_Move.ExchangeKey) )
        {
            pGameLogic->WeaponExchanged( obj_mgr::NullID );
        
            // If it's a weapon pickup, simply throw away the current weapon
            if ((Info.InventType >= INVENT_TYPE_WEAPON_START) && (Info.InventType <= INVENT_TYPE_WEAPON_END))
                m_Move.DropWeaponKey = TRUE ;
            
            // If it's a pack pickup, simply throw away the current pack
            if ((Info.InventType >= INVENT_TYPE_PACK_START) && (Info.InventType <= INVENT_TYPE_PACK_END))
                m_Move.DropPackKey = TRUE ;
        }
    }
}


//==============================================================================
// Pack functions
//==============================================================================

void player_object::SetPackCurrentType( player_object::invent_type Type )
{
    // Changing type?
    if (m_PackCurrentType != Type)
    {
        // Turn off current pack (will turn off sounds etc)
        SetPackActive(FALSE) ;

        // If dropping the repair pack and the current weapon is the repair gun, change it
        if( (m_PackCurrentType == INVENT_TYPE_ARMOR_PACK_REPAIR) && 
            (m_WeaponCurrentType == INVENT_TYPE_WEAPON_REPAIR_GUN) )
            m_WeaponRequestedType = m_Loadout.Weapons[m_WeaponCurrentSlot];

        // Set new data and send to clients
        m_PackCurrentType  = Type ;
        m_DirtyBits       |= PLAYER_DIRTY_BIT_PACK_TYPE ;
    }
}

//==============================================================================

void player_object::SetPackActive( xbool Active )
{
    // Changing active?
    if (m_PackActive != Active)
    {
        // Activating?
        if (Active)
        {
            // Activating special cases
            switch(m_PackCurrentType)
            {
                case INVENT_TYPE_ARMOR_PACK_SHIELD:
                    if (m_ProcessInput)
                        audio_PlayLooping(m_ShieldPackStartSfxID, SFX_PACK_SHIELD_ACTIVATE) ;
                    else
                        audio_PlayLooping(m_ShieldPackStartSfxID, SFX_PACK_SHIELD_ACTIVATE, &m_WorldPos) ;
                    break ;
            }
        }
        else
        {
            // Deactivating special cases
            switch(m_PackCurrentType)
            {
                case INVENT_TYPE_ARMOR_PACK_SHIELD:
                    audio_StopLooping(m_ShieldPackLoopSfxID) ;

                    // TO DO - Change this to the proper shield shutdown sound
                    if (m_ProcessInput)
                        audio_Play(SFX_PACK_SHIELD_ACTIVATE);
                    else
                        audio_Play(SFX_PACK_SHIELD_ACTIVATE,&m_WorldPos);
                    break ;
            }
        }

        // Set new data and send to clients
        m_PackActive = Active ;
        m_DirtyBits  |= PLAYER_DIRTY_BIT_PACK_TYPE ;
    }
}

//==============================================================================

void player_object::CheckForUsingPacks( f32 DeltaTime ) 
{
    s32 i ;

    // Setup current pack to use from inventory
    if (tgl.ServerPresent)
    {
        // Check inventory for first item that can be carried on the players back
        invent_type Type = INVENT_TYPE_NONE ;
        for (i = INVENT_TYPE_PACK_START ; i <= INVENT_TYPE_PACK_END ; i++)
        {
            // Get this item?
            if (m_Loadout.Inventory.Counts[i])
            {
                Type = (invent_type)i ;
                break ;
            }
        }

        // Switched to new pack
        SetPackCurrentType(Type) ;

        // If player has a pack, other than the satchel charge pack, then disconnect the player
        // from any current satchel charge that he may have thrown
        if ((m_PackCurrentType != INVENT_TYPE_NONE) && (m_PackCurrentType != INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE))
            DetachSatchelCharge(TRUE) ; // Fizzle out old

        // Blow up a satchel charge? (server only)
        if ( (m_Health > 0) && (tgl.ServerPresent) && (m_Move.PackKey) && (GetSatchelCharge()) )
        {
            // Attempy to get satchel charge object
            satchel_charge* pSatchelCharge = GetSatchelCharge() ;
            ASSERT(pSatchelCharge) ;

            // If charge is successfully detonated (it may not be armed yet), clear reference to it
            if (pSatchelCharge->Detonate(satchel_charge::EXPLOSION_BIG))
            {
                m_Move.PackKey = FALSE ;        // Don't activate other packs
                DetachSatchelCharge(FALSE) ;    // No fizzle
            }
        }

        // Update pack key held down time
        if (m_Move.PackKey)
            m_PackKeyTime += DeltaTime ;

        // Turn off pack?
        if( (m_Health == 0) || (GetArmed() == FALSE) )
            SetPackActive(FALSE) ;
        else
        {
            // Do pack logic...
            switch(m_PackCurrentType)
            {
                //--------------------------------------------------------------
                case INVENT_TYPE_NONE:
                    SetPackActive(FALSE) ;
                    break ;

                //--------------------------------------------------------------
                case INVENT_TYPE_ARMOR_PACK_SHIELD:
                    // Active?
                    if (m_PackActive)
                    {
                        // Deactive?
                        if (m_Move.PackKey)
                            SetPackActive(FALSE) ;
                        else
                        {
                            // Get recharge rate
                            f32 EnergyRecharge = m_MoveInfo->RECHARGE_RATE * DeltaTime ;

                            // Eat up energy
                            AddEnergy(-EnergyRecharge * SHIELD_PACK_DRAIN_SCALER) ;

                            // If energy is at zero, turn off the pack
                            if (m_Energy == 0)
                                SetPackActive(FALSE) ;
                        }
                    }
                    else
                    {
                        // Activate?
                        if (m_Move.PackKey)
                            SetPackActive(TRUE) ;
                    }
                    break ;

                //--------------------------------------------------------------
                case INVENT_TYPE_ARMOR_PACK_ENERGY:
                    // Energy pack is always active
                    SetPackActive(TRUE) ;
                    break ;

                //--------------------------------------------------------------
                case INVENT_TYPE_DEPLOY_PACK_INVENT_STATION:
                    if( m_Move.PackKey )
                    {
                        vector3      Point;
                        vector3      Normal;
                        object::type ObjType;

                        if( GetDeployAttemptData( Point, Normal, ObjType ) )
                        {
                            if( GameMgr.AttemptDeployStation( m_ObjectID,
                                                              Point,
                                                              Normal,
                                                              ObjType ) )
                            {
                                AddInvent( INVENT_TYPE_DEPLOY_PACK_INVENT_STATION, -1 );
                            }
                        } 
                    }
                    break;

                //--------------------------------------------------------------
                case INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR:
                    if( m_Move.PackKey )
                    {
                        vector3      Point;
                        vector3      Normal;
                        object::type ObjType;

                        if( GetDeployAttemptData( Point, Normal, ObjType ) )
                        {
                            if( GameMgr.AttemptDeploySensor( m_ObjectID,
                                                             Point,
                                                             Normal,
                                                             ObjType ) )
                            {
                                AddInvent( INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR, -1 );
                            }
                        } 
                    }
                    break;

                //--------------------------------------------------------------
                case INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR:
                    if( m_Move.PackKey )
                    {
                        vector3      Point;
                        vector3      Normal;
                        object::type ObjType;

                        if( GetDeployAttemptData( Point, Normal, ObjType ) )
                        {
                            if( GameMgr.AttemptDeployTurret( m_ObjectID,
                                                             Point,
                                                             Normal,
                                                             ObjType ) )
                            {
                                AddInvent( INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR, -1 );
                            }
                        } 
                    }
                    break;

                //--------------------------------------------------------------
                case INVENT_TYPE_DEPLOY_PACK_BARREL_AA:     
                case INVENT_TYPE_DEPLOY_PACK_BARREL_ELF:    
                case INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR: 
                case INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE:
                case INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA: 
                    if( m_Move.PackKey )
                    {
                        radian3       VRot;
                        f32           VFOV;
                        vector3       VZ, VP;
                        turret::kind  Kind = turret::AA;

                        GetView( VP, VRot, VFOV );
                        VZ.Set( VRot.Pitch, VRot.Yaw );
                        VZ *= DEPLOY_DIST;

                        switch( m_PackCurrentType )
                        {
                            case INVENT_TYPE_DEPLOY_PACK_BARREL_AA:         Kind = turret::AA;      break;
                            case INVENT_TYPE_DEPLOY_PACK_BARREL_ELF:        Kind = turret::ELF;     break;
                            case INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR:     Kind = turret::MORTAR;  break;
                            case INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE:    Kind = turret::MISSILE; break;
                            case INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA:     Kind = turret::PLASMA;  break;
                            default:                                        ASSERT( FALSE );        break;
                        }

                        if( GameMgr.AttemptDeployBarrel( m_ObjectID, VP, VZ, Kind ) )
                            AddInvent( m_PackCurrentType, -1 );
                    }
                    break;

                //--------------------------------------------------------------
                case INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE:
                
                    // Throw a new satchel?
                    if ((m_Move.PackKey) && (!GetSatchelCharge()))
                    {
                        // Calc hold ratio (0=min time, 1=max time)
                        //f32 HoldRatio = 0.0f ;

                        // Get world rotation (which takes into account being on vehicle)
                        f32 WorldPitch, WorldYaw ;
                        GetWorldPitchYaw( WorldPitch, WorldYaw ) ;

                        // Calc dir and pos for throwing objects
                        vector3 Direction(WorldPitch - R_10, WorldYaw);
                        vector3 Position = m_WorldPos;
                        Position.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.75f ;

                        // Throw satchel charge
		                satchel_charge* pSatchelCharge = (satchel_charge*)ObjMgr.CreateObject( object::TYPE_SATCHEL_CHARGE );
		                pSatchelCharge->Initialize( Position, Direction, m_ObjectID, m_TeamBits ) ;
		                ObjMgr.AddObject( pSatchelCharge, obj_mgr::AVAILABLE_SERVER_SLOT );

                        // Attach to charge
                        AttachSatchelCharge(pSatchelCharge->GetObjectID()) ;

                        // Update invent
                        AddInvent( INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE, -1 );
                    }
                    break ;

                //--------------------------------------------------------------
                case INVENT_TYPE_ARMOR_PACK_REPAIR:
                    
					// Toggle repair gun/weapon?
                    if (m_Move.PackKey)
                    {
						// Force player to press fire again
						m_DisabledInput.FireKey = TRUE ;
						m_Move.FireKey		    = FALSE ;

                        // Toggle between current weapon and repair gun
						if (m_WeaponCurrentType == INVENT_TYPE_WEAPON_REPAIR_GUN)
							m_WeaponRequestedType = m_Loadout.Weapons[m_WeaponCurrentSlot];
						else
							m_WeaponRequestedType = INVENT_TYPE_WEAPON_REPAIR_GUN ;
                    }
                    break ;
            }
        }
    }
}

//==============================================================================

xbool player_object::GetDeployAttemptData( vector3&      Point, 
                                           vector3&      Normal,
                                           object::type& ObjType )
{
    collider::collision     Collision;
    radian3                 VRot;
    f32                     VFOV;
    vector3                 VZ, VP;

    GetView( VP, VRot, VFOV );
    VZ.Set( VRot.Pitch, VRot.Yaw );
    VZ *= (GetType() == object::TYPE_BOT)
            ? BOT_DEPLOY_DIST
            : DEPLOY_DIST;

    // check for collision with world
    s_Collider.ClearExcludeList() ;
    s_Collider.RaySetup( this, VP, VP+VZ );
    ObjMgr.Collide( object::ATTR_SOLID, s_Collider );
    
    if( s_Collider.HasCollided() )
    {
        s_Collider.GetCollision( Collision );
        Point   = Collision.Point;
        Normal  = Collision.Plane.Normal;
        ObjType = ((object*)Collision.pObjectHit)->GetType();
        return( TRUE );
    }

    return( FALSE );
}

//==============================================================================
                                /*
void player_object::AttemptDeployTurretBarrel( void )
{
    collider                Segment;
    collider::collision     Collision;
    radian3                 VRot;
    f32                     VFOV;
    vector3                 VZ, VP;
    object*                 pObject;

    turret::kind            Kind = turret::AA;

    GetView( VP, VRot, VFOV );
    VZ.Set( VRot.Pitch, VRot.Yaw );
    VZ *= DEPLOY_DIST;

    // check for collision with world
    Segment.RaySetup( this, VP, VP+VZ );
    ObjMgr.Collide( object::ATTR_ASSET | object::ATTR_SOLID_STATIC, Segment );

    if( !Segment.HasCollided() )
    {
        // TO DO: Message "No turret found."
        return;
    }

    Segment.GetCollision( Collision );
    pObject = (object*)Collision.pObjectHit;

    if( pObject->GetType() != TYPE_TURRET_FIXED )        
    {
        // TO DO: Message "Can only place barrels on fixed base turrets."
        return;
    }

    if( (pObject->GetTeamBits() & m_TeamBits) == 0x00 )
    {
        // TO DO: Message "Can't place barrel, wrong team."
        return;
    }

    turret* pTurret = (turret*)pObject;

    switch( m_PackCurrentType )
    {
        case INVENT_TYPE_DEPLOY_PACK_BARREL_AA:         Kind = turret::AA;      break;
        case INVENT_TYPE_DEPLOY_PACK_BARREL_ELF:        Kind = turret::ELF;     break;
        case INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR:     Kind = turret::MORTAR;  break;
        case INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE:    Kind = turret::MISSILE; break;
        case INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA:     Kind = turret::PLASMA;  break;
        default:                                        ASSERT( FALSE );        break;
    }

    if( pTurret->ChangeBarrel( Kind ) )
    {
        // TO DO: Message "Barrel changed."
        AddInvent( m_PackCurrentType, -1 );
    }
    else
    {
        // TO DO: Message "Can't place barrel, turret not operational."
    }
}    
    */

//==============================================================================

void player_object::DebugCheckValidPosition( void )
{
    // Check terrain
    terrain* pTerrain;
    ObjMgr.StartTypeLoop( object::TYPE_TERRAIN );
    while( (pTerrain = (terrain*)ObjMgr.GetNextInType()) )
    {
        // Check center pos
        f32 H = pTerrain->m_Terrain.GetHeight( m_WorldPos.Z, m_WorldPos.X );
        ASSERT( m_WorldPos.Y >= H ) ;

        // Check bbox corners
        H = pTerrain->m_Terrain.GetHeight( m_WorldBBox.Min.Z, m_WorldBBox.Min.X ) ;
        ASSERT( m_WorldPos.Y >= H ) ;

        H = pTerrain->m_Terrain.GetHeight( m_WorldBBox.Min.Z, m_WorldBBox.Max.X ) ;
        ASSERT( m_WorldPos.Y >= H ) ;

        H = pTerrain->m_Terrain.GetHeight( m_WorldBBox.Max.Z, m_WorldBBox.Min.X ) ;
        ASSERT( m_WorldPos.Y >= H ) ;

        H = pTerrain->m_Terrain.GetHeight( m_WorldBBox.Max.Z, m_WorldBBox.Max.X ) ;
        ASSERT( m_WorldPos.Y >= H ) ;
    }
    ObjMgr.EndTypeLoop();

    // Check players
    player_object* pPlayer ;
    ObjMgr.StartTypeLoop( object::TYPE_PLAYER ) ;
    while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
    {
        if (        (pPlayer != this)
                &&  (pPlayer->m_PlayerState != PLAYER_STATE_CONNECTING)
                &&  (pPlayer->m_PlayerState != PLAYER_STATE_WAIT_FOR_GAME)
                &&  (pPlayer->m_PlayerState != PLAYER_STATE_DEAD) )
        {
            ASSERT(!(pPlayer->m_WorldBBox.Intersect(m_WorldBBox))) ;
        }
    }
    ObjMgr.EndTypeLoop();
}

//==============================================================================

void player_object::ProcessPermeableCollisions( collider& Collider )
{
    s32     i ;
    void*   pVoid ;
    object* pObject ;
    f32     T ;

    // Loop through all permeable objects that were hit...
    for (i = 0 ; i < Collider.GetNPermeables() ; i++)
    {
        // Get object that was hit
        Collider.GetPermeable(i, pVoid, T) ;

        // What type of object did we hit?
        pObject = (object*)pVoid ;
        switch( pObject->GetType() )
        {
            // Pickup?
            case object::TYPE_PICKUP:
                // Must be alive!
                ASSERT(m_Health > 0) ;
                // Try take the pickup
                HitPickup((pickup*)pObject) ;
                break ;

            // Flag?
            case object::TYPE_FLAG:
                // Tell the game manager that we've touched a flag
                pGameLogic->FlagTouched( pObject->GetObjectID(), m_ObjectID );
                break;

            // Nexus
            case object::TYPE_NEXUS:
                // Tell the game logic that we've touched the nexus
                pGameLogic->NexusTouched( m_ObjectID );
                break;

            // Player?
            case object::TYPE_PLAYER:
                ((player_object*)pObject)->HitByPlayer(this) ;
                break ;
        }
    }
}

//==============================================================================

xbool player_object::Step( const collider::collision& Collision,
                           const u32                  AttrMask,
                           const vector3&             Move, 
                           const vector3&             MoveDir,
                                 collider&            Collider, 
                                 f32&                 MaxStep )
{
    f32                 dHeight, FeetY ;
    collider::collision StepCollision ;
    bbox                WorldBBox ;
    vector3             WorldPos, StepForward, StepUp ;
    object*             pObjectHit ;
    xbool               CheckStep ;

    // Should only call this if we've already collided!
    ASSERT(Collision.Collided) ;

     // Start with only checking for stepping up walls
    CheckStep = (ABS(Collision.Plane.Normal.Y) < VERTICAL_STEP_DOT) ;

    // Fix the nasty lip that sticks out the ground at the doorway in sanctuary
    // (Hitting ceilings will not cause a step since the delta height will be way too big)
    CheckStep |= (Collision.Plane.Normal.Y == -1) ;

    // Special case objects
    pObjectHit = (object*)Collision.pObjectHit ;
    switch(pObjectHit->GetType())
    {
        // Make inventory stations easy to run over for better game play
        case object::TYPE_STATION_FIXED: 
        case object::TYPE_STATION_VEHICLE: 
        case object::TYPE_VPAD:
        case object::TYPE_TURRET_SENTRY:

            CheckStep = TRUE ;
            break ;

        // For scenics, check the geometry shape type
        case object::TYPE_SCENIC:
            {
                scenic* Scenic = (scenic*)pObjectHit ;
                
                // Which shape?
                switch(Scenic->GetShapeType())
                {
                    // Scenics that should always be stepped over for better game play
                    case SHAPE_TYPE_SCENERY_NEXUS_BASE:
                        CheckStep = TRUE ;
                        break ;

                    default:
                        CheckStep = FALSE ;
                        break ;
                }
            }
            break ;
    
        // Never step up these objects
        case TYPE_STATION_DEPLOYED:
        case TYPE_TURRET_FIXED:
        case TYPE_TURRET_CLAMP:
        case TYPE_SENSOR_REMOTE:
        case TYPE_SENSOR_MEDIUM:
        case TYPE_SENSOR_LARGE:
        case TYPE_GENERATOR:
        case TYPE_CORPSE:
        case TYPE_PLAYER:
        case TYPE_BOT:
        case TYPE_PICKUP:
        case TYPE_NEXUS:
        case TYPE_FLIPFLOP:
        case TYPE_PROJECTOR:
        case TYPE_FORCE_FIELD:
        case object::TYPE_TERRAIN:
            CheckStep = FALSE ;
            break ;
    }

    // Bail out?
    if (!CheckStep)
        return FALSE ;

    // Must be able to move a portion of the original movement
    StepForward = Move * 0.1f ;

    // If not moving forward enough, don't bother
    if (((StepForward.X * StepForward.X) + (StepForward.Z * StepForward.Z)) < (MOVING_EPSILON*MOVING_EPSILON))
        return FALSE ;

    // Move at least the min poly dist - this provides an extra bit of clearance for the player
    // (fixes the jerkyness seen on some walls - eg. sanctuary)
    StepForward += MoveDir * (MIN_FACE_DISTANCE*2) ;

    // Step movement too small for collider?
    if (StepForward.LengthSquared() < collider::MIN_EXT_DIST_SQR)
        return FALSE ;

    // Get current world pos (it's already at the impact point!)
    WorldPos   = m_WorldPos ;
    WorldBBox  = m_WorldBBox ;
   
    // Calculate where feet are at point of impact.
    // (the impact y might be lower than the feet y because the player is moved back
    //  from the impact plane a tad prior to this function being called).
    // This fixes sanctuary sloped corridor
    FeetY = WorldPos.Y ;
    if (Collision.Point.Y < FeetY)
        FeetY = Collision.Point.Y ;
 
    // Get height to step up
    dHeight = Collider.GetHighPoint() - FeetY ;

    // In theory the high point should always be above or equal to the feet,
    // but because the player is moved back from the impact plane prior to
    // this function being called, once again it may not be the case.
    // This happens sometimes on the sloped corridor in sanctuary (supprise, supprise!)
    if (dHeight < 0)
        dHeight = 0 ;

    // Don't step if top of poly is above the maximum step height
    if (dHeight > MaxStep)
        return FALSE ;

    // Aim for slightly above the step to keep away from the poly
    dHeight += MIN_FACE_DISTANCE*2 ;

    // Too small for collider?
    if (dHeight < collider::MIN_EXT_DIST)
        return FALSE ;

    // Debug info
    #ifdef PLAYER_DEBUG_STEP_COLL
        x_printf("Checking step dHeight:%f - ", dHeight) ;
    #endif

    // Check1: Must be collision free from "player pos -> player pos + step height"
    StepUp = vector3(0, dHeight, 0) ;
    Collider.ExtSetup( this, WorldBBox, StepUp ) ;
    ObjMgr.Collide(AttrMask, Collider ) ;
    Collider.GetCollision( StepCollision );
    if (StepCollision.Collided)
    {
        // Debug info
        #ifdef PLAYER_DEBUG_STEP_COLL
            x_printf("up fail\n") ;
        #endif

        return FALSE ;
    }

    // Success - move player ghost vars to above the step
    WorldPos  += StepUp ;
    WorldBBox.Translate(StepUp) ;

    // Bug fix - there must be a clearance distance ofthis much infront of the player.
    //           this helps fix the popping
    StepForward = MoveDir * PLAYER_STEP_CLEAR_DIST ;
    
    // Check2: Must be able to move some distance "player pos + step height -> player pos + step height + move"
    Collider.ExtSetup( this, WorldBBox, StepForward ) ;
    ObjMgr.Collide(AttrMask, Collider ) ;
    Collider.GetCollision( StepCollision );
    if (StepCollision.Collided)
    {
        // Debug info
        #ifdef PLAYER_DEBUG_STEP_COLL
            x_printf("forward fail\n") ;
        #endif

        return FALSE ;
    }

    // Success - put player just above the step
    m_WorldPos  = WorldPos ;
    m_WorldBBox = WorldBBox ;
    
    // Update max step
    MaxStep -= StepUp.Y ;
    if (MaxStep < 0)
        MaxStep = 0 ;

    // Debug info
    #ifdef PLAYER_DEBUG_STEP_COLL
        x_printf("STEPPED!\n") ;
    #endif
    
    return TRUE ;
}

//==============================================================================

xbool is_player_above_terrain(player_object *pPlayer)
{
    xbool Above = TRUE ;

    // Get player info
    bbox    BBox = pPlayer->GetBBox() ;
    vector3 Pos  = pPlayer->GetPosition() ;

    // Check terrain
    terrain* pTerrain;
    ObjMgr.StartTypeLoop( object::TYPE_TERRAIN );
    while( (pTerrain = (terrain*)ObjMgr.GetNextInType()) )
    {
        // Check center pos
        f32 H = pTerrain->m_Terrain.GetHeight( Pos.Z, Pos.X );
        if ( Pos.Y < H )
            Above = FALSE ;

        // Check bbox corners
        H = pTerrain->m_Terrain.GetHeight( BBox.Min.Z, BBox.Min.X ) ;
        if ( Pos.Y < H )
            Above = FALSE ;

        H = pTerrain->m_Terrain.GetHeight( BBox.Min.Z, BBox.Max.X ) ;
        if ( Pos.Y < H )
            Above = FALSE ;

        H = pTerrain->m_Terrain.GetHeight( BBox.Max.Z, BBox.Min.X ) ;
        if ( Pos.Y < H )
            Above = FALSE ;

        H = pTerrain->m_Terrain.GetHeight( BBox.Max.Z, BBox.Max.X ) ;
        if ( Pos.Y < H )
            Above = FALSE ;
    }
    ObjMgr.EndTypeLoop();

    return Above ;
}


xbool   log_fall_thru_grabbed=FALSE ;
f32     log_fall_thru_delta_time=0;
vector3 log_fall_thru_pos(0,0,0) ;
vector3 log_fall_thru_vel(0,0,0) ;
vector3 log_fall_thru_anim_motion_delta_pos(0,0,0) ;
s32     log_fall_thru_armor=0 ;
xbool   log_fall_thru_valid_before=TRUE ;
xbool   log_fall_thru_valid_after=TRUE ;


void player_object::KeepAboveTerrain( void )
{
    f32      H = m_WorldPos.Y ;
    terrain* pTerrain;

    // Loop through all terrain
    ObjMgr.StartTypeLoop( object::TYPE_TERRAIN );
    while( (pTerrain = (terrain*)ObjMgr.GetNextInType()) )
    {
        // Check bbox corners for highest terrain point
        H = MAX(H, pTerrain->m_Terrain.GetHeight( m_WorldBBox.Min.Z, m_WorldBBox.Min.X )) ;
        H = MAX(H, pTerrain->m_Terrain.GetHeight( m_WorldBBox.Min.Z, m_WorldBBox.Max.X )) ;
        H = MAX(H, pTerrain->m_Terrain.GetHeight( m_WorldBBox.Max.Z, m_WorldBBox.Min.X )) ;
        H = MAX(H, pTerrain->m_Terrain.GetHeight( m_WorldBBox.Max.Z, m_WorldBBox.Max.X )) ;
    }
    ObjMgr.EndTypeLoop();

    if( (m_WorldPos.Y + 100.0f) < H )
    {
        #ifdef X_ASSERT
        x_printf( "PLAYER FELL THROUGH TERRAIN\n" );
        #endif

        m_WorldPos.Y = (H + 0.05f);

        // Dump fall through log?
        if (log_fall_thru_grabbed)
        {
            x_DebugLog( "Player fell through terrain!\n"
                        "DeltaTime: %f\n"
                        "Armor: %d\n"
                        "*(u32*)&m_WorldPos.X           = %d (%g)\n"
                        "*(u32*)&m_WorldPos.Y           = %d (%g)\n"
                        "*(u32*)&m_WorldPos.Z           = %d (%g)\n"
                        "*(u32*)&m_Vel.X                = %d (%g)\n"
                        "*(u32*)&m_Vel.Y                = %d (%g)\n"
                        "*(u32*)&m_Vel.Z                = %d (%g)\n"
                        "*(u32*)&m_AnimMotionDeltaPos.X = %d (%g)\n"
                        "*(u32*)&m_AnimMotionDeltaPos.Y = %d (%g)\n"
                        "*(u32*)&m_AnimMotionDeltaPos.Z = %d (%g)\n",
                        log_fall_thru_delta_time,            
                        m_ArmorType,
                        *(u32*)&log_fall_thru_pos.X,                    log_fall_thru_pos.X,                  
                        *(u32*)&log_fall_thru_pos.Y,                    log_fall_thru_pos.Y,                  
                        *(u32*)&log_fall_thru_pos.Z,                    log_fall_thru_pos.Z,                  
                        *(u32*)&log_fall_thru_vel.X,                    log_fall_thru_vel.X,                  
                        *(u32*)&log_fall_thru_vel.Y,                    log_fall_thru_vel.Y,                  
                        *(u32*)&log_fall_thru_vel.Z,                    log_fall_thru_vel.Z,                  
                        *(u32*)&log_fall_thru_anim_motion_delta_pos.X,  log_fall_thru_anim_motion_delta_pos.X,
                        *(u32*)&log_fall_thru_anim_motion_delta_pos.Y,  log_fall_thru_anim_motion_delta_pos.Y,
                        *(u32*)&log_fall_thru_anim_motion_delta_pos.Z,  log_fall_thru_anim_motion_delta_pos.Z
                        );
        }
    }
}

//==============================================================================

X_FILE* log_fp = NULL;

void player_object::ApplyPhysics(void)
{
    // Locals
	collider::collision     Collision ;
    object*                 pObjectHit ;
    pickup*                 pPickup ;
    vector3                 DeltaPos ;
    xbool                   StuckInsideObject ;
    //s32                     i ;

    // NULL move?
    if (m_Move.DeltaTime <= 0)
        return ;

    // Temp
    //player_test_collision(this, Collision, m_WorldPos, vector3(0,1,0), TRUE) ;

    // temp
    //SetPlayerShape(m_PlayerInstance.GetShape(), (s32)m_PlayerInstance.GetTextureFrame()) ;

    // Record info for setting the dirty bits
    vector3     LastPos             = m_WorldPos ;
    vector3     LastVel             = m_Vel ;
    radian3     LastRot             = m_Rot ;

    // Read vars from move
	f32 DeltaTime = m_Move.DeltaTime ;

    //==========================================================================
    // Ground track check
	//==========================================================================

	// Only ground track if:
	// 1) The player start on the ground
	// 2) The player is not jetting or jumping
	// 3) The player is running with or into the ground
	xbool GroundTrack = FALSE ;
	if (        (PLAYER_DEBUG_GROUND_TRACK)          &&
                (m_ContactInfo.RunSurface)           && 
                (!((m_Move.JetKey)  || (m_Jetting))) && 
                (!((m_Move.JumpKey) || (m_Jumping))) )
	{
        GroundTrack = TRUE ;

        // SB - This check causes the player to loft going uphill sometimes so I've removed it!

		// Get movement direction
		//vector3 MoveDir = m_Vel ;
		//MoveDir.Normalize() ;

		// Moving into ground?
        //f32 Dot = v3_Dot(MoveDir, m_ContactInfo.RunNormal) ;
        //x_printfxy(2,5,"Dot:%f", Dot) ;
		//if (Dot >= GROUND_TRACKING_DOT)
			//GroundTrack = TRUE ;
        //else
            //GroundTrack = FALSE ;
	}

    //==========================================================================
    // Check for collecting any pickups inside the player bounds
    //==========================================================================
    if (tgl.ServerPresent)
    {
        // Check for collecting the exchange pickup first
        if (m_ExchangePickupKind != pickup::KIND_NONE)
        {
            // Now check all pickups
            ObjMgr.StartTypeLoop( object::TYPE_PICKUP ) ;
            while ( (pPickup = (pickup*)ObjMgr.GetNextInType()) )
            {
                // Should be a pickup
                ASSERT(pPickup->GetType() == object::TYPE_PICKUP) ;

                // Overlapping players bbox and same type as the exchange pickup?
                if ( (pPickup->GetKind() == m_ExchangePickupKind) && (m_WorldBBox.Intersect(pPickup->GetBBox())) )
                    HitPickup(pPickup) ;
            }
            ObjMgr.EndTypeLoop() ;
        }

        // Now check all pickups
        ObjMgr.StartTypeLoop( object::TYPE_PICKUP ) ;
        while ( (pPickup = (pickup*)ObjMgr.GetNextInType()) )
        {
            // Should be a pickup
            ASSERT(pPickup->GetType() == object::TYPE_PICKUP) ;

            // Overlapping players bbox?
            if (m_WorldBBox.Intersect(pPickup->GetBBox()))
                HitPickup(pPickup) ;
        }
        ObjMgr.EndTypeLoop() ;
    }

    //==========================================================================
    // Get player info from vehicle?
    //==========================================================================
    vehicle* Vehicle = IsAttachedToVehicle() ;
    if (Vehicle)
    {
        // Make sure vehicle vars are setup
        ASSERT(m_VehicleID != obj_mgr::NullID) ;
        ASSERT(m_VehicleSeat != -1) ;

        // If piloting a vehicle, do the vehicle physics now incase of a network update...
        if (IsVehiclePilot())
        {
            // Move the vehicle?
            if ((m_ProcessInput) || (tgl.pServer))
            {
                // Move
                Vehicle->ApplyPhysics(DeltaTime) ;

                // Put vehicle dirty bits into the player
                m_DirtyBits |= Vehicle->GetDirtyBits() ;
            }
        }
        
        // ApplyPhysics() may kick out player from vehicle if its destroyed
        Vehicle = IsAttachedToVehicle() ;

        if( Vehicle )
        {
            // Grab ground rotation from vehicle
            m_GroundRot = quaternion(Vehicle->GetDrawRot()) ;

            // Always setup player pos
            m_WorldPos = Vehicle->GetSeatPos(m_VehicleSeat) ;
        }
        
        CalcBounds() ;
    }
    else
    {
        //==========================================================================
        // Collision movement loop
	    //==========================================================================

        DEBUG_BEGIN_TIMER(ApplyPhysicsCollision)
        DEBUG_DEFINE_TIMER(Extrusion)

        // Logging
        xbool Logging = FALSE;//input_IsPressed(INPUT_KBD_F4);
        if( Logging && !log_fp )
        {
            log_fp = x_fopen("log.txt","wt");
            ASSERT(log_fp);
        }

        // Log fall through
        if (!log_fall_thru_grabbed)
        {
            log_fall_thru_delta_time            = DeltaTime;
            log_fall_thru_pos                   = m_WorldPos;
            log_fall_thru_vel                   = m_Vel ;
            log_fall_thru_anim_motion_delta_pos = m_AnimMotionDeltaPos ;
            log_fall_thru_armor                 = m_ArmorType ;
            log_fall_thru_valid_before          = is_player_above_terrain(this) ;
        }

	    // Clear impact velocity
	    m_ImpactVel.Zero() ;
        m_ImpactSpeed = 0 ;
        m_GroundImpactVel.Zero() ;
        m_GroundImpactSpeed = 0 ;

        // Loop until all time is gone
        s32     CollisionCount = 0 ;
        if( Logging )
        {
            x_fprintf(log_fp,"--------------------------------\n");
            x_fprintf(log_fp,"START POS(%f,%f,%f)\n",m_WorldPos.X,m_WorldPos.Y,m_WorldPos.Z);
        }

        // Setup collision types to collide with
        u32 AttrMask = PLAYER_COLLIDE_ATTRS ;
    
        // Only check for pickups on the server
        if ((tgl.ServerPresent) && (m_Health > 0))
            AttrMask |= PLAYER_PICKUPS_ATTRS ;

        // Keep the start position, incase we need to go back to it (when hitting creases)
        vector3 StartPos        = m_WorldPos ;
        vector3 StartVel        = m_Vel ;

        // Crease vars
        vector3 FirstNormal(0,1,0) ;

        // Put back to start pos
        m_WorldPos = StartPos ;
        m_Vel      = StartVel ;

        // Update bounds
		CalcBounds() ;

        // Make sure player is in a valid position!
        //DebugCheckValidPosition() ;

        // Start with all of time
        f32 RemainTime  = DeltaTime ;
        f32 MaxStep     = MAX_STEP_HEIGHT ;

        vector3 BackOffMove = vector3(0,0,0) ;

        // Reset the collider
        s_Collider.ClearExcludeList() ;

        // Try a number of attempts to do all of movement
        for (CollisionCount = 0 ; CollisionCount < MOVE_RETRY_COUNT ; CollisionCount++)
        {
            // Calculate amount of movement to take
            vector3 Move = (m_Vel * RemainTime) ;
    
            // Apply anim motion
            Move += (m_AnimMotionDeltaPos * RemainTime) / DeltaTime ;
        
            // Add backoff move
            Move += BackOffMove ;
            BackOffMove.Zero() ;

            // Fall out early due to not enough movement?
            ASSERT(MOVING_EPSILON >= collider::MIN_EXT_DIST) ;
            if (Move.LengthSquared() < (MOVING_EPSILON*MOVING_EPSILON))
                break ;

            // Log
            if( Logging )
            {
                x_fprintf(log_fp,"(%f %f %f)->",Move.X,Move.Y,Move.Z);
                x_fprintf(log_fp,"OLD POS(%f,%f,%f)\n",m_WorldPos.X,m_WorldPos.Y,m_WorldPos.Z);
                x_fprintf(log_fp,"(%f %f %f)\n",Move.X,Move.Y,Move.Z);
            }

            // Update bounds
		    CalcBounds() ;

            DEBUG_START_TIMER(Extrusion)

            // Prepare for an extrusion (ouch!)
            bbox PermeableBBox = m_WorldBBox;
            PermeableBBox.Max -= vector3(0.3f,0.0f,0.3f);
            PermeableBBox.Min += vector3(0.3f,0.0f,0.3f);
            s_Collider.ExtSetup( this, m_WorldBBox, PermeableBBox, Move ) ;

            // Do the collision!
            ObjMgr.Collide(AttrMask, s_Collider ) ;

            // Get results
            s_Collider.GetCollision( Collision );



            DEBUG_STOP_TIMER(Extrusion)

            // Process pickups etc
            if ((tgl.pServer) && (m_Health > 0))
                ProcessPermeableCollisions( s_Collider ) ;

            //======================================================================
            // Get object that was hit (if any)
            //======================================================================
            if (Collision.Collided)
            {
                // Get object collided with
                pObjectHit = (object *)Collision.pObjectHit ;
                ASSERT(pObjectHit) ;
            }
            else
                pObjectHit = NULL ;

            //======================================================================
            // Check for being stuck inside an object
            //======================================================================
            StuckInsideObject = FALSE ;
            if ((Collision.Collided) && (Collision.T == 0))
            {
                // What type of object did we hit?
                ASSERT( pObjectHit ) ;
                switch( pObjectHit->GetType() )
                {
                    // Don't do anything about these, let the plane culling take care of it
                    case object::TYPE_TERRAIN:
                    case object::TYPE_BUILDING:
                    case object::TYPE_FORCE_FIELD:

                    // Causes problems!
                    case object::TYPE_GENERATOR:
                    case object::TYPE_SCENIC:

                    // Stations aren't convex!
                    case object::TYPE_STATION_FIXED:
                    case object::TYPE_STATION_VEHICLE:
                    case object::TYPE_STATION_DEPLOYED:
                        break ;

                    // Rest of objects are okay to move out of...
                    default:
                        // If we are moving away from, or right on top of the object, then ignore the collision with it
                        DeltaPos = m_WorldBBox.GetCenter() - pObjectHit->GetBBox().GetCenter() ;
                        if (v3_Dot(DeltaPos, Move) >= 0)
                            StuckInsideObject = TRUE ;
                        break ;
                }
            }

            //======================================================================
            // Process a collision
            //======================================================================
            
            // Stuck?
            if (StuckInsideObject)
            {
                // Ignore the hit object next time
                ASSERT( pObjectHit ) ;
                s_Collider.Exclude(pObjectHit->GetObjectID().GetAsS32()) ;
            
                // Don't count this as a try
                CollisionCount-- ;
            }
            else
            // Regular collision?
            if (Collision.Collided)
            {
                //==================================================================
                // Log
                //==================================================================
                if( Logging )
                {
                    x_fprintf(log_fp,"Collided... vel (%f,%f,%f) T:%f\n",m_Vel.X,m_Vel.Y,m_Vel.Z,Collision.T);
                    x_fprintf(log_fp,"Plane %f %f %f %f\n",
                         Collision.Plane.Normal.X
                        ,Collision.Plane.Normal.Y
                        ,Collision.Plane.Normal.Z
                        ,Collision.Plane.D );
                }

                //==================================================================
                // Trigger special case objects into doing something...
                //==================================================================

                // What type of object did we hit?
                ASSERT( pObjectHit );
                switch( pObjectHit->GetType() )
                {
                    // FlipFlop?
                    case object::TYPE_FLIPFLOP:
                        if( !IsDead() )
                        {
                            // Let the game manager do it's stuff
                            GameMgr.SwitchTouched( pObjectHit->GetObjectID(), m_ObjectID );

                            // Inject an outward flinging force at the switch.
                            pain Pain ;
                            Pain                = g_PainBase[pain::MISC_FLIP_FLOP_FORCE] ;
                            Pain.Force          = Pain.MaxForce ;
                            Pain.MetalDamage    = 0 ;
                            Pain.EnergyDamage   = 0 ;
                            Pain.Distance       = Pain.ForceR1 ;
                            Pain.LineOfSight    = TRUE ;
                            Pain.Center         = pObjectHit->GetPainPoint() ;
                            Pain.TeamBits       = 0 ;
                            Pain.OriginID       = obj_mgr::NullID ;
                            Pain.VictimID       = obj_mgr::NullID ;
                            OnPain(Pain) ;
                        }
                        break;
                }

                //==================================================================
                // Reset fall through log if landed on a building
                //==================================================================
                if (pObjectHit->GetType() == object::TYPE_BUILDING)
                {
                    log_fall_thru_valid_before  = TRUE ;
                    log_fall_thru_grabbed       = FALSE ;
                }

                //==================================================================
                // Apply motion so far...
                //==================================================================

                // Calc movement length and direction
                f32     BackOffDist = MIN_FACE_DISTANCE ;
                f32     MoveLength  = Move.Length() ;
                vector3 MoveDir     = Move * (1.0f / MoveLength) ;

                // SB. Commented out unless needed by some tricky building...

                // Setup backoff move for next time
                //BackOffMove = Collision.Plane.Normal * MIN_FACE_DISTANCE ;

                // Calc backoff dist so that player is MIN_FACE_DISTANCE away from collision plane
                //f32     Dot     = -v3_Dot(MoveDir, Collision.Plane.Normal) ;
                //if (Dot > 0.0001f)
                    //BackOffDist = MIN_FACE_DISTANCE / Dot ;

                // Back off from collision plane
                Collision.T -= BackOffDist / MoveLength ; 
                Collision.T = MAX(0, Collision.T) ;
                ASSERT(Collision.T >= 0) ;
                ASSERT(Collision.T <= 1) ;

                // Put at collision point
                vector3 Motion = Move * Collision.T ;
                m_WorldPos  += Motion ;
                m_WorldBBox.Translate(Motion) ;

                //==================================================================
                // If player bbox (when put at collision point), straddles the plane
                // then it has it a lip
                //==================================================================

                /*                    
                // Skip lip test for terrain
                ASSERT(pObjectHit) ;
                if (pObjectHit->GetType() != object::TYPE_TERRAIN)
                {
                    xbool AbovePlane=FALSE, BelowPlane=FALSE ;
           
                    // Check the 8 corners
                    vector3 Corner ;
                    for (i = 0 ; i < 8 ; i++)
                    {
                        // Setup x
                        if (i & 1)
                            Corner.X = m_WorldBBox.Min.X ;
                        else
                            Corner.X = m_WorldBBox.Max.X ;

                        // Setup y
                        if (i & 2)
                            Corner.Y = m_WorldBBox.Min.Y ;
                        else
                            Corner.Y = m_WorldBBox.Max.Y ;

                        // Setup z
                        if (i & 4)
                            Corner.Z = m_WorldBBox.Min.Z ;
                        else
                            Corner.Z = m_WorldBBox.Max.Z ;

                        // Check point
                        BelowPlane |= Collision.Plane.InBack(Corner) ;
                        AbovePlane |= !BelowPlane ;
                    }

                    // On a lip?
                    if ((AbovePlane) && (BelowPlane))
                    {
                        // Debug info
                        #ifdef PLAYER_DEBUG_LIP_COLL
                            x_printf("Player is on a collision lip!\n") ;
                        #endif
                        
                        // Clear the normals of the axis which encloses the collision pt
                        for (i = 0 ; i < 3 ;i++)
                        {
                            // Does axis enclose the collision point?
                            if (        (Collision.Point[i] > m_WorldBBox.Min[i])
                                    &&  (Collision.Point[i] < m_WorldBBox.Max[i]) )
                            {
                                // Clear axis component
                                Collision.Plane.Normal[i] = 0 ;
                            }
                        }

                        // Finally, make sure the altered normal can be used
                        Collision.Plane.Normal.Normalize() ;
                    }
                }
                */

                //==================================================================
                // Update remaining time
                //==================================================================
                RemainTime *= 1 - Collision.T ;

                //==================================================================
                // Attempt to step the collision - if successful, skip collision, and do next move
                //==================================================================
                if (Step(Collision, AttrMask, Move, MoveDir, s_Collider, MaxStep))
                    continue ;

                //==================================================================
                // Record impact info
                //==================================================================

                m_ImpactVel   = m_Vel ;
                m_ImpactSpeed = -v3_Dot(m_Vel, Collision.Plane.Normal) ;

                // Record ground impact info?
                if (Collision.Plane.Normal.Y > 0)
                {
                    m_GroundImpactVel   = m_ImpactVel ;
                    m_GroundImpactSpeed = m_ImpactSpeed ;

                    // Record so land/run sounds can play
                    SetupContactInfo(Collision, 0) ;
                }                    


                //==================================================================
                //  The ObjMgr will apply pain if impact is too hard.  Will also
                //  handle collisions with another moveable object (player or 
                //  vehicle).
                //==================================================================
                ObjMgr.ProcessImpact( this, Collision );


                /*
                // Shake camera on ground impact
                if( bd > m_MoveInfo->GROUND_IMPACT_MIN_SPEED && isControlObject() )
                {
                    F32 ampScale = (bd - m_MoveInfo->groundImpactMinSpeed) / m_MoveInfo->minImpactSpeed;

                    if (isClientObject())
                    {
                       CameraShake *groundImpactShake = new CameraShake;
                       groundImpactShake->setDuration( m_MoveInfo->groundImpactShakeDuration );
                       groundImpactShake->setFrequency( m_MoveInfo->groundImpactShakeFreq );

                       VectorF shakeAmp = m_MoveInfo->groundImpactShakeAmp * ampScale;
                       groundImpactShake->setAmplitude( shakeAmp );
                       groundImpactShake->setFalloff( m_MoveInfo->groundImpactShakeFalloff );
                       groundImpactShake->init();
                       gCamFXMgr.addFX( groundImpactShake );
                    }
                }
                */

                //==================================================================
                // Land hard?
                //==================================================================
                if (    (m_ImpactSpeed > m_MoveInfo->MIN_IMPACT_SPEED) &&
                        (m_PlayerState >= PLAYER_STATE_MOVE_START) && 
                        (m_PlayerState <= PLAYER_STATE_MOVE_END) )
                {
                    // Scale how long we're down for 
                    f32   value   = (m_ImpactSpeed - m_MoveInfo->MIN_IMPACT_SPEED) ;
                    f32   range   = (m_MoveInfo->MIN_IMPACT_SPEED * 0.9f) ;
                    f32   recover = m_MoveInfo->RECOVER_DELAY ;
                    if (value < range) 
                        recover = 1 + (x_floor( recover * value / range) ) ;

                    m_LandRecoverTime = recover / 30.0f ;
                   
                    Player_SetupState(PLAYER_STATE_LAND) ;
                }

                //==================================================================
                // Calculate slide + bounce delta vectors and set new velocity
                //==================================================================

                vector3 Slide  = Collision.Plane.Normal * (m_ImpactSpeed + NORMAL_ELASTICITY) ;
                vector3 Bounce = Collision.Plane.ReflectVector(m_Vel) - m_Vel ;

                // Slide or bounce?
                ASSERT(pObjectHit) ;
                switch(pObjectHit->GetType())
                {
                    case TYPE_STATION_FIXED:
                    case TYPE_STATION_VEHICLE:
                    case TYPE_VPAD:
                    case TYPE_BUILDING:
                    case TYPE_TERRAIN:

                        // Just slide as normal
                        m_Vel += Slide ; 
                        break ;

                    default:

                        // Always slide if alive
                        if (m_Health > 0)
                        {
                            // Slide
                            m_Vel += Slide ;
                        }
                        else
                        {
                            // Try and bounce off objects we don't want to settle on during death
                            // Slide 50%
                            m_Vel += Slide * 0.5f ;

                            // Bounce 50%
                            m_Vel += Bounce * 0.5f ;
                        }
                        break ;
                }

                //==================================================================
                // Crease check
                //==================================================================
                if (CollisionCount == 0)
                {
                    FirstNormal = Collision.Plane.Normal ;

                    #ifdef DEBUG_COLLISION
                        // Setup set0 points (drawn in PlayerRender.cpp)
                        NPlayerCollPoints0 = NCollPoints ;
                        for (s32 i = 0 ; i < NCollPoints ; i++)
                            PlayerCollPoints0[i] = CollPoints[i] ;
                    #endif  // #ifdef DEBUG_COLLISION

                }
                else
                {
                    if (CollisionCount == 1)
                    {
                        #ifdef DEBUG_COLLISION
                            // Setup set1 points (drawn in PlayerRender.cpp)
                            NPlayerCollPoints1 = NCollPoints ;
                            for (s32 i = 0 ; i < NCollPoints ; i++)
                                PlayerCollPoints1[i] = CollPoints[i] ;
                        #endif  // #ifdef DEBUG_COLLISION

                        // Re-orient velocity along the creases
                        if (v3_Dot(Slide, FirstNormal) < 0 &&
                           v3_Dot(Collision.Plane.Normal, FirstNormal) < 0)
                        {
                            vector3 nv;
                            nv = v3_Cross(Collision.Plane.Normal, FirstNormal) ;
                            f32 nvl = nv.Length() ;
                            if (nvl > 0)
                            {
                                if (v3_Dot(nv, m_Vel) < 0)
                                    nvl = -nvl ;

                                nv     *= m_Vel.Length() / nvl ;
                                m_Vel  = nv ;

                                #ifdef PLAYER_DEBUG_CREASE_COLL
                                    x_printf("Sliding along crease...\n") ;
                                #endif
                            }
                        }
                    }
                }
            }
            else
            {
                //==================================================================
                // No collision - put at end of movement
                //==================================================================
                m_WorldPos += Move ;
                m_WorldBBox.Translate(Move) ;

                // Skip of out loop
                break ;
            }

            // Log
            if( Logging )
            {
                x_fprintf(log_fp,"NEW POS(%f,%f,%f)\n",m_WorldPos.X,m_WorldPos.Y,m_WorldPos.Z);
            }
        }

        // If max iterations was reached, we hit a crease, so put the player back to their start
        // pos so that they do not get stuck!
        if (CollisionCount == MOVE_RETRY_COUNT)
        {
            m_WorldPos  = StartPos ;
            m_Vel.Zero() ;

            #ifdef PLAYER_DEBUG_CREASE_COLL
                x_printf("Max collision iterations reached - skipping movement!\n") ;
            #endif
        }                

        // Store in fall through log
        if (!log_fall_thru_grabbed)
        {
            log_fall_thru_valid_after  = is_player_above_terrain(this) ;

            // Stop grabbing if the player has just gone through the terrain?
            if ((log_fall_thru_valid_before) && (!log_fall_thru_valid_after))
                log_fall_thru_grabbed=TRUE ;
        }

        // Keep the player above the terrain
        KeepAboveTerrain() ;

        DEBUG_END_TIMER(Extrusion)
        DEBUG_END_TIMER(ApplyPhysicsCollision)
    }

    //==========================================================================
	// Setup contact info and perform ground tracking
    //==========================================================================

    // Determine ground contact information and keep the collision for ground tracking
    SetupContactInfo(&Collision) ;

	// Only check for ground tracking if
	// 1) Ground tracking is enabled (see top of function)
	// 2) The player is not longer on the ground
	if ((GroundTrack) && (!m_ContactInfo.RunSurface))
	{
		// Get distance to the ground
		f32 Dist = m_ContactInfo.DistToSurface - TRACTION_DISTANCE ;

        // Make sure player is a set distance away from the plane!
        Dist -= MIN_FACE_DISTANCE ;

		// Within ground tracking limits?
		if ((Dist > 0) && (Dist <= GROUND_TRACKING_MAX_DIST))
		{
			// Snap down to the ground
			m_WorldPos.Y      -= Dist ;
			m_WorldBBox.Min.Y -= Dist ;
			m_WorldBBox.Max.Y -= Dist ;

			// Update new contact info!
	        SetupContactInfo( Collision, TRACTION_DISTANCE ) ;

			//x_printf("ground tracked!!\n") ;
		}
	}

    //==========================================================================
    // Check for using weapons, packs, throwing stuff etc
    // (done here to be in sync with weapon)
	//==========================================================================

    DEBUG_BEGIN_TIMER(ThrowFirePacks)
    CheckForThrowingAndFiring( DeltaTime ) ;
    CheckForUsingPacks( DeltaTime ) ;
    DEBUG_END_TIMER(ThrowFirePacks)

    //==========================================================================
    // Now we know which weapon to use, setup the final view
	//==========================================================================

    // We'll need this if the player/bot tries to fire
    UpdateViewAndWeaponFirePos() ;

    //==========================================================================
    // Update state logic
	//==========================================================================

    // Update weapon logic
    DEBUG_BEGIN_TIMER(WeaponAdvanceState)
	Weapon_AdvanceState(DeltaTime) ;
    DEBUG_END_TIMER(WeaponAdvanceState)

    // Update player logic
    DEBUG_BEGIN_TIMER(PlayerAdvanceState)
	Player_AdvanceState(DeltaTime) ;
    DEBUG_END_TIMER(PlayerAdvanceState)

    // Truncate so server and client controller players act exactly the same
    Truncate() ;

    // Set dirty bits
    if (m_WorldPos != LastPos)
        m_DirtyBits |= PLAYER_DIRTY_BIT_POS	;

    if (m_Vel != LastVel)
        m_DirtyBits |= PLAYER_DIRTY_BIT_VEL	;
    
    if (m_Rot != LastRot)
        m_DirtyBits |= PLAYER_DIRTY_BIT_ROT ;

    // Make sure ghost compression pos is up to date
    UpdateGhostCompPos() ;

    // Show stats
    //f32 MaxVel   = MAX(MAX(ABS(m_Vel.X), ABS(m_Vel.Y)), ABS(m_Vel.Z)) ;
    //x_printf("Vel:%f \n", MaxVel) ;
    //x_printf("Vel:%f \n", m_Vel.Length() ) ;
    //x_printfxy( 10, 1, "Speed = %1.2f\n", GetVel().Length() );
}


//==============================================================================

// Makes sure draw vars are up-to-date and updates position if attached to a vehicle
void player_object::UpdateDrawVars( void )
{
    //==========================================================================
    // Get position from vehicle?
    // Makes sure client and server blended pos is correct when for when
    // there is no prediction happening
    //==========================================================================

    vehicle* Vehicle = IsAttachedToVehicle() ;
    if (Vehicle)
    {
        // Grab ground rotation from vehicle
        m_GroundRot = quaternion(Vehicle->GetDrawRot()) ;

        // Setup new player pos
        SetPos(Vehicle->GetSeatPos(m_VehicleSeat)) ;

        // Since the vehicle is blending pos when needed, turn off player pos blend
        m_BlendMove.DeltaPos.Zero() ;
    }

    //==========================================================================
    // Setup blend draw pos and rot
    //==========================================================================

    m_DrawPos = m_WorldPos + (m_BlendMove.Blend * m_BlendMove.DeltaPos) ;
    m_DrawRot = m_Rot      + (m_BlendMove.Blend * m_BlendMove.DeltaRot) ;

    //==========================================================================
    // Setup draw bbox
    //==========================================================================
    ASSERT(m_MoveInfo) ;
    m_DrawBBox = m_MoveInfo->BOUNDING_BOX ;
    m_DrawBBox.Translate(m_DrawPos) ;

    //==========================================================================
    // Setup player and weapon blend draw rotations (takes ground/vehicle rotation into account)
    //==========================================================================
    
    // Player only has yaw
    m_PlayerDrawRot = radian3(0, m_DrawRot.Yaw, 0) ;
    
    // Weapon has pitch and yaw
    m_WeaponDrawRot = m_DrawRot ;

    // If player is sat in a seat, then he cannot spin around, so disable the player draw rotation
    if (IsSatDownInVehicle())
        m_PlayerDrawRot.Zero() ;

    // Take into account ground rotation? (0=ident, 1=all ground)
    if (m_GroundOrientBlend != 0)
    {
        quaternion qGroundRot   = BlendToIdentity(m_GroundRot, 1-m_GroundOrientBlend) ; // 0 = ident, 1 = ground

        m_PlayerDrawRot = (qGroundRot * quaternion(m_PlayerDrawRot)).GetRotation() ;
        m_WeaponDrawRot = (qGroundRot * quaternion(m_WeaponDrawRot)).GetRotation() ;
    }
}

//==============================================================================

// Advances player - this function only gets called once per frame, regardless of
// whether the player is on a server/client, or controlled or a ghost etc.
void player_object::Advance(f32 DeltaTime)
{
    s32         i ;
    vector3     DeltaPos ;

    //=====================================================================
    // Compute Average Velocity
    //=====================================================================
    {
        s32 i;

        m_Velocities[ m_VelocityID ]  = m_Vel;
    
        m_VelocityID++;
        if( m_VelocityID >= MAX_VELOCITIES )
            m_VelocityID = 0;

        m_AverageVelocity( 0, 0, 0 );
    
        for( i=0; i<MAX_VELOCITIES; i++ )
        {
            m_AverageVelocity += m_Velocities[i];
        }
    
        m_AverageVelocity /= MAX_VELOCITIES;
    }

    //=====================================================================
    // Increase health over time
    //=====================================================================
    
    // Only perform on server
    if (tgl.ServerPresent)
    {
        // If dead, stop any health increase
        if (m_Health == 0)
            m_HealthIncrease = 0 ;

        // Any health to increase?
        if (m_HealthIncrease > 0)
        {
            // Calculate how much to increase
            f32 Increase = HEALTH_INCREASE_PER_SEC * DeltaTime ;
            if (Increase > m_HealthIncrease)
                Increase = m_HealthIncrease ;

            // Update health
            AddHealth(Increase) ;

            // Update increase
            m_HealthIncrease -= Increase ;
        }
    }

    //=====================================================================
    // Turn on/off anim features depending upon the last frames screen size
    //=====================================================================

    // Leave all features on for controlled players!
    if (m_ProcessInput)
        m_MaxScreenPixelSize = 500 ;

    // Update features
    if (m_MaxScreenPixelSize < 20.0f)
    {
        m_PlayerInstance.SetEnableAnimBlending  (FALSE) ;
        m_PlayerInstance.SetEnableAdditiveAnims (FALSE) ;
        m_PlayerInstance.SetEnableMaskedAnims   (FALSE) ;
    }
    else
    if (m_MaxScreenPixelSize < 30.0f)
    {
        m_PlayerInstance.SetEnableAnimBlending  (FALSE) ;
        m_PlayerInstance.SetEnableAdditiveAnims (TRUE) ;
        m_PlayerInstance.SetEnableMaskedAnims   (TRUE) ;
    }
    else
    {
        m_PlayerInstance.SetEnableAnimBlending  (TRUE) ;
        m_PlayerInstance.SetEnableAdditiveAnims (TRUE) ;
        m_PlayerInstance.SetEnableMaskedAnims   (TRUE) ;
    }

    // Keep updating the lighting if the player was drawn
    if (m_MaxScreenPixelSize >= MIN_PIXEL_SIZE_UPDATE_LIGHTING)
    {
        DEBUG_BEGIN_TIMER(DoUpdateLighting)
        UpdateLighting();
        DEBUG_END_TIMER(DoUpdateLighting)
    }

    // Clear max screen size ready for next rendering
    m_MaxScreenPixelSize = 0.0f ;
    
    //=====================================================================
	// Advance view vars
    //=====================================================================

    // Update show zoom info
    if (m_ViewShowZoomTime > 0)
    {
        m_ViewShowZoomTime -= DeltaTime ;
        if (m_ViewShowZoomTime < 0)
            m_ViewShowZoomTime = 0 ;
    }

    view_type ViewType;
    if( IsSatDownInVehicle() )
        ViewType = GetVehicleViewType();
    else
        ViewType = GetPlayerViewType();

    // Advance view blend
    switch( ViewType )
    {
        case VIEW_TYPE_1ST_PERSON:
			if (m_ViewBlend > 0.0f)
			{
				m_ViewBlend -= s_CommonInfo.VIEW_BLEND_SPEED * DeltaTime ;
				if (m_ViewBlend < 0.0f)
					m_ViewBlend = 0.0f ;
			}
			break ;

        case VIEW_TYPE_3RD_PERSON:
			if (m_ViewBlend < 1.0f)
			{
				m_ViewBlend += s_CommonInfo.VIEW_BLEND_SPEED * DeltaTime ;
				if (m_ViewBlend > 1.0f)
					m_ViewBlend = 1.0f ;
			}
			break ;
    }

    //=====================================================================
    // Advance camera
    //=====================================================================
    
    vector3 ViewPos;
    radian3 ViewRot( R_0, R_0, R_0 );
    f32     SpringKs;
    f32     SpringKd;
    xbool   ForceSpringCam = FALSE;

    // Get the basic fixed pole camera orientation and acceleration constants
    vehicle* pVehicle = IsAttachedToVehicle();
    if( pVehicle && IsVehiclePilot() )
    {
        pVehicle->Get3rdPersonView( m_VehicleSeat, ViewPos, ViewRot );
        SpringKs = VehicleCamRotKs;
        SpringKd = VehicleCamRotKd;
        ForceSpringCam = TRUE;
    }
    else
    {
        ViewRot  = m_Rot;
        SpringKs = PlayerCamRotKs;
        SpringKd = PlayerCamRotKd;
    }

    // Apply any yaw
    ViewRot.Yaw += m_ViewFreeLookYaw;
    ViewRot.ModAngle2();

    if( (UseSpringCam == TRUE) || (ForceSpringCam == TRUE) )
    {
        //
        // Update Spring Camera
        //

        radian PitchDist   = x_ModAngle2( ViewRot.Pitch - m_CamRotAng.Pitch );
        radian YawDist     = x_ModAngle2( ViewRot.Yaw   - m_CamRotAng.Yaw   );
        
        f32    PitchVel    = (m_CamRotOld.Pitch - ViewRot.Pitch) * DeltaTime;
        f32    YawVel      = (m_CamRotOld.Yaw   - ViewRot.Yaw  ) * DeltaTime;
        
        f32    PitchForce  = (SpringKs * PitchDist) - (SpringKd * PitchVel);
        f32    YawForce    = (SpringKs * YawDist  ) - (SpringKd * YawVel  );
        
        m_CamRotOld.Pitch  = m_CamRotAng.Pitch;
        m_CamRotOld.Yaw    = m_CamRotAng.Yaw;
        
        m_CamRotAng.Pitch += PitchForce * DeltaTime;
        m_CamRotAng.Yaw   += YawForce   * DeltaTime;

        m_CamRotAng.ModAngle2();

        //
        // Update camera turning
        //

        radian3 CamMaxAng( R_0, R_0, R_0 );
        
        if( pVehicle )
        {
            CamMaxAng.Pitch = VehicleMaxPitchAng;
            CamMaxAng.Yaw   = VehicleMaxYawAng;
            SpringKs        = VehicleCamTiltKs;
            SpringKd        = VehicleCamTiltKd;
        }
        else
        {
            CamMaxAng.Pitch = PlayerMaxPitchAng;
            CamMaxAng.Yaw   = PlayerMaxYawAng;
            SpringKs        = PlayerCamTiltKs;
            SpringKd        = PlayerCamTiltKd;
        }
        
        // For split-screen we have to limit the look vertically
        if( tgl.NLocalPlayers > 1 )
        {
            CamMaxAng.Pitch *= 0.25f;
        }

        radian TargetPitch = m_Move.LookY * CamMaxAng.Pitch;
        radian TargetYaw   = m_Move.LookX * CamMaxAng.Yaw;
        
        // When the player dies we are in free look mode so remove any camera turning
        if( (GetHealth() == 0.0f) || m_Move.FreeLookKey )
        {
            TargetPitch = R_0;
            TargetYaw   = R_0;
        }
        
        PitchDist           = x_ModAngle2( TargetPitch - m_CamTiltAng.Pitch );
        YawDist             = x_ModAngle2( TargetYaw   - m_CamTiltAng.Yaw   );
        
        PitchVel            = (m_CamTiltOld.Pitch - TargetPitch) * DeltaTime;
        YawVel              = (m_CamTiltOld.Yaw   - TargetYaw  ) * DeltaTime;
        
        PitchForce          = (SpringKs * PitchDist) - (SpringKd * PitchVel);
        YawForce            = (SpringKs * YawDist  ) - (SpringKd * YawVel  );
        
        m_CamTiltOld.Pitch  = m_CamTiltAng.Pitch;
        m_CamTiltOld.Yaw    = m_CamTiltAng.Yaw;
        
        m_CamTiltAng.Pitch += PitchForce * DeltaTime;
        m_CamTiltAng.Yaw   += YawForce   * DeltaTime;

        m_CamTiltAng.ModAngle2();
    }
    else
    {
        m_CamTiltAng = radian3( R_0, R_0, R_0 ); 
    }

    //=====================================================================
    // Advance screen flash
    //=====================================================================

    // Running?
    if (m_ScreenFlashCount > 0)
    {
        m_ScreenFlashCount -= m_ScreenFlashSpeed * DeltaTime ;
        if (m_ScreenFlashCount < 0)
        {
            m_ScreenFlashCount = 0 ;
            m_ScreenFlashSpeed = 0 ;
        }
    }

    // This is used to try and determine if we should do a higher powered
    // jetting "kick" to get the motor going. Thrusts within 1 second of
    // each other will not cause another kick to go.
    if (m_FeedbackKickDelay)
    {
        m_FeedbackKickDelay -= DeltaTime;
        if (m_FeedbackKickDelay <= 0.0f)
            m_FeedbackKickDelay = 0.0f;
    }

    //=====================================================================
    // Update jet sounds
    //=====================================================================

	// Start/stop the jet sfx effect
    if (m_Jetting)
    {
        if (m_ProcessInput)
        {
            // Give it a little kick to get going when the jet is first started. There
            // seems to be a minimum threshold at which the motor has a problem starting.
            if ( (!m_JetSfxID) && (m_FeedbackKickDelay < 0.1f) )
            {
                input_Feedback(0.1f,0.4f,m_ControllerID);
            }
            else
            {
                input_Feedback(0.15f,0.15f,m_ControllerID);
            }
            m_FeedbackKickDelay = 1.0f;
        }

        // Play and update the jet sound
        audio_PlayLooping(m_JetSfxID, SFX_ARMOR_THRUST_LP1, &m_WorldPos) ;

        // Provide force feedback. Do this for a short time when the jet
        // position is updated so it will "automagically" stop when the sound
        // is stopped.
    }
    else
    {
	    // Stop the jet sfx effect
        audio_StopLooping(m_JetSfxID) ;
    }

    //=====================================================================
    // Update missile target sounds
    //=====================================================================

    // Update missile target sounds
    if (m_ProcessInput)
    {
        // Locked?
        if ((m_MissileTargetLockCount) && (m_Health > 0))
        {
            // Update sounds
            audio_PlayLooping(m_MissileTargetLockSfxID, SFX_WEAPONS_MISSLELAUNCHER_TARGET_INBOUND) ;
            audio_StopLooping(m_MissileTargetTrackSfxID) ;
        }
        else
        // Tracked?
        if ((m_MissileTargetTrackCount) && (m_Health > 0))
        {
            audio_PlayLooping(m_MissileTargetTrackSfxID, SFX_WEAPONS_MISSLELAUNCHER_TARGET_LOCK) ;
            audio_StopLooping(m_MissileTargetLockSfxID) ;
        }
        else
        {
            // Stop both sounds
            audio_StopLooping(m_MissileTargetLockSfxID) ;
            audio_StopLooping(m_MissileTargetTrackSfxID) ;
        }
    }

    // Update missile track counts
    UpdateMissileTargetCounts() ;

    //=====================================================================
    // Advance weapon instance
    //=====================================================================
    if (m_WeaponInstance.GetShape())
        m_WeaponInstance.Advance(DeltaTime) ;

    //=====================================================================
    // Advance backpack instance
    //=====================================================================
    if (m_BackpackInstance.GetShape())
    {
        // Setup pack texture/anim speeds
        switch(m_PackCurrentType)
        {
            case INVENT_TYPE_NONE:
                SetPackActive(FALSE) ;
                break ;

            case INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE:

                // Set to green light
                m_BackpackInstance.SetTextureAnimFPS(0.0f) ;
                m_BackpackInstance.SetTextureFrame(1) ;
                break ;

            case INVENT_TYPE_ARMOR_PACK_SHIELD:

                // Update shield pack sounds
                if (m_PackActive)
                {
                    // Play looping sound, only when startup sound is finished
                    if (!audio_IsPlaying(m_ShieldPackStartSfxID))
                    {
                        if (m_ProcessInput)
                            audio_PlayLooping(m_ShieldPackLoopSfxID, SFX_PACK_SHIELD_ON_LOOP) ;
                        else
                            audio_PlayLooping(m_ShieldPackLoopSfxID, SFX_PACK_SHIELD_ON_LOOP, &m_WorldPos) ;
                    }
                }
                else
                    audio_StopLooping(m_ShieldPackLoopSfxID) ;

                // Fall through to animate textures...

            case INVENT_TYPE_ARMOR_PACK_ENERGY:
                // Animate textures?
                if (m_PackActive)
                {
                    // Pulse the light
                    m_BackpackInstance.SetTextureAnimFPS(5.0f) ;
                    m_BackpackInstance.SetTexturePlaybackType(material::PLAYBACK_TYPE_PING_PONG) ;
                }
                else
                {
                    // Turn light off
                    m_BackpackInstance.SetTextureAnimFPS(0) ;
                    m_BackpackInstance.SetTextureFrame(0) ;
                }
                break ;

            case INVENT_TYPE_ARMOR_PACK_REPAIR:

                // Animate textures?
                if ((m_WeaponCurrentType == INVENT_TYPE_WEAPON_REPAIR_GUN) && (m_WeaponState == WEAPON_STATE_FIRE))
                {
                    // Pulse the light
                    m_BackpackInstance.SetTextureAnimFPS(5.0f) ;
                    m_BackpackInstance.SetTexturePlaybackType(material::PLAYBACK_TYPE_PING_PONG) ;
                }
                else
                {
                    // Turn light off
                    m_BackpackInstance.SetTextureAnimFPS(0) ;
                    m_BackpackInstance.SetTextureFrame(0) ;
                }
                break ;
        }

        // Advance texture/anim
        m_BackpackInstance.Advance(DeltaTime) ;
    }            

    //=====================================================================
    // Advance instances
    //=====================================================================
    if (m_PlayerInstance.GetShape())
    {
        // Accumulate anim motion?
        switch(m_PlayerState)
        {
            case PLAYER_STATE_TAUNT:
            case PLAYER_STATE_DEAD:
       
                // Advance anim and get movement
                m_PlayerInstance.Advance(DeltaTime, &DeltaPos, NULL) ;

                // Keep xz delta translation
                m_AnimMotionDeltaPos = vector3(DeltaPos.X, 0, DeltaPos.Z) ;
                m_AnimMotionDeltaPos.RotateY(m_Rot.Yaw) ;
            
                // If on the ground, re-direct motion along the ground
                if (m_ContactInfo.RunSurface)
                    m_AnimMotionDeltaPos = m_ContactInfo.BelowRotation.Rotate(m_AnimMotionDeltaPos) ;

                // No anim rotation...
                m_AnimMotionDeltaRot.Zero() ;

                // Remove root xz only
                m_PlayerInstance.SetRemoveRootNodePosX(TRUE) ;
                m_PlayerInstance.SetRemoveRootNodePosY(FALSE) ;
                m_PlayerInstance.SetRemoveRootNodePosZ(TRUE) ;
                break ;

            // Regular states the don't need xz (and y) position
            default:
                // Advance player
                m_PlayerInstance.Advance(DeltaTime) ;
                m_AnimMotionDeltaRot.Zero() ;
                m_AnimMotionDeltaPos.Zero() ;

                // Sat down in a vehicle?
                if (IsSatDownInVehicle())
                {
                    // Remove all root xyz
                    m_PlayerInstance.SetRemoveRootNodePosX(TRUE) ;
                    m_PlayerInstance.SetRemoveRootNodePosY(TRUE) ;
                    m_PlayerInstance.SetRemoveRootNodePosZ(TRUE) ;
                }
                else
                {
                    // Remove root xz only
                    m_PlayerInstance.SetRemoveRootNodePosX(TRUE) ;
                    m_PlayerInstance.SetRemoveRootNodePosY(FALSE) ;
                    m_PlayerInstance.SetRemoveRootNodePosZ(TRUE) ;
                }
                break ;
        }

        // Advance backpack if there is one and it has been activated
        if ((m_BackpackInstance.GetShape()) && (m_PackActive))
            m_BackpackInstance.Advance(DeltaTime) ;

        // Advance flag if there is one
        if (m_FlagInstance.GetShape())
            m_FlagInstance.Advance(DeltaTime) ;

        // Check for deleting the player recoil anim
        anim_state *Recoil = m_PlayerInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_RECOIL) ;
        if ((Recoil) && (Recoil->GetAnimPlayed()))
            m_PlayerInstance.DeleteAdditiveAnim(ADDITIVE_ANIM_ID_RECOIL) ;

        // Keep frames for re-starting run anims
        xbool PlayingRunAnim = FALSE ;
        anim* Anim = m_PlayerInstance.GetCurrentAnimState().GetAnim() ;
        ASSERT(Anim) ;
        f32 FrameCount = (f32)Anim->GetFrameCount() ;
        f32 Frame      = m_PlayerInstance.GetCurrentAnimState().GetFrame() / (FrameCount-1.0f) ;
        switch(Anim->GetType())
        {
            case ANIM_TYPE_CHARACTER_SIDE:
                m_SideAnimFrame = Frame ;
                PlayingRunAnim  = TRUE ;
                break ;

            case ANIM_TYPE_CHARACTER_FORWARD:
            case ANIM_TYPE_CHARACTER_BACKWARD:
                m_ForwardBackwardAnimFrame = Frame ;
                PlayingRunAnim             = TRUE ;
                break ;
        }

        //=====================================================================
        // Make running sound
        //=====================================================================

        // Make running sound?
        if ( (m_ContactInfo.RunSurface) && (PlayingRunAnim) && (!m_PlayerInstance.IsBlending()) )
        {
            // Anim frame lookup table for left foot hitting the ground for forward, backward, and side anims
            static f32 FootHitGroundFrame[CHARACTER_TYPE_TOTAL][ARMOR_TYPE_TOTAL][3] =
            {
                // CHARACTER_TYPE_FEMALE
                {
                    {9,7,7},            // ARMOR_TYPE_LIGHT
                    {9,15,5},           // ARMOR_TYPE_MEDIUM
                    {11, 7.5f, 11.5f},  // ARMOR_TYPE_HEAVY
                },
                // CHARACTER_TYPE_MALE
                {
                    {7,6,6.5f},         // ARMOR_TYPE_LIGHT
                    {8,5,7},            // ARMOR_TYPE_MEDIUM
                    {11, 7.5f, 11.5f},  // ARMOR_TYPE_HEAVY
                },
                // CHARACTER_TYPE_BIO
                {
                    {9,8,8},            // ARMOR_TYPE_LIGHT
                    {9,6,9},            // ARMOR_TYPE_MEDIUM
                    {12,9,12},          // ARMOR_TYPE_HEAVY
                }
            } ;

            // Sound lookup table for left foot hitting the ground
            static s32 LeftFootHitGroundSfx[ARMOR_TYPE_TOTAL][contact_info::SURFACE_TYPE_TOTAL] =
            {
                // ARMOR_TYPE_LIGHT
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE
                    SFX_ARMOR_LIGHT_LF_SOFT,    // SURFACE_TYPE_SOFT
                    SFX_ARMOR_LIGHT_LF_HARD,    // SURFACE_TYPE_HARD
                    SFX_ARMOR_LIGHT_LF_SNOW,    // SURFACE_TYPE_SNOW
                    SFX_ARMOR_LIGHT_LF_HARD,    // SURFACE_TYPE_METAL
                },

                // ARMOR_TYPE_MEDIUM
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE
                    SFX_ARMOR_MED_LF_SOFT,      // SURFACE_TYPE_SOFT
                    SFX_ARMOR_MED_LF_HARD,      // SURFACE_TYPE_HARD
                    SFX_ARMOR_MED_LF_SNOW,      // SURFACE_TYPE_SNOW
                    SFX_ARMOR_MED_LF_HARD,      // SURFACE_TYPE_METAL
                },

                // ARMOR_TYPE_HEAVY
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE 
                    SFX_ARMOR_HEAVY_LF_SOFT,    // SURFACE_TYPE_SOFT
                    SFX_ARMOR_HEAVY_LF_HARD,    // SURFACE_TYPE_HARD
                    SFX_ARMOR_HEAVY_LF_SNOW,    // SURFACE_TYPE_SNOW
                    SFX_ARMOR_HEAVY_LF_HARD,    // SURFACE_TYPE_METAL
                },
            } ;

            // Sound lookup table for right foot hitting the ground
            static s32 RightFootHitGroundSfx[ARMOR_TYPE_TOTAL][contact_info::SURFACE_TYPE_TOTAL] =
            {
                // ARMOR_TYPE_LIGHT
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE
                    SFX_ARMOR_LIGHT_RF_SOFT,    // SURFACE_TYPE_SOFT
                    SFX_ARMOR_LIGHT_RF_HARD,    // SURFACE_TYPE_HARD
                    SFX_ARMOR_LIGHT_RF_SNOW,    // SURFACE_TYPE_SNOW
                    SFX_ARMOR_LIGHT_RF_HARD     // SURFACE_TYPE_METAL
                },

                // ARMOR_TYPE_MEDIUM
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE
                    SFX_ARMOR_MED_RF_SOFT,      // SURFACE_TYPE_SOFT
                    SFX_ARMOR_MED_RF_HARD,      // SURFACE_TYPE_HARD
                    SFX_ARMOR_MED_RF_SNOW,      // SURFACE_TYPE_SNOW
                    SFX_ARMOR_MED_RF_HARD,      // SURFACE_TYPE_METAL
                },

                // ARMOR_TYPE_HEAVY
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE 
                    SFX_ARMOR_HEAVY_RF_SOFT,    // SURFACE_TYPE_SOFT
                    SFX_ARMOR_HEAVY_RF_HARD,    // SURFACE_TYPE_HARD
                    SFX_ARMOR_HEAVY_RF_SNOW,    // SURFACE_TYPE_SNOW
                    SFX_ARMOR_HEAVY_RF_HARD,    // SURFACE_TYPE_METAL
                },
            } ;

            // Lookup anim index into lookup table
            s32 AnimLookupIndex ;
            anim* Anim = m_PlayerInstance.GetCurrentAnimState().GetAnim() ;
            ASSERT(Anim) ;
            switch(Anim->GetType())
            {
                default:                            AnimLookupIndex = -1 ;  break ;
                case ANIM_TYPE_CHARACTER_FORWARD:   AnimLookupIndex = 0 ;   break ;
                case ANIM_TYPE_CHARACTER_BACKWARD:  AnimLookupIndex = 1 ;   break ;
                case ANIM_TYPE_CHARACTER_SIDE:      AnimLookupIndex = 2 ;   break ;
            }
    
            // Play sound?
            if (AnimLookupIndex != -1)
            {
                // Must be an anim looup
                ASSERT(AnimLookupIndex >= 0) ;
                ASSERT(AnimLookupIndex <  3) ;

                // Must be valid player
                ASSERT(m_CharacterType >= CHARACTER_TYPE_START) ;
                ASSERT(m_CharacterType <= CHARACTER_TYPE_END) ;

                ASSERT(m_ArmorType >= ARMOR_TYPE_START) ;
                ASSERT(m_ArmorType <= ARMOR_TYPE_END) ;

                // Lookup feet hitting the ground frame
                f32 LF = FootHitGroundFrame[m_CharacterType][m_ArmorType][AnimLookupIndex] ;
    
                // Original anims are at 30fps, but they may have been compressed
                LF *= Anim->GetFps() / 30.0f ;

                // Calc the other foots frame
                f32 RF = LF + (Anim->GetFrameCount() * 0.5f) ;
                if (RF >= Anim->GetFrameCount())
                    RF -= Anim->GetFrameCount() ;

                // Lookup last frame and current frame, making sure Current > Last
                f32 LastFrame, CurrentFrame ;
                if (m_PlayerInstance.GetCurrentAnimState().GetSpeed() > 0)
                {
                    // Playing forwards
                    LastFrame    = m_LastRunFrame ;
                    CurrentFrame = m_PlayerInstance.GetCurrentAnimState().GetFrame() ;
                }
                else
                {
                    // Playing backwards
                    CurrentFrame = m_LastRunFrame ;
                    LastFrame    = m_PlayerInstance.GetCurrentAnimState().GetFrame() ;
                }

                // Account for looping
                f32 LastFrameA, LastFrameB, CurrentFrameA, CurrentFrameB ;
                if (LastFrame > CurrentFrame)
                {
                    // Check for end of anim
                    LastFrameA    = LastFrame ;
                    CurrentFrameA = (f32)Anim->GetFrameCount() ;

                    // Check for start of anim
                    LastFrameB    = -1 ;
                    CurrentFrameB = CurrentFrame ;
                }
                else
                {
                    // Check inbetween times
                    LastFrameA    = LastFrame ;
                    CurrentFrameA = CurrentFrame ;

                    // Don't check
                    LastFrameB    = -1 ;
                    CurrentFrameB = -1 ;
                }

                // Lookup sound id
                s32 SoundID = SOUNDTYPE_NONE ;

                // Play left foot sound?
                if (    ((LF > LastFrameA) && (LF <= CurrentFrameA)) ||
                        ((LF > LastFrameB) && (LF <= CurrentFrameB)) )
                {
                    // Lookup random foot sound
                    if (x_irand(0, 100) > 50)
                        SoundID = LeftFootHitGroundSfx[m_ArmorType][m_ContactInfo.RunSurface] ;
                    else
                        SoundID = RightFootHitGroundSfx[m_ArmorType][m_ContactInfo.RunSurface] ;
                }

                // Play right foot sound?
                if (    ((RF > LastFrameA) && (RF <= CurrentFrameA)) ||
                        ((RF > LastFrameB) && (RF <= CurrentFrameB)) )
                {
                    // Lookup random foot sound
                    if (x_irand(0, 100) > 50)
                        SoundID = LeftFootHitGroundSfx[m_ArmorType][m_ContactInfo.RunSurface] ;
                    else
                        SoundID = RightFootHitGroundSfx[m_ArmorType][m_ContactInfo.RunSurface] ;
                }

                // Play sound?
                if (SoundID != SOUNDTYPE_NONE)
                {
                    static f32 PLAYER_FOOT_FALL_BASE_VOL   = 0.70f ;
                    static f32 PLAYER_FOOT_FALL_SPEED_VOL  = 0.25f ;
                    static f32 PLAYER_FOOT_FALL_RANDOM_VOL = 0.05f ;

                    // Calc volume
                    f32 Volume = PLAYER_FOOT_FALL_BASE_VOL +
                                 (ABS(m_PlayerInstance.GetCurrentAnimState().GetSpeed()) * PLAYER_FOOT_FALL_SPEED_VOL) +
                                 x_frand(-PLAYER_FOOT_FALL_RANDOM_VOL, PLAYER_FOOT_FALL_RANDOM_VOL) ;

                    // Calc random pitch
                    f32 Pitch = 1.0f + x_frand(-0.05f, 0.05f) ;

                    // Start the sound and set the new volume and pitch
                    SoundID = audio_Play(SoundID, &m_WorldPos) ;
                    audio_SetVolume(SoundID, Volume) ;
                    audio_SetPitch(SoundID, Pitch) ;
                }
            }
        }
    
        // Keep run frame for next time
        m_LastRunFrame = m_PlayerInstance.GetCurrentAnimState().GetFrame() ;

        //=====================================================================
        // Make landing sound
        //=====================================================================

        // Just hit a surface?
        //x_printfxy(2,10, "m_LastContactSurface:%d", m_LastContactSurface) ;
        //x_printfxy(2,11, "m_ContactInfo.ContactSurface:%d", m_ContactInfo.ContactSurface) ;
        if (       (m_LastContactSurface == contact_info::SURFACE_TYPE_NONE)
                && (m_ContactInfo.ContactSurface != contact_info::SURFACE_TYPE_NONE) )
        {
            // Sound lookup table for players landing on the ground
            static s32 LandGroundSfx[ARMOR_TYPE_TOTAL][contact_info::SURFACE_TYPE_TOTAL] =
            {
                // ARMOR_TYPE_LIGHT
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE
                    SFX_ARMOR_LIGHT_LAND_SOFT,  // SURFACE_TYPE_SOFT
                    SFX_ARMOR_LIGHT_LAND_HARD,  // SURFACE_TYPE_HARD
                    SFX_ARMOR_LIGHT_LAND_SNOW,  // SURFACE_TYPE_SNOW
                    SFX_ARMOR_LIGHT_LAND_HARD   // SURFACE_TYPE_METAL
                },

                // ARMOR_TYPE_MEDIUM
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE
                    SFX_ARMOR_MED_LAND_SOFT,    // SURFACE_TYPE_SOFT
                    SFX_ARMOR_MED_LAND_HARD,    // SURFACE_TYPE_HARD
                    SFX_ARMOR_MED_LAND_SNOW,    // SURFACE_TYPE_SNOW
                    SFX_ARMOR_MED_LAND_HARD     // SURFACE_TYPE_METAL
                },

                // ARMOR_TYPE_LIGHT
                {   SOUNDTYPE_NONE,             // SURFACE_TYPE_NONE
                    SFX_ARMOR_HEAVY_LAND_SOFT,  // SURFACE_TYPE_SOFT
                    SFX_ARMOR_HEAVY_LAND_HARD,  // SURFACE_TYPE_HARD
                    SFX_ARMOR_HEAVY_LAND_SNOW,  // SURFACE_TYPE_SNOW
                    SFX_ARMOR_HEAVY_LAND_HARD   // SURFACE_TYPE_METAL
                },
            } ;

            // Start with impact speed
            f32 LandSpeed = m_GroundImpactSpeed ;

            // If player speed is faster than impact speed, then use it!
            f32 PlayerSpeed = m_Vel.LengthSquared() ;
            if (PlayerSpeed > 0.00001f)
            {
                PlayerSpeed = x_sqrt(PlayerSpeed) ;
                if (PlayerSpeed > LandSpeed)
                    LandSpeed = PlayerSpeed ;
            }

            // Lookup sound to play
            s32 SoundID = LandGroundSfx[m_ArmorType][m_ContactInfo.ContactSurface] ;

            static f32 PLAYER_LAND_BASE_VOL   = 0.30f ;
            static f32 PLAYER_LAND_SPEED_VOL  = 0.1f ;

            // Calc volume
            f32 Volume = PLAYER_LAND_BASE_VOL + (PLAYER_LAND_SPEED_VOL * LandSpeed) ;
            if (Volume > 1.0f)
                Volume = 1.0f ;

            // Calc random pitch
            f32 Pitch = 1.0f + x_frand(-0.05f, 0.05f) ;

            // Start the sound and set the new volume and pitch
            SoundID = audio_Play(SoundID, &m_WorldPos) ;
            audio_SetVolume(SoundID, Volume) ;
            audio_SetPitch(SoundID, Pitch) ;
        }
    }

    // Keep contact surface for next time
    m_LastContactSurface = m_ContactInfo.ContactSurface ;

    //=====================================================================
    // Update draw blending
    //=====================================================================
    if (m_BlendMove.Blend != 0)
    {
        // Next blend
        m_BlendMove.Blend += m_BlendMove.BlendInc * DeltaTime ;
        if (m_BlendMove.Blend < 0)
            m_BlendMove.Blend = 0 ;
    }

    // Get ground rot amount (0=none, 1=all)
    if (IsAttachedToVehicle())
        m_GroundOrientBlend = 1 ;                   // Use all vehicle rot
    else
    if (m_Health == 0)
        m_GroundOrientBlend += DeltaTime / 2.0f ;   // Blend to ground rot for dying 
    else
        m_GroundOrientBlend = 0.0f ;                // Ident

    // Keep 0->1
    if (m_GroundOrientBlend < 0)
        m_GroundOrientBlend = 0 ;
    else
    if (m_GroundOrientBlend > 1)
        m_GroundOrientBlend = 1 ;

    // If dying, or taunting, blend to anim rotation
    switch(m_PlayerState)
    {
        case PLAYER_STATE_TAUNT:
        case PLAYER_STATE_DEAD:
            m_WeaponOrientBlend += DeltaTime / 1.0f ;
            break ;
        
        // Blend back to player controlled angle
        default:
            m_WeaponOrientBlend -= DeltaTime / 0.5f ;
            break ;
    }
    
    // Keep 0->1
    if (m_WeaponOrientBlend < 0)
        m_WeaponOrientBlend = 0 ;
    else
    if (m_WeaponOrientBlend > 1)
        m_WeaponOrientBlend = 1 ;


    //==========================================================================
    // Setup blend draw view free look which is used to turn the head
    // (for now I've ignored the m_BlendMove value since it looks better this way)
    //==========================================================================

    // Calc 1 >= FreeLookYaw >= 0
    f32 FreeLookYaw = (R_360 - (m_ViewFreeLookYaw + R_180)) / R_360 ;
    if (FreeLookYaw < 0)
        FreeLookYaw = 0 ;
    else
    if (FreeLookYaw > 1)
        FreeLookYaw = 1 ;

    // Blend to it proportional to the dist between them.
    // (0.1f is the min speed so that settling occurs)
    f32 FreeLookSpeed = (0.1f + (ABS(FreeLookYaw - m_DrawViewFreeLookYaw) * 3.0f)) * DeltaTime ;
    if (m_DrawViewFreeLookYaw < FreeLookYaw)
    {
        m_DrawViewFreeLookYaw += FreeLookSpeed ;
        if (m_DrawViewFreeLookYaw > FreeLookYaw)
            m_DrawViewFreeLookYaw = FreeLookYaw ;
    }
    else
    if (m_DrawViewFreeLookYaw > FreeLookYaw)
    {
        m_DrawViewFreeLookYaw -= FreeLookSpeed ;
        if (m_DrawViewFreeLookYaw < FreeLookYaw)
            m_DrawViewFreeLookYaw = FreeLookYaw ;
    }

    //=====================================================================
    // Update draw vars
    //=====================================================================
    UpdateDrawVars() ;

    //=====================================================================
    // Update jet particles
    //=====================================================================
    for (i = 0 ; i < m_nJets ; i++)
	    m_JetPfx[i].UpdateEffect( DeltaTime );

    //=====================================================================
    // Update damage wiggle
    //=====================================================================
    if( m_DamageTotalWiggleTime>0.0f )
    {
        m_DamageWiggleTime += DeltaTime;
        if( m_DamageWiggleTime >= m_DamageTotalWiggleTime )
        {
            m_DamageTotalWiggleTime = 0.0f;
            m_DamageWiggleTime = 0.0f;
        }
    }

    //=====================================================================
    // Update spawn time
    //=====================================================================
    if (m_SpawnTime > 0)
    {
        m_SpawnTime -= DeltaTime ;
        if (m_SpawnTime < 0)
        {
            m_SpawnTime = 0 ;
            SetArmed( TRUE );
        }
    }

    //=====================================================================
    // Update dismount time
    //=====================================================================
    if (m_VehicleDismountTime > 0)
    {
        m_VehicleDismountTime -= DeltaTime ;
        if (m_VehicleDismountTime < 0)
            m_VehicleDismountTime = 0 ;
    }

    //=====================================================================
    // Update out of bounds sfx
    //=====================================================================
    
    // Get current bounds status
    xbool OutOfBounds = IsOutOfBounds() ;
    
    // Just stepped out of bounds?
    if ((OutOfBounds) && (!m_LastOutOfBounds))
    {
        if( m_ProcessInput )
        {
            MsgMgr.Message( MSG_OUT_OF_BOUNDS, m_ObjectID.Slot );
            input_Feedback(1.0f,1.0f,m_ControllerID);       // Rumble really hard when out of bounds
        }
    }

    // Just stepped back in bounds?
    if ((!OutOfBounds) && (m_LastOutOfBounds))
    {
        if( m_ProcessInput )
        {
            MsgMgr.Message( MSG_IN_BOUNDS, m_ObjectID.Slot );
        }
    }

    // Keep for next time
    m_LastOutOfBounds = OutOfBounds ;

    //=====================================================================
    // Update target lock
    //=====================================================================

    if( tgl.ServerPresent )
    {
        if( m_TargetIsLocked )
        {
            if( m_LockAimSolved )
            {
                m_LockBreakTimer = 0.0f;
            }
            else
            {
                m_LockBreakTimer += DeltaTime;
                if( m_LockBreakTimer > BreakLockTime )
                {
                    // Break the target lock.
                    m_TargetIsLocked = FALSE;
                    m_LockedTarget   = obj_mgr::NullID;
                    m_DirtyBits     |= PLAYER_DIRTY_BIT_TARGET_LOCK;
                           
                    // Audio for server.
                    if( m_ProcessInput )
                        audio_Play( SFX_TARGET_LOCK_LOST );
                }
            }
        }
    }
}

//=============================================================================

//==============================================================================
//
// VIEW FUNCTIONS
//
//==============================================================================

// Returns string to describe view
const char *player_object::GetViewZoomFactorString()
{
    switch(fegl.WarriorSetups[m_WarriorID].ViewZoomFactor)
    {
        default:
            ASSERT(0) ;
        case 0:    return "x1" ;
        case 1:    return "x2" ;
        case 2:    return "x5" ;
        case 3:    return "x10" ;
        case 4:    return "x20" ;
    }
}

//==============================================================================

f32 player_object::GetViewZoomFactor()
{
    switch(fegl.WarriorSetups[m_WarriorID].ViewZoomFactor)
    {
        default:    ASSERT(0) ;
        case 0:     return 1.0f ;
        case 1:     return 2.0f ;
        case 2:     return 5.0f ;
        case 3:     return 10.0f ;
        case 4:     return 20.0f ;
    }
}

//==============================================================================

f32 player_object::GetCurrentZoomFactor( void ) const
{
    switch(m_Move.ViewZoomFactor)
    {
        default:    ASSERT(FALSE) ;
        case 0:     return  1.0f ;
        case 1:     return  2.0f ;
        case 2:     return  5.0f ;
        case 3:     return 10.0f ;
        case 4:     return 20.0f ;
    }
}

//==============================================================================

f32 player_object::GetCurrentFOV( void ) const
{
    return s_CommonInfo.VIEW_X1_FOV / GetCurrentZoomFactor() ;
}

//==============================================================================

void player_object::SetupValidViewPos(const vector3& Start, vector3& ViewPos) const
{
    // Get movement of ray
    vector3 Move = ViewPos - Start ;

    // Skip if too small
    if (Move.LengthSquared() < collider::MIN_EXT_DIST_SQR)
        return ;

    // Setup start bbox
    bbox StartBBox(Start, s_CommonInfo.VIEW_BACKOFF_DIST) ;
   
    // Setup extrusion collision
    s_Collider.ClearExcludeList() ;
	s_Collider.ExtSetup(this, StartBBox, Move) ;
    u32 Mask = PLAYER_COLLIDE_ATTRS ;

    // Exclude the vehicle the player is on
    vehicle* Vehicle = IsAttachedToVehicle();
    if( Vehicle )
        s_Collider.Exclude( Vehicle->GetObjectID().GetAsS32() ); 

    // Do the collision check...
    ObjMgr.Collide( Mask, s_Collider ) ;

    // Get results
    collider::collision Collision;
    s_Collider.GetCollision( Collision );

    // If hit something?
    if (Collision.Collided)
    {            
        // Put view pos at collision pt
        ViewPos = Start + (Move * Collision.T) ;
    }
}

//==============================================================================

f32 MinCamHeight = 0.0f;    // Height of look at point above corpse
f32 MaxCamDist   = 0.0f;    // Distance in meters from corpse

f32 player_object::Get3rdPersonView( vector3 &ViewPos, radian3 &ViewRot, f32 &ViewFOV ) const
{
    if( m_ViewZoomState == VIEW_ZOOM_STATE_ON )
    {
        Get1stPersonView( ViewPos, ViewRot, ViewFOV );
        return( 0.0f );
    }

    f32 CamDistanceT = 1.0f;

    // On vehicle?
    vehicle* pVehicle = IsAttachedToVehicle();
    
    // Start with pitch,yaw,0
	radian3 Rot = m_Rot;
	
    // On vehicle?
    if( pVehicle )
    {
        // If player can look and shoot on vehicle, use the regular view!
        const vehicle::seat& Seat = pVehicle->GetSeat( m_VehicleSeat );
        if( Seat.CanLookAndShoot )
            pVehicle = NULL;
    }

    // Setup height and distance from player for the camera position
    f32 Height = s_CommonInfo.VIEW_3RD_PERSON_HEIGHT;
    f32 Dist   = s_CommonInfo.VIEW_3RD_PERSON_DIST;

    // Take ground rot into account
    radian3 GroundRot( R_0, R_0, R_0 );
    if( m_GroundOrientBlend != 0 )
    {
        quaternion  qGroundRot = BlendToIdentity( m_GroundRot, 1.0f - m_GroundOrientBlend ); // 0 = ident, 1 = ground
        GroundRot = qGroundRot.GetRotation();

        quaternion qPlayerRot = quaternion( Rot );
        Rot = (qGroundRot * qPlayerRot).GetRotation();
    }

    // Get view from vehicle?
    if( pVehicle )
    {
        // Setup start
        vector3 LookAt = vector3( 0.0f, Height + 2.0f, 0.0f );
        LookAt.Rotate( GroundRot );
        LookAt += m_WorldPos;

        Rot = radian3( R_0, R_0, R_0 );
        
        // Get the rigid fixed poly camera position and orientation
        pVehicle->Get3rdPersonView( m_VehicleSeat, ViewPos, Rot );

        // Compute the Local Z-axis of camera
        vector3 LocalZ( 0.0f, 0.0f, 1.0f );
        LocalZ.Rotate ( m_CamRotAng );

        vector3 ViewVec = LookAt - ViewPos;

        // Compute new view position        
        ViewPos = LookAt - (LocalZ * ViewVec.Length());
        
        // Get the spring camera orientation
        Rot = m_CamRotAng;
        
        // Calculate distance from camera location to look at point
        f32 CamDist = (m_WorldPos - ViewPos).Length();
 
        // Collision check
        SetupValidViewPos( m_WorldPos, ViewPos );
        
        // Calculate parametric value for distance of camera from look at point
        f32 D = (m_WorldPos - ViewPos).Length();
        CamDistanceT = D / CamDist;
    
        // Apply any local camera yaw
        matrix4 M( radian3( R_0, m_CamTiltAng.Yaw, R_0 ) );
        M  *= matrix4( Rot );
        Rot = M.GetRotation();
    }
    else
    {
        // Adjust height and distance from player
        Height *= 1.0f - m_GroundOrientBlend;
        Dist   += MaxCamDist * m_GroundOrientBlend;
        
        // Clamp the height so we dont go inside the player
        if( Height < MinCamHeight )
            Height = MinCamHeight;
    
        // Setup look at point above players head
        vector3 LookAt = vector3( 0.0f, Height, 0.0f );
        LookAt.Rotate( GroundRot );
        LookAt += m_WorldPos;

        // When the player dies we should orient the camera axes to the ground orientation.
        // If the player is not dead then this has no effect on the spring camera orientation
        quaternion Q( GroundRot );
        Q *= quaternion( m_CamRotAng );
        Rot = Q.GetRotation();

        // Compute the Local Z-axis of camera
        vector3 LocalZ( 0.0f, 0.0f, 1.0f );
        LocalZ.Rotate( Rot );
        
        // Compute new view position        
        ViewPos = LookAt - (LocalZ * Dist);

        // Get the location of the first person camera
        vector3 TempViewPos;
        radian3 TempViewRot;
        f32     TempViewFOV;
        Get1stPersonView( TempViewPos, TempViewRot, TempViewFOV );

        // Calculate distance from camera location to players eye point
        f32 CamDist = (TempViewPos - ViewPos).Length();

        // Collision check
        SetupValidViewPos( TempViewPos, ViewPos );
    
        // Calculate parametric value for distance of camera to eye point
        f32 D = (TempViewPos - ViewPos).Length();
        CamDistanceT = D / CamDist;
    
        // Apply any local camera rotation
        matrix4 M( radian3( m_CamTiltAng.Pitch, m_CamTiltAng.Yaw, R_0 ) );
        M  *= matrix4( Rot );
        Rot = M.GetRotation();
    }

    // Set view
    ViewRot = Rot;
    ViewFOV = s_CommonInfo.VIEW_X1_FOV;
    
    // Fade out player when he obscures the reticle
    radian Ang = ABS( m_CamRotAng.Pitch );
    
    if( (Ang > PlayerFadeAng) && (IsDead() == FALSE) )
    {
        f32 T = 1.0f - ( (Ang - PlayerFadeAng) / (s_CommonInfo.VIEW_MAX_PITCH - PlayerFadeAng) );

        if( T < CamDistanceT )
            CamDistanceT = T;
    }

    // Ensure the player doesnt disappear completely
    if( CamDistanceT < 0.1f )
        CamDistanceT = 0.1f;
    
    return( CamDistanceT );
}

//==============================================================================

f32 player_object::Get1stPersonView(vector3 &ViewPos, radian3 &ViewRot, f32 &ViewFOV) const
{
    // Get field of view
    ViewFOV = GetCurrentFOV();

    // Get view from vehicle?
    vehicle* pVehicle = IsSatDownInVehicle();
    if( pVehicle )
    {
        // Get from vehicle instead
        pVehicle->Get1stPersonView( m_VehicleSeat, ViewPos, ViewRot );

        matrix4 CamRot( radian3( m_CamTiltAng.Pitch, m_CamTiltAng.Yaw, R_0 ) );
        matrix4 M( ViewRot );
        M *= CamRot;
        
        ViewRot = M.GetRotation();
        
        if( (pVehicle->GetType() == object::TYPE_VEHICLE_THUNDERSWORD) &&
            (m_ViewZoomState == VIEW_ZOOM_STATE_ON) )
        {
            bomber* pBomber = (bomber*)pVehicle;
            pBomber->GetBombPoint( ViewPos );
            ViewRot  = radian3( R_90, ViewRot.Yaw, R_0 );
        }
    }
    else
    {
        // Use weapon rotation and view position
        ViewRot = m_WeaponRot;
        ViewPos = m_ViewPos;
    }

    return( 1.0f );
}

//==============================================================================

void player_object::GetView(vector3 &ViewPos, radian3 &ViewRot, f32 &ViewFOV) const
{
    vehicle* pVehicle = IsAttachedToVehicle();

    if( pVehicle )
    {
        const vehicle::seat& Seat = pVehicle->GetSeat( m_VehicleSeat );

        if( (Seat.CanLookAndShoot) ||                       // passenger in transport?
            (m_ViewFreeLookYaw != 0) ||                     // currently using free look?
            (m_VehicleViewType == VIEW_TYPE_1ST_PERSON) )   // view set to 1st person?
        {
            Get1stPersonView( ViewPos, ViewRot, ViewFOV );
        }
        else
        {
            Get3rdPersonView( ViewPos, ViewRot, ViewFOV );
        }
    }
    else
    {
        if( (m_PlayerViewType == VIEW_TYPE_1ST_PERSON)  || (m_ViewFreeLookYaw != 0) )
            Get1stPersonView( ViewPos, ViewRot, ViewFOV );
        else
            Get3rdPersonView( ViewPos, ViewRot, ViewFOV );
    }
}

//==============================================================================

// Resets view types from warrior options
void player_object::ResetViewTypes( void )
{
    // Defaults
    m_PlayerViewType  = VIEW_TYPE_1ST_PERSON ;        // Start view
    m_VehicleViewType = VIEW_TYPE_1ST_PERSON ;        // Start view

    // Grab view options from warrior setup
    ASSERT( tgl.PC[0].PlayerIndex > 0 || tgl.PC[0].PlayerIndex < 32 );
    
    if( tgl.NLocalPlayers == 2 )
        ASSERT( tgl.PC[1].PlayerIndex > 0 || tgl.PC[1].PlayerIndex < 32 );

    if( (m_WarriorID == 0) || (m_WarriorID == 1) )
    {
        // Set players view
        if( fegl.WarriorSetups[m_WarriorID].View3rdPlayer )
            SetPlayerViewType( player_object::VIEW_TYPE_3RD_PERSON );
        else
            SetPlayerViewType( player_object::VIEW_TYPE_1ST_PERSON );

        // Set vehicle view
        if( fegl.WarriorSetups[m_WarriorID].View3rdVehicle )
            SetVehicleViewType( player_object::VIEW_TYPE_3RD_PERSON );
        else
            SetVehicleViewType( player_object::VIEW_TYPE_1ST_PERSON );
    }

/*
    if( m_ObjectID.Slot == tgl.PC[0].PlayerIndex )
    {
        // Set players view
        if( fegl.WarriorSetups[0].View3rdPlayer )
            SetPlayerViewType( player_object::VIEW_TYPE_3RD_PERSON );
        else
            SetPlayerViewType( player_object::VIEW_TYPE_1ST_PERSON );

        // Set vehicle view
        if( fegl.WarriorSetups[0].View3rdVehicle )
            SetVehicleViewType( player_object::VIEW_TYPE_3RD_PERSON );
        else
            SetVehicleViewType( player_object::VIEW_TYPE_1ST_PERSON );

    }
    
    if( tgl.NLocalPlayers == 2 )
    {
        if( m_ObjectID.Slot == tgl.PC[1].PlayerIndex )
        {
            // Set players view
            if( fegl.WarriorSetups[1].View3rdPlayer )
                SetPlayerViewType( player_object::VIEW_TYPE_3RD_PERSON );
            else
                SetPlayerViewType( player_object::VIEW_TYPE_1ST_PERSON );

            // Set vehicle view
            if( fegl.WarriorSetups[1].View3rdVehicle )
                SetVehicleViewType( player_object::VIEW_TYPE_3RD_PERSON );
            else
                SetVehicleViewType( player_object::VIEW_TYPE_1ST_PERSON );
        }
    }
*/
}

//==============================================================================

player_object::view_type player_object::GetPlayerViewType( void )
{
    if( IsAttachedToVehicle() && (IsSatDownInVehicle() == FALSE) )
        return( VIEW_TYPE_1ST_PERSON );

    // Return currently selected view
    if( !m_Armed )
        return( VIEW_TYPE_3RD_PERSON );
    else        
        return( m_PlayerViewType );
}

//==============================================================================

void player_object::SetPlayerViewType( view_type ViewType )
{
    m_PlayerViewType = ViewType;
    if( !tgl.ServerPresent )
        m_ViewChanged = TRUE ;
}

//==============================================================================

player_object::view_type player_object::GetVehicleViewType( void )
{
    // Return currently selected view
    return m_VehicleViewType;
}

//==============================================================================

void player_object::SetVehicleViewType( view_type ViewType )
{
    m_VehicleViewType = ViewType;
    if( !tgl.ServerPresent )
        m_ViewChanged = TRUE ;
}

//==============================================================================

view* player_object::GetView( void ) const
{
    return m_View;
}

//==============================================================================

void player_object::SetupView()
{
	vector3 ViewPos ;
	radian3 ViewRot ;
    f32     ViewFOV ;

    // Setup defaults
    ViewPos = m_WorldPos ;
    ViewRot.Zero() ;
    ViewFOV = s_CommonInfo.VIEW_X1_FOV ;

	// Only setup if player has input
	if ((!m_ProcessInput) || (!m_HasInputFocus) || (!m_View))
		return ;

	// All 3rd person?
	if ((m_ViewBlend == 1.0f) || (m_Health == 0))
	{
		m_CamDistanceT = Get3rdPersonView(ViewPos, ViewRot, ViewFOV) ;
		m_View->SetPosition(ViewPos) ;
		m_View->SetRotation(ViewRot) ;
        m_View->SetXFOV(ViewFOV) ;
	}
	else
	// All 1st person?
	if (m_ViewBlend == 0.0f)
	{
		m_CamDistanceT = Get1stPersonView(ViewPos, ViewRot, ViewFOV) ;

        // Add damage wiggle
        if( m_DamageTotalWiggleTime > 0.0f )
        {
            f32 R = m_DamageWiggleRadius*(1-(m_DamageWiggleTime/m_DamageTotalWiggleTime));
            ViewRot.Pitch += x_frand(-R,R);
            ViewRot.Yaw += x_frand(-R,R);
        }

		m_View->SetPosition(ViewPos) ;
		m_View->SetRotation(ViewRot) ;
        m_View->SetXFOV(ViewFOV) ;
	}
	else
	{
		vector3 FirstViewPos, ThirdViewPos ;
		radian3 FirstViewRot, ThirdViewRot ;
		f32     FirstViewFOV, ThirdViewFOV ;
        
		// Get views
		m_CamDistanceT = Get1stPersonView(FirstViewPos, FirstViewRot, FirstViewFOV) ;
		m_CamDistanceT = Get3rdPersonView(ThirdViewPos, ThirdViewRot, ThirdViewFOV) ;
        
		// Blend views
		vector3		BlendViewPos ;
        BlendLinear(FirstViewPos, ThirdViewPos, m_ViewBlend, BlendViewPos) ;

		quaternion	BlendViewRot = Blend (quaternion(FirstViewRot), quaternion(ThirdViewRot), m_ViewBlend) ;
        f32         BlendViewFOV = FirstViewFOV + ((ThirdViewFOV - FirstViewFOV) * m_ViewBlend) ;

		// Setup view
		matrix4 V2W ;
		V2W.Setup(BlendViewRot) ;
        V2W.SetTranslation(BlendViewPos) ;
		m_View->SetV2W(V2W) ;
        m_View->SetXFOV(BlendViewFOV) ;
	}
}

//==============================================================================

// Flash the screen
void player_object::FlashScreen(f32 TotalTime, const xcolor &Color)
{
	// Currently flashing?
	if (m_ScreenFlashCount > 0)
	{
        // don't accumulate the flash unless it's a flash grenade
        if ( Color == XCOLOR_WHITE )
        {
            TotalTime *= 0.5f;  // since we're using a FlashCount of 2.0
            f32 remaining;

            if ( m_ScreenFlashSpeed > 0.0f )
                remaining = m_ScreenFlashCount / m_ScreenFlashSpeed;
            else
                remaining = 0.0f;

            m_ScreenFlashCount = 2.0f ;    
            m_ScreenFlashSpeed = 1.0f / TotalTime;

            // Set dirty bits
            m_DirtyBits |= PLAYER_DIRTY_BIT_SCREEN_FLASH ;
        }

        return;
	}

    // Any flash?
    if (TotalTime > 0)
    {
        // Keep for sending...
        m_ScreenFlashColor = Color ;

        // Start the flash
        if ( Color == XCOLOR_WHITE )
        {
            m_ScreenFlashCount = 2.0f ;                         
            TotalTime *= 0.5f;  // since we're using a FlashCount of 2.0
        }
        else
            m_ScreenFlashCount = 1.0f ;

        m_ScreenFlashSpeed = 1.0f / TotalTime ;

        // Set dirty bits
        m_DirtyBits |= PLAYER_DIRTY_BIT_SCREEN_FLASH ;
    }
}

//==============================================================================

void player_object::PlaySound( s32 SoundType )
{
    // Keep the sound type
    m_SoundType = SoundType ;

    // Only play the sound if player is on this machine...
    if (m_ProcessInput)
        audio_Play(SoundType, &m_WorldPos) ;

    // Set dirty bits
    m_DirtyBits |= PLAYER_DIRTY_BIT_PLAY_SOUND ;
}

//==============================================================================

void player_object::PlayVoiceMenuSound( s32 SfxID, u32 TeamBits, s32 HudVoiceID )
{
    // Animation type                         LF MF HF      LM MM HM     LB MB HB 
    //
    // ANIM_TYPE_CHARACTER_CELEBRATE,                                
    // ANIM_TYPE_CHARACTER_CELEBRATE_BOW,     X  X                  
    // ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,   X     X          X  X 
    // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,  X  X  X       X  X  X            X
    // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,    X  X  X       X  X  X      X  X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_DISCO,      X          X      
    // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,          X       X  X  X      X  X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_ROCKY,                 X  X   
    // ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,         X       X  X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_JUMP,          X             X      X  X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_GORILLA,                            X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_GUN,                                X  X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_ROAR,                               X  X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_SHOOT,                              X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_SWIPE,                              X  X  X
    // ANIM_TYPE_CHARACTER_CELEBRATE_YEAH,                                     X
    // ANIM_TYPE_CHARACTER_TAUNT,                                  
    // ANIM_TYPE_CHARACTER_TAUNT_BEST,        X  X  X       X  X  X      X  X  X
    // ANIM_TYPE_CHARACTER_TAUNT_BUTT,        X  X                 
    // ANIM_TYPE_CHARACTER_TAUNT_IMP,         X  X  X       X  X  X
    // ANIM_TYPE_CHARACTER_TAUNT_KISS,        X  X                 
    // ANIM_TYPE_CHARACTER_TAUNT_BULL,                                   X  X  X
    // ANIM_TYPE_CHARACTER_TAUNT_HEAD,                                         X

    // A very nice lookup table...
    static s32 HudVoiceAnimTypes[CHARACTER_TYPE_TOTAL][ARMOR_TYPE_TOTAL][HUD_VID_TOTAL] =
    {
        // CHARACTER_TYPE_FEMALE
        {
            // ARMOR_TYPE_LIGHT
            // ANIM_TYPE_CHARACTER_CELEBRATE_BOW
            // ANIM_TYPE_CHARACTER_CELEBRATE_DANCE
            // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_BUTT
            // ANIM_TYPE_CHARACTER_TAUNT_IMP
            // ANIM_TYPE_CHARACTER_TAUNT_KISS
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_BOW,      // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_TAUNT_KISS,         // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,   // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,   // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_BUTT,         // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_TAUNT_KISS,         // HUD_VID_TAUNT_AWWW,
            },

            // ARMOR_TYPE_MEDIUM
            // ANIM_TYPE_CHARACTER_CELEBRATE_BOW
            // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_DISCO
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_BUTT
            // ANIM_TYPE_CHARACTER_TAUNT_IMP
            // ANIM_TYPE_CHARACTER_TAUNT_KISS
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_BOW,      // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_TAUNT_KISS,         // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_CELEBRATE_DISCO,    // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_BOW,      // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_DISCO,    // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_BUTT,         // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_TAUNT_KISS,         // HUD_VID_TAUNT_AWWW,
            },

            // ARMOR_TYPE_HEAVY
            // ANIM_TYPE_CHARACTER_CELEBRATE_DANCE
            // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX
            // ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT
            // ANIM_TYPE_CHARACTER_CELEBRATE_JUMP
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_IMP
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_JUMP,     // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,   // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_TAUNT_AWWW,
            }
        },

        // CHARACTER_TYPE_MALE
        {
            // ARMOR_TYPE_LIGHT
            // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_DISCO
            // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX
            // ANIM_TYPE_CHARACTER_CELEBRATE_ROCKY
            // ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_IMP
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_CELEBRATE_ROCKY,    // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_ROCKY,    // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,   // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_DISCO,    // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_DISCO,    // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_TAUNT_AWWW,
            },

            // ARMOR_TYPE_MEDIUM
            // ANIM_TYPE_CHARACTER_CELEBRATE_DANCE
            // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX
            // ANIM_TYPE_CHARACTER_CELEBRATE_ROCKY
            // ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_IMP
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_CELEBRATE_ROCKY,    // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_ROCKY,    // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,   // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_TAUNT_AWWW,
            },

            // ARMOR_TYPE_HEAVY
            // ANIM_TYPE_CHARACTER_CELEBRATE_DANCE
            // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX
            // ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT
            // ANIM_TYPE_CHARACTER_CELEBRATE_JUMP
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_IMP
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_JUMP,     // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,   // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_DANCE,    // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_TAUNT_IMP,          // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_CELEBRATE_TAUNT,    // HUD_VID_TAUNT_AWWW,
            }
        },


        // CHARACTER_TYPE_BIO
        {
            // ARMOR_TYPE_LIGHT
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX
            // ANIM_TYPE_CHARACTER_CELEBRATE_JUMP
            // ANIM_TYPE_CHARACTER_CELEBRATE_GORILLA
            // ANIM_TYPE_CHARACTER_CELEBRATE_GUN
            // ANIM_TYPE_CHARACTER_CELEBRATE_ROAR
            // ANIM_TYPE_CHARACTER_CELEBRATE_SHOOT
            // ANIM_TYPE_CHARACTER_CELEBRATE_SWIPE
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_BULL
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_GORILLA,  // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_GUN,      // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_BULL,         // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_JUMP,     // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_ROAR,     // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_CELEBRATE_SWIPE,    // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_CELEBRATE_ROAR,     // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_CELEBRATE_SHOOT,    // HUD_VID_TAUNT_AWWW,
            },

            // ARMOR_TYPE_MEDIUM
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX
            // ANIM_TYPE_CHARACTER_CELEBRATE_JUMP
            // ANIM_TYPE_CHARACTER_CELEBRATE_GORILLA
            // ANIM_TYPE_CHARACTER_CELEBRATE_GUN
            // ANIM_TYPE_CHARACTER_CELEBRATE_ROAR
            // ANIM_TYPE_CHARACTER_CELEBRATE_SHOOT
            // ANIM_TYPE_CHARACTER_CELEBRATE_SWIPE
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_BULL
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_GORILLA,  // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_GUN,      // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_BULL,         // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_JUMP,     // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_ROAR,     // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_CELEBRATE_SWIPE,    // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_CELEBRATE_ROAR,     // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_CELEBRATE_SHOOT,    // HUD_VID_TAUNT_AWWW,
            },

            // ARMOR_TYPE_HEAVY
            // ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE
            // ANIM_TYPE_CHARACTER_CELEBRATE_WAVE
            // ANIM_TYPE_CHARACTER_CELEBRATE_FLEX
            // ANIM_TYPE_CHARACTER_CELEBRATE_JUMP
            // ANIM_TYPE_CHARACTER_CELEBRATE_GUN
            // ANIM_TYPE_CHARACTER_CELEBRATE_ROAR
            // ANIM_TYPE_CHARACTER_CELEBRATE_SWIPE
            // ANIM_TYPE_CHARACTER_CELEBRATE_YEAH
            // ANIM_TYPE_CHARACTER_TAUNT_BEST
            // ANIM_TYPE_CHARACTER_TAUNT_BULL
            // ANIM_TYPE_CHARACTER_TAUNT_HEAD
            {
                ANIM_TYPE_NONE,                         // HUD_VID_NULL,

                ANIM_TYPE_CHARACTER_CELEBRATE_WAVE,     // HUD_VID_ANIMS1_HI,
                ANIM_TYPE_CHARACTER_CELEBRATE_YEAH,     // HUD_VID_ANIMS1_TAKE_THAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_GUN,      // HUD_VID_ANIMS1_AWWW,
                                            
                ANIM_TYPE_CHARACTER_TAUNT_BULL,         // HUD_VID_ANIMS2_MOVE,
                ANIM_TYPE_CHARACTER_TAUNT_BEST,         // HUD_VID_ANIMS2_IM_GREAT,
                ANIM_TYPE_CHARACTER_CELEBRATE_JUMP,     // HUD_VID_ANIMS2_WOOHOO,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_SALUTE,   // HUD_VID_ANIMS_IM_ON_IT,
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_ANIMS_AWESOME,
                                            
                ANIM_TYPE_CHARACTER_CELEBRATE_FLEX,     // HUD_VID_TAUNT_HAHA,
                ANIM_TYPE_CHARACTER_CELEBRATE_SWIPE,    // HUD_VID_TAUNT_GOT_NUMBER,
                ANIM_TYPE_CHARACTER_CELEBRATE_ROAR,     // HUD_VID_TAUNT_EAT_PLASMA,
                ANIM_TYPE_CHARACTER_TAUNT_HEAD,         // HUD_VID_TAUNT_AWWW,
            }
        }
    } ;

    // Make sure all the anims are present in the player shapes!
    #ifdef X_DEBUG
        for (s32 c = CHARACTER_TYPE_START ; c <= CHARACTER_TYPE_END ; c++)
        {
            for (s32 a = ARMOR_TYPE_START ; a <= ARMOR_TYPE_END ; a++)
            {
                for (s32 v = HUD_VID_START ; v <= HUD_VID_END ; v++)
                {
                    s32 AnimType = HudVoiceAnimTypes[c][a][v] ;
                    if (AnimType != ANIM_TYPE_NONE)
                    {
                        // Get shape for this type
                        shape* Shape = tgl.GameShapeManager.GetShapeByType(s_CharacterInfo[c][a].ShapeType) ;
                        ASSERT(Shape) ;

                        // Make sure the anim is there as expected!
                        ASSERT(Shape->GetAnimByType(AnimType)) ;
                    }
                }
            }
        }
    #endif

    // Must be a valid voice id
    ASSERT(HudVoiceID >= HUD_VID_NULL) ;
    ASSERT(HudVoiceID <  HUD_VID_TOTAL) ;

    // Must be valid player
    ASSERT(m_CharacterType >= CHARACTER_TYPE_START) ;
    ASSERT(m_CharacterType <= CHARACTER_TYPE_END) ;

    ASSERT(m_ArmorType >= ARMOR_TYPE_START) ;
    ASSERT(m_ArmorType <= ARMOR_TYPE_END) ;

    // Do an anim?
    s32 AnimType = HudVoiceAnimTypes[m_CharacterType][m_ArmorType][HudVoiceID] ;

    // Make sure the anim is there as expected!
    ASSERT(m_PlayerInstance.GetShape()) ;
    ASSERT(m_PlayerInstance.GetShape()->GetAnimByType(AnimType)) ;

    // Play the sound over the net and start the taunt anim if any
    SetNetSfxID(SfxID) ;
    SetNetSfxTeamBits(TeamBits) ;
    SetNetSfxTauntAnimType(AnimType) ;
}

//==============================================================================

// Increases health
void player_object::AddHealth(f32 Amount)
{
#ifdef T2_DESIGNER_BUILD
        if (GetType()==object::TYPE_PLAYER)
            return;
#endif
    if ( (fegl.PlayerInvulnerable && (Amount < 0.0f)) )
    {
        if (GetType()==object::TYPE_PLAYER)
            return;
    }

    // Keep copy for setting dirty bits
    f32 LastHealth = m_Health ;

    // Increasing health?
    if ((Amount > 0) && (m_Health < 100))
    {
        // Change amount
        m_Health += Amount ;

        // Cap
        if (m_Health > 100)
            m_Health = 100 ;
    }

    // Decrease health?
    if ((Amount < 0) && (m_Health > 0))
    {
        // Change amount
        m_Health += Amount ;
        if (m_Health <= 0)
            m_Health = 0 ;
    }

    if( g_TweakGame )
    {
        if( m_Health == 0.0f )
            m_Health = 100.0f;
    }

    // Send health?
    if (LastHealth != m_Health)
        m_DirtyBits |= PLAYER_DIRTY_BIT_HEALTH ;
}

//==============================================================================

// Increases energy
void player_object::AddEnergy(f32 Amount)
{
    // Keep copy for setting dirty bits
    f32 LastEnergy = m_Energy ;

    // Change amount
    m_Energy += Amount ;

    // Cap
    if (m_Energy < 0)
        m_Energy = 0 ;
    else
    if (m_Energy > m_MoveInfo->MAX_ENERGY)
        m_Energy = m_MoveInfo->MAX_ENERGY ;

    // Set dirty bit if it's changed
    if (LastEnergy != m_Energy)
        m_DirtyBits |= PLAYER_DIRTY_BIT_ENERGY ;
}

//==============================================================================

// Sets energy directly
void player_object::SetEnergy(f32 Amount)
{
    // Update?
    if (m_Energy != Amount)
    {
        m_Energy = Amount ;
        m_DirtyBits |= PLAYER_DIRTY_BIT_ENERGY ;
    }
}

//==============================================================================

f32 player_object::GetHeat( void )
{
    return m_Heat ;
}

//==============================================================================

void player_object::SetHeat( f32 Heat )
{
    // Cap
    if (Heat < 0)
        Heat = 0 ;
    else
    if (Heat > 1)
        Heat = 1 ;

    // Set?
    if (m_Heat != Heat)
    {
        // Record
        m_Heat = Heat ;

        // Hot?
        if (m_Heat > 0.5f)
            m_AttrBits |= object::ATTR_HOT ;    // Make hot!
        else
            m_AttrBits &= ~object::ATTR_HOT ;   // Make cold!
    }
}

//==============================================================================
//
// INVENTORY FUNCTIONS
//
//==============================================================================


// Sets up weapon and ammo - returns FALSE if weapon cannot be added
xbool player_object::AddWeapon(invent_type WeaponType)
{
    s32 i ;

    // Get info
    ASSERT(WeaponType >= INVENT_TYPE_WEAPON_START) ;
    ASSERT(WeaponType <= INVENT_TYPE_WEAPON_END) ;

    // Check to see if this player type can even carry it, or we already have it
    if (m_Loadout.Inventory.Counts[WeaponType] >= m_MoveInfo->INVENT_MAX[WeaponType])
        return FALSE ;

    // Is there a free weapon slot?
    for (i = 0 ; i < m_Loadout.NumWeapons ; i++)
    {
        // Free slot?
        if (m_Loadout.Weapons[i] == INVENT_TYPE_NONE)
        {
            // Put weapon in the slot
            m_Loadout.Weapons[i] = WeaponType ;

            // Update inventory
            m_Loadout.Inventory.Counts[WeaponType]++ ;

            // If not currently holding a weapon, switch to this new one!
            if (m_WeaponCurrentType == INVENT_TYPE_NONE)
                m_WeaponRequestedType = WeaponType ;

            m_DirtyBits |= PLAYER_DIRTY_BIT_LOADOUT ;

            // Success!
            return TRUE ;
        }
    }

    // Weapon cannot be added
    return FALSE ;
}

//==============================================================================

f32 player_object::GetWeaponAmmoCount( invent_type   WeaponType ) const
{
    // Get info
    ASSERT(WeaponType >= INVENT_TYPE_WEAPON_START) ;
    ASSERT(WeaponType <= INVENT_TYPE_WEAPON_END) ;
    weapon_info &Info = s_WeaponInfo[WeaponType] ;

    // Using energy?
    if (Info.AmmoType == INVENT_TYPE_NONE)
        return m_Energy ;
    else
        return (f32)GetInventCount(Info.AmmoType) ;
}

//==============================================================================

// Sets invent count
void player_object::SetInventCount(invent_type InventType, s32 Amount)
{
    // Must be within range
    ASSERT(InventType >= INVENT_TYPE_START) ;
    ASSERT(InventType <= INVENT_TYPE_END) ;

    // Only update if different
    if (m_Loadout.Inventory.Counts[InventType] != Amount)
    {
        m_Loadout.Inventory.Counts[InventType] = Amount ;
        m_DirtyBits |= PLAYER_DIRTY_BIT_LOADOUT ;
    }
}

//==============================================================================

s32 player_object::GetInventCount( invent_type   InventType ) const
{
    // Get info
    ASSERT(InventType >= INVENT_TYPE_START) ;
    ASSERT(InventType <= INVENT_TYPE_END) ;

    return m_Loadout.Inventory.Counts[InventType] ;
}

//==============================================================================

// Takes into account the ammo pack
s32 player_object::GetInventMaxCount( invent_type   InventType ) const
{
    // Lookup default max count
    ASSERT(m_MoveInfo) ;
    ASSERT(InventType >= INVENT_TYPE_START) ;
    ASSERT(InventType <= INVENT_TYPE_END) ;
    s32 MaxCount = m_MoveInfo->INVENT_MAX[InventType] ;

    // If player has armor pack, check for increasing max
    if (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_AMMO] > 0)
    {
        // Lookup ammo pack max count
        for (s32 i = 0 ; i < PICKUP_AMMO_INFO_LIST_SIZE ; i++)
        {
            // Found item in ammo pack list?
            if (PickupAmmoInfoList[i].InventType == InventType)
                MaxCount += PickupAmmoInfoList[i].Max ;
        }
    }

    // Done
    return MaxCount ;
}

//==============================================================================

xbool player_object::HasPack( void )
{
    for( s32 i=INVENT_TYPE_PACK_START ; i<=INVENT_TYPE_PACK_END ; i++ )
    {
        if( m_Loadout.Inventory.Counts[i] > 0 )
            return TRUE;
    }
    return FALSE;
}

//==============================================================================

xbool player_object::HasGrenade( void )
{
    // NOTE: Mine is also considered a grenade so that the player cannot carry grenades and mines!
    for( s32 i=INVENT_TYPE_GRENADE_BASIC ; i <= INVENT_TYPE_MINE ; i++ )
    {
        if( m_Loadout.Inventory.Counts[i] > 0 )
            return TRUE;
    }
    return FALSE;
}

//==============================================================================

// Increases invent count - FALSE is returned if invent is at max value and you are trying to increase it!
xbool player_object::AddInvent(invent_type InventType, s32 Amount)
{
    s32 i ;

    // If unlimited ammo cheat and decreasing ammo, and it's bloody ammo, then leave it!
    if (    (g_bUnlimitedAmmo)
         && (Amount < 0)
         && (InventType >= INVENT_TYPE_WEAPON_AMMO_START) 
         && (InventType <= INVENT_TYPE_WEAPON_AMMO_END) )
        return TRUE ;

    // Get info
    ASSERT(InventType >= INVENT_TYPE_START) ;
    ASSERT(InventType <= INVENT_TYPE_END) ;

    // Check for an ammo pack
    xbool bHasAmmoPack = (GetInventCount(INVENT_TYPE_ARMOR_PACK_AMMO) > 0) ;

    // Setup is weapon flag
    // NOTE: Mine is also considered a grenade so that the player cannot carry grenades and mines!
    xbool bIsWeapon  = (InventType >= INVENT_TYPE_WEAPON_START)  && (InventType <= INVENT_TYPE_WEAPON_END) ;
    xbool bIsPack    = (InventType >= INVENT_TYPE_PACK_START)    && (InventType <= INVENT_TYPE_PACK_END);
    xbool bIsGrenade = (InventType >= INVENT_TYPE_GRENADE_BASIC) && (InventType <= INVENT_TYPE_MINE);

    // Only bail on duplicates if adding, not subtracting
    if( Amount >= 0 )
    {
        // Exit if player already has a pack
        if( bIsPack && HasPack() )
            return FALSE;

        // Exit if player is carrying a different type of grenade
        if( bIsGrenade && HasGrenade() && (GetInventCount(InventType) == 0) )
            return FALSE;
    }

    // Weapons are special since the weapon ammo is also added..
    if ((bIsWeapon) && (Amount > 0))
        return AddWeapon(InventType) ;

    // Get current count & max count
    s32 Count    = GetInventCount(InventType) ;
    s32 MaxCount = GetInventMaxCount(InventType) ; // takes into account ammo pack

    // Keep last count for dirty bit setting
    s32 LastCount = Count ;

    // At max value already?
    if ((Amount > 0) && (Count == MaxCount))
        return FALSE ;

    // Modify count
    Count += Amount ;

    // Cap
    if (Count < 0)
        Count = 0 ;
    else
        if( Count > MaxCount )
            Count = MaxCount;

    // Invent changed?
    if (LastCount != Count)
    {
        // Set new count
        SetInventCount(InventType, Count) ;

        // Check if we had the ammo pack and just lost it
        if( (bHasAmmoPack) && (GetInventCount(INVENT_TYPE_ARMOR_PACK_AMMO) == 0) )
        {
            // Clamp all counts
            for( i=INVENT_TYPE_START ; i<=INVENT_TYPE_END ; i++ )
            {
                MaxCount = GetInventMaxCount((invent_type)i) ;
                if (GetInventCount((invent_type)i) > MaxCount)
                    SetInventCount((invent_type)i, MaxCount) ;
            }
        }

        // If removing a weapon, remove it from the loadout slot
        if ((bIsWeapon) && (Count == 0))
        {
            // Search all slots
            for (i = 0 ; i < m_Loadout.NumWeapons ; i++)
            {
                // In this slot?
                if (m_Loadout.Weapons[i] == InventType)
                    m_Loadout.Weapons[i] = INVENT_TYPE_NONE ;   // Remove
            }
        }

        // Flag it's dirty
        m_DirtyBits |= PLAYER_DIRTY_BIT_LOADOUT ;
    }

    // Okay
    return TRUE ;
}


//==============================================================================

const player_object::loadout& player_object::GetLoadout( void ) const
{
    return m_Loadout;
}

//==============================================================================

const player_object::loadout& player_object::GetInventoryLoadout( void ) const
{
    return m_InventoryLoadout;
}

//==============================================================================

void player_object::SetInventoryLoadout( const loadout& Loadout )
{
    m_InventoryLoadout        = Loadout;

    if( !tgl.ServerPresent )
        m_InventoryLoadoutChanged = TRUE ;

    // Make sure loadout is valid!
    ASSERT(Loadout.NumWeapons) ;
}

//==============================================================================

void player_object::InventoryReload( xbool AtDeployedStation )
{
    s32         NewSlot = 0; // Start with first weapon in loadout
    s32         i;
    invent_type InventType;

    // Should only be called on the server!
    // Clients get their loadouts from OnAcceptUpdate
    ASSERT(tgl.pServer) ;

    // Fizzle out any attached charge
    DetachSatchelCharge(TRUE) ;

    // Keep a copy of the current invent pack counts incase we need to restore them
    // due to the player obtaining a remove invent!
    s32 PackInventCount[INVENT_TYPE_PACK_TOTAL] ;
    s32 PackCurrentCount[INVENT_TYPE_PACK_TOTAL] ;
    for (i = 0 ; i < INVENT_TYPE_PACK_TOTAL ; i++)
    {
        PackInventCount[i]  = m_Loadout.Inventory.Counts[INVENT_TYPE_PACK_START + i] ;
        PackCurrentCount[i] = GetInventCount((invent_type)(INVENT_TYPE_PACK_START+i));
    }

    // Start with current inventory loadout
    m_Loadout = m_InventoryLoadout ;

    // Special case
    if (AtDeployedStation)
    {
        // Get player move info so we can check inventory types
        const move_info& MoveInfo = GetMoveInfo(m_CharacterType, m_ArmorType) ;

        // Keep same armor
        m_Loadout.Armor = m_ArmorType ;

        // Copy valid weapons
        m_Loadout.NumWeapons = 0 ;
        for (i = 0 ; i < m_InventoryLoadout.NumWeapons ; i++)
        {
            // Get weapon
            invent_type  WeaponType = m_InventoryLoadout.Weapons[i] ;
            ASSERT(WeaponType >= INVENT_TYPE_WEAPON_START) ;
            ASSERT(WeaponType <= INVENT_TYPE_WEAPON_END) ;

            // Get weapon info
            const weapon_info &WeaponInfo = GetWeaponInfo(WeaponType) ;
            ASSERT(WeaponInfo.AmmoType >= INVENT_TYPE_START) ;
            ASSERT(WeaponInfo.AmmoType <= INVENT_TYPE_END) ;

            // Add to list ONLY if player can carry it
            if ( (MoveInfo.INVENT_MAX[WeaponType]) && (m_Loadout.NumWeapons < (3+m_ArmorType)) )
                m_Loadout.Weapons[m_Loadout.NumWeapons++] = WeaponType ;
            else
            {
                // Clear weapon and ammo from inventory
                m_Loadout.Inventory.Counts[WeaponType] = 0 ;
                m_Loadout.Inventory.Counts[WeaponInfo.AmmoType] = 0 ;
            }
        }

        // Copy valid inventory list
        for (i = 0 ; i < INVENT_TYPE_TOTAL ; i++)
            m_Loadout.Inventory.Counts[i] = MIN(MoveInfo.INVENT_MAX[i], m_InventoryLoadout.Inventory.Counts[i]) ;

        // SPECIAL CASE - Has player just obtained another remove invent station?
        if (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION])
        {
            // Do not let the player get it! - Restore original packs
            for (i = 0 ; i < INVENT_TYPE_PACK_TOTAL ; i++)
                m_Loadout.Inventory.Counts[INVENT_TYPE_PACK_START + i] = PackInventCount[i] ;
        }

        // Validate definition
        for (i = INVENT_TYPE_START ; i <= INVENT_TYPE_END ; i++)
        {
            // Convert to type for easier debugging
            InventType = (invent_type)i ;

            // Make sure item can be carried by this type
            ASSERTS(m_Loadout.Inventory.Counts[i] <= MoveInfo.INVENT_MAX[i], "this armor cannot carry this item!") ;
        }
    }

    // SPECIAL CASES - Playing DM or HUNTER, do not allow following type, just leave current pack:
    //                 Remote Turret
    //                 Remote Sensors
    //                 AA Barrel
    //                 Mortar Barrel
    //                 Missile Barrel
    //                 Plasma Barrel
    game_type GameType = GameMgr.GetGameType();
    if( ((GameType == GAME_DM) || (GameType == GAME_HUNTER)) &&
        ((m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR ] > 0) ||
         (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR  ] > 0) ||
         (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_AA     ] > 0) ||
         (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR ] > 0) ||
         (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE] > 0) ||
         (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA ] > 0)) )
    {
        // Do not let the player get it! - Restore original packs
        for (i = 0 ; i < INVENT_TYPE_PACK_TOTAL ; i++)
            m_Loadout.Inventory.Counts[INVENT_TYPE_PACK_START + i] = PackCurrentCount[i] ;
    }

    // Make sure weapon inventory counts are setup correctly
    // (looks like the front end only sets up the weapon slots, not the invent counts)
    
    // Clear weapon counts
    for (i = INVENT_TYPE_WEAPON_START ; i <= INVENT_TYPE_WEAPON_END ; i++)
        m_Loadout.Inventory.Counts[i] = 0 ;

    // Set weapon counts from weapon slots
    for (i = 0 ; i < m_InventoryLoadout.NumWeapons ; i++)
    {
        // Get weapon
        invent_type  WeaponType = m_InventoryLoadout.Weapons[i] ;
        ASSERT(WeaponType >= INVENT_TYPE_WEAPON_START) ;
        ASSERT(WeaponType <= INVENT_TYPE_WEAPON_END) ;

        if (WeaponType != INVENT_TYPE_NONE)
            m_Loadout.Inventory.Counts[WeaponType] = 1 ;
    }

    // Dirty thoughts lead to dirty bits.
    m_DirtyBits |= PLAYER_DIRTY_BIT_LOADOUT;

    // Look for current weapon
    for( i=0 ; i<m_Loadout.NumWeapons ; i++ )
    {
        if( m_Loadout.Weapons[i] == m_WeaponCurrentType )
        {
            NewSlot = i;
            break ;
        }
    }
    
    // Select new weapon
    m_WeaponCurrentSlot   = NewSlot;
    m_WeaponRequestedType = m_Loadout.Weapons[NewSlot];
    m_DirtyBits |= PLAYER_DIRTY_BIT_WEAPON_TYPE ;

    // Switch Armor type (keep same character and skin)?
    if (m_ArmorType != m_Loadout.Armor)
        SetCharacter( m_CharacterType, m_Loadout.Armor, m_SkinType, m_VoiceType ) ;

    // If player has armor pack, increase the ammo counts!
    if (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_AMMO])
    {
        // Check all invent
        for (i = INVENT_TYPE_START ; i <= INVENT_TYPE_END ; i++)
        {
            // Convert to type for easier debugging
            InventType = (invent_type)i ;

            // If player is carrying this item, max it out!
            if (GetInventCount(InventType))
                SetInventCount(InventType, GetInventMaxCount(InventType)) ;
        }
    }

    //
    // Remote messages.
    //

    if( (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR] > 0) &&
        (GameMgr.GetGameType() != GAME_CAMPAIGN) )
    {
        s32 Current, Maximum;
        GameMgr.GetDeployStats( object::TYPE_TURRET_CLAMP, 
                                m_TeamBits, Current, Maximum );
        MsgMgr.Message( MSG_N_OF_M_TURRETS_DEPLOYED, 
                        m_ObjectID.Slot, 
                        ARG_N, Current,
                        ARG_M, Maximum );
    }

    if( (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION] > 0) &&
        (GameMgr.GetGameType() != GAME_CAMPAIGN) )
    {
        s32 Current, Maximum;
        GameMgr.GetDeployStats( object::TYPE_STATION_DEPLOYED, 
                                m_TeamBits, Current, Maximum );
        MsgMgr.Message( MSG_N_OF_M_STATIONS_DEPLOYED, 
                        m_ObjectID.Slot, 
                        ARG_N, Current,
                        ARG_M, Maximum );
    }

    if( (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR] > 0) &&
        (GameMgr.GetGameType() != GAME_CAMPAIGN) )
    {
        s32 Current, Maximum;
        GameMgr.GetDeployStats( object::TYPE_SENSOR_REMOTE, 
                                m_TeamBits, Current, Maximum );
        MsgMgr.Message( MSG_N_OF_M_SENSORS_DEPLOYED, 
                        m_ObjectID.Slot, 
                        ARG_N, Current,
                        ARG_M, Maximum );
    }
}

//==============================================================================

void player_object::InventoryReset( void )
{
    // Setup default loadout
    default_loadouts::GetLoadout(m_CharacterType, (default_loadouts::type)m_DefaultLoadoutType, m_Loadout) ;

    // Switch to correct armor type (keep same character and skin)?
    if( m_ArmorType != m_Loadout.Armor )
        SetCharacter( m_CharacterType, m_Loadout.Armor, m_SkinType, m_VoiceType ) ;

    // Flag dirty
    m_DirtyBits |= PLAYER_DIRTY_BIT_LOADOUT;

    // Select new weapon
    m_WeaponCurrentSlot   = 0 ;
    m_WeaponRequestedType = m_Loadout.Weapons[0];
    m_DirtyBits |= PLAYER_DIRTY_BIT_WEAPON_TYPE ;



//#if defined sbroumley || jnagle
#if 0
    // Setup other items
    m_Loadout.Inventory.Counts[INVENT_TYPE_MINE]                 = 5 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_BASIC]        = 5 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_BELT_GEAR_BEACON]     = 3 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_BELT_GEAR_HEALTH_KIT] = 2 ;
    
    // Test packs
    m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_AMMO]            = 1 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_ENERGY]          = 1 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_REPAIR]          = 1 ;
    //m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_SENSOR_JAMMER]   = 1 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_SHIELD]          = 1 ;

    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_AA]      = 1 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_ELF]     = 1 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR]  = 1 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE]  = 1 ;
    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA]  = 1 ;

    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION] = 1 ;
    
    //m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_MOTION_SENSOR]  = 1 ;
    
    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR]   = 1 ;

    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR]  = 1 ;
    //m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_OUTDOOR] = 1 ;

    m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE] = 1 ;
#endif
}

//==============================================================================

s32 player_object::GetWeaponCurrentSlot( void ) const
{
    return m_WeaponCurrentSlot;
}

//==============================================================================

const player_object::invent_type player_object::GetWeaponCurrentType( void ) const
{
    vehicle* pVehicle = IsVehiclePilot();

    if( pVehicle )
    {
        if( pVehicle->GetType() == TYPE_VEHICLE_SHRIKE )
            return( INVENT_TYPE_WEAPON_SHOCKLANCE );
        else
            return( INVENT_TYPE_NONE );
    }

    return( m_WeaponCurrentType );
}

//==============================================================================

s32 player_object::GetNumWeaponsHeld( void ) const
{
    s32 Num = 0;

    for( s32 i=0; i<m_Loadout.NumWeapons; i++ )
    {
        if( m_Loadout.Weapons[i] != player_object::INVENT_TYPE_NONE )
        {
            Num++;
        }
    }

    return( Num );
}

//==============================================================================

xbool player_object::IsItemHeld( invent_type ItemType ) const
{
    if( m_Loadout.Inventory.Counts[ ItemType ] > 0 )
    {
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}


//==============================================================================
// CHANGE TEAM FUNCTIONS
//==============================================================================

// Store request of team change - it will make it's way to the server...
void player_object::RequestChangeTeam( void )
{
	// Store up for later ready for sending to server (that's if we're not already a server!)
	m_ChangeTeam     = TRUE ;		// Flags the player wants to change teams
}

//==============================================================================

// Changes player team (only call from server!)
void player_object::ChangeTeam( u32 TeamBits )
{
	ASSERT(tgl.pServer) ;

    // Missiles and turrets and such can let off for a bit.
    ObjMgr.UnbindTargetID( m_ObjectID );

	// Die and create a corpse where the player used to be.
    Player_SetupState( PLAYER_STATE_DEAD );
	CreateCorpse();

	// Change team
	m_TeamBits = TeamBits;

	// Finally, respawn in a new location
	// (respawn will take care of sending the new team bits to all clients)
	Respawn();
}

//==============================================================================


//==============================================================================
// COLLISION FUNCTIONS
//==============================================================================

// Creates and sets up a pickup
pickup* player_object::CreatePickup( const vector3&        Pos,
                                     const vector3&        Vel,
                                           pickup::kind    Kind,
                                           pickup::flags   Flags,
                                           f32             Time )
{
    // Should only happen on the server
    ASSERT(tgl.ServerPresent) ;

    // Create a pickup object
    pickup* pPickup = (pickup*)ObjMgr.CreateObject( object::TYPE_PICKUP );
    ASSERT(pPickup) ;

    // Initialize the pickup
    pPickup->Initialize( Pos, Vel, Kind, Flags, Time ) ;

    // Creating an ammo pack pickup?
    if (Kind == pickup::KIND_ARMOR_PACK_AMMO)
    {
        // Store inventory counts in pickup
        for (s32 i = 0  ; i < PICKUP_AMMO_INFO_LIST_SIZE ; i++)
        {
            // Convert index to inventory object
            invent_type InventType = PickupAmmoInfoList[i].InventType ;

            // Lookup count info
            s32 MaxCount = m_MoveInfo->INVENT_MAX[InventType] ;
            s32 Count    = GetInventCount(InventType) ;

            // Making use of ammo pack?
            if (Count > MaxCount)
                Count -= MaxCount ; // Calc amount carried in the ammo pack itself
            else
                Count = 0 ;

            // Set ammo pack total
            pPickup->SetDataItem(i, (u8)Count) ;
        }
    }

    // Creating an ammo box pickup?
    if (Kind == pickup::KIND_AMMO_BOX)
    {
        // Store inventory counts in pickup
        for (s32 i = 0  ; i < PICKUP_AMMO_INFO_LIST_SIZE ; i++)
        {
            // Convert index to inventory object
            invent_type InventType = PickupAmmoInfoList[i].InventType ;

            // Lookup count info
            s32 Count    = GetInventCount(InventType) ;

            // Set ammo pack total
            pPickup->SetDataItem(i, (u8)Count) ;
        }
    }

    // Finally, add the pickup to the world
    ObjMgr.AddObject( pPickup, obj_mgr::AVAILABLE_SERVER_SLOT );

    // Return the created pickup
    return pPickup ;
}

//==============================================================================

// Called when player hits a pickup - return TRUE if pickup is taken, else FALSE
xbool player_object::HitPickup(pickup *pPickup)
{
	// Can't take if dead
	if (m_Health <= 0)
		return FALSE ;

    // Default to not taken
    xbool Taken = FALSE ;

    // Make have hit a pickup
    ASSERT(pPickup) ;
    ASSERT(pPickup->GetType() == object::TYPE_PICKUP) ;

    // ask the pickup if it's retrievable
    if ( pPickup->IsPickuppable() != TRUE )
        return FALSE;

    // Should only be called on server
    ASSERT(tgl.ServerPresent) ;

    // Trigger event
    pGameLogic->PickupTouched( pPickup->GetObjectID(), m_ObjectID );

    // Get pickup info
    const pickup::info& Info = pPickup->GetInfo() ;

    // Do what?
    switch( pPickup->GetKind() )
    {
        //======================================================================
        // Health kit/patch - just increases health
        //======================================================================
        case pickup::KIND_HEALTH_KIT:
        case pickup::KIND_HEALTH_PATCH:

            // Only take if health is less than 100
            if (m_Health < 100)
            {
                // Add to increase total
                m_HealthIncrease += Info.InventCount ;
                Taken = TRUE ;
            }
            break ;

        //======================================================================
        // Armor pack actually contains inventory counts for ammo
        //======================================================================
        case pickup::KIND_ARMOR_PACK_AMMO:

            // Try take the pack
            Taken = AddInvent(INVENT_TYPE_ARMOR_PACK_AMMO, 1) ;

            // If ammo pack was taken, also grab the ammo pack contents
            if (Taken)
            {
                // Add inventory counts in pickup
                for (s32 i = 0  ; i < PICKUP_AMMO_INFO_LIST_SIZE ; i++)
                {
                    // Convert index to inventory object
                    invent_type InventType = PickupAmmoInfoList[i].InventType ;
                    s32         Count      = (s32)pPickup->GetDataItem(i) ;
                    AddInvent((invent_type)InventType, Count) ;
                }
            }

            break ;

        //======================================================================
        // Ammo box actually contains inventory counts for ammo
        //======================================================================
        case pickup::KIND_AMMO_BOX:

            // Always take
            Taken = TRUE ;

            // If ammo box was taken, also grab the ammo pack contents
            if (Taken)
            {
                // Add inventory counts in pickup
                for (s32 i = 0  ; i < PICKUP_AMMO_INFO_LIST_SIZE ; i++)
                {
                    // Convert index to inventory object
                    invent_type InventType = PickupAmmoInfoList[i].InventType ;
                    s32         Count      = (s32)pPickup->GetDataItem(i) ;
                    AddInvent((invent_type)InventType, Count) ;
                }
            }
            break ;

        //======================================================================
        // Regular pickups - just add to inventory
        //======================================================================
        default:
            Taken = AddInvent((invent_type)Info.InventType, Info.InventCount) ;
            break ;
    }

    // Was the pickup successfully taken?
    if (Taken)
    {
        // Pickup message.
        MsgMgr.Message( MSG_PICKUP_TAKEN, m_ObjectID.Slot, ARG_PICKUP, pPickup->GetKind() );

        /* 
        ** DO NOT DO THIS UNLESS WE SUPPORT MULTIPLE POP_UP MESSAGES
        **
        // Remote counts message if applicable.
        s32 Current, Maximum;
        switch( pPickup->GetKind() )
        {
        case pickup::KIND_DEPLOY_PACK_TURRET_INDOOR:
            GameMgr.GetDeployStats( object::TYPE_TURRET_CLAMP, 
                                    m_TeamBits, Current, Maximum );
            MsgMgr.Message( MSG_N_OF_M_TURRETS_DEPLOYED, 
                            m_ObjectID.Slot, 
                            ARG_N, Current,
                            ARG_M, Maximum );
            break;

        case pickup::KIND_DEPLOY_PACK_INVENT_STATION:
            GameMgr.GetDeployStats( object::TYPE_STATION_DEPLOYED, 
                                    m_TeamBits, Current, Maximum );
            MsgMgr.Message( MSG_N_OF_M_STATIONS_DEPLOYED, 
                            m_ObjectID.Slot, 
                            ARG_N, Current,
                            ARG_M, Maximum );
            break;

        case pickup::KIND_DEPLOY_PACK_PULSE_SENSOR:
            GameMgr.GetDeployStats( object::TYPE_SENSOR_REMOTE, 
                                    m_TeamBits, Current, Maximum );
            MsgMgr.Message( MSG_N_OF_M_SENSORS_DEPLOYED, 
                            m_ObjectID.Slot, 
                            ARG_N, Current,
                            ARG_M, Maximum );
            break;

        default:
            // No message.
            break;
        }
        */

        // Delete the pickup from the world
        pPickup->SetState( pickup::STATE_DEAD );

        // Do fx
        FlashScreen(0.25f, xcolor(0,0,255,128)) ;
        switch( pPickup->GetKind() )
        {
        case pickup::KIND_HEALTH_PATCH:
        case pickup::KIND_HEALTH_KIT:
            PlaySound( SFX_MISC_HEALTH_PATCH );
            break;

        default:
            PlaySound( SFX_MISC_PICKUP04 );
            break;
        }        
    }
    
    // Return result
    return Taken ;
}

//==============================================================================

pain::type player_object::GetWeaponPainType( invent_type WeaponType ) const
{
    switch ( WeaponType )
    {
    case INVENT_TYPE_WEAPON_DISK_LAUNCHER:
        return pain::WEAPON_DISC;

    case INVENT_TYPE_WEAPON_PLASMA_GUN:
        return pain::WEAPON_PLASMA;
        
    case INVENT_TYPE_WEAPON_SNIPER_RIFLE:
        return pain::WEAPON_LASER;
        
    case INVENT_TYPE_WEAPON_MORTAR_GUN:
        return pain::WEAPON_MORTAR;
        
    case INVENT_TYPE_WEAPON_GRENADE_LAUNCHER:
        return pain::WEAPON_GRENADE;
        
    case INVENT_TYPE_WEAPON_CHAIN_GUN:
        return pain::WEAPON_CHAINGUN;
        
    case INVENT_TYPE_WEAPON_BLASTER:
        return pain::WEAPON_BLASTER;

    case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:
        return pain::WEAPON_MISSILE;
        
    case INVENT_TYPE_WEAPON_ELF_GUN:
        return pain::WEAPON_ELF;
        
    case INVENT_TYPE_GRENADE_BASIC:
        return pain::WEAPON_GRENADE_HAND;
        
    case INVENT_TYPE_GRENADE_FLASH:
        return pain::WEAPON_GRENADE_FLASH;
        
    case INVENT_TYPE_GRENADE_CONCUSSION:
        return pain::WEAPON_GRENADE_CONCUSSION;
        
    default:
        ASSERT( FALSE );
        return pain::MISC_FLIP_FLOP_FORCE;
    }
}


//==============================================================================
// VOTE FUNCTIONS
//==============================================================================

// Functions that can be called from the server/client ui player input dialog
// (if on a client - it will be sent to the server)
// Votes are processed on the server only - see player_object::ApplyMove

void player_object::VoteChangeMap( s32 MapIndex )
{
    // If these asserts fail, you need to update the 
    // "player_object::move::read/write" functions to cater for more bits
    ASSERT(MapIndex >= 0) ;
    ASSERT(MapIndex <= 127) ;

    // Setup change map vote info
    m_VoteType = VOTE_TYPE_CHANGE_MAP ;
    m_VoteData = (s8)MapIndex ;
}

//==============================================================================

void player_object::VoteKickPlayer( s32 PlayerIndex )
{
    // If these asserts fail, you need to update the 
    // "player_object::move::read/write" functions to cater for more bits
    ASSERT(PlayerIndex >= 0) ;
    ASSERT(PlayerIndex <= 31) ;

    // Setup kick player vote info
    m_VoteType = VOTE_TYPE_KICK_PLAYER ;
    m_VoteData = (s8)PlayerIndex ;
}

//==============================================================================

void player_object::VoteYes( void )
{
    // Setup cast yes info
    m_VoteType = VOTE_TYPE_YES ;
    m_VoteData = 0 ;
}

//==============================================================================

void player_object::VoteNo( void )
{
    // Setup cast no info
    m_VoteType = VOTE_TYPE_NO ;
    m_VoteData = 0 ;
}

//==============================================================================

// Query functions that can be called by the server/client ui dialogs to see 
// if start/cast voting can be enabled. NOTE: They are only valid for server/client
// controlled players and not ghost players.

xbool player_object::GetVoteCanStart( void )
{
    // You shouldn't call this for ghost players as it's invalid
    ASSERT(m_ProcessInput) ;

    return m_VoteCanStart ;
}

//==============================================================================

xbool player_object::GetVoteCanCast( void )
{
    // You shouldn't call this for ghost players as it's invalid
    ASSERT(m_ProcessInput) ;

    return m_VoteCanCast ;
}

//==============================================================================

// Vote functions that can be called by the server game manager
// (the info will be sent to the client controlled players)

void player_object::SetVoteCanStart( xbool bCanStart )
{
    // This will get get sent to the controlling client
    if (m_VoteCanStart != bCanStart)
    {
        // I've ran out of dirty bits so this just makes sure it gets across
        // to client controlled players - see PlayerNet.cpp WriteDirtyBits
        m_DirtyBits |= PLAYER_DIRTY_BIT_ENERGY ;
        m_VoteCanStart = bCanStart ;
    }
} 

//==============================================================================

void player_object::SetVoteCanCast( xbool bCanCast  )
{
    // This will get get sent to the controlling client
    if (m_VoteCanCast != bCanCast)
    {
        // I've ran out of dirty bits so this just makes sure it gets across
        // to client controlled players - see PlayerNet.cpp WriteDirtyBits
        m_DirtyBits |= PLAYER_DIRTY_BIT_ENERGY ;
        m_VoteCanCast  = bCanCast ;
    }
}

//==============================================================================

