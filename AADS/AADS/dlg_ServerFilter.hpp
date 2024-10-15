#ifndef _DLG_SERVERFILTER_HPP
#define _DLG_SERVERFILTER_HPP

//==============================================================================
//  
//  DLG_SERVERFILTER.hpp
//  
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "savedgame.hpp"
#include "dlg_message.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_server_filter_register( ui_manager* pManager );
extern ui_win*  dlg_server_filter_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;

class dlg_server_filter : public ui_dialog
{
public:
                    dlg_server_filter   ( void );
    virtual        ~dlg_server_filter   ( void );

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
    void            UpdateControls      (void);
    void            ResetFilter         (void);
//    void            ReadFilter          (const saved_filter* pFilter);
//    void            WriteFilter         (saved_filter* pFilter);
    ui_text*        m_pMaxPlayerText;
    ui_text*        m_pMinPlayerText;
    ui_slider*      m_pMaxPlayerSlider;
    ui_slider*      m_pMinPlayerSlider;
    ui_edit*        m_pBuddyString;
    ui_button*      m_pDone;
    ui_button*      m_pReset;
    ui_button*      m_pLoad;
    ui_button*      m_pSave;
    ui_combo*       m_pGameType;

	xbool			m_WaitingOnSave;
	xbool			m_SaveComplete;
    
//    saved_filter*   m_pFilter;

};

#endif // _DLG_SERVERFILTER_HPP
