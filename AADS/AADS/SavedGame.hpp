#ifndef __SAVEDGAME_HPP
#define __SAVEDGAME_HPP

#include "fe_globals.hpp"
#include "AADS/SpecialVersion.hpp"
#include "serverversion.hpp"
//---------------------------------------------------------
// This contains all the structs used when a game is saved
// We have a single file of 'saved_game' type.

#define MAX_SAVED_WARRIORS  16
#define MAX_SAVED_FILTERS    4


struct saved_game
{
    saved_options   Options;                        // Needs to be first thing because we load only this during memcard check
    s32             Checksum;
    s32             FilterCount;
//    saved_filter    Filters[MAX_SAVED_FILTERS];
    s8              LastWarriorIndex[2];            // Last warrior slot that was saved so we can autoload
    s8              LastWarriorValid;               // Bitmask of which last saved warrior index is valid (if any)
    s32             WarriorCount;
    warrior_setup   Warrior[MAX_SAVED_WARRIORS];
};
//
// Needs to be changed when we get product code from Sony
//
#if defined(WIDE_BETA) // SPECIAL_VERSION
#define SAVE_DIRECTORY  "BASLUS-28011"
#define SAVE_FILENAME   "BASLUS-28011"
#else
#define SAVE_DIRECTORY  "BASLUS-20149"
#define SAVE_FILENAME   "BASLUS-20149"
#endif

#define SAVE_FILE_SIZE  (110*1024)
#define STR_NO_SAVED_GAMES      "No Saved Games"
#define STR_NO_SAVED_WARRIORS   "No Saved Warriors"
#define STR_NO_CARD_PRESENT     "No Memory Card present"

#define SAVED_CHECKSUM_SEED (0x4afb0023^SERVER_VERSION)

#endif