//=========================================================================
//
//  dlg_buddies.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_frame.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_dlg_vkeyboard.hpp"

#include "dlg_Buddies.hpp"
#include "dlg_loadsave.hpp"

#include "fe_globals.hpp"
#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

#include "StringMgr\StringMgr.hpp"


extern xbool g_ControlModified;

//=========================================================================
//  Campaign Dialog
//=========================================================================

enum controls
{
    IDC_CANCEL,
    IDC_OK,

    IDC_ADD,
    IDC_EDIT,
    IDC_DELETE,

    IDC_LIST
};

ui_manager::control_tem BuddiesControls[] =
{
    { IDC_CANCEL,   IDS_CANCEL,   "button",    8, 251,  96,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OK,       IDS_OK,       "button",  216, 251,  96,  24, 2, 2, 1, 1, ui_win::WF_VISIBLE },

    { IDC_ADD,      IDS_ADD,      "button",    8, 224,  96,  24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_EDIT,     IDS_EDIT,     "button",  112, 224,  96,  24, 1, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DELETE,   IDS_DELETE,   "button",  216, 224,  96,  24, 2, 1, 1, 1, ui_win::WF_VISIBLE },

    { IDC_LIST,     IDS_NULL,     "listbox",   8,  50, 304, 168, 0, 0, 3, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem BuddiesDialog =
{
    IDS_BUDDIES,
    3, 3,
    sizeof(BuddiesControls)/sizeof(ui_manager::control_tem),
    &BuddiesControls[0],
    0
};

// 336, 223

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

void dlg_buddies_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "buddies", &BuddiesDialog, &dlg_buddies_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_buddies_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags,void *pUserData )
{
    dlg_buddies* pDialog = new dlg_buddies;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags,pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_buddies
//=========================================================================

void dlg_buddies::BackupBuddies( void )
{
    m_OldnBuddies = m_pWarriorSetup->nBuddies;
    x_memcpy( m_OldBuddies, m_pWarriorSetup->Buddies, sizeof(m_OldBuddies) );
}

//=========================================================================

void dlg_buddies::RestoreBuddies( void )
{
    m_pWarriorSetup->nBuddies = m_OldnBuddies;
    x_memcpy( m_pWarriorSetup->Buddies, m_OldBuddies, sizeof(m_OldBuddies) );
}

//=========================================================================

dlg_buddies::dlg_buddies( void )
{
}

//=========================================================================

dlg_buddies::~dlg_buddies( void )
{
}

//=========================================================================

xbool dlg_buddies::Create( s32                        UserID,
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

    m_pWarriorSetup = (warrior_setup*)pUserData;
    ASSERT( m_pWarriorSetup );
    BackupBuddies();

    m_BackgroundColor   = FECOL_DIALOG2;

    // Initialize Data
    m_InUpdate      = 0;
    m_iSelectedItem = -1;
    m_AddNameDone   = FALSE;
    m_EditNameDone  = FALSE;
    m_KeyboardOk    = FALSE;
    m_DoDelete      = FALSE;

    m_pOk           = (ui_button*) FindChildByID( IDC_OK );
    m_pCancel       = (ui_button*) FindChildByID( IDC_CANCEL );
    m_pAdd          = (ui_button*) FindChildByID( IDC_ADD );
    m_pEdit         = (ui_button*) FindChildByID( IDC_EDIT );
    m_pDelete       = (ui_button*) FindChildByID( IDC_DELETE );
    m_pList         = (ui_listbox*)FindChildByID( IDC_LIST );

    for( s32 i=0 ; i<m_pWarriorSetup->nBuddies ; i++ )
    {
        m_pList->AddItem( m_pWarriorSetup->Buddies[i] );
    }

    if( m_pList->GetItemCount() > 0 )
        m_pList->SetSelection( 0 );

    // Update button states
    m_pAdd   ->SetFlag( WF_DISABLED, (m_pWarriorSetup->nBuddies == FE_MAX_BUDDIES) );
    m_pEdit  ->SetFlag( WF_DISABLED, (m_pList->GetSelection() == -1) );
    m_pDelete->SetFlag( WF_DISABLED, (m_pList->GetSelection() == -1) );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_buddies::Destroy( void )
{
    // Destory the dialog
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_buddies::Render( s32 ox, s32 oy )
{
    // Only render if visible
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
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_BLACK, m_Label );
            rb.Translate( -1, -1 );
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

void dlg_buddies::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;

    // Add Buddy
    if( m_AddNameDone )
    {
        m_AddNameDone = FALSE;
        if( m_KeyboardOk )
        {
            m_KeyboardOk = FALSE;

            if( m_pWarriorSetup->nBuddies < FE_MAX_BUDDIES )
            {
                // Add into list and select
                s32 iItem = m_pList->AddItem( m_BuddyName );
                m_pList->SetSelection( iItem );

                // Add into warrior setup
                x_wstrncpy( m_pWarriorSetup->Buddies[m_pWarriorSetup->nBuddies], m_BuddyName, FE_MAX_WARRIOR_NAME-1 );
                m_pWarriorSetup->Buddies[m_pWarriorSetup->nBuddies][FE_MAX_WARRIOR_NAME-1] = 0;
                m_pWarriorSetup->nBuddies++;
            }
        }
    }

    // Edit Buddy
    if( m_EditNameDone )
    {
        m_EditNameDone = FALSE;
        if( m_KeyboardOk )
        {
            m_KeyboardOk = FALSE;

            if( m_iSelectedItem != -1 )
            {
                // Edit in list
                m_pList->SetItemLabel( m_iSelectedItem, m_BuddyName );

                // Edit in warrior setup
                x_wstrncpy( m_pWarriorSetup->Buddies[m_iSelectedItem], m_BuddyName, FE_MAX_WARRIOR_NAME-1 );
                m_pWarriorSetup->Buddies[m_iSelectedItem][FE_MAX_WARRIOR_NAME-1] = 0;
            }
        }
    }

    // Delete Buddy
    if( m_DoDelete == DLG_MESSAGE_YES )
    {
        m_DoDelete = FALSE;

        if( (m_iSelectedItem != -1) && (m_pWarriorSetup->nBuddies > 0) )
        {
            // Remove from list
            m_pList->DeleteItem( m_iSelectedItem );

            // Remove from warrior setup
            for( s32 i=m_iSelectedItem ; i<FE_MAX_BUDDIES-1 ; i++ )
            {
                x_wstrncpy( m_pWarriorSetup->Buddies[i], m_pWarriorSetup->Buddies[i+1], FE_MAX_WARRIOR_NAME );
            }
            m_pWarriorSetup->nBuddies--;
        }
    }

    // Update button states
    m_pAdd   ->SetFlag( WF_DISABLED, (m_pWarriorSetup->nBuddies == FE_MAX_BUDDIES) );
    m_pEdit  ->SetFlag( WF_DISABLED, (m_pList->GetSelection() == -1) );
    m_pDelete->SetFlag( WF_DISABLED, (m_pList->GetSelection() == -1) );

    if( (m_pManager->GetWindowUnderCursor( m_UserID ) == m_pAdd) && (m_pAdd->GetFlags() & WF_DISABLED) )
    {
        if( (m_pEdit->GetFlags() & WF_DISABLED) == 0 )
            GotoControl( m_pEdit );
        else
            GotoControl( m_pOk );
    }

    if( (m_pManager->GetWindowUnderCursor( m_UserID ) == m_pDelete) && (m_pDelete->GetFlags() & WF_DISABLED) )
    {
        GotoControl( m_pAdd );
    }
}

//=========================================================================

void dlg_buddies::OnPadSelect( ui_win* pWin )
{
    // Check for OK button
    if( pWin == (ui_win*)m_pOk )
    {
        if( (x_memcmp( m_OldBuddies, m_pWarriorSetup->Buddies, sizeof(m_OldBuddies) ) != 0) ||
            (m_OldnBuddies != m_pWarriorSetup->nBuddies) )
        {
			g_ControlModified = TRUE;
        }

        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
		
/*
        if( LastSaved != tgl.HighestCampaignMission )
        {
            LastSaved = tgl.HighestCampaignMission;
            load_save_params options(0,(load_save_mode)(SAVE_CAMPAIGN_OPTIONS|ASK_FIRST),NULL);
		    m_pManager->OpenDialog( m_UserID, 
							  "optionsloadsave", 
							  irect(0,0,300,150), 
							  NULL, 
							  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
							  (void *)&options );
        }
*/
    }

    // Check for CANCEL button
    if( pWin == (ui_win*)m_pCancel )
    {
        RestoreBuddies();
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }

    // Check for ADD button
    if( pWin == (ui_win*)m_pAdd )
    {
        m_BuddyName.Clear();
        irect   r = m_pManager->GetUserBounds( m_UserID );
        ui_dlg_vkeyboard* pVKeyboard = (ui_dlg_vkeyboard*)m_pManager->OpenDialog( m_UserID, "ui_vkeyboard", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
        pVKeyboard->SetLabel( StringMgr( "ui", IDS_ADD_BUDDY ) );
        pVKeyboard->ConnectString( &m_BuddyName, FE_MAX_WARRIOR_NAME-1 );
        pVKeyboard->SetReturn( &m_AddNameDone, &m_KeyboardOk );
    }

    // Check for EDIT button
    if( pWin == (ui_win*)m_pEdit )
    {
        m_iSelectedItem = m_pList->GetSelection();
        if( m_iSelectedItem != -1 )
        {
            m_BuddyName = m_pList->GetSelectedItemLabel();
            irect   r = m_pManager->GetUserBounds( m_UserID );
            ui_dlg_vkeyboard* pVKeyboard = (ui_dlg_vkeyboard*)m_pManager->OpenDialog( m_UserID, "ui_vkeyboard", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
            pVKeyboard->SetLabel( StringMgr( "ui", IDS_EDIT_BUDDY ) );
            pVKeyboard->ConnectString( &m_BuddyName, FE_MAX_WARRIOR_NAME-1 );
            pVKeyboard->SetReturn( &m_EditNameDone, &m_KeyboardOk );
        }
    }

    // Check for DELETE button
    if( pWin == (ui_win*)m_pDelete )
    {
        m_iSelectedItem = m_pList->GetSelection();
        if( m_iSelectedItem != -1 )
        {
            dlg_message* pMD = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                                     "message",
                                                                     irect(0,0,250,120),
                                                                     NULL,
                                                                     WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
            pMD->Configure( StringMgr("ui", IDS_DELETE_BUDDY),
                            StringMgr("ui", IDS_YES),
                            StringMgr("ui", IDS_NO),
                            StringMgr("ui", IDS_ARE_YOU_SURE),
                            HUDCOL_TEXT_WHITE,
                            &m_DoDelete );
        }
    }
}

//=========================================================================

void dlg_buddies::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    RestoreBuddies();
    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
}

//=========================================================================

void dlg_buddies::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================
