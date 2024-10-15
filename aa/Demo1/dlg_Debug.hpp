//==============================================================================
//  
//  dlg_debug.hpp
//  
//==============================================================================

#ifndef DLG_DEBUG_HPP
#define DLG_DEBUG_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

class ui_button;
class ui_check;

//==============================================================================
//  dlg_debug
//==============================================================================

extern void     dlg_debug_register  ( ui_manager* pManager );
extern ui_win*  dlg_debug_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_debug : public ui_dialog
{
public:
                    dlg_debug           ( void );
    virtual        ~dlg_debug           ( void );

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
    ui_button*      m_pNextMap;
    ui_button*      m_pLeaveGame;
    ui_button*      m_pAutoWin;

    ui_check*       m_pDebugInfo;
    ui_check*       m_pHideHUD;
    ui_check*       m_pInvincible;
    ui_check*       m_pHumanTeam;

    ui_check*       m_pAutoAimHint;
    ui_check*       m_pAutoAimHelp;
    ui_check*       m_pManiacMode;
    ui_check*       m_pUnlimitedAmmo;
    ui_check*       m_pFlickerTerrainHole;
    ui_check*       m_pFlickerIndoorTerrain;
    ui_check*       m_pDoBlending;
    ui_check*       m_pShowBlending;
    ui_button*      m_pChangeTeam;
};

//==============================================================================
#endif // DLG_DEBUG_HPP
//==============================================================================
