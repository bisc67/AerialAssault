//==============================================================================
//  
//  ui_button.hpp
//  
//==============================================================================

#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_TYPES_HPP
#include "x_types.hpp"
#include "x_math.hpp"
#endif

#include "ui_control.hpp"

//==============================================================================
//  ui_button
//==============================================================================

extern ui_win* ui_button_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags );

class ui_button : public ui_control
{
public:
                    ui_button           ( void );
    virtual        ~ui_button           ( void );

    xbool           Create              ( s32           UserID,
                                          ui_manager*   pManager,
                                          const irect&  Position,
                                          ui_win*       pParent,
                                          s32           Flags );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );
    virtual void    OnUpdate            ( f32 DeltaTime );

protected:
    s32             m_iElement;
};

//==============================================================================
#endif // UI_BUTTON_HPP
//==============================================================================
