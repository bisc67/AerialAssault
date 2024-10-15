//=============================================================================
//
//  Training Mission 1 Logic
//
//=============================================================================

#ifndef CT1_LOGIC_HPP
#define CT1_LOGIC_HPP

//=============================================================================
//  INCLUDES
//=============================================================================

#include "Campaign_Logic.hpp"
#include "HUD/hud_manager.hpp"

//=============================================================================
//  DEFINES
//=============================================================================

const f32 StickHoldTime = 0.1f;

//=============================================================================
//  CLASSES
//=============================================================================

class ct1_logic : public campaign_logic
{

//-----------------------------------------------------------------------------
//  Public Functions
//-----------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag );
virtual void    EnforceBounds   ( f32 DeltaTime );

//-----------------------------------------------------------------------------
//  Private Types
//-----------------------------------------------------------------------------

protected:

    enum game_status
    {
        STATE_IDLE,
        STATE_HUD,
        STATE_MOVEMENT,
    };
    
//-----------------------------------------------------------------------------
//  Private Functions
//-----------------------------------------------------------------------------

protected:
        
        void        HUDTraining     ( void );
        void        Movement        ( void );

        void        AdvanceStates   ( void );
        void        NextState       ( void );
        void        Training        ( s32 AudioID, hud_glow Glow = HUDGLOW_NONE );

//-----------------------------------------------------------------------------
//  Private Data
//-----------------------------------------------------------------------------

protected:

    //
    // Objects
    //
    
    object::id      m_Switch1;
    object::id      m_Switch2;
    object::id      m_Switch3;
    object::id      m_Switch4;
    object::id      m_Switch5;
    object::id      m_Generator;

    //
    // Flags
    //
    
    xbool           m_bInitState;
    xbool           m_bObjective;
    xbool           m_bLookLeft;
    xbool           m_bLookRight;
    xbool           m_bMoveLeft;
    xbool           m_bMoveRight;
    
    //
    // Variables
    //

    s32             m_State;
    s32             m_NewState;
    s32             m_Audio;
    s32             m_nSwitches;
};

//=============================================================================
//  GLOBAL VARIABLES
//=============================================================================

extern ct1_logic CT1_Logic;

//=============================================================================
#endif
//=============================================================================


