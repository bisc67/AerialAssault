//==============================================================================
//
//  MortarGun.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "AADS/Globals.hpp"
#include "Objects/Projectiles/Mortar.hpp"
#include "Objects/Projectiles/AutoAim.hpp"


//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME    0.5f



//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::MortarGun_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		MortarGun_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			MortarGun_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			MortarGun_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		MortarGun_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	MortarGun_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::MortarGun_ExitState()
{
    // Call exit state functions of current state
    switch(m_WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:										     break ; 
	    case WEAPON_STATE_IDLE:			MortarGun_Exit_IDLE();               break ; 
	    case WEAPON_STATE_FIRE:											     break ; 
	    case WEAPON_STATE_RELOAD:										     break ; 
	    case WEAPON_STATE_DEACTIVATE:	                                     break ; 
    }
}

//==============================================================================

// Sets up a new state
void player_object::MortarGun_SetupState(weapon_state WeaponState)
{
    // Call setup function
    switch(WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:		MortarGun_Setup_ACTIVATE	    () ; break ; 
	    case WEAPON_STATE_IDLE:			MortarGun_Setup_IDLE		    () ; break ; 
	    case WEAPON_STATE_FIRE:			MortarGun_Setup_FIRE		    () ; break ; 
	    case WEAPON_STATE_RELOAD:		MortarGun_Setup_RELOAD		    () ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	MortarGun_Setup_DEACTIVATE	    () ; break ; 
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

void player_object::MortarGun_Setup_ACTIVATE()
{
	// Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_MORTAR_ACTIVATE) ;
        input_Feedback(0.25f,0.75f,m_ControllerID);
    }
    else
        audio_Play(SFX_WEAPONS_MORTAR_ACTIVATE, &m_WorldPos) ;

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::MortarGun_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::MortarGun_Setup_IDLE()
{
    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::MortarGun_Exit_IDLE()
{
    // Stop sfx
    audio_StopLooping(m_WeaponSfxID) ;
}

void player_object::MortarGun_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::MortarGun_Setup_FIRE()
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
	    // Only create the mortar on the server
	    if (tgl.ServerPresent)
	    {
		    // Fire!
		    mortar* pMortar = (mortar*)ObjMgr.CreateObject( object::TYPE_MORTAR );
            
            xbool GotDir = FALSE;
            
            if( UseAutoAimHelp )
            {
                vector3     TargetPos;
                vector3     TargetVec;
                object::id  TargetID;
                
                GotDir = GetAutoAimPoint( this, TargetVec, TargetPos, TargetID );
                
                if( GotDir )
                {
                    (void)TargetPos;
                    pMortar->Initialize( GetWeaponFirePos(), TargetVec, m_ObjectID, m_ObjectID, m_TeamBits );
                }
            }
            
            if( GotDir == FALSE )
            {
                pMortar->Initialize( GetWeaponFirePos(), GetWeaponFireDir(), m_ObjectID, m_ObjectID, m_TeamBits );
            }
            
            ObjMgr.AddObject( pMortar, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

	    // Play sound!
	    // audio_Play(SFX_WEAPONS_MORTAR_FIRE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_FIRE() ;
}

void player_object::MortarGun_Advance_FIRE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_FIRE(DeltaTime) ;
}


//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::MortarGun_Setup_RELOAD()
{
    // Call default
    if ( (m_ProcessInput) &&
            GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire )
	    audio_Play(SFX_WEAPONS_MORTAR_RELOAD) ;
    Weapon_Setup_RELOAD() ;
}

void player_object::MortarGun_Advance_RELOAD(f32 DeltaTime)
{
    // Play sound!
    //if (        (m_WeaponLastStateTime < RELOAD_SFX_DELAY_TIME)
            //&&  (m_WeaponStateTime >= RELOAD_SFX_DELAY_TIME) )
    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::MortarGun_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::MortarGun_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
