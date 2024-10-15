//=========================================================================
//
//  dlg_server_filter.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_font.hpp"

#include "dlg_serverfilter.hpp"
#include "dlg_loadsave.hpp"
#include "gamemgr/gamemgr.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"
#include "dlg_onlinejoin.hpp"

//==============================================================================
// Online Dialog
//==============================================================================

enum controls
{
    IDC_GAME_TYPE_TEXT,
    IDC_GAME_TYPE_BOX,
    IDC_MIN_PLAYERS_TEXT,
    IDC_MAX_PLAYERS_TEXT,
    IDC_BUDDIES_TEXT,
    IDC_MIN_PLAYERS_SLIDER,
    IDC_MAX_PLAYERS_SLIDER,
    IDC_BUDDIES_EDIT,
    IDC_DONE,
    IDC_SAVE,
    IDC_LOAD,
    IDC_RESET
};

ui_manager::control_tem ServerFilterControls[] =
{
    { IDC_GAME_TYPE_BOX,      IDS_NULL,           "combo",  134,  48, 170,  20, 0,  1, 4, 1, ui_win::WF_VISIBLE },
    { IDC_MIN_PLAYERS_SLIDER, IDS_NULL,           "slider", 134,  74, 170,  24, 0,  2, 4, 1, ui_win::WF_VISIBLE },
    { IDC_MAX_PLAYERS_SLIDER, IDS_NULL,           "slider", 134, 104, 170,  24, 0,  3, 4, 1, ui_win::WF_VISIBLE },
    { IDC_BUDDIES_EDIT,       IDS_NULL,           "edit",   134, 134, 170,  24, 0,  4, 4, 1, ui_win::WF_VISIBLE },

    { IDC_GAME_TYPE_TEXT,     IDS_SF_GAME_TYPE,   "text",    10,  48, 120,  20, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_MIN_PLAYERS_TEXT,   IDS_SF_MIN_PLAYERS, "text",     8,  74,  80,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_MAX_PLAYERS_TEXT,   IDS_SF_MAX_PLAYERS, "text",     8, 104,  80,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_BUDDIES_TEXT,       IDS_SF_BUDDIES,     "text",     8, 134,  80,  24, 0,  0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

	{ IDC_LOAD,               IDS_LOAD,           "button",  12, 168,  64,  24, 0,  5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SAVE,               IDS_SAVE,           "button",  87, 168,  64,  24, 1,  5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_RESET,              IDS_SF_RESET,       "button", 162, 168,  64,  24, 2,  5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DONE,               IDS_DONE,           "button", 237, 168,  64,  24, 3,  5, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem ServerFilterDialog =
{
    IDS_SF_TITLE,
    4, 7,
    sizeof(ServerFilterControls)/sizeof(ui_manager::control_tem),
    &ServerFilterControls[0],
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

void dlg_server_filter_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "serverfilter", &ServerFilterDialog, &dlg_server_filter_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_server_filter_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_server_filter* pDialog = new dlg_server_filter;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_server_filter
//=========================================================================

dlg_server_filter::dlg_server_filter( void )
{
}

//=========================================================================

dlg_server_filter::~dlg_server_filter( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_server_filter::Create( s32                        UserID,
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

    ((ui_text*)FindChildByID( IDC_MIN_PLAYERS_TEXT  ))->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    ((ui_text*)FindChildByID( IDC_MAX_PLAYERS_TEXT  ))->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    ((ui_text*)FindChildByID( IDC_BUDDIES_TEXT      ))->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    ((ui_text*)FindChildByID( IDC_GAME_TYPE_TEXT    ))->SetLabelFlags( ui_font::v_center|ui_font::h_left );

    m_pMaxPlayerText    = (ui_text*)    FindChildByID( IDC_MAX_PLAYERS_TEXT );
    m_pMinPlayerText    = (ui_text*)    FindChildByID( IDC_MIN_PLAYERS_TEXT );
    m_pMaxPlayerSlider  = (ui_slider*)  FindChildByID( IDC_MAX_PLAYERS_SLIDER );
    m_pMinPlayerSlider  = (ui_slider*)  FindChildByID( IDC_MIN_PLAYERS_SLIDER );
    m_pBuddyString      = (ui_edit*)    FindChildByID( IDC_BUDDIES_EDIT );
    m_pDone             = (ui_button*)  FindChildByID( IDC_DONE );
    m_pReset            = (ui_button*)  FindChildByID( IDC_RESET );
    m_pGameType         = (ui_combo*)   FindChildByID( IDC_GAME_TYPE_BOX );
    m_pLoad             = (ui_button*)  FindChildByID( IDC_LOAD );
    m_pSave             = (ui_button*)  FindChildByID( IDC_SAVE );

    m_pMaxPlayerSlider->SetRange(0,16);
    m_pMinPlayerSlider->SetRange(0,16);
    m_pMaxPlayerSlider->SetStep(1);
    m_pMinPlayerSlider->SetStep(1);
    m_pBuddyString->SetMaxCharacters(32-1);

    m_pGameType->AddItem( "ANY",              GAME_NONE   );
    m_pGameType->AddItem( "CAPTURE THE FLAG", GAME_CTF    );
    m_pGameType->AddItem( "CAPTURE & HOLD",   GAME_CNH    );
    m_pGameType->AddItem( "TEAM DEATHMATCH",  GAME_TDM    );
    m_pGameType->AddItem( "DEATHMATCH",       GAME_DM     );
    m_pGameType->AddItem( "HUNTERS",          GAME_HUNTER );
//    m_pGameType->AddItem( "CAMPAIGN",         GAME_CAMPAIGN );
    m_pGameType->SetSelection( 0 );

    ASSERT(pUserData);
    m_pFilter = (saved_filter *)pUserData;
    ReadFilter(m_pFilter);

    m_BackgroundColor   = FECOL_DIALOG2;
	m_WaitingOnSave     = FALSE;
	m_SaveComplete		= FALSE;


    // Return success code
    return Success;
}

//=========================================================================

void dlg_server_filter::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_server_filter::Render( s32 ox, s32 oy )
{
    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        // Fade out background
//        const irect& ru = m_pManager->GetUserBounds( m_UserID );
//        m_pManager->RenderRect( ru, xcolor(0,0,0,64), FALSE );

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

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}

//=========================================================================

void dlg_server_filter::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_server_filter::OnPadSelect( ui_win* pWin )
{
    if( pWin == (ui_win*)m_pDone )
    {
        WriteFilter(m_pFilter);
		if (fegl.FilterModified)
		{
			load_save_params options(0,(load_save_mode)(SAVE_FILTER|ASK_FIRST),this,m_pFilter);
			m_pManager->OpenDialog( m_UserID, 
							  "optionsloadsave", 
							  irect(0,0,300,150), 
							  NULL, 
							  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
							  (void *)&options );
			fegl.FilterModified = FALSE;
			m_WaitingOnSave = TRUE;
			m_SaveComplete=FALSE;
		}
		else
		{
	        m_pManager->EndDialog(m_UserID,TRUE);
		}

    }
    else if (pWin == (ui_win*)m_pReset)
    {
        ResetFilter();
    }
    else if (pWin == (ui_win*)m_pLoad)
    {
        load_save_params options(0,LOAD_FILTER,this,m_pFilter);
        m_pManager->OpenDialog( m_UserID, 
                              "filtersloadsave", 
                              irect(0,0,300,200), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
    }
    else if (pWin == (ui_win*)m_pSave)
    {
        load_save_params options(0,SAVE_FILTER,this,m_pFilter);
		WriteFilter(m_pFilter);
        m_pManager->OpenDialog( m_UserID, 
                              "filtersloadsave", 
                              irect(0,0,300,200), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
    }
}

//=========================================================================

void dlg_server_filter::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    WriteFilter(m_pFilter);
    // Close dialog and step back
    // TODO: Play Sound
	if (fegl.FilterModified)
	{
		load_save_params options(0,(load_save_mode)(SAVE_FILTER|ASK_FIRST),this,m_pFilter);
		m_pManager->OpenDialog( m_UserID, 
						  "optionsloadsave", 
						  irect(0,0,300,150), 
						  NULL, 
						  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
						  (void *)&options );
		m_WaitingOnSave = TRUE;
		m_SaveComplete=FALSE;
		fegl.FilterModified = FALSE;
	}
	else
	{
	    m_pManager->EndDialog( m_UserID, TRUE );
	}

}

//=========================================================================

void dlg_server_filter::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    if (Command == WN_USER)
    {
		ReadFilter(m_pFilter);
		if (m_WaitingOnSave)
		{
			m_SaveComplete=TRUE;
		}
    }
    UpdateControls();

}

//=========================================================================
void dlg_server_filter::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;

	if (m_WaitingOnSave)
	{
		if (m_SaveComplete)
		{
			m_SaveComplete=FALSE;
			m_WaitingOnSave=FALSE;
	        m_pManager->EndDialog(m_UserID,TRUE);
		}
	}
	else
	{
		UpdateControls();
	}
}

//=========================================================================
void dlg_server_filter::UpdateControls(void)
{

    if (m_pMaxPlayerSlider->GetValue() <= m_pMinPlayerSlider->GetValue() )
    {
        m_pMaxPlayerSlider->SetValue(m_pMinPlayerSlider->GetValue()+1);
    }

    if (m_pMinPlayerSlider->GetValue() > m_pMaxPlayerSlider->GetValue() )
    {
        m_pMinPlayerSlider->SetValue(m_pMaxPlayerSlider->GetValue()-1);
    }

    m_pMaxPlayerText->SetLabel( xwstring(xfs("MAX PLAYERS %d",m_pMaxPlayerSlider->GetValue())) );
    m_pMinPlayerText->SetLabel( xwstring(xfs("MIN PLAYERS %d",m_pMinPlayerSlider->GetValue())) );
}

//=========================================================================
void dlg_server_filter::ResetFilter(void)
{
    m_pFilter->BuddyString[0] = 0x0;
    m_pMinPlayerSlider->SetValue(0);
    m_pMaxPlayerSlider->SetValue(16);
    m_pBuddyString->SetLabel( m_pFilter->BuddyString );
    m_pGameType->SetSelection(0);
}

//=========================================================================
void dlg_server_filter::ReadFilter(const saved_filter* pFilter)
{

    m_pMinPlayerSlider->SetValue(pFilter->MinPlayers);
    m_pMaxPlayerSlider->SetValue(pFilter->MaxPlayers);
    m_pBuddyString->SetLabel( pFilter->BuddyString );

    if ( (pFilter->GameType >= 0) && (pFilter->GameType < m_pGameType->GetItemCount()) )
    {
        m_pGameType->SetSelection(pFilter->GameType);
    }
    else
    {
        m_pGameType->SetSelection(0);
    }

    UpdateControls();
}

//=========================================================================
void dlg_server_filter::WriteFilter(saved_filter* pFilter)
{
	s32 newmin,newmax,newtype;

	newmin = m_pMinPlayerSlider->GetValue();
	newmax = m_pMaxPlayerSlider->GetValue();
	newtype= m_pGameType->GetSelectedItemData();

	if ( (pFilter->MinPlayers != newmin) ||
		 (pFilter->MaxPlayers != newmax) ||
		 (pFilter->GameType   != newtype) )
	{
		fegl.FilterModified = TRUE;
	}

	if (x_wstrcmp(pFilter->BuddyString,(const xwchar*)m_pBuddyString->GetLabel())!=0 )
	{
		fegl.FilterModified = TRUE;
	}

    x_wstrcpy( pFilter->BuddyString, m_pBuddyString->GetLabel() );

    ASSERT(m_pGameType->GetSelectedItemData() == m_pGameType->GetSelection() );

    pFilter->MinPlayers = newmin;
    pFilter->MaxPlayers = newmax;
    pFilter->GameType = newtype;
}