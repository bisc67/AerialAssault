//==============================================================================
//
//  MissileLauncher.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "demo1/gameserver.hpp"
#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "Demo1/Globals.hpp"
#include "Objects/Projectiles/Missile.hpp"
#include "HUD\hud_Icons.hpp"
#include "Objects\Vehicles\Vehicle.hpp"

//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME     0.5f

#define MAX_RANGE               400.0f
#define MIN_RANGE                40.0f
#define MAX_RANGE_SQ            ( MAX_RANGE * MAX_RANGE )
#define MIN_RANGE_SQ            ( MIN_RANGE * MIN_RANGE )
#define MID_RADIUS              ( (MAX_RANGE - MIN_RANGE) / 2.0f )
#define MID_DIST                ( (MAX_RANGE + MIN_RANGE) / 2.0f )

#define BEACON_LOCK             R_15

//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::MissileLauncher_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		MissileLauncher_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			MissileLauncher_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			MissileLauncher_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		MissileLauncher_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	MissileLauncher_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::MissileLauncher_ExitState()
{
	// Call exit state functions of current state
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:										   ; break ; 
		case WEAPON_STATE_IDLE:			MissileLauncher_Exit_IDLE()           ; break ; 
		case WEAPON_STATE_FIRE:											   ; break ; 
		case WEAPON_STATE_RELOAD:										   ; break ; 
		case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::MissileLauncher_SetupState(weapon_state WeaponState)
{
	// Call setup function
	switch(WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		MissileLauncher_Setup_ACTIVATE	    () ; break ; 
		case WEAPON_STATE_IDLE:			MissileLauncher_Setup_IDLE		    () ; break ; 
		case WEAPON_STATE_FIRE:			MissileLauncher_Setup_FIRE		    () ; break ; 
		case WEAPON_STATE_RELOAD:		MissileLauncher_Setup_RELOAD		() ; break ; 
		case WEAPON_STATE_DEACTIVATE:	MissileLauncher_Setup_DEACTIVATE	() ; break ; 
	}
}

//==============================================================================
//
// STATE FUNCTIONS
//
//==============================================================================

//==============================================================================
// WEAPON_STATE_ACTIVATE
//==============================================================================

void player_object::MissileLauncher_Setup_ACTIVATE()
{
	// Play sound!
    if (m_ProcessInput)
    {
	    audio_Play(SFX_WEAPONS_MISSLELAUNCHER_ACTIVATE) ;
    }
    else
    {
	    audio_Play(SFX_WEAPONS_MISSLELAUNCHER_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::MissileLauncher_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::MissileLauncher_Setup_IDLE()
{
    // Call default
    Weapon_Setup_IDLE() ;
    
    SetWeaponTargetID( obj_mgr::NullID );
    SetMissileLock(FALSE) ;

    // Clear sfx id's
    m_MissileFirerSearchSfxID = -1 ;
    m_MissileFirerLockSfxID   = -1 ;
}

void player_object::MissileLauncher_Exit_IDLE()
{
    // Stop sounds
    audio_StopLooping(m_MissileFirerSearchSfxID) ;
    audio_StopLooping(m_MissileFirerLockSfxID) ;
}

void player_object::MissileLauncher_Advance_IDLE(f32 DeltaTime)
{
    // look for hot target twice per second

    if ( tgl.ServerPresent &&
         ( m_WeaponStateTime > 0.25f ) && 
         ( GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire) )
    {
        vector3                 ViewPos;
        radian3                 ViewRot;
        f32                     ViewFOV;
        collider::collision     Collision;

        // get the view
        GetView( ViewPos, ViewRot, ViewFOV );

        vector3 ViewDir(0.0f, 0.0f, MID_DIST);

        ViewDir.Rotate( ViewRot );
        ViewDir += ViewPos;

        // find the hot objects in my LOS
        ObjMgr.Select( object::ATTR_HOT, 
                        ViewDir,
                        MID_RADIUS );

        object*     pObject;
        object*     pClosestObj = NULL;
        radian      Closest     = BEACON_LOCK;

        while( (pObject = ObjMgr.GetNextSelected()) )
        {

            if ( pObject->GetHealth() <= 0.0f )
                continue;

            vector3 BBPos = pObject->GetBBox().GetCenter();
            
            vector3 Tmp = ( BBPos - m_WorldPos );
            f32 LenSq = Tmp.LengthSquared();

      
            if ( ( LenSq < MAX_RANGE_SQ ) && ( LenSq > MIN_RANGE_SQ ) )
            {
                // check the angle
                radian DifAng = v3_AngleBetween( (ViewDir-ViewPos), Tmp );

                if ( ( pObject->GetType() == TYPE_BEACON ) && ( pObject->GetTeamBits() & m_TeamBits ) )
                {
                    if ( DifAng < BEACON_LOCK )
                    {
                        // No LOS check on beacons
                        if ( DifAng < Closest )
                        {
                            Closest = DifAng;
                            pClosestObj = pObject;
                        }
                    }
                }
                else
                {
                    if ( (DifAng < R_10 ) && (!( pObject->GetTeamBits() & m_TeamBits )) )
                    {
                        if ( DifAng < Closest )
                        {
                            // finally, check to see if we have LOS
                            s_Collider.ClearExcludeList() ;
                            s_Collider.RaySetup( this, ViewPos, BBPos, GetObjectID().GetAsS32(), TRUE );
                            ObjMgr.Collide  ( ATTR_SOLID, s_Collider );

                            if ( s_Collider.HasCollided() )
                            {
                                s_Collider.GetCollision( Collision );

                                // ignore if the only thing we collide with is the target obj
                                if ( Collision.pObjectHit == pObject )
                                {
                                    Closest = DifAng;
                                    pClosestObj = pObject;
                                }
                            }
                            else
                            {
                                Closest = DifAng;
                                pClosestObj = pObject;
                            }
                        }
                    }
                }
            }
        }
        ObjMgr.ClearSelection();

        // Clear target if player is dead
        if (IsDead())
            pClosestObj = NULL ;

        // was there a valid target?
        if ( pClosestObj != NULL )
        {
            // Tell object it's being tracked
            pClosestObj->MissileTrack() ;

            // Turn lock off
            if ( ( m_WeaponTargetID == obj_mgr::NullID ) || ( pClosestObj->GetObjectID() != m_WeaponTargetID ) )
            {
                // reset the state timer and record the target
                m_WeaponStateTime = 0.0f;
                SetWeaponTargetID( pClosestObj->GetObjectID() );
                SetMissileLock(FALSE) ;
            }
            else
            {
                // check the time to see if it has been 1 second yet
                if ( ( m_WeaponStateTime > 0.5f ) && ( m_MissileLock == FALSE ) )
                    SetMissileLock(TRUE) ;
            }
        }
        else
        {
            // No target
            SetWeaponTargetID( obj_mgr::NullID );
            SetMissileLock(FALSE) ;
        }
    }

    // Playe firer sounds for controlling players
    if (m_ProcessInput)
    {
        // Play locked sound? (has priority over search sfx)
        if ( m_MissileLock )
        {
            audio_PlayLooping(m_MissileFirerLockSfxID, SFX_WEAPONS_MISSLELAUNCHER_FIRER_LOCK) ;
            audio_StopLooping(m_MissileFirerSearchSfxID) ;
        }
        else
        {
            // Play search sound?
            if ( m_WeaponTargetID != obj_mgr::NullID )
            {
                audio_PlayLooping(m_MissileFirerSearchSfxID, SFX_WEAPONS_MISSLELAUNCHER_FIRER_SEARCH) ;
                audio_StopLooping(m_MissileFirerLockSfxID) ;
            }
            else
            {
                // Stop sounds
                audio_StopLooping(m_MissileFirerSearchSfxID) ;
                audio_StopLooping(m_MissileFirerLockSfxID) ;
            }
        }
    }

    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}


//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::MissileLauncher_Setup_FIRE()
{
    // Out of ammo?
    ASSERT(m_WeaponInfo) ;
    if (GetWeaponAmmoCount(m_WeaponCurrentType) < m_WeaponInfo->AmmoNeededToAllowFire)
    {
	    // Play sound only for controlling player
        if (m_ProcessInput)
	        audio_Play(SFX_WEAPONS_OUT_OF_AMMO_ALL) ;
    }
    else
    {
	    // Only create the missile on the server
	    if ( tgl.ServerPresent )
	    {
            if ( !m_MissileLock )
            {
                Weapon_SetupState( WEAPON_STATE_IDLE );
                return;
            }

		    // Fire!
		    missile* pMissile = (missile*)ObjMgr.CreateObject( object::TYPE_MISSILE );
            pMissile->Initialize( GetWeaponFireL2W(), m_TeamBits, m_ObjectID, m_WeaponTargetID );
            ObjMgr.AddObject( pMissile, obj_mgr::AVAILABLE_SERVER_SLOT );

// no more dumb fire.   :-(    JN
//            else
//                pMissile->Initialize( L2W, m_TeamBits, m_ObjectID, obj_mgr::NullID );

        }

        if (m_ProcessInput)
        {
            input_Feedback(0.25f,0.75f,m_ControllerID);
        }

    }

    // Call default
    Weapon_Setup_FIRE() ;
}

void player_object::MissileLauncher_Advance_FIRE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_FIRE(DeltaTime) ;
}


//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::MissileLauncher_Setup_RELOAD()
{
    // Play sound!
    if ( (m_ProcessInput) &&
            GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire )
        audio_Play(SFX_WEAPONS_MISSILELAUNCHER_RELOAD) ;

    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::MissileLauncher_Advance_RELOAD(f32 DeltaTime)
{

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::MissileLauncher_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::MissileLauncher_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
//  Render effects
//==============================================================================

void player_object::MissileLauncher_RenderFx( void )
{
    if( !m_HasInputFocus )
        return;

    object* pObject = ObjMgr.GetObjectFromID( m_WeaponTargetID );
    if( !pObject )
        return;

    // Tracking a target.  Get its position.

    vector3 Position = pObject->GetBlendPainPoint();

    HudIcons.Add( hud_icons::MISSILE_TRACK, Position, m_TeamBits );

    if( m_MissileLock )
    {
        HudIcons.Add( hud_icons::MISSILE_LOCK, Position, m_TeamBits );
    }
}

//==============================================================================
