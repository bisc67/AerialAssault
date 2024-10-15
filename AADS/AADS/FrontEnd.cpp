//==============================================================================
//
//  FrontEnd.cpp
//
//==============================================================================

#include "entropy.hpp"
#include "tokenizer\tokenizer.hpp"
#include "globals.hpp"
#include "fe_globals.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "hud\hud_manager.hpp"

#include "dlg_WarriorSetup.hpp"
#include "dlg_loadsave.hpp"
#include "dlg_MainMenu.hpp"
#include "dlg_Online.hpp"
#include "dlg_OnlineHost.hpp"
#include "dlg_OnlineJoin.hpp"
#include "dlg_Objectives.hpp"
#include "dlg_Controls.hpp"
#include "dlg_Buddies.hpp"
#include "dlg_Inventory.hpp"
#include "dlg_Vehicle.hpp"
#include "dlg_Score.hpp"
#include "dlg_Admin.hpp"
#include "dlg_Debug.hpp"
#include "dlg_Message.hpp"
#include "dlg_ClientLimits.hpp"
#include "dlg_Campaign.hpp"
#include "dlg_Brief.hpp"
#include "dlg_MissionLoad.hpp"
#include "dlg_MissionEnd.hpp"
#include "dlg_SoundTest.hpp"
#include "dlg_controlconfig2.hpp"
#include "dlg_player.hpp"
#include "dlg_OnlineMainMenu.hpp"
#include "dlg_OfflineMainMenu.hpp"
#include "dlg_OptionsMainMenu.hpp"
#include "dlg_AudioOptions.hpp"
#include "dlg_NetworkOptions.hpp"
#include "dlg_DeBrief.hpp"
#include "ui_joinlist.hpp"

#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "pointlight\pointlight.hpp"

#include "objects\bot\pathgenerator.hpp"
#include "Objects\Bot\MasterAI.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"
#include "StringMgr\StringMgr.hpp"

#include "StringMgr\StringMgr.hpp"
#include "Demo1\Data\UI\ui_strings.h"
#include "FrontEnd.hpp"

#include "Demo1/SpecialVersion.hpp"

s32 LoadMissionStage = 0;

xwchar  g_JoinPassword[FE_MAX_ADMIN_PASS] = {0};

//==============================================================================
//  Switches
//==============================================================================

#define BYPASS_FRONTEND     1
s32 BYPASS=0;
s32 MISSION=5+11;
s32 NUM_PLAYERS=1;

//==============================================================================
//  Externals
//==============================================================================

extern server_manager* pSvrMgr;
extern ui_manager::dialog_tem MissionLoadDialog;
extern ui_manager::dialog_tem MissionEndDialog;
extern ui_manager::dialog_tem BriefDialog;
extern ui_manager::dialog_tem DeBriefDialog;

void EndMissionDialogs( void );

//==============================================================================
//  Statics
//==============================================================================

static xbool    s_FrontEndLoaded = FALSE;

//==============================================================================
//  Favorite Setup Tables
//==============================================================================

struct favorite
{
    player_object::armor_type    Armor;
    s32                          nWeapons;
    player_object::invent_type   Weapon1;
    player_object::invent_type   Weapon2;
    player_object::invent_type   Weapon3;
    player_object::invent_type   Weapon4;
    player_object::invent_type   Weapon5;
    player_object::invent_type   Pack;
    player_object::invent_type   Grenade;
    object::type                 Vehicle;
};

favorite Favorites[FE_TOTAL_FAVORITES] =
{
    // LIGHT
    { // SCOUT SNIPER
        player_object::ARMOR_TYPE_LIGHT,
        3,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_ARMOR_PACK_ENERGY,
        player_object::INVENT_TYPE_GRENADE_FLARE,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // SCOUT FLAG CHASER
        player_object::ARMOR_TYPE_LIGHT,
        3,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_ARMOR_PACK_ENERGY,
        player_object::INVENT_TYPE_GRENADE_CONCUSSION,
        object::TYPE_VEHICLE_WILDCAT
    },

        { // SCOUT DEFENSE
        player_object::ARMOR_TYPE_LIGHT,
        3,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_BLASTER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_ARMOR_PACK_SHIELD,
        player_object::INVENT_TYPE_MINE,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // DECOY
        player_object::ARMOR_TYPE_LIGHT,
        3,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_BLASTER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE,
        player_object::INVENT_TYPE_MINE,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // INFILTRATOR
        player_object::ARMOR_TYPE_LIGHT,
        3,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE,
        player_object::INVENT_TYPE_WEAPON_BLASTER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_ARMOR_PACK_ENERGY,
        player_object::INVENT_TYPE_GRENADE_BASIC,
        object::TYPE_VEHICLE_WILDCAT
    },

    // MEDIUM
    { // ASSAULT OFFENSE
        player_object::ARMOR_TYPE_MEDIUM,
        4,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION,
        player_object::INVENT_TYPE_GRENADE_BASIC,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // ASSAULT_DEPLOYER
        player_object::ARMOR_TYPE_MEDIUM,
        4,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_BLASTER,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR,
        player_object::INVENT_TYPE_MINE,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // ASSAULT DEFENSE
        player_object::ARMOR_TYPE_MEDIUM,
        4,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR,
        player_object::INVENT_TYPE_GRENADE_FLARE,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // TAILGUNNER
        player_object::ARMOR_TYPE_MEDIUM,
        4,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_ARMOR_PACK_AMMO,
        player_object::INVENT_TYPE_GRENADE_CONCUSSION,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // TURRET FARMER
        player_object::ARMOR_TYPE_MEDIUM,
        4,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_BLASTER,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_NONE,
        player_object::INVENT_TYPE_ARMOR_PACK_REPAIR,
        player_object::INVENT_TYPE_GRENADE_FLARE,
        object::TYPE_VEHICLE_WILDCAT
    },

    // HEAVY
    { // JUGGERNAUT OFFENSE
        player_object::ARMOR_TYPE_HEAVY,
        5,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_MORTAR_GUN,
        player_object::INVENT_TYPE_ARMOR_PACK_AMMO,
        player_object::INVENT_TYPE_GRENADE_BASIC,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // JUGGERNAUT DEPLOYER
        player_object::ARMOR_TYPE_HEAVY,
        5,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_BLASTER,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_MORTAR_GUN,
        player_object::INVENT_TYPE_ARMOR_PACK_ENERGY,
        player_object::INVENT_TYPE_GRENADE_FLARE,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // JUGGERNAUT DEFENSE
        player_object::ARMOR_TYPE_HEAVY,
        5,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_PLASMA_GUN,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_MORTAR_GUN,
        player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR,
        player_object::INVENT_TYPE_GRENADE_FLARE,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // HEAVY LOVE
        player_object::ARMOR_TYPE_HEAVY,
        5,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_MORTAR_GUN,
        player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION,
        player_object::INVENT_TYPE_GRENADE_CONCUSSION,
        object::TYPE_VEHICLE_WILDCAT
    },

    { // FLAG DEFENDER
        player_object::ARMOR_TYPE_HEAVY,
        5,
        player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_CHAIN_GUN,
        player_object::INVENT_TYPE_WEAPON_BLASTER,
        player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER,
        player_object::INVENT_TYPE_WEAPON_MORTAR_GUN,
        player_object::INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR,
        player_object::INVENT_TYPE_MINE,
        object::TYPE_VEHICLE_WILDCAT
    }
};

//==============================================================================
//  Warrior Control Editing Defaults
//==============================================================================

struct control_default
{
    const char*             Name;
    warrior_control_data    Controls[WARRIOR_CONTROL_SIZEOF];
};

#define INPUT_NULL              32768
#define INPUT_VOICE_CHAT_RIGHT  32769
#define INPUT_VOICE_CHAT_LEFT   32770

static control_default DefaultControls[4] =
{
    {
        "CONFIG A",
        {
            { WARRIOR_CONTROL_SHIFT,        INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MOVE,         INPUT_PS2_STICK_LEFT_X,   0 },
            { WARRIOR_CONTROL_LOOK,         INPUT_PS2_STICK_RIGHT_X,  0 },
            { WARRIOR_CONTROL_FREELOOK,     INPUT_PS2_BTN_R_STICK,    0 },
            { WARRIOR_CONTROL_JUMP,         INPUT_PS2_BTN_L1,         0 },
            { WARRIOR_CONTROL_FIRE,         INPUT_PS2_BTN_L2,         0 },
            { WARRIOR_CONTROL_JET,          INPUT_PS2_BTN_R1,         0 },
            { WARRIOR_CONTROL_ZOOM,         INPUT_PS2_BTN_R2,         0 },
            { WARRIOR_CONTROL_NEXT_WEAPON,  INPUT_PS2_BTN_CROSS,      0 },
            { WARRIOR_CONTROL_GRENADE,      INPUT_PS2_BTN_TRIANGLE,   0 },
            { WARRIOR_CONTROL_VOICE_MENU,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_VOICE_CHAT,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MINE,         INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_USE_PACK,     INPUT_PS2_BTN_CIRCLE,     0 },
            { WARRIOR_CONTROL_ZOOM_IN,      INPUT_PS2_BTN_L_UP,       0 },
            { WARRIOR_CONTROL_ZOOM_OUT,     INPUT_PS2_BTN_L_DOWN,     0 },
            { WARRIOR_CONTROL_USE_HEALTH,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_TARGET_LASER, INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_WEAPON,  INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_PACK,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DEPLOY_BEACON,INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_FLAG,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_SUICIDE,      INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_OPTIONS,      INPUT_PS2_BTN_START,      0 },
            { WARRIOR_CONTROL_TARGET_LOCK,  INPUT_PS2_BTN_SQUARE,     0 },
            { WARRIOR_CONTROL_COMPLIMENT,   INPUT_PS2_BTN_L_RIGHT,    0 },
            { WARRIOR_CONTROL_TAUNT,        INPUT_PS2_BTN_L_LEFT,     0 },
            { WARRIOR_CONTROL_EXCHANGE,     INPUT_PS2_BTN_SELECT,     0 }
        }
    },

    {
        "CONFIG B",
        {
            { WARRIOR_CONTROL_SHIFT,        INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MOVE,         INPUT_PS2_STICK_LEFT_X,   0 },
            { WARRIOR_CONTROL_LOOK,         INPUT_PS2_STICK_RIGHT_X,  0 },
            { WARRIOR_CONTROL_FREELOOK,     INPUT_PS2_BTN_R_STICK,    0 },
            { WARRIOR_CONTROL_JUMP,         INPUT_PS2_BTN_R1,         0 },
            { WARRIOR_CONTROL_FIRE,         INPUT_PS2_BTN_R2,         0 },
            { WARRIOR_CONTROL_JET,          INPUT_PS2_BTN_L1,         0 },
            { WARRIOR_CONTROL_ZOOM,         INPUT_PS2_BTN_L2,         0 },
            { WARRIOR_CONTROL_NEXT_WEAPON,  INPUT_PS2_BTN_CROSS,      0 },
            { WARRIOR_CONTROL_GRENADE,      INPUT_PS2_BTN_TRIANGLE,   0 },
            { WARRIOR_CONTROL_VOICE_MENU,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_VOICE_CHAT,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MINE,         INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_USE_PACK,     INPUT_PS2_BTN_CIRCLE,     0 },
            { WARRIOR_CONTROL_ZOOM_IN,      INPUT_PS2_BTN_L_UP,       0 },
            { WARRIOR_CONTROL_ZOOM_OUT,     INPUT_PS2_BTN_L_DOWN,     0 },
            { WARRIOR_CONTROL_USE_HEALTH,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_TARGET_LASER, INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_WEAPON,  INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_PACK,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DEPLOY_BEACON,INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_FLAG,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_SUICIDE,      INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_OPTIONS,      INPUT_PS2_BTN_START,      0 },
            { WARRIOR_CONTROL_TARGET_LOCK,  INPUT_PS2_BTN_SQUARE,     0 },
            { WARRIOR_CONTROL_COMPLIMENT,   INPUT_PS2_BTN_L_RIGHT,    0 },
            { WARRIOR_CONTROL_TAUNT,        INPUT_PS2_BTN_L_LEFT,     0 },
            { WARRIOR_CONTROL_EXCHANGE,     INPUT_PS2_BTN_SELECT,     0 }
        }
    },
   
    {
        "CONFIG C",
        {
            { WARRIOR_CONTROL_SHIFT,        INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MOVE,         INPUT_PS2_STICK_RIGHT_X,  0 },
            { WARRIOR_CONTROL_LOOK,         INPUT_PS2_STICK_LEFT_X,   0 },
            { WARRIOR_CONTROL_FREELOOK,     INPUT_PS2_BTN_R_STICK,    0 },
            { WARRIOR_CONTROL_JUMP,         INPUT_PS2_BTN_R1,         0 },
            { WARRIOR_CONTROL_FIRE,         INPUT_PS2_BTN_R2,         0 },
            { WARRIOR_CONTROL_JET,          INPUT_PS2_BTN_L1,         0 },
            { WARRIOR_CONTROL_ZOOM,         INPUT_PS2_BTN_L2,         0 },
            { WARRIOR_CONTROL_NEXT_WEAPON,  INPUT_PS2_BTN_L_DOWN,     0 },
            { WARRIOR_CONTROL_GRENADE,      INPUT_PS2_BTN_L_UP,       0 },
            { WARRIOR_CONTROL_VOICE_MENU,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_VOICE_CHAT,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MINE,         INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_USE_PACK,     INPUT_PS2_BTN_L_LEFT,     0 },
            { WARRIOR_CONTROL_ZOOM_IN,      INPUT_PS2_BTN_TRIANGLE,   0 },
            { WARRIOR_CONTROL_ZOOM_OUT,     INPUT_PS2_BTN_CROSS,      0 },
            { WARRIOR_CONTROL_USE_HEALTH,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_TARGET_LASER, INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_WEAPON,  INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_PACK,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DEPLOY_BEACON,INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_FLAG,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_SUICIDE,      INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_OPTIONS,      INPUT_PS2_BTN_START,      0 },
            { WARRIOR_CONTROL_TARGET_LOCK,  INPUT_PS2_BTN_L_RIGHT,    0 },
            { WARRIOR_CONTROL_COMPLIMENT,   INPUT_PS2_BTN_CIRCLE,     0 },                                    
            { WARRIOR_CONTROL_TAUNT,        INPUT_PS2_BTN_SQUARE,     0 },
            { WARRIOR_CONTROL_EXCHANGE,     INPUT_PS2_BTN_SELECT,     0 }
        }
    },

    {
        "CONFIG D",
        {
            { WARRIOR_CONTROL_SHIFT,        INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MOVE,         INPUT_PS2_STICK_RIGHT_X,  0 },
            { WARRIOR_CONTROL_LOOK,         INPUT_PS2_STICK_LEFT_X,   0 },
            { WARRIOR_CONTROL_FREELOOK,     INPUT_PS2_BTN_R_STICK,    0 },
            { WARRIOR_CONTROL_JUMP,         INPUT_PS2_BTN_L1,         0 },
            { WARRIOR_CONTROL_FIRE,         INPUT_PS2_BTN_L2,         0 },
            { WARRIOR_CONTROL_JET,          INPUT_PS2_BTN_R1,         0 },
            { WARRIOR_CONTROL_ZOOM,         INPUT_PS2_BTN_R2,         0 },
            { WARRIOR_CONTROL_NEXT_WEAPON,  INPUT_PS2_BTN_L_DOWN,     0 },
            { WARRIOR_CONTROL_GRENADE,      INPUT_PS2_BTN_L_UP,       0 },
            { WARRIOR_CONTROL_VOICE_MENU,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_VOICE_CHAT,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_MINE,         INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_USE_PACK,     INPUT_PS2_BTN_L_LEFT,     0 },
            { WARRIOR_CONTROL_ZOOM_IN,      INPUT_PS2_BTN_TRIANGLE,   0 },
            { WARRIOR_CONTROL_ZOOM_OUT,     INPUT_PS2_BTN_CROSS,      0 },
            { WARRIOR_CONTROL_USE_HEALTH,   INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_TARGET_LASER, INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_WEAPON,  INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_PACK,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DEPLOY_BEACON,INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_DROP_FLAG,    INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_SUICIDE,      INPUT_UNDEFINED,          0 },
            { WARRIOR_CONTROL_OPTIONS,      INPUT_PS2_BTN_START,      0 },
            { WARRIOR_CONTROL_TARGET_LOCK,  INPUT_PS2_BTN_L_RIGHT,    0 },
            { WARRIOR_CONTROL_COMPLIMENT,   INPUT_PS2_BTN_CIRCLE,     0 },                                    
            { WARRIOR_CONTROL_TAUNT,        INPUT_PS2_BTN_SQUARE,     0 },
            { WARRIOR_CONTROL_EXCHANGE,     INPUT_PS2_BTN_SELECT,     0 }
        }
    }

};

//==============================================================================
//  SetWarriorControlDefault
//==============================================================================

static void SetControl( player_object::game_key* pGameKey, warrior_control_data* pControlData, s32 ControlIndex )
{
    s32 MainGadget  = pControlData[ControlIndex].GadgetID;
    s32 ShiftGadget = pControlData[WARRIOR_CONTROL_SHIFT].GadgetID;
    s32 ShiftState  = pControlData[ControlIndex].IsShifted;

    ASSERT( !ShiftState || (ShiftGadget != INPUT_UNDEFINED) );

    // Check if we should set shift gadget to undefined
    if( !ShiftState && (ShiftGadget != INPUT_UNDEFINED) )
    {
        xbool   HasShiftedCounterpart = 0;

        for( s32 i=0 ; i<WARRIOR_CONTROL_SIZEOF ; i++ )
        {
            if( (pControlData[i].GadgetID == MainGadget) &&
                (pControlData[i].IsShifted) )
            {
                HasShiftedCounterpart = 1;
                break;
            }
        }

        // If no shifted version then make shift gadget undefined
        if( !HasShiftedCounterpart )
            ShiftGadget = INPUT_UNDEFINED;
    }

    // Set GameKey
    pGameKey->MainGadget  = (input_gadget)MainGadget;
    pGameKey->ShiftGadget = (input_gadget)ShiftGadget;
    pGameKey->ShiftState  = ShiftState;
}


void SetWarriorControlDefault( warrior_setup* pWarriorSetup, s32 iDefault )
{
    ASSERT( (iDefault>=0) && (iDefault<4) );

    // Setup Default to control data
    pWarriorSetup->ControlConfigID = iDefault;
    for( s32 i=0 ; i<WARRIOR_CONTROL_SIZEOF ; i++ )
        pWarriorSetup->ControlEditData[i] = DefaultControls[iDefault].Controls[i];

    // Set into actual control structure
	pWarriorSetup->ControlInfo.PS2_MOVE_LEFT_RIGHT.      MainGadget = (input_gadget)(pWarriorSetup->ControlEditData[WARRIOR_CONTROL_MOVE].GadgetID  );
    pWarriorSetup->ControlInfo.PS2_MOVE_FORWARD_BACKWARD.MainGadget = (input_gadget)(pWarriorSetup->ControlEditData[WARRIOR_CONTROL_MOVE].GadgetID+1);
    pWarriorSetup->ControlInfo.PS2_LOOK_LEFT_RIGHT.      MainGadget = (input_gadget)(pWarriorSetup->ControlEditData[WARRIOR_CONTROL_LOOK].GadgetID  );
    pWarriorSetup->ControlInfo.PS2_LOOK_UP_DOWN.         MainGadget = (input_gadget)(pWarriorSetup->ControlEditData[WARRIOR_CONTROL_LOOK].GadgetID+1);

    SetControl( &pWarriorSetup->ControlInfo.PS2_FREE_LOOK,        &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_FREELOOK      );
    SetControl( &pWarriorSetup->ControlInfo.PS2_JUMP,             &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_JUMP          );
    SetControl( &pWarriorSetup->ControlInfo.PS2_FIRE,             &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_FIRE          );
    SetControl( &pWarriorSetup->ControlInfo.PS2_JET,              &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_JET           );
    SetControl( &pWarriorSetup->ControlInfo.PS2_ZOOM,             &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_ZOOM          );
    SetControl( &pWarriorSetup->ControlInfo.PS2_NEXT_WEAPON,      &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_NEXT_WEAPON   );
    SetControl( &pWarriorSetup->ControlInfo.PS2_GRENADE,          &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_GRENADE       );
    SetControl( &pWarriorSetup->ControlInfo.PS2_VOICE_MENU,       &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_VOICE_MENU    );
    SetControl( &pWarriorSetup->ControlInfo.PS2_MINE,             &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_MINE          );
    SetControl( &pWarriorSetup->ControlInfo.PS2_USE_PACK,         &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_USE_PACK      );
    SetControl( &pWarriorSetup->ControlInfo.PS2_ZOOM_IN,          &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_ZOOM_IN       );
    SetControl( &pWarriorSetup->ControlInfo.PS2_ZOOM_OUT,         &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_ZOOM_OUT      );
    SetControl( &pWarriorSetup->ControlInfo.PS2_HEALTH_KIT,       &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_USE_HEALTH    );
    SetControl( &pWarriorSetup->ControlInfo.PS2_TARGETING_LASER,  &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_TARGET_LASER  );
    SetControl( &pWarriorSetup->ControlInfo.PS2_DROP_WEAPON,      &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_WEAPON   );
    SetControl( &pWarriorSetup->ControlInfo.PS2_DROP_PACK,        &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_PACK     );
    SetControl( &pWarriorSetup->ControlInfo.PS2_DROP_BEACON,      &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DEPLOY_BEACON );
    SetControl( &pWarriorSetup->ControlInfo.PS2_DROP_FLAG,        &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_FLAG     );
    SetControl( &pWarriorSetup->ControlInfo.PS2_SUICIDE,          &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_SUICIDE       );
    SetControl( &pWarriorSetup->ControlInfo.PS2_OPTIONS,          &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_OPTIONS       );
    SetControl( &pWarriorSetup->ControlInfo.PS2_TARGET_LOCK,      &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_TARGET_LOCK   );
    SetControl( &pWarriorSetup->ControlInfo.PS2_COMPLIMENT,       &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_COMPLIMENT    );
    SetControl( &pWarriorSetup->ControlInfo.PS2_TAUNT,            &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_TAUNT         );
    SetControl( &pWarriorSetup->ControlInfo.PS2_EXCHANGE,         &pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_EXCHANGE      );
}

//==============================================================================
//  InitWarriorSetup
//==============================================================================

void InitFavorite( player_object::loadout&      Loadout,
                   favorite&                    Favorite )
{
    s32     i;

    // Get Info for Armor Type
    const player_object::move_info& MoveInfo = player_object::GetMoveInfo( player_object::CHARACTER_TYPE_MALE, Favorite.Armor );

    // Init the Loadout
    Loadout.Armor       = Favorite.Armor;
    Loadout.NumWeapons  = Favorite.nWeapons;
    Loadout.Weapons[0]  = Favorite.Weapon1;
    Loadout.Weapons[1]  = Favorite.Weapon2;
    Loadout.Weapons[2]  = Favorite.Weapon3;
    Loadout.Weapons[3]  = Favorite.Weapon4;
    Loadout.Weapons[4]  = Favorite.Weapon5;
    Loadout.VehicleType = Favorite.Vehicle;

    // Pack
    if( Favorite.Pack == player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR )
        Loadout.Inventory.Counts[ Favorite.Pack ] = 2;
    else
        Loadout.Inventory.Counts[ Favorite.Pack ] = 1;

    // Grenades
    Loadout.Inventory.Counts[ Favorite.Grenade ] = MoveInfo.INVENT_MAX[ Favorite.Grenade ];

    // Mines
//    Loadout.Inventory.Counts[ player_object::INVENT_TYPE_MINE ] = 0;//MoveInfo.INVENT_MAX[ player_object::INVENT_TYPE_MINE ];

    // Set Healthkit
    Loadout.Inventory.Counts[player_object::INVENT_TYPE_BELT_GEAR_HEALTH_KIT] = 0;//MoveInfo.INVENT_MAX[player_object::INVENT_TYPE_BELT_GEAR_BEACON];

    // Set Beacons
    Loadout.Inventory.Counts[player_object::INVENT_TYPE_BELT_GEAR_BEACON] = 0;//MoveInfo.INVENT_MAX[player_object::INVENT_TYPE_BELT_GEAR_BEACON];

    // Weapon Ammo Counts
    for( i=0 ; i<5 ; i++ )
    {
        const player_object::weapon_info& WeaponInfo = player_object::GetWeaponInfo( Loadout.Weapons[i] );
        Loadout.Inventory.Counts[ WeaponInfo.AmmoType ] = MoveInfo.INVENT_MAX[ WeaponInfo.AmmoType ];
    }

}

void InitWarriorSetup( warrior_setup& Setup, s32 iWarrior )
{
    s32 i;

    x_memset( &Setup, 0, sizeof(warrior_setup) );
    xwstring Wide( xfs("Warrior %d", iWarrior+1) );
    x_wstrcpy( Setup.Name, Wide );
    Setup.ArmorType     = player_object::ARMOR_TYPE_LIGHT;
    Setup.CharacterType = player_object::CHARACTER_TYPE_MALE;
    Setup.SkinType      = player_object::SKIN_TYPE_BEAGLE;
    Setup.VoiceType     = player_object::VOICE_TYPE_MALE_HERO;

    for( i=0 ; i<FE_TOTAL_FAVORITES ; i++ )
    {
        InitFavorite( Setup.Favorites[i],  Favorites[i]  );
    }

    Setup.ActiveFavoriteCategory    = 0;
    Setup.ActiveFavorite[0]         = 0;
    Setup.ActiveFavorite[1]         = 0;
    Setup.ActiveFavorite[2]         = 0;
    Setup.ActiveFavoriteArmor       = 0;
    Setup.ViewZoomFactor            = 2;
    Setup.LastPauseTab              = 0;
    Setup.KeyboardEnabled           = 0;
    Setup.DualshockEnabled          = 1;
    Setup.InvertPlayerY             = 0;
    Setup.InvertVehicleY            = 0;

    for( i=0 ; i<FE_MAX_BUDDIES ; i++ )
        Setup.Buddies[0][i]             = 0;
    Setup.nBuddies = 0;

    // Load control info
    Setup.ControlInfo.SetDefaults() ;
//    Setup.ControlInfo.LoadFromFile( "Data/Characters/ControlDefines.txt" );
    SetWarriorControlDefault( &Setup, WARRIOR_CONTROL_CONFIG_A );
}

//==============================================================================
//  InitFrontEnd
//==============================================================================

xbool InitFrontEnd( f32 DeltaSec )
{
    (void)DeltaSec;

    // Create a full screen view
    view v;
    eng_MaximizeViewport( v );

    // Restore old view
    eng_SetView( v, 0 );
    eng_ActivateView( 0 );

    // Setup Scissoring
#ifdef TARGET_PS2
    s32 X0,Y0,X1,Y1;
    v.GetViewport( X0,Y0,X1,Y1 );
    gsreg_Begin();
    gsreg_SetScissor(X0,Y0,X1,Y1);
    eng_PushGSContext(1);
    gsreg_SetScissor(X0,Y0,X1,Y1);
    eng_PopGSContext();
    gsreg_End();
#endif

    // Init the UI

#ifdef TARGET_PC
#define DATAPATH "data\\ui\\pc\\"
#else
#define DATAPATH "data\\ui\\ps2\\"
#endif

    if( !s_FrontEndLoaded )
    {
        // Load UI Fonts
#ifdef TARGET_PC
        tgl.pUIManager->LoadFont( "small",  DATAPATH"PC_font_small.xbmp" );
        tgl.pUIManager->LoadFont( "medium", DATAPATH"font_medium.xbmp" );
        tgl.pUIManager->LoadFont( "large",  DATAPATH"font_large.xbmp" );
#else
        tgl.pUIManager->LoadFont( "small",  DATAPATH"font_small.xbmp" );
        tgl.pUIManager->LoadFont( "medium", DATAPATH"font_medium.xbmp" );
        tgl.pUIManager->LoadFont( "large",  DATAPATH"font_large.xbmp" );
#endif

        // Load UI backgrounds
        tgl.pUIManager->LoadBackground( "background1", DATAPATH"background1.xbmp" );

        // Load UI graphic elements
        tgl.pUIManager->LoadElement( "button",         DATAPATH"ui_button.xbmp",          5, 3, 1 );
        tgl.pUIManager->LoadElement( "button_check",   DATAPATH"ui_check.xbmp",           5, 3, 1 );
        tgl.pUIManager->LoadElement( "button_combo1",  DATAPATH"ui_combo1.xbmp",          5, 3, 1 );
        tgl.pUIManager->LoadElement( "button_combo2",  DATAPATH"ui_combo2.xbmp",          5, 3, 1 );
        tgl.pUIManager->LoadElement( "button_combob",  DATAPATH"ui_combob.xbmp",          5, 3, 1 );
        tgl.pUIManager->LoadElement( "button_edit",    DATAPATH"ui_edit.xbmp",            5, 3, 1 );
        tgl.pUIManager->LoadElement( "frame",          DATAPATH"ui_frame.xbmp",           1, 3, 3 );
        tgl.pUIManager->LoadElement( "frame1",         DATAPATH"ui_frame1.xbmp",          1, 3, 3 );
        tgl.pUIManager->LoadElement( "sb_arrowdown",   DATAPATH"ui_sb_arrowdown.xbmp",    5, 1, 1 );
        tgl.pUIManager->LoadElement( "sb_arrowup",     DATAPATH"ui_sb_arrowup.xbmp",      5, 1, 1 );
        tgl.pUIManager->LoadElement( "sb_container",   DATAPATH"ui_sb_container.xbmp",    5, 3, 3 );
        tgl.pUIManager->LoadElement( "sb_thumb",       DATAPATH"ui_sb_thumb.xbmp",        5, 1, 3 );
        tgl.pUIManager->LoadElement( "tab",            DATAPATH"ui_tab.xbmp",             5, 3, 1 );
        tgl.pUIManager->LoadElement( "slider_bar",     DATAPATH"ui_slider_bar.xbmp",      1, 3, 1 );
        tgl.pUIManager->LoadElement( "slider_thumb",   DATAPATH"ui_slider_thumb.xbmp",    5, 1, 1 );
        tgl.pUIManager->LoadElement( "highlight",      DATAPATH"ui_highlight.xbmp",       1, 3, 3 );
        tgl.pUIManager->LoadElement( "controller",     DATAPATH"ui_controller.xbmp",      0, 0, 0 );

        // Register windows classes
        tgl.pUIManager->RegisterWinClass( "joinlist",  &ui_joinlist_factory  );

        // Register Dialog Classes
        dlg_main_menu_register              ( tgl.pUIManager );
//      dlg_online_main_menu_register       ( tgl.pUIManager );
//      dlg_offline_main_menu_register      ( tgl.pUIManager );
//      dlg_options_main_menu_register      ( tgl.pUIManager );
        dlg_warrior_setup_register          ( tgl.pUIManager );
        dlg_controlconfig_register          ( tgl.pUIManager );
        dlg_buddies_register                ( tgl.pUIManager );
        dlg_load_save_register              ( tgl.pUIManager );
        dlg_online_register                 ( tgl.pUIManager );
        dlg_online_host_register            ( tgl.pUIManager );
        dlg_online_join_register            ( tgl.pUIManager );
        dlg_message_register                ( tgl.pUIManager );
        dlg_clientlimits_register           ( tgl.pUIManager );
        dlg_campaign_register               ( tgl.pUIManager );
        dlg_brief_register                  ( tgl.pUIManager );
        dlg_mission_load_register           ( tgl.pUIManager );
        dlg_mission_end_register            ( tgl.pUIManager );
        dlg_sound_test_register             ( tgl.pUIManager );
//        ui_controllist_register             ( tgl.pUIManager );
//        dlg_server_filter_register          ( tgl.pUIManager );
        dlg_audio_options_register          ( tgl.pUIManager );
        dlg_network_options_register        ( tgl.pUIManager );
        dlg_debrief_register                ( tgl.pUIManager );

        // Register Pause menu dialogs
        dlg_objectives_register  ( tgl.pUIManager );
        dlg_controls_register    ( tgl.pUIManager );
        dlg_inventory_register   ( tgl.pUIManager );
        dlg_vehicle_register     ( tgl.pUIManager );
        dlg_score_register       ( tgl.pUIManager );
        dlg_admin_register       ( tgl.pUIManager );
        dlg_debug_register       ( tgl.pUIManager );
        dlg_player_register      ( tgl.pUIManager );

        // Initialize the Warrior Setups

        // Set Front End Loaded
        s_FrontEndLoaded = TRUE;
    }

    // Setup default state
//    fegl.State = FE_STATE_MAIN_MENU;
    tgl.UserInfo.UsingConfig = FALSE;
    tgl.JumpTime = FALSE;

    // Create a user and open the main window, or activate the old user
    if( fegl.UIUserID == 0 )
    {
        irect r;
        s32 width,height;

        eng_GetRes(width,height);
        r.Set( 0, 0, width,height );
		if( r.GetHeight() > 448 )
			r.t += 32;
        fegl.UIUserID = tgl.pUIManager->CreateUser( -1, r );
		tgl.pUIManager->EnableUser(fegl.UIUserID,FALSE);
        ASSERT( fegl.UIUserID );
        tgl.pUIManager->SetUserBackground( fegl.UIUserID, tgl.pUIManager->FindBackground( "background1" ) );
        tgl.pUIManager->OpenDialog( fegl.UIUserID, "main menu", irect(16,16,512-16, 448-16), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER );
    }
    else
    {
        // Remove any Loading or Score Dialogs left around
//        EndMissionDialogs();

        // Enabled the existing user
        tgl.pUIManager->EnableUser( fegl.UIUserID, TRUE );
    }

    // Bypass frontend for testing purposes
#if BYPASS_FRONTEND
    if( BYPASS )
	{
        // Set State
        fegl.State = FE_STATE_ENTER_GAME;

        fegl.NCampaignMissions  = 11;
        fegl.NTrainingMissions  =  5;
        
        GameMgr.SetNCampaignMissions( fegl.NCampaignMissions );

        // Setup the Game Type and Mission
        tgl.NRequestedPlayers   = NUM_PLAYERS;
        fegl.nLocalPlayers      = NUM_PLAYERS;
        fegl.GameType           = GAME_CAMPAIGN;
        //fegl.GameType           = GAME_CTF;
        fegl.CampaignMission    = MISSION;
        
        fegl.ServerSettings.MaxPlayers  = NUM_PLAYERS;
        fegl.ServerSettings.MaxClients  = NUM_PLAYERS-1;

        fegl.ServerSettings.BotsEnabled = FALSE;
        fegl.ServerSettings.BotsNum     = 0;
        GameMgr.SetPlayerLimits( fegl.ServerSettings.MaxPlayers,
                                 fegl.ServerSettings.BotsNum,
                                 fegl.ServerSettings.BotsEnabled );

        if( fegl.GameType == GAME_CAMPAIGN )
        {
            if( fegl.CampaignMission > fegl.NCampaignMissions )
                x_strcpy( fegl.MissionName, xfs( "Training%d", MISSION-fegl.NCampaignMissions ) );
            else
                x_strcpy( fegl.MissionName, xfs( "Mission%02d", fegl.CampaignMission ) );
        }
        else
        {
            x_strcpy( fegl.MissionName, xfs( "Avalon", fegl.CampaignMission ) );
            pSvrMgr->SetAsServer( TRUE );
        }

        pSvrMgr->SetName( xwstring( "Local" ) );
        x_wstrcpy( tgl.UserInfo.MyName, xwstring( "NONAME" ) );
        x_wstrcpy( tgl.UserInfo.Server, pSvrMgr->GetName() );

        pSvrMgr->SetIsInGame( TRUE );
        tgl.UserInfo.IsServer = TRUE;
        tgl.UserInfo.MyAddr = pSvrMgr->GetLocalAddr();
        tgl.UserInfo.UsingConfig = TRUE;
    }
#endif

    return TRUE;
}

//==============================================================================

void KillFrontEnd( void )
{
    // Kill all Users
//    tgl.pUIManager->DeleteAllUsers( );

    tgl.pUIManager->EnableUser( fegl.UIUserID, FALSE );
}

//==============================================================================

xbool UpdateFrontEnd( f32 DeltaSec )
{
    xbool   Iterate;

    (void)DeltaSec;

    // run frontend and fill out UserInfo
    if( tgl.UserInfo.UsingConfig )
        return TRUE;

    tgl.pUIManager->ProcessInput( DeltaSec );
    tgl.pUIManager->Update( DeltaSec/2.0f );
    tgl.pUIManager->Update( DeltaSec/2.0f );


    // Front End Logic
    do
    {
        Iterate = FALSE;

        switch( fegl.State )
        {
        // Either enter online if all setup or enter warrior setup
        case FE_STATE_GOTO_WARRIOR_SETUP:
            if( fegl.iWarriorSetup >= fegl.nLocalPlayers )
            {
                fegl.State = fegl.GotoGameState;
                Iterate = TRUE;
            }
            else
            {
                fegl.State = FE_STATE_WARRIOR_SETUP;
                ui_dialog* pDialog = NULL;
                if( fegl.iWarriorSetup == 0 )
                    pDialog = tgl.pUIManager->OpenDialog( fegl.UIUserID, "warrior setup", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
                else
                    pDialog = tgl.pUIManager->OpenDialog( fegl.UIUserID, "warrior2 setup", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
                ASSERT( pDialog );
                xwchar  Message[64];
                x_wstrcpy( Message, StringMgr( "ui", IDS_WARRIOR_SETUP_PLAYER_N ) );
                xwchar  Number   = xwchar('0') + (fegl.iWarriorSetup+1);
                s32     Len      = x_wstrlen( Message );
                Message[Len-1] = Number;
                pDialog->SetLabel( Message );
            }
            break;

        // Enter online
        case FE_STATE_GOTO_ONLINE:
            fegl.State = FE_STATE_ONLINE;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "online", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER|ui_win::WF_NO_ACTIVATE );
            break;

        case FE_STATE_GOTO_OFFLINE:
            fegl.State = FE_STATE_OFFLINE;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "online", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER|ui_win::WF_NO_ACTIVATE );
            break;

        // Enter campaign
        case FE_STATE_GOTO_CAMPAIGN:
            fegl.State = FE_STATE_CAMPAIGN;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "campaign", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
            break;

        // Enter campaign briefing
        case FE_STATE_GOTO_CAMPAIGN_BRIEFING:
            fegl.State = FE_STATE_CAMPAIGN_BRIEFING;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "brief", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
            break;

        case FE_STATE_GOTO_SOUNDTEST:
            fegl.State = FE_STATE_SOUNDTEST;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "sound test", irect(0,0,376,400), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL);
            break;

        case FE_STATE_GOTO_OPTIONS:
            fegl.State = FE_STATE_OPTIONS;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "options", irect(72,16,512-72,448-16), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER);
            break;

        // Options menu, leads to  audio and network settings.
        case FE_STATE_GOTO_OPTIONS_MAIN_MENU:
            fegl.State = FE_STATE_OPTIONS_MAIN_MENU;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "options main menu", irect(16,16,512-16, 448-16), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
            break;

        // Audio options.
        case FE_STATE_GOTO_AUDIO_OPTIONS:
            fegl.State = FE_STATE_AUDIO_OPTIONS;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "audio options", irect(104,112,104+304,320), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER );
            break;

        // Network options.
        case FE_STATE_GOTO_NETWORK_OPTIONS:
            fegl.State = FE_STATE_NETWORK_OPTIONS;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "network options", irect(48,112,462,320), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER );
            break;

        // Online mode, holds one and two player online games.
        case FE_STATE_GOTO_ONLINE_MAIN_MENU:
            fegl.State = FE_STATE_ONLINE;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "online main menu", irect(16,16,512-16, 448-16), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
            break;

        // Offline mode, holds one player, two player and campaign mode.
        case FE_STATE_GOTO_OFFLINE_MAIN_MENU:
            fegl.State = FE_STATE_OFFLINE;
            tgl.pUIManager->OpenDialog( fegl.UIUserID, "offline main menu", irect(16,16,512-16, 448-16), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
            break;

        }
    } while( Iterate );

    return FALSE;
}

//==============================================================================

void AdvanceMission( void )
{
    mission_def* pMissionDef = fegl.pMissionDef;

    ASSERT( pMissionDef );

    #ifdef DEMO_DISK_HARNESS
    // Do not advance mission if within demo harness.
    #else

    // Is a next mission defined?
    if( fegl.pNextMissionDef )
    {
        // Set new Mission
        pMissionDef = fegl.pNextMissionDef;
        fegl.pNextMissionDef = NULL;
    }
    else
    {
        // Can't advance if no MissionDef
        if( pMissionDef )
        {
            // Seek for next mission of same type
            pMissionDef++;
            while( pMissionDef->GameType != fegl.GameType )
            {
                // Check for looping in table
                if( pMissionDef->GameType == GAME_NONE )
                    pMissionDef = &fe_Missions[0];
                else
                    pMissionDef++;
            }
        }
    }

    #endif

    // Save off mission data
    fegl.pMissionDef = pMissionDef;
    x_strcpy( fegl.MissionName, pMissionDef->Folder );
}

//==============================================================================

void GetMissionDefFromNameAndType( void )
{
    mission_def*    pFoundMissionDef = NULL;
    mission_def*    pMissionDef      = fe_Missions;

    while( pMissionDef->GameType != GAME_NONE )
    {
        if( (pMissionDef->GameType == fegl.GameType) &&
            (x_strcmp( pMissionDef->Folder, fegl.MissionName ) == 0) )
        {
            pFoundMissionDef = pMissionDef;
        }

        pMissionDef++;
    }

    fegl.pMissionDef = pFoundMissionDef;
}

//==============================================================================

void ShowMissionLoadDialog( void )
{
    // Check if Mission Load dialog is already on the stack
    if( !fegl.MissionLoadDialogActive && !fegl.MissionEndDialogActive )
    {
        // Setup Mission Loading Dialog
        VERIFY( tgl.pUIManager->OpenDialog( fegl.UIUserID, "mission load", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER ) );
        tgl.pUIManager->EnableUser( fegl.UIUserID, TRUE );
    }
}

//==============================================================================

void ShowMissionEndDialog( xbool AllowContinue )
{
    if( fegl.State != FE_STATE_ENTER_CAMPAIGN )
    {
        // Display Game Over screen
        VERIFY( tgl.pUIManager->OpenDialog( fegl.UIUserID, "mission end", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER, &AllowContinue ) );
    }
    else if( tgl.MissionSuccess &&  (fegl.LastPlayedCampaign > 5) )
    {
        tgl.pUIManager->OpenDialog( fegl.UIUserID, "debrief", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
    }

    tgl.pUIManager->EnableUser( fegl.UIUserID, TRUE );
}

//==============================================================================

void EndMissionDialogs( void )
{
    xbool               done    = FALSE;
    s32                 UserID  = fegl.UIUserID;
    ui_manager::user*   pUser   = tgl.pUIManager->GetUser( UserID );

    // Loop through mission dialogs popping off stack
    while( (pUser->DialogStack.GetCount() > 0) && !done )
    {
        ui_dialog* pDialog =  pUser->DialogStack[pUser->DialogStack.GetCount()-1];
        ui_manager::dialog_tem* pTemplate = pDialog->GetTemplate();

        if( (pTemplate == &MissionLoadDialog) || 
            (pTemplate == &MissionEndDialog)  || 
            (pTemplate == &BriefDialog) )
        {
            tgl.pUIManager->EndDialog( UserID, TRUE );
        }
        else
            done = TRUE;
    }

    // Hide the front end
    tgl.pUIManager->EnableUser( fegl.UIUserID, FALSE );
}

//==============================================================================

extern ui_manager::dialog_tem MainMenuDialog;

void GotoMainMenuDialog( void )
{
    xbool               done    = FALSE;
    s32                 UserID  = fegl.UIUserID;
    ui_manager::user*   pUser   = tgl.pUIManager->GetUser( UserID );

    // Loop through mission dialogs popping off stack
    while( (pUser->DialogStack.GetCount() > 0) && !done )
    {
        ui_dialog* pDialog =  pUser->DialogStack[pUser->DialogStack.GetCount()-1];
        ui_manager::dialog_tem* pTemplate = pDialog->GetTemplate();

        if( pTemplate != &MainMenuDialog )
        {
            tgl.pUIManager->EndDialog( UserID, TRUE );
        }
        else
            done = TRUE;
    }
}
