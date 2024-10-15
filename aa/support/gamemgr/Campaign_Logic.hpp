//=============================================================================
//
//  Campaign Logic
//
//=============================================================================

#ifndef CAMPAIGN_LOGIC_HPP
#define CAMPAIGN_LOGIC_HPP

//=============================================================================
//  INCLUDES
//=============================================================================

#include "GameLogic.hpp"
#include "Objects\Player\PlayerObject.hpp"
#include "Objects\Bot\BotObject.hpp"
#include "Demo1\Data\Missions\Campaign.h"
#include "StringMgr\StringMgr.hpp"

//=============================================================================
//  DEFINES
//=============================================================================

const u32 ALLIED_TEAM_BITS      =   0x01;
const u32 ENEMY_TEAM_BITS       =   0x02;

const s32 ATTACK_WAVE_ALL       =  -1;
const s32 ATTACK_WAVE_1         =   0;
const s32 ATTACK_WAVE_2         =   1;
const s32 ATTACK_WAVE_3         =   2;

const s32 MAX_BOT_SPAWN_POINTS  =  32;
const s32 MAX_BOT_PLAYERS       =  16;
const s32 MAX_VEHICLE_WAYPOINTS =  32;
const s32 MAX_WAYPOINTS         =   8;

const s32 STATE_IDLE            =   0;

enum team
{
    TEAM_ALLIES,
    TEAM_ENEMY,
    MAX_TEAMS
};

enum task
{
    TASK_ATTACK,
    TASK_GOTO,
    TASK_DEFEND,
    TASK_PATROL,
    TASK_MORTAR,
    TASK_REPAIR,
    TASK_DESTROY,
    TASK_DEATHMATCH,
    TASK_ANTAGONIZE,
};

#define  PAD_RIGHT_STICK_UP         ( 1 <<  0 )
#define  PAD_RIGHT_STICK_DOWN       ( 1 <<  1 )
#define  PAD_RIGHT_STICK_LEFT       ( 1 <<  2 )
#define  PAD_RIGHT_STICK_RIGHT      ( 1 <<  3 )
#define  PAD_LEFT_STICK_UP          ( 1 <<  4 )
#define  PAD_LEFT_STICK_DOWN        ( 1 <<  5 )
#define  PAD_LEFT_STICK_LEFT        ( 1 <<  6 )
#define  PAD_LEFT_STICK_RIGHT       ( 1 <<  7 )
#define  PAD_JUMP_BUTTON            ( 1 <<  8 )
#define  PAD_JET_BUTTON             ( 1 <<  9 )
#define  PAD_FIRE_BUTTON            ( 1 << 10 )
#define  PAD_EXCHANGE_WEAPON_BUTTON ( 1 << 11 )
#define  PAD_NEXT_WEAPON_BUTTON     ( 1 << 12 )
#define  PAD_ZOOM_BUTTON            ( 1 << 13 )
#define  PAD_ZOOM_IN_BUTTON         ( 1 << 14 )
#define  PAD_ZOOM_OUT_BUTTON        ( 1 << 15 )
#define  PAD_DROP_GRENADE_BUTTON    ( 1 << 16 )
#define  PAD_USE_PACK_BUTTON        ( 1 << 17 )
#define  PAD_VIEW_CHANGE_BUTTON     ( 1 << 18 )
#define  PAD_AUTOLOCK_BUTTON        ( 1 << 19 )
#define  PAD_SPECIAL_JUMP_BUTTON    ( 1 << 20 )

#define  PAD_RIGHT_STICK            ( PAD_RIGHT_STICK_UP     | PAD_RIGHT_STICK_DOWN   | PAD_RIGHT_STICK_LEFT   | PAD_RIGHT_STICK_RIGHT )
#define  PAD_LEFT_STICK             ( PAD_LEFT_STICK_UP      | PAD_LEFT_STICK_DOWN    | PAD_LEFT_STICK_LEFT    | PAD_LEFT_STICK_RIGHT  )
#define  PAD_CHANGE_WEAPON_BUTTONS  ( PAD_NEXT_WEAPON_BUTTON | PAD_EXCHANGE_WEAPON_BUTTON )

//=============================================================================
//  TYPES
//=============================================================================

class campaign_logic : public game_logic
{

//-----------------------------------------------------------------------------
//  Public Functions
//-----------------------------------------------------------------------------
    
public:

                        campaign_logic          ( void );
                       ~campaign_logic          ( void );
        
static  campaign_logic* GetCampaignLogic        ( s32 CampaignMission );
static  campaign_logic* GetTrainingLogic        ( s32 TrainingMission );

virtual void            PlayerDied              ( const pain& Pain );
virtual void            PlayerOnVehicle         ( object::id PlayerID, object::id VehicleID );
virtual void            SwitchTouched           ( object::id SwitchID, object::id PlayerID  ); 
virtual void            PickupTouched           ( object::id PickupID, object::id PlayerID  );
virtual void            Render                  ( void );
virtual void            EnforceBounds           ( f32 DeltaTime );

static  void            SetCampaignObjectives   ( s32 Index );
        void            PrintObjective          ( s32 Index );
        void            ErrorText               ( s32 Index );
        void            ClearErrorText          ( void );
        void            GameInfo                ( s32 Index );
        
//-----------------------------------------------------------------------------
//  Private Types
//-----------------------------------------------------------------------------

protected:

    struct spawnpt
    {
        object::id WaypointID;  // object ID of the actual spawn point in the world
    
        team Team;              // the team this spawn point belongs to
        u8   Wave;              // the attack wave number
        u8   nLight;            // number of light armored bots
        u8   nMedium;           // number of medium armored bots
        u8   nHeavy;            // number of heavy armored bots
        u8   nTotal;            // total number of bots this spawn point will create
    };

//-----------------------------------------------------------------------------
//  Private Functions
//-----------------------------------------------------------------------------

protected:

virtual void            Initialize              ( void );
virtual void            BeginGame               ( void );
virtual void            EndGame                 ( void );
virtual void            EnterGame               ( s32 PlayerIndex );
virtual void            ExitGame                ( s32 PlayerIndex );
virtual void            AdvanceTime             ( f32 DeltaTime );
        void            Success                 ( s32 ID );
        void            Failed                  ( s32 ID );

        xbool           AddSpawnPoint           ( object::id ItemID,  const char *pTag );
        void            InitSpawnPoint          ( object::id ItemID,  team Team, s32 Wave, s32 nLight, s32 nMedium, s32 nHeavy );
        
        f32             DistanceToObject        ( object::id FromID, object::id ToID );
        f32             DistanceToObject        ( object::id ID );
        f32             DistanceToClosestPlayer ( object::id ID, team Team );

        object::id      CreateBot               ( vector3&                   Position,
                                                  bot_object::character_type CharType,
                                                  bot_object::skin_type      SkinType,
                                                  bot_object::voice_type     VoiceType,
                                                  default_loadouts::type     LoadoutType,
                                                  s32 Wave = ATTACK_WAVE_1 );
        
        void            DestroyBot              ( object::id   BotID );
        void            AddBot                  ( team Team,   object::id BotID );
        void            RemoveBot               ( object::id   BotID ); 
        void            RemoveDeadBots          ( object::id* pBotID );
        
        void            SpawnBots               ( team Team, s32 Wave );
        void            SpawnAllies             ( s32  Wave );
        void            SpawnAllies             ( void );
        void            SpawnEnemies            ( s32  Wave );
        void            SpawnEnemies            ( void );
        
        void            OrderBots               ( team Team, task Task, object::id ObjectID, u32 WaveBits );
        void            OrderAllies             ( task Task, object::id ObjectID, s32 Wave = ATTACK_WAVE_ALL );
        void            OrderAllies             ( task Task, s32 Wave = ATTACK_WAVE_ALL );
        void            OrderEnemies            ( task Task, object::id ObjectID, s32 Wave = ATTACK_WAVE_ALL );
        void            OrderEnemies            ( task Task, s32 Wave = ATTACK_WAVE_ALL );
        
        void            AddWaypoint             ( object::id ID );
        void            ClearWaypoint           ( object::id ID );

        void            SetState                ( s32 State );
        void            GamePrompt              ( s32 ID, xbool Override = FALSE );
        void            GameAudio               ( s32 ID );
        void            GameText                ( const xwchar* pText );
        void            ClearText               ( void );
        
        void            GetLaserTargets         ( void );
        
        object::id      InitVehicle             ( object::type Type );
        xbool           AddVehiclePoint         ( object::id ItemID, const char* pTag );
        xbool           LaunchBombs             ( vehicle* pVehicle );
        xbool           UpdateVehicle           ( object::id VehicleID, f32 DeltaTime );
        void            FireVehicleWeapon       ( void );
        void            ControlBot              ( object::id BotID, player_object::move& Movement );
        void            RenderVehiclePoints     ( void );

        void            DestroyItem             ( object::id ItemID );
        void            PowerLoss               ( object::id ItemID );
        void            PowerGain               ( object::id ItemID );
        
        void            EnablePadButtons        ( u32 PadBits );
        void            DisablePadButtons       ( u32 PadBits );
        f32             GetStickValue           ( u32 PadBits );
        xbool           IsStickPressed          ( u32 PadBits, f32 Sec = 0.0f );
        xbool           IsButtonPressed         ( u32 PadBits, f32 Sec = 0.0f );

        void            SetHealth               ( f32 Health );
        void            SetInventCount          ( player_object::invent_type InventType, s32 Count );
        s32             GetInventCount          ( player_object::invent_type InventType );
        s32             GetNumWeaponsHeld       ( void );
        player_object::
        invent_type     GetCurrentWeapon        ( void );
        xbool           HasWeaponFired          ( player_object::invent_type Type );
        xbool           IsItemHeld              ( player_object::invent_type Item );
        xbool           IsPlayerShielded        ( void ) const;
        xbool           IsPlayerArmed           ( void ) const;
const   player_object::
        loadout&        GetLoadout              ( void ) const;
const   player_object::
        loadout&        GetInventoryLoadout     ( void ) const;
        xbool           IsAttachedToVehicle     ( object::type VehicleType = object::TYPE_NULL ) const;
        xbool           IsItemDeployed          ( object::type Type ) const;
        s32             GetVehicleSeat          ( void ) const;
        void            SetVehicleLoadout       ( object::type VehicleType );
        
        void            CreatePickup            ( pickup::kind PickupType, const vector3& Position, xbool IsRespawning = FALSE );
        void            CreatePickup            ( pickup::kind PickupType, object::id ObjectID,     xbool IsRespawning = FALSE );
        void            CreateBeacon            ( object::id ObjectID );
        void            RemoveBeacon            ( object::id ObjectID );
        
        xbool           IsVehiclePresent        ( object::type VehicleType ) const;
        void            SetVehicleLimits        ( s32 MaxCycles, s32 MaxShrikes, s32 MaxBombers, s32 MaxTransports );
        
//-----------------------------------------------------------------------------
//  Private Data
//-----------------------------------------------------------------------------

protected:

    xbool           m_bInitialized;
    xbool           m_bExit;
    s32             m_GameState;
    f32             m_Time;
    f32             m_Timeout;
    s32             m_ChannelID;
    
    s32             m_nAllies;
    s32             m_nEnemies;

    object::id      m_AlliesID [ MAX_BOT_PLAYERS ];
    object::id      m_EnemiesID[ MAX_BOT_PLAYERS ];
    object::id      m_PlayerID;
    object::id      m_VehicleID;

    s32             m_nWaypoints;
    object::id      m_Waypoints[MAX_WAYPOINTS][2];

    s32             m_nSpawnPoints;
    spawnpt         m_SpawnPoints[ MAX_BOT_SPAWN_POINTS ];
    
    xbool           m_bPlayerDied;
    xbool           m_bPlayerOnVehicle;
    xbool           m_bAlliedDied;

    xbool           m_bSwitchTouched;
    xbool           m_bSwitchLost;
    team            m_SwitchState;
    object::id      m_SwitchID;

    xbool           m_bPickupTouched;
    pickup::kind    m_PickupType;

    team            m_TargetTeam;

    xbool           m_bHasRepairPack;
    xbool           m_bPickedUpRepairPack;

    object::id      m_TargetID;
    xbool           m_bNewTarget;
    
    s32             m_nVehPoints;
    s32             m_NextVehPoint;
    object::id      m_VehPoints[ MAX_VEHICLE_WAYPOINTS ];

    object::id      m_PilotID;
    xbool           m_bFireWeapon;
    xbool           m_bPathCompleted;
    
    f32             m_VehicleDelay;

    player_object::armor_type m_DeadArmorType;
    
    xbool           m_bWeaponFired;
};

extern bot_object::bot_debug g_BotDebug;

extern xbool g_AutoWinMission;

//=============================================================================
#endif
//=============================================================================


