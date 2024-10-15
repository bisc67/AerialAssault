//=============================================================================
//
//  Training Mission 2 Logic
//
//=============================================================================

#ifndef CT2_LOGIC_HPP
#define CT2_LOGIC_HPP

//=============================================================================
//  INCLUDES
//=============================================================================

#include "Campaign_Logic.hpp"

//=============================================================================
//  DEFINES
//=============================================================================

//=============================================================================
//  CLASSES
//=============================================================================

class ct2_logic : public campaign_logic
{

//-----------------------------------------------------------------------------
//  Public Functions
//-----------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag );
virtual void    WeaponFired     ( object::id ItemID );
virtual void    WeaponExchanged ( object::id ItemID );
virtual void    ItemAutoLocked  ( object::id ItemID,   object::id PlayerID );
virtual void    PickupTouched   ( object::id PickupID, object::id PlayerID );
virtual void    EnforceBounds   ( f32 DeltaTime );

//-----------------------------------------------------------------------------
//  Private Types
//-----------------------------------------------------------------------------

protected:

    enum game_status
    {
        STATE_IDLE,
        STATE_WEAPONS,
        STATE_SNIPER,
        STATE_AUTOLOCK,
    };
    
//-----------------------------------------------------------------------------
//  Private Functions
//-----------------------------------------------------------------------------

protected:
        
        void        AdvanceStates   ( void );
        void        NextState       ( xbool DoClearText = TRUE );
        void        SetNextState    ( s32 State );
        void        Weapons         ( void );
        void        Sniper          ( void );
        void        AutoLock        ( void );
        object::id  CreateDerm      ( object::id WaypointID );
        void        UpdateDerm      ( object::id BotID );

//-----------------------------------------------------------------------------
//  Private Data
//-----------------------------------------------------------------------------

protected:

    //
    // Objects
    //
    
    object::id      m_WeaponGen;
    object::id      m_SniperGen;
    object::id      m_BotGen;
    object::id      m_DiskLauncher;
    object::id      m_Plasma;
    object::id      m_GrenadeLauncher;
    object::id      m_ChainGun;
    object::id      m_Blaster;
    object::id      m_Sniper;
    object::id      m_Waypoint1;
    object::id      m_Waypoint2;
    object::id      m_Bot;
    object::id      m_FFGen;
    object::id      m_BotPt;
    object::id      m_ChuteGen;
    object::id      m_WeaponPt;
    object::id      m_AmmoPt;

    //
    // Flags
    //

    xbool           m_bInitState;
    xbool           m_bObjective;
    xbool           m_bDiskLauncher;
    xbool           m_bPlasma;
    xbool           m_bGrenadeLauncher;
    xbool           m_bChainGun;
    xbool           m_bBlaster;
    xbool           m_bSniper;
    xbool           m_bCycleWeapon;
    xbool           m_bExchangeWeapon;
    xbool           m_bBotGenPower;
    xbool           m_bWeaponFired;
    xbool           m_bWeaponExchanged;
    xbool           m_bAutoLocked;
    xbool           m_bBotJet;
    xbool           m_bAmmoBox;
    xbool           m_bAmmoTaken;
    xbool           m_bBotKilled;
    
    //
    // Variables
    //

    s32             m_State;
    s32             m_NewState;
    s32             m_Audio;
    s32             m_Prompt;
    s32             m_NumWeaponsUsed;
    
    player_object::invent_type m_LastWeapon;
};

//=============================================================================
//  GLOBAL VARIABLES
//=============================================================================

extern ct2_logic CT2_Logic;

//=============================================================================
#endif
//=============================================================================


