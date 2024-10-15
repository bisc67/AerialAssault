//==============================================================================
//  
//  dlg_Online.hpp
//  
//==============================================================================

#ifndef DLG_ONLINE_HPP
#define DLG_ONLINE_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "ui\ui_tabbed_dialog.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_online_register ( ui_manager* pManager );
extern ui_win*  dlg_online_factory  ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_online : public ui_dialog
{
public:
                    dlg_online          ( void );
    virtual        ~dlg_online          ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData );

    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );
    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );

protected:
    ui_tabbed_dialog*       m_pTabbed;
    ui_dialog*              m_pJoin;
    ui_dialog*              m_pHost;
};

//==============================================================================
#endif // DLG_ONLINE_HPP
//==============================================================================
