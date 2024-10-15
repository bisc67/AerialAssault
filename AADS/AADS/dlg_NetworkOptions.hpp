//==============================================================================
//  
//  dlg_Options.hpp
//  
//==============================================================================

#ifndef DLG_NETWORK_OPTIONS_HPP
#define DLG_NETWORK_OPTIONS_HPP

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

extern void     dlg_network_options_register( ui_manager* pManager );
extern ui_win*  dlg_network_options_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;

class dlg_network_options : public ui_dialog
{
public:
                    dlg_network_options    ( void );
    virtual        ~dlg_network_options    ( void );

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
    s32             GetDialogState      ( void ) { return m_DialogState; }

protected:
    void            UpdateControls      ( void );
    void            WarningBox          (const xwchar *pTitle,const xwchar *pMessage,const xwchar* pButton = xwstring("DONE"));
    void            UpdateConnectStatus (f32 DeltaTime);
    void            ReadOptions         ( void );
    void            WriteOptions        ( xbool AllowCloseNetwork );
	void			OptionBox			( const char* pTitle,const char* pMessage, const xwchar *pYes, const xwchar *pNo,const xbool DefaultToNo );

protected:
    ui_listbox*     m_pConnectionList;
    ui_button*      m_pConnect;
    ui_check*       m_pAutoConnect;

//    ui_button*      m_pLoad;
//    ui_button*      m_pSave;
//    ui_button*      m_pDone;
    ui_button*      m_pOk;
    ui_button*      m_pCancel;
	ui_button*		m_pModify;

    ui_frame*       m_pFrame;

    xbool           m_IsConnected;
    net_config_list m_ConfigList;
    f32             m_CardScanDelay;
    s32             m_LastCardStatus;
    s32             m_LastCardPresent;
    s32             m_DialogState;
    f32             m_MessageDelay;
    s32             m_MessageResult;
	s32				m_CardWithConfig;
	s32				m_CardStatus[2];
    char            m_MessageBuffer[128];
    s32             m_ActiveInterfaceId;
    dlg_message     *m_pMessage;
    xbool           m_HasChanged;

};

//==============================================================================
#endif // dlg_options_HPP
//==============================================================================
