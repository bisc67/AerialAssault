//==============================================================================
//
//  WeaponStates.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "GameMgr/GameMgr.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "Demo1/Globals.hpp"


//==============================================================================
// DEFINES
//==============================================================================

#define NEW_WEAPON_FIRE_DELAY   0.5f    // Time to wait before enabling fire when swapping to a new weapon



//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Makes sure the correct weapon shape is displayed
void player_object::Weapon_CheckForUpdatingShape()
{
    ASSERT(m_WeaponCurrentType >= INVENT_TYPE_WEAPON_START) ;
    ASSERT(m_WeaponCurrentType <= INVENT_TYPE_WEAPON_END) ;

    ASSERT(m_WeaponRequestedType >= INVENT_TYPE_WEAPON_START) ;
    ASSERT(m_WeaponRequestedType <= INVENT_TYPE_WEAPON_END) ;

    // If you assert here, it means you messed with m_WeaponCurrentType, instead of m_WeaponRequestedType!
    ASSERT(m_WeaponInfo == &s_WeaponInfo[m_WeaponCurrentType]) ;

    // Swap to new weapon?
    if ( (m_WeaponRequestedType != m_WeaponCurrentType) || (!m_WeaponInstance.GetShape()) )
    {
        // Clear fire delay so player can fire straight away
        m_WeaponFireDelay = NEW_WEAPON_FIRE_DELAY ;

        // Send over the net...
        m_DirtyBits |= PLAYER_DIRTY_BIT_WEAPON_TYPE	;

        // Make sure exit state is called to stop looping sounds etc.
        Weapon_CallExitState() ;

        // Use new info
        m_WeaponCurrentType = m_WeaponRequestedType ;
        m_WeaponInfo        = &s_WeaponInfo[m_WeaponCurrentType] ;

        // Set new shape
        if (m_WeaponCurrentType == INVENT_TYPE_NONE)
        {
            // No weapon
            SetWeaponShape(NULL) ;
        }
        else
        {
            SetWeaponShape(tgl.GameShapeManager.GetShapeByType(s_InventInfo[m_WeaponCurrentType].ShapeType)) ;
            ASSERT(m_WeaponInstance.GetShape()) ;
        }

        // Bring on new weapon (so that exit weapon is called!)
        // NOTE: Force calling of it no matter what since the chaingun sits in activate
        //       due to no animation playing until fire is pressed!
        Weapon_SetupState(WEAPON_STATE_ACTIVATE, FALSE) ;   
    }
}

//==============================================================================

// Advance current state
void player_object::Weapon_AdvanceState(f32 DeltaTime)
{
    // Not armed?
    if (!m_Armed)
    {
        // Stop all the sounds and get out of here
        audio_StopLooping(m_WeaponSfxID) ;
        audio_StopLooping(m_WeaponSfxID2) ;
        return ;
    }

    // Keep shape up to date
    Weapon_CheckForUpdatingShape() ;

    // Update state time
    m_WeaponLastStateTime = m_WeaponStateTime ;
    m_WeaponStateTime += DeltaTime ;

    // Update weapon fire delay
    if (m_WeaponFireDelay > 0)
    {
        m_WeaponFireDelay -= DeltaTime ;
        if (m_WeaponFireDelay < 0)
            m_WeaponFireDelay = 0 ;
    }

    // Set default anim speed
    m_WeaponInstance.GetCurrentAnimState().SetSpeed(1.0f) ;

    // Call advance function
    Weapon_CallAdvanceState(DeltaTime) ;

	// Update the weapon sfx
	audio_SetPosition(m_WeaponSfxID, &m_WorldPos) ;
}

//==============================================================================

// Sets up a new weapon state
void player_object::Weapon_SetupState(weapon_state WeaponState, xbool SkipIfAlreadyInState /*=TRUE*/ )
{
    ASSERT(WeaponState >= WEAPON_STATE_START) ;
    ASSERT(WeaponState <= WEAPON_STATE_END) ;

    ASSERT(m_WeaponCurrentType >= INVENT_TYPE_WEAPON_START) ;
    ASSERT(m_WeaponCurrentType <= INVENT_TYPE_WEAPON_END) ;

    ASSERT(m_WeaponRequestedType >= INVENT_TYPE_WEAPON_START) ;
    ASSERT(m_WeaponRequestedType <= INVENT_TYPE_WEAPON_END) ;

    // If you assert here, it means you messed with m_WeaponCurrentType, instead of m_WeaponRequestedType!
    ASSERT(m_WeaponInfo == &s_WeaponInfo[m_WeaponCurrentType]) ;

    // Make sure the weapon has a shape assigned (it's need for anim setting!)
    if ((m_WeaponInstance.GetShape() == NULL) && (s_InventInfo[m_WeaponCurrentType].ShapeType != SHAPE_TYPE_NONE))
    {
        SetWeaponShape(tgl.GameShapeManager.GetShapeByType(s_InventInfo[m_WeaponCurrentType].ShapeType)) ;
        ASSERT(m_WeaponInstance.GetShape()) ;
    }

    // State changed?
    if ((m_WeaponState != WeaponState) || (!SkipIfAlreadyInState))
    {
        m_DirtyBits |= PLAYER_DIRTY_BIT_WEAPON_STATE ;

        // Call exit state functions
        Weapon_CallExitState() ;

		// Set state now, so setup function can put weapon in a different state if needed
        m_WeaponLastState = m_WeaponState ;
        m_WeaponState     = WeaponState ;

        // Call setup state function
        Weapon_CallSetupState(WeaponState) ;
    
		// Reset state time
		m_WeaponStateTime = 0.0f ;
    }
}

//==============================================================================

// Calls weapon advance function
void player_object::Weapon_CallAdvanceState(f32 DeltaTime)
{
    // Call advance function
	switch(m_WeaponCurrentType)
	{
		case INVENT_TYPE_WEAPON_DISK_LAUNCHER:		DiskLauncher_AdvanceState       (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_PLASMA_GUN:		    PlasmaGun_AdvanceState          (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_CHAIN_GUN:		    ChainGun_AdvanceState           (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_SNIPER_RIFLE:	    LaserRifle_AdvanceState         (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_MORTAR_GUN:	        MortarGun_AdvanceState          (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_GRENADE_LAUNCHER:   GrenadeLauncher_AdvanceState    (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_BLASTER:            Blaster_AdvanceState            (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_ELF_GUN:            Elf_AdvanceState                (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_REPAIR_GUN:         RepairGun_AdvanceState          (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_TARGETING_LASER:    TargetingLaser_AdvanceState     (DeltaTime) ;   break ;
        case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:   MissileLauncher_AdvanceState    (DeltaTime) ;   break ;
	}
}

//==============================================================================

// Calls weapon exit function
void player_object::Weapon_CallExitState()
{
    // Call exit state functions
    switch(m_WeaponCurrentType)
    {
	    case INVENT_TYPE_WEAPON_DISK_LAUNCHER:	   DiskLauncher_ExitState     () ; break ;
        case INVENT_TYPE_WEAPON_PLASMA_GUN:		   PlasmaGun_ExitState        () ; break ;
        case INVENT_TYPE_WEAPON_CHAIN_GUN:		   ChainGun_ExitState         () ; break ;
        case INVENT_TYPE_WEAPON_SNIPER_RIFLE:	   LaserRifle_ExitState       () ; break ;
        case INVENT_TYPE_WEAPON_MORTAR_GUN:	       MortarGun_ExitState        () ; break ;
        case INVENT_TYPE_WEAPON_GRENADE_LAUNCHER:  GrenadeLauncher_ExitState  () ; break ;
        case INVENT_TYPE_WEAPON_BLASTER:           Blaster_ExitState          () ; break ;
        case INVENT_TYPE_WEAPON_ELF_GUN:           Elf_ExitState              () ; break ;
        case INVENT_TYPE_WEAPON_REPAIR_GUN:        RepairGun_ExitState        () ; break ;
        case INVENT_TYPE_WEAPON_TARGETING_LASER:   TargetingLaser_ExitState   () ; break ;
        case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:   MissileLauncher_ExitState () ; break ;
    }
}

//==============================================================================

// Calls weapon setup function
void player_object::Weapon_CallSetupState(weapon_state WeaponState)
{
    // Call setup state function
	switch(m_WeaponCurrentType)
	{
		case INVENT_TYPE_WEAPON_DISK_LAUNCHER:	   DiskLauncher_SetupState     (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_PLASMA_GUN:		   PlasmaGun_SetupState        (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_CHAIN_GUN:		   ChainGun_SetupState         (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_SNIPER_RIFLE:	   LaserRifle_SetupState       (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_MORTAR_GUN:	       MortarGun_SetupState        (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_GRENADE_LAUNCHER:  GrenadeLauncher_SetupState  (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_BLASTER:           Blaster_SetupState          (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_ELF_GUN:           Elf_SetupState              (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_REPAIR_GUN:        RepairGun_SetupState        (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_TARGETING_LASER:   TargetingLaser_SetupState   (WeaponState) ; break ;
        case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:   MissileLauncher_SetupState (WeaponState) ; break ;
	}
}

//==============================================================================

void player_object::Weapon_RenderFx( void )
{
    // Call setup state function
	switch(m_WeaponCurrentType)
	{
        case INVENT_TYPE_WEAPON_ELF_GUN:           Elf_RenderFx                ()  ; break ;
        case INVENT_TYPE_WEAPON_REPAIR_GUN:        RepairGun_RenderFx          ()  ; break ;
        case INVENT_TYPE_WEAPON_TARGETING_LASER:   TargetingLaser_RenderFx     ()  ; break ;
        case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:  MissileLauncher_RenderFx    ()  ; break ;
        case INVENT_TYPE_WEAPON_CHAIN_GUN:         ChainGun_RenderFx           ()  ; break ;
        case INVENT_TYPE_WEAPON_BLASTER:           Blaster_RenderFx            ()  ; break ;
	}
}

//==============================================================================


//==============================================================================
//
// DEFAULT WEAPON STATE FUNCTIONS
//
//==============================================================================


//==============================================================================
// WEAPON_STATE_ACTIVATE
//==============================================================================

void player_object::Weapon_Setup_ACTIVATE()
{
    // Start anim
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_ACTIVATE, 0.25f) ;
}

void player_object::Weapon_Advance_ACTIVATE(f32 DeltaTime)
{
    (void)DeltaTime;

    // Anim done?
    if (m_WeaponInstance.GetCurrentAnimState().GetAnimPlayed())
        Weapon_SetupState(WEAPON_STATE_IDLE) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::Weapon_Setup_IDLE()
{
    // Start anim
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_IDLE, 0.25f) ;
}

void player_object::Weapon_Advance_IDLE(f32 DeltaTime)
{
    (void)DeltaTime;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::Weapon_Setup_FIRE()
{
    ASSERT(m_WeaponInfo) ;

    // Default to not fired
    xbool bFired = FALSE ;

    // Update fire delay
    m_WeaponFireDelay += m_WeaponInfo->FireDelay ;

    // Energy weapon?
    if (m_WeaponInfo->AmmoType == INVENT_TYPE_NONE)
    {
        // Not enough energy?
        if (m_Energy < m_WeaponInfo->AmmoNeededToAllowFire)
        {
            Weapon_SetupState(WEAPON_STATE_IDLE, FALSE) ;
            return ;
        }

        // Update energy
        f32 EnergyUsed = m_WeaponInfo->AmmoUsedPerFire;
        switch( m_ArmorType )
        {
        case ARMOR_TYPE_LIGHT:   EnergyUsed *= 1.00f;    break;
        case ARMOR_TYPE_MEDIUM:  EnergyUsed *= 0.75f;    break;
        case ARMOR_TYPE_HEAVY:   EnergyUsed *= 0.50f;    break;
        }
        AddEnergy(-EnergyUsed) ;

        // Do recoil anim
        bFired = TRUE ;
    }
    else
    // Only animate if weapon has ammo?
    if (GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire)
    {
        // Update ammo (only if not doing a move update since the invent isn't always sent!)
        if (!m_NetMoveUpdate)
            AddInvent(m_WeaponInfo->AmmoType, (s32)-m_WeaponInfo->AmmoUsedPerFire) ;

        // Do recoil anim
        bFired = TRUE ;
    }

    // Will weapon be fired?
    if (bFired)
    {
        // Already firing/reloading?
        if (        (m_WeaponInstance.GetAnimByType() == ANIM_TYPE_WEAPON_FIRE)
                ||  (m_WeaponInstance.GetAnimByType() == ANIM_TYPE_WEAPON_RELOAD) )
        {
            // Already firing - jump to start of anim - fixed blaster + grenade launcher
            m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_FIRE,   // Type
                                           0.0f,                    // Blend time
                                           0,                       // Start frame
                                           TRUE) ;                  // Force blend
        }
        else
        {
            // Blend into firing
            m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_FIRE,   // Type
                                           0.1f,                    // Blend time
                                           0,                       // Start frame
                                           TRUE) ;                  // Force blend
        }

        // Start the recoil anim
        m_PlayerInstance.SetAdditiveAnimByType(ADDITIVE_ANIM_ID_RECOIL, ANIM_TYPE_CHARACTER_RECOIL) ;

        // Set to frame zero incase it's already playing
        anim_state *Recoil = m_PlayerInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_RECOIL) ;
        ASSERT(Recoil) ;
        if (Recoil->GetAnim())
        {
            Recoil->SetFrame(0.0f) ;
            Recoil->SetAnimPlayed(0) ;
        }
        
        pGameLogic->WeaponFired();
    }
}

void player_object::Weapon_Advance_FIRE(f32 DeltaTime)
{
    (void)DeltaTime;

    // Anim done?
    if (m_WeaponInstance.GetCurrentAnimState().GetAnimPlayed())
        Weapon_SetupState(WEAPON_STATE_RELOAD) ;
}

//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::Weapon_Setup_RELOAD()
{
    // Start anim
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_RELOAD, 0.25f) ;
}

void player_object::Weapon_Advance_RELOAD(f32 DeltaTime)
{
    (void)DeltaTime;

    // Anim done?
    if (m_WeaponInstance.GetCurrentAnimState().GetAnimPlayed())
        Weapon_SetupState(WEAPON_STATE_IDLE) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::Weapon_Setup_DEACTIVATE()
{
    // Start anim
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_DEACTIVATE, 0.25f) ;
}

void player_object::Weapon_Advance_DEACTIVATE(f32 DeltaTime)
{
    (void)DeltaTime ;
}

//==============================================================================
