//==============================================================================
//  
//  dlg_OnlineJoin.hpp
//  
//==============================================================================

#ifndef DLG_MISSION_LOAD_HPP
#define DLG_MISSION_LOAD_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_mission_load_register( ui_manager* pManager );
extern ui_win*  dlg_mission_load_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;

class dlg_mission_load : public ui_dialog
{
public:
                    dlg_mission_load    ( void );
    virtual        ~dlg_mission_load    ( void );

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
    ui_listbox*     m_pServers;
    ui_button*      m_pBack;
    ui_button*      m_pJoin;
};

//==============================================================================
#endif // DLG_MISSION_LOAD_HPP
//==============================================================================
