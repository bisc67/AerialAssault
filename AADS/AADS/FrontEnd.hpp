//==============================================================================
//
//  FrontEnd.hpp
//
//==============================================================================

#ifndef FRONT_END_HPP
#define FRONT_END_HPP

struct warrior_setup;

//==============================================================================

enum warrior_controls
{
    WARRIOR_CONTROL_CONFIG_A,
    WARRIOR_CONTROL_CONFIG_B,
    WARRIOR_CONTROL_CONFIG_C,
    WARRIOR_CONTROL_CONFIG_D
//    WARRIOR_CONTROL_COMMANDER
};

extern void SetWarriorControlDefault    ( warrior_setup* pWarriorSetup, s32 iDefault );

extern void AdvanceMission              ( void );          // Advance to next mission in cycle

//==============================================================================
#endif //FRONT_END_HPP
//==============================================================================

