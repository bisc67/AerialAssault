//==============================================================================
//
//  LaserRifle.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "Demo1/Globals.hpp"
#include "Objects/Projectiles/laser.hpp"


//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME    0.5f
#define MIN_LASERRIFLE_ENERGY    5.0f

//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::LaserRifle_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		LaserRifle_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			LaserRifle_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			LaserRifle_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		LaserRifle_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	LaserRifle_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::LaserRifle_ExitState()
{
    // Call exit state functions of current state
    switch(m_WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:										   ; break ; 
	    case WEAPON_STATE_IDLE:			LaserRifle_Exit_IDLE()             ; break ; 
	    case WEAPON_STATE_FIRE:											   ; break ; 
	    case WEAPON_STATE_RELOAD:										   ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
    }
}

//==============================================================================

// Sets up a new state
void player_object::LaserRifle_SetupState(weapon_state WeaponState)
{
	// Call setup function
	switch(WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		LaserRifle_Setup_ACTIVATE	() ; break ; 
		case WEAPON_STATE_IDLE:			LaserRifle_Setup_IDLE		() ; break ; 
		case WEAPON_STATE_FIRE:			LaserRifle_Setup_FIRE		() ; break ; 
		case WEAPON_STATE_RELOAD:		LaserRifle_Setup_RELOAD		() ; break ; 
		case WEAPON_STATE_DEACTIVATE:	LaserRifle_Setup_DEACTIVATE	() ; break ; 
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

void player_object::LaserRifle_Setup_ACTIVATE()
{
	// Play sound!
    if (m_ProcessInput)
    {
    	audio_Play(SFX_WEAPONS_SNIPERRIFLE_ACTIVATE) ;
    }
    else
    {
    	audio_Play(SFX_WEAPONS_SNIPERRIFLE_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::LaserRifle_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::LaserRifle_Setup_IDLE()
{
    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::LaserRifle_Exit_IDLE()
{
}

void player_object::LaserRifle_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::LaserRifle_Setup_FIRE()
{
    ASSERT(m_WeaponInfo) ;

    // Enough energy?
    if (m_Energy >= m_WeaponInfo->AmmoNeededToAllowFire)
    {
        // Only create the laser blast on the server
        if (tgl.ServerPresent)
        {
		    // Fire!
		    laser* pLaser = (laser*)ObjMgr.CreateObject( object::TYPE_LASER );
		    pLaser->Initialize( GetWeaponFirePos(), MIN(100.0f, (m_Energy*100.0f)), m_TeamBits, m_ObjectID );
            ObjMgr.AddObject( pLaser, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

        // Call default
        Weapon_Setup_FIRE() ;
    }
}

void player_object::LaserRifle_Advance_FIRE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_FIRE(DeltaTime) ;
}


//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::LaserRifle_Setup_RELOAD()
{
}

void player_object::LaserRifle_Advance_RELOAD(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::LaserRifle_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::LaserRifle_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
