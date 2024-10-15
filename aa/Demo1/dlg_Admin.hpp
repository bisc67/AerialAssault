//==============================================================================
//  
//  dlg_admin.hpp
//  
//==============================================================================

#ifndef DLG_ADMIN_HPP
#define DLG_ADMIN_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

class ui_button;
class ui_check;
class ui_slider;
class ui_text;

//==============================================================================
//  dlg_admin
//==============================================================================

extern void     dlg_admin_register  ( ui_manager* pManager );
extern ui_win*  dlg_admin_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_admin : public ui_dialog
{
public:
                    dlg_admin           ( void );
    virtual        ~dlg_admin           ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData );
    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );

    virtual void    OnPadNavigate       ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );
    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );

protected:

    u32             GetKickMask         ( void );

    ui_check*       m_pTargetLock;
    ui_button*      m_pNextMap;
    ui_button*      m_pChangeMap;
    ui_button*      m_pTeamchangePlayer;
    ui_button*      m_pKickPlayer;
    ui_button*      m_pShutdownServer;

    ui_check*       m_pEnableBots;
    ui_slider*      m_pSLBotAI;
    ui_slider*      m_pSLMinPlayers;
    ui_slider*      m_pSLMaxPlayers;
    ui_slider*      m_pSLTeamDamage;
    ui_slider*      m_pSLVotePass;
    ui_text*        m_pTXBotAI;
    ui_text*        m_pTXMinPlayers;
    ui_text*        m_pTXMaxPlayers;
    ui_text*        m_pTXTeamDamage;
    ui_text*        m_pTXVotePass;

    s32             m_iChangeMap;
    s32             m_iTeamchangePlayer;
    s32             m_iKickPlayer;
    s32             m_DoShutdownServer;
    s32             m_DoNextMap;
    xbool           m_Updating;
};

//==============================================================================
#endif // DLG_ADMIN_HPP
//==============================================================================
