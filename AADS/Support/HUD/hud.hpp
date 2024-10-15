//==============================================================================
//  
//  hud.hpp
//  
//==============================================================================

#ifndef HUD_HPP
#define HUD_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_TYPES_HPP
#include "x_types.hpp"
#endif

#include "hud_font.hpp"

//==============================================================================
//  HUD
//==============================================================================

#define HUD_CHAT_LINES      4
#define HUD_CHAT_LINE_LEN   256
#define HUD_POPUP_LEN       128

class player_object;

class hud
{
public:
    enum Weapons
    {
        none,
        disc,
        plasma,
        sniper,
        mortar,
        grenade,
        chaingun,
        blaster,
        elf,
        missile,
        targetlaser,
        num_weapons
    };

    enum menu_mode
    {
        menu_null,
        menu_voice,
        menu_inventory
    };

    enum voice_command_flags
    {
        HUD_VC_GLOBAL   = 0x80000000
    };

    struct voice_command
    {
        const char* pLabel;
        const char* pText;
        s32         ID;
    };

    struct voice_table
    {
        const char*     pTitle;
        voice_command   Commands[12];

    };

protected:
    // General
    xbool           m_Initialized;                          // HUD is initialized
    s32             m_Width;                                // Width of window
    s32             m_Height;                               // Height of window
    s32             m_XOffset;
    s32             m_YOffset;

    // Font
    hud_font        m_Font;                                 // Font for HUD

    // Textures
    xbitmap         m_Window;                               // Window bitmap, has 9 textures packed into it
    xbitmap         m_Weapons[num_weapons];                 // Weapons textures, TODO: Pack into single texture
    xbitmap         m_ReticleBase;                          // Reticle base texture
    xbitmap         m_Reticles[num_weapons];                // Reticle textures, TODO: Pack into single texture
    xbitmap         m_Compass;                              // Compass texture
    xbitmap         m_NESW;                                 // NESW texture for compass
    xbitmap         m_VoiceChatController;                  // Voice Chat controller

    // Weapon & Player Info
    player_object*  m_pPlayer;                              // Pointer to player
    s32             m_Ammo[num_weapons];                    // Ammo counts

    // Chat data
    xcolor          m_ChatColor[HUD_CHAT_LINES];            // Colors for chat lines
    char*           m_pChatLine[HUD_CHAT_LINES];            // Chat line text pointers
    char*           m_pChatMemory;                          // Pointer to chat memory
    s32             m_iChatLine;                            // Index of last chat line displayed

    // Menu System
    menu_mode       m_MenuMode;                             // Menu Mode
    xbool           m_MenuLX;                               // Analog debounce
    xbool           m_MenuLY;                               // Analog debounce
    xbool           m_MenuRX;                               // Analog debounce
    xbool           m_MenuRY;                               // Analog debounce

    // Voice Commands
    voice_table*    m_pVoiceTable;                          // Pointer to current voice table
    voice_command*  m_pVoiceCommand;                        // Last selected voice command

    // Inventory Screens
    s32             m_InventorySelection;                   // Inventory option selected

    // Popup Messages
    f32             m_PopupTimer;                           // Timer for popup display
    char            m_PopupMessage[HUD_POPUP_LEN];          // Popup message pointer
    xcolor          m_PopupColor;                           // Color of popup message
    f32             m_PopupAlpha;                           // Alpha for popup message

protected:
    void    RenderWindow            ( const irect& R, f32 Alpha=1.0f );
    void    RenderCompass           ( s32 x, s32 y );
    void    RenderMeter             ( s32 x, s32 y );
    void    RenderChat              ( s32 x, s32 y );
    void    RenderWeapons           ( s32 x, s32 y );
    void    RenderInventory         ( s32 x, s32 y );
                                    
    void    Render4Way              ( s32 x, s32 y, s32 w, s32 h, voice_command* pCommands );
    void    RenderVoiceMenu         ( s32 x, s32 y, voice_table* pVoiceTable );
                                    
    void    RenderPopup             ( void );
                                    
    s32     VoiceCommandSelect      ( s32 Selection );
                                    
public:                             
            hud                     ( void );
           ~hud                     ( void );
                                    
    void    Init                    ( void );
    void    Kill                    ( void );
                                    
    void    Render                  ( void );
    void    ProcessTime             ( f32 Delta );
                                    
    // Chat Window                  
    void    AddChatLine             ( xcolor Color, const char* pString );
                                    
    // Menu System                  
    void    MenuInputInit           ( f32 LX, f32 LY, f32 RX, f32 RY );
    void    MenuInput               ( f32 LX, f32 LY, f32 RX, f32 RY,
                                      xbool DU, xbool DD, xbool DL, xbool DR,
                                      xbool B1, xbool B2, xbool B3, xbool B4 );
    xbool   MenuIsActive            ( void );
    xbool   VoiceMenuIsActive       ( void );
    void    ToggleVoiceMenu         ( void );
    void    ToggleInventoryMenu     ( void );
    s32     GetVoiceCommand         ( void );
    s32     GetInventorySelection   ( void );

    // Popup Messages
    void    DisplayMessage          ( const char* pString, xcolor TextColor, f32 Time, f32 Alpha=1.0f );
};

//==============================================================================
#endif // HUD_HPP
//==============================================================================
