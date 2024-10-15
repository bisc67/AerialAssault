//==============================================================================
//  
//  dlg_buddies.hpp
//  
//==============================================================================

#ifndef DLG_BUDDIES_HPP
#define DLG_BUDDIES_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "fe_globals.hpp"

//==============================================================================
//  dlg_debrief
//==============================================================================

class ui_combo;
class ui_listbox;
class ui_button;
class ui_frame;
class ui_textbox;

extern void     dlg_buddies_register  ( ui_manager* pManager );
extern ui_win*  dlg_buddies_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags,void *pUserData );

class dlg_buddies : public ui_dialog
{
public:
    void            BackupBuddies       ( void );
    void            RestoreBuddies      ( void );

                    dlg_buddies         ( void );
    virtual        ~dlg_buddies         ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData);
    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );
    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );

protected:
    s32             m_InUpdate;
    
    ui_button*      m_pOk;
    ui_button*      m_pCancel;
    ui_button*      m_pAdd;
    ui_button*      m_pEdit;
    ui_button*      m_pDelete;

    ui_listbox*     m_pList;

    warrior_setup*  m_pWarriorSetup;
    s32             m_OldnBuddies;
    xwchar          m_OldBuddies[FE_MAX_WARRIOR_NAME][FE_MAX_BUDDIES];

    s32             m_iSelectedItem;
    xwstring        m_BuddyName;
    xbool           m_AddNameDone;
    xbool           m_EditNameDone;
    xbool           m_KeyboardOk;
    xbool           m_DoDelete;
};

//==============================================================================
#endif // DLG_BUDDIES_HPP
//==============================================================================
