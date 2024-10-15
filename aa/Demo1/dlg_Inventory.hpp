//==============================================================================
//  
//  dlg_inventory.hpp
//  
//==============================================================================

#ifndef DLG_INVENTORY_HPP
#define DLG_INVENTORY_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "objects\player\PlayerObject.hpp"

//==============================================================================
//  dlg_inventory
//==============================================================================

class ui_combo;
class ui_frame;
class ui_check;
struct warrior_setup;

extern void     dlg_inventory_register  ( ui_manager* pManager );
extern ui_win*  dlg_inventory_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_inventory : public ui_dialog
{
public:
                            dlg_inventory           ( void );
    virtual                ~dlg_inventory           ( void );

    xbool                   Create                  ( s32                       UserID,
                                                      ui_manager*               pManager,
                                                      ui_manager::dialog_tem*   pDialogTem,
                                                      const irect&              Position,
                                                      ui_win*                   pParent,
                                                      s32                       Flags,
                                                      void*                     pUserData );
    virtual void            Destroy                 ( void );

    virtual void            Render                  ( s32 ox=0, s32 oy=0 );

    virtual void            OnNotify                ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );
    virtual void            OnPadNavigate           ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void            OnPadSelect             ( ui_win* pWin );
    virtual void            OnPadBack               ( ui_win* pWin );

    void                    SetupWeaponCombo        ( ui_combo* pCombo, const player_object::move_info& MoveInfo );
    void                    AddWeaponToCombo        ( ui_combo* pCombo, const char* Name, s32 Type, const player_object::move_info& MoveInfo );
    void                    RemoveWeaponFromCombo   ( ui_combo* pCombo, s32 Type );

    void                    SetupPackCombo          ( ui_combo* pCombo, const player_object::move_info& MoveInfo );

    void                    LoadoutToControls       ( void );
    void                    ControlsToLoadout       ( void );

protected:
    player_object*          m_pPlayer;
    warrior_setup*          m_pWarriorSetup;
    player_object::loadout* m_pLoadout;

    ui_combo*               m_pFavArmor;
    ui_check*               m_pFav[5];

    ui_frame*               m_pFrame2;

    ui_combo*               m_pWeapon[5];
    ui_combo*               m_pPack;
    ui_combo*               m_pGrenade;

    s32                     m_BlockNotify;
    s32                     m_WarriorID;
};

//==============================================================================
#endif // DLG_INVENTORY_HPP
//==============================================================================
