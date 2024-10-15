//=============================================================================
//
//  Training Mission 5 Logic
//
//=============================================================================

#ifndef CT5_LOGIC_HPP
#define CT5_LOGIC_HPP

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

class ct5_logic : public campaign_logic
{

//-----------------------------------------------------------------------------
//  Public Functions
//-----------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag    );
virtual void    StationUsed     ( object::id PlayerID );
virtual void    EnforceBounds   ( f32 DeltaTime );

//-----------------------------------------------------------------------------
//  Private Types
//-----------------------------------------------------------------------------

protected:

    enum game_status
    {
        STATE_IDLE,
        STATE_INTRO,
        STATE_GRAVCYCLE,
        STATE_TRANSPORT,
        STATE_BOMBER,
        STATE_SCOUT,
    };
    
//-----------------------------------------------------------------------------
//  Private Functions
//-----------------------------------------------------------------------------

protected:

        void        AdvanceStates   ( void );
        void        NextState       ( void );
        xbool       Mission         ( void );
        void        Intro           ( void );
        void        GravCycle       ( void );
        void        Transport       ( void );
        void        Bomber          ( void );
        void        Scout           ( void );
        void        SetVehicleUI    ( s32 StringID );

//-----------------------------------------------------------------------------
//  Private Data
//-----------------------------------------------------------------------------

protected:

    //
    // Objects
    //
    
    object::id      m_Pad1;
    object::id      m_Pad2;
    object::id      m_Pad3;
    object::id      m_Pad4;
    object::id      m_Waypt1;
    object::id      m_Waypt2;
    object::id      m_Waypt3;

    //
    // Flags
    //
    
    xbool           m_bInitState;
    xbool           m_bObjective;
    xbool           m_bStationUsed;
    xbool           m_bHint;
    xbool           m_bPadPower;
    xbool           m_bGetIn;
    xbool           m_bInMenu;
    
    //
    // Variables
    //

    s32             m_State;
    s32             m_NewState;
    s32             m_Audio;

    s32             m_Audio1;
    s32             m_Audio2;
    s32             m_Obj1;
    s32             m_Obj2;
    object::id      m_CurrPad;
    object::id      m_NextPad;
    object::type    m_Vehicle;
};

//=============================================================================
//  GLOBAL VARIABLES
//=============================================================================

extern ct5_logic CT5_Logic;

//=============================================================================
#endif
//=============================================================================



