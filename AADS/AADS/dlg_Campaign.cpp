//=========================================================================
//
//  dlg_campaign.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "objects\player\PlayerObject.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"
#include "StringMgr\StringMgr.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_frame.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_textbox.hpp"

#include "dlg_Campaign.hpp"
#include "dlg_LoadSave.hpp"

#include "fe_globals.hpp"
#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

#include "StringMgr\StringMgr.hpp"
#include "Demo1\Data\UI\ui_strings.h"

//=========================================================================

extern server_manager* pSvrMgr;

//=========================================================================
//  Campaign Dialog
//=========================================================================

enum controls
{
//    IDC_DIFFICULTY,
    IDC_MISSIONS,
    IDC_COMMUNIQUE,

    IDC_BACK,
//    IDC_LOAD,
//    IDC_SAVE,
    IDC_START
};

ui_manager::control_tem CampaignControls[] =
{
    { IDC_MISSIONS,     IDS_MISSIONS,   "listbox",   8,     56, 184, 318, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_COMMUNIQUE,   IDS_NULL,       "textbox", 200,     56, 272, 318, 1, 1, 1, 1, ui_win::WF_VISIBLE },

    { IDC_BACK,         IDS_BACK,       "button",    8,    384,  96,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_LOAD,         IDS_LOAD,       "button",  131,    384,  96,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_SAVE,         IDS_SAVE,       "button",  253,    384,  96,  24, 1, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_START,        IDS_CONTINUE,   "button",  376,    384,  96,  24, 1, 2, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem CampaignDialog =
{
    IDS_CAMPAIGN,
    2, 3,
    sizeof(CampaignControls)/sizeof(ui_manager::control_tem),
    &CampaignControls[0],
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

void dlg_campaign_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "campaign", &CampaignDialog, &dlg_campaign_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_campaign_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags,void *pUserData )
{
    dlg_campaign* pDialog = new dlg_campaign;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags,pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_campaign
//=========================================================================

dlg_campaign::dlg_campaign( void )
{
    StringMgr.LoadTable( "communique", "data/missions/communique.bin" );
}

//=========================================================================

dlg_campaign::~dlg_campaign( void )
{
    Destroy();
    StringMgr.UnloadTable( "communique" );
}

//=========================================================================

xbool dlg_campaign::Create( s32                        UserID,
                            ui_manager*                pManager,
                            ui_manager::dialog_tem*    pDialogTem,
                            const irect&               Position,
                            ui_win*                    pParent,
                            s32                        Flags,
                            void*                      pUserData)
{
    xbool   Success = FALSE;
    s32     i       = 0;

    ASSERT( pManager );
    (void)pUserData;

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    // Initialize Data
    m_InUpdate = 0;

//    m_pDifficulty = (ui_combo*)  FindChildByID( IDC_DIFFICULTY );
    m_pMissions   = (ui_listbox*)FindChildByID( IDC_MISSIONS );
    m_pCommunique = (ui_textbox*)FindChildByID( IDC_COMMUNIQUE );
    m_pBack       = (ui_button*) FindChildByID( IDC_BACK );
//    m_pLoad       = (ui_button*) FindChildByID( IDC_LOAD );
//    m_pSave       = (ui_button*) FindChildByID( IDC_SAVE );
    m_pStart      = (ui_button*) FindChildByID( IDC_START );

//    m_pDifficulty->SetLabelWidth( 96 );

//    m_pDifficulty->AddItem( "Easy", 0 );
//    m_pDifficulty->AddItem( "Medium", 0 );
//    m_pDifficulty->AddItem( "Hard", 0 );
//    m_pDifficulty->SetSelection( 0 );

    m_pMissions  ->SetBackgroundColor( FECOL_DIALOG );
    m_pCommunique->SetBackgroundColor( FECOL_DIALOG );

    m_pCommunique->SetLabelFlags( ui_font::h_left|ui_font::v_top );
    m_pCommunique->SetLabel( StringMgr( "communique", 0 ) );

    fegl.ServerSettings.BotsDifficulty = 0.5f;

    s32 Count = 0;
    fegl.NCampaignMissions = 0;
    fegl.NTrainingMissions = 0;

    // Add all training missions to list
    for( i=0; fe_Missions[i].GameType != GAME_NONE; i++ )
    {
        if( fe_Missions[i].GameType == GAME_CAMPAIGN )
        {
            if( !x_strncmp( fe_Missions[i].Folder, "Training", 8 ))
            {
                s32 iItem = m_pMissions->AddItem( StringMgr( "MissionName", fe_Missions[i].DisplayNameString ), (s32)&fe_Missions[i] );
                m_pMissions->EnableItem( iItem, (iItem < tgl.HighestCampaignMission) );
                fegl.NTrainingMissions++;
                Count++;
            }
        }
    }
    
    // Add all campaign missions to list    
    for( i=0; fe_Missions[i].GameType != GAME_NONE; i++ )
    {
        if( fe_Missions[i].GameType == GAME_CAMPAIGN )
        {
            if( !x_strncmp( fe_Missions[i].Folder, "Mission", 7 ))
            {
                s32 iItem = m_pMissions->AddItem( StringMgr( "MissionName", fe_Missions[i].DisplayNameString ), (s32)&fe_Missions[i] );
                m_pMissions->EnableItem( iItem, (iItem < tgl.HighestCampaignMission) );
                fegl.NCampaignMissions++;
                Count++;
            }
        }
    }
    
    GameMgr.SetNCampaignMissions( fegl.NCampaignMissions );
/*
    // Add all missions to list
    s32 Count = 0;
    for( i=0 ; fe_Missions[i].GameType != GAME_NONE ; i++ )
    {
        if( fe_Missions[i].GameType == GAME_CAMPAIGN )
        {
            m_pMissions->AddItem( StringMgr( "MissionName", fe_Missions[i].DisplayNameString ), (s32)&fe_Missions[i] );
            Count++;
        }
    }
*/
//    if( tgl.MissionSuccess || (fegl.LastPlayedCampaign == 0) )
//        m_OldMission = fegl.LastPlayedCampaign;//tgl.HighestCampaignMission;
//    else
//        m_OldMission = fegl.LastPlayedCampaign-1;
    m_OldMission = fegl.LastPlayedCampaign;

//    if( Count > 0 )
//    {
    if( ((fegl.LastPlayedCampaign+1) <= tgl.HighestCampaignMission) && (tgl.MissionSuccess) )
    {
        m_pMissions->SetSelection( fegl.LastPlayedCampaign );
    }
    else
    {
        if( fegl.LastPlayedCampaign > 0 )
            m_pMissions->SetSelection( fegl.LastPlayedCampaign - 1 );
        else
            m_pMissions->SetSelection( fegl.LastPlayedCampaign );
    }
//    }
//    else
//    {
//        m_pMissions->SetSelection( -1 );
//    }

    // Set Initial Controls
    CampaignToControls();

    // Clear Mission Sucess flag in tgl
    tgl.MissionSuccess = FALSE;

    // Return success code
    return Success;
}

//=========================================================================

void dlg_campaign::Destroy( void )
{
    // Destory the dialog
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_campaign::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );

/*
    // Get window to render briefing
    irect   Window = m_pFrame->GetPosition();
    Window.Deflate( 8, 6 );
    LocalToScreen( Window );

    if( m_pMissions->GetSelection() != -1 )
    {
        mission_def* pMissionDef = (mission_def*)m_pMissions->GetSelectedItemData();
        const xwstring& s = m_pManager->WordWrapString( 0, Window, pMissionDef->MissionDescription );
        m_pManager->RenderText( 0, Window, 0, xcolor(255,255,255,224), s );
    }
*/
}

//=========================================================================

void dlg_campaign::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;

    // Enable the missions you can play
    for( s32 i=0 ; i<m_pMissions->GetItemCount() ; i++ )
    {
        if( (i+1) <= tgl.HighestCampaignMission )
            m_pMissions->EnableItem( i, TRUE );
        else
            m_pMissions->EnableItem( i, FALSE );
    }

    if( (m_OldMission != fegl.LastPlayedCampaign) || (tgl.MissionSuccess) )
    {        
        if( ((fegl.LastPlayedCampaign+1) <= tgl.HighestCampaignMission) && (tgl.MissionSuccess) )
        {
            m_pMissions->SetSelection( fegl.LastPlayedCampaign );
        }
        else
        {
            if( fegl.LastPlayedCampaign > 0 )
                m_pMissions->SetSelection( fegl.LastPlayedCampaign - 1 );
            else
                m_pMissions->SetSelection( fegl.LastPlayedCampaign );
        }

        // Clear Mission Sucess flag in tgl
        tgl.MissionSuccess = FALSE;
        
        m_OldMission = fegl.LastPlayedCampaign;
    }
}

//=========================================================================

void dlg_campaign::OnPadSelect( ui_win* pWin )
{
    // Check for BACK button
    if( pWin == (ui_win*)m_pBack )
    {
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }

/*
    else if (pWin == (ui_win *)m_pLoad)
    {
        load_save_params options(0,LOAD_CAMPAIGN_OPTIONS,this);
        m_pManager->OpenDialog( m_UserID, 
                              "optionsloadsave", 
                              irect(0,0,300,150), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
    }
    else if (pWin == (ui_win *)m_pSave)
    {
        load_save_params options(0,SAVE_CAMPAIGN_OPTIONS,this);
        m_pManager->OpenDialog( m_UserID, 
                              "optionsloadsave", 
                              irect(0,0,300,150), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
    }
*/

    // Check for START button
    else if( pWin == (ui_win*)m_pStart )
    {
        if( m_pMissions->GetSelection() != -1 )
        {
            // Set State
            fegl.State = FE_STATE_GOTO_CAMPAIGN_BRIEFING;
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
			if (fegl.ModemConnection)
			{
				fegl.DeliberateDisconnect = TRUE;
				net_ActivateConfig(FALSE);
			}

            mission_def*    pMissionDef = (mission_def*)m_pMissions->GetSelectedItemData();
            ASSERT( pMissionDef );
            x_strcpy( fegl.MissionName, pMissionDef->Folder );
            fegl.GameHost        = TRUE;
            fegl.GameType        = GAME_CAMPAIGN;
            fegl.CampaignMission = m_pMissions->GetSelection() + 1;
            fegl.LastPlayedCampaign = m_pMissions->GetSelection() + 1;

            if( fegl.CampaignMission > fegl.NTrainingMissions )
            {
                // Set correct campaign mission number 1 -> 13
                fegl.CampaignMission -= fegl.NTrainingMissions;
            }
            else
            {
                // Set correct training mission number
                // Note: training mission numbering starts AFTER the campaign missions
                fegl.CampaignMission += fegl.NCampaignMissions;
            }

/*
            // set up bot difficulty
            switch ( m_pDifficulty->GetSelection() )
            {
            case 0:
                fegl.ServerSettings.BotsDifficulty = 0.1f;
                break;
            case 1:
                fegl.ServerSettings.BotsDifficulty = 0.5f;
                break;
            case 2:
                fegl.ServerSettings.BotsDifficulty = 1.0f;
                break;
            default:
                ASSERT( 0 ); // Not set up for changes to GUI...
                fegl.ServerSettings.BotsDifficulty = 0.1f;
            }
*/
            // Skip briefing on training missions
            if( fegl.CampaignMission > fegl.NCampaignMissions )
            {
                // Set State
                fegl.State = FE_STATE_ENTER_CAMPAIGN;

                // Configure the server
                pSvrMgr->SetAsServer( FALSE );      // Not a server when in a campaign
                pSvrMgr->SetName( StringMgr( "ui", IDS_LOCAL ) );
                tgl.UserInfo.MyName[0] = '\0';
                x_wstrcpy( tgl.UserInfo.Server, pSvrMgr->GetName() );

                tgl.UserInfo.IsServer = TRUE;
                tgl.UserInfo.MyAddr = pSvrMgr->GetLocalAddr();
                tgl.UserInfo.UsingConfig = TRUE;
            }

        }        
    }
}

//=========================================================================

void dlg_campaign::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
}

//=========================================================================

void dlg_campaign::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    ControlsToCampaign();
    CampaignToControls();

    // If X pressed in Mission List goto the host control
    if( (pSender == (ui_win*)m_pMissions) && (Command == WN_LIST_ACCEPTED) )
    {
        GotoControl( (ui_control*)m_pStart );
    }
}

//=========================================================================

void dlg_campaign::CampaignToControls( void )
{
    // Only enter if not already updating
    if( !m_InUpdate )
    {
        // Set updating flag
        m_InUpdate++;

        // Set Communique string
        s32 Mission = m_pMissions->GetSelection();
        if( Mission != -1 )
            m_pCommunique->SetLabel( StringMgr( "communique", Mission ) );
        else
            m_pCommunique->SetLabel( xwstring("") );

        // Clear updating flag
        m_InUpdate--;
    }
}

//=========================================================================

void dlg_campaign::ControlsToCampaign( void )
{
    if( !m_InUpdate )
    {
    }
}

//=========================================================================
