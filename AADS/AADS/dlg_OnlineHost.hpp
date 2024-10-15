//==============================================================================
//  
//  dlg_OnlineHost.hpp
//  
//==============================================================================

#ifndef DLG_ONLINE_HOST_HPP
#define DLG_ONLINE_HOST_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_online_host_register( ui_manager* pManager );
extern ui_win*  dlg_online_host_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;

class dlg_online_host : public ui_dialog
{
public:
                    dlg_online_host     ( void );
    virtual        ~dlg_online_host     ( void );

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
    void            UpdateControls      ( void );
    void            FillMissionsList    ( void );
	void			WriteOptions		( void );
	void			ReadOptions			( void );

protected:
    ui_combo*       m_pGameType;
    ui_listbox*     m_pMissions;
    ui_edit*        m_pServerName;
    ui_edit*        m_pServerPassword;
    ui_slider*      m_pMaxPlayers;
    ui_slider*      m_pMaxClients;
    ui_text*        m_pTeamDamage;
    ui_slider*      m_pTeamDamageSlider;

//    ui_button*      m_pAdvanced;
    ui_button*      m_pBack;
//    ui_button*      m_pLoad;
//    ui_button*      m_pSave;
    ui_button*      m_pHost;

    ui_check*       m_pEnableTargetLock;
    ui_check*       m_pEnableBots;

    ui_text*        m_pNumBotsText;
    ui_slider*      m_pNumBotsSlider;

    ui_text*        m_pBotsAIText;
    ui_slider*      m_pBotsAISlider;

    ui_text*        m_pTextName;
    ui_text*        m_pTextPass;
    ui_text*        m_pTextPlayers;
    ui_text*        m_pTextClients;

    ui_text*        m_pTextTimeLimit;
    ui_slider*      m_pTimeLimitSlider;
    ui_text*        m_pTextVotePass;
    ui_slider*      m_pVotePassSlider;

    s32             m_DisableUpdate;
    xbool           m_WaitingOnSave;
    xbool           m_WaitingOnClose;

};

//==============================================================================
#endif // DLG_ONLINE_HOST_HPP
//==============================================================================
