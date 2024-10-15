//==============================================================================
//  
//  dlg_vehicle.hpp
//  
//==============================================================================

#ifndef DLG_VEHICLE_HPP
#define DLG_VEHICLE_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "objects\player\PlayerObject.hpp"

//==============================================================================
//  dlg_vehicle
//==============================================================================

class ui_check;
class ui_frame;
struct warrior_setup;

extern void     dlg_vehicle_register  ( ui_manager* pManager );
extern ui_win*  dlg_vehicle_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_vehicle : public ui_dialog
{
public:
                            dlg_vehicle             ( void );
    virtual                ~dlg_vehicle             ( void );

    xbool                   Create                  ( s32                       UserID,
                                                      ui_manager*               pManager,
                                                      ui_manager::dialog_tem*   pDialogTem,
                                                      const irect&              Position,
                                                      ui_win*                   pParent,
                                                      s32                       Flags,
                                                      void*                     pUserData );
    virtual void            Destroy                 ( void );

    virtual void            Render                  ( s32 ox=0, s32 oy=0 );
    void                    RenderVehicle           ( irect Window, s32 VehicleType );

    virtual void            OnNotify                ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );
    virtual void            OnPadNavigate           ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void            OnPadSelect             ( ui_win* pWin );
    virtual void            OnPadBack               ( ui_win* pWin );

    void                    VehicleToControls       ( void );
    void                    ControlsToVehicle       ( void );

protected:
    player_object*          m_pPlayer;
    warrior_setup*          m_pWarriorSetup;

    ui_check*               m_pVehicle[4];
    ui_frame*               m_pFrame2;

    s32                     m_BlockNotify;
};

    void                    SetVehicleButtonState   ( s32 StringID, xbool Enabled );

//==============================================================================
#endif // DLG_VEHICLE_HPP
//==============================================================================
