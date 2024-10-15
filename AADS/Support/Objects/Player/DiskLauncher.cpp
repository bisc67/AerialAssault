//==============================================================================
//
//  DiskLauncher.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "AADS/Globals.hpp"
#include "Objects/Projectiles/Disk.hpp"


//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME    0.5f



//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::DiskLauncher_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		DiskLauncher_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			DiskLauncher_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			DiskLauncher_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		DiskLauncher_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	DiskLauncher_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::DiskLauncher_ExitState()
{
	// Call exit state functions of current state
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:										   ; break ; 
		case WEAPON_STATE_IDLE:			DiskLauncher_Exit_IDLE()           ; break ; 
		case WEAPON_STATE_FIRE:											   ; break ; 
		case WEAPON_STATE_RELOAD:										   ; break ; 
		case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::DiskLauncher_SetupState(weapon_state WeaponState)
{
	// Call setup function
	switch(WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		DiskLauncher_Setup_ACTIVATE	    () ; break ; 
		case WEAPON_STATE_IDLE:			DiskLauncher_Setup_IDLE		    () ; break ; 
		case WEAPON_STATE_FIRE:			DiskLauncher_Setup_FIRE		    () ; break ; 
		case WEAPON_STATE_RELOAD:		DiskLauncher_Setup_RELOAD		() ; break ; 
		case WEAPON_STATE_DEACTIVATE:	DiskLauncher_Setup_DEACTIVATE	() ; break ; 
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

void player_object::DiskLauncher_Setup_ACTIVATE()
{
	// Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_SPINFUSOR_ACTIVATE) ;
    }
    else
    {
        audio_Play(SFX_WEAPONS_SPINFUSOR_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::DiskLauncher_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::DiskLauncher_Setup_IDLE()
{
    // Start looping sound fx
    audio_PlayLooping(m_WeaponSfxID, SFX_WEAPONS_SPINFUSOR_IDLE_LP, &m_WorldPos) ;

    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::DiskLauncher_Exit_IDLE()
{
    // Stop
    audio_StopLooping(m_WeaponSfxID) ;
}

void player_object::DiskLauncher_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::DiskLauncher_Setup_FIRE()
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
	    // Only create the disk on the server
	    if (tgl.ServerPresent)
	    {
		    // Fire!
		    disk* pDisk = (disk*)ObjMgr.CreateObject( object::TYPE_DISK );
		    pDisk->Initialize( GetTweakedWeaponFireL2W(), m_TeamBits, m_ObjectID );
            ObjMgr.AddObject( pDisk, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

        if (m_ProcessInput)
        {
            input_Feedback(0.15f,0.4f,m_ControllerID);
        }
    }

    // Call default
    Weapon_Setup_FIRE() ;
}

void player_object::DiskLauncher_Advance_FIRE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_FIRE(DeltaTime) ;
}


//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::DiskLauncher_Setup_RELOAD()
{
    // Play sound!
    if ( (m_ProcessInput) &&
            GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire )
        audio_Play(SFX_WEAPONS_SPINFUSOR_RELOAD) ;

    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::DiskLauncher_Advance_RELOAD(f32 DeltaTime)
{

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::DiskLauncher_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::DiskLauncher_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
