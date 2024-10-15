//==============================================================================
//  
//  dlg_pauseoptions.hpp
//  
//==============================================================================

#ifndef DLG_PAUSEOPTIONS_HPP
#define DLG_PAUSEOPTIONS_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

//==============================================================================
//  dlg_pauseoptions
//==============================================================================

class ui_check;
class ui_button;

extern void     dlg_pauseoptions_register  ( ui_manager* pManager );
extern ui_win*  dlg_pauseoptions_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_pauseoptions : public ui_dialog
{
public:
                    dlg_pauseoptions    ( void );
    virtual        ~dlg_pauseoptions    ( void );

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

protected:
    ui_check*       m_pSurround;
};

//==============================================================================
#endif // DLG_PAUSEOPTIONS_HPP
//==============================================================================
