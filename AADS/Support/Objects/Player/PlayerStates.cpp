//==============================================================================
//
//  PlayerStates.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "Entropy.hpp"
#include "NetLib/bitstream.hpp"
#include "AADS/Globals.hpp"
#include "AADS/GameServer.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "Objects/Projectiles/Grenade.hpp"
#include "Objects/Projectiles/Mine.hpp"
#include "Objects/Projectiles/Bubble.hpp"
#include "Objects/Pickups/Pickups.hpp"
#include "Objects/Vehicles/GndEffect.hpp"
#include "Objects/Projectiles/SatchelCharge.hpp"
#include "Shape/SelectionList.hpp"
#include "GameMgr/GameMgr.hpp"
#include "HUD/hud_Manager.hpp"


//==============================================================================
// DEFINES
//==============================================================================


#define PLAY_RUN_SPEED				0.1f


//==============================================================================
// USERFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::Player_AdvanceState(f32 DeltaTime)
{
	// Update state time
    m_PlayerStateTime += DeltaTime ;

	// Call advance function
	switch(m_PlayerState)
	{
        case PLAYER_STATE_CONNECTING:		Player_Advance_CONNECTING		(DeltaTime) ;    break ;
        case PLAYER_STATE_WAIT_FOR_GAME:	Player_Advance_WAIT_FOR_GAME    (DeltaTime) ;    break ;
        
        case PLAYER_STATE_TAUNT:		    Player_Advance_TAUNT			(DeltaTime) ;    break ;
        case PLAYER_STATE_AT_INVENT:	    Player_Advance_AT_INVENT		(DeltaTime) ;    break ;
		case PLAYER_STATE_DEAD:		        Player_Advance_DEAD				(DeltaTime) ;    break ;
		                                    
        case PLAYER_STATE_IDLE:		        Player_Advance_IDLE				(DeltaTime) ;    break ;
		case PLAYER_STATE_RUN:			    Player_Advance_RUN				(DeltaTime) ;    break ;
		case PLAYER_STATE_JUMP:		        Player_Advance_JUMP				(DeltaTime) ;    break ;
		case PLAYER_STATE_LAND:		        Player_Advance_LAND				(DeltaTime) ;    break ;
		case PLAYER_STATE_JET_THRUST:       Player_Advance_JET_THRUST       (DeltaTime) ;    break ;
		case PLAYER_STATE_JET_FALL:         Player_Advance_JET_FALL         (DeltaTime) ;    break ;
		case PLAYER_STATE_FALL:             Player_Advance_FALL             (DeltaTime) ;    break ;
                                            
        case PLAYER_STATE_VEHICLE:          Player_Advance_VEHICLE          (DeltaTime) ;    break ;
	}
}

//==============================================================================

const char* player_object::Player_GetStateString( player_state State )
{
#define CASE_STATE_STRING(__state__)    case __state__ :   return #__state__ ;
    switch(State)
    {
        CASE_STATE_STRING(PLAYER_STATE_CONNECTING)
        CASE_STATE_STRING(PLAYER_STATE_WAIT_FOR_GAME)
        CASE_STATE_STRING(PLAYER_STATE_TAUNT)
        CASE_STATE_STRING(PLAYER_STATE_AT_INVENT)
        CASE_STATE_STRING(PLAYER_STATE_DEAD)
        CASE_STATE_STRING(PLAYER_STATE_IDLE)
        CASE_STATE_STRING(PLAYER_STATE_RUN)
        CASE_STATE_STRING(PLAYER_STATE_JUMP)
        CASE_STATE_STRING(PLAYER_STATE_LAND)
        CASE_STATE_STRING(PLAYER_STATE_JET_THRUST)
        CASE_STATE_STRING(PLAYER_STATE_JET_FALL)
        CASE_STATE_STRING(PLAYER_STATE_FALL)
        CASE_STATE_STRING(PLAYER_STATE_VEHICLE)
    }

    return "add me to this function!" ;
}

//==============================================================================

// Sets up a new state
void player_object::Player_SetupState(player_state State, xbool SkipIfAlreadyInState /*=TRUE*/ )
{
    // Do not allow blending from death state anim!
    // (the player screen size logic will auto-turn this back on!)
    if (m_PlayerState == PLAYER_STATE_DEAD)
        m_PlayerInstance.SetEnableAnimBlending(FALSE) ;

    // State changed?
    if ((m_PlayerState != State) || (!SkipIfAlreadyInState))
    {
        m_DirtyBits |= PLAYER_DIRTY_BIT_PLAYER_STATE ;

		// Call exit state functions of current state
        switch(m_PlayerState)
        {
            case PLAYER_STATE_CONNECTING:		                            break ;
            case PLAYER_STATE_WAIT_FOR_GAME: Player_Exit_WAIT_FOR_GAME() ;  break ;
            
            case PLAYER_STATE_IDLE:		                                    break ;
            case PLAYER_STATE_RUN:			                                break ;
            case PLAYER_STATE_JUMP:		                                    break ;
            case PLAYER_STATE_LAND:		                                    break ;
            case PLAYER_STATE_JET_THRUST:                                   break ;
            case PLAYER_STATE_JET_FALL:                                     break ;
            case PLAYER_STATE_FALL:                                         break ;

            case PLAYER_STATE_VEHICLE:     Player_Exit_VEHICLE       () ;   break ;
            case PLAYER_STATE_DEAD:		                                    break ;
        }

		// Record new state now, before calling setup state, so that the setup state
        // function can put the player in another state if need be!
        // (eg. setup land - damages player, so setup death is needed)
        m_PlayerLastState = m_PlayerState ;
        m_PlayerState     = State ;

		// Default anim speed to normal playback
        m_PlayerInstance.GetCurrentAnimState().SetSpeed(1.0f) ;

        // Setup logic
        switch(State)
        {
            case PLAYER_STATE_CONNECTING:		Player_Setup_CONNECTING		() ;    break ;
            case PLAYER_STATE_WAIT_FOR_GAME:	Player_Setup_WAIT_FOR_GAME  () ;    break ;

            case PLAYER_STATE_TAUNT:	        Player_Setup_TAUNT			() ;    break ;
            case PLAYER_STATE_AT_INVENT:        Player_Setup_AT_INVENT		() ;    break ;
                                                
            case PLAYER_STATE_IDLE:		        Player_Setup_IDLE			() ;    break ;
            case PLAYER_STATE_RUN:			    Player_Setup_RUN			() ;    break ;
            case PLAYER_STATE_JUMP:		        Player_Setup_JUMP			() ;    break ;
            case PLAYER_STATE_LAND:		        Player_Setup_LAND			() ;    break ;
            case PLAYER_STATE_JET_THRUST:       Player_Setup_JET_THRUST     () ;    break ;
            case PLAYER_STATE_JET_FALL:         Player_Setup_JET_FALL		() ;    break ;
            case PLAYER_STATE_FALL:             Player_Setup_FALL		    () ;    break ;
                                                
            case PLAYER_STATE_VEHICLE:          Player_Setup_VEHICLE		() ;    break ;
            case PLAYER_STATE_DEAD:		        Player_Setup_DEAD			() ;    break ;
        }

		// Reset state time
		m_PlayerStateTime = 0.0f ;
    }
}

//==============================================================================

void player_object::Player_PlayHardLandSfx()
{
    // SB - This sound is taken care of in "player_object::Advance" now!
	// TO DO - Check surface type for snow etc
	// Play hard land effect sfx
	//s32 SoundType = -1 ;
	//switch(m_ArmorType)
	//{
		//case ARMOR_TYPE_LIGHT:	SoundType = SFX_ARMOR_LIGHT_LAND_HARD ; break ;
		//case ARMOR_TYPE_MEDIUM:	SoundType = SFX_ARMOR_LIGHT_LAND_HARD ; break ;
		//case ARMOR_TYPE_HEAVY:	SoundType = SFX_ARMOR_LIGHT_LAND_HARD ; break ;
	//}
	//audio_Play(SoundType, &m_WorldPos) ;

    if (m_ProcessInput)
    {
        input_Feedback(0.1f,0.75f,m_ControllerID);
    }
}

//==============================================================================

void player_object::Player_PlaySoftLandSfx()
{
    // SB - This sound is taken care of in "player_object::Advance" now!

	// TO DO - Check surface type for snow etc
	// Play soft land effect sfx
	//s32 SoundType = -1 ;
	//switch(m_ArmorType)
	//{
		//case ARMOR_TYPE_LIGHT:	SoundType = SFX_ARMOR_LIGHT_LAND_SOFT ; break ;
		//case ARMOR_TYPE_MEDIUM:	SoundType = SFX_ARMOR_LIGHT_LAND_SOFT ; break ;
		//case ARMOR_TYPE_HEAVY:	SoundType = SFX_ARMOR_LIGHT_LAND_SOFT ; break ;
	//}
	//audio_Play(SoundType, &m_WorldPos) ;
    if (m_ProcessInput)
    {
        input_Feedback(0.1f,0.25f,m_ControllerID);
    }
}

//==============================================================================


//==============================================================================
//
// STATE FUNCTIONS
//
//==============================================================================

#define PICK_NEW_MOVE_STATE_TIME    (4.0f / 30.0f)  // Time interval before choosing a new move state
#define CONTACT_TIME                0.25f           // Time before choosing a non-contact anim


// Given current movement info, selects the appropriate moving state
void player_object::Player_ChooseMovingState( f32 DeltaTime )
{
    // Jump?
    if (m_Jumping)
    {
        // Only play if about to jump
        if ((m_PlayerState != PLAYER_STATE_JUMP) || (m_ContactInfo.JumpSurface))
            Player_SetupState(PLAYER_STATE_JUMP, FALSE) ;
    }
    else
    {
        // Update anim timer
        if (m_PickNewMoveStateTime > 0)
            m_PickNewMoveStateTime -= DeltaTime ;

        // Setup new state?
        if (m_PickNewMoveStateTime <= 0)
        {
            m_PickNewMoveStateTime += PICK_NEW_MOVE_STATE_TIME ;

            // Nothing under the feet?
            if (m_LastContactTime >= CONTACT_TIME)
            {
                // Jet?
                if (m_Jetting)
                    Player_SetupState(PLAYER_STATE_JET_THRUST) ;
                else
                // Falling fast?
                if (m_Falling)
                    Player_SetupState(PLAYER_STATE_FALL) ;
                // Normal fall?
                else
                    Player_SetupState(PLAYER_STATE_JET_FALL) ;
            }
            else
            // Run/Idle?
            if (m_ContactInfo.RunSurface)
                Player_ChooseSurfaceState( DeltaTime ) ;
        }
    }
}

//==============================================================================

void player_object::Player_ChooseSurfaceState( f32 DeltaTime )
{
    (void)DeltaTime ;

    // Get XZ velocity
    vector3 Vel ;
    Vel.X = m_Vel.X ;
    Vel.Y = 0.0f ;
    Vel.Z = m_Vel.Z ;

    // Add in blended movement as velocity
    vector3 DrawPos = m_WorldPos + (m_BlendMove.Blend * m_BlendMove.DeltaPos) ;
    Vel.X += m_WorldPos.X - DrawPos.X ;
    Vel.Z += m_WorldPos.Z - DrawPos.Z ;

    // Run?
    if (Vel.Length() >= PLAY_RUN_SPEED)
	    Player_SetupState(PLAYER_STATE_RUN) ;
    else
	    Player_SetupState(PLAYER_STATE_IDLE) ;
}


//==============================================================================
// PLAYER_STATE_CONNECTING
//==============================================================================

void player_object::Player_Setup_CONNECTING()
{
}

void player_object::Player_Advance_CONNECTING(f32 DeltaTime)
{
    (void)DeltaTime ;

    // Simply wait for server to kick off player
}

//==============================================================================
// PLAYER_STATE_WAIT_FOR_GAME
//==============================================================================

void player_object::Player_Setup_WAIT_FOR_GAME()
{
    // Make sure things can hit this fine warrior!
    m_AttrBits |= (object::ATTR_SOLID_DYNAMIC | object::ATTR_REPAIRABLE | object::ATTR_ELFABLE | object::ATTR_SENSED) ;

    // Stop other players going through
    m_AttrBits &= ~object::ATTR_PERMEABLE ;

    // Disable all keys until they are released
    m_DisabledInput.Set() ;

    // Start the anim (with no blend!)
    m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_IDLE_GAME_START, 0) ;

    // Game manager will take player out of this state...
}

//==============================================================================

void player_object::Player_Advance_WAIT_FOR_GAME(f32 DeltaTime)
{
    (void)DeltaTime;
}

void player_object::Player_Exit_WAIT_FOR_GAME()
{
    // Start the spawn timer
    m_SpawnTime = SPAWN_TIME ;

    // Disarmed during spawn
    SetArmed( FALSE );
}


//==============================================================================
// PLAYER_STATE_TAUNT
//==============================================================================

void player_object::Player_Setup_TAUNT()
{
}

//==============================================================================

void player_object::Player_Advance_TAUNT(f32 DeltaTime)
{
    (void)DeltaTime;

    // Done?
    if (m_PlayerInstance.GetCurrentAnimState().GetAnimPlayed())
        Player_SetupState(PLAYER_STATE_IDLE) ;
}


//==============================================================================
// PLAYER_STATE_AT_INVENT
//==============================================================================

void player_object::Player_Setup_AT_INVENT( void )
{
    // Set anim
    m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_IDLE, 0.25f) ;

    // Stop movement
    m_Vel.Zero() ;
    f32 DeltaTime = m_Move.DeltaTime;
    m_Move.Clear() ;
    m_Move.DeltaTime = DeltaTime;

    // Causes attached objects to fizzle out
    m_NodeCollID ^= 1 ;
}

void player_object::Player_Advance_AT_INVENT( f32 DeltaTime )
{
    // Inventory object takes care of taking player out of this state...
    // (or the player may get killed!)
    (void)DeltaTime ;
}

//==============================================================================
// PLAYER_STATE_IDLE
//==============================================================================

void player_object::Player_Setup_IDLE()
{
    m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_IDLE, 0.25f) ;
}

void player_object::Player_Advance_IDLE(f32 DeltaTime)
{
    (void)DeltaTime;

    // Turn off idle anim when in 1st pseron view since the camera code adds the bob
    if ((m_ProcessInput) && (m_PlayerViewType == VIEW_TYPE_1ST_PERSON))
        m_PlayerInstance.GetCurrentAnimState().SetSpeed(0) ;
    else
        m_PlayerInstance.GetCurrentAnimState().SetSpeed(1) ;

    // Choose run or idle
    Player_ChooseSurfaceState( DeltaTime ) ;
}

//==============================================================================
// PLAYER_STATE_RUN
//==============================================================================

void player_object::Player_Setup_RUN()
{
}

void player_object::Player_Advance_RUN(f32 DeltaTime)
{
    (void)DeltaTime;

    // Get vel
    vector3 LocalVel = m_Vel ;

    // Add in blended movement as velocity
    vector3 DrawPos = m_WorldPos + (m_BlendMove.Blend * m_BlendMove.DeltaPos) ;
    LocalVel.X += m_WorldPos.X - DrawPos.X ;
    LocalVel.Z += m_WorldPos.Z - DrawPos.Z ;
    
    // Put into local space    
    LocalVel.RotateY(-m_Rot.Yaw) ;

    // Choose current frame and anim type
    f32 StartFrame, Speed, Dir  ;
    s32 AnimType ;

    // Use side movement?
    if (ABS(LocalVel.X) > ABS(LocalVel.Z))
    {
        AnimType   = ANIM_TYPE_CHARACTER_SIDE ;
        StartFrame = m_SideAnimFrame ;

        Speed = ABS(LocalVel.X) / m_MoveInfo->MAX_SIDE_SPEED ;
        if (LocalVel.X < 0)
            Dir = -1 ;
        else
            Dir = 1 ;
    }
    else
    {
        StartFrame = m_ForwardBackwardAnimFrame ;
        Dir        = 1 ;
        if (LocalVel.Z > 0)
        {
            Speed    = ABS(LocalVel.Z) / m_MoveInfo->MAX_FORWARD_SPEED ;
            AnimType = ANIM_TYPE_CHARACTER_FORWARD ;
        }
        else
        {
            Speed    = ABS(LocalVel.Z) / m_MoveInfo->MAX_BACKWARD_SPEED ;
            AnimType = ANIM_TYPE_CHARACTER_BACKWARD ;
        }
    }

    // Set frame and speed
    f32 AnimSpeed = MAX(0.25f, MIN(1, Speed+0.25f)) ;
    m_PlayerInstance.GetCurrentAnimState().SetSpeed(AnimSpeed*Dir) ;
    m_PlayerInstance.SetAnimByType(AnimType,    // Type
                                   0.2f,        // BlendTime
                                   StartFrame,  // StartFrame
                                   FALSE) ;     // ForceBlend
    
    // Choose run or idle
    Player_ChooseSurfaceState( DeltaTime ) ;
}

//==============================================================================
// PLAYER_STATE_JUMP
//==============================================================================

void player_object::Player_Setup_JUMP()
{
    // Flag jumping for client ghosts
    m_Jumping = TRUE ;

    // Already playing jump anim?
    if (m_PlayerInstance.GetCurrentAnimState().GetAnim()->GetType() == ANIM_TYPE_CHARACTER_JUMP_UP)
    {
        // Make sure first part of anim has played before restarting
        if (m_PlayerInstance.GetCurrentAnimState().GetFrame() < 5)
            return ;
    }

    // Ski-ing?
    m_PlayerInstance.GetCurrentAnimState().SetSpeed(1.0f) ;
	if (m_Vel.Length() >= s_CommonInfo.SKI_JUMP_SPEED)
    {
	    // Go to jump anim fast!
        m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_JUMP_UP,     // Type
                                       0.1f,                            // BlendTime
                                       0.25f,                           // StartFrame
                                       TRUE) ;                          // Force blend (incase same type is playing)
    }
    else
    {
        // Restart anim
        m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_JUMP_UP,     // Type
                                       0.1f,                            // BlendTime
                                       0.0f,                            // StartFrame
                                       TRUE) ;                          // Force blend (incase same type is playing)

    }
}

void player_object::Player_Advance_JUMP(f32 DeltaTime)
{
    (void)DeltaTime;

    // Goto fall?
    if (m_PlayerInstance.GetCurrentAnimState().GetAnimPlayed())
    {
        Player_SetupState(PLAYER_STATE_JET_FALL) ;
        m_Jumping = FALSE ;
    }

    // Wait for landing before choosing a new state
    //if (m_ContactInfo.RunSurface)
        //m_PickNewMoveStateTime = 0 ;
    //else
        //m_PickNewMoveStateTime = PICK_NEW_MOVE_STATE_TIME ;
}


//==============================================================================
// PLAYER_STATE_LAND
//==============================================================================

// Returns amount of land damage that will occur or 0 if none
f32 player_object::GetLandDamage()
{
    // Get speed of moving into ground
    f32 Speed = m_GroundImpactSpeed ;

    // Get amount of speed over land hard speed
    f32 DamageSpeed = Speed - m_MoveInfo->MAX_JUMP_SPEED ;
    
    // Do any damage?
    if (DamageSpeed > 0.0f)
        return (0.5f * DamageSpeed) ;
    else
        return 0 ;
}

void player_object::Player_Setup_LAND()
{
    // Play land anim
    m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_LAND, 0.2f) ;

	// Play hard land effect sfx
	Player_PlayHardLandSfx() ;

    // Do some damage on server only!
    if( tgl.pServer )
    {
        // TO DO - Add pain in impact

        f32 Damage = GetLandDamage() ;
        //AddHealth( -Damage ) ;
        //FlashScreen((Damage * s_CommonInfo.MAX_HURT_SCREEN_FLASH_TIME) / 100.0f, xcolor(255,0,0,(u8)((Damage * 255.0f) / 100.0f))) ;
        if (m_ProcessInput)
        {
            input_Feedback(0.1f,Damage,m_ControllerID);
        }
    }        
}

void player_object::Player_Advance_LAND(f32 DeltaTime)
{
    (void)DeltaTime;

    // Wait for recover time to complete before picking a new state
    if ((m_LandRecoverTime == 0) && (m_PlayerInstance.GetCurrentAnimState().GetAnimPlayed()))
        m_PickNewMoveStateTime = 0 ;
    else
        m_PickNewMoveStateTime = PICK_NEW_MOVE_STATE_TIME ;
}


//==============================================================================
// PLAYER_STATE_JET_THRUST
//==============================================================================

void player_object::Player_Setup_JET_THRUST()
{
	// Set anim
	m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_JET, 0.25f) ;
}

void player_object::Player_Advance_JET_THRUST(f32 DeltaTime)
{
    (void)DeltaTime;
}

//==============================================================================
// PLAYER_STATE_JET_FALL
//==============================================================================

void player_object::Player_Setup_JET_FALL()
{
	// Set anim
	m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_IDLE, 0.25f) ;
}

void player_object::Player_Advance_JET_FALL(f32 DeltaTime)
{
    (void)DeltaTime;
}


//==============================================================================
// PLAYER_STATE_FALL
//==============================================================================

void player_object::Player_Setup_FALL()
{
	// Set anim
	m_PlayerInstance.SetAnimByType(ANIM_TYPE_CHARACTER_FALL, 0.25f) ;

    // Flag falling for client ghosts
    m_Falling = TRUE ;
}

void player_object::Player_Advance_FALL(f32 DeltaTime)
{
    (void)DeltaTime;
}

//==============================================================================
// PLAYER_STATE_VEHICLE
//==============================================================================

void player_object::Player_Setup_VEHICLE()
{
    vehicle* pVehicle = IsAttachedToVehicle();

    if( pVehicle == NULL )
        // Clear rotation
        m_Rot.Zero();
    else
        // Player rotation is relative to rotation of vehicle
        m_Rot.Yaw -= pVehicle->GetRot().Yaw;
}

void player_object::Player_Exit_VEHICLE()
{
    // Get off vehicle
    DetachFromVehicle() ;

    // Give weapon back
    SetArmed(TRUE) ;
}

void player_object::Player_Advance_VEHICLE(f32 DeltaTime)
{
    (void)DeltaTime;

    // Clear movement
    m_Vel.Zero();

    // Get vehicle (although it may not have been created yet!)
    vehicle* pVehicle = IsAttachedToVehicle();
    if( pVehicle )
    {
        // Make sure vehicle vars are setup
        ASSERT( m_VehicleSeat != -1 );
        
        // Lookup seat info
        const vehicle::seat& Seat = pVehicle->GetSeat( m_VehicleSeat );
        
        // Give player weapon if this is one of those seats...
        SetArmed( Seat.CanLookAndShoot );

        // Get and set anim type
        s32 AnimType = Seat.PlayerAnimType;
        m_PlayerInstance.SetAnimByType( AnimType, 0 ); // no blending!

        // Animation special cases
        switch( AnimType )
        {
            // Set to correct lean frame (maps input from -1->1 to 0->1
            case ANIM_TYPE_CHARACTER_PILOT_GRAV_CYCLE:
                m_PlayerInstance.GetCurrentAnimState().SetSpeed( 0 );
                m_PlayerInstance.GetCurrentAnimState().SetFrameParametric( 0.5f );
                break;
        }

        if( m_ProcessInput )
        {
            input_Feedback( 0.1f, 0.3f, m_ControllerID );
        }
    }

    // Jump out of vehicle?
    if( (tgl.pServer) && (m_Move.JumpKey) )
    {
        static f32 DISMOUNT_VEHICLE_CHECK_DIST = 3.0f;

        // Setup collision types to collide with
        u32 AttrMask = PLAYER_COLLIDE_ATTRS;

        // Make starting pos of collision check be above the vehicle
        bbox  StartBBox  = m_WorldBBox;
        StartBBox.Max.Y -= 1.0f;    // Take 1m off the top (in case head is already thru something)

        // Check collision to get from the player to above the vehicle
        vector3 Move = vector3( 0, DISMOUNT_VEHICLE_CHECK_DIST+1, 0 );     // Vector up.
        s_Collider.ClearExcludeList();
        if( pVehicle )
            s_Collider.Exclude( pVehicle->GetObjectID().GetAsS32() );
		s_Collider.ExtSetup( this, StartBBox, Move );
        ObjMgr.Collide( AttrMask, s_Collider );

        // Only allowed to jump out if there is nothing overhead.
        if( !s_Collider.HasCollided() )
        {
            // Now then, we want to possition the player as close to the vehicle as 
            // possible, so fire an Extrusion BACK down towards the vehicle.

            StartBBox  = m_WorldBBox;   
            StartBBox.Max.Y += DISMOUNT_VEHICLE_CHECK_DIST;    // Start "up".
            StartBBox.Min.Y += DISMOUNT_VEHICLE_CHECK_DIST;
            Move.Y = -DISMOUNT_VEHICLE_CHECK_DIST;             // Vector back down.

            s_Collider.ClearExcludeList();
		    s_Collider.ExtSetup( this, StartBBox, Move );
            ObjMgr.Collide( AttrMask, s_Collider );

            // Calc collision distance moved
            if( s_Collider.HasCollided() )
            {
                f32 Dist = DISMOUNT_VEHICLE_CHECK_DIST * (1.0f - s_Collider.GetCollisionT());

                // Put player at collision pos
                m_WorldPos.Y      += Dist;
                m_WorldBBox.Min.Y += Dist;
                m_WorldBBox.Max.Y += Dist;

                // Fix up view and weapon fire pos
                m_ViewPos.Y += Dist;
                m_WeaponFireL2W.Translate( vector3( 0, Dist, 0 ) );
            }

            // Don't take damage/get onto vehicle for a while
            m_VehicleDismountTime = 2.0f;

            // Do the jump!
            if( pVehicle )
                m_Vel = pVehicle->GetVelocity();
            m_Vel.Y += m_MoveInfo->JUMP_FORCE / GetMass();
            Player_SetupState( PLAYER_STATE_JUMP );
        }
        else
        {
            // Play sound.
            if( m_VehicleDismountTime == 0.0f )
            {
                tgl.pServer->SendAudio( SFX_POWER_INVENTORYSTATION_DENIED, m_ObjectID.Slot );
                m_VehicleDismountTime = 2.0f;
            }
        }
    }
}

//==============================================================================
// PLAYER_STATE_DEAD
//==============================================================================

// Death anims that can happen in the air
static s32 PlayerDeathAirAnims[] =
{
    ANIM_TYPE_CHARACTER_DIE_FORWARD,
    ANIM_TYPE_CHARACTER_DIE_BACKWARD,
    ANIM_TYPE_CHARACTER_DIE_SIDE,
    ANIM_TYPE_CHARACTER_DIE_KNEES,
    ANIM_TYPE_CHARACTER_DIE_SLUMP,
    ANIM_TYPE_CHARACTER_DIE_SPIN,
    0
} ;

// Death anims that can happen on the ground
static s32 PlayerDeathGroundAnims[] =
{
    ANIM_TYPE_CHARACTER_DIE_FORWARD,
    ANIM_TYPE_CHARACTER_DIE_BACKWARD,
    ANIM_TYPE_CHARACTER_DIE_SIDE,
    ANIM_TYPE_CHARACTER_DIE_CHEST,
    ANIM_TYPE_CHARACTER_DIE_HEAD,
    ANIM_TYPE_CHARACTER_DIE_KNEES,
    ANIM_TYPE_CHARACTER_DIE_LEFT_LEG,
    ANIM_TYPE_CHARACTER_DIE_RIGHT_LEG,
    ANIM_TYPE_CHARACTER_DIE_LEFT_SIDE,
    ANIM_TYPE_CHARACTER_DIE_RIGHT_SIDE,
    ANIM_TYPE_CHARACTER_DIE_SLUMP,
    ANIM_TYPE_CHARACTER_DIE_SPIN,
    0
} ;

void player_object::Player_Setup_DEAD()
{
    // Clear health increase
    m_HealthIncrease = 0 ;

    // Clear energy and heat
    SetEnergy(0) ;
    SetHeat(0) ;

    // Clear Voice Chat Menu
    if( m_HUDUserID )
        tgl.pHUDManager->DeactivateVoiceMenu( m_HUDUserID );

    // Make sure nothing hits this shell of a former warrior.
    m_AttrBits &= ~(object::ATTR_SOLID_DYNAMIC | object::ATTR_REPAIRABLE | object::ATTR_ELFABLE | object::ATTR_SENSED) ;

    // Allow other players to go through and collect ammo
    m_AttrBits |= object::ATTR_PERMEABLE ;

    // Get player shape
    shape* Shape = m_PlayerInstance.GetShape() ;
    ASSERT(Shape) ;

    // Add all dead anims to list
    selection_list  SelList ;
    s32*            DeathAnimType ;
    
    // Use ground or air anims?
    if (m_ContactInfo.RunSurface)
        DeathAnimType = PlayerDeathGroundAnims ;
    else
        DeathAnimType = PlayerDeathAirAnims ;

    // Add valid types
    while(*DeathAnimType)
    {
        // Only add anim if it exists
        if (Shape->GetAnimByType(*DeathAnimType))
            SelList.AddElement(*DeathAnimType, 1.0f) ;

        // Next anim
        DeathAnimType++ ;
    }

    // Choose random anim from list
    ASSERT(m_PlayerInstance.GetShape()) ;
    s32     AnimType = SelList.ChooseElement() ;
    anim*   Anim     = m_PlayerInstance.GetShape()->GetAnimByType(AnimType) ;

    // Keep anim for corpse generation, and start it!
    m_DeathAnimIndex = Anim->GetIndex() ;
    m_PlayerInstance.SetAnimByIndex(m_DeathAnimIndex, 0.1f) ;

    // Play scream
    audio_Play( GetVoiceSfxBase() + 134, &m_WorldPos );

    // On server?
    if( tgl.ServerPresent )
    {
        vector3 Pos = m_WorldPos;
        Pos.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.75f ;

        // Create pickup of current weapon?
        if (s_InventInfo[m_WeaponCurrentType].PickupKind != pickup::KIND_NONE)
        {
            // Clear inventory
            SetInventCount(m_WeaponCurrentType, 0) ;

            // Choose random xz direction, always move up
            vector3 Vel( 0, 0, 10 );
            Vel.RotateX( x_frand( -R_60,  R_10 ) );
            Vel.RotateY( x_frand(   R_0, R_360 ) );
            Vel += m_Vel;                      // Add on players velocity

            // Create the pickup
            CreatePickup( Pos, 
                          Vel,
                          s_InventInfo[m_WeaponCurrentType].PickupKind,
                          pickup::flags(pickup::FLAG_ANIMATED), 
                          20 ) ;
        }

        // Create pickup of current pack?
        if (s_InventInfo[m_PackCurrentType].PickupKind != pickup::KIND_NONE)
        {
            // Clear inventory
            SetInventCount(m_PackCurrentType, 0) ;

            // Choose random xz direction, always move up
            vector3 Vel( 0, 0, 10 );
            Vel.RotateX( x_frand( -R_60,  R_10 ) );
            Vel.RotateY( x_frand(   R_0, R_360 ) );
            Vel += m_Vel;                      // Add on players velocity

            // Create the pickup
            CreatePickup( Pos, 
                          Vel,
                          s_InventInfo[m_PackCurrentType].PickupKind,
                          pickup::flags(pickup::FLAG_ANIMATED), 
                          20 ) ;
        }

        // Create ammo box pickup
        {
            // Choose random xz direction, always move up
            vector3 Vel( 0, 0, 10 );
            Vel.RotateX( x_frand( -R_60,  R_10 ) );
            Vel.RotateY( x_frand(   R_0, R_360 ) );
            Vel += m_Vel;                      // Add on players velocity

            // Create the pickup
            CreatePickup( Pos, 
                          Vel,
                          pickup::KIND_AMMO_BOX,
                          pickup::flags(pickup::FLAG_ANIMATED), 
                          20 ) ;
        }

        // Create repair patch pickup
        {
            // Choose random xz direction, always move up
            vector3 Vel( 0, 0, 10 );
            Vel.RotateX( x_frand( -R_60,  R_10 ) );
            Vel.RotateY( x_frand(   R_0, R_360 ) );
            Vel += m_Vel;                      // Add on players velocity

            // Create the pickup
            CreatePickup( Pos, 
                          Vel,
                          pickup::KIND_HEALTH_PATCH,
                          pickup::flags(pickup::FLAG_ANIMATED), 
                          20 ) ;
        }

        // Clear pack
        SetPackCurrentType(INVENT_TYPE_NONE) ;
        SetPackActive(FALSE) ;

        // Clear grenade counts.
        SetInventCount( player_object::INVENT_TYPE_GRENADE_BASIC,      0 );
        SetInventCount( player_object::INVENT_TYPE_GRENADE_CONCUSSION, 0 );
        SetInventCount( player_object::INVENT_TYPE_GRENADE_FLARE,      0 );
        SetInventCount( player_object::INVENT_TYPE_MINE,               0 );

        // Disconnect from satchel charged (if connected)
        DetachSatchelCharge(TRUE) ; // Fizzle out old

        // Break the target lock
        if( m_TargetIsLocked )
        {
            m_TargetIsLocked = FALSE;
            m_LockedTarget = obj_mgr::NullID;
            m_DirtyBits |= PLAYER_DIRTY_BIT_TARGET_LOCK;
        }
    }
}

void player_object::Player_Advance_DEAD( f32 DeltaTime )
{
    (void)DeltaTime;

    // Allow opportunity to respawn?
    if( !m_Respawn )
    {
        return;
    }

    // Wait for longer if suicided
    f32 WaitTime ;
    if (m_DamagePain.PainType == pain::MISC_PLAYER_SUICIDE)
        WaitTime = 6 ;
    else
        WaitTime = 2 ;

    // Done waiting yet?
    if (m_PlayerStateTime < WaitTime)
        return ;

    // Re-spawn? (on server only)
    if (        (m_PlayerInstance.GetCurrentAnimState().GetAnim()->GetIndex() == m_DeathAnimIndex)
            &&  (tgl.pServer)
            &&  ((m_Move.FireKey) || (GetType() == object::TYPE_BOT))
            &&  (!m_PlayerInstance.IsBlending()) )
    {
        // Create corpse object
        CreateCorpse() ;
       
        // Respawn the player
        Respawn() ;
    }
}

//==============================================================================
// Stimuli functions
//==============================================================================

f32 DAMAGE_WIGGLE_RADIUS        =  0.25f;
f32 DAMAGE_WIGGLE_MIN_TIME      =  0.25f;
f32 DAMAGE_WIGGLE_MAX_TIME      =  1.50f;
f32 DAMAGE_WIGGLE_MAX_FORCE     = 50.00f;

// Pain event
void player_object::OnPain( const pain& Pain )
{
    // Hidden?
    if(m_PlayerState == PLAYER_STATE_CONNECTING)
        return;

    // Record pain for death delay (only if alive so it's not overwritten)
    if (m_Health > 0)
        m_DamagePain = Pain ;

    // Protected by vehicle?
    vehicle* pVehicle = (vehicle*)IsAttachedToVehicle();
    if (        (pVehicle)                                      // on a vehicle
            &&  (pVehicle->GetTeamBits() & m_TeamBits)          // vehicle is friendly
            &&  (Pain.DirectHit == FALSE)                       // not a direct hit
            &&  (Pain.MetalDamage >= 0.0f)                      // real pain, not healing
            &&  (Pain.PainType != pain::MISC_BOUNDS_HURT)       // not bounds pain
            &&  (Pain.PainType != pain::MISC_BOUNDS_DEATH)      // not bounds death
            &&  (Pain.PainType != pain::MISC_PLAYER_SUICIDE)    // not suicide pain
            &&  (Pain.PainType != pain::MISC_FORCE_FIELD) )     // not force field pain
        return ;

    // Protected by spawn?
    if (        (m_SpawnTime > 0)
            &&  (Pain.PainType != pain::MISC_PLAYER_SUICIDE) )
        return ;

    // Check if player is jumping from vehicle
    if( GetVehicleDismountTime() > 0.0f )
    {
        // Don't apply pain if we collide with the vehicle we are jumping from
        if( Pain.OriginID == m_LastVehicleID )
            return;

        if( IN_RANGE( pain::HIT_BY_GRAV_CYCLE, Pain.PainType, pain::HIT_BY_TRANSPORT ) )
        {
            object* pObject = ObjMgr.GetObjectFromID( Pain.OriginID );
            if( pObject )
            {
                if( pObject->GetAttrBits() & object::ATTR_PLAYER )
                {
                    player_object* pPilot   = (player_object*)pObject;
                    vehicle*       pVehicle = NULL;
                    if( pPilot->GetVehicleStatus( pVehicle ) == VEHICLE_STATUS_PILOT )
                    {
                        if( pVehicle && (pVehicle->GetObjectID() == m_LastVehicleID) )
                        {
                            return;
                        }
                    }
                }
            }
        }
    }

    // Check if player is locked to a station.
    if( m_PlayerState == PLAYER_STATE_AT_INVENT )
    {
        if( IN_RANGE( pain::WEAPON_LASER, Pain.PainType, pain::WEAPON_LASER_HEAD_SHOT ) )
            return;
    }

    // Setup update velocity flag
    xbool bUpdateVel = (m_PlayerState != PLAYER_STATE_AT_INVENT) &&
                       (m_PlayerState != PLAYER_STATE_VEHICLE) ;

    // Take mass into account (ie. apply an impulse)
    f32 Force = Pain.Force;
    Force /= GetMass() ;

    // Wiggle?
    if( (Force > 0.0f) && (Pain.PainType != pain::MISC_FLIP_FLOP_FORCE) )
    {
        f32 F = (Force/DAMAGE_WIGGLE_MAX_FORCE);
        F = MINMAX( 0.0f, F, 1.0f );
        if( !Pain.LineOfSight ) 
            F *= 0.5f;
        SetDamageWiggle( DAMAGE_WIGGLE_MIN_TIME + F*(DAMAGE_WIGGLE_MAX_TIME-DAMAGE_WIGGLE_MIN_TIME), DAMAGE_WIGGLE_RADIUS*F );
    }

    // Move the player?
    if( (Pain.LineOfSight) && (bUpdateVel) && (Force > 0.0f) )
    {
        vector3 ForceDir = m_WorldBBox.GetCenter() - Pain.Center;
        ForceDir.Normalize();
        m_Vel += ForceDir * Force;
        m_DirtyBits |= PLAYER_DIRTY_BIT_VEL ;

        if (m_ProcessInput)
        {
            f32 feedbackforce;

            feedbackforce = Force / 20.0f;
            input_Feedback(0.25f,feedbackforce,m_ControllerID);
        }
        // Concussion grenade?
        if( Pain.PainType == pain::WEAPON_GRENADE_CONCUSSION )
        {
            // Consider dropping stuff.
            ApplyConcussion( Pain.Distance );
        }
    }

    // Damage (or heal) the player?
    if( (Pain.LineOfSight) && 
        (!IsDead()) && 
        ((Pain.MetalDamage != 0.0f) || (Pain.EnergyDamage != 0.0f)) )
    {
        // Check for death
        f32 HealthBeforePain = m_Health ;

        // Update health and energy
        AddHealth( -Pain.MetalDamage * 100.0f ) ;
        AddEnergy( -Pain.EnergyDamage * m_MoveInfo->MAX_ENERGY );

        // Only kill on the server
        if (tgl.ServerPresent)
        {
            // Did player just die?
            if ((HealthBeforePain > 0) && (m_Health == 0))
            {
                // Clear health increase
                m_HealthIncrease = 0 ;

                // Die
                Player_SetupState(PLAYER_STATE_DEAD, FALSE) ;

                // DMT TO DO: Maybe drop some health.

                // Inform the GameMgr of this unfortunate death.
                pGameLogic->PlayerDied( Pain );

                // And the missiles and the turrets and such can let off for a bit.
                ObjMgr.UnbindTargetID( m_ObjectID );
            }
        }

        // Flash the screen
        FlashScreen((Pain.MetalDamage * s_CommonInfo.MAX_HURT_SCREEN_FLASH_TIME), xcolor(255,0,0,(u8)((Pain.MetalDamage * 255.0f)))) ;

        // Flash the shield?
        if( IsShielded() && (m_Energy > 0.0f) && (Pain.EnergyDamage > 0.0f) )
        {
            // Create shield
            CreateShieldEffect( m_ObjectID, m_Energy / m_MoveInfo->MAX_ENERGY );
        }
    }

    #if 0
    {
        object::id PlayerID( tgl.PC[ tgl.WC ].PlayerIndex, -1 );
        
        object* pOrigin = ObjMgr.GetObjectFromID( Pain.OriginID );
        
        if( pOrigin )
        {
            if( Pain.OriginID.Slot == PlayerID.Slot )
            {
                x_printf( "HIT\n" );
            }
        }
    }
    #endif

    // Keep accels and vels in limits for sending over the net...
    Truncate() ;
}

//==============================================================================

void player_object::ApplyConcussion( f32 Distance )
{
    // Items are dropped only if concussion is within 8 meters.
    if( Distance > 8.0f )
        return;

    // Drop the flag?  Only if not in hunter.
    if( (GameMgr.GetGameType() != GAME_HUNTER) && (HasFlag()) )
    {
        pGameLogic->ThrowFlags( m_ObjectID );
    }

    // Nothing else is dropped if you are in a vehicle.
    if( IsAttachedToVehicle() )
        return;

    // Drop the weapon.
    if ( (m_WeaponCurrentType != INVENT_TYPE_NONE) &&
         (GetInventCount( m_WeaponCurrentType ) > 0) )
    {
        // Throw from shoulders
        vector3 Pos = m_WorldPos;
        Pos.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.75f ;

        // Choose random xz direction, always move up
        vector3 Vel(0, 0, 30) ;                 // Vel
        Vel.RotateX(m_DrawRot.Pitch - R_65) ;   // Throw up
        Vel.RotateY(m_DrawRot.Yaw + x_frand(-R_20, R_20) );            // Throw forward

        // Create the pickup
        CreatePickup( Pos, 
                      Vel,
                      s_InventInfo[m_WeaponCurrentType].PickupKind,
                      pickup::flags(pickup::FLAG_ANIMATED), 
                      20 );

        // Remove from inventory
        AddInvent(m_WeaponCurrentType, -1) ;

        // Request the weapon in the same slot since it may have been 
        // updated to "no weapon"..
        m_WeaponRequestedType = m_Loadout.Weapons[m_WeaponCurrentSlot] ;
    }

    // Drop the pack.
    if( (Distance < 6.0f) && (m_PackCurrentType != INVENT_TYPE_NONE) )
    {
        // Throw from shoulders
        vector3 Pos = m_WorldPos;
        Pos.Y += m_MoveInfo->BOUNDING_BOX.Max.Y * 0.75f ;

        // Choose random xz direction, always move up
        vector3 Vel(0, 0, 10) ;                 // Vel
        Vel.RotateX(m_DrawRot.Pitch - R_45) ;   // Throw up
        Vel.RotateY(m_DrawRot.Yaw) ;            // Throw forward
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

//==============================================================================
