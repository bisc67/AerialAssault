//=============================================================================
//
//  Training Mission 4 Logic
//
//=============================================================================

#ifndef CT4_LOGIC_HPP
#define CT4_LOGIC_HPP

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

class ct4_logic : public campaign_logic
{

//-----------------------------------------------------------------------------
//  Public Functions
//-----------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag    );
virtual void    ItemRepaired    ( object::id ItemID, object::id PlayerID, xbool Score );
virtual void    ItemDeployed    ( object::id ItemID, object::id OriginID );
virtual void    StationUsed     ( object::id PlayerID );
virtual void    SatchelDetonated( object::id PlayerID );
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
        void        CanOnlyUsePack  ( player_object::invent_type PackType );

//-----------------------------------------------------------------------------
//  Private Data
//-----------------------------------------------------------------------------

protected:

    //
    // Objects
    //
    
    object::id      m_RepairPack;
    object::id      m_Bunker;
    object::id      m_Inven;
    object::id      m_InvenPos;
    object::id      m_Turret;

    //
    // Flags
    //
    
    xbool           m_bInitState;
    xbool           m_bObjective;
    xbool           m_bHint;
    xbool           m_bInvenRepaired;
    xbool           m_bInvenUsed;
    xbool           m_bSatchelDeployed;
    xbool           m_bSatchelDestroyed;
    xbool           m_bPowerDownInven;
    
    //
    // Variables
    //

    s32             m_State;
    s32             m_NewState;
    s32             m_Audio;
};

//=============================================================================
//  GLOBAL VARIABLES
//=============================================================================

extern ct4_logic CT4_Logic;

//=============================================================================
#endif
//=============================================================================



