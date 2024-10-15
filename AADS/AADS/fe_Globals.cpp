//=========================================================================
//
//  fe_Globals.cpp
//
//=========================================================================

#include "AADS/SpecialVersion.hpp"

//=========================================================================

#include "fe_globals.hpp"
#include "GameMgr/GameMgr.hpp"

//=========================================================================
//  Global Structure
//=========================================================================

t2_fe_globals   fegl;

//=============================================================================
//  Missions Tables
//=============================================================================

enum 
{
    MISSION_ABOMINABLE,
    MISSION_AVALON,
    MISSION_BEGGARSRUN,
    MISSION_DAMNATION,
    MISSION_DEATHBIRDSFLY,
    MISSION_DESICCATOR,
    MISSION_EQUINOX,
    MISSION_ESCALADE,
    MISSION_FIRESTORM,
    MISSION_FLASHPOINT,
    MISSION_GEHENNA,
    MISSION_INSALUBRIA,
    MISSION_INVICTUS,
    MISSION_JACOBSLADDER,
    MISSION_KATABATIC,
    MISSION_MISSION01,
    MISSION_MISSION02,
    MISSION_MISSION03,
    MISSION_MISSION04,
    MISSION_MISSION05,
    MISSION_MISSION06,
    MISSION_MISSION07,
    MISSION_MISSION08,
    MISSION_MISSION10,
    MISSION_MISSION11,
    MISSION_MISSION13,
    MISSION_MYRKWOOD,
    MISSION_OASIS,
    MISSION_OVERREACH,
    MISSION_PARANOIA,
    MISSION_PYROCLASM,
    MISSION_QUAGMIRE,
    MISSION_RASP,
    MISSION_RECALESCENCE,
    MISSION_REVERSION,
    MISSION_RIMEHOLD,
    MISSION_SANCTUARY,
    MISSION_SIROCCO,
    MISSION_SLAPDASH,
    MISSION_SUNDRIED,
    MISSION_THINICE,
    MISSION_TOMBSTONE,
    MISSION_TRAINING1,
    MISSION_TRAINING2,
    MISSION_TRAINING3,
    MISSION_TRAINING4,
    MISSION_TRAINING5,
    MISSION_UNDERHILL,
    MISSION_WHITEOUT,
};

// SPECIAL_VERSION

mission_def fe_Missions[] =
{
    { GAME_CNH      ,   "Abominable"     ,   MISSION_ABOMINABLE        }, 
    { GAME_CTF      ,   "Avalon"         ,   MISSION_AVALON            }, 
    { GAME_CTF      ,   "BeggarsRun"     ,   MISSION_BEGGARSRUN        }, 
    { GAME_CTF      ,   "Damnation"      ,   MISSION_DAMNATION         }, 
    { GAME_CTF      ,   "DeathBirdsFly"  ,   MISSION_DEATHBIRDSFLY     }, 
    { GAME_CTF      ,   "Desiccator"     ,   MISSION_DESICCATOR        }, 
    { GAME_CNH      ,   "Equinox"        ,   MISSION_EQUINOX           }, 
    { GAME_TDM      ,   "Equinox"        ,   MISSION_EQUINOX           }, 
    { GAME_DM       ,   "Equinox"        ,   MISSION_EQUINOX           }, 
    { GAME_TDM      ,   "Escalade"       ,   MISSION_ESCALADE          }, 
    { GAME_DM       ,   "Escalade"       ,   MISSION_ESCALADE          }, 
    { GAME_HUNTER   ,   "Escalade"       ,   MISSION_ESCALADE          }, 
    { GAME_CTF      ,   "Firestorm"      ,   MISSION_FIRESTORM         }, 
    { GAME_CNH      ,   "Firestorm"      ,   MISSION_FIRESTORM         }, 
    { GAME_CNH      ,   "Flashpoint"     ,   MISSION_FLASHPOINT        }, 
    { GAME_HUNTER   ,   "Gehenna"        ,   MISSION_GEHENNA           }, 
    { GAME_CNH      ,   "Insalubria"     ,   MISSION_INSALUBRIA        }, 
    { GAME_TDM      ,   "Invictus"       ,   MISSION_INVICTUS          }, 
    { GAME_DM       ,   "Invictus"       ,   MISSION_INVICTUS          }, 
    { GAME_CNH      ,   "JacobsLadder"   ,   MISSION_JACOBSLADDER      }, 
    { GAME_CTF      ,   "Katabatic"      ,   MISSION_KATABATIC         }, 
    { GAME_CAMPAIGN ,   "Mission01"      ,   MISSION_MISSION01         }, 
    { GAME_CAMPAIGN ,   "Mission02"      ,   MISSION_MISSION02         }, 
    { GAME_CAMPAIGN ,   "Mission03"      ,   MISSION_MISSION03         }, 
    { GAME_CAMPAIGN ,   "Mission04"      ,   MISSION_MISSION04         }, 
    { GAME_CAMPAIGN ,   "Mission05"      ,   MISSION_MISSION05         }, 
    { GAME_CAMPAIGN ,   "Mission06"      ,   MISSION_MISSION06         }, 
    { GAME_CAMPAIGN ,   "Mission07"      ,   MISSION_MISSION07         }, 
    { GAME_CAMPAIGN ,   "Mission08"      ,   MISSION_MISSION08         }, 
    { GAME_CAMPAIGN ,   "Mission10"      ,   MISSION_MISSION10         }, 
    { GAME_CAMPAIGN ,   "Mission11"      ,   MISSION_MISSION11         }, 
    { GAME_CAMPAIGN ,   "Mission13"      ,   MISSION_MISSION13         }, 
    { GAME_TDM      ,   "Myrkwood"       ,   MISSION_MYRKWOOD          }, 
    { GAME_DM       ,   "Myrkwood"       ,   MISSION_MYRKWOOD          }, 
    { GAME_HUNTER   ,   "Myrkwood"       ,   MISSION_MYRKWOOD          }, 
    { GAME_TDM      ,   "Oasis"          ,   MISSION_OASIS             }, 
    { GAME_DM       ,   "Oasis"          ,   MISSION_OASIS             }, 
    { GAME_CNH      ,   "Overreach"      ,   MISSION_OVERREACH         }, 
    { GAME_CTF      ,   "Paranoia"       ,   MISSION_PARANOIA          }, 
    { GAME_TDM      ,   "Paranoia"       ,   MISSION_PARANOIA          }, 
    { GAME_DM       ,   "Paranoia"       ,   MISSION_PARANOIA          }, 
    { GAME_DM       ,   "Pyroclasm"      ,   MISSION_PYROCLASM         }, 
    { GAME_CTF      ,   "Quagmire"       ,   MISSION_QUAGMIRE          }, 
    { GAME_TDM      ,   "Rasp"           ,   MISSION_RASP              }, 
    { GAME_DM       ,   "Rasp"           ,   MISSION_RASP              }, 
    { GAME_HUNTER   ,   "Rasp"           ,   MISSION_RASP              }, 
    { GAME_CTF      ,   "Recalescence"   ,   MISSION_RECALESCENCE      }, 
    { GAME_CTF      ,   "Reversion"      ,   MISSION_REVERSION         }, 
    { GAME_TDM      ,   "Rimehold"       ,   MISSION_RIMEHOLD          }, 
    { GAME_DM       ,   "Rimehold"       ,   MISSION_RIMEHOLD          }, 
    { GAME_HUNTER   ,   "Rimehold"       ,   MISSION_RIMEHOLD          }, 
    { GAME_CTF      ,   "Sanctuary"      ,   MISSION_SANCTUARY         }, 
    { GAME_CNH      ,   "Sirocco"        ,   MISSION_SIROCCO           }, 
    { GAME_CTF      ,   "Slapdash"       ,   MISSION_SLAPDASH          }, 
    { GAME_TDM      ,   "SunDried"       ,   MISSION_SUNDRIED          }, 
    { GAME_DM       ,   "SunDried"       ,   MISSION_SUNDRIED          }, 
    { GAME_HUNTER   ,   "SunDried"       ,   MISSION_SUNDRIED          }, 
    { GAME_CTF      ,   "ThinIce"        ,   MISSION_THINICE           }, 
    { GAME_CTF      ,   "Tombstone"      ,   MISSION_TOMBSTONE         }, 
    { GAME_CAMPAIGN ,   "Training1"      ,   MISSION_TRAINING1         }, 
    { GAME_CAMPAIGN ,   "Training2"      ,   MISSION_TRAINING2         }, 
    { GAME_CAMPAIGN ,   "Training3"      ,   MISSION_TRAINING3         }, 
    { GAME_CAMPAIGN ,   "Training4"      ,   MISSION_TRAINING4         }, 
    { GAME_CAMPAIGN ,   "Training5"      ,   MISSION_TRAINING5         }, 
    { GAME_TDM      ,   "Underhill"      ,   MISSION_UNDERHILL         }, 
    { GAME_DM       ,   "Underhill"      ,   MISSION_UNDERHILL         }, 
    { GAME_HUNTER   ,   "Underhill"      ,   MISSION_UNDERHILL         }, 
    { GAME_TDM      ,   "Whiteout"       ,   MISSION_WHITEOUT          }, 
    { GAME_DM       ,   "Whiteout"       ,   MISSION_WHITEOUT          }, 
    { GAME_NONE     ,   "NONE"           ,   0                         },
};

//=============================================================================
