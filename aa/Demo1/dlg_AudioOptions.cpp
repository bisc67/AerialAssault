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

#include "dlg_AudioOptions.hpp"
#include "dlg_message.hpp"
#include "dlg_loadsave.hpp"
#include "masterserver/masterserver.hpp"
#include "fe_globals.hpp"
#include "globals.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

#include "StringMgr\StringMgr.hpp"


//==============================================================================
// Online Dialog
//==============================================================================

// The navigation grid is a matrix defined in the ui_manager::dialog_tem structure. In this case,
// it's a 3x2 matrix.
// With each button, the 4 values define x,y position within that matrix and width and height
// of the box active area. In the sample below, "Cancel" occupies position (0,1) and is (1,1) wide
// "Connection Types" is at position (0,0) with a size (3,1) and so on.
#define NET_BASE_X 6
#define NET_BASE_Y 235

#define AUD_BASE_X 4
#define AUD_BASE_Y 44

#define BTN_BASE_X 50
#define BTN_BASE_Y 409

enum controls
{
    IDC_MASTER_VOL_SLIDER,
    IDC_EFFECTS_VOL_SLIDER,
    IDC_MUSIC_VOL_SLIDER,
    IDC_VOICE_VOL_SLIDER,
    IDC_MASTER_VOL_TEXT,
    IDC_EFFECTS_VOL_TEXT,
    IDC_MUSIC_VOL_TEXT,
    IDC_VOICE_VOL_TEXT,
    IDC_SURROUND,
    IDC_AUDIO_FRAME,
    IDC_AUDIO_SETTINGS_TEXT,
//    IDC_LOAD,
//    IDC_SAVE,
//    IDC_DONE,
    IDC_CANCEL,
    IDC_OK
};

ui_manager::control_tem AudioOptionsControls[] =
{
    { IDC_CANCEL,                     IDS_CANCEL,                "button",   16, 178,      96, 24, 0,  5, 1, 1, ui_win::WF_VISIBLE },

    // Sliders
    { IDC_MASTER_VOL_SLIDER,          IDS_NULL,                  "slider",  160, 48+24*0, 128, 24, 0,  0, 2, 1, ui_win::WF_VISIBLE },
    { IDC_EFFECTS_VOL_SLIDER,         IDS_NULL,                  "slider",  160, 48+24*1, 128, 24, 0,  1, 2, 1, ui_win::WF_VISIBLE },
    { IDC_MUSIC_VOL_SLIDER,           IDS_NULL,                  "slider",  160, 48+24*2, 128, 24, 0,  2, 2, 1, ui_win::WF_VISIBLE },
    { IDC_VOICE_VOL_SLIDER,           IDS_NULL,                  "slider",  160, 48+24*3, 128, 18, 0,  3, 2, 1, ui_win::WF_VISIBLE },

    // Slider labels
    { IDC_MASTER_VOL_TEXT,            IDS_MASTER_VOL,            "text",      8, 48+24*0, 130, 24, 0,  0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_EFFECTS_VOL_TEXT,           IDS_EFFECTS_VOL,           "text",      8, 48+24*1, 130, 24, 0,  0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_MUSIC_VOL_TEXT,             IDS_MUSIC_VOL,             "text",      8, 48+24*2, 130, 24, 0,  0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_VOICE_VOL_TEXT,             IDS_VOICE_VOL,             "text",      8, 48+24*3, 130, 24, 0,  0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_SURROUND,                   IDS_SURROUND,              "check",    16, 50+24*4, 272, 24, 0,  4, 2, 1, ui_win::WF_VISIBLE },

    // Text labels for onscreen extras
    { IDC_AUDIO_FRAME,                IDS_NULL,                  "frame",   0,     0,     304,210, 0,  0, 0, 0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},

    { IDC_OK,                         IDS_OK,                    "button",  192, 178,      96, 24, 1,  5, 1, 1, ui_win::WF_VISIBLE },

    // Master buttons
//    { IDC_LOAD,                       IDS_LOAD,                  "button",  BTN_BASE_X,     AUD_BASE_Y+14+24*5,      80, 24, 0,  8, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_SAVE,                       IDS_SAVE,                  "button",  BTN_BASE_X+100, AUD_BASE_Y+14+24*5,      80, 24, 1,  8, 1, 1, ui_win::WF_VISIBLE },
//    { IDC_DONE,                       IDS_DONE,                  "button",  BTN_BASE_X+200, AUD_BASE_Y+14+24*5,      80, 24, 2,  8, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem AudioOptionsDialog =
{
    IDS_OPTIONS_TITLE,
    2, 6,
    sizeof(AudioOptionsControls)/sizeof(ui_manager::control_tem),
    &AudioOptionsControls[0],
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
};

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================
//extern server_manager*  pSvrMgr;
s32 SoundID;
xbool  InitDialog = TRUE;

//=========================================================================
//  Registration function
//=========================================================================

void dlg_audio_options_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "audio options", &AudioOptionsDialog, &dlg_audio_options_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_audio_options_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_audio_options* pDialog = new dlg_audio_options;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_options
//=========================================================================

dlg_audio_options::dlg_audio_options( void )
{
}

//=========================================================================

dlg_audio_options::~dlg_audio_options( void )
{
    Destroy();
}

void dlg_audio_options::BackupOptions(void)
{
    m_MasterVolume  = tgl.MasterVolume;
    m_EffectsVolume = tgl.EffectsVolume;
    m_MusicVolume   = tgl.MusicVolume;
    m_VoiceVolume   = tgl.VoiceVolume;
    m_Surround      = fegl.AudioSurround;
}

void dlg_audio_options::RestoreOptions(void)
{
    tgl.MasterVolume   = m_MasterVolume;
    tgl.EffectsVolume  = m_EffectsVolume;
    tgl.MusicVolume    = m_MusicVolume;
    tgl.VoiceVolume    = m_VoiceVolume;
    fegl.AudioSurround = m_Surround;

    audio_SetMasterVolume(tgl.MasterVolume,tgl.MasterVolume);
    audio_SetContainerVolume(tgl.EffectsPackageId,tgl.EffectsVolume);
    audio_SetContainerVolume(tgl.MusicPackageId,tgl.MusicVolume);
    audio_SetContainerVolume(tgl.VoicePackageId,tgl.VoiceVolume);
    audio_SetSurround(fegl.AudioSurround);
}

void dlg_audio_options::ReadOptions(void)
{
	// Go figure!
	// For some reason, the floating point rounding isn't working right.
	// 0.6f * 50.0f is NOT 30.0f it's 29.999997f.

    m_pMasterVol->SetValue ((s32)((tgl.MasterVolume  + 0.01f) *  45.00f));
    m_pEffectsVol->SetValue((s32)((tgl.EffectsVolume + 0.01f) *  58.50f)); // Marc changed to setup volumes, do not touch please!
    m_pMusicVol->SetValue  ((s32)((tgl.MusicVolume   + 0.01f) *  80.00f)); // Marc changed to setup volumes, do not touch please!
    m_pVoiceVol->SetValue  ((s32)((tgl.VoiceVolume   + 0.01f) *  50.00f));

    if (fegl.AudioSurround)
    {
        m_pSurround->SetFlags(m_pSurround->GetFlags() | WF_SELECTED);
    }
    else
    {
        m_pSurround->SetFlags(m_pSurround->GetFlags() & ~WF_SELECTED );
    }

}

void dlg_audio_options::WriteOptions(xbool AllowCloseNetwork)
{
    (void)AllowCloseNetwork;

    f32 tempmaster;
    f32 tempeffects;
    f32 tempmusic;
    f32 tempvoice;

    tempmaster  = m_pMasterVol->GetValue()  /  45.00f;
    tempeffects = m_pEffectsVol->GetValue() /  58.50f; // Marc changed to setup volumes, do not touch please!
    tempmusic   = m_pMusicVol->GetValue()   /  80.00f; // Marc changed to setup volumes, do not touch please!
    tempvoice   = m_pVoiceVol->GetValue()   /  50.00f;

    if (tempmaster != tgl.MasterVolume)
    {
        tgl.MasterVolume   = tempmaster;
        audio_SetMasterVolume(tgl.MasterVolume,tgl.MasterVolume);
		fegl.OptionsModified = TRUE;

    }

    if (tempeffects != tgl.EffectsVolume)
    {
        tgl.EffectsVolume  = tempeffects;
        audio_SetContainerVolume(tgl.EffectsPackageId,tgl.EffectsVolume);
		fegl.OptionsModified = TRUE;
    }

    if (tempmusic != tgl.MusicVolume)
    {
        tgl.MusicVolume    = tempmusic;
        audio_SetContainerVolume(tgl.MusicPackageId,tgl.MusicVolume);
		fegl.OptionsModified = TRUE;
    }

    if (tempvoice != tgl.VoiceVolume)
    {
        tgl.VoiceVolume    = tempvoice;
        audio_SetContainerVolume(tgl.VoicePackageId,tgl.VoiceVolume);
		fegl.OptionsModified = TRUE;
        x_DebugMsg( xfs("Voice=%f\n",tempvoice) );
    }

	xbool	newsurr;

	newsurr = (m_pSurround->GetFlags() & WF_SELECTED)!=0;

	if ( (newsurr != fegl.AudioSurround) )
	{
		fegl.OptionsModified = TRUE;
	}
    fegl.AudioSurround     = newsurr;
    audio_SetSurround(fegl.AudioSurround);

}

//=========================================================================

xbool dlg_audio_options::Create( s32                        UserID,
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

    BackupOptions();

    m_BackgroundColor   = FECOL_DIALOG2;

    m_pMasterVolText    = (ui_text*)   FindChildByID( IDC_MASTER_VOL_TEXT    );
    m_pEffectsVolText   = (ui_text*)   FindChildByID( IDC_EFFECTS_VOL_TEXT   );
    m_pMusicVolText     = (ui_text*)   FindChildByID( IDC_MUSIC_VOL_TEXT     );
    m_pVoiceVolText     = (ui_text*)   FindChildByID( IDC_VOICE_VOL_TEXT     );
    m_pMasterVol        = (ui_slider*) FindChildByID( IDC_MASTER_VOL_SLIDER  );
    m_pEffectsVol       = (ui_slider*) FindChildByID( IDC_EFFECTS_VOL_SLIDER );
    m_pMusicVol         = (ui_slider*) FindChildByID( IDC_MUSIC_VOL_SLIDER   );
    m_pVoiceVol         = (ui_slider*) FindChildByID( IDC_VOICE_VOL_SLIDER   );
    m_pSurround         = (ui_check*)  FindChildByID( IDC_SURROUND           );
//    m_pDone             = (ui_button*) FindChildByID( IDC_DONE               );
//    m_pSave             = (ui_button*) FindChildByID( IDC_SAVE               );
//    m_pLoad             = (ui_button*) FindChildByID( IDC_LOAD               );
    m_pFrame            = (ui_frame*)  FindChildByID( IDC_AUDIO_FRAME        );
    m_pCancel           = (ui_button*) FindChildByID( IDC_CANCEL             );
    m_pOk               = (ui_button*) FindChildByID( IDC_OK                 );

    // Set the label for the frame.
    m_pFrame->EnableTitle( (xwstring)StringMgr( "ui", IDS_AUDIO_SETTINGS ), TRUE );

    m_LastCardStatus    = -1;
    m_CardScanDelay     = 0.25f;            // Allow a short time for all the dialogs to appear before hitting memcard
    m_HasChanged        = FALSE;

    m_pMasterVolText ->SetLabelFlags( ui_font::h_right|ui_font::v_center );
    m_pEffectsVolText->SetLabelFlags( ui_font::h_right|ui_font::v_center );
    m_pMusicVolText  ->SetLabelFlags( ui_font::h_right|ui_font::v_center );
    m_pVoiceVolText  ->SetLabelFlags( ui_font::h_right|ui_font::v_center );

    m_pMasterVol ->SetRange(0,50);
    m_pEffectsVol->SetRange(0,50);
    m_pMusicVol  ->SetRange(0,50);
    m_pVoiceVol  ->SetRange(0,50);

    m_pMasterVol ->SetStep(1, 2);
    m_pEffectsVol->SetStep(1, 2);
    m_pMusicVol  ->SetStep(1, 2);
    m_pVoiceVol  ->SetStep(1, 2);

    ReadOptions();
    
    m_pMessage = NULL;

    InitDialog = FALSE;

    // Return success code
    return Success;
}

//=========================================================================

void dlg_audio_options::Destroy( void )
{
    ui_dialog::Destroy();
    InitDialog = TRUE;
    audio_Stop( SoundID );
}

//=========================================================================

void dlg_audio_options::Render( s32 ox, s32 oy )
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

void dlg_audio_options::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_audio_options::OnPadSelect( ui_win* pWin )
{
    // Check for OK button
    if( pWin == (ui_win*)m_pOk )
    {
        WriteOptions(TRUE);

        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        m_pManager->EndDialog( m_UserID, TRUE );

		if (fegl.OptionsModified)
		{
			load_save_params options(0,(load_save_mode)(SAVE_AUDIO_OPTIONS|ASK_FIRST),NULL);
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
    else if (pWin == (ui_win*)m_pCancel )
    {
        RestoreOptions();
		fegl.OptionsModified = FALSE;

        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        m_pManager->EndDialog( m_UserID, TRUE );
    }

/*
    else if (pWin == (ui_win *)m_pLoad)
    {
        load_save_params options(0,LOAD_AUDIO_OPTIONS,this);
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
        load_save_params options(0,SAVE_AUDIO_OPTIONS,this);
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

void dlg_audio_options::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    RestoreOptions();
	fegl.OptionsModified = FALSE;

    // Close dialog and step back
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    m_pManager->EndDialog( m_UserID, TRUE );

/*
    WriteOptions(TRUE);
	if (fegl.OptionsModified)
	{
		load_save_params options(0,(load_save_mode)(SAVE_AUDIO_OPTIONS|ASK_FIRST),NULL);
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

void dlg_audio_options::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pData;
    (void)pSender;

    if( pSender == m_pVoiceVol && !InitDialog )
    {
        audio_Stop( SoundID );
        SoundID = audio_Play( SFX_VOICE_MALE_1 + x_irand( 56, 59 ), AUDFLAG_CHANNELSAVER );
    }
    
    if (Command == WN_USER)
    {
        ReadOptions();
    }
}

//=========================================================================

void dlg_audio_options::UpdateControls( void )
{
}

//=========================================================================
void dlg_audio_options::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
//    s32 state,Id;

    (void)pWin;
    (void)DeltaTime;
    WriteOptions(FALSE);
}

//=========================================================================
void dlg_audio_options::WarningBox(const xwchar *pTitle,const xwchar *pMessage,const xwchar* pButton)
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
                                                        irect(0,0,300,200),
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

