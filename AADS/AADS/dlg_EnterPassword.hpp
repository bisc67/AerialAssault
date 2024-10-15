//==============================================================================
//  
//  dlg_enterpassword.hpp
//  
//==============================================================================

#ifndef DLG_ENTERPASSWORD_HPP
#define DLG_ENTERPASSWORD_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

//==============================================================================
//  dlg_enterpassword
//==============================================================================

extern void     dlg_enterpassword_register( ui_manager* pManager );
extern ui_win*  dlg_enterpassword_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_button;
class ui_text;
class ui_edit;

class dlg_enterpassword : public ui_dialog
{
public:
                    dlg_enterpassword   ( void );
    virtual        ~dlg_enterpassword   ( void );

    void            Configure           ( const xwchar*             Title,
                                          const xwchar*             Yes,
                                          const xwchar*             No,
                                          const xwchar*             Message,
                                          const xcolor              MessageColor,
                                          s32*                      pResult = NULL );

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
    xwstring        m_JoinPassword;

    ui_button*      m_pOk;
    ui_button*      m_pCancel;
    ui_edit*        m_pPassword;
};

//==============================================================================
#endif // DLG_ENTERPASSWORD_HPP
//==============================================================================
