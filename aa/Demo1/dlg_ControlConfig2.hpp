//==============================================================================
//  
//  dlg_ControlConfig.hpp
//  
//==============================================================================

#ifndef DLG_CONTROLCONFIG_HPP
#define DLG_CONTROLCONFIG_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "fe_Globals.hpp"

//==============================================================================
//  dlg_controlconfig
//==============================================================================

extern void     dlg_controlconfig_register( ui_manager* pManager );
extern ui_win*  dlg_controlconfig_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_check;
class ui_slider;
class ui_button;
class ui_text;
class ui_frame;

class dlg_controlconfig : public ui_dialog
{
//    friend class ui_controllist;

public:
                    dlg_controlconfig   ( void );
    virtual        ~dlg_controlconfig   ( void );

    void            SetTimeout          (f32 timeout );
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
    void            UpdateControls      ( void );

protected:
    warrior_setup*      m_pWarriorSetup;
    warrior_setup       m_WarriorBackup;
    ui_button*          m_pOk;
    ui_button*          m_pCancel;

//    ui_controllist*     m_pControlList;

    ui_frame*           m_pControllerFrame;
    ui_combo*           m_pControllerConfig;

    ui_text*            m_pTextSensX;
    ui_text*            m_pTextSensY;
    ui_slider*          m_pSensX;
    ui_slider*          m_pSensY;
    ui_button*          m_pInvertPlayerY;
    ui_button*          m_pInvertVehicleY;

//    ui_button*          m_pRestore;
    ui_combo*          m_pAutoAim;
    ui_check*           m_pDualshock;

    s32                 m_SetDefaultConfig;
    s32                 m_LastDefaultConfig;

    xbool               m_DualshockEnabled;
    s32                 m_SensitivityX;
    s32                 m_SensitivityY;

    xbool               m_InvertPlayerY;
    xbool               m_InvertVehicleY;
};

//==============================================================================
#endif // DLG_CONTROLCONFIG_HPP
//==============================================================================
