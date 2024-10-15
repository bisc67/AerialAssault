//==============================================================================
//  
//  dlg_Message.hpp
//  
//==============================================================================

#ifndef DLG_MESSAGE_HPP
#define DLG_MESSAGE_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

enum
{
    DLG_MESSAGE_IDLE,
    DLG_MESSAGE_YES,
    DLG_MESSAGE_NO,
    DLG_MESSAGE_BACK,
};

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_message_register( ui_manager* pManager );
extern ui_win*  dlg_message_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_button;
class ui_text;

class dlg_message : public ui_dialog
{
public:
                    dlg_message         ( void );
    virtual        ~dlg_message         ( void );

    void            Configure           ( const xwchar*             Title,
                                          const xwchar*             Yes,
                                          const xwchar*             No,
                                          const xwchar*             Message,
                                          const xcolor              MessageColor,
                                          s32*                      pResult = NULL,
                                          const xbool               DefaultToNo = TRUE,
										  const xbool				AllowCancel = FALSE);

    void            SetTimeout          (f32 timeout );
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
    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime);

protected:
    void            UpdateControls      ( void );

protected:
    ui_button*      m_pYes;
    ui_button*      m_pNo;

    xwstring        m_Message;
    xcolor          m_MessageColor;
    s32             *m_pResult;
    f32             m_Timeout;
	xbool			m_AllowCancel;
};

//==============================================================================
#endif // DLG_MESSAGE_HPP
//==============================================================================
