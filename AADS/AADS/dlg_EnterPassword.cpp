//=========================================================================
//
//  dlg_enterpassword.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_font.hpp"

#include "dlg_enterpassword.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"
#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"
#include "fe_globals.hpp"

extern xbool    g_GotJoinPassword;
extern xwchar   g_JoinPassword[FE_MAX_ADMIN_PASS];

//==============================================================================
// Online Dialog
//==============================================================================

enum controls
{
    IDC_CANCEL,
    IDC_OK,
    IDC_PASSWORD
};

ui_manager::control_tem EnterPasswordControls[] =
{
    { IDC_CANCEL,   IDS_CANCEL,             "button", 0, 0, 80, 24, 1,  1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OK,       IDS_OK,                 "button", 0, 0, 80, 24, 0,  1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_PASSWORD, IDS_NULL,               "edit",   0, 0, 80, 24, 0,  0, 2, 1, ui_win::WF_VISIBLE }
};

ui_manager::dialog_tem EnterPasswordDialog =
{
    IDS_ENTER_PASSWORD,
    2, 2,
    sizeof(EnterPasswordControls)/sizeof(ui_manager::control_tem),
    &EnterPasswordControls[0],
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

void dlg_enterpassword_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "enter password", &EnterPasswordDialog, &dlg_enterpassword_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_enterpassword_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_enterpassword* pDialog = new dlg_enterpassword;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_enterpassword
//=========================================================================

dlg_enterpassword::dlg_enterpassword( void )
{
}

//=========================================================================

dlg_enterpassword::~dlg_enterpassword( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_enterpassword::Create( s32                        UserID,
                                 ui_manager*                pManager,
                                 ui_manager::dialog_tem*    pDialogTem,
                                 const irect&               Position,
                                 ui_win*                    pParent,
                                 s32                        Flags,
                                 void*                      pUserData)
{
    xbool   Success = FALSE;

    (void)pUserData;

    ASSERT( pManager );

    // Make it input modal
    Flags |= WF_INPUTMODAL;

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_BackgroundColor   = FECOL_DIALOG2;

    m_pOk       = (ui_button*)FindChildByID( IDC_OK        );
    m_pCancel   = (ui_button*)FindChildByID( IDC_CANCEL    );
    m_pPassword = (ui_edit*  )FindChildByID( IDC_PASSWORD  );

    s32 w = Position.GetWidth();
    s32 h = Position.GetHeight();

    irect cp;

    // Move Cancel button
    cp = m_pCancel->GetPosition();
    cp.Translate( w-cp.GetWidth()-8, h-cp.GetHeight()-8 );
    m_pCancel->SetPosition( cp );

    // Move Ok button
    cp = m_pOk->GetPosition();
    cp.Translate( 8, h-cp.GetHeight()-8 );
    m_pOk->SetPosition( cp );

    m_JoinPassword = g_JoinPassword;

    // Move Password edit
    cp = m_pPassword->GetPosition();
    cp.SetWidth( w-(48*2) );
    cp.Translate( 48, 8+(h-cp.GetHeight())/2 );
    m_pPassword->SetPosition( cp );
    m_pPassword->SetLabel( m_JoinPassword );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_enterpassword::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_enterpassword::Render( s32 ox, s32 oy )
{
    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        // Fade out background
//        const irect& ru = m_pManager->GetUserBounds( m_UserID );
//        m_pManager->RenderRect( ru, xcolor(0,0,0,128), FALSE );

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

void dlg_enterpassword::OnUpdate(ui_win *pWin,f32 DeltaTime)
{
    (void)pWin;
    (void)DeltaTime;
}

//=========================================================================

void dlg_enterpassword::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_enterpassword::OnPadSelect( ui_win* pWin )
{
    if( pWin == (ui_win*)m_pOk )
    {
        m_JoinPassword = m_pPassword->GetLabel();
        g_GotJoinPassword = TRUE;
        x_wstrncpy( g_JoinPassword, m_JoinPassword, FE_MAX_ADMIN_PASS-1 );
        g_JoinPassword[FE_MAX_ADMIN_PASS-1] = 0;

        // Close dialog and step back
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
    }
    else if( pWin == (ui_win*)m_pCancel )
    {
        // Close dialog and step back
        m_pManager->EndDialog(m_UserID,TRUE);
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
    }
}

//=========================================================================

void dlg_enterpassword::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Close dialog and step back
    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
}

//=========================================================================

void dlg_enterpassword::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================

void dlg_enterpassword::UpdateControls( void )
{
}

//=========================================================================
