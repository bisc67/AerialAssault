//==============================================================================
//  
//  ui_controllist.hpp
//  
//==============================================================================

#ifndef UI_CONTROLLIST_HPP
#define UI_CONTROLLIST_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_TYPES_HPP
#include "x_types.hpp"
#include "x_math.hpp"
#endif

#include "ui/ui_listbox.hpp"
#include "fe_Globals.hpp"

//==============================================================================
//  ui_controllist
//==============================================================================

extern void ui_controllist_register( ui_manager* pManager );
extern ui_win* ui_controllist_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags );

class ui_controllist : public ui_listbox
{
    friend class dlg_ControlConfig;

public:
                    ui_controllist          ( void );
    virtual        ~ui_controllist          ( void );

    xbool           Create                  ( s32           UserID,
                                              ui_manager*   pManager,
                                              const irect&  Position,
                                              ui_win*       pParent,
                                              s32           Flags );

    virtual void    Render                  ( s32 ox=0, s32 oy=0 );
    virtual void    RenderItem              ( irect r, const item& Item, const xcolor& c1, const xcolor& c2 );
    virtual void    OnPadSelect             ( ui_win* pWin );
    virtual void    OnPadBack               ( ui_win* pWin );

    void                    SetDefault      ( s32 iDefault );
    warrior_control_data*   GetControlData  ( void );

protected:
    warrior_setup*  m_pWarriorSetup;
    s32             m_NewKeyBinding;
};

//==============================================================================
#endif // UI_CONTROLLIST_HPP
//==============================================================================
