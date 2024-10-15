//==============================================================================
//  
//  dlg_ClientLimits.hpp
//  
//==============================================================================

#ifndef DLG_CLIENTLIMITS_HPP
#define DLG_CLIENTLIMITS_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "dlg_message.hpp"

//==============================================================================
//  dlg_clientlimits
//==============================================================================

extern void     dlg_clientlimits_register( ui_manager* pManager );
extern ui_win*  dlg_clientlimits_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_clientlimits : public dlg_message
{
public:
                    dlg_clientlimits    ( void );
    virtual        ~dlg_clientlimits    ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );
//    virtual void    OnPadBack           ( ui_win* pWin );
};

//==============================================================================
#endif // DLG_CLIENTLIMITS_HPP
//==============================================================================
