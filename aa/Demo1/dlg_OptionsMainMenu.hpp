//==============================================================================
//  
//  dlg_MainMenu.hpp
//  
//==============================================================================

#ifndef DLG_OPTIONS_MAIN_MENU_HPP
#define DLG_OPTIONS_MAIN_MENU_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "ui\ui_frame.hpp"

//==============================================================================
//  dlg_main_menu
//==============================================================================

extern void     dlg_options_main_menu_register  ( ui_manager* pManager );
extern ui_win*  dlg_options_main_menu_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_button;

class dlg_options_main_menu : public ui_dialog
{
public:
                        dlg_options_main_menu       ( void );
    virtual            ~dlg_options_main_menu       ( void );

    xbool               Create              ( s32                       UserID,
                                              ui_manager*               pManager,
                                              ui_manager::dialog_tem*   pDialogTem,
                                              const irect&              Position,
                                              ui_win*                   pParent,
                                              s32                       Flags,
                                              void*                     pUserData);
    virtual void        Destroy             ( void );

    virtual void        Render              ( s32 ox=0, s32 oy=0 );

    virtual void        OnPadSelect         ( ui_win* pWin );
    virtual void        OnPadBack           ( ui_win* pWin );
protected:
    ui_button*          m_pConnection;
    ui_button*          m_pAudio;
    ui_button*          m_pNetwork;
    ui_button*          m_pCredits;
    ui_button*          m_pBack;
};

//==============================================================================
#endif // DLG_MAIN_MENU_HPP
//==============================================================================
