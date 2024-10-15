//==============================================================================
//
//  GrenadeLauncher.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "Demo1/Globals.hpp"
#include "Objects/Projectiles/Grenade.hpp"
#include "Objects/Projectiles/AutoAim.hpp"


//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME    0.5f



//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::GrenadeLauncher_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		GrenadeLauncher_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			GrenadeLauncher_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			GrenadeLauncher_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		GrenadeLauncher_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	GrenadeLauncher_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::GrenadeLauncher_ExitState()
{
    // Call exit state functions of current state
    switch(m_WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:										   ; break ; 
	    case WEAPON_STATE_IDLE:			GrenadeLauncher_Exit_IDLE()           ; break ; 
	    case WEAPON_STATE_FIRE:											   ; break ; 
	    case WEAPON_STATE_RELOAD:										   ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
    }
}

//==============================================================================

// Sets up a new state
void player_object::GrenadeLauncher_SetupState(weapon_state WeaponState)
{
    // Call setup function
    switch(WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:		GrenadeLauncher_Setup_ACTIVATE	    () ; break ; 
	    case WEAPON_STATE_IDLE:			GrenadeLauncher_Setup_IDLE		    () ; break ; 
	    case WEAPON_STATE_FIRE:			GrenadeLauncher_Setup_FIRE		    () ; break ; 
	    case WEAPON_STATE_RELOAD:		GrenadeLauncher_Setup_RELOAD		() ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	GrenadeLauncher_Setup_DEACTIVATE	() ; break ; 
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

void player_object::GrenadeLauncher_Setup_ACTIVATE()
{
    // Stop texture animation
    m_WeaponInstance.SetTexturePlaybackType( material::PLAYBACK_TYPE_ONCE_ONLY );
    m_WeaponInstance.SetTextureAnimFPS(0);
    m_WeaponInstance.SetTextureFrame(100) ;

	// Play sound!
    if (m_ProcessInput)
    {
        input_Feedback(0.25f,0.4f,m_ControllerID);
	    audio_Play(SFX_WEAPONS_GRENADELAUNCHER_ACTIVATE) ;
    }
    else
	    audio_Play(SFX_WEAPONS_GRENADELAUNCHER_ACTIVATE, &m_WorldPos) ;


    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::GrenadeLauncher_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::GrenadeLauncher_Setup_IDLE()
{
    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::GrenadeLauncher_Exit_IDLE()
{

}

void player_object::GrenadeLauncher_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::GrenadeLauncher_Setup_FIRE()
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
	    // Only create the grenade on the server
	    if (tgl.ServerPresent)
	    {
		    // Fire!
            grenade* pGrenade = (grenade*)ObjMgr.CreateObject( object::TYPE_GRENADE );

            xbool   GotDir = FALSE;            
            vector3 TargetVec;

            if( UseAutoAimHelp )
            {
                vector3     TargetPos;
                object::id  TargetID;
                
                GotDir = GetAutoAimPoint( this, TargetVec, TargetPos, TargetID );
            }
            
            if( !GotDir )
            {
                TargetVec = GetWeaponFireDir();
            }
            
            pGrenade->Initialize( GetWeaponFirePos(), 
                                  TargetVec, 
                                  m_Vel,
                                  m_ObjectID, 
                                  m_TeamBits, 
                                  GetWeaponInfo( INVENT_TYPE_WEAPON_GRENADE_LAUNCHER ).MuzzleSpeed, 
                                  grenade::TYPE_LAUNCHED );
            ObjMgr.AddObject( pGrenade, obj_mgr::AVAILABLE_SERVER_SLOT );
        }    
    }

    // Call default
    Weapon_Setup_FIRE() ;
}

void player_object::GrenadeLauncher_Advance_FIRE(f32 DeltaTime)
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

void player_object::GrenadeLauncher_Setup_RELOAD()
{
    if ( (m_ProcessInput) &&
            GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire )
	    audio_Play(SFX_WEAPONS_GRENADELAUNCHER_RELOAD) ;
    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::GrenadeLauncher_Advance_RELOAD(f32 DeltaTime)
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

void player_object::GrenadeLauncher_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::GrenadeLauncher_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
