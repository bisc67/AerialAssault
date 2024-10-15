//==============================================================================
//
//  Blaster.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "AADS/Globals.hpp"
#include "Objects/Projectiles/Blaster.hpp"


//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME    0.5f



//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::Blaster_AdvanceState(f32 DeltaTime)
{
    // Set spin speed
    m_WeaponInstance.GetCurrentAnimState().SetSpeed(0.5f) ;

	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		Blaster_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			Blaster_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			Blaster_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		Blaster_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	Blaster_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::Blaster_ExitState()
{
    // Call exit state functions of current state
    switch(m_WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:										   ; break ; 
	    case WEAPON_STATE_IDLE:			    Blaster_Exit_IDLE()          ; break ; 
	    case WEAPON_STATE_FIRE:											   ; break ; 
	    case WEAPON_STATE_RELOAD:										   ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
    }
}

//==============================================================================

// Sets up a new state
void player_object::Blaster_SetupState(weapon_state WeaponState)
{
    // Call setup function
    switch(WeaponState)
    {
	    case WEAPON_STATE_ACTIVATE:		Blaster_Setup_ACTIVATE	() ; break ; 
	    case WEAPON_STATE_IDLE:			Blaster_Setup_IDLE		() ; break ; 
	    case WEAPON_STATE_FIRE:			Blaster_Setup_FIRE		() ; break ; 
	    case WEAPON_STATE_RELOAD:		Blaster_Setup_RELOAD		() ; break ; 
	    case WEAPON_STATE_DEACTIVATE:	Blaster_Setup_DEACTIVATE	() ; break ; 
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

void player_object::Blaster_Setup_ACTIVATE()
{
    // Stop texture animation
    m_WeaponInstance.SetTexturePlaybackType( material::PLAYBACK_TYPE_LOOP );
    
    m_WeaponSpinSpeed = 0.0f;
	
    // Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_BLASTER_ACTIVATE) ;
    }
    else
    {
        audio_Play(SFX_WEAPONS_BLASTER_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::Blaster_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::Blaster_Setup_IDLE()
{
    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::Blaster_Exit_IDLE()
{
    audio_StopLooping(m_WeaponSfxID) ;
}

void player_object::Blaster_Advance_IDLE(f32 DeltaTime)
{

    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::Blaster_Setup_FIRE()
{
    ASSERT(m_WeaponInfo) ;

    // Out of ammo?
    if (GetWeaponAmmoCount(m_WeaponCurrentType) < m_WeaponInfo->AmmoNeededToAllowFire)
    {
	    // Play sound only for controlling player
        if (m_ProcessInput)
	        audio_Play(SFX_WEAPONS_OUT_OF_AMMO_ALL) ;
    }
    else
    {
	    // Only create the blaster projectile on the server
	    if (tgl.ServerPresent)
	    {
		    // Fire!
		    blaster* pBlast = (blaster*)ObjMgr.CreateObject( object::TYPE_BLASTER );
		    pBlast->Initialize( GetTweakedWeaponFireL2W(), m_TeamBits, m_ObjectID );
            ObjMgr.AddObject( pBlast, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

        m_WeaponSpinSpeed = 0.6f;

        if (m_ProcessInput)
        {
            input_Feedback(0.25f,0.2f,m_ControllerID);            
        }
    }

    // Call default
    Weapon_Setup_FIRE() ;
}

void player_object::Blaster_Advance_FIRE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_FIRE(DeltaTime) ;
}


//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::Blaster_Setup_RELOAD()
{
    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::Blaster_Advance_RELOAD(f32 DeltaTime)
{
    // Play sound!
    //if (        (m_WeaponLastStateTime < RELOAD_SFX_DELAY_TIME)
      //      &&  (m_WeaponStateTime >= RELOAD_SFX_DELAY_TIME) )
    // Add the glow if necessary
//    if ( m_WeaponLastState != WEAPON_STATE_IDLE )

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::Blaster_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::Blaster_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================

void player_object::Blaster_RenderFx( void )
{
    // Add the glow if necessary (piggyback on m_WeaponSpinSpeed, no need in making yet another player variable)
    if ( m_WeaponSpinSpeed > 0.0f )
    {
        m_WeaponSpinSpeed -= tgl.DeltaLogicTime;
        m_WeaponSpinSpeed = MAX(0, m_WeaponSpinSpeed);
        if ( m_WeaponSpinSpeed < 0.6f )
        {
            f32 a = ( m_WeaponSpinSpeed / 0.6f );
            vector3 V = GetWeaponFirePos() - m_WorldPos + m_DrawPos;
            tgl.PostFX.AddFX( V, vector2(0.5f, 0.5f), xcolor(255,255,255,(u8)(64*a)) );
        }
    }
}

//==============================================================================
