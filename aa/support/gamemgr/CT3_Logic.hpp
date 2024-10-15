//=============================================================================
//
//  Training Mission 3 Logic
//
//=============================================================================

#ifndef CT3_LOGIC_HPP
#define CT3_LOGIC_HPP

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

class ct3_logic : public campaign_logic
{

//-----------------------------------------------------------------------------
//  Public Functions
//-----------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag );
virtual void    ItemDestroyed   ( object::id ItemID, object::id PlayerID );
virtual void    WeaponFired     ( object::id ItemID );
virtual void    EnforceBounds   ( f32 DeltaTime );

//-----------------------------------------------------------------------------
//  Private Types
//-----------------------------------------------------------------------------

protected:

    enum game_status
    {
        STATE_IDLE,
        STATE_MISSION,
    };
    
//-----------------------------------------------------------------------------
//  Private Functions
//-----------------------------------------------------------------------------

protected:

        void        AdvanceStates   ( void );
        void        NextState       ( void );
        void        Mission         ( void );

//-----------------------------------------------------------------------------
//  Private Data
//-----------------------------------------------------------------------------

protected:

    //
    // Objects
    //
    
    object::id      m_RepairPatch;
    object::id      m_RepairKit;
    object::id      m_Inven;
    object::id      m_Turret1;
    object::id      m_Turret2;
    object::id      m_Turret3;
    object::id      m_Turret4;
    object::id      m_Turret5;
    object::id      m_TurretGen;

    //
    // Flags
    //
    
    xbool           m_bInitState;
    xbool           m_bObjective;
    xbool           m_bWeaponFired;
    xbool           m_bGrenadeUsed;
    
    //
    // Variables
    //

    s32             m_State;
    s32             m_NewState;
    s32             m_Audio;
    s32             m_NumPickups;
};

//=============================================================================
//  GLOBAL VARIABLES
//=============================================================================

extern ct3_logic CT3_Logic;

//=============================================================================
#endif
//=============================================================================



