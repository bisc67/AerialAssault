//==============================================================================
//  
//  ui_combo.hpp
//  
//==============================================================================

#ifndef UI_COMBO_HPP
#define UI_COMBO_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_TYPES_HPP
#include "x_types.hpp"
#include "x_math.hpp"
#endif

#include "ui_control.hpp"

//==============================================================================
//  ui_combo
//==============================================================================

#define COMBO_DATA_FIELDS   2

extern ui_win* ui_combo_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags );

class ui_combo : public ui_control
{
    struct item
    {
        xwstring    Label;
        xbool       Enabled;
        s32         Data[COMBO_DATA_FIELDS];
    };

public:
                    ui_combo                ( void );
    virtual        ~ui_combo                ( void );

    xbool           Create                  ( s32           UserID,
                                              ui_manager*   pManager,
                                              const irect&  Position,
                                              ui_win*       pParent,
                                              s32           Flags );

    virtual void    Render                  ( s32 ox=0, s32 oy=0 );

    virtual void    OnCursorEnter           ( ui_win* pWin );
    virtual void    OnPadNavigate           ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void    OnPadSelect             ( ui_win* pWin );
    virtual void    OnPadShoulder           ( ui_win* pWin, s32 Direction );
    virtual void    OnLBDown                ( ui_win* pWin );


    void            SetLabelWidth           ( s32 Width );

    s32             AddItem                 ( const xwstring& Item, s32 Data1 = 0, s32 Data2 = 0 );
    s32             AddItem                 ( const xwchar*   Item, s32 Data1 = 0, s32 Data2 = 0 );
    void            SetItemEnabled          ( s32 iItem, xbool State );
    void            DeleteAllItems          ( void );
    void            DeleteItem              ( s32 iItem );

    s32             GetItemCount            ( void ) const;
    const xwstring& GetItemLabel            ( s32 iItem ) const;
    s32             GetItemData             ( s32 iItem, s32 Index ) const;
    const xwstring& GetSelectedItemLabel    ( void ) const;
    s32             GetSelectedItemData     ( s32 Index = 0 ) const;

    s32             FindItemByLabel         ( const xwstring& Label );
    s32             FindItemByData          ( s32 Data, s32 Index = 0 );

    s32             GetSelection            ( void ) const;
    void            SetSelection            ( s32 iSelection );
    void            ClearSelection          ( void );

    void            EnablePopupList         ( xbool State );

protected:
    s32             m_iElement1;
    s32             m_iElement2;
    s32             m_iElementb;
    s32             m_LabelWidth;
    xarray<item>    m_Items;
    s32             m_iSelection;
};

//==============================================================================
#endif // UI_COMBO_HPP
//==============================================================================
