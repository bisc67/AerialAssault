//=========================================================================
//
//  dlg_main_menu.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_globals.hpp"

#include "AudioMgr\audio.hpp"
#include "LabelSets\Tribes2Types.hpp"

#include "ui\ui_font.hpp"
#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"

#include "dlg_MainMenu.hpp"

#include "titles.hpp"

#include "serverman.hpp"

#include "data\ui\ui_strings.h"

#include "Demo1/SpecialVersion.hpp"

//=========================================================================
//  Switches
//=========================================================================

extern server_manager*  pSvrMgr;
xbool  CampaignFinished = FALSE;

//=========================================================================
//  Main Menu Dialog
//=========================================================================

enum controls
{
    IDC_ONLINE_FRAME,
    IDC_OFFLINE_FRAME,
    IDC_OPT_FRAME,
    IDC_ONLINE_P1,
    IDC_ONLINE_P2,
    IDC_OFFLINE_P1,
    IDC_OFFLINE_P2,
    IDC_CAMPAIGN,
    IDC_TXT_ONLINE,
    IDC_TXT_OFFLINE,
    IDC_TXT_OPTIONS,
    IDC_AUDIO,
    IDC_NETWORK,
    IDC_CREDITS,
    IDC_SOUND_TEST,
    IDC_EXIT_GAME
};


ui_manager::control_tem MainMenuControls[] =
{
    // Frames.
    { IDC_ONLINE_FRAME,     IDS_NULL,                  "frame1",   120,  63, 240, 70, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_OFFLINE_FRAME,    IDS_NULL,                  "frame1",   120, 154, 240, 98, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_OPT_FRAME,        IDS_NULL,                  "frame1",   120, 273, 240, 98, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    // Text.
    { IDC_TXT_ONLINE,       IDS_ONLINE,                "text",     71,  51, 340, 24, 0, 0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_TXT_OFFLINE,      IDS_OFFLINE,               "text",     71, 142, 340, 24, 0, 0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_TXT_OPTIONS,      IDS_OPTIONS,               "text",     71, 261, 340, 24, 0, 0, 0, 0, ui_win::WF_VISIBLE },

    // Buttons.
#if defined( E3_SERVER )
    { IDC_ONLINE_P1,        IDS_ONE_PLAYER,            "button",  128,  73, 224, 24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_ONLINE_P2,        IDS_TWO_PLAYER,            "button",  128, 101, 224, 24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OFFLINE_P1,       IDS_ONE_PLAYER_OFFLINE,    "button",  128, 164, 224, 24, 0, 2, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_OFFLINE_P2,       IDS_TWO_PLAYER_OFFLINE,    "button",  128, 192, 224, 24, 0, 3, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_CAMPAIGN,         IDS_CAMPAIGN,              "button",  128, 220, 224, 24, 0, 4, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },    
    { IDC_AUDIO,            IDS_AUDIO,                 "button",  128, 283, 224, 24, 0, 5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_NETWORK,          IDS_NETWORK,               "button",  128, 311, 224, 24, 0, 6, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CREDITS,          IDS_CREDITS,               "button",  128, 339, 224, 24, 0, 7, 1, 1, ui_win::WF_VISIBLE },

#elif defined( E3_CLIENT )
    { IDC_ONLINE_P1,        IDS_ONE_PLAYER,            "button",  128,  73, 224, 24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_ONLINE_P2,        IDS_TWO_PLAYER,            "button",  128, 101, 224, 24, 0, 1, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_OFFLINE_P1,       IDS_ONE_PLAYER_OFFLINE,    "button",  128, 164, 224, 24, 0, 2, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_OFFLINE_P2,       IDS_TWO_PLAYER_OFFLINE,    "button",  128, 192, 224, 24, 0, 3, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_CAMPAIGN,         IDS_CAMPAIGN,              "button",  128, 220, 224, 24, 0, 4, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },    
    { IDC_AUDIO,            IDS_AUDIO,                 "button",  128, 283, 224, 24, 0, 5, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_NETWORK,          IDS_NETWORK,               "button",  128, 311, 224, 24, 0, 6, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_CREDITS,          IDS_CREDITS,               "button",  128, 339, 224, 24, 0, 7, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },

#elif defined( WIDE_BETA )
    { IDC_ONLINE_P1,        IDS_ONE_PLAYER,            "button",  128,  73, 224, 24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_ONLINE_P2,        IDS_TWO_PLAYER,            "button",  128, 101, 224, 24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OFFLINE_P1,       IDS_ONE_PLAYER_OFFLINE,    "button",  128, 164, 224, 24, 0, 2, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_OFFLINE_P2,       IDS_TWO_PLAYER_OFFLINE,    "button",  128, 192, 224, 24, 0, 3, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },
    { IDC_CAMPAIGN,         IDS_CAMPAIGN,              "button",  128, 220, 224, 24, 0, 4, 1, 1, ui_win::WF_VISIBLE | ui_win::WF_DISABLED },    
    { IDC_AUDIO,            IDS_AUDIO,                 "button",  128, 283, 224, 24, 0, 5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_NETWORK,          IDS_NETWORK,               "button",  128, 311, 224, 24, 0, 6, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CREDITS,          IDS_CREDITS,               "button",  128, 339, 224, 24, 0, 7, 1, 1, ui_win::WF_VISIBLE },

#else
    { IDC_ONLINE_P1,        IDS_ONE_PLAYER,            "button",  128,  73, 224, 24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_ONLINE_P2,        IDS_TWO_PLAYER,            "button",  128, 101, 224, 24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OFFLINE_P1,       IDS_ONE_PLAYER_OFFLINE,    "button",  128, 164, 224, 24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OFFLINE_P2,       IDS_TWO_PLAYER_OFFLINE,    "button",  128, 192, 224, 24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CAMPAIGN,         IDS_CAMPAIGN,              "button",  128, 220, 224, 24, 0, 4, 1, 1, ui_win::WF_VISIBLE },    
    { IDC_AUDIO,            IDS_AUDIO,                 "button",  128, 283, 224, 24, 0, 5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_NETWORK,          IDS_NETWORK,               "button",  128, 311, 224, 24, 0, 6, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CREDITS,          IDS_CREDITS,               "button",  128, 339, 224, 24, 0, 7, 1, 1, ui_win::WF_VISIBLE },
#endif

#if defined(TARGET_PC) || defined(DEMO_DISK_HARNESS)
    { IDC_EXIT_GAME,    IDS_EXIT_GAME,  "button",  128, 376, 224, 24, 0, 8, 1, 1, ui_win::WF_VISIBLE },
#endif
#ifdef INCLUDE_SOUND_TEST
    { IDC_SOUND_TEST,   IDS_SOUND_TEST, "button",  356, 376, 124, 24, 0, 8, 1, 1, ui_win::WF_VISIBLE },
#endif
};

ui_manager::dialog_tem MainMenuDialog =
{
    IDS_MAIN_MENU,
    1, 9,
    sizeof(MainMenuControls)/sizeof(ui_manager::control_tem),
    &MainMenuControls[0],
    0
};

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Registration function
//=========================================================================

void dlg_main_menu_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "main menu", &MainMenuDialog, &dlg_main_menu_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_main_menu_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_main_menu* pDialog = new dlg_main_menu;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_main_menu
//=========================================================================

dlg_main_menu::dlg_main_menu( void )
{
}

//=========================================================================

dlg_main_menu::~dlg_main_menu( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_main_menu::Create( s32                        UserID,
                             ui_manager*                pManager,
                             ui_manager::dialog_tem*    pDialogTem,
                             const irect&               Position,
                             ui_win*                    pParent,
                             s32                        Flags,
                             void*                      pUserData )
{
    xbool   Success = FALSE;

    (void)pUserData;

    ASSERT( pManager );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_pOnline         = (ui_frame*)  FindChildByID( IDC_ONLINE_FRAME  );
    m_pTXT_Online     = (ui_text*)   FindChildByID( IDC_TXT_ONLINE    );
    m_pOnlineP1       = (ui_button*) FindChildByID( IDC_ONLINE_P1     );
    m_pOnlineP2       = (ui_button*) FindChildByID( IDC_ONLINE_P2     );
    
    m_pOffline        = (ui_frame*)  FindChildByID( IDC_OFFLINE_FRAME );
    m_pTXT_Offline    = (ui_text*)   FindChildByID( IDC_TXT_OFFLINE   );
    m_pOfflineP1      = (ui_button*) FindChildByID( IDC_OFFLINE_P1    );
    m_pOfflineP2      = (ui_button*) FindChildByID( IDC_OFFLINE_P2    );
    m_pCampaign       = (ui_button*) FindChildByID( IDC_CAMPAIGN      );

    m_pOpt            = (ui_frame*)  FindChildByID( IDC_OPT_FRAME     );
    m_pTXT_Options    = (ui_text*)   FindChildByID( IDC_TXT_ONLINE    );
    m_pAudio          = (ui_button*) FindChildByID( IDC_AUDIO         );
    m_pNetwork        = (ui_button*) FindChildByID( IDC_NETWORK       );
    m_pCredits        = (ui_button*) FindChildByID( IDC_CREDITS       );

#if defined(TARGET_PC) || defined(DEMO_DISK_HARNESS)
    m_pExitGame   = (ui_button*)FindChildByID( IDC_EXIT_GAME );
#endif

#ifdef INCLUDE_SOUND_TEST
    m_pSoundTest  = (ui_button*)FindChildByID( IDC_SOUND_TEST );
#endif

    GotoControl( (ui_control*)m_pOnlineP1 );
    m_pOnline->EnableNewFrame( TRUE , 52 );
    m_pOffline->EnableNewFrame( TRUE, 60 );
    m_pOpt->EnableNewFrame( TRUE, 60 );

    m_ButtonPushed = 0;
    m_pNetOptions = NULL;

    // Change the frame type.
    m_pOnline->ChangeElement();
    m_pOffline->ChangeElement();
    m_pOpt->ChangeElement();

    // Return success code
    return Success;
}

//=========================================================================

void dlg_main_menu::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_main_menu::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );

    xbool IsNetworked = (pSvrMgr->GetLocalAddr().IP != 0);

    // Get window rectangle
    irect   r;
    r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
    irect   rb = r;
    rb.Deflate( 1, 1 );
    rb.SetHeight( 40 );
    rb.Deflate( 16, 0 );

    // Set title regarding online / offline
    if( IsNetworked ) 
    {
        m_pManager->RenderText( 2, rb, ui_font::v_center|ui_font::h_right, XCOLOR_WHITE, "ONLINE" );
    }
    else
    {
        m_pManager->RenderText( 2, rb, ui_font::v_center|ui_font::h_right, XCOLOR_WHITE, "OFFLINE" );
    }

#ifdef E3_CLIENT
    {
        const char* pString = "WORK IN PROGRESS -- E3 CLIENT DEMONSTRATION";

        r.Set( 0, m_Position.b-16-12, 512, m_Position.b-16 );    
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_BLACK, pString );
        r.Translate( -1, -1 );
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_YELLOW, pString );
    }
#endif

#ifdef E3_SERVER
    {
        const char* pString = "WORK IN PROGRESS -- E3 SERVER DEMONSTRATION";

        r.Set( 0, m_Position.b-16-12, 512, m_Position.b-16 );    
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_BLACK, pString );
        r.Translate( -1, -1 );
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_YELLOW, pString );
    }
#endif

#ifdef WIDE_BETA
    {
        const char* pString = "WORK IN PROGRESS -- ONLINE BETA TEST";

        r.Set( 0, m_Position.b-16-12, 512, m_Position.b-16 );    
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_BLACK, pString );
        r.Translate( -1, -1 );
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, XCOLOR_YELLOW, pString );
    }
#endif
}

//=========================================================================

void dlg_main_menu::OnPadSelect( ui_win* pWin )
{
    (void)pWin;

    // Single player online
    if( pWin == (ui_win*)m_pOnlineP1 )
    {
        xbool IsNetworked = (pSvrMgr->GetLocalAddr().IP != 0);
        
        fegl.GameMode         = FE_MODE_ONLINE_ONE_PLAYER;
        fegl.GameModeOnline   = TRUE;

        if( !IsNetworked )
        {
            LaunchNetworkOption();
            m_ButtonPushed = 1;
        }
        else
        {
            tgl.NRequestedPlayers = 1;
            fegl.nLocalPlayers    = 1;
            fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
            fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
            fegl.iWarriorSetup    = 0;

            audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
           
        }
        
        
/*
        tgl.NRequestedPlayers = 1;
        fegl.nLocalPlayers    = 1;
        fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
        fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
        fegl.iWarriorSetup    = 0;

        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
*/        
        if( CampaignFinished )
        {
            warrior_setup WarriorSetup;    
            WarriorSetup = fegl.WarriorSetups[0];
            fegl.WarriorSetups[0] = fegl.WarriorSetups[2];
            fegl.WarriorSetups[2] = WarriorSetup;

            CampaignFinished = FALSE;
        }
    }

    // Two player online
    if( pWin == (ui_win*)m_pOnlineP2 )
    {
        xbool IsNetworked = (pSvrMgr->GetLocalAddr().IP != 0);
        
        fegl.GameMode         = FE_MODE_ONLINE_TWO_PLAYER;
        fegl.GameModeOnline   = TRUE;

        if( !IsNetworked )
        {
            LaunchNetworkOption();
            m_ButtonPushed = 2;
        }
        else
        {
            tgl.NRequestedPlayers = 2;
            fegl.nLocalPlayers    = 2;
            fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
            fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
            fegl.iWarriorSetup    = 0;

            audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
        }
/*
        tgl.NRequestedPlayers = 2;
        fegl.nLocalPlayers    = 2;
        fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
        fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
        fegl.iWarriorSetup    = 0;


        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
*/        
        if( CampaignFinished )
        {
            warrior_setup WarriorSetup;    
            WarriorSetup = fegl.WarriorSetups[0];
            fegl.WarriorSetups[0] = fegl.WarriorSetups[2];
            fegl.WarriorSetups[2] = WarriorSetup;

            CampaignFinished = FALSE;
        }

    }


    // Single player offline
    if( pWin == (ui_win*)m_pOfflineP1 )
    {
        tgl.NRequestedPlayers = 1;
        fegl.nLocalPlayers    = 1;
        fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
        fegl.GotoGameState    = FE_STATE_GOTO_OFFLINE;
        fegl.iWarriorSetup    = 0;
        fegl.GameMode         = FE_MODE_OFFLINE_ONE_PLAYER;
        fegl.GameModeOnline   = FALSE;

        audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
        
        if( CampaignFinished )
        {
            warrior_setup WarriorSetup;    
            WarriorSetup = fegl.WarriorSetups[0];
            fegl.WarriorSetups[0] = fegl.WarriorSetups[2];
            fegl.WarriorSetups[2] = WarriorSetup;

            CampaignFinished = FALSE;
        }

        m_ButtonPushed = 0;
    }

    // Two player offline
    if( pWin == (ui_win*)m_pOfflineP2 )
    {
        tgl.NRequestedPlayers = 2;
        fegl.nLocalPlayers    = 2;
        fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
        fegl.GotoGameState    = FE_STATE_GOTO_OFFLINE;
        fegl.iWarriorSetup    = 0;
        fegl.GameMode         = FE_MODE_OFFLINE_TWO_PLAYER;
        fegl.GameModeOnline   = FALSE;

        audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
        
        if( CampaignFinished )
        {
            warrior_setup WarriorSetup;    
            WarriorSetup = fegl.WarriorSetups[0];
            fegl.WarriorSetups[0] = fegl.WarriorSetups[2];
            fegl.WarriorSetups[2] = WarriorSetup;

            CampaignFinished = FALSE;
        }
        m_ButtonPushed = 0;
    }

    // Campaign
    if( pWin == (ui_win*)m_pCampaign )
    {
        tgl.NRequestedPlayers = 1;
        fegl.nLocalPlayers    = 1;
        fegl.State            = FE_STATE_GOTO_CAMPAIGN;
        fegl.GotoGameState    = FE_STATE_GOTO_CAMPAIGN;
        fegl.iWarriorSetup    = 0;
        fegl.GameMode         = FE_MODE_CAMPAIGN;
        fegl.GameModeOnline   = FALSE;

        audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
        
        if( !CampaignFinished )
        {
            warrior_setup WarriorSetup;    
            WarriorSetup = fegl.WarriorSetups[0];
            fegl.WarriorSetups[0] = fegl.WarriorSetups[2];
            fegl.WarriorSetups[2] = WarriorSetup;

            CampaignFinished = TRUE;
        }
        m_ButtonPushed = 0;

    }
    // Options
    if ( pWin==(ui_win*)m_pAudio)
    {
        fegl.State          = FE_STATE_GOTO_AUDIO_OPTIONS;
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
        m_ButtonPushed = 0;
    }

    if ( pWin==(ui_win*)m_pNetwork)
    {
        fegl.State          = FE_STATE_GOTO_NETWORK_OPTIONS;
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
        m_ButtonPushed = 0;
    }

    if (pWin == (ui_win*)m_pCredits)
    {
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
        m_ButtonPushed = 0;
        title_Credits();
    }
/*
    // Online.
    if( pWin == (ui_win*)m_pOnline )
    {
        fegl.State            = FE_STATE_GOTO_ONLINE_MAIN_MENU;

        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }

    // Offline.
    if( pWin == (ui_win*)m_pOffline )
    {
        fegl.State            = FE_STATE_GOTO_OFFLINE_MAIN_MENU;
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }
    if ( pWin==(ui_win*)m_pOptions)
    {
        fegl.State          = FE_STATE_GOTO_OPTIONS_MAIN_MENU;
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
    }

    if (pWin == (ui_win*)m_pCredits)
    {
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN );
        title_Credits();
    }
*/
#if defined(TARGET_PC) || defined(DEMO_DISK_HARNESS)
    if ( pWin == (ui_win*)m_pExitGame )
    {
        tgl.ExitApp = TRUE;
        audio_Play(SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
    }
#endif


#ifdef INCLUDE_SOUND_TEST
    if( pWin == (ui_win*)m_pSoundTest )
    {
        fegl.State            = FE_STATE_GOTO_SOUNDTEST;
        audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
    }
#endif

}

//=========================================================================

void dlg_main_menu::LaunchNetworkOption ( void )
{
    m_pNetOptions = (dlg_network_options*)m_pManager->OpenDialog(m_UserID, "network options", irect(48,112,462,320), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER );
}

//=========================================================================

void dlg_main_menu::OnUpdate ( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;

    xbool IsNetworked = (pSvrMgr->GetLocalAddr().IP != 0);
    
    if( m_ButtonPushed != 0 && IsNetworked )
    {
        if( m_pNetOptions )
        {
            if( m_pNetOptions->GetDialogState() == 0 )
            {
                // Single player online
                if( m_ButtonPushed == 1 )
                {

                    tgl.NRequestedPlayers = 1;
                    fegl.nLocalPlayers    = 1;
                    fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
                    fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
                    fegl.iWarriorSetup    = 0;

                    audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
                    
                    m_pManager->EndDialog( m_UserID, TRUE );
                    m_pNetOptions = NULL;
                }

                // Two player online
                if( m_ButtonPushed == 2 )
                {

                    tgl.NRequestedPlayers = 2;
                    fegl.nLocalPlayers    = 2;
                    fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
                    fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
                    fegl.iWarriorSetup    = 0;

                    audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );

                    m_pManager->EndDialog( m_UserID, TRUE );
                    m_pNetOptions = NULL;
                }

                m_ButtonPushed = 0;
            }
        }
/*        else
        {
            // Single player online
            if( m_ButtonPushed == 1 )
            {

                tgl.NRequestedPlayers = 1;
                fegl.nLocalPlayers    = 1;
                fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
                fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
                fegl.iWarriorSetup    = 0;

                audio_Play( SFX_FRONTEND_SELECT_01_OPEN );

                if( m_pNetOptions )
                    m_pNetOptions->Destroy();
    
                m_pNetOptions = NULL;
            }

            // Two player online
            if( m_ButtonPushed == 2 )
            {

                tgl.NRequestedPlayers = 2;
                fegl.nLocalPlayers    = 2;
                fegl.State            = FE_STATE_GOTO_WARRIOR_SETUP;
                fegl.GotoGameState    = FE_STATE_GOTO_ONLINE;
                fegl.iWarriorSetup    = 0;

                audio_Play( SFX_FRONTEND_SELECT_01_OPEN );

                if( m_pNetOptions )
                    m_pNetOptions->Destroy();
    
                m_pNetOptions = NULL;
            }

            m_ButtonPushed = 0;

        }
*/
    }

}

//=========================================================================
