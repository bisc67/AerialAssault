//=========================================================================
//
//  hud_manager.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "demo1/Globals.hpp"

#include "GameMgr/GameMgr.hpp"
#include "ObjectMgr/ObjectMgr.hpp"

#include "hud_manager.hpp"
#include "ui/ui_manager.hpp"
#include "ui/ui_font.hpp"
#include "demo1\data\ui\ui_strings.h"
#include "demo1\data\missions\campaign.h"

#include "StringMgr/StringMgr.hpp"

#include "objects/vehicles/vehicle.hpp"
#include "objects/vehicles/bomber.hpp"
#include "objects/vehicles/asstank.hpp"
#include "objects/projectiles/satchelcharge.hpp"

#include "../../demo1/fe_colors.hpp"

//=========================================================================
//  Defines
//=========================================================================

// Health Energy Display

const s32 HealthEnergyWidth     = 128;      // Real Width:   91
const s32 HealthEnergyHeight    =  64;      // Real Height:  34
const s32 HealthMeterOffsetX    =   0;
const s32 HealthMeterOffsetY    =  18;

const s32 HealthBarOffsetX      =  17;
const s32 HealthBarOffsetY      =   5;
const s32 HealthBarWidth        =  60;
const s32 HealthBarHeight       =   6;

const s32 EnergyBarOffsetX      =  17;
const s32 EnergyBarOffsetY      =  16;
const s32 EnergyBarWidth        =  60;
const s32 EnergyBarHeight       =   6;

const s32 HeatBarOffsetX        =  17;
const s32 HeatBarOffsetY        =  24;
const s32 HeatBarWidth          =  60;
const s32 HeatBarHeight         =   4;

// 1 Player Score Board

const s32 ScoreBoardWidth       =  86;
const s32 ScoreBoardHeight      = 116;
const s32 ScoreBoardTextWidth   =  54;
const s32 ScoreBoardTextHeight  =  11;
const s32 ScoreBoardOffsetY     =   8;
const s32 ScoreLabelOffsetX     =  18;
const s32 ScoreLabelOffsetY     =   6;
const s32 ScoreValueOffsetY     =  23;
const s32 FlagsLabelOffsetY     =  40;
const s32 FlagsValueOffsetY     =  57;
const s32 TimerValueOffsetX     =  10;      // Position of clock relative to top left corner of scoreboard
const s32 TimerValueOffsetY     =  80;
const s32 TimerValueOffsetCX    =  26;      // Position of clock relative to top left corner of clock piece
const s32 TimerValueOffsetCY    =  13;

// 2 Player Score Board

const s32 ScoreBoard00Width     =  79;
const s32 ScoreBoard02Width     =  93;
const s32 ScoreBoard00Height    =  47;
const s32 ScoreBoard01Height    =  33;
const s32 ScoreBoard02Height    =  26;
const s32 ScoreBoard03Height    =  33;
const s32 ScoreBoard04Height    =  50;

const s32 ScoreLabel1OffsetX    =  13;      // Player 1 score positions
const s32 ScoreLabel1OffsetY    =  13;
const s32 ScoreValue1OffsetX    =   5;
const s32 ScoreValue1OffsetY    =  29;
const s32 FlagsLabel1OffsetX    =  13;
const s32 FlagsLabel1OffsetY    =   0;
const s32 FlagsValue1OffsetX    =   5;
const s32 FlagsValue1OffsetY    =  15;

const s32 ScoreLabel2OffsetX    =  13;      // Player 2 score positions
const s32 ScoreLabel2OffsetY    =  -1;
const s32 ScoreValue2OffsetX    =   5;
const s32 ScoreValue2OffsetY    =  15;
const s32 FlagsLabel2OffsetX    =  13;
const s32 FlagsLabel2OffsetY    =  -1;
const s32 FlagsValue2OffsetX    =   5;
const s32 FlagsValue2OffsetY    =  15;

const s32 TimerValue1OffsetX    =  34;      // Position of clock relative to top left corner of middle piece
const s32 TimerValue1OffsetY    =   5;

const s32 LedGlowWidth          =  68;      // Scale glow up a bit (an extra 4 pixels)
const s32 LedGlowHeight         =  64;
const s32 LedGlowRealHeight     =  50;

// Inventory

const s32 StatBarOffsetX        =  19;      // Position of statbar relative to top left corner of scoreboard
const s32 StatBarOffsetY        = 106;
const s32 StatBarOffsetCX       =  19;      // Position of statbar relative to top left corner of clock piece
const s32 StatBarOffsetCY       =  39;

const s32 StatBar1OffsetX       =  12;      // Position of statbar relative to top left corner of scoreboard00 piece
const s32 StatBar1OffsetY       =  10;
const s32 StatBar2OffsetX       =  12;      // Position of statbar relative to top left corner of scoreboard04 piece
const s32 StatBar2OffsetY       =  39;
const s32 StatBarWidth          = 128;
const s32 StatBarHeight         = 256;
const s32 StatBarRealHeight     = 180;
const s32 StatBarMaxIcons       =   5;

const s32 InventoryOffsetX      =  11;      // Position of icon rect relative to top left corner of statbar
const s32 InventoryOffsetY      =  24;
const s32 Inventory1OffsetY     =-128;      // Offset to top icon for player 1 in split screen
const s32 Inventory2OffsetY     =  29;      // Offset to top icon for player 2 in split screen
const s32 InventoryIconShiftX   =  10;      // Amount to move icons relative to text value
const s32 InventoryIconShiftY   =   8;
const s32 InventoryIconAddY     =  31;      // Number of lines separating the top of icons
      
const s32 FlagIconTextX         =  16;      // Offset to position text under icon
const s32 FlagIconTextY         =  29;

// Voting

const s32 VotePipeWidth         =  256;
const s32 VotePipeHeight        =   16;
const s32 VotePipeOffsetY       =  -32;     // Offset from bottom of screen (1 player)
const s32 VotePipeLStop         =   32;     // Minimum width of left pipe   (2 player)
const s32 VotePipeRStop         =  112;     // Minimum width of right pipe  (2 player)
const f32 VotePipeAccel         = 8192;
const s32 VoteFadeSpeed         =    6;
const s32 VoteTrackRealWidth    =   16;
const s32 VoteTrackWidth        =    3;
const s32 VoteSlatHeight        =    4;
const s32 VoteSlatRealHeight    =   16;
const f32 VoteSlatTime          =    0.1f;
const f32 VoteBoxHeight         =   16;
const f32 VotePauseTime         =    0.1f;
const s32 VotesNeededBarWidth   =    1;
const s32 VoteCountOffsetX      =   10;

// Vehicle

const s32 VehiclePanelOffsetX   =    0;
const s32 VehiclePanelOffsetY   =   15;
const s32 VehiclePanelAddY      =    8;
const s32 VehicleGlass1OffsetX  =    2;
const s32 VehicleGlass1OffsetY  =   32;
const s32 VehicleGlass2OffsetX  =    2;
const s32 VehicleGlass2OffsetY  =   42;
const s32 VehicleIconOffsetX    =   14;
const s32 VehicleIconOffsetY    =    0;

const s32 VehicleHealthOffsetX  =   18;
const s32 VehicleHealthOffsetY  =    8;
const s32 VehicleEnergyOffsetX  =   18;
const s32 VehicleEnergyOffsetY  =   18;
const s32 VehicleWeaponOffsetX  =   18;
const s32 VehicleWeaponOffsetY  =   28;
const s32 VehicleBarWidth       =   58;
const s32 VehicleBarHeight      =    5;

const s32 VehiclePanelWidth     =  128;
const f32 VehiclePanelTime      =    0.75f;

// HUD Training Glows

const s32 HealthMeterGlowX      =  -17;
const s32 HealthMeterGlowY      =  -16;
const s32 ScoreBoardGlowX       =  -42;
const s32 ScoreBoardGlowY       =   -1;
const s32 StatBarGlowX          =  -28;
const s32 StatBarGlowY          =  -36;
const s32 ReticleGlowX          =  -64;
const s32 ReticleGlowY          =  -64;

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

volatile s32 OVERLAY_ALPHA = 160;

u32 s_ReticleColors[] =
{
    ARGB(224,161,211,245),       // HUD_WEAPON_NONE
    ARGB(224,161,211,245),       // HUD_WEAPON_DISK_LAUNCHER
    ARGB(224,161,211,245),       // HUD_WEAPON_PLASMA_GUN
    ARGB(224,255,  0,  0),       // HUD_WEAPON_SNIPER_RIFLE
    ARGB(224,161,211,245),       // HUD_WEAPON_MORTAR_GUN
    ARGB(224,161,211,245),       // HUD_WEAPON_GRENADE_LAUNCHER
    ARGB(224,161,211,245),       // HUD_WEAPON_CHAIN_GUN
    ARGB(224,161,211,245),       // HUD_WEAPON_BLASTER
    ARGB(224,161,211,245),       // HUD_WEAPON_ELF_GUN
    ARGB(224,161,211,245),       // HUD_WEAPON_MISSILE_LAUNCHER
    ARGB(224,161,211,245),       // HUD_WEAPON_REPAIR_GUN
    ARGB(224,161,211,245),       // HUD_WEAPON_SHOCKLANCE
    ARGB(224,  0,255,  0),       // HUD_WEAPON_TARGETING_LASER
    ARGB(224,238,156,  0),       // HUD_WEAPON_SHRIKE
    ARGB(224,238,156,  0),       // HUD_WEAPON_ASSAULT_GUN
    ARGB(224,238,156,  0),       // HUD_WEAPON_ASSAULT_SHELL
};

xbool   HUD_DebugInfo   = FALSE;
xbool   HUD_HideHUD     = FALSE;
xcolor  HUD_DebugColor(255,255,255,255);

xcolor  HUD_HealthCol    (  42, 178,  46, 255 );
xcolor  HUD_EnergyCol    (  55,  57, 178, 255 );
xcolor  HUD_HeatCol      ( 149,  47,  47, 255 );
xcolor  HUD_StatBarCol   ( 255, 255, 255,  70 );
xcolor  HUD_ScoreLabelCol( 128, 192, 255, 192 );
xcolor  HUD_ScoreValueCol(   0,   0,   0, 255 );
xcolor  HUD_TimerValueCol( 255, 255, 255, 192 );
xcolor  HUD_NormalGlowCol( 160, 192, 255, 255 );
xcolor  HUD_TargetGlowCol( 255, 255,  80, 255 );
xcolor  HUD_LockedGlowCol( 255,  80,  80, 255 );
xcolor  HUD_IconGlowCol  ( 180, 220, 255, 255 );
xcolor  HUD_ZeroGlowCol  ( 255, 100, 100, 255 );
xcolor  HUD_VoteTextCol  ( 255, 255, 255, 224 );
xcolor  HUD_VoteBoxCol   (   0,   0,   0, 255 );
xcolor  HUD_VotePosCol   (  63, 127,  63, 255 );
xcolor  HUD_VoteNegCol   ( 127,  31,  31, 255 );
xcolor  HUD_PulseCol     ( 150, 255, 200, 255 );
xcolor  HUD_FlagTextCol  ( 100, 255, 255, 255 );
xcolor  HUD_ShieldedCol  ( 100, 150, 200, 255 );

static s32 s_VoteStates[2][11]=
{
    {
        VOTE_OPEN_FULL,
        VOTE_CLOSING,
        VOTE_PAUSE,
        VOTE_OPENING_VOTEBOX,
        VOTE_OPENING_SLATS,
        VOTE_VOTING,
        VOTE_CLOSING_SLATS,
        VOTE_CLOSING_VOTEBOX,
        VOTE_PAUSE,
        VOTE_OPENING,
        VOTE_DONE,
    },
    {
        VOTE_CLOSED,
        VOTE_OPENING_VOTEBOX,
        VOTE_OPENING_SLATS,
        VOTE_VOTING,
        VOTE_CLOSING_SLATS,
        VOTE_CLOSING_VOTEBOX,
        VOTE_DONE,
    }
};

xbool TestVoting = FALSE;
xbool TestChat   = FALSE;
xbool DrawGuard  = FALSE;

//=========================================================================
//  Helpers
//=========================================================================

//=========================================================================
//  hud_manager
//=========================================================================

extern xbool DO_SCREEN_SHOTS;
extern xbool CYEDIT_ON;
hud_manager::hud_manager( void )
{
    // Clear Data
    m_pUIManager = NULL;
}

//=========================================================================

hud_manager::~hud_manager( void )
{
    Kill();
}

//=========================================================================

void hud_manager::Init( ui_manager* pUIManager )
{
    s32     i;

    ASSERT( pUIManager );

    // Set Data
    m_pUIManager = pUIManager;
    m_Time       = 0;


#ifdef TARGET_PS2
#define DATA_PATH "ps2"
#endif

#ifdef TARGET_PC
#define DATA_PATH "pc"
#endif

    Initialize();
    // Load HUD graphics
    m_Bmp[ HUD_BMP_HEALTH_METER    ].Load( "data/hud/"DATA_PATH"/hud_health_meter.xbmp"            );
    m_Bmp[ HUD_BMP_1P_STATBAR      ].Load( "data/hud/"DATA_PATH"/hud_1p_statbar.xbmp"              );
    m_Bmp[ HUD_BMP_1P_SCOREBOARD   ].Load( "data/hud/"DATA_PATH"/hud_1p_scoreboard.xbmp"           );
    m_Bmp[ HUD_BMP_1P_SCOREBOARD_B ].Load( "data/hud/"DATA_PATH"/hud_1p_scoreboard_blank.xbmp"     );
    m_Bmp[ HUD_BMP_1P_SCOREBOARD_C ].Load( "data/hud/"DATA_PATH"/hud_1p_scoreboard_clock.xbmp"     );
    m_Bmp[ HUD_BMP_2P_SCOREBOARD00 ].Load( "data/hud/"DATA_PATH"/hud_2p_scoreboard00.xbmp"         );
    m_Bmp[ HUD_BMP_2P_SCOREBOARD01 ].Load( "data/hud/"DATA_PATH"/hud_2p_scoreboard01.xbmp"         );
    m_Bmp[ HUD_BMP_2P_SCOREBOARD02 ].Load( "data/hud/"DATA_PATH"/hud_2p_scoreboard02.xbmp"         );
    m_Bmp[ HUD_BMP_2P_SCOREBOARD03 ].Load( "data/hud/"DATA_PATH"/hud_2p_scoreboard03.xbmp"         );
    m_Bmp[ HUD_BMP_2P_SCOREBOARD04 ].Load( "data/hud/"DATA_PATH"/hud_2p_scoreboard04.xbmp"         );
    m_Bmp[ HUD_BMP_2P_STATBAR      ].Load( "data/hud/"DATA_PATH"/hud_2p_statbar.xbmp"              );
    m_Bmp[ HUD_BMP_LED_GLOW        ].Load( "data/hud/"DATA_PATH"/hud_led_glow.xbmp"                );

    m_Bmp[ HUD_BMP_HEALTH_METER_G  ].Load( "data/hud/"DATA_PATH"/hud_health_meter_glow.xbmp"       );
    m_Bmp[ HUD_BMP_1P_SCOREBOARD_G ].Load( "data/hud/"DATA_PATH"/hud_1p_scoreboard_glow.xbmp"      );
    m_Bmp[ HUD_BMP_1P_STATBAR_G    ].Load( "data/hud/"DATA_PATH"/hud_1p_statbar_glow.xbmp"         );
    m_Bmp[ HUD_BMP_RETICLE_G       ].Load( "data/hud/"DATA_PATH"/hud_ret_base_glow.xbmp"           );

    m_Bmp[ HUD_BMP_BEACON          ].Load( "data/hud/"DATA_PATH"/hud_icon_beacon.xbmp"             );
    m_Bmp[ HUD_BMP_GRENADE         ].Load( "data/hud/"DATA_PATH"/hud_icon_grenade.xbmp"            );
    m_Bmp[ HUD_BMP_GRENADE_FRAG    ].Load( "data/hud/"DATA_PATH"/hud_icon_grenade_fragment.xbmp"   );
    m_Bmp[ HUD_BMP_GRENADE_FLARE   ].Load( "data/hud/"DATA_PATH"/hud_icon_grenade_flare.xbmp"      );
    m_Bmp[ HUD_BMP_GRENADE_CONCUSS ].Load( "data/hud/"DATA_PATH"/hud_icon_grenade_concussion.xbmp" );
    m_Bmp[ HUD_BMP_MINE            ].Load( "data/hud/"DATA_PATH"/hud_icon_mine.xbmp"               );
    m_Bmp[ HUD_BMP_HEALTHKIT       ].Load( "data/hud/"DATA_PATH"/hud_icon_healthkit.xbmp"          );
    m_Bmp[ HUD_BMP_PACK_AMMO       ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_ammo.xbmp"          );
    m_Bmp[ HUD_BMP_PACK_ENERGY     ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_energy.xbmp"        );
    m_Bmp[ HUD_BMP_PACK_INVENTORY  ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_inventory.xbmp"     );
    m_Bmp[ HUD_BMP_PACK_REPAIR     ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_repair.xbmp"        );
    m_Bmp[ HUD_BMP_PACK_SATCHEL    ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_satchel.xbmp"       );
    m_Bmp[ HUD_BMP_PACK_SENSOR     ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_sensor.xbmp"        );
    m_Bmp[ HUD_BMP_PACK_SHIELD     ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_shield.xbmp"        );
    m_Bmp[ HUD_BMP_PACK_TURRET     ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_turret.xbmp"        );
    m_Bmp[ HUD_BMP_PACK_BARREL     ].Load( "data/hud/"DATA_PATH"/hud_icon_pack_barrel.xbmp"        );

    m_Bmp[ HUD_BMP_VOTE_PIPE       ].Load( "data/hud/"DATA_PATH"/hud_vote_pipe.xbmp"               );
    m_Bmp[ HUD_BMP_VOTE_SLAT_TOP   ].Load( "data/hud/"DATA_PATH"/hud_vote_slat_top.xbmp"           );
    m_Bmp[ HUD_BMP_VOTE_SLAT_BOT   ].Load( "data/hud/"DATA_PATH"/hud_vote_slat_bot.xbmp"           );
    m_Bmp[ HUD_BMP_VOTE_TRACK      ].Load( "data/hud/"DATA_PATH"/hud_vote_track.xbmp"              );
    m_Bmp[ HUD_BMP_CHAT_GLASS      ].Load( "data/hud/"DATA_PATH"/hud_chat_glass.xbmp"              );

    m_Bmp[ HUD_BMP_VEH_HEALTH1     ].Load( "data/hud/"DATA_PATH"/hud_veh_health_meter01.xbmp"      );
    m_Bmp[ HUD_BMP_VEH_HEALTH2     ].Load( "data/hud/"DATA_PATH"/hud_veh_health_meter02.xbmp"      );
    m_Bmp[ HUD_BMP_VEH_STATBAR     ].Load( "data/hud/"DATA_PATH"/hud_veh_statbar.xbmp"             );
    
    m_Bmp[ HUD_BMP_VI_ASSAULT      ].Load( "data/hud/"DATA_PATH"/hud_vi_assault.xbmp"              );
    m_Bmp[ HUD_BMP_VI_BOMBER       ].Load( "data/hud/"DATA_PATH"/hud_vi_bomber.xbmp"               );
    m_Bmp[ HUD_BMP_VI_HAPC         ].Load( "data/hud/"DATA_PATH"/hud_vi_hapc.xbmp"                 );
    m_Bmp[ HUD_BMP_VI_MPB          ].Load( "data/hud/"DATA_PATH"/hud_vi_mpb.xbmp"                  );
    m_Bmp[ HUD_BMP_VI_SHRIKE       ].Load( "data/hud/"DATA_PATH"/hud_vi_shrike.xbmp"               );
    m_Bmp[ HUD_BMP_VI_WILDCAT      ].Load( "data/hud/"DATA_PATH"/hud_vi_wildcat.xbmp"              );
    m_Bmp[ HUD_BMP_V_SEAT          ].Load( "data/hud/"DATA_PATH"/hud_v_seat.xbmp"                  );
    m_Bmp[ HUD_BMP_FLAG            ].Load( "data/hud/"DATA_PATH"/hud_icon_flag.xbmp"               );

    // Load Elements
    m_ScoreElement = m_pUIManager->LoadElement( "hud_score", "data/hud/"DATA_PATH"/hud_score.xbmp", 1, 3, 3 );

    // Load Reticle graphics
    m_Reticle[HUD_WEAPON_NONE            ].Load( "data/hud/"DATA_PATH"/hud_ret_base.xbmp"          );
    m_Reticle[HUD_WEAPON_DISK_LAUNCHER   ].Load( "data/hud/"DATA_PATH"/hud_ret_disc.xbmp"          );
    m_Reticle[HUD_WEAPON_PLASMA_GUN      ].Load( "data/hud/"DATA_PATH"/hud_ret_plasma.xbmp"        );
    m_Reticle[HUD_WEAPON_SNIPER_RIFLE    ].Load( "data/hud/"DATA_PATH"/hud_ret_sniper.xbmp"        );
    m_Reticle[HUD_WEAPON_MORTAR_GUN      ].Load( "data/hud/"DATA_PATH"/hud_ret_mortar.xbmp"        );
    m_Reticle[HUD_WEAPON_GRENADE_LAUNCHER].Load( "data/hud/"DATA_PATH"/hud_ret_grenade.xbmp"       );
    m_Reticle[HUD_WEAPON_CHAIN_GUN       ].Load( "data/hud/"DATA_PATH"/hud_ret_chaingun.xbmp"      );
    m_Reticle[HUD_WEAPON_BLASTER         ].Load( "data/hud/"DATA_PATH"/hud_ret_blaster.xbmp"       );
    m_Reticle[HUD_WEAPON_ELF_GUN         ].Load( "data/hud/"DATA_PATH"/hud_ret_elf.xbmp"           );
    m_Reticle[HUD_WEAPON_MISSILE_LAUNCHER].Load( "data/hud/"DATA_PATH"/hud_ret_missile.xbmp"       );
    m_Reticle[HUD_WEAPON_REPAIR_GUN      ].Load( "data/hud/"DATA_PATH"/hud_ret_blaster.xbmp"       );
    m_Reticle[HUD_WEAPON_SHOCKLANCE      ].Load( "data/hud/"DATA_PATH"/hud_ret_blaster.xbmp"       );
    m_Reticle[HUD_WEAPON_TARGETING_LASER ].Load( "data/hud/"DATA_PATH"/hud_ret_targetlaser.xbmp"   );
    m_Reticle[HUD_WEAPON_SHRIKE          ].Load( "data/hud/"DATA_PATH"/hud_ret_shrike.xbmp"        );
    m_Reticle[HUD_WEAPON_ASSAULT_GUN     ].Load( "data/hud/"DATA_PATH"/hud_ret_assault_gun.xbmp"   );
    m_Reticle[HUD_WEAPON_ASSAULT_SHELL   ].Load( "data/hud/"DATA_PATH"/hud_ret_assault_shell.xbmp" );

#ifdef TARGET_PC
    // Load Reticle graphics
    m_HighReticle[HUD_WEAPON_NONE            ].Load( "data/hud/"DATA_PATH"/hud_ret_base.xbmp"          );
    m_HighReticle[HUD_WEAPON_DISK_LAUNCHER   ].Load( "data/hud/"DATA_PATH"/hud_ret_disc.xbmp"          );
    m_HighReticle[HUD_WEAPON_PLASMA_GUN      ].Load( "data/hud/"DATA_PATH"/hud_ret_plasma.xbmp"        );
    m_HighReticle[HUD_WEAPON_SNIPER_RIFLE    ].Load( "data/hud/"DATA_PATH"/hud_ret_sniper.xbmp"        );
    m_HighReticle[HUD_WEAPON_MORTAR_GUN      ].Load( "data/hud/"DATA_PATH"/hud_ret_mortar.xbmp"        );
    m_HighReticle[HUD_WEAPON_GRENADE_LAUNCHER].Load( "data/hud/"DATA_PATH"/hud_ret_grenade.xbmp"       );
    m_HighReticle[HUD_WEAPON_CHAIN_GUN       ].Load( "data/hud/"DATA_PATH"/hud_ret_chaingun.xbmp"      );
    m_HighReticle[HUD_WEAPON_BLASTER         ].Load( "data/hud/"DATA_PATH"/hud_ret_blaster.xbmp"       );
    m_HighReticle[HUD_WEAPON_ELF_GUN         ].Load( "data/hud/"DATA_PATH"/hud_ret_elf.xbmp"           );
    m_HighReticle[HUD_WEAPON_MISSILE_LAUNCHER].Load( "data/hud/"DATA_PATH"/hud_ret_missile.xbmp"       );
    m_HighReticle[HUD_WEAPON_REPAIR_GUN      ].Load( "data/hud/"DATA_PATH"/hud_ret_blaster.xbmp"       );
    m_HighReticle[HUD_WEAPON_SHOCKLANCE      ].Load( "data/hud/"DATA_PATH"/hud_ret_blaster.xbmp"       );
    m_HighReticle[HUD_WEAPON_TARGETING_LASER ].Load( "data/hud/"DATA_PATH"/hud_ret_targetlaser.xbmp"   );
    m_HighReticle[HUD_WEAPON_SHRIKE          ].Load( "data/hud/"DATA_PATH"/hud_ret_shrike.xbmp"        );
    m_HighReticle[HUD_WEAPON_ASSAULT_GUN     ].Load( "data/hud/"DATA_PATH"/hud_ret_assault_gun.xbmp"   );
    m_HighReticle[HUD_WEAPON_ASSAULT_SHELL   ].Load( "data/hud/"DATA_PATH"/hud_ret_assault_shell.xbmp" );

    // Register the High resulotion Reticles
    for( i=0 ; i<HUD_NUM_WEAPONS ; i++ )
        VERIFY( vram_Register( m_HighReticle[i] ) );
#endif

    // Register Bitmaps
    for( i=0 ; i<HUD_NUM_BITMAPS ; i++ )
        VERIFY( vram_Register( m_Bmp[i] ) );

    // Register Reticles
    for( i=0 ; i<HUD_NUM_WEAPONS ; i++ )
        VERIFY( vram_Register( m_Reticle[i] ) );

    m_pObjective = NULL;
}

//=========================================================================

void hud_manager::Kill( void )
{
    // Destroy Users
    while( m_Users.GetCount() > 0 )
    {
        user*   pUser = m_Users[0];

        // Destroy User
        delete pUser;
        m_Users.Delete( 0 );
    }
}

//=========================================================================

void hud_manager::Initialize( void )
{
    m_VoteState = 0;
    m_VoteFade  = 0;
    m_VoteTimer = 0;
    m_GlowItem  = HUDGLOW_NONE;
}

//=========================================================================

s32 hud_manager::CreateUser( player_object* pPlayer, s32 UIUserID )
{
    ASSERT( pPlayer );
    ASSERT( UIUserID );

    // Create new user struct
    user*   pUser = new user;
    ASSERT( pUser );
    if( pUser )
    {
        // Clear struct
        x_memset( pUser, 0, sizeof(user) );

        // Set Data
        pUser->pPlayer           = pPlayer;
        pUser->UIUserID          = UIUserID;
        pUser->ReticleVisible    = TRUE;
        pUser->VoiceMenuActive   = FALSE;

        pUser->IsInVehicle       = FALSE;
        pUser->Time              = 0.0f;
        pUser->YPos              = 0.0f;

        pUser->NumInventoryItems = 0;
        pUser->MissileLockState  = MISSILE_NO_LOCK;
        
        pUser->Popup[0].Timer    = 0.0f;
        pUser->Popup[1].Timer    = 0.0f;
        pUser->Popup[2].Timer    = 0.0f;
        
        pUser->ChatGlassY        = 0.0f;
        pUser->ChatLinesUsed     = 0;
        
        ClearChat( pUser );

        // Add an entry to the users list
        m_Users.Append() = pUser;
    }

    return (s32)pUser;
}

//=========================================================================

void hud_manager::DeleteAllUsers( void )
{
    while( m_Users.GetCount() > 0 )
    {
        DeleteUser( (s32)m_Users[0] );
    }
}

//=========================================================================

void hud_manager::DeleteUser( s32 UserID )
{
    s32     Index;

    ASSERT( (m_Users.Find( (user*)UserID )) != -1 );

    // Find the users index
    Index = m_Users.Find( (user*)UserID );

    // Delete the user
    delete m_Users[Index];
    m_Users.Delete( Index );
}

//=========================================================================

hud_manager::user* hud_manager::GetUser( s32 UserID ) const
{
    ASSERT( (m_Users.Find( (user*)UserID )) != -1 );

    user*   pUser = (user*)UserID;
    return pUser;
}

//=========================================================================

player_object* hud_manager::GetPlayer( s32 UserID ) const
{
    ASSERT( (m_Users.Find( (user*)UserID )) != -1 );

    user*   pUser = (user*)UserID;
    return pUser->pPlayer;
}

//=========================================================================

void hud_manager::Update( f32 DeltaTime )
{
    // Update Time
    m_Time += DeltaTime;

    // Loop through each user
    for( s32 i=0 ; i<m_Users.GetCount() ; i++ )
    {
        user* pUser = m_Users[i];
        ASSERT( pUser );
        
        player_object* pPlayer = pUser->pPlayer;
        ASSERT( pPlayer );

        // Missile lock
        if( pPlayer->IsMissileLockedOn() )
        {
            SetMissileLock( pPlayer->GetPlayerIndex(), MISSILE_LOCKED );
        }
        else
        {
            if( pPlayer->IsMissileTracking() )
                SetMissileLock( pPlayer->GetPlayerIndex(), MISSILE_TRACKING );
            else
                SetMissileLock( pPlayer->GetPlayerIndex(), MISSILE_NO_LOCK );
        }

        vehicle* pVehicle = pPlayer->IsAttachedToVehicle();
        
        if( pVehicle != NULL )
        {
            pUser->IsInVehicle         = TRUE;
            pUser->VehicleType         = pVehicle->GetType() - (s32)object::TYPE_VEHICLE_WILDCAT;
            pUser->VehicleHealth       = pVehicle->GetHealth();
            pUser->VehicleEnergy       = pVehicle->GetEnergy();
            pUser->VehicleWeaponEnergy = pVehicle->GetWeaponEnergy();
            pUser->Time               += DeltaTime;
            
            if( pUser->Time > VehiclePanelTime )
                pUser->Time = VehiclePanelTime;
        
            if( (pVehicle->GetType() == object::TYPE_VEHICLE_HAVOC) && pPlayer->IsVehiclePassenger() )
                pUser->YPos = (f32)VehiclePanelAddY;
            else
                pUser->YPos = 0.0f;
        }
        else
        {
            pUser->IsInVehicle  = FALSE;
            pUser->Time        -= DeltaTime;
            
            if( pUser->Time < 0.0f )
                pUser->Time = 0.0f;
        }
    
        // Slide vehicle panel
        f32 T = pUser->Time / VehiclePanelTime;
        pUser->XPos = x_sin( R_90 * T ) * VehiclePanelWidth;

        UpdateInventory( pUser, DeltaTime );
        UpdateChat     ( pUser, DeltaTime );
        UpdatePopups   ( pUser, DeltaTime );
    }
    
    UpdateVote( DeltaTime );
}

//=========================================================================

xbool hud_manager::ProcessInput( s32 UserID, s32 ControllerID, f32 DeltaTime )
{
    xbool   Processed = FALSE;
    s32     Input = 0;
    s32     Debounced;

    ASSERT( (m_Users.Find( (user*)UserID )) != -1 );

    user* pUser = (user*)UserID;

    if( pUser->VoiceMenuActive )
    {
        s32 Result     = -1;
        s32 HudVoiceID = 0 ;

        Processed = TRUE;

        // Don't process input the first time around
//        if( pUser->VoiceMenuTimer > 0.0f )
        {
            // Controller input
            Input |= input_IsPressed( INPUT_PS2_BTN_TRIANGLE, ControllerID ) ?  1 : 0;
            Input |= input_IsPressed( INPUT_PS2_BTN_CIRCLE,   ControllerID ) ?  2 : 0;
            Input |= input_IsPressed( INPUT_PS2_BTN_CROSS,    ControllerID ) ?  4 : 0;
            Input |= input_IsPressed( INPUT_PS2_BTN_SQUARE,   ControllerID ) ?  8 : 0;
            Input |= input_IsPressed( INPUT_PS2_BTN_R2,       ControllerID ) ? 16 : 0;

            // Keyboard input
            Input |= input_IsPressed( INPUT_KBD_UP,    ControllerID ) ?  1 : 0;
            Input |= input_IsPressed( INPUT_KBD_RIGHT, ControllerID ) ?  2 : 0;
            Input |= input_IsPressed( INPUT_KBD_DOWN,  ControllerID ) ?  4 : 0;
            Input |= input_IsPressed( INPUT_KBD_LEFT,  ControllerID ) ?  8 : 0;
            Input |= input_IsPressed( INPUT_KBD_BACK,  ControllerID ) ? 16 : 0;

            Debounced = Input & ~pUser->LastInput;
            pUser->LastInput = Input;

            if( Debounced &  1 )
                Result = VoiceMenu_Navigate( pUser->VoiceMenu, 0, HudVoiceID );
            else if( Debounced &  2 )
                Result = VoiceMenu_Navigate( pUser->VoiceMenu, 1, HudVoiceID );
            else if( Debounced &  4 )
                Result = VoiceMenu_Navigate( pUser->VoiceMenu, 2, HudVoiceID );
            else if( Debounced &  8 )
                Result = VoiceMenu_Navigate( pUser->VoiceMenu, 3, HudVoiceID );
            else if( Debounced & 16 )
            {
                Result = 0;
            }
        }

        pUser->VoiceMenuTimer += DeltaTime;

        // Check Result of Navigation
        if( Result != -1 )
        {
            pUser->VoiceMenuActive = FALSE;

            // Play sound?
            if( Result != 0 )
            {
                s32 SfxID = pUser->pPlayer->GetVoiceSfxBase() + (Result & 0x7fffffff) ;

                u32 TeamBits = pUser->pPlayer->GetTeamBits();
                if( Result & 0x80000000 )
                    TeamBits = 0xFFFFFFFF;

                pUser->pPlayer->PlayVoiceMenuSound( SfxID, TeamBits, HudVoiceID ) ;
            }
        }
    }

    // Return TRUE if input was processed
    return Processed;
}

//=========================================================================

void hud_manager::Render( void ) const
{
    s32     i;
    s32     nRendered = 0;

    eng_Begin( "HUD" );

    // If in screenshot mode then disable the HUD rendering
    if( HUD_HideHUD )
        goto End;

    // Loop through each user to render
    for( i=0 ; i<m_Users.GetCount() ; i++ )
    {
        user* pUser = m_Users[i];
        ASSERT( pUser );

        // Only render HUD when no UI is up & not in free cam mode
        if( ( m_pUIManager->GetNumUserDialogs( pUser->UIUserID ) == 0 ) &&
            ( !tgl.UseFreeCam[pUser->pPlayer->GetWarriorID()] ) )
        {
            // Increment number of HUDs rendered
            nRendered++;

            // Get Rectangle for the users window on screen
            const irect& wr = m_pUIManager->GetUserBounds( pUser->UIUserID );

            // Render Chat
            RenderChat( pUser, wr );
            
            // Render popups
            RenderPopups( pUser, wr );

            // Set rect for health/energy/heat gauge
            irect or( 0, 0, HealthEnergyWidth,  HealthEnergyHeight );
            or.Translate(   HealthMeterOffsetX, HealthMeterOffsetY );
            
            // Render health and energy
            if( m_Users.GetCount() == 1 )
            {
                RenderHealthEnergy( pUser, wr, or );
            }
            else
            {
                if( i == 0 )
                    RenderHealthEnergy( pUser, wr, or );
                else
                    RenderHealthEnergy( pUser, wr, or );
            }

            // Render vehicle meter
            RenderVehicle( pUser, wr );

            // HUD Training only on first training mission
            if( (GameMgr.GetGameType() == GAME_CAMPAIGN) && (GameMgr.GetCampaignMission() == (GameMgr.GetNCampaignMissions()+1) ))
                RenderHUDGlow( wr );
            
            // Voice Menu or reticle
            if( pUser->VoiceMenuActive )
            {
                // Render Voice Menu
                RenderVoiceMenu( pUser, wr );
            }
            else
            {
                // Render Reticle
                RenderReticle( pUser, wr );
            }

        }
        
        // Render Debug Info
        if( HUD_DebugInfo )
        {
            const irect& wr = m_pUIManager->GetUserBounds( pUser->UIUserID );
            irect r = wr;
            r.Deflate( 0, 40 );
            r.b = r.t + 16;
            xwstring DebugInfo;
            DebugInfo += tgl.MissionName;
            DebugInfo += " : ";
            const vector3& Pos = pUser->pPlayer->GetPosition();
            DebugInfo += xwstring( xfs( "%.2f, %.2f, %.2f", Pos.X, Pos.Y, Pos.Z ) );
            m_pUIManager->RenderText( 0, r, ui_font::v_top|ui_font::h_center, xcolor(  0,  0,  0,255), DebugInfo );
            r.Translate( -1, -1 );
            m_pUIManager->RenderText( 0, r, ui_font::v_top|ui_font::h_center, HUD_DebugColor, DebugInfo );
        }
    }
    
    // The following pieces are always displayed and are NOT hidden by the ingame menu
    for( i=0 ; i<m_Users.GetCount() ; i++ )
    {
        user* pUser = m_Users[i];
        ASSERT( pUser );

        // Get Rectangle for the users window on screen
        const irect& wr = m_pUIManager->GetUserBounds( pUser->UIUserID );

        // Render vote
        if( i == 0 )
            RenderVote( pUser, wr );
        
        // Render scoreboard/inventory panel
        RenderPanel( pUser, wr, (i == 0) );

        // Render training mission objectives
        if( GameMgr.IsCampaignMission() )
        {
            if( GameMgr.GetCampaignMission() > GameMgr.GetNCampaignMissions() )
                RenderObjectives( wr );
        }
    }

End:
    
    if( DrawGuard )
    {
        draw_Begin( DRAW_LINES, DRAW_2D | DRAW_NO_ZBUFFER );
        draw_Color( XCOLOR_WHITE );
        
        draw_Vertex( vector3(  16,  16, 0 ) );
        draw_Vertex( vector3( 496,  16, 0 ) );
        
        draw_Vertex( vector3( 496,  16, 0 ) );
        draw_Vertex( vector3( 496, 432, 0 ) );
        
        draw_Vertex( vector3( 496, 432, 0 ) );
        draw_Vertex( vector3(  16, 432, 0 ) );
        
        draw_Vertex( vector3(  16, 432, 0 ) );
        draw_Vertex( vector3(  16,  16, 0 ) );
        draw_End();
    }
    
    eng_End();
}

//=========================================================================

void hud_manager::RenderReticle( user* pUser, const irect& wr ) const
{
    player_object*  pPlayer = pUser->pPlayer;
    ASSERT( pPlayer );

    // Exit if DEAD.
    if( pPlayer->IsDead() )
        return;
    
    vehicle* pVehicle = pPlayer->IsAttachedToVehicle();

    // Exit if not armed.
    if( !pPlayer->GetArmed() && !pVehicle )
        return;

    // Get Weapon
    s32 PlayerWeapon = pPlayer->GetWeaponCurrentType();
    s32 Weapon = PlayerWeapon - player_object::INVENT_TYPE_WEAPON_START;

    // Check for player in vehicle and update weapon accordingly
    if( pVehicle )
    {
        s32 Seat = pPlayer->GetVehicleSeat();

        switch( pVehicle->GetType() )
        {
        case object::TYPE_VEHICLE_SHRIKE:
            Weapon = HUD_WEAPON_SHRIKE;
            break;

        case object::TYPE_VEHICLE_THUNDERSWORD:
            if( pPlayer->IsZoomed() )
                Weapon = HUD_WEAPON_ELF_GUN;
            else
                Weapon = HUD_WEAPON_NONE;
            break;

        case object::TYPE_VEHICLE_HAVOC:
            if( Seat == 0 )
                Weapon = HUD_WEAPON_NONE;
            break;

        case object::TYPE_VEHICLE_BEOWULF:
            if( Seat == 0 )
                Weapon = HUD_WEAPON_NONE;
            else
            {
                if( ((asstank*)pVehicle)->GetTurretBarrel() == 0 )
                    Weapon = HUD_WEAPON_ASSAULT_SHELL;
                else
                    Weapon = HUD_WEAPON_ASSAULT_GUN;
            }
            break;

        case object::TYPE_VEHICLE_WILDCAT:
            Weapon = HUD_WEAPON_NONE;
            break;

        case object::TYPE_VEHICLE_JERICHO2:
            Weapon = HUD_WEAPON_NONE;
            break;
        }
    }

    s32 XRes, YRes;
    eng_GetRes(XRes, YRes);

    // Render Reticle for Weapon
    if( (Weapon>=0) && (Weapon<HUD_NUM_WEAPONS) )
    {
        f32 bw, bh;

#ifdef TARGET_PC

        // If the resolution of the game is greater than 1024, use the high res reticule.
        if( XRes >= 1024 )
        {
            bw = (f32)m_HighReticle[Weapon].GetWidth();
            bh = (f32)m_HighReticle[Weapon].GetHeight();
        }
        else
        {
            bw = (f32)m_Reticle[Weapon].GetWidth();
            bh = (f32)m_Reticle[Weapon].GetHeight();
        }

#endif
#ifndef TARGET_PC        
        // PS2 reticule.
        bw = (f32)m_Reticle[Weapon].GetWidth();
        bh = (f32)m_Reticle[Weapon].GetHeight();
#endif
        xcolor ReticleColor = xcolor(s_ReticleColors[Weapon]);

        // Render Ammo Count
        if( (Weapon != HUD_WEAPON_NONE) && (Weapon != HUD_WEAPON_SHRIKE) && (Weapon != HUD_WEAPON_ASSAULT_GUN) && (Weapon != HUD_WEAPON_ASSAULT_SHELL) )
        {
            const player_object::loadout&       Loadout     = pPlayer->GetLoadout();
            const player_object::weapon_info&   WeaponInfo  = pPlayer->GetWeaponInfo( (player_object::invent_type)PlayerWeapon ) ;
            irect r = wr;
            r.l = (wr.l+wr.r)/2 - 24;
            r.t = (wr.t+wr.b)/2 + 24;
            r.r = r.l;
            r.b = r.t;
            if( WeaponInfo.AmmoType != player_object::INVENT_TYPE_NONE )
            {
                s32 Count = Loadout.Inventory.Counts[WeaponInfo.AmmoType];
                if( Count == 0 )
                    ReticleColor = HUDCOL_TEXT_RED;
                xwstring Wide( xfs( "%d", Count ) );
                tgl.pUIManager->RenderText( 0, r, ui_font::h_right|ui_font::v_top, xcolor(ReticleColor), (const xwchar*)Wide );
            }
        }

        // Render reticle outline
        if( (Weapon != HUD_WEAPON_SNIPER_RIFLE   ) &&
            (Weapon != HUD_WEAPON_ASSAULT_GUN    ) &&
            (Weapon != HUD_WEAPON_ASSAULT_SHELL  ) &&
            (Weapon != HUD_WEAPON_TARGETING_LASER) )
        {
            draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D|DRAW_NO_ZBUFFER );
#ifdef TARGET_PC
        
            // Draw the correct reticule
            if( XRes >= 1024 )
            {
                draw_SetTexture( m_HighReticle[HUD_WEAPON_NONE] );
            }
            else
            {
                draw_SetTexture( m_Reticle[HUD_WEAPON_NONE] );
            }                
#endif
#ifndef TARGET_PC
            // PS2 reticule.
            draw_SetTexture( m_Reticle[HUD_WEAPON_NONE] );
#endif
            draw_DisableBilinear();
            draw_Sprite( vector3((f32)wr.l+(wr.GetWidth()-64)/2.0f,(f32)wr.t+(wr.GetHeight()-64)/2.0f, 0.0f), vector2(64.0f,64.0f), ReticleColor );
            draw_End( );
        }

        // Render reticle detail
        if( Weapon != HUD_WEAPON_NONE )
        {
            draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D|DRAW_NO_ZBUFFER );

#ifdef TARGET_PC
            // Draw the correct reticule
            if( XRes >= 1024 )
            {
                draw_SetTexture( m_HighReticle[Weapon] );
            }
            else
            {
                draw_SetTexture( m_Reticle[Weapon] );
            }        
#endif
#ifndef TARGET_PC
            // PS2 reticule.
            draw_SetTexture( m_Reticle[Weapon] );
#endif
            draw_DisableBilinear();
            draw_Sprite( vector3((f32)wr.l+(wr.GetWidth()-bw)/2.0f,(f32)wr.t+(wr.GetHeight()-bh)/2.0f, 0.0f), vector2((f32)bw,(f32)bh), ReticleColor );
            draw_End( );
        }
    }

/*
    // Render Flag Icon if carrying flag in CTF game
    if( pPlayer->HasFlag() && (GameMgr.GetGameType() == GAME_CTF) )
    {
        //irect r = wr;
        vector3 Pos;
        Pos.X = (f32)((wr.l+wr.r)/2 - 26 - 14);
        Pos.Y = (f32)((wr.t+wr.b)/2 - 26 -  8);
        Pos.Z = 0.0f;

        f32 t = x_fmod( m_Time * 2.0f, 1.0f );
        u8  A = (u8)((x_sin( 2.0f * PI * t ) + 1.0f) / 2.0f * 192 + 32);

        draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D|DRAW_NO_ZBUFFER );
        draw_SetTexture( m_Bmp[HUD_BMP_FLAG] );
        draw_DisableBilinear();
        xcolor c = xcolor(128,224,255,A);
        c.A = A;
        draw_Sprite( Pos, vector2(16.0f,16.0f), c );
        draw_End( );
    }
*/

    // Render Target Info
    {
        // Get Player View Vector
        view*   pView   = pPlayer->GetView();
        vector3 ViewZ   = pView  ->GetViewZ();
        vector3 ViewPos = pView  ->GetPosition();

#define SEL_DISTANCE    600.0f
#define SEL_DISTANCE2   (SEL_DISTANCE/2.0f)

        object*     pObject;
        vector3     ObjPos;
        vector3     ViewToObj;
        f32         DistSq;
        object*     pClosestObj   = NULL;
        object*     pClosestWPObj = NULL;
        vector3     ClosestPos;
        f32         Closest     = F32_MAX;
        f32         ClosestWP   = F32_MAX;
        f32         tanAngle    = x_tan( pView->GetYFOV() / 15.0f );

        // find the labeled objects in my LOS
        ObjMgr.Select( object::ATTR_LABELED, ViewPos + ViewZ*(SEL_DISTANCE2), SEL_DISTANCE2 );

        // Loop through all objects collected
        while( (pObject = ObjMgr.GetNextSelected()) )
        {
            // Don't check yourself or vehicle
            if( (pObject != pPlayer) && (pObject != pVehicle) )
            {
                ObjPos    = pObject->GetBBox().GetCenter();
                ViewToObj = (ObjPos - ViewPos);
                DistSq    = ViewToObj.LengthSquared();

                // Distance check & Bounding sphere check
                if( (DistSq < Closest) &&
                    (pView->SphereInConeAngle( ObjPos, pObject->GetBBox().GetRadius()/2.0f, tanAngle )) )
                {
                    Closest     = DistSq;
                    ClosestPos  = ObjPos;
                    pClosestObj = pObject;
                }
            }
        }
        ObjMgr.ClearSelection();

        // Loop through labelled waypoints.
        ObjMgr.StartTypeLoop( object::TYPE_WAYPOINT );
        while( (pObject = ObjMgr.GetNextInType()) )
        {
            if( pObject->GetAttrBits() & object::ATTR_LABELED )
            {
                ObjPos    = pObject->GetBBox().GetCenter();
                ViewToObj = (ObjPos - ViewPos);
                DistSq    = ViewToObj.LengthSquared();

                if( (DistSq < ClosestWP) &&
                    (pView->SphereInConeAngle( ObjPos, pObject->GetBBox().GetRadius()/2.0f, tanAngle )) )
                {
                    ClosestWP     = DistSq;
                    pClosestWPObj = pObject;
                }
            }
        }
        ObjMgr.EndTypeLoop();

        // Loop through labelled flags.
        ObjMgr.StartTypeLoop( object::TYPE_FLAG );
        while( (pObject = ObjMgr.GetNextInType()) )
        {
            if( pObject->GetAttrBits() & object::ATTR_LABELED )
            {
                ObjPos    = pObject->GetBBox().GetCenter();
                ViewToObj = (ObjPos - ViewPos);
                DistSq    = ViewToObj.LengthSquared();

                if( (DistSq < ClosestWP) &&
                    (pView->SphereInConeAngle( ObjPos, pObject->GetBBox().GetRadius()/2.0f, tanAngle )) )
                {
                    ClosestWP     = DistSq;
                    pClosestWPObj = pObject;
                }
            }
        }
        ObjMgr.EndTypeLoop(); 

        // Now check for obstruction
        if( pClosestObj )
        {
            object::type Type = pClosestObj->GetType();

            // Waypoints and flags are not obstructed
            if( (Type != object::TYPE_WAYPOINT) && (Type != object::TYPE_FLAG) )
            {
                // Check for obstruction with a line of sight test.
                collider Ray;
                Ray.RaySetup( NULL, ViewPos, ClosestPos );
                ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
                if( Ray.HasCollided() )
                {
                    // Obstructed so use waypoint of flag
                    pClosestObj = pClosestWPObj;
                    Closest = ClosestWP;
                }
            }
        }
        else
        {
            // No object found so use waypoint or flag
            pClosestObj = pClosestWPObj;
            Closest = ClosestWP;
        }

        // If there is a closest object to the reticle then render its details
        if( pClosestObj )
        {
            irect r = wr;
            r.l = (wr.l+wr.r)/2 + 24;
            r.t = (wr.t+wr.b)/2 + 24;
            r.r = r.l + 40;
            r.b = r.t + 9;

            // Get name, health and distance
            const xwchar* pName  = pClosestObj->GetLabel();
            f32           Health = pClosestObj->GetHealth();
            f32           Dist   = Closest;

            // Get real distance
            if( Closest >= 1.0f )
                Dist = x_sqrt( Closest );

            // Render label
            tgl.pUIManager->RenderText( 0, r, ui_font::h_left | ui_font::v_top, xcolor(224,224,224,192), pName );

            // Render health bar
            xbool Friendly = (pPlayer->GetTeamBits() & pClosestObj->GetTeamBits()) != 0;
            if( 1 ) //Friendly || (pClosestObj->GetAttrBits() & object::ATTR_PLAYER) )
            {
                if( Health >= 0.0f )
                {
                    r.Translate( 0, 15 );

                    if( Health > 0.0f )
                    {
                        irect r2 = r;
                        r2.Deflate( 1, 1 );
                        r2.Translate( -1, 0 );
                        r2.r = r2.l + MAX(1, (s32)(r2.GetWidth() * Health));
                        if( Friendly )
                            tgl.pUIManager->RenderRect( r2, HUDCOL_RET_HEALTH, FALSE );
                        else
                            tgl.pUIManager->RenderRect( r2, HUDCOL_RET_HEALTH_ENEMY, FALSE );
                    }
                    tgl.pUIManager->RenderRect( r,  xcolor(224,224,224,192), TRUE );
                }
            }

            // Render Distance
            r = wr;
            r.l = (wr.l+wr.r)/2 + 24;
            r.b = (wr.t+wr.b)/2 - 24;
            r.r = r.l + 40;
            r.t = r.b - 9;

            xwstring Wide( xfs( "%dm", (s32)Dist ) );
            tgl.pUIManager->RenderText( 0, r, ui_font::h_left|ui_font::v_top, xcolor(224,224,224,192), (const xwchar*)Wide );
        }
    }
}

//=========================================================================

struct vehicle_seat
{
    s16 x;
    s16 y;
};

struct vehicle_info
{
    vehicle_seat*   pSeats;
    s32             nSeats;
    s32             Bitmap;
    xbool           HasWeapons;
};

void hud_manager::RenderVehicle( user* pUser, const irect& wr ) const
{
    static vehicle_seat Assault[2] = { {33, 9}, {25,26} };
    static vehicle_seat Bomber [3] = { {29, 6}, {29,15}, {29,45} };
    static vehicle_seat HAPC   [5] = { {12,14}, {17,20}, {24, 8}, {24,20}, {17, 8} };

    static vehicle_info Vehicles[6] = {
        { NULL,    0, HUD_BMP_VI_WILDCAT, FALSE },
        { Assault, 0, HUD_BMP_VI_ASSAULT, TRUE  },
        { NULL,    0, HUD_BMP_VI_MPB,     TRUE  },
        { NULL,    0, HUD_BMP_VI_SHRIKE,  TRUE  },      
        { Bomber,  0, HUD_BMP_VI_BOMBER,  TRUE  },
        { HAPC,    5, HUD_BMP_VI_HAPC,    FALSE },
    };

    vehicle_info& Info = Vehicles[ pUser->VehicleType ];

    //
    // Render vehicle panel
    //
    
    irect PanelRect( 0, 0, 0, 0 );
    PanelRect.Translate(  wr.l, wr.t );
    PanelRect.Translate(  HealthMeterOffsetX,  HealthMeterOffsetY  );
    PanelRect.Translate(  VehiclePanelOffsetX, VehiclePanelOffsetY );
    PanelRect.Translate( -VehiclePanelWidth  + (s32)pUser->XPos, (s32)pUser->YPos );

    s32 GlassX, GlassY;

    if( Info.HasWeapons )
    {
        RenderBitmap( HUD_BMP_VEH_HEALTH2, PanelRect.l, PanelRect.t );
        GlassX = VehicleGlass2OffsetX;
        GlassY = VehicleGlass2OffsetY;
    }
    else
    {
        RenderBitmap( HUD_BMP_VEH_HEALTH1, PanelRect.l, PanelRect.t );
        GlassX = VehicleGlass1OffsetX;
        GlassY = VehicleGlass1OffsetY;
    }

    irect GlassRect( PanelRect );
    GlassRect.Translate( GlassX, GlassY );
    RenderBitmap( HUD_BMP_VEH_STATBAR, GlassRect.l, GlassRect.t, HUD_StatBarCol );
    
    irect IconRect( GlassRect );
    IconRect.Translate( VehicleIconOffsetX, VehicleIconOffsetY );

    if(( Info.Bitmap != HUD_BMP_VI_ASSAULT ) &&
       ( Info.Bitmap != HUD_BMP_VI_MPB     ))
    {
        RenderBitmap( Info.Bitmap, IconRect.l, IconRect.t );
    }
    
    //
    // Render vehicle bars
    //

    irect BarRect( 0, 0, VehicleBarWidth, VehicleBarHeight );
    BarRect.Translate( PanelRect.l, PanelRect.t );
    
    // Render vehicle health
    irect TRect( BarRect );    
    TRect.Translate( VehicleHealthOffsetX, VehicleHealthOffsetY );
    RenderHealthBar( TRect, pUser->VehicleHealth );

    // Render vehicle energy
    TRect = BarRect;
    TRect.Translate( VehicleEnergyOffsetX, VehicleEnergyOffsetY );
    RenderBar( TRect, HUDCOL_VEHICLE_ENERGY, pUser->VehicleEnergy );

    if( Info.HasWeapons )
    {
        // Render vehicle weapon energy
        TRect = BarRect;
        TRect.Translate( VehicleWeaponOffsetX, VehicleWeaponOffsetY );
        RenderBar( TRect, HUDCOL_VEHICLE_WEAPON_ENERGY, pUser->VehicleWeaponEnergy );
    }

    vehicle* pVehicle = pUser->pPlayer->IsAttachedToVehicle();
    if( pVehicle )
    {
        // Render the seat status
        for( s32 i=1; i<Info.nSeats; i++ )
        {
            if( pVehicle->GetSeatPlayer( i ) != NULL )
            {
                s32 X = IconRect.l + Info.pSeats[i].x;
                s32 Y = IconRect.t + Info.pSeats[i].y;
                
                irect Rect( X, Y, X+5, Y+5 );
            
                RenderBitmap( HUD_BMP_V_SEAT, Rect );
            }
        }
    }
}

//=========================================================================

void hud_manager::RenderHealthEnergy( user* pUser, const irect& wr, const irect& or ) const
{
    irect Rect = or;
    Rect.Translate( wr.l, wr.t );

    RenderBitmap( HUD_BMP_HEALTH_METER, Rect, XCOLOR_WHITE );

    {
        irect br;

        // Read Values from player
        f32 Health = pUser->pPlayer->GetHealth();
        f32 Energy = pUser->pPlayer->GetEnergy();
        f32 Heat   = pUser->pPlayer->GetHeat();

        // Render Health Bar
        br = Rect;
        br.Translate   ( HealthBarOffsetX, HealthBarOffsetY );
        br.SetWidth    ( HealthBarWidth  );
        br.SetHeight   ( HealthBarHeight );
        RenderHealthBar( br, Health );

        // Render Energy Bar
        br = Rect;
        br.Translate   ( EnergyBarOffsetX, EnergyBarOffsetY );
        br.SetWidth    ( EnergyBarWidth  );
        br.SetHeight   ( EnergyBarHeight );
        //RenderBar      ( br, xcolor( 20, 24,247,224), Energy );
        RenderBar      ( br, HUD_EnergyCol, Energy );

        // Calculate Alpha for Flashing Bars
        f32 t = x_fmod( m_Time, 2.0f );
        u8 A = (u8)((x_sin( 4.0f * PI * t ) + 3.0f) / 4.0f * 224);

        // Render Heat Signature Bar
        br = Rect;
        br.Translate   ( HeatBarOffsetX, HeatBarOffsetY );
        br.SetWidth    ( HeatBarWidth  );
        br.SetHeight   ( HeatBarHeight );
        
        //xcolor c = HUDCOL_PLAYER_HEAT;
        xcolor c = HUD_HeatCol;
        if( Heat < 0.75f )
            c.A = 224;
        else
            c.A = A;
        RenderBar( br, c, Heat );
    }
}

//=========================================================================

void hud_manager::AddInventory( user* pUser, s32 Icon, s32 Count, inven_type Type )
{
    inventory& Inventory = pUser->Inventory[ pUser->NumInventoryItems ];
    
    Inventory.Icon  = Icon;
    Inventory.Count = Count;
    Inventory.Type  = Type;
    
    pUser->NumInventoryItems++;
}

//=========================================================================

void hud_manager::UpdateInventory( user* pUser, f32 DeltaTime )
{
    (void)DeltaTime;
    player_object* pPlayer = pUser->pPlayer;
    const player_object::loadout&   Loadout   = pPlayer->GetLoadout();
    const player_object::inventory& Inventory = Loadout.Inventory;

    pUser->NumInventoryItems = 0;

    // Determine number of grenades held
    s32 GrenadeIcon = HUD_BMP_GRENADE;
    s32 NumGrenades = 0;
    s32 Grenades;

    if( (Grenades   = Inventory.Counts[ player_object::INVENT_TYPE_GRENADE_FLARE ]) )
    {
        NumGrenades = Grenades;
        GrenadeIcon = HUD_BMP_GRENADE_FLARE;
    }

    if( (Grenades   = Inventory.Counts[ player_object::INVENT_TYPE_GRENADE_CONCUSSION ]) )
    {
        NumGrenades = Grenades;
        GrenadeIcon = HUD_BMP_GRENADE_CONCUSS;
    }

    if( (Grenades   = Inventory.Counts[ player_object::INVENT_TYPE_GRENADE_BASIC ]) )
    {
        NumGrenades = Grenades;
        GrenadeIcon = HUD_BMP_GRENADE_FRAG;
    }

    if( (Grenades   = Inventory.Counts[ player_object::INVENT_TYPE_MINE ]) )
    {
        NumGrenades = Grenades;
        GrenadeIcon = HUD_BMP_MINE;
    }

    //
    // Grenade Icon
    //
    
    AddInventory( pUser, GrenadeIcon, NumGrenades, INVEN_COUNT_LEFT );

//  s32 NumMines = Inventory.Counts[ player_object::INVENT_TYPE_MINE ];
//  if( NumMines ) AddInventory( pUser, HUD_BMP_MINE, NumMines, INVEN_COUNT_LEFT );

    // Packs
    player_object::invent_type Type = pPlayer->GetCurrentPack();
    s32 PackIcon = 0;
    
    // Give the player an example pack in Training mission 1
    if( GameMgr.GetCampaignMission() == (GameMgr.GetNCampaignMissions() + 1) )
        Type = player_object::INVENT_TYPE_ARMOR_PACK_ENERGY;

    switch( Type )
    {
        case player_object::INVENT_TYPE_ARMOR_PACK_AMMO             : PackIcon = HUD_BMP_PACK_AMMO;      break;
        case player_object::INVENT_TYPE_ARMOR_PACK_ENERGY           : PackIcon = HUD_BMP_PACK_ENERGY;    break;
        case player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION  : PackIcon = HUD_BMP_PACK_INVENTORY; break;
        case player_object::INVENT_TYPE_ARMOR_PACK_REPAIR           : PackIcon = HUD_BMP_PACK_REPAIR;    break;
        case player_object::INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE  : PackIcon = HUD_BMP_PACK_SATCHEL;   break;
        case player_object::INVENT_TYPE_ARMOR_PACK_SHIELD           : PackIcon = HUD_BMP_PACK_SHIELD;    break;
        case player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR    :
        case player_object::INVENT_TYPE_DEPLOY_PACK_MOTION_SENSOR   : PackIcon = HUD_BMP_PACK_SENSOR;    break;
        case player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_AA       : 
        case player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_ELF      : 
        case player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE  : 
        case player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR   : 
        case player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA   : PackIcon = HUD_BMP_PACK_BARREL;    break;
        case player_object::INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR   : PackIcon = HUD_BMP_PACK_TURRET;    break;
        case player_object::INVENT_TYPE_NONE                        : PackIcon = 0;                      break;
        
        default : ASSERT( FALSE ); // Unknown pack type!
    }

    //
    // Flags Icon
    //
    
    if( (GameMgr.GetGameType() == GAME_HUNTER) && (m_Users.GetCount() == 2) )
    {
        // No room on scoreboard to display flags in 2 player Hunter - so we use the flag icon instead
        const game_score& Score = GameMgr.GetScore();
        s32 NumFlags = Score.Player[ pPlayer->GetPlayerIndex() ].Deaths;
        
        if( NumFlags < 100 )
            AddInventory( pUser, HUD_BMP_FLAG, NumFlags, INVEN_COUNT_LEFT );
        else
            AddInventory( pUser, HUD_BMP_FLAG, NumFlags, INVEN_COUNT_LEFT );
    }

    //
    // Check for Satchel Charge deployed in world
    //

    satchel_charge* pSatchel;
    
    ObjMgr.StartTypeLoop( object::TYPE_SATCHEL_CHARGE );
    while( (pSatchel = (satchel_charge*)ObjMgr.GetNextInType()) )
    {
        if( pSatchel->GetOriginID() == pPlayer->GetObjectID() )
            break;
    }
    ObjMgr.EndTypeLoop();
    
    if( pSatchel )
    {
        PackIcon = HUD_BMP_PACK_SATCHEL;
    }
    
    //
    // Pack Icon
    //
    
    if( PackIcon )
    {
        inven_type Type  = INVEN_NORMAL;
        s32        Count = 0;
        
        switch( PackIcon )
        {
            case HUD_BMP_PACK_SHIELD :
                if( pPlayer->IsShielded() )
                    Type = INVEN_PULSE2;
                break;
           
            case HUD_BMP_PACK_SATCHEL :
                if( pSatchel )
                {
                    if( pSatchel->IsArmed() )
                        Type = INVEN_PULSE2;
                }
                break;
            
            case HUD_BMP_PACK_SENSOR :
                Type  = INVEN_COUNT_LEFT;
                Count = Inventory.Counts[ player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR ];
                break;
        }
    
        AddInventory( pUser, PackIcon, Count, Type );
    }

    //
    // Flag Icon
    //
    
    if( GameMgr.GetGameType() == GAME_CTF )
    {
        if( pPlayer->HasFlag() )
        {
            AddInventory( pUser, HUD_BMP_FLAG, 0, INVEN_PULSE1 );
        }
    }
}

//=========================================================================

void hud_manager::RenderInventory( const user* pUser, s32 XPos, s32 YPos ) const
{
    s32 X     = XPos;
    s32 Y     = YPos;
    s32 IconX = InventoryIconShiftX + X;
    s32 IconY = InventoryIconShiftY;
    s32 Flags = (ui_font::v_top | ui_font::h_center);
    irect R;
    
    for( s32 i=0; i<pUser->NumInventoryItems; i++ )
    {
        const inventory& Item = pUser->Inventory[i];

        xcolor IconColor( HUD_IconGlowCol );
        xcolor TextColor( HUD_IconGlowCol );

        xbool  HasCount = FALSE;
        f32    T;

        switch( Item.Type )
        {
            case INVEN_NORMAL      :
                break;
            
            case INVEN_COUNT_LEFT  :
                HasCount = TRUE;
                R.Set( X, Y, X, Y );
                if( Item.Count == 0 )
                    TextColor = HUD_ZeroGlowCol;
                break;
            
            case INVEN_COUNT_BELOW :
                HasCount = TRUE;
                R.Set( IconX, Y - IconY, IconX, Y - IconY );
                R.Translate( FlagIconTextX, FlagIconTextY );
                break;
            
            case INVEN_PULSE1      :
                T = x_fmod( m_Time, 1.0f );
                IconColor.A = (u8)( ((x_sin( 2.0f * PI * T ) / 4.0f) * 255) + 192 );
                break;
            
            case INVEN_PULSE2      :
                T = 0.5f + (x_sin( 2.0f * PI * x_fmod( m_Time, 1.0f ) ) * 0.5f);
                IconColor.R = HUD_IconGlowCol.R + (u8)( (HUD_ShieldedCol.R - HUD_IconGlowCol.R) * T );
                IconColor.G = HUD_IconGlowCol.G + (u8)( (HUD_ShieldedCol.G - HUD_IconGlowCol.G) * T );
                IconColor.B = HUD_IconGlowCol.B + (u8)( (HUD_ShieldedCol.B - HUD_IconGlowCol.B) * T );
                break;
            
            default                : ASSERT( FALSE );
        }

        RenderBitmap( Item.Icon, IconX, Y - IconY, IconColor );
        
        if( HasCount == TRUE )
        {
            xwstring Wide( xfs( "%d", Item.Count ) );
            RenderTextDropShadow( R, Flags, XCOLOR_BLACK, (const xwchar*)Wide );
            RenderText          ( R, Flags, TextColor,    (const xwchar*)Wide );
        }
        
        Y += InventoryIconAddY;
    }
}

//=========================================================================

void hud_manager::RenderGlow( const irect& Rect, lock_state MissileLockState ) const
{
    xcolor Color;
    xbool  Pulse = FALSE;

    switch( MissileLockState )
    {
        case MISSILE_NO_LOCK  : Color = HUD_NormalGlowCol; Pulse = FALSE; break;
        case MISSILE_TRACKING : Color = HUD_TargetGlowCol; Pulse = TRUE;  break;
        case MISSILE_LOCKED   : Color = HUD_LockedGlowCol; Pulse = TRUE;  break;
        default               : ASSERT( FALSE );
    }
    
    if( Pulse )
    {
        // Pulse the alpha
        f32 t   = x_fmod( m_Time, 2.0f );
        Color.A = (u8)( (x_sin( 4.0f * PI * t ) + 3.0f) / 4.0f * 255.0f ); 
    }
    
    RenderBitmap( HUD_BMP_LED_GLOW, Rect, Color );
}

//=========================================================================

void hud_manager::RenderPanel( user* pUser, const irect& wr, xbool IsPlayer1 ) const
{
    const xwchar* Label[2];
    s32           Value[2];
    s32           MyTeam     = 0;
    xbool         IsCampaign = FALSE;
    game_type     GameType   = GAME_CTF;
    static s32    LastTeam;

    xbool ShowPanel = FALSE;

    // When player is using ingame menu we should hide the panel
    // Note: the timer piece is always displayed
    if( ( m_pUIManager->GetNumUserDialogs( pUser->UIUserID ) == 0 ) &&
        ( !tgl.UseFreeCam[pUser->pPlayer->GetWarriorID()] ) )
        ShowPanel = TRUE;

    // Get Score Info from Game Manager
    const game_score& Score = GameMgr.GetScore();

    // Get Player Index
    s32 PlayerIndex = pUser->pPlayer->GetPlayerIndex();
    if( (PlayerIndex >= 0) && (PlayerIndex < 32) )
    {
        MyTeam = Score.Player[PlayerIndex].Team;

        // Set MyTeam=0 if not team based
        if( !Score.IsTeamBased )
            MyTeam = 0;

        ASSERT( (MyTeam == 0) || (MyTeam == 1) );

        // Hack: so we can display a normal looking scoreboard in Training Mission 1

        GameType = Score.GameType;
        
        if( GameType == GAME_CAMPAIGN )
        {
            if( GameMgr.GetCampaignMission() == (GameMgr.GetNCampaignMissions()+1) )
                GameType   = GAME_CTF;
            else
                IsCampaign = TRUE;
        }
        
        switch( GameType )
        {
        case GAME_CAMPAIGN:
            break;
        
        case GAME_DM:
            Label[0] = StringMgr( "ui", IDS_SCORE );
            Label[1] = StringMgr( "ui", IDS_KILLS );
            Value[0] = Score.Player[PlayerIndex].Score;
            Value[1] = Score.Player[PlayerIndex].Kills;
            break;
        
        case GAME_HUNTER:
            Label[0] = StringMgr( "ui", IDS_SCORE );
            Label[1] = StringMgr( "ui", IDS_FLAGS );
            Value[0] = Score.Player[PlayerIndex].Score;
            Value[1] = Score.Player[PlayerIndex].Deaths;
            break;
        
        case GAME_CTF:
        case GAME_CNH:
        case GAME_TDM:
        default:
            if( IsPlayer1 == TRUE )
            {
                Label[0] = Score.Team[ MyTeam     ].Name;
                Value[0] = Score.Team[ MyTeam     ].Score;
                Label[1] = Score.Team[ MyTeam ^ 1 ].Name;
                Value[1] = Score.Team[ MyTeam ^ 1 ].Score;
                LastTeam = MyTeam;
            }
            else
            {
                Label[0] = Score.Team[ LastTeam ^ 1 ].Name;
                Value[0] = Score.Team[ LastTeam ^ 1 ].Score;
            }
            break;
        }
    }

    if( m_Users.GetCount() == 1 )
    {
        //
        // 1 Player Game
        //

        if( IsPlayer1 == TRUE )
        {
            if( ShowPanel == TRUE )
            {
                // Position of panel on right hand edge of screen
                irect PanelRect( 0, 0, ScoreBoardWidth, ScoreBoardHeight );
                PanelRect.Translate( wr.r - ScoreBoardWidth, wr.t + ScoreBoardOffsetY );
                
                if( IsCampaign == TRUE )
                {
                    // Campaign Missions
                
                    RenderStatBar( pUser, PanelRect.l + StatBarOffsetCX, PanelRect.t + StatBarOffsetCY, FALSE );
                    
                    RenderBitmap( HUD_BMP_1P_SCOREBOARD_C, PanelRect.l, PanelRect.t );
                
                    // Timer Value
                    irect TempRect( 0, 0, ScoreBoardTextWidth, ScoreBoardTextHeight );
                    TempRect.Translate( PanelRect.l, PanelRect.t );
                    TempRect.Translate( TimerValueOffsetCX, TimerValueOffsetCY );
                    RenderClock( TempRect );
                }
                else
                {
                    // Single Player Game
                
                    RenderStatBar( pUser, PanelRect.l + StatBarOffsetX, PanelRect.t + StatBarOffsetY, FALSE );
                
                    RenderBitmap( HUD_BMP_1P_SCOREBOARD, PanelRect.l, PanelRect.t );
                    
                    // Position text box within panel
                    irect TRect( 0, 0, ScoreBoardTextWidth, ScoreBoardTextHeight );
                    TRect.Translate( PanelRect.l, PanelRect.t );
                    TRect.Translate( ScoreLabelOffsetX, 0 );
                
                    s32 CFlags = ui_font::v_top | ui_font::h_center;
                    s32 RFlags = ui_font::v_top | ui_font::h_right;
                    
                    // Score Label
                    irect TempRect = TRect;
                    TempRect.Translate( 0, ScoreLabelOffsetY );
                    RenderText( TempRect, CFlags, HUD_ScoreLabelCol, Label[0] );
                    
                    // Score Value
                    TempRect = TRect;
                    TempRect.Translate( -8, ScoreValueOffsetY );
                    m_pUIManager->RenderText( 0, TempRect, RFlags, HUD_ScoreValueCol, xfs( "%d", Value[0] ));
                    
                    // Flags Label
                    TempRect = TRect;
                    TempRect.Translate( 0, FlagsLabelOffsetY );
                    RenderText( TempRect, CFlags, HUD_ScoreLabelCol, Label[1] );
                    
                    // Flags Value
                    TempRect = TRect;
                    TempRect.Translate( -8, FlagsValueOffsetY );
                    m_pUIManager->RenderText( 0, TempRect, RFlags, HUD_ScoreValueCol, xfs( "%d", Value[1] ));
                    
                    // Timer Value
                    TempRect = TRect;
                    TempRect.Translate( TimerValueOffsetX, TimerValueOffsetY );
                    RenderClock( TempRect );
                }
            }
        }
    }
    else
    {
        //
        // 2 Player Game
        //

        s32 CFlags = ui_font::v_top | ui_font::h_center;
        s32 RFlags = ui_font::v_top | ui_font::h_right;

        if( IsPlayer1 == TRUE )
        {
            s32 X = wr.r -  ScoreBoard02Width;
            s32 Y = wr.b - (ScoreBoard02Height / 2);
            
            irect TRect( 0, 0, ScoreBoardTextWidth, ScoreBoardTextHeight );
            TRect.Translate( X, Y );
        
            // Timer Value
            irect TempRect = TRect;
            TempRect.Translate( TimerValue1OffsetX, TimerValue1OffsetY );
            RenderBitmap( HUD_BMP_2P_SCOREBOARD02, X, Y );
            RenderClock ( TempRect );
            Y -= ScoreBoard00Height;

            if( ShowPanel == TRUE )
            {
                // Upper Piece
                X = wr.r - ScoreBoard00Width;
                TRect.Set( 0, 0, ScoreBoardTextWidth, ScoreBoardTextHeight );
                TRect.Translate( X, Y );
                RenderBitmap( HUD_BMP_2P_SCOREBOARD00, X, Y );
                
                // Label
                TempRect = TRect;
                TempRect.Translate( ScoreLabel1OffsetX, ScoreLabel1OffsetY );
                RenderText( TempRect, CFlags, HUD_ScoreLabelCol, Label[0] );
                
                // Value
                TempRect = TRect;
                TempRect.Translate( ScoreValue1OffsetX, ScoreValue1OffsetY );
                m_pUIManager->RenderText( 0, TempRect, RFlags, HUD_ScoreValueCol, xfs( "%d", Value[0] ));
                
                X = wr.r - ScoreBoard00Width;
                RenderStatBar( pUser, X + StatBar1OffsetX, Y + StatBar1OffsetY, TRUE );
            }
        }
        else
        {
            if( ShowPanel == TRUE )
            {
                // Lower Piece
                s32 X = wr.r -  ScoreBoard00Width;
                s32 Y = wr.t + (ScoreBoard02Height / 2);
                irect TRect( 0, 0, ScoreBoardTextWidth, ScoreBoardTextHeight );
                TRect.Translate( X, Y );
                RenderBitmap( HUD_BMP_2P_SCOREBOARD04, X, Y );
                
                // Label
                irect TempRect = TRect;
                TempRect.Translate( FlagsLabel2OffsetX, FlagsLabel2OffsetY );
                RenderText( TempRect, CFlags, HUD_ScoreLabelCol, Label[0] );
                
                // Value
                TempRect = TRect;
                TempRect.Translate( FlagsValue2OffsetX, FlagsValue2OffsetY );
                m_pUIManager->RenderText( 0, TempRect, RFlags, HUD_ScoreValueCol, xfs( "%d", Value[0] ));
                
                RenderStatBar( pUser, X + StatBar2OffsetX, Y + StatBar2OffsetY, FALSE );
            }
        }
    }
}

//=========================================================================

void hud_manager::RenderStatBar( user* pUser, s32 X, s32 Y, xbool IsFlipped ) const
{
    irect GlowRect;
    irect StatRect;

    s32 InvenX = X + InventoryOffsetX;
    s32 InvenY = Y;

    if( m_Users.GetCount() == 1 )
    {
        InvenY += InventoryOffsetY;
    
        s32 NumIcons = MIN( StatBarMaxIcons, pUser->NumInventoryItems + 2 );
        s32 Height   = (StatBarRealHeight / StatBarMaxIcons) * (StatBarMaxIcons - NumIcons);
    
        GlowRect.Set( 0, 0, LedGlowWidth, -LedGlowHeight );
        GlowRect.Translate( X, Y + LedGlowRealHeight );
    
        StatRect.Set( 0, 0, StatBarWidth,  StatBarHeight );
        StatRect.Translate( X, Y );
        StatRect.Translate( 0, -Height );
        
        RenderBitmap( HUD_BMP_1P_STATBAR, StatRect, HUD_StatBarCol );
    }
    else
    {
        if( IsFlipped == TRUE )
        {
            GlowRect.Set( 0, 0, LedGlowWidth, LedGlowHeight );
            GlowRect.Translate( X, Y - LedGlowRealHeight );
            
            StatRect.Set( 0, 0, StatBarWidth, -256 );
            InvenY += Inventory1OffsetY;
        }
        else
        {
            GlowRect.Set( 0, 0, LedGlowWidth, -LedGlowHeight );
            GlowRect.Translate( X, Y + LedGlowRealHeight );
            
            StatRect.Set( 0, 0, StatBarWidth,  256 );
            InvenY += Inventory2OffsetY;
        }

        StatRect.Translate( X, Y );
        RenderBitmap( HUD_BMP_2P_STATBAR, StatRect, HUD_StatBarCol );
    }

    RenderGlow( GlowRect, pUser->MissileLockState );
    RenderInventory( pUser, InvenX, InvenY );
}

//=========================================================================

void hud_manager::RenderScoreBox( const irect& r, xbool Separator ) const
{
    xcolor          Colors[2];

    // Setup Colors
    Colors[0] = xcolor(23,44,42,OVERLAY_ALPHA/2);
    Colors[1] = xcolor(30,59,56,OVERLAY_ALPHA/2);

    // Render Score Frame
    m_pUIManager->RenderElement( m_ScoreElement, r, 0, xcolor(255,255,255,OVERLAY_ALPHA) );

    // Render Lines on top of Frame
    draw_Begin( DRAW_LINES, DRAW_2D|DRAW_USE_ALPHA|DRAW_NO_ZBUFFER );
    {
        for( s32 c=0 ; c<2 ; c++ )
        {
            draw_Color( Colors[c] );

            draw_Vertex( vector3( (f32)(r.l+2), (f32)(r.t+18-c), 0.0f ) );
            draw_Vertex( vector3( (f32)(r.r-2), (f32)(r.t+18-c), 0.0f ) );

            if( Separator )
            {
                draw_Vertex( vector3( (f32)(r.l+69-c), (f32)(r.t+2), 0.0f ) );
                draw_Vertex( vector3( (f32)(r.l+69-c), (f32)(r.b-2), 0.0f ) );
            }
        }
    }
    draw_End();
}

//=========================================================================

void hud_manager::RenderClock( const irect& r ) const
{
    s32     TimeRemaining;
    char    Time[32];

    // campaign missions 6-13 dont need the clock
    if( GameMgr.GetTimeLimit() < 0 ) return;

    // Get time remaining in match
    TimeRemaining = (s32)GameMgr.GetTimeRemaining();
    if( TimeRemaining >= 0 )
    {
        ASSERT( (TimeRemaining >= 0) && (TimeRemaining < 60000) );
        x_sprintf( Time, "%2d:%02d", (TimeRemaining / 60), (TimeRemaining % 60) );

        m_pUIManager->RenderText( 0, r, (ui_font::v_top | ui_font::h_center), HUD_TimerValueCol, Time );
    }
}

//=========================================================================

void hud_manager::RenderHealthBar( irect r, f32 Value ) const
{
    // Calculate Alpha for Flashing Bars
    f32 t = x_fmod( m_Time, 2.0f );
    u8 A = (u8)((x_sin( 4.0f * PI * t ) + 3.0f) / 4.0f * 224);

    // Render Health Bar
    xcolor c;
    if( Value > 0.60f )
        c = HUD_HealthCol;
        //c = HUDCOL_HEALTH_HI;
    else if( Value > 0.25f )
        c = HUDCOL_HEALTH_MED;
    else
    {
        c = HUDCOL_HEALTH_LOW;
        c.A = A;
    }
    
    // Always show some health on the screen.
    if( Value < 0.04f && Value != 0.0f)
        Value = 0.04f;

    RenderBar( r, c, Value );
}

//=========================================================================

void hud_manager::RenderBar( irect r, const xcolor& c1, f32 Value ) const
{
    // Draw Black Background
    draw_Rect( r, xcolor(0,0,0,224), FALSE );

    // Draw Bar
    if( Value > 0.0f )
    {
        r.r = r.l + (s32)(r.GetWidth()*Value);
        draw_Rect( r, c1, FALSE );
    }
    else
    {
        r.l = r.r + (s32)(r.GetWidth()*Value);
        draw_Rect( r, c1, FALSE );
    }
}

//=========================================================================

void hud_manager::RenderVoiceMenuText( voice_table& VoiceMenu, s32 Index, irect r ) const
{
    xcolor  Color[2];
    s32     ColorIndex  = 0;
    s32     Count       = 0;
    s32     i;

    // Setup Colors
    Color[0] = HUDCOL_VOICEMENU_TERMINAL;
    Color[1] = HUDCOL_VOICEMENU_LINK;

    // Count Active Entries
    for( i=0 ; i<3 ; i++ )
    {
        if( VoiceMenu.Entries[Index+i].Pos != 0 )
            Count++;
    }

    // Only render if Count > 0
    if( Count > 0 )
    {
        // Scale Box according to number of entries
        r.Deflate( 0, (18*(3-Count))/2 );

        // Render Box
        m_pUIManager->RenderElement( m_ScoreElement, r, 0 );

        r.Deflate( 0, 4 );
        r.SetHeight( 18 );

        // Render Seperators
        draw_Begin( DRAW_LINES, DRAW_USE_ALPHA|DRAW_2D|DRAW_NO_ZBUFFER );
        draw_Color( xcolor(30,59,56,192) );
        for( i=0 ; i<(Count-1) ; i++ )
        {
            draw_Vertex( vector3((f32)(r.l+2),(f32)r.t+18+i*18,0.0f) );
            draw_Vertex( vector3((f32)(r.r-2),(f32)r.t+18+i*18,0.0f) );
        }
        draw_End();

        // Render Text for all entries
        i=Index;
        while( i<(Index+3) )
        {
            // Move to next entry with something in it
            while( (i<(Index+3)) && (VoiceMenu.Entries[i].Pos == 0) ) i++;

            if( i<(Index+3) )
            {
                // Get Link or Terminal Color
                if( VoiceMenu.Entries[i].ID == -1 )
                    ColorIndex = 0;
                else
                    ColorIndex = 1;

                // Render String
                irect r2 = r;
                r2.Translate( 0, -2 );
                m_pUIManager->RenderText( 0, r2, ui_font::h_center|ui_font::v_center, Color[ColorIndex], xwstring(VoiceMenu.Entries[i].pLabel) );

                // Next String
                i++;
                r.Translate( 0, 18 );
            }
        }
    }
}

void hud_manager::RenderVoiceMenu( user* pUser, const irect& wr ) const
{
    // Check if the menu is active
//    if( pUser->VoiceMenuActive )
    {
        s32     cx = (wr.l+wr.r)/2;
        s32     cy = (wr.t+wr.b)/2;
        s32     ox = 48;
        s32     oy = 32;
        s32     w  = 96;
        s32     h  = 18*3+8;

        irect   r;

        r.Set( cx-w/2, cy-h-oy, cx-w/2+w, cy-oy );
        RenderVoiceMenuText( pUser->VoiceMenu,  0, r );
        
        r.Set( cx+ox, cy-h/2, cx+w+ox, cy-h/2+h );
        RenderVoiceMenuText( pUser->VoiceMenu,  3, r );

        r.Set( cx-w/2, cy+oy, cx-w/2+w, cy+h+oy );
        RenderVoiceMenuText( pUser->VoiceMenu,  6, r );
        
        r.Set( cx-w-ox, cy-h/2, cx-ox, cy-h/2+h );
        RenderVoiceMenuText( pUser->VoiceMenu,  9, r );
    }
}

//=========================================================================

void hud_manager::RenderBitmap( s32 BitmapID, const irect& Rect, xcolor Color ) const
{
    const xbitmap& Bitmap = m_Bmp[ BitmapID ];
    vector2 WH( (f32)Rect.GetWidth(), (f32)Rect.GetHeight() );
    
    draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA | DRAW_TEXTURED | DRAW_2D | DRAW_NO_ZBUFFER );
    draw_SetTexture( Bitmap );
    draw_DisableBilinear();
    draw_Sprite( vector3( (f32)Rect.l, (f32)Rect.t, 0.0f ), WH, Color );
    draw_End();
}

//=========================================================================

void hud_manager::RenderBitmap( s32 BitmapID, s32 X, s32 Y, xcolor Color ) const
{
    const xbitmap& Bitmap = m_Bmp[ BitmapID ];
    vector2 WH( (f32)Bitmap.GetWidth(), (f32)Bitmap.GetHeight() );
    
    draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA | DRAW_TEXTURED | DRAW_2D | DRAW_NO_ZBUFFER );
    draw_SetTexture( Bitmap );
    draw_DisableBilinear();
    draw_Sprite( vector3( (f32)X, (f32)Y, 0.0f ), WH, Color );
    draw_End();
}

//=========================================================================

void hud_manager::RenderTextDropShadow( const irect& aRect, s32 Flags, xcolor Color, const xwchar* pString ) const
{
    irect Rect = aRect;

    Rect.Translate( 1, 1 );
    m_pUIManager->RenderText( 0, Rect, Flags, Color, pString );
}

//=========================================================================

void hud_manager::RenderTextOutline( const irect& aRect, s32 Flags, xcolor Color, const xwchar* pString ) const
{
    irect Rect = aRect;

    Rect.Translate( -1, -1 );
    m_pUIManager->RenderText( 0, Rect, Flags, Color, pString );

    Rect.Translate(  2,  0 );
    m_pUIManager->RenderText( 0, Rect, Flags, Color, pString );

    Rect.Translate(  0,  2 );
    m_pUIManager->RenderText( 0, Rect, Flags, Color, pString );

    Rect.Translate( -2,  0 );
    m_pUIManager->RenderText( 0, Rect, Flags, Color, pString );
}

//=========================================================================

void hud_manager::RenderText( const irect& Rect, s32 Flags, xcolor Color, const xwchar* pString ) const
{
    m_pUIManager->RenderText( 0, Rect, Flags, Color, pString );
}

//=========================================================================

void hud_manager::RenderText( const irect& Rect, s32 Flags, s32 Alpha, const xwchar* pString ) const
{
    m_pUIManager->RenderText( 0, Rect, Flags, Alpha, pString );
}

//=========================================================================

void hud_manager::ActivateVoiceMenu( s32 UserID )
{
    ASSERT( (m_Users.Find( (user*)UserID )) != -1 );

    user* pUser = (user*)UserID;

    if( !pUser->VoiceMenuActive )
    {
        VoiceMenu_Init( pUser->VoiceMenu );
        pUser->VoiceMenuActive  = TRUE;
        pUser->VoiceMenuTimer   = 0.0f;
        pUser->LastInput        = -1;
    }
}

//=========================================================================

void hud_manager::DeactivateVoiceMenu( s32 UserID )
{
    ASSERT( (m_Users.Find( (user*)UserID )) != -1 );

    user* pUser = (user*)UserID;

    pUser->VoiceMenuActive = FALSE;
}

//=========================================================================

xbool hud_manager::IsVoiceMenuActive( s32 UserID ) const
{
    ASSERT( (m_Users.Find( (user*)UserID )) != -1 );

    user* pUser = (user*)UserID;

    return pUser->VoiceMenuActive;
}

//=========================================================================

void hud_manager::RenderObjectives( const irect& wr ) const
{
    const xwchar* pString = NULL;
    irect r = wr;
    
    if( m_pObjective )
    {
        // Build line rectangle
        r.Deflate( 16, 16 );
        r.t = r.b - 14 - 14 - 14;
        r.b = r.t + 14;
        pString = m_pObjective;
    }
    
    // This is now rendered from the ingame menu
    /*
    if( m_pCampaignObjectives )
    {
        user* pUser = m_Users[0];
        player_object* pPlayer = pUser->pPlayer;
        if( pPlayer )
        {
            if( tgl.pUIManager->GetNumUserDialogs( pPlayer->GetUserID() ) > 0 )
            {
                r.Deflate( 16, 16 );
                r.SetHeight( r.GetHeight() / 2 );
                r.Translate( 16, r.GetHeight() + 32 );
    
                pString = m_pCampaignObjectives;
            }
        }
    }
    */

    if( pString )
    {
        // Render Objectives
        RenderTextOutline( r, ui_font::v_top | ui_font::h_left, XCOLOR_BLACK,  pString );
        RenderText       ( r, ui_font::v_top | ui_font::h_left, FECOL_NEUTRAL, pString );
    }
}

//=========================================================================

void hud_manager::SetObjective( const xwchar* pString )
{
    m_pObjective = pString;
}

//=========================================================================

const xwchar* hud_manager::GetObjective( void )
{
    return( m_pObjective );
}

//=========================================================================

void hud_manager::RenderVote( user* pUser, const irect& wr ) const
{
    (void)pUser;
    s32 YPos;

    if( m_Users.GetCount() == 1 )
        YPos = wr.b + VotePipeOffsetY;
    else
        YPos = wr.b - (VotePipeHeight / 2) + 2;

    //
    // Render Vote Pipe
    //

    irect LRect( 0, 0,  VotePipeWidth, VotePipeHeight );
    irect RRect( 0, 0, -VotePipeWidth, VotePipeHeight ); 

    f32 K = m_XAdd / (VotePipeWidth - VotePipeLStop);
    s32 LX, RX;

    if( m_Users.GetCount() == 1 )
    {
        LX = wr.l - (s32)m_XAdd;
        RX = wr.r + (s32)m_XAdd;
    }
    else
    {
        s32 XAdd = (s32)( (VotePipeWidth - VotePipeRStop) * K );
        LX = wr.l - (s32)m_XAdd;
        RX = wr.r + (s32)  XAdd;
    }

    LRect.Translate( LX, YPos );
    RRect.Translate( RX, YPos ); 

    //
    // Render Voting Components
    //
    
    s32  S = GetCurrentState();
    
    if(( S == VOTE_OPENING_VOTEBOX ) ||
       ( S == VOTE_CLOSING_VOTEBOX ) ||
       ( S == VOTE_OPENING_SLATS   ) ||
       ( S == VOTE_CLOSING_SLATS   ) ||
       ( S == VOTE_VOTING          ))
    {
        // Render top slat
        irect Rect( LRect.r, LRect.t, RRect.r, LRect.t + VoteSlatRealHeight );
        irect TopRect( Rect );
        TopRect.Translate( 0, (s32)m_YAdd );
        RenderBitmap( HUD_BMP_VOTE_SLAT_TOP, TopRect, XCOLOR_WHITE );

        // Render bottom slot
        irect BotRect( LRect.r, LRect.t, RRect.r, LRect.t + VoteSlatRealHeight );
        BotRect.Translate( 0, -(s32)m_YAdd   );
        BotRect.Translate( 0, VoteSlatHeight );
        RenderBitmap( HUD_BMP_VOTE_SLAT_BOT, BotRect, XCOLOR_WHITE );

        // Render left side piece
        irect LeftRect( TopRect.l, TopRect.t, TopRect.l + VoteTrackRealWidth, BotRect.t );
        RenderBitmap( HUD_BMP_VOTE_TRACK, LeftRect, XCOLOR_WHITE );

        // Render right side piece
        irect RightRect( TopRect.r, TopRect.t, TopRect.r - VoteTrackRealWidth, BotRect.t );
        RenderBitmap( HUD_BMP_VOTE_TRACK, RightRect, XCOLOR_WHITE );
        
        // Render glass
        irect GlassRect( TopRect.l + VoteTrackWidth, TopRect.t + VoteSlatHeight, TopRect.r - VoteTrackWidth, BotRect.t );
        draw_Rect( GlassRect, HUD_VoteBoxCol, FALSE );

        u8     A = (u8)( m_VoteFade * 255.0f );
        xcolor B( 0, 0, 0, A );
        xcolor C;

        // HACK!
        // TODO: this check can be removed when player initiating vote casts his vote automatically
        if( (m_Votes1 >= 0.0f) && (m_Votes2 >= 0.0f) )
        {
        // Compute width of vote bars
        f32 Width = GlassRect.GetWidth() / 1.0f;    // divide by 2.0 for old vote method
        s32 W1    = (s32)(Width * m_Votes1);
        s32 W2    = (s32)(Width * m_Votes2);

        // Render positive votes
        irect VoteRect1( GlassRect.l, GlassRect.t, GlassRect.l + W1, GlassRect.b );
        C   = HUD_VotePosCol;
        C.A = A;
        draw_Rect( VoteRect1, C, FALSE );
        
        // Render negative votes
        irect VoteRect2( GlassRect.r, GlassRect.t, GlassRect.r - W2, GlassRect.b );
        C   = HUD_VoteNegCol;
        C.A = A;
        draw_Rect( VoteRect2, C, FALSE );
        
        // Render votes needed indictator
        irect VoteRect3( GlassRect.l - VotesNeededBarWidth, GlassRect.t, GlassRect.l + VotesNeededBarWidth, GlassRect.b );
        f32 Range = GlassRect.GetWidth() - (VotesNeededBarWidth * 2.0f);
        VoteRect3.Translate( VotesNeededBarWidth,   0 );
        VoteRect3.Translate( (s32)(Range * m_VotesNeeded), 0 );
        C   = XCOLOR_YELLOW;
        C.A = (A>>1);
        draw_Rect( VoteRect3, C, FALSE );

        xwstring TextFor    ( xfs( "%d", m_VotesFor     ) );
        xwstring TextAgainst( xfs( "%d", m_VotesAgainst ) );
        
        // Render vote counts For
        irect VoteRect4( GlassRect.l, GlassRect.t, GlassRect.l, GlassRect.b );
        VoteRect4.Translate( VoteCountOffsetX, 0 );
        C = HUD_VoteTextCol;
        C.A = A;
        RenderTextOutline( VoteRect4, ui_font::v_top | ui_font::h_center, B, (const xwchar*)TextFor );
        RenderText       ( VoteRect4, ui_font::v_top | ui_font::h_center, C, (const xwchar*)TextFor );
        
        // Render vote counts Against
        irect VoteRect5( GlassRect.r, GlassRect.t, GlassRect.r, GlassRect.b );
        VoteRect5.Translate( -VoteCountOffsetX, 0 );
        C = HUD_VoteTextCol;
        C.A = A;
        RenderTextOutline( VoteRect5, ui_font::v_top | ui_font::h_center, B, (const xwchar*)TextAgainst );
        RenderText       ( VoteRect5, ui_font::v_top | ui_font::h_center, C, (const xwchar*)TextAgainst );
        }
        
        // Render vote text
        C = HUD_VoteTextCol;
        C.A = A;
        RenderTextOutline( GlassRect, ui_font::v_top | ui_font::h_center, B, m_pVoteString );
        RenderText       ( GlassRect, ui_font::v_top | ui_font::h_center, C, m_pVoteString );
    }

    RenderBitmap( HUD_BMP_VOTE_PIPE, LRect );
    RenderBitmap( HUD_BMP_VOTE_PIPE, RRect );
}

//=========================================================================

void hud_manager::UpdateVote( f32 DeltaTime )
{
    s32           i;
    s32           For;
    s32           Against;
    s32           Missing;
    s32           Needed;
    const xwchar* pMessage;

    xbool         Voting = FALSE;

    for( i = 0; i < m_Users.GetCount(); i++ )
    {
        if( (GameMgr.IsPlayerInVote( m_Users[i]->pPlayer->GetObjectID().Slot )) &&
            (GameMgr.GetVoteData( pMessage, For, Against, Missing, Needed )) )
        {
            Voting = TRUE;
            StartVote( pMessage );
            //s32 Total = For + Against + Missing;
            //s32 Kill  = Total - Needed;
            //f32 V1 = (f32)For / (f32)Needed;
            //f32 V2;
            //if( Kill == 0 )
            //    V2 = (Against) ? 1.0f : 0.0f;
            //else
            //    V2 = (f32)Against / (f32)Kill;

            // NOTE: Until the player who initiates a vote has his vote casted automatically,
            // set the parametric vote values to -1 so the renderer can identify no votes being set.
            s32 Total = For + Against;

            f32 V1 = -1.0f;
            f32 V2 = -1.0f;

            if( Total > 0 )
            {
                V1 = (f32)For     / Total;
                V2 = (f32)Against / Total; 
            }

            SetVote( V1, V2 );
            
            f32 N = (f32)Needed / 100.0f;
            SetVotesNeeded( N );
            
            // Set the integer vote counts
            SetForAgainst( For, Against );
        }
    }

    if( !Voting )
        StopVote();

    switch( GetCurrentState() )
    {
        case VOTE_OPEN_FULL     :
            
            m_XAdd   = (f32)VotePipeWidth;
            m_YAdd   = 0.0f;
            m_XSpeed = 0.0f;
            break;

        case VOTE_OPENING_VOTEBOX :
            
            m_XAdd   += m_XSpeed      * DeltaTime;
            m_XSpeed += VotePipeAccel * DeltaTime;
            
            if( m_XAdd > (VotePipeWidth - VotePipeLStop) )
            {
                m_XAdd   = (f32)(VotePipeWidth - VotePipeLStop);
                m_XSpeed = 0;
                NextState();
            }
            break;
        
        case VOTE_OPENING_SLATS :
        
            m_YAdd -= ((VoteBoxHeight / 2.0f) / VoteSlatTime) * DeltaTime;
            
            if( m_YAdd < -VoteBoxHeight / 2.0f )
            {
                m_YAdd = -VoteBoxHeight / 2.0f;
                
                m_VoteFade += VoteFadeSpeed * DeltaTime;
                if( m_VoteFade > 1.0f )
                {
                    m_VoteFade = 1.0f;
                    NextState();
                }
            }
            break;
        
        case VOTE_OPENING       :
            
            m_XAdd   += m_XSpeed      * DeltaTime;
            m_XSpeed += VotePipeAccel * DeltaTime;

            if( m_XAdd > VotePipeWidth )
            {
                m_XAdd   = (f32)VotePipeWidth;
                m_XSpeed = 0;
                NextState();
            }
            break;
        
        case VOTE_CLOSING       :
                
            m_XAdd   -= m_XSpeed      * DeltaTime;
            m_XSpeed += VotePipeAccel * DeltaTime;

            if( m_XAdd < 0 )
            {
                m_XAdd   = 0;
                m_XSpeed = 0;
                NextState();
            }
            break;
        
        case VOTE_CLOSING_VOTEBOX :

            m_XAdd   -= m_XSpeed      * DeltaTime;
            m_XSpeed += VotePipeAccel * DeltaTime;
            
            if( m_XAdd < 0 )
            {
                m_XAdd   = 0;
                m_XSpeed = 0;
                NextState();
            }
            break;

        case VOTE_CLOSING_SLATS :

            m_VoteFade -= VoteFadeSpeed * DeltaTime;
            if( m_VoteFade < 0 )
            {
                m_VoteFade = 0;
                
                m_YAdd += ((VoteBoxHeight / 2.0f) / VoteSlatTime) * DeltaTime;
            
                if( m_YAdd >= 0.0f )
                {
                    m_YAdd = 0.0f;
                    NextState();
                }
            }
            break;
            
        case VOTE_CLOSED        :
            
            m_XAdd   = 0.0f;
            m_YAdd   = 0.0f;
            m_XSpeed = 0.0f;
            break;

        case VOTE_PAUSE         :
        
            m_VoteTimer += DeltaTime;
            
            if( m_VoteTimer > VotePauseTime )
            {
                m_VoteTimer = 0;
                NextState();
            }
            
        case VOTE_VOTING        :
            break;

        case VOTE_DONE          :
            break;
            
        default                 : return;
    }

    // TODO: REMOVE!! -------------------------------------------
    if( TestVoting )
    {
        if( input_IsPressed( INPUT_PS2_BTN_CIRCLE ) ||
            input_IsPressed( INPUT_KBD_I ))
        {
            StartVote( (const xwchar*)xwstring( "Vote to change level to DeathBirdsFly" ) );
            StopVote();
        }
    
        if( input_IsPressed( INPUT_PS2_BTN_CROSS ))
        {
            m_Votes1 += DeltaTime * 0.5f;
            m_Votes2 += DeltaTime * 0.5f;
        
            if( m_Votes1 > 1.0f ) m_Votes1 = 0;
            if( m_Votes2 > 1.0f ) m_Votes2 = 0;
        }
    }
    
    if( TestChat )
    {
        if( input_WasPressed( INPUT_PS2_BTN_CIRCLE ) ||
            input_WasPressed( INPUT_KBD_I ))
        {
            static s32 ChatNum;
            char Text[128];
            x_sprintf( Text, "This an example Chat String %d\n", ChatNum );
            AddChat( 0, (const xwchar*)xwstring( Text ) );
            ChatNum++;
        }
    }
    // TODO: REMOVE!! -------------------------------------------
}

//=========================================================================

void hud_manager::NextState( void )
{
    m_VoteState++;
    
    if( GetCurrentState() == VOTE_DONE )
    {
        m_VoteState = 0;
        m_VoteTimer = 0;
    }
}

//=========================================================================

s32 hud_manager::GetCurrentState( void ) const
{
    s32 N = ( m_Users.GetCount() == 1 ) ? 0 : 1;
    return( s_VoteStates[N][ m_VoteState ] );
}

//=========================================================================

void hud_manager::StartVote( const xwchar* pString )
{
    s32  S = GetCurrentState();
    
    if(( S == VOTE_OPEN_FULL ) ||
       ( S == VOTE_CLOSED    ))
    {
        NextState();
        m_pVoteString = pString;
        SetVote( 0.0f, 0.0f );
        SetVotesNeeded( 0.0f );
        SetForAgainst( 0, 0 );
    }
}

//=========================================================================

void hud_manager::StopVote( void )
{
    s32 S = GetCurrentState();
    
    if( S == VOTE_VOTING )
    {
        NextState();
    }
}

//=========================================================================

void hud_manager::SetVote( f32 Vote1, f32 Vote2 )
{
    m_Votes1 = Vote1;
    m_Votes2 = Vote2;
}

//=========================================================================

void hud_manager::SetVotesNeeded( f32 Needed )
{
    m_VotesNeeded = Needed;
}

//=========================================================================

void hud_manager::SetForAgainst( s32 For, s32 Against )
{
    m_VotesFor     = For;
    m_VotesAgainst = Against;
}

//=========================================================================

xbool hud_manager::IsReadyToVote( void )
{
    return( GetCurrentState() == VOTE_VOTING );
}

//=========================================================================

void hud_manager::RenderHUDGlow( const irect& wr ) const
{
    xcolor Color( HUD_PulseCol );

    f32 t   = x_fmod( m_Time, 2.0f );
    Color.A = (u8)( (x_sin( 4.0f * PI * t ) + 3.0f) / 4.0f * 255.0f ); 

    if( m_GlowItem == HUDGLOW_HEALTHMETER )
    {
        irect Rect( wr );
        Rect.Translate( HealthMeterOffsetX, HealthMeterOffsetY );
        Rect.Translate( HealthMeterGlowX,   HealthMeterGlowY   );
        RenderBitmap( HUD_BMP_HEALTH_METER_G, Rect.l, Rect.t, Color );
    }
    
    if( m_GlowItem == HUDGLOW_SCOREBOARD )
    {
        irect Rect( wr.r - ScoreBoardWidth, wr.t + ScoreBoardOffsetY, 0, 0 );
        Rect.Translate( ScoreBoardGlowX, ScoreBoardGlowY );
        RenderBitmap( HUD_BMP_1P_SCOREBOARD_G, Rect.l, Rect.t, Color );
    }

    if( m_GlowItem == HUDGLOW_STATBAR )
    {
        irect Rect( wr.r - ScoreBoardWidth, wr.t + ScoreBoardOffsetY, 0, 0 );
        Rect.Translate( StatBarOffsetX, StatBarOffsetY );
        Rect.Translate( StatBarGlowX,   StatBarGlowY   );
        RenderBitmap( HUD_BMP_1P_STATBAR_G, Rect.l, Rect.t, Color );
    }
    
    if( m_GlowItem == HUDGLOW_RETICLE )
    {
        s32 X = (s32)wr.GetCenter().X;
        s32 Y = (s32)wr.GetCenter().Y;
        
        RenderBitmap( HUD_BMP_RETICLE_G, X + ReticleGlowX, Y + ReticleGlowY, Color );
    }
}

//=========================================================================

void hud_manager::SetHUDGlow( hud_glow Item )
{
    m_GlowItem = Item;

    if( m_GlowItem == HUDGLOW_TEXT1 )
    {
        AddChat(  0, StringMgr( "Campaign", TEXT_T1_HUD_TEXT1 ) );
        Popup( 0, 0, StringMgr( "Campaign", TEXT_T1_HUD_TEXT2 ) );
        Popup( 1, 0, StringMgr( "Campaign", TEXT_T1_HUD_TEXT3 ) );
    }
}

//=========================================================================

void hud_manager::SetMissileLock( s32 PlayerIndex, lock_state LockState )
{
    user* pUser = GetUserFromPlayerID( PlayerIndex );

    if( pUser )    
        pUser->MissileLockState = LockState;
}

//=========================================================================

hud_manager::user* hud_manager::GetUserFromPlayerID( s32 PlayerIndex ) const
{
    object::id PlayerID( PlayerIndex, -1);

    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
    user*          pUser   = NULL;

    if( pPlayer )
    for( s32 i=0; i<m_Users.GetCount(); i++ )
    {
        if( m_Users[i]->pPlayer == pPlayer )
            pUser = m_Users[i];
    }

    return( pUser );
}

//=========================================================================

