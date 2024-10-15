//==============================================================================
//
//  PlasmaGun.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "Demo1/Globals.hpp"
#include "Objects/Projectiles/Plasma.hpp"


//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME    0.5f



//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::PlasmaGun_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		PlasmaGun_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			PlasmaGun_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			PlasmaGun_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		PlasmaGun_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	PlasmaGun_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::PlasmaGun_ExitState()
{
    // Call exit state functions of current state
    switch(m_WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:										   ; break ; 
	    case WEAPON_STATE_IDLE:			    PlasmaGun_Exit_IDLE()          ; break ; 
	    case WEAPON_STATE_FIRE:											   ; break ; 
	    case WEAPON_STATE_RELOAD:										   ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
    }
}

//==============================================================================

// Sets up a new state
void player_object::PlasmaGun_SetupState(weapon_state WeaponState)
{
    // Call setup function
    switch(WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:		PlasmaGun_Setup_ACTIVATE	() ; break ; 
	    case WEAPON_STATE_IDLE:			PlasmaGun_Setup_IDLE		() ; break ; 
	    case WEAPON_STATE_FIRE:			PlasmaGun_Setup_FIRE		() ; break ; 
	    case WEAPON_STATE_RELOAD:		PlasmaGun_Setup_RELOAD		() ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	PlasmaGun_Setup_DEACTIVATE	() ; break ; 
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

void player_object::PlasmaGun_Setup_ACTIVATE()
{
    // Stop texture animation
    m_WeaponInstance.SetTexturePlaybackType( material::PLAYBACK_TYPE_ONCE_ONLY );
    m_WeaponInstance.SetTextureAnimFPS(0);
    m_WeaponInstance.SetTextureFrame(100) ;

	// Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_PLASMA_ACTIVATE) ;
        input_Feedback(0.25f,0.3f,m_ControllerID);
    }
    else
    {
        audio_Play(SFX_WEAPONS_PLASMA_ACTIVATE, &m_WorldPos) ;
    }


    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::PlasmaGun_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::PlasmaGun_Setup_IDLE()
{
    // Start sound
   	audio_PlayLooping(m_WeaponSfxID, SFX_WEAPONS_PLASMA_IDLE_LOOP, &m_WorldPos) ;

    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::PlasmaGun_Exit_IDLE()
{
    // Stop sound
    audio_StopLooping(m_WeaponSfxID) ;
}

void player_object::PlasmaGun_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::PlasmaGun_Setup_FIRE()
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
	    // Only create the plasma on the server
	    if (tgl.ServerPresent)
	    {
		    // Fire!
		    plasma* pPlasma = (plasma*)ObjMgr.CreateObject( object::TYPE_PLASMA );
		    pPlasma->Initialize( GetTweakedWeaponFireL2W(), m_TeamBits, m_ObjectID );
            ObjMgr.AddObject( pPlasma, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

    }

    // Call default
    Weapon_Setup_FIRE() ;
}

void player_object::PlasmaGun_Advance_FIRE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_FIRE(DeltaTime) ;

    // Start texture animation with delay
    if (m_WeaponStateTime > 0.5f)
    {
        m_WeaponInstance.SetTextureAnimFPS(20);
        m_WeaponInstance.SetTextureFrame(0) ;
    }
}


//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::PlasmaGun_Setup_RELOAD()
{
    if ( (m_ProcessInput) &&
            GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire )
	    audio_Play(SFX_WEAPONS_PLASMA_RELOAD) ;
    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::PlasmaGun_Advance_RELOAD(f32 DeltaTime)
{
    // Play sound!
    //if (        (m_WeaponLastStateTime < RELOAD_SFX_DELAY_TIME)
      //      &&  (m_WeaponStateTime >= RELOAD_SFX_DELAY_TIME) )

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::PlasmaGun_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::PlasmaGun_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
