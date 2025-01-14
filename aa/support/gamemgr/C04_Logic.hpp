//==============================================================================
//
//  C04_Logic.hpp
//
//==============================================================================

#ifndef C04_LOGIC_HPP
#define C04_LOGIC_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "CNH_Logic.hpp"

//==============================================================================
//  DEFINES
//==============================================================================


//==============================================================================
//  TYPES
//==============================================================================

class c04_logic : public cnh_logic
{

//------------------------------------------------------------------------------
//  Public Functions
//------------------------------------------------------------------------------

public:

//------------------------------------------------------------------------------
//  Private Functions
//------------------------------------------------------------------------------

protected:

virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    BeginGame       ( void );

//------------------------------------------------------------------------------
//  Private Storage
//------------------------------------------------------------------------------

protected:

    f32     DelayTime;
    xbool   Victorious;
    xbool   GameOver;
    s32     ChannelID;
};

//==============================================================================
//  GLOBAL VARIABLES
//==============================================================================

extern c04_logic  C04_Logic;

//==============================================================================
#endif // C04_LOGIC_HPP
//==============================================================================
