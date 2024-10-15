//==============================================================================
//  
//  dlg_OnlineJoin.hpp
//  
//==============================================================================

#ifndef DLG_ONLINE_JOIN_HPP
#define DLG_ONLINE_JOIN_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "savedgame.hpp"
#include "serverman.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_online_join_register( ui_manager* pManager );
extern ui_win*  dlg_online_join_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;
class ui_joinlist;

class dlg_online_join : public ui_dialog
{
public:
                    dlg_online_join     ( void );
    virtual        ~dlg_online_join     ( void );

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
    virtual void    OnPadShoulder       ( ui_win* pWin, s32 Direction );
    virtual void    OnPadShoulder2      ( ui_win* pWin, s32 Direction );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );

    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );
    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );

protected:
    void            UpdateControls      ( void );
    void            FillServerList      ( void );
    void            AddServerToList     ( const server_info *pServer, const xcolor &color );
    void            PopulateServerInfo  ( const server_info *pServerInfo );
    xbool           ApplyFilterRules    ( const server_info *pServerInfo );
	void			ForceRefresh		( void );
	void			Purge				( void );

protected:
    ui_joinlist*    m_pServerList;
//    ui_listbox*     m_pServers;
//    ui_listbox*     m_pServerName;
//    ui_listbox*     m_pPlayerCount;
//    ui_listbox*     m_pGameType;
//    ui_listbox*     m_pIcons;

    ui_button*      m_pBack;
    ui_button*      m_pJoin;
//    ui_button*      m_pSetFilter;
    ui_button*      m_pRefresh;
    ui_combo*       m_pSort;
//    ui_check*       m_pEnableFilter;
    s32             m_LocalServerCount;
	s32				m_OldEnableValue;

    xarray<server_info*>    m_SortList;

    ui_text*        m_pInfoPlayers;
    ui_text*        m_pInfoBots;
    ui_text*        m_pInfoMaxPlayers;
    ui_text*        m_pInfoServerName;
    ui_text*        m_pInfoHasBuddy;
    ui_text*        m_pInfoHasPassword;
    ui_text*        m_pInfoResponse;
    ui_text*        m_pInfoComplete;
	ui_text*		m_pInfoClients;
	ui_text*		m_pInfoTargetLock;
	ui_text*		m_pInfoModem;
	ui_text*		m_pServerCount;

    xwstring        m_JoinPassword;
};

//==============================================================================
#endif // DLG_ONLINE_JOIN_HPP
//==============================================================================
