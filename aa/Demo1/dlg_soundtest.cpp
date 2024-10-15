//=========================================================================
//
//  dlg_sound_test.cpp
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
#include "audiomgr/audio.hpp"

#include "dlg_SoundTest.hpp"
#include "dlg_message.hpp"
#include "specialversion.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

#ifdef INCLUDE_SOUND_TEST
typedef struct s_sound_labels
{
    u32     id;
    const char *  pLabel;
} sound_labels;

#undef BEGIN_LABEL_SET
#undef LABEL_VALUE
#undef END_LABEL_SET

// Begins enum set
#define BEGIN_LABEL_SET(__name__)   sound_labels __name__##Strings[] = {

// Adds a label to enum set
#define LABEL_VALUE(__name__, __value__, __desc__)   { __value__,__desc__},

// Ends enum set
#define END_LABEL_SET(__name__) {NULL,NULL},} ;

#include "labelsets/soundtypes.hpp"

#endif
//==============================================================================
// Online Dialog
//==============================================================================

enum controls
{
    IDC_CANCEL,
    IDC_PLAY,
    IDC_SOUNDS,
    IDC_CATEGORY
};

// The navigation grid is a matrix defined in the ui_manager::dialog_tem structure. In this case,
// it's a 3x2 matrix.
// With each button, the 4 values define x,y position within that matrix and width and height
// of the box active area. In the sample below, "Cancel" occupies position (0,1) and is (1,1) wide
// "Connection Types" is at position (0,0) with a size (3,1) and so on.
ui_manager::control_tem SoundTestControls[] =
{
    { IDC_CANCEL,       IDS_CANCEL,     "button",    8,     360,  80,  24, 0,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_PLAY,         IDS_PLAY,       "button",    180,   360,  80,  24, 1,  2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SOUNDS,       IDS_NULL,       "listbox",   8,     188, 350, 160, 0,  1, 2, 1, ui_win::WF_VISIBLE },
    { IDC_CATEGORY,     IDS_CATEGORY,   "listbox",   8,     52, 350, 120, 0,  0, 2, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem SoundTestDialog =
{
    IDS_SOUND_TEST_TITLE,
    2, 3,
    sizeof(SoundTestControls)/sizeof(ui_manager::control_tem),
    &SoundTestControls[0],
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
};

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Registration function
//=========================================================================

void dlg_sound_test_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "sound test", &SoundTestDialog, &dlg_sound_test_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_sound_test_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_sound_test* pDialog = new dlg_sound_test;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_sound_test
//=========================================================================

dlg_sound_test::dlg_sound_test( void )
{
}

//=========================================================================

dlg_sound_test::~dlg_sound_test( void )
{
    Destroy();
}

//=========================================================================
void dlg_sound_test::FillCategoryList( void )
{
    m_pCategoryList->DeleteAllItems();
#ifdef INCLUDE_SOUND_TEST
    xstring Label;
    sound_labels *pLabels;
    xbool found;
    s32 i;

    pLabels = SoundTypes_02Strings;

    while (pLabels->pLabel != 0)
    {
        // Go through the items in the category list. If we get a match with
        // any of the existing items, let's bail and don't bother adding it
        // to the master category list
        found = FALSE;
        for (i=0;i<m_pCategoryList->GetItemCount();i++)
        {
            Label = m_pCategoryList->GetItemLabel(i);
            if (x_strncmp(Label,pLabels->pLabel,x_strlen(Label))==0)
            {
                found = TRUE;
                break;
            }
        }

        if (!found)
        {
            s32 length;
            s32 undercount;
            char labelbuff[128];
            const char *pChar;
            char *pNew;

            undercount=2;

            pChar = pLabels->pLabel;
            length = 0;
            pNew = labelbuff;

            while ( *pChar )
            {
                if (*pChar == '_')
                {
                    undercount--;
                    if (undercount==0)
                    {
                        *pNew=0x0;
                        break;
                    }
                }
                *pNew++ = *pChar++;
            }
            m_pCategoryList->AddItem(labelbuff);
        }

        pLabels++;
    }
    m_pCategoryList->SetSelection(0);
#endif
}

//=========================================================================
void dlg_sound_test::FillSampleList( s32 ListIndex )
{
	(void)ListIndex;

    m_pSampleList->SetExitOnSelect( FALSE );
    m_pSampleList->DeleteAllItems();

#ifdef INCLUDE_SOUND_TEST
    xstring Label;
    sound_labels *pLabels;
    Label = m_pCategoryList->GetItemLabel(ListIndex);

    pLabels = SoundTypes_02Strings;

    while (pLabels->pLabel != 0)
    {
        if (x_strncmp(Label,pLabels->pLabel,x_strlen(Label))==0)
        {
            m_pSampleList->AddItem(pLabels->pLabel+x_strlen(Label)+1,pLabels->id);
        }
        pLabels++;
    }
    m_pSampleList->SetSelection(0);
#endif
}

//=========================================================================

xbool dlg_sound_test::Create( s32                        UserID,
                                ui_manager*                pManager,
                                ui_manager::dialog_tem*    pDialogTem,
                                const irect&               Position,
                                ui_win*                    pParent,
                                s32                        Flags,
                                void*                      pUserData )
{
    xbool   Success = FALSE;

	(void)UserID;
	(void)pManager;
	(void)pDialogTem;
	(void)Position;
	(void)pParent;
	(void)Flags;

    (void)pUserData;
#ifdef INCLUDE_SOUND_TEST
    ASSERT( pManager );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_BackgroundColor   = FECOL_DIALOG2;

    m_pSampleList       = (ui_listbox*)FindChildByID( IDC_SOUNDS   );
    m_pCategoryList     = (ui_listbox*)FindChildByID( IDC_CATEGORY );
    m_pCancel           = (ui_button*) FindChildByID( IDC_CANCEL   );
    m_pPlay             = (ui_button*) FindChildByID( IDC_PLAY     );

    FillCategoryList();
    FillSampleList(m_pCategoryList->GetSelection());
#endif
    // Return success code
    return Success;
}

//=========================================================================

void dlg_sound_test::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_sound_test::Render( s32 ox, s32 oy )
{
	(void)ox;
	(void)oy;
#ifdef INCLUDE_SOUND_TEST
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
#endif
}

//=========================================================================

void dlg_sound_test::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_sound_test::OnPadSelect( ui_win* pWin )
{
	(void)pWin;
#ifdef INCLUDE_SOUND_TEST
    if( pWin == (ui_win*)m_pCancel )
    {
        // Close dialog and step back
        audio_StopAll();
        m_pManager->EndDialog( m_UserID, TRUE );
        // TODO: Play Sound
    }
    else if (pWin == (ui_win*)m_pPlay)
    {
        audio_StopAll();
        audio_Play(m_pSampleList->GetItemData(m_pSampleList->GetSelection()), AUDFLAG_CHANNELSAVER);
    }
#endif
}

//=========================================================================

void dlg_sound_test::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Close dialog and step back
    m_pManager->EndDialog( m_UserID, TRUE );
    // TODO: Play Sound
}

//=========================================================================

void dlg_sound_test::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pData;
    (void)pSender;
    (void)Command;

#ifdef INCLUDE_SOUND_TEST
    // If X pressed in Sound list go directly to "Play"
    if( (pSender == (ui_win*)m_pSampleList) && (Command == WN_LIST_ACCEPTED) )
    {
        audio_StopAll();
        audio_Play(m_pSampleList->GetItemData(m_pSampleList->GetSelection()), AUDFLAG_CHANNELSAVER);
    }

    if( (pSender == (ui_win*)m_pCategoryList) && (Command == WN_LIST_ACCEPTED) )
    {
        FillSampleList(m_pCategoryList->GetSelection());
    }
#endif
}

//=========================================================================

//=========================================================================
void dlg_sound_test::OnUpdate( ui_win* pWin, f32 DeltaTime )
{

    (void)pWin;
    (void)DeltaTime;
    


}

