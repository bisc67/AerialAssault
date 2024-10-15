//=========================================================================
//
//  dlg_debrief.cpp
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

#include "dlg_DeBrief.hpp"
#include "dlg_loadsave.hpp"

#include "fe_globals.hpp"
#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

#include "StringMgr\StringMgr.hpp"
#include "Demo1\Data\UI\ui_strings.h"
#include "Demo1\Data\Missions\Campaign.h"

//=========================================================================

extern server_manager* pSvrMgr;

//=========================================================================
//  Campaign Dialog
//=========================================================================

enum controls
{
    IDC_IMAGE,
    IDC_BRIEF,

    IDC_BACK,
    IDC_START
};

ui_manager::control_tem DeBriefControls[] =
{
    { IDC_START,   IDS_CONTINUE,   "button",  376,    384,  96,  24, 1, 1, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_BACK,    IDS_BACK,       "button",    8,    384,  96,  24, 0, 1, 1, 1, ui_win::WF_VISIBLE },

    { IDC_BRIEF,   IDS_NULL,       "textbox",   8,     56, 272, 318, 0, 0, 2, 1, ui_win::WF_VISIBLE },
    { IDC_IMAGE,   IDS_NULL,       "frame",   288,     56, 184, 318, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem DeBriefDialog =
{
    IDS_DEBRIEFING,
    2, 2,
    sizeof(DeBriefControls)/sizeof(ui_manager::control_tem),
    &DeBriefControls[0],
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
s32 LastSaved;
//=========================================================================
//  Registration function
//=========================================================================

void dlg_debrief_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "debrief", &DeBriefDialog, &dlg_debrief_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_debrief_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags,void *pUserData )
{
    dlg_debrief* pDialog = new dlg_debrief;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags,pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_debrief
//=========================================================================

dlg_debrief::dlg_debrief( void )
{
    StringMgr.LoadTable( "campaign", "data/missions/campaign.bin" );
}

//=========================================================================

dlg_debrief::~dlg_debrief( void )
{
//    Destroy();
    StringMgr.UnloadTable( "campaign" );
}

//=========================================================================

s32 DeBriefStrings[] =
{
    IDS_NULL,
    TEXT_M01_DEBRIEF,
    TEXT_M02_DEBRIEF,
    TEXT_M03_DEBRIEF,
    TEXT_M04_DEBRIEF,
    TEXT_M05_DEBRIEF,
    TEXT_M06_DEBRIEF,
    TEXT_M07_DEBRIEF,
    TEXT_M08_DEBRIEF,
//    IDS_NULL,
    TEXT_M10_DEBRIEF,
    TEXT_M11_DEBRIEF,
//    IDS_NULL,
    TEXT_M13_DEBRIEF,
};

s32 DeBriefAudio[] =
{
    -1,
    M01_DEBRIEF,
    M02_DEBRIEF,
    M03_DEBRIEF,
    M04_DEBRIEF,
    M05_DEBRIEF,
    M06_DEBRIEF,
    M07_DEBRIEF,
    M08_DEBRIEF,
//    IDS_NULL,
    M10_DEBRIEF,
    M11_DEBRIEF,
//    IDS_NULL,
    M13_DEBRIEF,
};

#ifdef TARGET_PC
#define DATAPATH "data\\ui\\pc\\"
#else
#define DATAPATH "data\\ui\\ps2\\"
#endif

const char* DeBriefBitmaps[] =
{
    NULL,
    DATAPATH"mission_01.xbmp",
    DATAPATH"mission_02.xbmp",
    DATAPATH"mission_03.xbmp",
    DATAPATH"mission_04.xbmp",
    DATAPATH"mission_05.xbmp",
    DATAPATH"mission_06.xbmp",
    DATAPATH"mission_07.xbmp",
    DATAPATH"mission_08.xbmp",
    DATAPATH"mission_10.xbmp",
    DATAPATH"mission_11.xbmp",
    DATAPATH"mission_13.xbmp"
};

xbool dlg_debrief::Create( s32                        UserID,
                           ui_manager*                pManager,
                           ui_manager::dialog_tem*    pDialogTem,
                           const irect&               Position,
                           ui_win*                    pParent,
                           s32                        Flags,
                           void*                      pUserData)
{
    xbool   Success = FALSE;
//    s32     i       = 0;

    ASSERT( pManager );
    (void)pUserData;

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    // Initialize Data
    m_InUpdate = 0;

    m_pImage      = (ui_frame*)  FindChildByID( IDC_IMAGE );
    m_pBrief      = (ui_textbox*)FindChildByID( IDC_BRIEF );
    m_pStart      = (ui_button*) FindChildByID( IDC_START );

    m_pBrief->SetBackgroundColor( FECOL_DIALOG );

    m_pBrief->SetLabelFlags( ui_font::h_left|ui_font::v_top );
//    m_pBrief->SetLabel( xwstring("TODO: Briefing") );

    m_pBrief->SetLabel( StringMgr( "campaign", DeBriefStrings[fegl.CampaignMission] ) );

    // IMPORTANT NOTE!!!
    // When the debrief screen gets implemented - search for "HACK" in campaign_logic.cpp
    // Remove the lines following it as they increment the HighestCampaignMission also

    // Increment the players available missions
    if( tgl.HighestCampaignMission == fegl.LastPlayedCampaign )
    {
        if( tgl.HighestCampaignMission < 16 )
            tgl.HighestCampaignMission++;

        #ifdef DEMO_DISK_HARNESS
        tgl.HighestCampaignMission = 5;
        #endif
        // TODO: Autosave???
    }

    VERIFY( m_Image.Load( DeBriefBitmaps[fegl.CampaignMission] ) );
    vram_Register( m_Image );
    
    m_SoundID = audio_Play( DeBriefAudio[fegl.CampaignMission], AUDFLAG_CHANNELSAVER );
    // Return success code
    return Success;
}

//=========================================================================

void dlg_debrief::Destroy( void )
{
    // Delete Image
    vram_Unregister( m_Image );
    m_Image.Kill();

    audio_Stop( m_SoundID );
    
    // Destory the dialog
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_debrief::Render( s32 ox, s32 oy )
{
//    ui_dialog::Render( ox, oy );

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
    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        // Get window rectangle
        irect   r;
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        irect   rb = r;
        rb.Deflate( 1, 1 );

        // Render frame
        if( m_Flags & WF_BORDER )
        {
            // Render background color
            if( m_BackgroundColor.A > 0 )
            {
                m_pManager->RenderRect( rb, m_BackgroundColor, FALSE );
            }

            // Render Title Bar Gradient
            rb.SetHeight( 40 );
            xcolor c1 = FECOL_TITLE1;
            xcolor c2 = FECOL_TITLE2;
            m_pManager->RenderGouraudRect( rb, c1, c1, c2, c2, FALSE );

            // Render the Frame
            m_pManager->RenderElement( m_iElement, r, 0 );

            // Render Title
            rb.Deflate( 16, 0 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_WHITE, m_Label );
        }

        // Render Image
        irect ir = m_pImage->GetPosition();
        LocalToScreen( ir );
#if defined(PAL_RELEASE)
        ir.Translate( 2, 2+32 );
#else
        ir.Translate( 2, 2 );
#endif

        draw_Begin( DRAW_SPRITES, DRAW_2D|DRAW_TEXTURED|DRAW_NO_ZBUFFER|DRAW_USE_ALPHA );
        draw_SetTexture( m_Image );
        draw_Sprite( vector3((f32)ir.l,(f32)ir.t,0.0f),vector2((f32)m_Image.GetWidth(), (f32)m_Image.GetHeight()), XCOLOR_WHITE );
        draw_End();

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }

}

//=========================================================================

void dlg_debrief::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;
}

//=========================================================================

void dlg_debrief::OnPadSelect( ui_win* pWin )
{
    // Check for START button
    if( pWin == (ui_win*)m_pStart )
    {
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
		
        if( LastSaved != tgl.HighestCampaignMission )
        {
            LastSaved = tgl.HighestCampaignMission;
            load_save_params options(0,(load_save_mode)(SAVE_CAMPAIGN_ENDOFGAME|ASK_FIRST),NULL);
		    m_pManager->OpenDialog( m_UserID, 
							  "optionsloadsave", 
							  irect(0,0,300,150), 
							  NULL, 
							  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
							  (void *)&options );
        }
/*
        // Set State
        fegl.State = FE_STATE_ENTER_CAMPAIGN;
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );

        // Configure the server
        pSvrMgr->SetAsServer( FALSE );      // Not a server when in a campaign
        pSvrMgr->SetName( StringMgr( "ui", IDS_LOCAL ) );
        tgl.UserInfo.MyName[0] = '\0';
        x_wstrcpy( tgl.UserInfo.Server, pSvrMgr->GetName() );

        tgl.UserInfo.IsServer = TRUE;
        tgl.UserInfo.MyAddr = pSvrMgr->GetLocalAddr();
        tgl.UserInfo.UsingConfig = TRUE;
*/
    }
}

//=========================================================================

void dlg_debrief::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
}

//=========================================================================

void dlg_debrief::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================
