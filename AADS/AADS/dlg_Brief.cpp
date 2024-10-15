//=========================================================================
//
//  dlg_brief.cpp
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

#include "dlg_Brief.hpp"

#include "fe_globals.hpp"
#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

#include "StringMgr\StringMgr.hpp"
#include "Demo1\Data\UI\ui_strings.h"
#include "Demo1\Data\Missions\Campaign.h"
#include "masterserver/masterserver.hpp"

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

ui_manager::control_tem BriefControls[] =
{
    { IDC_START,   IDS_CONTINUE,   "button",  376,    384,  96,  24, 1, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_BACK,    IDS_BACK,       "button",    8,    384,  96,  24, 0, 1, 1, 1, ui_win::WF_VISIBLE },

    { IDC_BRIEF,   IDS_NULL,       "textbox",   8,     56, 272, 318, 0, 0, 2, 1, ui_win::WF_VISIBLE },
    { IDC_IMAGE,   IDS_NULL,       "frame",   288,     56, 184, 318, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem BriefDialog =
{
    IDS_BRIEFING,
    2, 2,
    sizeof(BriefControls)/sizeof(ui_manager::control_tem),
    &BriefControls[0],
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

void dlg_brief_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "brief", &BriefDialog, &dlg_brief_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_brief_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags,void *pUserData )
{
    dlg_brief* pDialog = new dlg_brief;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags,pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_brief
//=========================================================================

dlg_brief::dlg_brief( void )
{
    StringMgr.LoadTable( "campaign", "data/missions/campaign.bin" );
}

//=========================================================================

dlg_brief::~dlg_brief( void )
{
    StringMgr.UnloadTable( "campaign" );
}

//=========================================================================

s32 BriefStrings[] =
{
    IDS_NULL,
    TEXT_M01_BRIEFING,
    TEXT_M02_BRIEFING,
    TEXT_M03_BRIEFING,
    TEXT_M04_BRIEFING,
    TEXT_M05_BRIEFING,
    TEXT_M06_BRIEFING,
    TEXT_M07_BRIEFING,
    TEXT_M08_BRIEFING,
    TEXT_M10_BRIEFING,
    TEXT_M11_BRIEFING,
    TEXT_M13_BRIEFING,
};

s32 BriefAudio[] =
{
    -1,
    M01_BRIEFING,
    M02_BRIEFING,
    M03_BRIEFING,
    M04_BRIEFING,
    M05_BRIEFING,
    M06_BRIEFING,
    M07_BRIEFING,
    M08_BRIEFING,
//    IDS_NULL,
    M10_BRIEFING,
    M11_BRIEFING,
//    IDS_NULL,
    M13_BRIEFING,
};

#ifdef TARGET_PC
#define DATAPATH "data\\ui\\pc\\"
#else
#define DATAPATH "data\\ui\\ps2\\"
#endif

const char* BriefBitmaps[] =
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

xbool dlg_brief::Create( s32                        UserID,
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
    m_pBack       = (ui_button*) FindChildByID( IDC_BACK );
    m_pStart      = (ui_button*) FindChildByID( IDC_START );

    m_pImage->SetBackgroundColor( xcolor(0,0,0,0) );

    m_pBrief->SetBackgroundColor( FECOL_DIALOG );

    m_pBrief->SetLabelFlags( ui_font::h_left|ui_font::v_top );
//    m_pBrief->SetLabel( xwstring("TODO: Briefing") );

    m_pBrief->SetLabel( StringMgr( "campaign", BriefStrings[fegl.CampaignMission] ) );

    VERIFY( m_Image.Load( BriefBitmaps[fegl.CampaignMission] ) );
    vram_Register( m_Image );

    m_SoundID = audio_Play( BriefAudio[fegl.CampaignMission], AUDFLAG_CHANNELSAVER );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_brief::Destroy( void )
{
    // Delete Image
    vram_Unregister( m_Image );
    m_Image.Kill();

    audio_Stop( m_SoundID );

    // Destory the dialog
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_brief::Render( s32 ox, s32 oy )
{
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
        ir.Translate( 2, 2 );

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

void dlg_brief::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;
}

//=========================================================================

void dlg_brief::OnPadSelect( ui_win* pWin )
{
    // Check for BACK button
    if( pWin == (ui_win*)m_pBack )
    {
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }

    // Check for START button
    if( pWin == (ui_win*)m_pStart )
    {
        // Set State
        fegl.State = FE_STATE_ENTER_CAMPAIGN;
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );

        // Configure the server
        pSvrMgr->SetAsServer( FALSE );          // This will make the server not broadcast it's presence in campaign mode
        pSvrMgr->SetName( StringMgr( "ui", IDS_LOCAL ) );
		tgl.pMasterServer->AcquireDirectory(FALSE);
		fegl.DeliberateDisconnect = TRUE;
		net_ActivateConfig(FALSE);

        tgl.UserInfo.MyName[0] = '\0';
        x_wstrcpy( tgl.UserInfo.Server, pSvrMgr->GetName() );

        tgl.UserInfo.IsServer = TRUE;
        tgl.UserInfo.MyAddr = pSvrMgr->GetLocalAddr();
        tgl.UserInfo.UsingConfig = TRUE;
    }
}

//=========================================================================

void dlg_brief::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
}

//=========================================================================

void dlg_brief::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================
