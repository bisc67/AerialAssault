//==============================================================================
//
//  BotCombat.cpp
//
//  Contains methods for the bot_object class that relate to combat
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================
#include "BotObject.hpp"
#include "Disk.hpp"
#include "Plasma.hpp"
#include "Bullet.hpp"
#include "Blaster.hpp"
#include "Grenade.hpp"
#include "Aimer.hpp"
#include "Objects/Vehicles/Vehicle.hpp"
#include "LabelSets/Tribes2Types.hpp"

//==============================================================================
//  DEFINES
//==============================================================================
static const f32 SPLASH_DAMAGE_HEIGHT = 2.0f;
static const f32 MAX_CLOSE_RANGE = 15.0f;
static const f32 MAX_CLOSE_RANGE_SQ = x_sqr( MAX_CLOSE_RANGE );
static const f32 RADIAN_CLOSE_ENOUGH = R_1;
static const f32 AIM_PURSUIT_PREDICTION_CONSTANT = 0.8f;
static const f32 MIN_SNIPER_RIFLE_SELECTION_ENERGY = 0.5f;
static const f32 AIR_SUPERIORITY_HEIGHT_DIFFERENCE = 5.0f;
static const f32 MAX_WEAPON_CHANGE_TIME = 2.0f; // seconds
static const f32 MAX_AIM_FUDGE_TIME = 5.0f;
static const f32 INIT_AIM_FUDGE_SCALER = 20.0f;

//==============================================================================
//  TYPES
//==============================================================================

//==============================================================================
//  STORAGE
//==============================================================================

extern bot_object::bot_debug g_BotDebug;
extern f32 CHAIN_GUN_FIRE_HOLD_TIME;
extern s32 g_BotFrameCount;
extern vector3 Set1[50];
extern vector3 Set2[50];
extern s32     SetI;
extern xbool SHOW_BLENDING;

//----------------------------------------------------------------------
// This is a 4-d lookup table, where the first dimension is range (close or
// long), the second dimension is splash/no splash, and the third dimension
// is the armor type of the target (light, med, heavy), and the fourth
// dimension is the sorted best weapon list for the situation.
//----------------------------------------------------------------------
static const s32 NUM_BOT_WEAPONS = 8;

//==============================================================================
//  FUNCTIONS
//==============================================================================


//==============================================================================
// SelectWeapon()
// Sets the bot's requested weapon slot to be the best weapon to use
// against the given target. The next weapon button will need to be pressed
// until this weapon is used.
//==============================================================================
void bot_object::SelectWeapon( const object& Target )
{
    s32 CurRequestedSlot = m_RequestedWeaponSlot;

    //--------------------------------------------------
    // Score all the weapons
    //--------------------------------------------------
    weapon_score pScores[10];
    ASSERT( m_Loadout.NumWeapons <= 10 );

    s32 i;

    for ( i = 0; i < m_Loadout.NumWeapons; ++i )
    {
        pScores[i] = ScoreWeapon( m_Loadout.Weapons[i], Target );
    }

    //--------------------------------------------------
    // Is our current weapon among the best?
    //--------------------------------------------------
    xbool CurrentIsBest = TRUE;
    for ( i = 0; i < m_Loadout.NumWeapons; ++i )
    {
        if ( pScores[m_RequestedWeaponSlot] < pScores[i] )
        {
            CurrentIsBest = FALSE;
            break;
        }
    }

    //--------------------------------------------------
    // If not among best, randomly pick among the best
    //--------------------------------------------------
    if ( !CurrentIsBest )
    {
        // store the slots of the best
        s32 pBestSlots[10];
        s32 NumSlots = 0;

        for ( i = 0; i < m_Loadout.NumWeapons; ++i )
        {
            if ( (NumSlots == 0)
                || (pScores[i] > pScores[pBestSlots[0]]) )
            {
                // we have a new best score
                NumSlots = 1;
                pBestSlots[0] = i;
            }
            else if ( pScores[i] == pScores[pBestSlots[0]] )
            {
                // another of the best
                pBestSlots[NumSlots] = i;
                ++NumSlots;
            }
        }

        // now pick one
        m_RequestedWeaponSlot = pBestSlots[x_irand( 0, NumSlots-1 )];
    }

    // Make sure we're not prematurely changing from the missile launcher
    if ( CurRequestedSlot != m_RequestedWeaponSlot )
    {
        if ( m_RequestedWeaponSlot == INVENT_TYPE_WEAPON_MISSILE_LAUNCHER )
        {
            m_SelectedMissileLauncherTimer.Reset();
            m_SelectedMissileLauncherTimer.Start();
        }
        else if ( (CurRequestedSlot == INVENT_TYPE_WEAPON_MISSILE_LAUNCHER)
            && ((m_SelectedMissileLauncherTimer.ReadSec() < 5.0f)
                || (m_MissileLock)) )
        {
            // Deny the weapon change, we must keep the missile launcher for 5s.
            m_RequestedWeaponSlot = CurRequestedSlot;
        }
    }

    // Use this line to force a weapon to be selected
    //m_RequestedWeaponSlot = WeaponInSlot( INVENT_TYPE_WEAPON_BLASTER );
}   

//==============================================================================
// ScoreWeapon
//==============================================================================
bot_object::weapon_score bot_object::ScoreWeapon( invent_type WeaponType
    , const object& Target )
{
    //--------------------------------------------------
    // If we don't have enough ammo, no good
    //--------------------------------------------------
    if ( GetWeaponAmmoCount( WeaponType )
        < s_WeaponInfo[WeaponType].AmmoNeededToAllowFire )
    {
        return WEAPON_SCORE_USELESS;
    }

    //--------------------------------------------------
    // Set up some useful information for later
    //--------------------------------------------------
    f32 DistSQ = (Target.GetPosition() - m_WorldPos).LengthSquared();
    const player_object* pTargetPlayer
        = (Target.GetAttrBits() & object::ATTR_PLAYER)
        ? (const player_object*)(&Target)
        : NULL;

    const xbool TargetIsJericho = (Target.GetType() == TYPE_VEHICLE_JERICHO2);

    const xbool TargetIsReallyTough
        =     ((Target.GetType() == TYPE_TURRET_FIXED)
            || (Target.GetAttrBits() & ATTR_VEHICLE)
            || ((Target.GetAttrBits() & ATTR_PLAYER)
                && (((player_object*)(&Target))->IsAttachedToVehicle())));
    
    const xbool TargetIsTough
        =   (TargetIsReallyTough
            || (Target.GetType() == TYPE_GENERATOR)
            || (Target.GetType() == TYPE_STATION_FIXED));

    const xbool TargetIsMovingPlayer = (pTargetPlayer
        && (pTargetPlayer->GetVel().LengthSquared() > 0.1f));
    const xbool TargetIsCarryingFlag = (pTargetPlayer
        && (pTargetPlayer->HasFlag()));
    const xbool TargetIsAttachedToVehicle = (pTargetPlayer
        && (pTargetPlayer->IsAttachedToVehicle()));

    collider            Ray;
    collider::collision Collision;
    
    vector3 Bullseye;
    GetTargetBullseye( Bullseye, Target, WeaponType );
    Ray.RaySetup    ( NULL, GetMuzzlePosition(), Bullseye, m_ObjectID.GetAsS32(), TRUE );
    Ray.Exclude(Target.GetObjectID().GetAsS32()) ;
    ObjMgr.Collide  ( ATTR_SOLID_STATIC, Ray );
    Ray.GetCollision(Collision) ;
    const xbool CanSeeTarget = !Collision.Collided;
    
    //--------------------------------------------------
    // Score weapon
    //--------------------------------------------------
    switch ( WeaponType )
    {
    case INVENT_TYPE_WEAPON_DISK_LAUNCHER:
    {
        const xbool Near = (DistSQ < x_sqr( 50.0f ));

        if ( CanSeeTarget
            && !TargetIsTough
            && ((Near && pTargetPlayer)
                || !TargetIsMovingPlayer) )
        {
            if ( TargetIsCarryingFlag )
            {
                return WEAPON_SCORE_APPROPRIATE;
            }
            else
            {
                return WEAPON_SCORE_DESIRABLE;
            }
        }
        
        if ( CanSeeTarget
            && !Near 
            && CanSplashDamage( Target ) )
        {
            return WEAPON_SCORE_APPROPRIATE;
        }
        
        return WEAPON_SCORE_INAPPROPRIATE;
    }
        
    case INVENT_TYPE_WEAPON_PLASMA_GUN:
    {
        const xbool Near = (DistSQ < x_sqr( 20.0f ));
        const xbool Far  = (DistSQ > x_sqr( 100.0f ));

        if ( !CanSeeTarget || Far )
        {
            return WEAPON_SCORE_USELESS;
        }

        if ( TargetIsReallyTough )
        {
            return WEAPON_SCORE_INAPPROPRIATE;
        }
        
        if ( Near || !TargetIsMovingPlayer )
        {
            if ( TargetIsCarryingFlag )
            {
                return WEAPON_SCORE_APPROPRIATE;
            }
            else
            {
                return WEAPON_SCORE_DESIRABLE;
            }
        }
        
        if ( !Near && CanSplashDamage( Target ) )
        {
            return WEAPON_SCORE_APPROPRIATE;
        }
        
        return WEAPON_SCORE_INAPPROPRIATE;
    }
    
    case INVENT_TYPE_WEAPON_SNIPER_RIFLE:
    {
        if ( !CanSeeTarget ) return WEAPON_SCORE_USELESS;

        if ( !TargetIsMovingPlayer && !TargetIsTough )
        {
            if ( m_Energy > 0.9f )
            {
                if ( TargetIsCarryingFlag )
                {
                    return WEAPON_SCORE_APPROPRIATE;
                }
                else
                {
                    return WEAPON_SCORE_DESIRABLE;
                }
            }

            if ( m_Energy > 0.5f )
            {
                return WEAPON_SCORE_APPROPRIATE;
            }
        }

        return WEAPON_SCORE_INAPPROPRIATE;
    }
    
    case INVENT_TYPE_WEAPON_MORTAR_GUN:
    {
        // see if we can hit target
        if ( (Target.GetObjectID() == m_MortarSelectObjectID)
            || (!m_MortarSelectTimer.IsRunning()
                || (m_MortarSelectTimer.ReadSec() > 2.0f)) )
        {
            // time to check again
            m_MortarSelectCanHitTarget = CanHitTargetArcing(
                INVENT_TYPE_WEAPON_MORTAR_GUN
                , Target );
            m_MortarSelectTimer.Reset();
            m_MortarSelectObjectID = Target.GetObjectID();
        }

        if ( !m_MortarSelectCanHitTarget )
        {
            return WEAPON_SCORE_USELESS;
        }
        
        if (Target.GetType() == TYPE_TURRET_CLAMP)
        {
            return WEAPON_SCORE_DESIRABLE;
        }

        const xbool Near = (DistSQ < x_sqr( 35.0f ));

        if ( Near || !TargetIsMovingPlayer )
        {
            if ( TargetIsCarryingFlag || TargetIsJericho )
            {
                return WEAPON_SCORE_APPROPRIATE;
            }
            else
            {
                return WEAPON_SCORE_DESIRABLE;
            }
        }

        if ( !Near && TargetIsMovingPlayer )
        {
            return WEAPON_SCORE_USELESS;
        }

        return WEAPON_SCORE_INAPPROPRIATE;
    }
    
    case INVENT_TYPE_WEAPON_GRENADE_LAUNCHER:
    {
        const xbool Near = (DistSQ < x_sqr( 20.0f ));

        if ( !TargetIsTough
            && (Near || !TargetIsMovingPlayer) )
        {
            if ( TargetIsCarryingFlag )
            {
                return WEAPON_SCORE_APPROPRIATE;
            }
            else
            {
                return WEAPON_SCORE_DESIRABLE;
            }
        }

        if ( !Near && TargetIsMovingPlayer )
        {
            return WEAPON_SCORE_USELESS;
        }

        return WEAPON_SCORE_INAPPROPRIATE;
    }
    
    case INVENT_TYPE_WEAPON_CHAIN_GUN:
    {
        if ( !CanSeeTarget ) return WEAPON_SCORE_USELESS;

        const xbool Near = (DistSQ < x_sqr( 10.0f ));
        const xbool Far = (DistSQ > x_sqr( 100.0f ));

        if ( !TargetIsTough
            && ((!Near && !Far)
                || (Target.GetAttrBits() & object::ATTR_VEHICLE)) )
        {
            if ( TargetIsCarryingFlag )
            {
                return WEAPON_SCORE_APPROPRIATE;
            }
            else
            {
                return WEAPON_SCORE_INAPPROPRIATE;
            }
        }

        if ( Near && TargetIsMovingPlayer )
        {
            return WEAPON_SCORE_INAPPROPRIATE;
        }

        return WEAPON_SCORE_INAPPROPRIATE;
    }
    
    case INVENT_TYPE_WEAPON_BLASTER:
    {
        if ( !CanSeeTarget ) return WEAPON_SCORE_USELESS;

        const xbool Near = (DistSQ < x_sqr( 30.0f ));

        if ( Near && !TargetIsTough )
        {
            return WEAPON_SCORE_APPROPRIATE;
        }

        return WEAPON_SCORE_INAPPROPRIATE;
    }
    
    case INVENT_TYPE_WEAPON_ELF_GUN:
    {
        const xbool Near = (DistSQ < x_sqr( 10.0f ));

        if ( Near )
        {
            // check to see if we have a teammate nearby
            player_object* pPlayer;

            ObjMgr.Select( object::ATTR_PLAYER , m_WorldPos, 20.0f );

            xbool NearbyTeammate = FALSE;

            while ( (pPlayer = (player_object*)(ObjMgr.GetNextSelected()))
                != NULL )
            {
                if ( pPlayer->GetTeamBits() & GetTeamBits() )
                {
                    NearbyTeammate = TRUE;
                    break;
                }
            }

            ObjMgr.ClearSelection();

            if ( NearbyTeammate )
            {
                return WEAPON_SCORE_DESIRABLE;
            }

            return WEAPON_SCORE_APPROPRIATE;
        }

        return WEAPON_SCORE_USELESS;
    }
    
    case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:
    {
        if ( !CanSeeTarget ) return WEAPON_SCORE_USELESS;

        const xbool Near = (DistSQ < x_sqr( 20.0f ));

        // Missile Launcher must be at least 20 m from target to get a lock.
        if ( Near )     
        {
            return WEAPON_SCORE_USELESS;
        }

        // carrying flag, in vehicle, or is vehicle
        if (    TargetIsReallyTough
                || TargetIsCarryingFlag
                || TargetIsAttachedToVehicle
                || (Target.GetAttrBits() & object::ATTR_HOT) )
        {
            return WEAPON_SCORE_DESIRABLE;
        }

        return WEAPON_SCORE_USELESS;
    }
    
    case INVENT_TYPE_WEAPON_SHOCKLANCE:
    {
        const xbool Near = (DistSQ < x_sqr( 5.0f ));

        if ( Near && !TargetIsTough )
        {
            return WEAPON_SCORE_DESIRABLE;
        }

        return WEAPON_SCORE_USELESS;
    }

    case INVENT_TYPE_START:
        return WEAPON_SCORE_USELESS;
    
    default:
        ASSERT( 0 ); // Trying an unknown weapon...
        return WEAPON_SCORE_USELESS;
    }
}



//==============================================================================
// GetAimDirection
//
//==============================================================================
void bot_object::AimAtTarget( const object& Target )
{
    // We're going to aim the requested weapon, not the
    // current weapon, so that we don't have as far to
    // go when the requested weapon is the current weapon
    const invent_type& RequestedWeaponType
        = (m_CurrentGoal.GetGoalID() == bot_goal::ID_REPAIR ? 
                player_object::INVENT_TYPE_WEAPON_REPAIR_GUN 
                : m_Loadout.Weapons[m_RequestedWeaponSlot]);

    // Get the object's velocity if applicable
    vector3 TargetVelocity;
    if ( Target.GetType() == object::TYPE_PLAYER
        || Target.GetType() == object::TYPE_BOT )
    {
        TargetVelocity = ((player_object *)(&Target))->GetVel();
    }
    else if ( Target.GetAttrBits() & ATTR_VEHICLE )
    {
        TargetVelocity = ((vehicle*)(&Target))->GetVel();
    }
    else
    {
        TargetVelocity.Set( 0.0f, 0.0f, 0.0f );
    }

    //--------------------------------------------------
    // Adjust target and source velocities for difficulty
    // level we'll assume the bots undercompensate for
    // velocity
    //--------------------------------------------------
    TargetVelocity.X *= m_Difficulty;
    TargetVelocity.Z *= m_Difficulty;

    vector3 SourceVelocity = m_Vel;
    SourceVelocity.X *= m_Difficulty;
    SourceVelocity.Z *= m_Difficulty;

    //--------------------------------------------------
    // Determine the firing speed, velocity
    // inheritance, and lifetime of a shot
    //--------------------------------------------------
    const weapon_info& WeaponInfo = GetWeaponInfo( RequestedWeaponType );

    //--------------------------------------------------
    // Now set m_AimDirection...
    //--------------------------------------------------

    // bullseye
    GetTargetBullseye( m_Bullseye, Target, RequestedWeaponType );

    vector3 AimPosition;

    const vector3& WeaponPosition = GetWeaponPos() ;

    vector3 ViewPos;
    radian3 Temp1;
    f32     Temp2;
    Get1stPersonView( ViewPos, Temp1, Temp2 );

    switch ( RequestedWeaponType )
    {
    // "Instant weapons
    case INVENT_TYPE_WEAPON_ELF_GUN:
    case INVENT_TYPE_WEAPON_REPAIR_GUN:
    case INVENT_TYPE_WEAPON_SHOCKLANCE:
    case INVENT_TYPE_WEAPON_SNIPER_RIFLE:
    case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:
        m_AimDirection = m_Bullseye - ViewPos;

        // Screw up aiming for moving player targets
        if ( Target.GetAttrBits() & object::ATTR_PLAYER )
        {
            const player_object* pPlayer = (const player_object*)(&Target);
            const vector3 RelativeVelocity = pPlayer->GetVel() - m_Vel;
            m_AimDirection += ((1 - m_Difficulty) * RelativeVelocity * 0.2f);
        }
        m_AimDirection.Normalize();
        break;
        
    // Linear projectile weapons
    case INVENT_TYPE_WEAPON_DISK_LAUNCHER:
    case INVENT_TYPE_WEAPON_PLASMA_GUN:
    case INVENT_TYPE_WEAPON_CHAIN_GUN:
        if ( !CalculateLinearAimDirection(
            m_Bullseye
            , TargetVelocity
            , WeaponPosition
            , vector3( 0, 0, 0 ) //SourceVelocity
            , WeaponInfo.VelocityInheritance
            , WeaponInfo.MuzzleSpeed
            , WeaponInfo.MaxAge * 1000
            , m_AimDirection
            , AimPosition) )
        {
            m_AimDirection.Set( 0.0f, 0.0f, 0.0f );
        }
        
        break;

    // Blaster
    case INVENT_TYPE_WEAPON_BLASTER:
        if ( !CalculateLinearAimDirection(
            m_Bullseye
            , TargetVelocity - m_Vel
            , WeaponPosition
            , vector3( 0, 0, 0 ) //SourceVelocity
            , WeaponInfo.VelocityInheritance
            , WeaponInfo.MuzzleSpeed
            , WeaponInfo.MaxAge * 1000
            , m_AimDirection
            , AimPosition) )
        {
            m_AimDirection.Set( 0.0f, 0.0f, 0.0f );
        }
        
        break;
    // Arcing weapons
    case INVENT_TYPE_WEAPON_MORTAR_GUN:
        if ( !CalculateArcingAimDirection(
            m_Bullseye
            , TargetVelocity
            , WeaponPosition
            , vector3( 0, 0, 0 ) 
            , WeaponInfo.VelocityInheritance
            , WeaponInfo.MuzzleSpeed
            , WeaponInfo.MaxAge * 1000
            , m_AimDirection
            , AimPosition) )
        {
            m_AimDirection.Set( 0.0f, 0.0f, 0.0f );
        }
        break;

    case INVENT_TYPE_WEAPON_GRENADE_LAUNCHER:
        if ( !CalculateArcingAimDirection(
            m_Bullseye
            , TargetVelocity
            , WeaponPosition
            , SourceVelocity
            , WeaponInfo.VelocityInheritance
            , WeaponInfo.MuzzleSpeed
            , WeaponInfo.MaxAge * 1000
            , m_AimDirection
            , AimPosition) )
        {
            m_AimDirection.Set( 0.0f, 0.0f, 0.0f );
        }
        break;
    }

    if ( m_AimDirection.LengthSquared() > 0 )
    {
        FaceDirection( m_AimDirection );
    }
    else
    {
        FaceDirection( m_Rot );
    }

    if ( g_BotDebug.DrawAimingMarkers )
    {
        // Draw where we're aiming
        const vector3 ToTarget = m_Bullseye - WeaponPosition;
        vector3 Facing( m_Rot );
        Facing.Normalize();
        Facing *= ToTarget.Length();
        DrawLine( WeaponPosition, WeaponPosition + Facing, XCOLOR_RED );

        // Draw where we want to aim
        Facing = m_AimDirection;
        Facing.Normalize();
        Facing *= ToTarget.Length();
        DrawLine( WeaponPosition, WeaponPosition + Facing, XCOLOR_GREEN );
        DrawLine( WeaponPosition, WeaponPosition + ToTarget, XCOLOR_PURPLE );

    }
}

//==============================================================================
// CanSplashDamage
// returns TRUE if the target is in a location where splash damage would
// be significant
//==============================================================================
xbool bot_object::CanSplashDamage( const object& Target ) const
{
    // TODO: this won't work in buildings when the floor isn't coincident
    // with the terrain...
    
    // TODO: make this a little smarter by checking for being near walls but
    // in the air.

    if ( Target.GetAttrBits() & object::ATTR_PERMANENT )
    {
        // permanent stuff is mounted somewhere, so it's splashable
        return true;
    }

    if ( Target.GetAttrBits() & object::ATTR_PLAYER )
    {
        const player_object* pPlayer = (player_object*)(&Target);
        if ( (pPlayer->OnGround() || pPlayer->NearGround())
            && (pPlayer->GetPosition().Y <= GetMuzzlePosition().Y) )
        {
            return true;
        }
    }
    
    // We can splash damage if target is within so many meters of the ground
    const vector3& TargetPos = Target.GetPosition();
    const f32 GroundLevel = m_pTerr->GetHeight( TargetPos.Z, TargetPos.X );

    if ( (TargetPos.Y - GroundLevel) <= SPLASH_DAMAGE_HEIGHT )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//==============================================================================
// WeaponInSlot
// returns appropriate weapon slot if weapon is in loadout, -1 if
// weapon is not in loadout
//==============================================================================
s32 bot_object::WeaponInSlot( invent_type WeaponTYPE ) const
{
    s32 i;

    for ( i = 0; i < m_Loadout.NumWeapons; ++i )
    {
        if ( m_Loadout.Weapons[i] == WeaponTYPE )
        {
            return i;
        }
    }

    return -1;
}


//==============================================================================
// FacingTarget()
//
// returns true if we are nearly facing the given target object
//==============================================================================
xbool bot_object::FacingTarget( const object& Target ) const 
{
    const vector3 DeltaTarget = Target.GetPosition() - m_WorldPos;
                
    if ( (radian_DeltaAngle( m_Rot.Yaw, DeltaTarget.GetYaw() )
        <= RADIAN_CLOSE_ENOUGH)
        && (radian_DeltaAngle( m_Rot.Pitch, DeltaTarget.GetPitch() )
            <= RADIAN_CLOSE_ENOUGH) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//==============================================================================
// FacingAimDirection()
//
// returns true if we are nearly facing m_AimDirection
//==============================================================================
xbool bot_object::FacingAimDirection( f32 Distance /* = 0.0f */ ) const 
{
    if ( m_AimDirection.LengthSquared() < 0.000001f )
    {
        return FALSE;
    }
    else
    {
#if 0  // mreed: this is a possible different way of doing things, and it didn't seem to help
        vector3 ToTarget = m_Bullseye - GetMuzzlePosition();
        vector3 Facing = vector3( m_Rot );
        ASSERT( Facing.LengthSquared() - 1.0f < 0.0001f );

        Facing *= ToTarget.Length();

        return ((m_WorldPos + Facing) - m_Bullseye).LengthSquared() <= x_sqr( 0.2f );
#else
        static const f32 MAX_DIST = 0.02f;
        
        const radian MaxAngle
            = (Distance > 0.0f)
            ? x_atan( MAX_DIST / Distance )
            : RADIAN_CLOSE_ENOUGH;
        const radian Angle = v3_AngleBetween( vector3(m_Rot), m_AimDirection );
        return ( Angle <= MaxAngle);
#endif
    }
}

//==============================================================================
// GetNearestVisibleEnemy()
//
//==============================================================================
player_object*  bot_object::GetNearestVisibleEnemy( void )
{
    //----------------------------------------------------------------------
    // Look first for nearest enemy player, then for nearest enemy bot
    //----------------------------------------------------------------------
    player_object* NearestEnemy = NULL;
    f32 NearestDistSQ = F32_MAX;
    player_object* TestPlayer;

    ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
    while ( (TestPlayer = (player_object*)(ObjMgr.GetNextInType())) != NULL )
    {
        if ( !TestPlayer->IsDead()
            && IsPlayerCloserVisibleEnemy( NearestDistSQ, *TestPlayer ) )
        {
            NearestEnemy = TestPlayer;
        }
    }
    ObjMgr.EndTypeLoop();

    //----------------------------------------------------------------------
    // Now for the nearest enemy bot
    //----------------------------------------------------------------------
    ObjMgr.StartTypeLoop( object::TYPE_BOT );
    while ( (TestPlayer = (player_object*)(ObjMgr.GetNextInType())) != NULL )
    {
        if ( !TestPlayer->IsDead()
            && IsPlayerCloserVisibleEnemy( NearestDistSQ, *TestPlayer ) )
        {
            NearestEnemy = TestPlayer;
        }
    }
    ObjMgr.EndTypeLoop();

    return NearestEnemy;
}

//==============================================================================
// TryToFireOnTarget()
//==============================================================================
void bot_object::TryToFireOnTarget( const object& Target
    , xbool ForceCanHit /* = FALSE */ ) 
{
    
    // we're not blind
    if (m_ScreenFlashCount > 0)
    {
        // we're blind
        return;
    }
        
    // Select a weapon, aim, and fire if possible
    SelectWeapon( Target );

    // set up aiming
    m_Aiming = TRUE;
    m_AimingTarget = Target.GetObjectID();
    
    if ( m_RequestedWeaponSlot == m_WeaponCurrentSlot ) 
    {
        invent_type CurWeapon = m_Loadout.Weapons[m_WeaponCurrentSlot];
        const xbool UsingMissileLauncher
            = (CurWeapon == INVENT_TYPE_WEAPON_MISSILE_LAUNCHER);
        const xbool Locked = (UsingMissileLauncher && m_MissileLock);
        const xbool Arcing = ((CurWeapon == INVENT_TYPE_WEAPON_MORTAR_GUN)
            || (CurWeapon == INVENT_TYPE_WEAPON_GRENADE_LAUNCHER));

        if (// Have missile lock
            Locked

            || (// Not using missile
                !UsingMissileLauncher

                // Facing the right way
                && FacingAimDirection( (Target.GetPosition()-GetMuzzlePosition()).Length() )

                // Can hit them
                && (ForceCanHit
                    || (Arcing
                        ? CanHitTargetArcing( CurWeapon, Target )
                        : CanHitTarget( Target )))) )
        {
            m_Rot.Pitch = m_AimDirection.GetPitch();
            m_Rot.Yaw = m_AimDirection.GetYaw();
            if ( g_BotDebug.DrawAimingMarkers )
            {
                SHOW_BLENDING = FALSE;
                if( SetI < 50 )
                {
                    Set1[SetI] = GetMuzzlePosition();
                    Set2[SetI] = Set1[SetI] + ((vector3)m_Rot * (Target.GetPosition()-m_WorldPos).Length());
                    SetI++;
                }
                else
                {
                    SetI = 0;
                }
            }
            
            // Fire away...
            m_Move.FireKey = TRUE;

            // if we're just starting to fire the chain gun, we need to record
            // that fact so that we hold the button down for a while
            if ( (m_Loadout.Weapons[m_WeaponCurrentSlot]
                    == player_object::INVENT_TYPE_WEAPON_CHAIN_GUN)
                && (m_ChainGunFireTimer.ReadSec() >= CHAIN_GUN_FIRE_HOLD_TIME
                    || !m_ChainGunFireTimer.IsRunning()) )
            {
                m_ChainGunFireTimer.Reset();
                m_ChainGunFireTimer.Start();
            }
        }
    }
    else
    {
        if ( m_NextWeaponKeyTimer.ReadSec()
            >= (( 1 - m_Difficulty ) * MAX_WEAPON_CHANGE_TIME) )
        {
            m_NextWeaponKeyTimer.Reset();
            m_NextWeaponKeyTimer.Start();

            // we need to advance to the next weapon
            m_Move.NextWeaponKey = TRUE;
        }
        
        // Stop firing to save ammo
        m_Move.FireKey = FALSE;
    }

    //--------------------------------------------------
    // Check to see if a grenade is called-for
    //--------------------------------------------------
    const vector3& TargetPos = Target.GetPosition();
    const vector3& TargetCenter = GetTargetCenter( Target );
    
    const xbool HaveAirSuperiority = ((m_WorldPos.Y - TargetPos.Y)
        > AIR_SUPERIORITY_HEIGHT_DIFFERENCE);

    player_object::invent_type GrenadeType;
    
    if (GetInventCount(INVENT_TYPE_GRENADE_BASIC) > 0)
    {
        GrenadeType = INVENT_TYPE_GRENADE_BASIC;
    }
    else if (GetInventCount(INVENT_TYPE_GRENADE_FLASH) > 0)
    {
        ASSERT( FALSE ); // We shouldn't be getting any flash grenades.
        GrenadeType = INVENT_TYPE_GRENADE_FLASH;
    }
    else if (GetInventCount(INVENT_TYPE_GRENADE_CONCUSSION) > 0)
    {
        GrenadeType = INVENT_TYPE_GRENADE_CONCUSSION;
    }
    else
    {
        GrenadeType = INVENT_TYPE_NONE;
    }
    
    if ( GrenadeType != INVENT_TYPE_NONE
        && GrenadeType != INVENT_TYPE_GRENADE_FLARE
        && m_WillThrowGrenades
        && HaveAirSuperiority
        && (v3_AngleBetween( vector3(m_Rot), TargetCenter - m_WorldPos )
            < R_5)
        && ((TargetCenter - m_WorldPos).LengthSquared() < (40.0f * 40.0f))
        && CanSplashDamage( Target )
        && (!WouldSplashSelf( GrenadeType, vector3(m_Rot) ) || Defending()) )
    {
        // Throw a grenade
        m_Move.GrenadeKey = TRUE;
    }
}

//==============================================================================
// IsPlayerCloserVisibleEnemy()
//
// Service method for GetNearestVisibleEnemy()
//==============================================================================
xbool bot_object::IsPlayerCloserVisibleEnemy( f32& NearestDistSQ
    , const player_object& TestPlayer )
{
    if ( !(GetTeamBits() & TestPlayer.GetTeamBits())
        && this != &TestPlayer )
    {
        // enemy, so check distance...
        const f32 DistSQ
            = (GetPos() - TestPlayer.GetPos()).LengthSquared();
            
        if ( DistSQ < NearestDistSQ )
        {
            // closest so far, so check visibility...
            collider            Ray;
            collider::collision Collision;
            Ray.RaySetup    ( NULL, 
                              GetMuzzlePosition(), 
                              TestPlayer.GetBBox().GetCenter(), 
                              m_ObjectID.GetAsS32(), TRUE );
            Ray.Exclude(TestPlayer.GetObjectID().GetAsS32()) ;
            ObjMgr.Collide  ( ATTR_SOLID_STATIC, Ray );
            Ray.GetCollision(Collision) ;

            if ( !Collision.Collided )
            {
                NearestDistSQ = DistSQ;
                return TRUE;
            }
        }
    }

    return FALSE;
}

//==============================================================================
// GetNearestEnemy()
//
//==============================================================================
player_object*  bot_object::GetNearestEnemy( void ) const
{
    //----------------------------------------------------------------------
    // Look first for nearest enemy player, then for nearest enemy bot
    //----------------------------------------------------------------------
    player_object* NearestEnemy = NULL;
    f32 NearestDistSQ = F32_MAX;
    player_object* TestPlayer;

    ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
    while ( (TestPlayer = (player_object*)(ObjMgr.GetNextInType())) != NULL )
    {
        if ( !(GetTeamBits() & TestPlayer->GetTeamBits())
            && ((GetPos() - TestPlayer->GetPos()).LengthSquared()
                < NearestDistSQ) )
        {
            NearestDistSQ = (GetPos() - TestPlayer->GetPos()).LengthSquared();
            NearestEnemy = TestPlayer;
        }
    }
    ObjMgr.EndTypeLoop();

    //----------------------------------------------------------------------
    // Now for the nearest enemy bot
    //----------------------------------------------------------------------
    ObjMgr.StartTypeLoop( object::TYPE_BOT );
    while ( (TestPlayer = (player_object*)(ObjMgr.GetNextInType())) != NULL )
    {
        if ( !(GetTeamBits() & TestPlayer->GetTeamBits())
            && ((GetPos() - TestPlayer->GetPos()).LengthSquared()
                < NearestDistSQ)
            && (s32)this != (s32)TestPlayer )
        {
            NearestDistSQ = (GetPos() - TestPlayer->GetPos()).LengthSquared();
            NearestEnemy = TestPlayer;
        }
    }
    ObjMgr.EndTypeLoop();

    return NearestEnemy;
}

//==============================================================================
// MortarTarget()
// returns true if we actually got an aim location
//==============================================================================
xbool bot_object::MortarTarget( const object& Target )
{
    m_RequestedWeaponSlot
        = WeaponInSlot( player_object::INVENT_TYPE_WEAPON_MORTAR_GUN );

    if ( m_RequestedWeaponSlot == -1 )
    {
        m_RequestedWeaponSlot = 0;
        ASSERT( 0 ); // Trying to fire mortar without mortar gun
    }

    // set up aiming
    m_Aiming = TRUE;
    m_AimingTarget = Target.GetObjectID();

    m_Move.FireKey = FALSE;

    if ( m_WeaponCurrentSlot == m_RequestedWeaponSlot )
    {
        m_Move.NextWeaponKey = FALSE; // make sure other states don't mess up

        if ( FacingAimDirection() )
            /*
            && CanHitTargetArcing(
                player_object::INVENT_TYPE_WEAPON_MORTAR_GUN
                , Target) )
                */
        {
            m_Move.FireKey = TRUE;
        }
    }
    else
    {
        m_Move.NextWeaponKey = TRUE;
    }

    return (m_AimDirection.LengthSquared() > 0.00001f);
}

//==============================================================================
// RepairTarget()
//==============================================================================
void bot_object::RepairTarget( const object& Target )
{
    FaceLocation( Target.GetPosition() );
    if( m_WeaponCurrentType != INVENT_TYPE_WEAPON_REPAIR_GUN
        && (m_RepairPackTimer.ReadSec() > 0.5f) )
    {
        m_Move.PackKey = TRUE;
        m_RepairPackTimer.Reset();
        m_RepairPackTimer.Start();
    }
    else
    {
        m_Move.FireKey = (v3_AngleBetween( vector3(m_Rot), m_FaceDirection ) < R_5);
    }
}

//==============================================================================
// GetMuzzlePosition()
//==============================================================================
vector3 bot_object::GetMuzzlePosition( void ) const
{
    return GetWeaponFirePos() ;
}

//==============================================================================
// CanHitTarget()
//==============================================================================
xbool bot_object::CanHitTarget( const object& Target )
{
    vector3 Bullseye;
    GetTargetBullseye(
        Bullseye,
        Target,
        m_Loadout.Weapons[m_RequestedWeaponSlot] );

    const f32 DistSQ = (m_WorldPos - Bullseye).LengthSquared();
    if (  (DistSQ > x_sqr( 300.0f ))
        || (DistSQ > x_sqr( tgl.Fog.GetVisDistance() )) )
    {
        return false;
    }

//  const vector3 Offset( 0.0f, 0.1f, 0.0f );

#ifdef nmreed
    static s32 LastFrameCalled = 0;
    static s32 nCallsThisFrame = 0;
    static s32 TotalCalls = 0;
    static s32 TotalFrames = 0;
    static s32 MaxCallsPerFrame = 0;
    

    if ( LastFrameCalled != g_BotFrameCount )
    {
        TotalFrames += (g_BotFrameCount - LastFrameCalled);
        LastFrameCalled = g_BotFrameCount;
        TotalCalls += nCallsThisFrame;

        if ( nCallsThisFrame > MaxCallsPerFrame )
        {
            MaxCallsPerFrame = nCallsThisFrame;
        }
        
        nCallsThisFrame = 0;

        if ( TotalFrames % 100 == 0 )
        {
            const f32 Avg = TotalCalls / TotalFrames;
            x_printf( "CanHitTarget Max:%i Avg:%f\n", MaxCallsPerFrame, Avg );
        }
    }
    else
    {
        ++nCallsThisFrame;
    }
#endif

    // NOTE: using this vector rather than actual muzzle pos because the
    // caller of the function may have temporarily changed worldpos to see
    // if the target could be if if the bot were standing in a certain
    // location. The fireposition is cached and will not be modified by
    // simply changing m_WorldPos. So, this vector 1.5 meters above our
    // feet should work well enough.
    const vector3 MuzzlePos = m_WorldPos + vector3(0.0f, 1.5f, 0.0f );
    
    // Points too close?
    if ( (MuzzlePos - Bullseye).LengthSquared() < collider::MIN_RAY_DIST_SQR)
        return TRUE ;

    // SteveB : Speedup - used line of sight ray
    collider            Ray;
    collider::collision Collision;
    Ray.RaySetup    ( NULL, MuzzlePos, Bullseye, m_ObjectID.GetAsS32(), TRUE );
    Ray.Exclude(Target.GetObjectID().GetAsS32()) ;
    ObjMgr.Collide  ( ATTR_SOLID_STATIC, Ray );
    Ray.GetCollision(Collision) ;

    if ( Collision.Collided == FALSE )
    {
        return TRUE;
    }
    else
    {
        return (Collision.Point - MuzzlePos).LengthSquared() >= DistSQ;
    }
    
    // If collided with the target, then we 
    //ClearShot = PointsCanSeeEachOther(
            //MuzzlePos
            //, Bullseye
            //, ATTR_SOLID_STATIC
            //, Collision );

    //object* pHitObject = (object*)(Collision.pObjectHit);

    //if ( ClearShot
        //|| (pHitObject->GetObjectID()
            //== Target.GetObjectID()) )
    //{
       //return TRUE;
    //}
    
    //return FALSE;
}

//==============================================================================
// CanHitTargetArcing()
//==============================================================================
xbool bot_object::CanHitTargetArcing( player_object::invent_type WeaponType
    , const object& Target )
{
    const f32 DistSQ = (m_WorldPos - Target.GetPosition()).LengthSquared();
    if (  (DistSQ > x_sqr( 300.0f ))
        || (DistSQ > x_sqr( tgl.Fog.GetVisDistance() )) )
    {
        return false;
    }
    
    ASSERT( WeaponType == INVENT_TYPE_WEAPON_GRENADE_LAUNCHER
        || WeaponType == INVENT_TYPE_WEAPON_MORTAR_GUN );

    //--------------------------------------------------
    // Get the aim direction
    //--------------------------------------------------
    vector3 AimDirection;
    vector3 AimPosition;
    vector3 TargetCenter;
    GetTargetBullseye( TargetCenter, Target, WeaponType );

    // NOTE: using this vector rather than actual muzzle pos because the
    // caller of the function may have temporarily changed worldpos to see
    // if the target could be if if the bot were standing in a certain
    // location. The fireposition is cached and will not be modified by
    // simply changing m_WorldPos. So, this vector 1.5 meters above our
    // feet should work well enough.
    const vector3 MuzzlePosition = m_WorldPos + vector3(0.0f, 1.5f, 0.0f );
    const weapon_info& WeaponInfo = GetWeaponInfo( WeaponType );

    const xbool InRange = CalculateArcingAimDirection(
        TargetCenter
        , vector3( 0.0f, 0.0f, 0.0f )
        , MuzzlePosition
        , m_Vel
        , WeaponInfo.VelocityInheritance
        , WeaponInfo.MuzzleSpeed
        , WeaponInfo.MaxAge * 1000
        , AimDirection
        , AimPosition );

    if ( !InRange )
    {
        return FALSE;
    }

    //--------------------------------------------------
    // Now check to see where the impact would be
    //--------------------------------------------------
    vector3 ImpactPos;
    object::id ObjectHitID;

    CalculateArcingImpactPos(
        MuzzlePosition
        , (AimDirection * WeaponInfo.MuzzleSpeed) + m_Vel
        , ImpactPos
        , ObjectHitID
        , this
        , m_ObjectID.GetAsS32()
        , arcing::GetGravity() );

    return ObjectHitID == Target.GetObjectID()
        || (ImpactPos - TargetCenter).LengthSquared() <= 20.0f
        || ((ImpactPos - m_WorldPos).LengthSquared()
            > (TargetCenter - m_WorldPos).LengthSquared()) ;
}

//==============================================================================
// GetTargetBullseye()
//==============================================================================
void bot_object::GetTargetBullseye( vector3& Bullseye
    , const object& Target
    , const bot_object::invent_type& WeaponType )
{
    switch ( Target.GetType() )
    {
    case object::TYPE_TURRET_FIXED:
    {
        Bullseye = Target.GetPosition();
        Bullseye.Y = Target.GetBBox().GetCenter().Y;
        break;
    }
        
    case object::TYPE_TURRET_SENTRY:
    case object::TYPE_TURRET_CLAMP:
    case object::TYPE_MINE:
        Bullseye = Target.GetPosition();
        break;

    default:
        Bullseye = GetTargetCenter( Target );
    }
    
    // modify bullseye based on weapon type
    switch ( WeaponType )
    {
    // "Instant weapons
    case INVENT_TYPE_WEAPON_SNIPER_RIFLE:
        if ( Target.GetType() == object::TYPE_PLAYER
            || Target.GetType() == object::TYPE_BOT )
        {
            // Aim for the face
            player_object* pPlayer = (player_object*)(&Target);
            vector3 ViewPos;
            radian3 Temp1;
            f32     Temp2;
            pPlayer->Get1stPersonView( ViewPos, Temp1, Temp2 );
            
            Bullseye.Y = ViewPos.Y;
        }
        break;
        
    case INVENT_TYPE_WEAPON_DISK_LAUNCHER:
    case INVENT_TYPE_WEAPON_PLASMA_GUN:
    {
        // Check to see if we should adjust for splash damage
        if ( (Target.GetType() == object::TYPE_PLAYER
                || Target.GetType() == object::TYPE_BOT)
            && CanSplashDamage( Target ) )
        {
            // shoot at the base of the object
            Bullseye.Y = Target.GetBBox().Min.Y;
        }
        break;
    }
        
    case INVENT_TYPE_WEAPON_REPAIR_GUN:
        // shoot near the base of the object
        Bullseye.Y = Target.GetBBox().Min.Y;
        break;

    default:
        // do nothing
        break;
    }
    
}

//==============================================================================
// WouldSplashSelf()
//==============================================================================
xbool bot_object::WouldSplashSelf( invent_type WeaponType
    , const vector3& AimDirection )
{
    if ( InsideBuilding() )
    {
        return FALSE; // Don't CARE
    }

    vector3 CheckDir = AimDirection;
    CheckDir.Normalize();
    
#ifdef nmreed
        static s32 LastFrameCalled = 0;
        static s32 nCallsThisFrame = 0;
        static s32 TotalCalls = 0;
        static s32 TotalFrames = 0;
        static s32 MaxCallsPerFrame = 0;
    

        if ( LastFrameCalled != g_BotFrameCount )
        {
            TotalFrames += (g_BotFrameCount - LastFrameCalled);
            LastFrameCalled = g_BotFrameCount;
            TotalCalls += nCallsThisFrame;

            if ( nCallsThisFrame > MaxCallsPerFrame )
            {
                MaxCallsPerFrame = nCallsThisFrame;
            }
        
            nCallsThisFrame = 0;

            if ( TotalFrames % 100 == 0 )
            {
                const f32 Avg = (f32)TotalCalls / (f32)TotalFrames;
                x_printf( "WouldSplashSelf Max:%i Avg:%5.2f\n", MaxCallsPerFrame, Avg );
            }
        }
        else
        {
            ++nCallsThisFrame;
        }
#endif

    // check for a collision
    collider::collision Collision;
    const vector3 MuzzlePosition = GetMuzzlePosition();
    const xbool ClearShot = PointsCanSeeEachOther( MuzzlePosition
        , MuzzlePosition + (CheckDir * 15.0f )
        , ATTR_SOLID_STATIC
        , Collision );

    if ( ClearShot )
    {
        return FALSE;
    }
    else
    {
        return SplashTooClose( WeaponType, Collision.Point );
    }
}

//==============================================================================
// SplashTooClose()
//==============================================================================
xbool bot_object::SplashTooClose( invent_type WeaponType
    , const vector3& CollisionPoint )
{
        const vector3 ToTarget = CollisionPoint - m_WorldPos;
        const f32 TargetDistSQ = ToTarget.LengthSquared();
        const f32 DamageRadiusSQ = x_sqr( ObjMgr.GetDamageRadius(
            GetWeaponPainType( WeaponType ) ) );
        return (TargetDistSQ <= DamageRadiusSQ);
}








