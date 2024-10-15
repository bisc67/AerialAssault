//=========================================================================
//
//  dlg_load_save.cpp
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

#include "dlg_LoadSave.hpp"
#include "dlg_message.hpp"
#include "savedgame.hpp"
#include "cardmgr/cardmgr.hpp"
#include "globals.hpp"
#include "titles.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "audiomgr/audio.hpp"
#include "fe_colors.hpp"

#include "StringMgr\StringMgr.hpp"
#include "Demo1\Data\UI\ui_strings.h"

extern xbool UseAutoAimHelp;
extern xbool UseAutoAimHint;
//==============================================================================
// Online Dialog
//==============================================================================

enum controls
{
    IDC_CANCEL,
    IDC_DELETE,
    IDC_SAVE,
    IDC_DIRECTORY,
    IDC_SLOT_NUMBER,
};

ui_manager::control_tem WarriorLoadSaveControls[] =
{
    { IDC_CANCEL,       IDS_CANCEL,     "button",    8, 218,  80,  24, 0,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DELETE,       IDS_DELETE,     "button",  110, 218,  80,  24, 1,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SAVE,         IDS_SAVE,       "button",  212, 218,  80,  24, 2,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SLOT_NUMBER,  IDS_SAVE,       "combo",     8,  52, 284,  20, 0,  0, 0, 0, 0 },
    { IDC_DIRECTORY,    IDS_NULL,       "listbox",   8,  50, 284, 160, 0,  0, 3, 2, ui_win::WF_VISIBLE },

};

ui_manager::dialog_tem WarriorLoadSaveDialog =
{
    IDS_LOAD_SAVE_WARRIOR,
    3, 3,
    sizeof(WarriorLoadSaveControls)/sizeof(ui_manager::control_tem),
    &WarriorLoadSaveControls[0],
    0
};

ui_manager::control_tem OptionsLoadSaveControls[] =
{
    { IDC_CANCEL,       IDS_CANCEL,     "button",    8,  80,  80,  24, 0,  1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DELETE,       IDS_DELETE,     "button",  110, 208,  80,  24, 0,  0, 0, 0, ui_win::WF_DISABLED },
    { IDC_SAVE,         IDS_SAVE,       "button",  210,  80,  80,  24, 1,  1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SLOT_NUMBER,  IDS_SAVE,       "combo",     8,  52, 284,  20, 0,  0, 0, 0, 0 },
    { IDC_DIRECTORY,    IDS_NULL,       "listbox",   8,  50, 284,28*2, 0,  0, 0, 0, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem OptionsLoadSaveDialog =
{
    IDS_LOAD_AND_SAVE_OPTIONS,
    2, 2,
    sizeof(OptionsLoadSaveControls)/sizeof(ui_manager::control_tem),
    &OptionsLoadSaveControls[0],
    0
};

ui_manager::control_tem FiltersLoadSaveControls[] =
{
    { IDC_CANCEL,       IDS_CANCEL,     "button",    8, 170,  80,  24, 0,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DELETE,       IDS_DELETE,     "button",  110, 170,  80,  24, 1,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SAVE,         IDS_SAVE,       "button",  212, 170,  80,  24, 2,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SLOT_NUMBER,  IDS_SAVE,       "combo",     8,  52, 284,  20, 0,  0, 0, 0, 0 },
    { IDC_DIRECTORY,    IDS_NULL,       "listbox",   8,  50, 284,28*4, 0,  1, 3, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem FiltersLoadSaveDialog =
{
    IDS_LOAD_AND_SAVE_FILTERS,
    3, 3,
    sizeof(FiltersLoadSaveControls)/sizeof(ui_manager::control_tem),
    &FiltersLoadSaveControls[0],
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

void dlg_load_save_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "warriorloadsave", &WarriorLoadSaveDialog, &dlg_load_save_factory );
    pManager->RegisterDialogClass( "optionsloadsave", &OptionsLoadSaveDialog, &dlg_load_save_factory );
    pManager->RegisterDialogClass( "filtersloadsave", &FiltersLoadSaveDialog, &dlg_load_save_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_load_save_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_load_save* pDialog = new dlg_load_save;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_load_save
//=========================================================================

dlg_load_save::dlg_load_save( void )
{
}

//=========================================================================

dlg_load_save::~dlg_load_save( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_load_save::Create( s32                        UserID,
                                ui_manager*                pManager,
                                ui_manager::dialog_tem*    pDialogTem,
                                const irect&               Position,
                                ui_win*                    pParent,
                                s32                        Flags,
                                void*                      pUserData )
{
    xbool   Success = FALSE;
    load_save_params *pParams;
    m_DialogsAllocated = 0;

    pParams = (load_save_params *)pUserData;
    m_Mode = (load_save_mode)(pParams->GetMode() & ~MODE_OPTION_MASK);
    m_pParent = pParams->GetDialog();

    ASSERT( pManager );

    m_pManager = pManager;
    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_BackgroundColor   = FECOL_DIALOG2;
    m_CardScanDelay     = 0.0f;
    m_CurrentCard       = 0;
	x_memset(m_CardInfo,0,sizeof(m_CardInfo));
    m_LastCardStatus    = -1;
    m_CardCount         = 0;
    m_LastCardCount     = -1;
    m_pMessage          = NULL;
    m_ForceReload       = TRUE;
    m_StateStackTop     = 0;
    m_SelectedCard      = -1;
    // Set initial state
    SetState(LS_STATE_IDLE);
    ((ui_combo*) FindChildByID( IDC_SLOT_NUMBER ))->SetLabelWidth(124);
    ((ui_combo*) FindChildByID( IDC_SLOT_NUMBER ))->SetLabel("MEMORY CARD");

    m_pDirectory = (ui_listbox*)FindChildByID( IDC_DIRECTORY );
    m_pCancel    = (ui_button*) FindChildByID( IDC_CANCEL    );
    m_pSave      = (ui_button*) FindChildByID( IDC_SAVE      );
    m_pDelete    = (ui_button*) FindChildByID( IDC_DELETE    );
    m_pSlot      = (ui_combo*)  FindChildByID( IDC_SLOT_NUMBER);
    m_Exit       = FALSE;
	m_DataError	 = 0;

    m_pSlot->SetLabelWidth(123);
    x_memset(&m_SavedData,0,sizeof(saved_game));
    m_pDirectory->AddItem( STR_NO_CARD_PRESENT );
    m_pDirectory->SetSelection( 0 );

    m_IsSaving = FALSE;
    // Hide the delete button
    if (m_pDelete)
        m_pDelete->SetFlags((m_pDelete->GetFlags() & ~WF_VISIBLE) | WF_DISABLED);

    m_pWarrior = NULL;
//    m_pFilter  = NULL;
    m_PlayerIndex = 0;
    switch (m_Mode)
    {
    case LOAD_WARRIOR:
        SetLabel("LOAD WARRIOR");
        m_pWarrior = (warrior_setup* )pParams->GetData();
        m_pDelete->SetFlags((m_pDelete->GetFlags() | WF_VISIBLE) & ~WF_DISABLED);
        m_PlayerIndex = pParams->GetPlayerIndex();
        ASSERT(m_pWarrior);
        x_strcpy(m_Filename,SAVE_FILENAME);
        break;
    case SAVE_WARRIOR:
        SetFlags((GetFlags() & ~WF_VISIBLE)|WF_DISABLED);
        m_pWarrior = (warrior_setup* )pParams->GetData();
        m_PlayerIndex = pParams->GetPlayerIndex();
        ASSERT(m_pWarrior);
        SetLabel( xwstring( xfs("SAVE WARRIOR %d", pParams->GetPlayerIndex()+1) ) );
        x_strcpy(m_Filename,SAVE_FILENAME);
        m_IsSaving = TRUE;
        SetState(LS_STATE_START_SAVE);
        break;
    case LOAD_NETWORK_OPTIONS:
    case LOAD_CAMPAIGN_OPTIONS:
    case LOAD_AUDIO_OPTIONS:
        SetFlags((GetFlags() & ~WF_VISIBLE)|WF_DISABLED);
        SetLabel("LOAD OPTIONS");
        x_strcpy(m_Filename,SAVE_FILENAME);
        SetState(LS_STATE_LOAD);
        break;
    case SAVE_NETWORK_OPTIONS:
    case SAVE_CAMPAIGN_OPTIONS:
	case SAVE_CAMPAIGN_ENDOFGAME:
    case SAVE_AUDIO_OPTIONS:

        SetFlags((GetFlags() & ~WF_VISIBLE)|WF_DISABLED);
        m_IsSaving = TRUE;
        SetLabel("SAVE OPTIONS");
        x_strcpy(m_Filename,SAVE_FILENAME);
        SetState(LS_STATE_START_SAVE);
        break;
/*
    case LOAD_FILTER:
        m_pFilter = (saved_filter* )pParams->GetData();
        ASSERT(m_pFilter);
        m_pDelete->SetFlags((m_pDelete->GetFlags() | WF_VISIBLE) & ~WF_DISABLED);
        SetLabel("LOAD FILTERS");
        x_strcpy(m_Filename,SAVE_FILENAME);
        break;
    case SAVE_FILTER:
        m_pFilter = (saved_filter* )pParams->GetData();
        ASSERT(m_pFilter);
        m_IsSaving = TRUE;
        SetLabel("SAVE FILTERS");
        x_strcpy(m_Filename,SAVE_FILENAME);
        SetState(LS_STATE_START_SAVE);
        break;
*/
    default:
        ASSERT(FALSE);
        break;
    }
    if (m_IsSaving)
        m_pSave->SetLabel("SAVE");
    else
        m_pSave->SetLabel("LOAD");

    // The above code will set this to the required save state. We then push it
    // and switch to the checking state. When this completes, it will pop back
    // to the previous state (set above).
    PushState(LS_STATE_CHECKING_CARD);

	if (pParams->GetMode() & ASK_FIRST)
	{
		PushState(LS_STATE_ASK_SAVE);
	}

    // Return success code
    return Success;
}

//=========================================================================

void dlg_load_save::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_load_save::Render( s32 ox, s32 oy )
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

void dlg_load_save::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_load_save::OnPadSelect( ui_win* pWin )
{
    if( pWin == (ui_win*)m_pCancel )
    {
        // Close dialog and step back
        m_Exit = TRUE;
        // TODO: Play Sound
    }
    else if( pWin == (ui_win*)m_pDelete )
    {
        s32 index;

		index = m_pDirectory->GetSelection();

		if (m_Mode == LOAD_FILTER)
		{
			if (index >= m_SavedData.FilterCount)
			{
				WarningBox("ERROR!","No filter selected to delete");
				m_MessageDelay = 1000.0f;
				SetState(LS_STATE_MESSAGE_DELAY);
			}
			else
			{
				SetState(LS_STATE_START_DELETE);
			}        // TODO: Delete File
		}
		else
		{
			if (index >= m_SavedData.WarriorCount)
			{
				WarningBox("ERROR!","No warrior selected to delete");
				m_MessageDelay = 1000.0f;
				SetState(LS_STATE_MESSAGE_DELAY);
			}
			else
			{
				SetState(LS_STATE_START_DELETE);
			}        // TODO: Delete File
		}
    }
    else if( pWin == (ui_win*)m_pSave )
    {
        if (m_IsSaving)
            SetState(LS_STATE_START_SAVE);
        else
            SetState(LS_STATE_LOAD);
        // TODO: Save File
    }
}

//=========================================================================

void dlg_load_save::OnPadBack( ui_win* pWin )
{
    (void)pWin;

	if (m_IsSaving)
	{
		m_StateStackTop = 0;
		m_LastCardCount = 0;
		m_SelectedCard = -1;
		SetState(LS_STATE_IDLE);
		PushState(LS_STATE_START_SAVE);
		PushState(LS_STATE_CHECKING_CARD);
		PushState(LS_STATE_ASK_SAVE);
		m_ForceReload=TRUE;
		PushState(LS_STATE_MESSAGE_DELAY_POP);
	}
    // Close dialog and step back
    //m_Exit = TRUE;
    // TODO: Play Sound
}

//=========================================================================

void dlg_load_save::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    if ((pSender == (ui_win *)m_pDirectory) && (Command == WN_LIST_ACCEPTED) )
    {
        GotoControl( (ui_control *)m_pSave);
    }



//    UpdateControls();
}

//=========================================================================

void dlg_load_save::UpdateControls( void )
{
}

//=========================================================================

void dlg_load_save::WarningBox( const char* pTitle, const char* pMessage, xbool AllowPrematureExit )
{
    const xwchar* pDone;

    if( AllowPrematureExit )  pDone = StringMgr( "ui", IDS_DONE );
    else                      pDone = NULL; //StringMgr( "ui", IDS_NULL );

    if( m_pMessage && (m_MessageResult == DLG_MESSAGE_IDLE) )
    {
        m_pManager->EndDialog( m_UserID, TRUE );
        m_pMessage = NULL;
        m_DialogsAllocated--;
    }

    m_pMessage = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                       "message",
                                                       irect(0,0,300,200),
                                                       NULL,
                                                       WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
    m_DialogsAllocated++;
    ASSERT(m_pMessage);
    m_pMessage->Configure( xwstring(pTitle), 
                           NULL, 
                           pDone, 
                           xwstring(pMessage), 
                           XCOLOR_WHITE,
                           &m_MessageResult );
}

//=========================================================================

void dlg_load_save::OptionBox( const char* pTitle,const char* pMessage, const xwchar *pYes, const xwchar *pNo,const xbool DefaultToNo, const xbool AllowCancel )
{
    if (pYes == NULL)
    {
        pYes = StringMgr("ui",IDS_YES);
    }
    if (pNo == NULL)
    {
        pNo = StringMgr("ui",IDS_NO);
    }

    if( m_pMessage && (m_MessageResult == DLG_MESSAGE_IDLE) )
    {
        m_pManager->EndDialog(m_UserID,TRUE);
        m_DialogsAllocated--;
    }
    m_pMessage = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                       "message",
                                                       irect(0,0,300,200),
                                                       NULL,
                                                       WF_VISIBLE|WF_BORDER|WF_DLG_CENTER);
    m_DialogsAllocated++;
    ASSERT(m_pMessage);
    m_pMessage->Configure( xwstring(pTitle), 
                           pYes,
                           pNo,
                           xwstring(pMessage), 
                           XCOLOR_WHITE,&m_MessageResult,DefaultToNo,AllowCancel);
}

//=========================================================================
void dlg_load_save::PushState(load_save_state NewState)
{
    ASSERT(m_StateStackTop < LS_STATE_STACK_SIZE);
    m_StateStack[m_StateStackTop] = m_DialogState;
    m_StateStackTop++;
    m_DialogState = NewState;
}

//=========================================================================
void dlg_load_save::PopState(void)
{
    if (m_StateStackTop == 0)
    {
        SetState(LS_STATE_IDLE);
    }
    else
    {
        m_StateStackTop--;
        SetState(m_StateStack[m_StateStackTop]);
    }
}

//=========================================================================
void dlg_load_save::SetState(load_save_state NewState)
{
    m_DialogState = NewState;
}

//=========================================================================
void dlg_load_save::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    char    name[128];
    s32     i;

    (void)pWin;
    (void)DeltaTime;

    if (m_Exit)
    {
        ui_win* pParent = m_pParent;

        // If we need to close the message, let's do that now
        if (m_pMessage && (m_MessageResult == DLG_MESSAGE_IDLE) )
        {
            m_pManager->EndDialog(m_UserID,TRUE);
            m_pMessage = NULL;
        }
        m_pManager->EndDialog(m_UserID,TRUE);

		if( pParent )
			pParent->OnNotify( pParent, NULL, WN_USER, NULL );

        return;
    }

    switch (GetState())
    {

    //-------------------------------------------------------------------------
    case LS_STATE_IDLE:
        // The message result will be set to DLG_MESSAGE_IDLE while no user
        // input has been given. If the user cancels or accepts in any manner,
        // then their message box is automatically destroyed so we do not need
        // to do the EndDialog in that case.
        m_CardScanDelay -= DeltaTime;
        if (m_CardScanDelay < 0.0f)
        {
            GetCardCount();
			if (CheckCardForErrors())
				break;
#if 0
			if (m_LastCardStatus == CARD_ERR_UNPLUGGED)
			{
				// This will force it to the card 1, card 2 menu
				ErrorReset(LS_ERR_CARD_REMOVED);
			}
#endif
            m_CardScanDelay = 2.0f;
        }

        if (m_ForceReload)
        {
            PushState(LS_STATE_CHECKING_CARD);
			m_ForceReload=FALSE;
        }

        if ( m_pMessage && (m_MessageResult == DLG_MESSAGE_IDLE) )
        {
            m_pManager->EndDialog(m_UserID,TRUE);
            m_DialogsAllocated--;
        }
        m_pMessage =NULL;

		switch (m_Mode)
		{
		case SAVE_NETWORK_OPTIONS:
		case SAVE_CAMPAIGN_OPTIONS:
		case SAVE_CAMPAIGN_ENDOFGAME:
		case SAVE_AUDIO_OPTIONS:
		case LOAD_NETWORK_OPTIONS:
		case LOAD_CAMPAIGN_OPTIONS:
		case LOAD_AUDIO_OPTIONS:
			m_Exit = TRUE;
			break;
		default:
			break;
		}
    break;
    //-------------------------------------------------------------------------
    case LS_STATE_CHECKING_CARD:
        WarningBox("PLEASE WAIT...","Checking for a memory card(8MB)\n"
									"(for PlayStation""\x10""2)...\n",FALSE);
        SetState(LS_STATE_WAIT_CHECK);
        m_MessageDelay = 0.25f;
        PushState(LS_STATE_MESSAGE_DELAY_POP);
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_WAIT_CHECK:
        // This will wait for the memory card data to be loaded & verified
        PopState();
        // If we do this twice, then it should definitely load the data from
        // the specified card
        GetCardCount();
        if (m_CardCount==0)
        {

			ErrorReset(LS_ERR_NO_CARD);
        }
        else
        {
            LoadCardData();
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_START_SAVE:
        xbool exists;

		if (CheckCardForErrors())
			return;
        m_MessageDelay = 2.0f;
        if (m_Mode == SAVE_WARRIOR)
        {
            exists = FALSE;
            for (i=0;i<m_SavedData.WarriorCount;i++)
            {
                if( x_wstrncmp( m_SavedData.Warrior[i].Name, m_pWarrior->Name, 31 ) ==0 )
                {
                    exists = TRUE;
                    break;
                }
            }

            if (exists)
            {
                OptionBox("WARNING!",xfs("\"%s\"\n"
                                         "already exists on the memory card\n"
                                         "(8MB)(for PlayStation""\x10""2)\n"
                                         "in MEMORY CARD slot %d.\n"
                                         "\n"
										 "Do you wish to overwrite \n"
										 "the saved warrior?",(const char*)xstring(m_pWarrior->Name),m_CurrentCard+1));
                SetState(LS_STATE_CONFIRM_OVERWRITE);
            }
            else
            {
                if (m_SavedData.WarriorCount >= MAX_SAVED_WARRIORS-1)
                {
					ErrorReset(LS_ERR_TOO_MANY_WARRIORS);
                }
                else
                {
                    WarningBox("PLEASE WAIT...",xfs("Saving Warrior.\n\n"
#if defined(PAL_RELEASE)
                                                    "Do not remove the memory card\n"
                                                    "(8MB)(for PlayStation""\x10""2), reset\n"
													"or switch off the console.\0"
                                                    "                   \n"
                                                    "                   \n"
                                                    "                  \n"
                                                    "                  \n",m_CurrentCard+1),FALSE);
#else
                                                    "Please do not switch off or reset\n"
                                                    "your PlayStation""\x10""2 as this may\n"
                                                    "cause the saved data on the memory\n"
                                                    "card (8MB)(for PlayStation""\x10""2) in\n"
													"MEMORY CARD slot %d to be damaged.",m_CurrentCard+1),FALSE);
#endif
                    SetState(LS_STATE_WAIT_SAVE);
                    
                }
            }
        }
        else if (m_Mode == SAVE_FILTER)
        {
			if (m_SavedData.FilterCount >= MAX_SAVED_FILTERS)
			{
				ErrorReset(LS_ERR_TOO_MANY_FILTERS);
				break;
			}
			else
			{
				WarningBox("PLEASE WAIT...",xfs("Saving Filter.\n\n"
#if defined(PAL_RELEASE)
                                                    "Do not remove the memory card\n"
                                                    "(8MB)(for PlayStation""\x10""2), reset\n"
													"or switch off the console.\0"
                                                    "                   \n"
                                                    "                   \n"
                                                    "                  \n"
                                                    "                  \n",m_CurrentCard+1),FALSE);
#else
                                                    "Please do not switch off or reset\n"
                                                    "your PlayStation""\x10""2 as this may\n"
                                                    "cause the saved data on the memory\n"
                                                    "card (8MB)(for PlayStation""\x10""2) in\n"
													"MEMORY CARD slot %d to be damaged.",m_CurrentCard+1),FALSE);
#endif
				SetState(LS_STATE_WAIT_SAVE);
			}
        }
        else if (m_Mode == SAVE_CAMPAIGN_OPTIONS)
		{
            WarningBox("PLEASE WAIT...",    xfs("Saving Settings.\n\n"
#if defined(PAL_RELEASE)
                                                    "Do not remove the memory card\n"
                                                    "(8MB)(for PlayStation""\x10""2), reset\n"
													"or switch off the console.\0"
                                                    "                   \n"
                                                    "                   \n"
                                                    "                  \n"
                                                    "                  \n",m_CurrentCard+1),FALSE);
#else
                                                    "Please do not switch off or reset\n"
                                                    "your PlayStation""\x10""2 as this may\n"
                                                    "cause the saved data on the memory\n"
                                                    "card (8MB)(for PlayStation""\x10""2) in\n"
													"MEMORY CARD slot %d to be damaged.",m_CurrentCard+1),FALSE);
#endif
            SetState(LS_STATE_WAIT_SAVE);
		}
        else if (m_Mode == SAVE_CAMPAIGN_ENDOFGAME)
		{
            WarningBox("PLEASE WAIT...",    xfs("Saving Campaign.\n\n"
#if defined(PAL_RELEASE)
                                                    "Do not remove the memory card\n"
                                                    "(8MB)(for PlayStation""\x10""2), reset\n"
													"or switch off the console.\0"
                                                    "                   \n"
                                                    "                   \n"
                                                    "                  \n"
                                                    "                  \n",m_CurrentCard+1),FALSE);
#else
                                                    "Please do not switch off or reset\n"
                                                    "your PlayStation""\x10""2 as this may\n"
                                                    "cause the saved data on the memory\n"
                                                    "card (8MB)(for PlayStation""\x10""2) in\n"
													"MEMORY CARD slot %d to be damaged.",m_CurrentCard+1),FALSE);
#endif
            SetState(LS_STATE_WAIT_SAVE);
		}
		else
        {
            WarningBox("PLEASE WAIT...",    xfs("Saving Options.\n\n"
#if defined(PAL_RELEASE)
                                                    "Do not remove the memory card\n"
                                                    "(8MB)(for PlayStation""\x10""2), reset\n"
													"or switch off the console.\0"
                                                    "                   \n"
                                                    "                   \n"
                                                    "                  \n"
                                                    "                  \n",m_CurrentCard+1),FALSE);
#else
                                                    "Please do not switch off or reset\n"
                                                    "your PlayStation""\x10""2 as this may\n"
                                                    "cause the saved data on the memory\n"
                                                    "card (8MB)(for PlayStation""\x10""2) in\n"
													"MEMORY CARD slot %d to be damaged.",m_CurrentCard+1),FALSE);
#endif
            SetState(LS_STATE_WAIT_SAVE);
            
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_CONFIRM_OVERWRITE:
        switch (m_MessageResult)
        {
        case DLG_MESSAGE_IDLE:
            break;
        case DLG_MESSAGE_YES:
            WarningBox("PLEASE WAIT...",xfs("Saving data.\n\n"
#if defined(PAL_RELEASE)
                                                    "Do not remove the memory card\n"
                                                    "(8MB)(for PlayStation""\x10""2), reset\n"
													"or switch off the console.\0"
                                                    "                  \n"
                                                    "                  \n"
                                                    "                  \n"
                                                    "                  \n",m_CurrentCard+1),FALSE);
#else
                                                    "Please do not switch off or reset\n"
                                                    "your PlayStation""\x10""2 as this may\n"
                                                    "cause the saved data on the memory\n"
                                                    "card (8MB)(for PlayStation""\x10""2) in\n"
													"MEMORY CARD slot %d to be damaged.",m_CurrentCard+1),FALSE);
#endif
            SetState(LS_STATE_WAIT_SAVE);
            break;
        default:
            SetState(LS_STATE_IDLE);
            m_Exit = TRUE;
            break;
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_WAIT_SAVE:
        m_MessageDelay -= DeltaTime;
        if (m_MessageDelay < 0.0f)
        {
            SetState(LS_STATE_SAVE);
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_WAIT_DELETE:
        m_MessageDelay -= DeltaTime;
        if (m_MessageDelay < 0.0f)
        {
            SetState(LS_STATE_DELETE);
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_SAVE:
        card_file *pFile;

		switch (m_Mode)
		{
		case SAVE_WARRIOR:
            for (i=0;i<m_SavedData.WarriorCount;i++)
            {
                if (x_wstrncmp(m_SavedData.Warrior[i].Name,m_pWarrior->Name,31)==0)
                {
                    break;
                }
            }

            m_SavedData.Warrior[i] = *m_pWarrior;
            m_SavedData.LastWarriorIndex[m_PlayerIndex] = i;
            m_SavedData.LastWarriorValid |= (1<<m_PlayerIndex);
            // 
            // If we just appended, let's increase the count stored on the card
            if (i==m_SavedData.WarriorCount)
            {
                m_SavedData.WarriorCount++;
            }
			break;
/*
		case SAVE_FILTER:
			m_SavedData.Filters[m_SavedData.FilterCount]=*m_pFilter;
			m_SavedData.FilterCount++;
			break;
*/
        case SAVE_NETWORK_OPTIONS:
		    title_GetOptions(&m_SavedData.Options,NETWORK_OPTIONS);
			break;
		case SAVE_AUDIO_OPTIONS:
		    title_GetOptions(&m_SavedData.Options,AUDIO_OPTIONS);
			break;
		case SAVE_CAMPAIGN_OPTIONS:
		case SAVE_CAMPAIGN_ENDOFGAME:
			title_GetOptions(&m_SavedData.Options,CAMPAIGN_OPTIONS);
			break;
		default:
			ASSERT(FALSE);
		}

        PrepareSavedData();

        x_sprintf(name,"card0%d:/%s/%s",m_CurrentCard,SAVE_DIRECTORY,m_Filename);
		// Make sure the old file is removed just in case we had
		// a bad partial write on the last write
		card_Delete(name);
        pFile = card_Open(name,"w");

        if ((s32)pFile>0)
        {
			s32 length;

            length = card_Write(pFile,&m_SavedData,sizeof(saved_game));
            card_Close(pFile);
            m_ForceReload = TRUE;
            SetState(LS_STATE_IDLE);
			fegl.WarriorModified = FALSE;
			fegl.OptionsModified = FALSE;
			fegl.IngameModified  = FALSE;
			fegl.FilterModified  = FALSE;
			if (length != sizeof(saved_game))
			{
				pFile = (card_file*)-1;
			}
			else
            {
                m_Exit = TRUE;
                return;
            }
        }
        else
        {
            if ((s32)pFile == CARD_ERR_UNFORMATTED)
            {
                audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
                OptionBox("ERROR!",xfs("The memory card (8MB)(for\n"
										"PlayStation""\x10""2) in MEMORY\n"
										"CARD slot %d does not appear\n"
										"to be formatted.\n"
										"Do you wish to format it now?",m_CurrentCard+1)); 
				m_MessageDelay = 1000000.0f;
                SetState(LS_STATE_ASK_FORMAT);
                break;
            }

            title_GetOptions(&m_SavedData.Options,ALL_OPTIONS);
			// File may not exist so let's try to create the directory and associated files
            if ((s32)pFile == CARD_ERR_NOTFOUND)
            {
                if (m_CardInfo[m_CurrentCard].Free * 1024 < SAVE_FILE_SIZE)
                {
					ErrorReset(LS_ERR_FULL);
					break;
                }
				else
				{
					x_sprintf(name,"card0%d:/%s",m_CurrentCard,SAVE_DIRECTORY);
					card_MkDir(name);
					card_ChDir(name);
					card_MkIcon("Tribes Aerial Assault");
					x_sprintf(name,"card0%d:%s",m_CurrentCard,m_Filename);
					pFile = card_Open(name,"wb");
					if ((s32)pFile > 0)
					{
						card_Write(pFile,&m_SavedData,sizeof(saved_game));
						card_Close(pFile);
						fegl.WarriorModified = FALSE;
						fegl.OptionsModified = FALSE;
						fegl.IngameModified  = FALSE;
						fegl.FilterModified  = FALSE;
						m_ForceReload = TRUE;
						SetState(LS_STATE_IDLE);
						m_Exit = TRUE;

					}
				}

            }

        }
        if ((s32)pFile < 0)
        {
			// If we've tried to save and it failed, let's retry but only with campaigns
			ErrorReset(LS_ERR_SAVE_FAILED);
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_MESSAGE_DELAY:
        m_MessageDelay -= DeltaTime;
        if ( (m_MessageDelay < 0.0f) || (m_MessageResult != DLG_MESSAGE_IDLE) )
        {
            SetState(LS_STATE_IDLE);
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_MESSAGE_DELAY_EXIT:
        m_MessageDelay -= DeltaTime;
        if ( (m_MessageDelay < 0.0f) || (m_MessageResult != DLG_MESSAGE_IDLE) )
        {
            m_Exit = TRUE;
            SetState(LS_STATE_IDLE);
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_MESSAGE_DELAY_POP:
        m_MessageDelay -= DeltaTime;
        if ( (m_MessageDelay < 0.0f) || (m_MessageResult != DLG_MESSAGE_IDLE) )
        {
            PopState();
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_ASK_FORMAT:
        switch (m_MessageResult)
        {
        case DLG_MESSAGE_YES:
            SetState(LS_STATE_FORMAT_DELAY);
            WarningBox("PLEASE WAIT...",xfs("Formatting the memory card (8MB)\n"
                                            "(for PlayStation""\x10""2) in MEMORY\n"
											"CARD slot %d. Do not remove the memory\n"
											"card (8MB)(For PlayStation""\x10""2), reset\n"
											"or switch off the console.\n"
											"This will take a few moments.",m_CurrentCard+1),FALSE);
            m_MessageDelay = 1.0f;
            break;
        case DLG_MESSAGE_IDLE:
            break;
        default:
			ErrorReset(LS_ERR_SAVE_NOT_FORMATTED);
            break;
        }
        break;

    //-------------------------------------------------------------------------
	case LS_STATE_ASK_SAVE:
		switch (m_Mode)
		{
		case SAVE_WARRIOR:
			OptionBox("ALERT!", xfs("You have modified warrior\n"
                                    "\"%s\"\n"
                                    "\n"
									"Do you wish to save your changes\n"
									"to a memory card (8MB)(for\n"
									"PlayStation""\x10""2)?", (const char*)xstring(m_pWarrior->Name)));
			break;
		case SAVE_CAMPAIGN_OPTIONS:
			OptionBox("ALERT!", xfs("You have modified your settings!\n"
									"Do you wish to save your settings\n"
									"to a memory card (8MB)(for\n"
									"PlayStation""\x10""2)?"));
			break;
		case SAVE_CAMPAIGN_ENDOFGAME:
			OptionBox("ALERT!", xfs("Level Complete!\n"
									"Do you wish to save your status\n"
									"to a memory card (8MB)(for\n"
									"PlayStation""\x10""2)?"));
			break;
		case SAVE_NETWORK_OPTIONS:
		case SAVE_AUDIO_OPTIONS:
			OptionBox("ALERT!", xfs("You have modified your options.\n"
									"Do you wish to save this to\n"
									"a memory card (8MB)(for\n"
									"PlayStation""\x10""2)?"));
			break;
		case SAVE_FILTER:
			OptionBox("ALERT!", xfs("You have modified your filter.\n"
									"Do you wish to save this to\n"
									"a memory card (8MB)(for\n"
									"PlayStation""\x10""2)?"));
			break;
		default:
			ASSERT(FALSE);
		}

		SetState(LS_STATE_WAIT_ASK_SAVE);
		break;

	//-------------------------------------------------------------------------
	case LS_STATE_WAIT_ASK_SAVE:
		switch (m_MessageResult)
		{
		case DLG_MESSAGE_YES:
			PopState();
			break;
		case DLG_MESSAGE_IDLE:
			break;
		default:
			m_Exit = TRUE;
			SetState(LS_STATE_IDLE);
		}
		break;
    //-------------------------------------------------------------------------
    case LS_STATE_SELECT_CARD:
        xbool Selected;

        Selected = (m_CurrentCard == 1);
		OptionBox("ALERT!", xfs("There is more than one memory\n"
                                "card (8MB)(for PlayStation""\x10""2)\n"
								"present. Which MEMORY CARD\n"
								"slot would you like to use?\n"
								"Press the ""\x80"" button to cancel."
                                ),xwstring("SLOT 1"),xwstring("SLOT 2"),Selected,TRUE);
        SetState(LS_STATE_WAIT_CARD);
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_WAIT_CARD:
        switch (m_MessageResult)
        {
        case DLG_MESSAGE_IDLE:
            break;
        case DLG_MESSAGE_YES:
            m_SelectedCard = 0;
            m_CurrentCard = 0;
            PopState();
            LoadCardData();
			CheckCardForErrors();
            break;
        case DLG_MESSAGE_NO:
            m_SelectedCard = 1;
            m_CurrentCard = 1;
            PopState();
            LoadCardData();
			CheckCardForErrors();
            break;
        default:
            m_Exit = TRUE;
            SetState(LS_STATE_IDLE);
            break;
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_FORMAT_DELAY:
        m_MessageDelay -= DeltaTime;
        if (m_MessageDelay <0.0f)
        {
            m_LastCardStatus = card_Format(xfs("card0%d:",m_CurrentCard));
			card_Check(xfs("card0%d:",m_CurrentCard),&m_CardInfo[m_CurrentCard]);
            if (m_LastCardStatus==CARD_ERR_OK)
            {
                SetState(LS_STATE_START_SAVE);
                break;
            }
            else
            {
				ErrorReset(LS_ERR_UNABLE_TO_FORMAT);
            }
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_START_DELETE:
        m_MessageDelay = 2.0f;
		if (CheckCardForErrors())
			break;
        if (m_LastCardStatus != CARD_ERR_OK)
        {
			ErrorReset(LS_ERR_NO_CARD);
        }
        else
        {
			if (m_Mode == LOAD_WARRIOR)
			{
                s32 index;

                index = m_pDirectory->GetSelection();
				OptionBox("WARNING!",xfs("Are you sure you wish to\n"
									 "delete warrior\n"
                                     "'%s'\n"
									 "saved on the memory card (8MB)(for\n"
									 "PlayStation""\x10""2) in MEMORY\n"
									 "CARD slot %d?",(const char*)xstring(m_SavedData.Warrior[index].Name),m_CurrentCard+1));
			}
			else
			{
				OptionBox("WARNING!","Are you sure you wish to\n"
                                     "delete this filter?");
			}
            SetState(LS_STATE_CONFIRM_DELETE);
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_CONFIRM_DELETE:
        switch (m_MessageResult)
        {
        case DLG_MESSAGE_IDLE:
            break;
        case DLG_MESSAGE_YES:
            s32 i;
            s32 index;

            index = m_pDirectory->GetSelection();

			if (m_Mode == LOAD_WARRIOR)
			{
				for (i=index;i<m_SavedData.WarriorCount;i++)
				{
					m_SavedData.Warrior[i]=m_SavedData.Warrior[i+1];
				}
				m_SavedData.WarriorCount--;
                for (i=0;i<2;i++)
                {
                    // When we delete a warrior, we need to check to see whether or not
                    // the indices used for the last saved warriors are still valid. If they
                    // are greater than the deleted warrior index, then we need to decrement it.
                    // If they are the same, we invalidate it (since we just deleted the warrior
                    // associated with it).
                    if ( m_SavedData.LastWarriorValid & (1<<i) )
                    {
                        if ( m_SavedData.LastWarriorIndex[i] > index )
                        {
                            m_SavedData.LastWarriorIndex[i]--;
                        }

                        if ( m_SavedData.LastWarriorIndex[i] == index )
                        {
                            m_SavedData.LastWarriorValid &= ~(1<<i);
                        }
                    }
                }
			}
/*
			else
			{
				ASSERT(m_Mode == LOAD_FILTER);
				for (i=index;i<m_SavedData.FilterCount;i++)
				{
					m_SavedData.Filters[i]=m_SavedData.Filters[i+1];
				}
				m_SavedData.FilterCount--;
			}
*/
            WarningBox("PLEASE WAIT...",xfs("Deleting data.\n\n"
#if defined(PAL_RELEASE)
                                                    "Do not remove the memory card\n"
                                                    "(8MB)(for PlayStation""\x10""2), reset\n"
													"or switch off the console.\0"
                                                    "                   \n"
                                                    "                   \n"
                                                    "                  \n"
                                                    "                  \n",m_CurrentCard+1),FALSE);
#else
                                                    "Please do not switch off or reset\n"
                                                    "your PlayStation""\x10""2 as this may\n"
                                                    "cause the saved data on the memory\n"
                                                    "card (8MB)(for PlayStation""\x10""2) in\n"
													"MEMORY CARD slot %d to be damaged.",m_CurrentCard+1),FALSE);
#endif
            SetState(LS_STATE_WAIT_DELETE);
            break;
        default:
            SetState(LS_STATE_IDLE);
            break;
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_DELETE:

        x_sprintf(name,"card0%d:/%s/%s",m_CurrentCard,SAVE_DIRECTORY,m_Filename);
        pFile = card_Open(name,"w");

        PrepareSavedData();

        if ((s32)pFile>0)
        {
            card_Write(pFile,&m_SavedData,sizeof(saved_game));
            card_Close(pFile);
			fegl.WarriorModified = FALSE;
			fegl.OptionsModified = FALSE;
			fegl.IngameModified  = FALSE;
			fegl.FilterModified  = FALSE;
            m_ForceReload = TRUE;
            SetState(LS_STATE_IDLE);

        }
        else
        {
            if ((s32)pFile < 0)
            {
                m_MessageDelay = 1000.0f;
                SetState(LS_STATE_MESSAGE_DELAY);
                audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
                WarningBox("ERROR!","Delete warrior failed.");
            }
        }
        break;
    //-------------------------------------------------------------------------
    case LS_STATE_ERR_CORRUPT:
        m_MessageDelay = 7.0f;
        SetState(LS_STATE_MESSAGE_CORRUPT_WAIT);
        audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
        OptionBox("WARNING!",xfs("The saved game stored on the memory\n"
								  "card (8MB)(for PlayStation""\x10""2) in\n"
								  "MEMORY CARD slot %d appears to be\n"
								  "damaged. This message will no longer\n"
								  "appear once you have saved a game.",m_CurrentCard+1),
								  xwstring("CANCEL"),xwstring("OK"));
        x_memset((u8 *)&m_SavedData,0,sizeof(m_SavedData));
		title_GetOptions(&m_SavedData.Options,ALL_OPTIONS);
        break;
    //-------------------------------------------------------------------------
	case LS_STATE_MESSAGE_CORRUPT_WAIT:
		switch (m_MessageResult)
		{
		case DLG_MESSAGE_NO:
			PopState();
			break;
		case DLG_MESSAGE_IDLE:
			break;
		default:
			// If we were waiting on confirm/cancel of the CORRUPT message, then
			// we just exit if they cancel otherwise, restart the save process.
			if (m_Mode == SAVE_CAMPAIGN_ENDOFGAME)
			{
				m_StateStackTop = 0;
				m_LastCardCount = 0;
				m_SelectedCard = -1;
				SetState(LS_STATE_IDLE);
				PushState(LS_STATE_START_SAVE);
				PushState(LS_STATE_CHECKING_CARD);
				PushState(LS_STATE_ASK_SAVE);
				m_ForceReload=TRUE;
		        PushState(LS_STATE_MESSAGE_DELAY_POP);
			}
			else
			{
				m_Exit = TRUE;
				SetState(LS_STATE_IDLE);
			}
		}
		break;
    //-------------------------------------------------------------------------
    case LS_STATE_ERR_NOTFORMATTED:
        audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
        OptionBox("ERROR!",xfs("The memory card (8MB)(for\n"
							   "PlayStation""\x10""2) in MEMORY\n"
							   "CARD slot %d has not been\n"
							   "formatted. Do you wish\n"
                               "to format it now?",m_CurrentCard+1)); 
        SetState(LS_STATE_ASK_FORMAT);
        break;

    //-------------------------------------------------------------------------
    case LS_STATE_LOAD:

        s32 index;
		if (CheckCardForErrors())
			break;

        switch (m_Mode)
        {
        case LOAD_WARRIOR:
            index = m_pDirectory->GetSelection();
            if (index >= m_SavedData.WarriorCount)
            {
				ErrorReset(LS_ERR_NO_SAVED_WARRIORS);
                break;
            }
            else
            {
                *m_pWarrior = m_SavedData.Warrior[index];
				UseAutoAimHint		= m_SavedData.Options.UseAutoAimHint;
				UseAutoAimHelp		= m_SavedData.Options.UseAutoAimHelp;

				fegl.WarriorModified = FALSE;
//                title_SetOptions(&m_SavedData.Options);
                m_Exit = TRUE;
				m_pParent->OnNotify(m_pParent,this,WN_USER,NULL);
            }
            break;
        case LOAD_NETWORK_OPTIONS:
		case LOAD_AUDIO_OPTIONS:
		case LOAD_CAMPAIGN_OPTIONS:

            if (!ValidateSavedData())
            {
				ErrorReset(LS_ERR_NO_OPTIONS);
                break;
            }
            else
            {
				switch (m_Mode)
				{
				case LOAD_NETWORK_OPTIONS:
	                title_SetOptions(&m_SavedData.Options,NETWORK_OPTIONS);
					break;
				case LOAD_AUDIO_OPTIONS:
	                title_SetOptions(&m_SavedData.Options,AUDIO_OPTIONS);
					break;
				case LOAD_CAMPAIGN_OPTIONS:
	                title_SetOptions(&m_SavedData.Options,CAMPAIGN_OPTIONS);
					break;
				default:
					ASSERT(FALSE);
				}
				m_pParent->OnNotify(m_pParent,this,WN_USER,NULL);
                m_Exit = TRUE;
				fegl.OptionsModified = FALSE;
            }
            break;
/*
        case LOAD_FILTER:
			index = m_pDirectory->GetSelection();
			if (index >= m_SavedData.FilterCount)
			{
                audio_Play(SFX_FRONTEND_ERROR);
				WarningBox("ERROR!",xfs("No saved filter on the memory\n"
										"card (8MB)(for PlayStation""\x10""2)\n"
										"in MEMORY CARD slot %d.",m_CurrentCard+1));
				m_MessageDelay = 1000.0f;
				SetState(LS_STATE_MESSAGE_DELAY);
				
				break;
			}
			else
			{
				ASSERT(m_pFilter);
				*m_pFilter = m_SavedData.Filters[index];
				m_pParent->OnNotify(m_pParent,this,WN_USER,NULL);
                m_Exit = TRUE;
				//title_SetOptions(&m_SavedData.Options);
				fegl.FilterModified = FALSE;
			}
            break;
*/
        }
        break;

    //-------------------------------------------------------------------------
    default:
        ASSERT(FALSE);
    }

}

//=========================================================================
s32 dlg_load_save::GetCardCount(void)
{
    s32         status[2];
    s32         lastcard;
    
    lastcard = m_CurrentCard;
    m_CardCount = 0;

	x_memset(m_CardInfo,0,sizeof(m_CardInfo));

    status[0] = card_Check("card00:",&m_CardInfo[0]);
    if ( status[0] != CARD_ERR_UNPLUGGED)
    {
        m_CurrentCard = 0;
        m_CardCount++;
    }

    status[1] = card_Check("card01:",&m_CardInfo[1]);
    if ( status[1] != CARD_ERR_UNPLUGGED)
    {
        m_CurrentCard = 1;
        m_CardCount++;
    }

    if (m_CardCount > 1)
    {
        // Need to figure out which controller has focus at
        // this point to pick the right card!
//        ui_manager::user* pUser = m_pManager->GetUser(m_UserID);
        if (m_SelectedCard == -1)
        {
            m_CurrentCard = m_PlayerIndex;
            PushState(LS_STATE_SELECT_CARD);
        }
        else
        {
            m_CurrentCard = m_SelectedCard;
        }
    }

    if (m_CardCount != m_LastCardCount)
    {
        m_ForceReload=TRUE;
        m_LastCardCount = m_CardCount;
    }

    m_pSlot->DeleteAllItems();
    m_pSlot->AddItem(xwstring(xfs("SLOT %d",m_CurrentCard+1)));
    m_pSlot->SetSelection(0);

    return m_CardCount;
}

void dlg_load_save::LoadCardData(void)
{
    card_info   Info[2];
    char        name[128];
    s32         status[2];

    m_ForceReload = FALSE;
	m_DataError	  = 0;
    status[0] = card_Check("card00:",&Info[0]);
    status[1] = card_Check("card01:",&Info[1]);

    m_LastCardStatus = status[m_CurrentCard];
    if (status[m_CurrentCard] == 0)
    {
        card_file *pFile;

        x_sprintf(name,"card0%d:/%s/%s",m_CurrentCard,SAVE_DIRECTORY,m_Filename);
        pFile = card_Open(name,"r");
        m_pDirectory->DeleteAllItems();
		if ((s32)pFile == CARD_ERR_UNFORMATTED)
		{
			m_LastCardStatus = CARD_ERR_UNFORMATTED;
		}

		if ((s32)pFile == CARD_ERR_NOTFOUND)
		{
			m_DataError = 2;
		}

        if ((s32)pFile<=0)
        {
            switch (m_Mode)
            {
            case LOAD_WARRIOR:
            case SAVE_WARRIOR:
                m_pDirectory->AddItem( "No saved warriors." );
                break;
            case LOAD_NETWORK_OPTIONS:
            case SAVE_NETWORK_OPTIONS:
            case LOAD_AUDIO_OPTIONS:
            case SAVE_AUDIO_OPTIONS:
            case LOAD_CAMPAIGN_OPTIONS:
            case SAVE_CAMPAIGN_OPTIONS:
			case SAVE_CAMPAIGN_ENDOFGAME:
                m_pDirectory->AddItem( "No saved options." );
                break;
            case LOAD_FILTER:
            case SAVE_FILTER:
                m_pDirectory->AddItem( "No saved filters." );
                break;
            default:
                ASSERT(FALSE);
            }
            m_pDirectory->SetSelection(0);

			if (!m_IsSaving)
            {
                if (m_CardCount == 0)
                {
                    ErrorReset(LS_ERR_NO_CARD);
                }
            }
            x_memset((u8 *)&m_SavedData,0,sizeof(m_SavedData));
			title_GetOptions(&m_SavedData.Options,ALL_OPTIONS);

        }
        else
        {
            s32     i;
            card_Read(pFile,&m_SavedData,sizeof(saved_game));
            card_Close(pFile);
            
            tgl.JumpTime = TRUE;

            if (!ValidateSavedData())
			{
				m_DataError = 1;
			}
            if ( m_SavedData.WarriorCount < 0 )
                m_DataError = 1;

            if (m_SavedData.WarriorCount > MAX_SAVED_WARRIORS)
                m_SavedData.WarriorCount = MAX_SAVED_WARRIORS;

			if (m_DataError)
			{
		        x_memset(&m_SavedData,0,sizeof(saved_game));
				title_GetOptions(&m_SavedData.Options,ALL_OPTIONS);
			}

            switch (m_Mode)
            {
            case LOAD_WARRIOR:
            case SAVE_WARRIOR:
                for (i=0;i<m_SavedData.WarriorCount;i++)
                {
                    m_pDirectory->AddItem( m_SavedData.Warrior[i].Name );
                }

                if (i==0)
                {
                    m_pDirectory->AddItem( STR_NO_SAVED_WARRIORS );
                }
                break;
            case LOAD_NETWORK_OPTIONS:
            case SAVE_NETWORK_OPTIONS:
            case LOAD_AUDIO_OPTIONS:
            case SAVE_AUDIO_OPTIONS:
            case LOAD_CAMPAIGN_OPTIONS:
            case SAVE_CAMPAIGN_OPTIONS:
			case SAVE_CAMPAIGN_ENDOFGAME:
                m_pDirectory->AddItem( "Options present." );
                break;

            case SAVE_FILTER:
            case LOAD_FILTER:
                for (i=0;i<m_SavedData.FilterCount;i++)
                {
                    m_pDirectory->AddItem( xwstring(xfs("Filter %d",i+1)) );
                }
                if (i==0)
                {
                    m_pDirectory->AddItem( xwstring("No Saved Filters") );
                }
                break;
            }
            m_pDirectory->SetSelection( 0 );
        }
    }
    else
    {
        m_pDirectory->DeleteAllItems();
        if (status[m_CurrentCard] == CARD_ERR_CHANGED)
        {
            m_pDirectory->AddItem( "Checking memory card (PS2)" );
            m_DataError = 0;
        }
        else
        {
            m_pDirectory->AddItem( STR_NO_CARD_PRESENT );
            m_DataError = 2;
        }
        m_pDirectory->SetSelection(0);
        x_memset(&m_SavedData,0,sizeof(saved_game));
		title_GetOptions(&m_SavedData.Options,ALL_OPTIONS);

    }
}

load_save_params::load_save_params(s32 PlayerIndex,load_save_mode Mode,ui_dialog *pDialog,void *pData)
{
    m_pDialog = pDialog;
    m_Mode    = Mode;
    m_pData   = pData;
    m_PlayerIndex = PlayerIndex;
}


xbool dlg_load_save::ValidateSavedData(void)
{
    xbool   valid;
    s32     Checksum;

    valid = TRUE;

    Checksum = m_SavedData.Checksum;

    m_SavedData.Checksum = 0;

    if (Checksum != card_Checksum((u8 *)&m_SavedData,sizeof(m_SavedData),SAVED_CHECKSUM_SEED))
    {
        valid = FALSE;
    }
    m_SavedData.Checksum = Checksum;

    Checksum = m_SavedData.Options.Checksum;
    m_SavedData.Options.Checksum = 0;
    if (Checksum != card_Checksum((u8 *)&m_SavedData.Options,sizeof(m_SavedData.Options),SAVED_CHECKSUM_SEED))
    {
        valid = FALSE;
    }
    m_SavedData.Options.Checksum = Checksum;
    return valid;    
}

void dlg_load_save::PrepareSavedData(void)
{

    // We need to zero out the old checksum fields as the values in there will interfere with the full
    // checksum calculation which includes the sum in the data block.
    // We need to checksum the options first since this will also affect the overall checksum
    m_SavedData.Options.Checksum = 0;
    m_SavedData.Options.Checksum = card_Checksum((u8 *)&m_SavedData.Options,sizeof(m_SavedData.Options),SAVED_CHECKSUM_SEED);

    m_SavedData.Checksum = 0;
    m_SavedData.Checksum = card_Checksum((u8 *)&m_SavedData,sizeof(m_SavedData),SAVED_CHECKSUM_SEED);
}

//=========================================================================
void dlg_load_save::ErrorReset(ls_errorcode Error)
{
	xstring ErrorText;

	// If we've tried to save and it failed, let's retry but only with campaigns
	audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
	switch(Error)
	{
	case LS_ERR_SAVE_FAILED:
		ErrorText.Format(	"Save data failed.");
		break;
	case LS_ERR_NO_CARD:
		ErrorText.Format(	"There is no memory card (8MB)\n"
							"(for PlayStation""\x10""2) present. \n"
                            "Please insert a memory card\n"
							"(8MB)(for PlayStation""\x10""2)\n"
							"in to any MEMORY CARD slot.\n");
		break;
	case LS_ERR_LOAD_NOT_FORMATTED:
		ErrorText.Format(	"The memory card (8MB)(for\n"
							"PlayStation""\x10""2) in MEMORY CARD\n"
							"slot %d is not formatted. No data\n"
							"can be loaded from the memory\n"
							"card (8MB)(for PlayStation""\x10""2)\n"
							"in slot %d.\n",m_CurrentCard+1,m_CurrentCard+1);
		break;
	case LS_ERR_SAVE_NOT_FORMATTED:
		ErrorText.Format(	"Cannot save unless the memory\n"
							"card (8MB)(for PlayStation""\x10""2) in\n"
							"MEMORY CARD slot %d is formatted.\n"
							"Please format the memory card\n"
							"(8MB)(for PlayStation""\x10""2) before\n"
							"attempting to save again.\n",m_CurrentCard+1);
		break;
	case LS_ERR_UNABLE_TO_FORMAT:
		ErrorText.Format(	"Unable to format the memory card\n"
							"(8MB)(for PlayStation""\x10""2) in\n"
							"MEMORY CARD slot %d, please try\n"
							"another memory card (8MB)(for\n"
							"PlayStation""\x10""2).",m_CurrentCard+1);
		break;
	case LS_ERR_WRONG_TYPE:
#if defined(PAL_RELEASE)
        ErrorText.Format(   "No memory card (8MB)(for\n"
                            "PlayStation""\x10""2) inserted in\n"
                            "MEMORY CARD slot %d.\n"
                            "Please insert a memory\n"
                            "card(8MB)(for PlayStation""\x10""2).\0"
                            "                         "
                            "                         ",m_CurrentCard+1);
#else
        ErrorText.Format(	"The MEMORY CARD in slot %d is\n"
                            "not a memory card (8MB)(for\n"
							"PlayStation""\x10""2). This memory card\n"
							"cannot be used with this game.\n"
							"Please insert a memory card (8MB)\n"
							"(for PlayStation""\x10""2).",m_CurrentCard+1);
#endif
		break;
	case LS_ERR_FULL:
		ErrorText.Format(	"There is not enough free space\n"
							"on the memory card (8MB)(for\n"
							"PlayStation""\x10""2) in MEMORY CARD\n"
							"slot %d. %dKB is required. You will\n"
							"not be able to save until you remove\n"
                            "some items from this memory\n"
                            "card (8MB)(for PlayStation""\x10""2).",m_CurrentCard+1,SAVE_FILE_SIZE/1024);
		break;
	case LS_ERR_NO_OPTIONS:
		ErrorText.Format(	"No options have been saved.");
		break;
	case LS_ERR_NO_SAVED_WARRIORS:
        ErrorText.Format(	"There are no warriors saved\n"
                            "on the memory card (8MB)(for\n"
							"PlayStation""\x10""2) in MEMORY\n"
							"CARD slot %d.\n"
							"Please insert a memory card\n"
							"(8MB)(for PlayStation""\x10""2) with\n"
							"a saved warrior and try again.\n",m_CurrentCard+1);
		break;
	case LS_ERR_NO_SAVED_DATA:
        ErrorText.Format(	"There is no saved game data\n"
                            "on the memory card (8MB)(for\n"
							"PlayStation""\x10""2) in MEMORY CARD\n"
							"slot %d. Please insert a memory card\n"
							"(8MB)(for PlayStation""\x10""2) with a\n"
							" saved game before trying to load.",m_CurrentCard+1);
		break;
	case LS_ERR_TOO_MANY_WARRIORS:
		ErrorText.Format(	"Too many warriors are saved on\n"
							"the memory card (8MB)(for\n"
							"PlayStation""\x10""2) in MEMORY\n"
							"CARD slot %d. Please delete\n"
							"a warrior before trying to\n"
							"save again.",m_CurrentCard+1);
		break;
	case LS_ERR_CARD_REMOVED:
		ErrorText.Format(	"The memory card (8MB)(for\n"
							"PlayStation""\x10""2) in MEMORY\n"
							"CARD slot %d was removed.\n",m_CurrentCard+1);
		break;
	case LS_ERR_TOO_MANY_FILTERS:
		ErrorText.Format(	"Too many filters saved. Please\n"
							"delete a filter before\n"
                            "saving again.");
		break;
	case LS_ERR_CORRUPT:
        ErrorText.Format(	"The saved game stored on the memory\n"
							"card (8MB)(for PlayStation""\x10""2) in\n"
							"MEMORY CARD slot %d appears to be\n"
							"damaged. This message will no longer\n"
							"appear once you have saved a game.",m_CurrentCard+1);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	WarningBox("ERROR!",ErrorText);
	if (m_IsSaving)
	{
		m_StateStackTop = 0;
		m_LastCardCount = 0;
		m_SelectedCard = -1;
		SetState(LS_STATE_IDLE);
		PushState(LS_STATE_START_SAVE);
		PushState(LS_STATE_CHECKING_CARD);
		PushState(LS_STATE_ASK_SAVE);
		m_ForceReload=TRUE;
		PushState(LS_STATE_MESSAGE_DELAY_POP);
	}
	else
	{
		PushState(LS_STATE_MESSAGE_DELAY_EXIT);
	}

	m_Exit = FALSE;
    m_MessageDelay = 1000000.0f;
}

xbool dlg_load_save::CheckCardForErrors(void)
{
    if (m_CardCount == 0)
    {
        ErrorReset(LS_ERR_NO_CARD);
		return TRUE;
    }

	if (m_LastCardStatus == CARD_ERR_UNFORMATTED)
	{
		if (m_IsSaving)
		{
            audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
            OptionBox("ERROR!",xfs("The memory card (8MB)(for\n"
									"PlayStation""\x10""2) in MEMORY\n"
									"CARD slot %d does not appear\n"
									"to be formatted.\n"
									"Do you wish to format it now?",m_CurrentCard+1)); 
			PushState(LS_STATE_ASK_FORMAT);
		}
		else
		{
			ErrorReset(LS_ERR_LOAD_NOT_FORMATTED);
		}
		return TRUE;
	}

	if (m_LastCardStatus == CARD_ERR_WRONGTYPE)
	{
		ErrorReset(LS_ERR_WRONG_TYPE);
		return TRUE;
	}

    if ( m_DataError )
	{
		if (!m_IsSaving)
		{
			if (m_DataError == 2)
			{
				ErrorReset(LS_ERR_NO_SAVED_DATA);
			}
			else
			{
				ErrorReset(LS_ERR_CORRUPT);
			}
			return TRUE;
		}
	}

	return FALSE;
}