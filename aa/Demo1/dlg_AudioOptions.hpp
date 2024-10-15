//==============================================================================
//  
//  dlg_Options.hpp
//  
//==============================================================================

#ifndef DLG_AUDIO_OPTIONS_HPP
#define DLG_AUDIO_OPTIONS_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "ui\ui_frame.hpp"
#include "dlg_message.hpp"
#include "support/netlib/netlib.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_audio_options_register( ui_manager* pManager );
extern ui_win*  dlg_audio_options_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;

class dlg_audio_options : public ui_dialog
{
public:
                    dlg_audio_options   ( void );
    virtual        ~dlg_audio_options   ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData );

    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

    virtual void    OnPadNavigate       ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );

    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );
    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );


protected:
    void            UpdateControls      ( void );
    void            WarningBox          (const xwchar *pTitle,const xwchar *pMessage,const xwchar* pButton = xwstring("DONE"));
    void            UpdateConnectStatus (f32 DeltaTime);

    void            BackupOptions       ( void );
    void            RestoreOptions      ( void );
    void            ReadOptions         ( void );
    void            WriteOptions        ( xbool AllowCloseNetwork );

protected:
    ui_text*        m_pMasterVolText;
    ui_text*        m_pEffectsVolText;
    ui_text*        m_pMusicVolText;
    ui_text*        m_pVoiceVolText;
    ui_slider*      m_pMasterVol;
    ui_slider*      m_pEffectsVol;
    ui_slider*      m_pMusicVol;
    ui_slider*      m_pVoiceVol;
    ui_check*       m_pSurround;
    ui_frame*       m_pFrame;

//    ui_button*      m_pLoad;
//    ui_button*      m_pSave;
//    ui_button*      m_pDone;

    ui_button*      m_pCancel;
    ui_button*      m_pOk;

    f32             m_CardScanDelay;
    s32             m_LastCardStatus;
    s32             m_LastCardPresent;
    f32             m_MessageDelay;
    s32             m_MessageResult;
    s32             m_ActiveInterfaceId;
    dlg_message     *m_pMessage;
    xbool           m_HasChanged;

    f32             m_MasterVolume;
    f32             m_EffectsVolume;
    f32             m_MusicVolume;
    f32             m_VoiceVolume;
    xbool           m_Surround;
};

//==============================================================================
#endif // dlg_options_HPP
//==============================================================================
