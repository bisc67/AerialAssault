//=========================================================================
//
//  dlg_options.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_button.hpp"
#include "serverman.hpp"
#include "cardmgr/cardmgr.hpp"
#include "audiomgr/audio.hpp"
#include "LabelSets\Tribes2Types.hpp"

#include "dlg_NetworkOptions.hpp"
#include "dlg_message.hpp"
#include "dlg_loadsave.hpp"
#include "masterserver/masterserver.hpp"
#include "fe_globals.hpp"
#include "globals.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

#include "StringMgr\StringMgr.hpp"
#include "Demo1/SpecialVersion.hpp"


//==============================================================================
// Online Dialog
//==============================================================================

// The navigation grid is a matrix defined in the ui_manager::dialog_tem structure. In this case,
// it's a 3x2 matrix.
// With each button, the 4 values define x,y position within that matrix and width and height
// of the box active area. In the sample below, "Cancel" occupies position (0,1) and is (1,1) wide
// "Connection Types" is at position (0,0) with a size (3,1) and so on.
#define NET_BASE_X 4
#define NET_BASE_Y 44

#define BTN_BASE_X 50
#define BTN_BASE_Y 409

enum controls
{
    IDC_CANCEL,
    IDC_OK,
//    IDC_LOAD,
//    IDC_SAVE,
//    IDC_DONE,
    IDC_CONNECTION_TYPES,
    IDC_CONNECT,
    IDC_AUTO_CONNECT,
    IDC_NETWORK_FRAME,
    IDC_NETWORK_CONFIGURATION_TEXT,
	IDC_MODIFY,
};

ui_manager::control_tem NetworkOptionsControls[] =
{
    { IDC_CONNECT,                    IDS_CONNECT,               "button",    8,  44+26*4, 140, 24, 0,  6, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CANCEL,                     IDS_CANCEL,                "button",    8,  58+24*5, 140, 24, 0,  8, 1, 1, ui_win::WF_VISIBLE },

    { IDC_CONNECTION_TYPES,           IDS_NULL,                  "listbox",   8,  52+24*0, 400, 90, 0,  5, 3, 1, ui_win::WF_VISIBLE },
    
    { IDC_NETWORK_FRAME,              IDS_NULL,                  "frame",     0,        0, 414,210, 0,  0, 0, 0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},

	{ IDC_MODIFY,					  IDS_MODIFY,				 "button",  157,  44+26*4, 100, 24, 1,  6, 1, 1, ui_win::WF_VISIBLE },

    { IDC_AUTO_CONNECT,               IDS_AUTO_CONNECT,          "check",   266,  44+26*4, 140, 24, 2,  6, 1, 1, ui_win::WF_VISIBLE },
    
    { IDC_OK,                         IDS_OK,                    "button",  266,  58+24*5, 140, 24, 2,  8, 1, 1, ui_win::WF_VISIBLE },

    // Master buttons
//    { IDC_LOAD,                       IDS_LOAD,                  "button",  BTN_BASE_X+20,     NET_BASE_Y+14+24*5,      80, 24, 0,  8, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_SAVE,                       IDS_SAVE,                  "button",  BTN_BASE_X+120, NET_BASE_Y+14+24*5,      80, 24, 1,  8, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_DONE,                       IDS_DONE,                  "button",  BTN_BASE_X+220, NET_BASE_Y+14+24*5,      80, 24, 2,  8, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem NetworkOptionsDialog =
{
    IDS_OPTIONS_TITLE,
    3, 9,
    sizeof(NetworkOptionsControls)/sizeof(ui_manager::control_tem),
    &NetworkOptionsControls[0],
    0
};

//=========================================================================
//  Defines
//=========================================================================
enum 
{
    STATE_IDLE,
    STATE_MESSAGE_DELAY,
    STATE_WAIT_CONNECT,
    STATE_WAIT_ACTIVATE,
    STATE_DELAY_ACTIVATE,
    STATE_DELAY_THEN_EXIT,
	STATE_WAIT_MODIFY_QUESTION,
	STATE_START_MODIFY,
};

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================
extern server_manager*  pSvrMgr;

//=========================================================================
//  Registration function
//=========================================================================

void dlg_network_options_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "network options", &NetworkOptionsDialog, &dlg_network_options_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_network_options_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_network_options* pDialog = new dlg_network_options;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_options
//=========================================================================

dlg_network_options::dlg_network_options( void )
{
}

//=========================================================================

dlg_network_options::~dlg_network_options( void )
{
    Destroy();
}

void dlg_network_options::ReadOptions(void)
{
    if (fegl.AutoConnect)
    {
        m_pAutoConnect->SetFlags(m_pAutoConnect->GetFlags() | WF_SELECTED);
    }
    else
    {
        m_pAutoConnect->SetFlags(m_pAutoConnect->GetFlags() & ~WF_SELECTED );
    }

	if (fegl.LastNetConfig < m_pConnectionList->GetItemCount())
	{
		m_pConnectionList->SetSelection(fegl.LastNetConfig);
	}
	else
	{
		m_pConnectionList->SetSelection(m_pConnectionList->GetItemCount()-1);
	}
}

void dlg_network_options::WriteOptions(xbool AllowCloseNetwork)
{
    (void)AllowCloseNetwork;
	xbool newauto;
	s32   newid;

	newauto = (m_pAutoConnect->GetFlags() & WF_SELECTED) != 0;

	newid = m_pConnectionList->GetSelection();

	if (newid == -1)
	{
		newid = fegl.LastNetConfig;
	}
	if ( (fegl.AutoConnect != newauto) ||
		 (fegl.LastNetConfig != newid) )
	{
		fegl.OptionsModified = TRUE;
	}
    fegl.AutoConnect = newauto;
    m_ActiveInterfaceId = newid;
    fegl.LastNetConfig = m_ActiveInterfaceId;

}

//=========================================================================
void dlg_network_options::UpdateConnectStatus(f32 DeltaTime)
{
    s32 i;
    card_info Info;
	s32 card_status[2];
	s32 error,card,error2;


    m_CardScanDelay -=DeltaTime;
    if (m_CardScanDelay <=0.0f)
    {
        m_CardScanDelay = 1.0f;
        card_status[0] = card_Check("card00:",&Info);
		card_status[1] = card_Check("card01:",&Info);

		if (card_status[0]==CARD_ERR_CHANGED)
		{
			card_status[0]=0;
		}

		if (card_status[1]==CARD_ERR_CHANGED)
		{
			card_status[1]=0;
		}
		// The current card is valid and we have a configuration so we just bail
		if ( (card_status[m_CardWithConfig]==0) && (m_ConfigList.Count > 0) )
			return;

		// No status change, just bail
		if ( (card_status[0] == m_CardStatus[0]) &&
			 (card_status[1] == m_CardStatus[1]) )
			 return;

		m_CardStatus[0] = card_status[0];
		m_CardStatus[1] = card_status[1];

        m_pConnectionList->DeleteAllItems();

		error = 0;
		card = 0;

        if (m_CardStatus[0] == 0)
		{
			error = net_GetConfigList("mc0:BWNETCNF/BWNETCNF",&m_ConfigList);
			if (error || (m_ConfigList.Count==0) )
			{
				error2 = net_GetConfigList("mc1:BWNETCNF/BWNETCNF",&m_ConfigList);
				if ( m_ConfigList.Count > 0 )
				{
					card = 1;
					error = 0;
				}
			}
		}
		else
		{
			card = 1;
            error = net_GetConfigList("mc1:BWNETCNF/BWNETCNF",&m_ConfigList);
		}

		m_CardWithConfig = card;

        if (error < 0)
        {
            audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
#ifdef DEMO_DISK_HARNESS
            WarningBox( xwstring("ERROR!"),
                        xwstring(xfs("The Network Configuration file on the\n"
									 "memory card (8MB)(for PlayStation""\x10""2)\n"
									 "in MEMORY CARD slot %d is for another\n"
									 "PlayStation""\x10""2 console and cannot be\n"
									 "used.\n",card+1)) );
#else
			GotoControl ((ui_control*)m_pModify);
            WarningBox( xwstring("ERROR!"),
                        xwstring(xfs("The Network Configuration file on the\n"
									 "memory card (8MB)(for PlayStation""\x10""2)\n"
									 "in MEMORY CARD slot %d is for another\n"
									 "PlayStation""\x10""2 console and cannot be\n"
									 "used. Please select the MODIFY button to\n"
									 "start the Network Configuration Utility.\n",card+1)) );
					m_DialogState = STATE_MESSAGE_DELAY;
#endif
            m_MessageDelay = 1000.0f;
			m_DialogState = STATE_MESSAGE_DELAY;
            x_memset(&m_ConfigList,0,sizeof(m_ConfigList));
        }
        else
        {
            for (i=0;i<m_ConfigList.Count;i++)
            {
                m_pConnectionList->AddItem( m_ConfigList.Name[i] );
                m_pConnectionList->EnableItem(i,m_ConfigList.Available[i]);
            }
            if (m_ConfigList.Count)
            {
				if ((fegl.LastNetConfig >=0) && (fegl.LastNetConfig < m_ConfigList.Count) )
					m_ActiveInterfaceId = fegl.LastNetConfig;
				else 
				{
					m_ActiveInterfaceId = 0;
				}
                m_pConnectionList->SetSelection(m_ActiveInterfaceId);
            }
            else
            {
                m_pConnectionList->SetSelection(-1);
				if ( (m_CardStatus[0] == CARD_ERR_UNPLUGGED) && (m_CardStatus[1] == CARD_ERR_UNPLUGGED) )
				{
					WarningBox( xwstring("ERROR!"),
								xwstring("There is no memory card (8MB)\n"
										 "(for PlayStation""\x10""2) present.\n"
										 "Please insert a memory card\n"
										 "(8MB)(for PlayStation""\x10""2).\n"));
					m_MessageDelay = 1000.0f;
					m_DialogState = STATE_MESSAGE_DELAY;
	                audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
				}
				else
				{
					WarningBox( xwstring("ERROR!"),
								xwstring("No Network Configuration file is present.\n"
										 "Please insert a memory card (8MB)\n"
										 "(for PlayStation""\x10""2) with a valid\n"
										 "Network Configuration file or select\n"
										 "the MODIFY button to start the Network\n"
										 "Configuration Utility.") );
					m_MessageDelay = 1000.0f;
					m_DialogState = STATE_MESSAGE_DELAY;
	                audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
				}
            }
        }
        if (m_ConfigList.Count==0)
        {
            s32 index;
            x_memset(&m_ConfigList,0,sizeof(m_ConfigList));
            index = m_pConnectionList->AddItem( "No configurations available" );
            m_pConnectionList->EnableItem(index,FALSE);
        }
        else
        {
            if ( (m_ActiveInterfaceId >=0) && (m_ActiveInterfaceId < m_ConfigList.Count) )
			{
                m_pConnectionList->SetSelection(m_ActiveInterfaceId);
			}
            else
            {
                m_pConnectionList->SetSelection(0);
            }
        }
    }

    m_IsConnected = (pSvrMgr->GetLocalAddr().IP != 0);
    // Set title regarding online / offline
    if( m_IsConnected )
        m_pConnect->SetLabel( "DISCONNECT" );
    else
        m_pConnect->SetLabel( "CONNECT" );
}

//=========================================================================

xbool dlg_network_options::Create( s32                        UserID,
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

    m_BackgroundColor   = FECOL_DIALOG2;

    m_pConnectionList   = (ui_listbox*)FindChildByID( IDC_CONNECTION_TYPES              );
    m_pConnect          = (ui_button*) FindChildByID( IDC_CONNECT                       );
    m_pAutoConnect      = (ui_check*)  FindChildByID( IDC_AUTO_CONNECT                  );
	m_pModify			= (ui_button*) FindChildByID( IDC_MODIFY						);
    m_pFrame            = (ui_frame*)  FindChildByID( IDC_NETWORK_FRAME                 );
//    m_pDone             = (ui_button*) FindChildByID( IDC_DONE                          );
//    m_pSave             = (ui_button*) FindChildByID( IDC_SAVE                          );
//    m_pLoad             = (ui_button*) FindChildByID( IDC_LOAD                          );
    m_pOk               = (ui_button*) FindChildByID( IDC_OK                            );
    m_pCancel           = (ui_button*) FindChildByID( IDC_CANCEL                        );

    m_pFrame->EnableTitle( (xwstring)StringMgr( "ui", IDS_NETWORK_SETTINGS ), TRUE );

    m_LastCardStatus    = -1;
    m_CardScanDelay     = 0.25f;            // Allow a short time for all the dialogs to appear before hitting memcard
    m_DialogState       = STATE_IDLE;
    m_ActiveInterfaceId = fegl.LastNetConfig;
    m_HasChanged        = FALSE;
	m_CardWithConfig	= 0;
	m_CardStatus[0]		= -1;
	m_CardStatus[1]		= -1;

	GotoControl((ui_control*)m_pConnect);
    x_memset(&m_ConfigList,0,sizeof(m_ConfigList));

    m_pConnectionList->SetSelection(-1);

    ReadOptions();
    
    m_pMessage = NULL;

    #ifdef DEMO_DISK_HARNESS
    m_pModify->SetFlag( WF_DISABLED, 1 );
    #endif
    UpdateConnectStatus(0.0f);
    // Return success code
    return Success;
}

//=========================================================================

void dlg_network_options::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_network_options::Render( s32 ox, s32 oy )
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

        }

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}


//=========================================================================

void dlg_network_options::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_network_options::OnPadSelect( ui_win* pWin )
{
    // Check for OK button
    if( pWin == (ui_win *)m_pOk )
    {
	    WriteOptions(TRUE);
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        m_pManager->EndDialog( m_UserID, TRUE );
		if (fegl.OptionsModified)
		{
			load_save_params options(0,(load_save_mode)(SAVE_NETWORK_OPTIONS|ASK_FIRST),NULL);
			m_pManager->OpenDialog( m_UserID, 
								  "optionsloadsave", 
								  irect(0,0,300,150), 
								  NULL, 
								  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
								  (void *)&options );
			fegl.OptionsModified = FALSE;
		}
    }

    // Check for CANCEL button
    else if( pWin == (ui_win*)m_pCancel )
    {
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        m_pManager->EndDialog( m_UserID, TRUE );
    }

    // Check for CONNECT button
    else if( pWin == (ui_win*)m_pConnect )
    {
	    WriteOptions(TRUE);

        if (m_IsConnected)
        {
            WarningBox( xwstring("PLEASE WAIT..."),
                        xwstring("Disconnecting from the network.\n"
                                 "This will take a moment."),NULL );
            m_MessageDelay = 2.0f;
            m_DialogState  = STATE_MESSAGE_DELAY;
			fegl.DeliberateDisconnect = TRUE;
            net_ActivateConfig(FALSE);
            m_IsConnected = FALSE;
        }
        else
        {
            m_ActiveInterfaceId = m_pConnectionList->GetSelection();
            if (m_ActiveInterfaceId < 0)
            {
				if (m_ConfigList.Count > 0)
				{
                    #ifdef DEMO_DISK_HARNESS
					WarningBox( xwstring("ERROR!"),
								xwstring("The supported network hardware is not\n"
										 "correctly connected to the PlayStation""\x10""2\n"
										 "console. Please turn off the console\n"
										 "and check the hardware connection.\n") );
                    #else
					GotoControl ((ui_control*)m_pModify);
					WarningBox( xwstring("ERROR!"),
								xwstring("The supported network hardware is not\n"
										 "correctly connected to the PlayStation""\x10""2\n"
										 "console. Please turn off the console\n"
										 "and check the hardware connection. If\n"
										 "you continue to have trouble, please use\n"
										 "the MODIFY button to start the Network\n"
										 "Configuration Utility.\n") );
                    #endif
				}
				else
				{
					if ( (m_CardStatus[0] == CARD_ERR_UNPLUGGED) && (m_CardStatus[0] == CARD_ERR_UNPLUGGED) )
					{
						WarningBox( xwstring("ERROR!"),
									xwstring("There is no memory card (8MB)\n"
											 "(for PlayStation""\x10""2) present.\n"
											 "Please insert a memory card\n"
											 "(8MB)(for PlayStation""\x10""2).\n"));
					}
					else
					{
#ifdef DEMO_DISK_HARNESS
						WarningBox( xwstring("ERROR!"),
									xwstring("No configurations are available.\n"
											 "Please insert a memory card (8MB)\n"
											 "(for PlayStation""\x10""2) with a valid\n"
											 "Network Configuration file.") );
#else
						GotoControl ((ui_control*)m_pModify);
						WarningBox( xwstring("ERROR!"),
									xwstring("No configurations are available.\n"
											 "Please insert a memory card (8MB)\n"
											 "(for PlayStation""\x10""2) with a valid\n"
											 "Network Configuration file or select\n"
											 "the MODIFY button to start the Network\n"
											 "Configuration Utility.") );
#endif
					}
				}
                audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
                m_MessageDelay = 1000.0f;
                m_DialogState = STATE_MESSAGE_DELAY;
            }
            else
            {
                WarningBox( xwstring("PLEASE WAIT..."),
                            xwstring("Activating selected network\n"
                                     "configuration. This will\n"
                                     "take a moment."),NULL );
                m_MessageDelay = 1.0f;
                m_DialogState  = STATE_DELAY_ACTIVATE;
            }
        }
        
    }
	else if (pWin == (ui_win*)m_pModify)
	{
		OptionBox( "ALERT!",
					"To modify Your Network Configuration file,\n"
					 "we need to run the Network Configuration\n"
					 "Utility. When this is done, the game will\n"
					 "restart. You will lose any unsaved game\n"
					 "data. Do you wish to continue?",
					 xwstring("Yes"),
					 xwstring("No"),
					 TRUE);
		m_DialogState = STATE_WAIT_MODIFY_QUESTION;
	}


/*
    else if (pWin == (ui_win *)m_pLoad)
    {
        WriteOptions(TRUE);
        load_save_params options(0,LOAD_NETWORK_OPTIONS,this);
        m_pManager->OpenDialog( m_UserID, 
                              "optionsloadsave", 
                              irect(0,0,300,150), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
        m_HasChanged = FALSE;
    }
    else if (pWin == (ui_win *)m_pSave)
    {
        WriteOptions(TRUE);
        load_save_params options(0,SAVE_NETWORK_OPTIONS,this);
        m_pManager->OpenDialog( m_UserID, 
                              "optionsloadsave", 
                              irect(0,0,300,150), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
        m_HasChanged = FALSE;
    }
*/
}

//=========================================================================

void dlg_network_options::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Close dialog and step back
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );

//    WriteOptions(TRUE);
    m_pManager->EndDialog( m_UserID, TRUE );

/*
	if (fegl.OptionsModified)
	{
		load_save_params options(0,(load_save_mode)(SAVE_NETWORK_OPTIONS|ASK_FIRST),NULL);
		m_pManager->OpenDialog( m_UserID, 
							  "optionsloadsave", 
							  irect(0,0,300,150), 
							  NULL, 
							  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
							  (void *)&options );
		fegl.OptionsModified = FALSE;
	}
*/
}

//=========================================================================

void dlg_network_options::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pData;

    // If X pressed in Connection List goto directly to the select button
    if( (pSender == (ui_win*)m_pConnectionList) && (Command == WN_LIST_ACCEPTED) )
    {
        GotoControl( (ui_control*)m_pConnect );
    }
    if (Command == WN_USER)
    {
        ReadOptions();
    }
}

//=========================================================================

void dlg_network_options::UpdateControls( void )
{
}

//=========================================================================
void dlg_network_options::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    s32 state,Id;

    (void)pWin;
    UpdateConnectStatus(DeltaTime);

    switch (m_DialogState)
    {

    //-------------------------------------------------------------------------
    case STATE_IDLE:
        // The message result will be set to DLG_MESSAGE_IDLE while no user
        // input has been given. If the user cancels or accepts in any manner,
        // then their message box is automatically destroyed so we do not need
        // to do the EndDialog in that case.
        if ( m_pMessage && (m_MessageResult == DLG_MESSAGE_IDLE) )
        {
            m_pManager->EndDialog(m_UserID,TRUE);
//            m_pManager->SetCursorPos( m_UserID, 358, 282 );
            m_pMessage =NULL;
        }
    break;
    //-------------------------------------------------------------------------
    case STATE_MESSAGE_DELAY:
		if (m_MessageDelay >=0)
		{
			m_MessageDelay -= DeltaTime;
			if (m_MessageDelay < 0.0f)
			{
				m_DialogState = STATE_IDLE;
			}
		}
        break;

    case STATE_DELAY_THEN_EXIT:
        m_MessageDelay -= DeltaTime;
        if (m_MessageDelay < 0.0f)
        {
            m_pManager->EndDialog(m_UserID,TRUE);       // Delete the message box
            m_pMessage = NULL;
            m_DialogState = STATE_IDLE;
        }
        break;

	case STATE_WAIT_MODIFY_QUESTION:
		if (m_MessageResult == DLG_MESSAGE_IDLE)
			break;
		if (m_MessageResult == DLG_MESSAGE_YES)
		{
			m_DialogState = STATE_START_MODIFY;
			m_MessageDelay = 2.0f;
            WarningBox( xwstring("PLEASE WAIT..."),
                        xwstring("It will take a few moments for the\n"
								 "Network Configuration Utility to load.\n"),NULL);
		}
		else
			m_DialogState = STATE_IDLE;
		break;

	case STATE_START_MODIFY:
		m_MessageDelay -= DeltaTime;
		if (m_MessageDelay < 0.0f)
		{
			tgl.ExitApp = TRUE;
			m_DialogState = STATE_IDLE;
		}
		break;

    case STATE_WAIT_CONNECT:
        m_MessageDelay -= DeltaTime;
        state = net_GetAttachStatus(Id);
        if ((m_MessageDelay < 0.0f) )
        {
            audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
            #ifdef DEMO_DISK_HARNESS
            WarningBox( xwstring("ERROR!"),
                        xwstring("Timed out while attempting to connect.\n"
								   "Please try again."));
            #else
			GotoControl ((ui_control*)m_pModify);
            WarningBox( xwstring("ERROR!"),
                        xwstring("Timed out while attempting to connect.\n"
								   "Please try again. If you still have\n"
								   "trouble connecting, please check\n"
								   "Your Network Configuration file\n"
								   "by selecting the MODIFY button to\n"
								   "start the Network Configuration\n"
								   "Utility."));
            #endif
            m_DialogState = STATE_MESSAGE_DELAY;
            m_MessageDelay = 1000.0f;
        }
        else if (state == ATTACH_STATUS_ERROR)
        {
			connect_status status;
			char textbuff[256];

			net_GetConnectStatus(status);

            audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
			x_sprintf(textbuff, "An error occurred while trying to\n"
								"connect. Please try again. If you\n"
								"still have trouble connecting, please\n"
								"check Your Network Configuration file\n"
								"by selecting the MODIFY button to run\n"
								"the Network Configuration Utility.");
			if (status.ErrorText[0])
			{
				char* pDest,*pSrc,length;
				x_strcat(textbuff,"\nError message : ");
				pDest = textbuff+x_strlen(textbuff);
				pSrc = status.ErrorText;
				length = 0;

				while (*pSrc)
				{
					if (length > 30)
					{
						*pDest++ = '\n';
						length = 0;
					}
					*pDest++ = *pSrc++;
					length++;
				}
				*pDest=0;
			}

            WarningBox( xwstring("ERROR!"),
                        xwstring(textbuff) );
            m_DialogState = STATE_MESSAGE_DELAY;
            m_MessageDelay = 1000.0f;
        }
        else
        {
            if (m_IsConnected && (m_MessageDelay < 89.0f))
            {
                char temp[32];

                if (m_pAutoConnect->GetFlags() & WF_SELECTED)
                {
					if (m_pMessage)
					{
			            m_pManager->EndDialog(m_UserID,TRUE);
						m_pMessage = NULL;
					}
					GotoControl(m_pOk);
                }
                else
                {
                    GotoControl(m_pAutoConnect);
                }
				interface_info info;

				net_GetInterfaceInfo(-1,info);

                net_IPToStr(info.Address,temp);
#if defined(PAL_RELEASE)
                x_sprintf(m_MessageBuffer,"Connected. Your IP\n"
                                          "address is %s.\n     ",temp);
#else
                x_sprintf(m_MessageBuffer,"Connected. Your network\n"
                                          "address is %s.\n",temp);
#endif
				connect_status status;

				net_GetConnectStatus(status);

				if (status.ConnectSpeed > 1)
				{
					x_strcat(m_MessageBuffer,xfs("Connected at %2.1fK.",(f32)status.ConnectSpeed/1000.0f));
					fegl.ModemConnection = TRUE;
				}
				else
				{
					fegl.ModemConnection = (status.ConnectSpeed != 0);
				}
                WarningBox( xwstring("SUCCESS!"),
                            xwstring(m_MessageBuffer),xwstring("DONE"));

                audio_Play(SFX_MISC_RESPAWN, AUDFLAG_CHANNELSAVER);
                m_DialogState = STATE_DELAY_THEN_EXIT;
                m_MessageDelay = 10.0f;

            }
            else
            {
                if (m_MessageResult != DLG_MESSAGE_IDLE)
                {
					fegl.DeliberateDisconnect = TRUE;
                    net_ActivateConfig(FALSE);
                    WarningBox( xwstring("CANCELLED!"),
                                xwstring("Network connect aborted.\n"
									   "Please try again. If you still have\n"
									   "trouble connecting, please check\n"
									   "Your Network Configuration file by\n"
									   "selecting the MODIFY button to\n"
									   "start the Network Configuration\n"
									   "Utility."));
                m_DialogState = STATE_DELAY_THEN_EXIT;
                m_MessageDelay = 1000.0f;
                  
                }
                else
                {

                    x_sprintf(m_MessageBuffer,"Connecting to the network.\n"
                                              "This will take several moments.\n"
                                              "Timeout remaining: %d seconds.",(s32)m_MessageDelay);
                    m_pMessage->Configure( xwstring("PLEASE WAIT..."), 
                               NULL,
                               xwstring("CANCEL"), 
                               xwstring(m_MessageBuffer), 
                               XCOLOR_WHITE,
                               &m_MessageResult
                               );
                }

            }
        }
        break;
    case STATE_DELAY_ACTIVATE:
        m_MessageDelay -= DeltaTime;
        if (m_MessageDelay <=0.0f)
        {
            char path[64];
            s32 status;

            x_sprintf(path,"mc%d:BWNETCNF/BWNETCNF",m_CardWithConfig);
            m_DialogState = STATE_WAIT_ACTIVATE;
            m_MessageDelay = 10.0f;
            fegl.LastNetConfig = m_pConnectionList->GetSelection();
			if (fegl.LastNetConfig < 0)
				fegl.LastNetConfig = 0;

            status = net_SetConfiguration(path,fegl.LastNetConfig);
            if (status < 0)
            {
                audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
				GotoControl ((ui_control*)m_pModify);
                WarningBox( xwstring("ERROR!"),
                            xwstring("An unknown error occurred while\n"
                                     "trying to activate the selected\n"
                                     "configuration. Please select the\n"
                                     "MODIFY button to start the Network\n"
									 "Configuration Utility to configure\n"
									 "Your Network Configuration file."));
                m_DialogState = STATE_MESSAGE_DELAY;
                m_MessageDelay = 1000.0f;
            }

        }
        break;

    case STATE_WAIT_ACTIVATE:
        m_MessageDelay -= DeltaTime;
        if (m_MessageDelay < 0.0f)
        {
            audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
			GotoControl ((ui_control*)m_pModify);
            WarningBox( xwstring("ERROR!"),
                        xwstring("No interfaces became active.\n"
                                 "Please try again or select the\n"
								 "MODIFY button to start the\n"
								 "Network Configuration Utility.\n"));
			m_DialogState = STATE_MESSAGE_DELAY;
            m_MessageDelay = 1000.0f;
        }
        else
        {
            state = net_GetAttachStatus(Id);
            if ( (state == ATTACH_STATUS_CONFIGURED) ||
                 (state == ATTACH_STATUS_ATTACHED) )
            {
                m_ActiveInterfaceId = m_pConnectionList->GetSelection();
                fegl.LastNetConfig = m_ActiveInterfaceId;
                m_MessageDelay = 90.9f;                 // Timeout for waiting for a connection
                m_DialogState  = STATE_WAIT_CONNECT;
                net_ActivateConfig(TRUE);
            }
            else
            {
                x_sprintf(m_MessageBuffer,"Activating selected network\n"
                                          "configuration. This will\n"
                                          "take a moment.\n"
                                          "Timeout remaining: %d seconds.",(s32)m_MessageDelay);

                m_pMessage->Configure( xwstring("PLEASE WAIT..."), 
                           NULL, 
                           NULL, 
                           xwstring(m_MessageBuffer), 
                           XCOLOR_WHITE,
                           &m_MessageResult
                           );
            }
        }
        break;
    }
    if (m_DialogState != STATE_IDLE)
    {
        if (m_MessageResult != DLG_MESSAGE_IDLE)
        {
            m_pMessage = NULL;
            m_DialogState = STATE_IDLE;
        }
    }


}

//=========================================================================
void dlg_network_options::WarningBox(const xwchar *pTitle,const xwchar *pMessage,const xwchar* pButton)
{
    if (m_pMessage)
    {
        if (m_MessageResult == DLG_MESSAGE_IDLE)
        {
            m_pManager->EndDialog(m_UserID,TRUE);
        }
    }
    m_pMessage = (dlg_message *)m_pManager->OpenDialog(m_UserID,
                                                        "message",
                                                        irect(0,0,310,200),
                                                        NULL,
                                                        WF_VISIBLE|WF_BORDER|WF_DLG_CENTER);
    ASSERT(m_pMessage);
    m_pMessage->Configure( pTitle, 
                           NULL,
                           pButton, 
                           pMessage, 
                           XCOLOR_WHITE,
                           &m_MessageResult
                           );
}

//=========================================================================
void dlg_network_options::OptionBox( const char* pTitle,const char* pMessage, const xwchar *pYes, const xwchar *pNo,const xbool DefaultToNo )
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
    }
    m_pMessage = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                       "message",
                                                       irect(0,0,310,200),
                                                       NULL,
                                                       WF_VISIBLE|WF_BORDER|WF_DLG_CENTER);
    ASSERT(m_pMessage);
    m_pMessage->Configure( xwstring(pTitle), 
                           pYes,
                           pNo,
                           xwstring(pMessage), 
                           XCOLOR_WHITE,&m_MessageResult,DefaultToNo);
}
