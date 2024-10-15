//==============================================================================
//  
//  dlg_MainMenu.hpp
//  
//==============================================================================

#ifndef DLG_MAIN_MENU_HPP
#define DLG_MAIN_MENU_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "ui\ui_frame.hpp"
#include "ui\ui_text.hpp"
#include "dlg_NetworkOptions.hpp"
#include "Demo1/SpecialVersion.hpp"

//==============================================================================
//  dlg_main_menu
//==============================================================================

extern void     dlg_main_menu_register  ( ui_manager* pManager );
extern ui_win*  dlg_main_menu_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_button;

class dlg_main_menu : public ui_dialog
{
public:
                        dlg_main_menu       ( void );
    virtual            ~dlg_main_menu       ( void );

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
    virtual void        OnUpdate            ( ui_win* pWin, f32 DeltaTime );
    void                LaunchNetworkOption ( void );

protected:
    ui_button*          m_pOnlineP1;
    ui_button*          m_pOnlineP2;
    ui_button*          m_pOfflineP1;
    ui_button*          m_pOfflineP2;
    ui_button*          m_pCampaign;
    ui_button*          m_pSoundTest;
    ui_button*          m_pAudio;
    ui_button*          m_pNetwork;
    ui_button*          m_pCredits;

    ui_frame*           m_pOnline;
    ui_frame*           m_pOffline;
    ui_frame*           m_pOpt;

    ui_text*            m_pTXT_Online;
    ui_text*            m_pTXT_Offline;
    ui_text*            m_pTXT_Options;
    s32                 m_ButtonPushed;
    dlg_network_options* m_pNetOptions;
#if defined(TARGET_PC) || defined(DEMO_DISK_HARNESS)
    ui_button*          m_pExitGame;
#endif
//    ui_button*          m_pMemoryCard;
};

//==============================================================================
#endif // DLG_MAIN_MENU_HPP
//==============================================================================
