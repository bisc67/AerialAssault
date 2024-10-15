//=========================================================================
//
//  dlg_message.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_font.hpp"

#include "dlg_Message.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"
#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

//==============================================================================
// Online Dialog
//==============================================================================

enum controls
{
    IDC_MSG_NO,
    IDC_MSG_YES
};

ui_manager::control_tem MessageControls[] =
{
    { IDC_MSG_NO,   IDS_NO,     "button", 0, 0, 80, 24, 1,  0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_MSG_YES,  IDS_YES,    "button", 0, 0, 80, 24, 0,  0, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem MessageDialog =
{
    IDS_MESSAGE,
    2, 1,
    sizeof(MessageControls)/sizeof(ui_manager::control_tem),
    &MessageControls[0],
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

void dlg_message_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "message", &MessageDialog, &dlg_message_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_message_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_message* pDialog = new dlg_message;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_message
//=========================================================================

dlg_message::dlg_message( void )
{
}

//=========================================================================

dlg_message::~dlg_message( void )
{
    Destroy();
}

//=========================================================================

void dlg_message::Configure( const xwchar* Title, const xwchar* Yes, const xwchar* No, const xwchar* Message, const xcolor MessageColor, s32 *pResult, const xbool DefaultToNo, const xbool AllowCancel )
{
    xbool   bYes  = TRUE;
    xbool   bNo   = TRUE;
    xwchar  Empty = 0;

    SetLabel( Title );
	m_AllowCancel = AllowCancel;

    if( !Yes )        Yes = &Empty;
    if( !No  )        No  = &Empty;

    m_pYes->SetLabel( Yes );
    m_pNo ->SetLabel( No  );

    if( x_wstrlen(Yes) == 0 )
    {
        m_pYes->SetFlags( (m_pYes->GetFlags() & ~WF_VISIBLE) | WF_DISABLED );
        bYes = FALSE;
    }
	else
	{
        m_pYes->SetFlags( (m_pYes->GetFlags() & ~WF_DISABLED) | WF_VISIBLE );
		bYes = TRUE;
	}

    if( x_wstrlen(No) == 0 )
    {
        m_pNo ->SetFlags( (m_pNo ->GetFlags() & ~WF_VISIBLE) | WF_DISABLED );
        bNo = FALSE;
    }
	else
	{
        m_pNo ->SetFlags( (m_pNo ->GetFlags() & ~WF_DISABLED) | WF_VISIBLE );
		bNo = TRUE;
	}

    // Configure for a single button
    if( bNo && !bYes )
    {
        irect rw = GetPosition();
        irect rb = m_pNo->GetPosition();

        rb.l = rw.GetWidth() / 2 - 70;
        rb.r = rw.GetWidth() / 2 + 70;

        m_pNo->SetPosition( rb );
    }

    m_Message       = Message;
    m_MessageColor  = MessageColor;
    m_pResult       = pResult;
    m_Timeout       = -1;

    if( m_pResult )
    {
        *m_pResult = DLG_MESSAGE_IDLE;
    }

    // Activate appropriate control
    if ( bNo && DefaultToNo )
        GotoControl( m_pNo );
    else if( bYes )
        GotoControl( m_pYes );
}

void dlg_message::SetTimeout          (f32 timeout )
{
    m_Timeout   = timeout;
}

//=========================================================================

xbool dlg_message::Create( s32                        UserID,
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

    m_pYes = (ui_button*) FindChildByID( IDC_MSG_YES );
    m_pNo  = (ui_button*) FindChildByID( IDC_MSG_NO  );
	m_AllowCancel = FALSE;

    s32 w = Position.GetWidth();
    s32 h = Position.GetHeight();

    irect cp;

    // Move No button
    cp = m_pNo->GetPosition();
    cp.Translate( w-cp.GetWidth()-8, h-cp.GetHeight()-8 );
    m_pNo->SetPosition( cp );

    // Move Yes button
    cp = m_pYes->GetPosition();
    cp.Translate( 8, h-cp.GetHeight()-8 );
    m_pYes->SetPosition( cp );

    // Clear pointer to result code
    m_pResult = NULL;
    m_Timeout = -1;

    // Return success code
    return Success;
}

//=========================================================================

void dlg_message::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_message::Render( s32 ox, s32 oy )
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
            rb.Deflate( 16, 1 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_BLACK, m_Label );
            rb.Translate( -1, -1 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_WHITE, m_Label );
        }

        // Render Message
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        r.Deflate( 8, 40 );
        r.t += 9;
//      m_pManager->RenderElement( m_iElement, r, 0 );
        r.Deflate( 4, 4 );
        m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, m_MessageColor, m_Message );

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }

}

void dlg_message::OnUpdate(ui_win *pWin,f32 DeltaTime)
{
    (void)pWin;
    if (m_Timeout >=0.0f)
    {
        m_Timeout -= DeltaTime;
        if (m_Timeout <=0.0f)
        {
            m_pManager->EndDialog(m_UserID,TRUE);
        }
    }
}

//=========================================================================

void dlg_message::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_message::OnPadSelect( ui_win* pWin )
{
    if( pWin == (ui_win*)m_pYes )
    {
        // Close dialog and step back
        if( m_pResult )
            *m_pResult = DLG_MESSAGE_YES;
        
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
    else if( pWin == (ui_win*)m_pNo )
    {
        if( m_pResult )
            *m_pResult = DLG_MESSAGE_NO;

        m_pManager->EndDialog(m_UserID,TRUE);
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
}

//=========================================================================

void dlg_message::OnPadBack( ui_win* pWin )
{
    (void)pWin;

	if (m_AllowCancel)
	{// Close dialog and step back
		m_pManager->EndDialog( m_UserID, TRUE );
		if (m_pResult)
		{
			*m_pResult = DLG_MESSAGE_BACK;
		}
		audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
	}
}

//=========================================================================

void dlg_message::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================

void dlg_message::UpdateControls( void )
{
}

//=========================================================================
