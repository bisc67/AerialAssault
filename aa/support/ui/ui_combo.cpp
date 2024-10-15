//=========================================================================
//
//  ui_combo.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "..\AudioMgr\audio.hpp"
#include "..\LabelSets\Tribes2Types.hpp"

#include "ui_combo.hpp"
#include "ui_manager.hpp"
#include "ui_font.hpp"
#include "ui_listbox.hpp"
#include "ui_dlg_list.hpp"
//#include "ui_dlg_combolist.hpp"

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Factory function
//=========================================================================

ui_win* ui_combo_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    ui_combo* pcombo = new ui_combo;
    pcombo->Create( UserID, pManager, Position, pParent, Flags );

    return (ui_win*)pcombo;
}

//=========================================================================
//  ui_combo
//=========================================================================

ui_combo::ui_combo( void )
{
}

//=========================================================================

ui_combo::~ui_combo( void )
{
    Destroy();
}

//=========================================================================

xbool ui_combo::Create( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    xbool   Success;

    Success = ui_control::Create( UserID, pManager, Position, pParent, Flags );

    // Initialize data
    m_iElement1 = m_pManager->FindElement( "button_combo1" );
    m_iElement2 = m_pManager->FindElement( "button_combo2" );
    m_iElementb = m_pManager->FindElement( "button_combob" );
    ASSERT( m_iElement1 != -1 );
    ASSERT( m_iElement2 != -1 );
    ASSERT( m_iElementb != -1 );
    m_iSelection        = -1;
    m_LabelWidth        = 0;

    return Success;
}

//=========================================================================

void ui_combo::Render( s32 ox, s32 oy )
{
    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        xcolor  LabelColor1 = XCOLOR_WHITE;
        xcolor  LabelColor2 = XCOLOR_BLACK;
        xcolor  TextColor1  = XCOLOR_WHITE;
        xcolor  TextColor2  = XCOLOR_BLACK;
        s32     State       = 0;

        // Calculate rectangle
        irect    br;
        irect    r, r2;
        br.Set( (m_Position.l+ox), (m_Position.t+oy), (m_Position.r+ox), (m_Position.b+oy) );
        r = br;
        r2 = r;
        r.r = r.l + m_LabelWidth;
        r2.l = r.r;

        // Render appropriate state
        if( m_Flags & WF_DISABLED )
        {
            State = ui_manager::CS_DISABLED;
            LabelColor1 = XCOLOR_GREY;
            LabelColor2 = XCOLOR_BLACK;
            TextColor1  = XCOLOR_GREY;
            TextColor2  = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_HIGHLIGHT )
        {
            State = ui_manager::CS_HIGHLIGHT;
            LabelColor1 = XCOLOR_WHITE;
            LabelColor2 = XCOLOR_BLACK;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_SELECTED )
        {
            State = ui_manager::CS_SELECTED;
            LabelColor1 = XCOLOR_WHITE;
            LabelColor2 = XCOLOR_BLACK;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == (WF_HIGHLIGHT|WF_SELECTED) )
        {
            State = ui_manager::CS_HIGHLIGHT_SELECTED;
            LabelColor1 = XCOLOR_WHITE;
            LabelColor2 = XCOLOR_BLACK;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else
        {
            State = ui_manager::CS_NORMAL;
            LabelColor1 = XCOLOR_WHITE;
            LabelColor2 = XCOLOR_BLACK;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }

        if( m_iSelection != -1 )
        {
            if( !m_Items[m_iSelection].Enabled )
            {
                TextColor1  = XCOLOR_GREY;
                TextColor2  = XCOLOR_BLACK;
            }
        }

        // Add Highlight to list
        if( m_Flags & WF_HIGHLIGHT )
            m_pManager->AddHighlight( m_UserID, br );

        // Render Button
        if( m_LabelWidth > 0 )
        {
            // Render 2 field Combo
            m_pManager->RenderElement( m_iElement1, r,  State );
            m_pManager->RenderElement( m_iElement2, r2, State );

            // Render Label Text
            r.Translate( 1, -1 );
            m_pManager->RenderText( m_Font, r, ui_font::h_center|ui_font::v_center, LabelColor2, m_Label );
            r.Translate( -1, -1 );
            m_pManager->RenderText( m_Font, r, ui_font::h_center|ui_font::v_center, LabelColor1, m_Label );
        }
        else
        {
            // Render single field Combo
            m_pManager->RenderElement( m_iElementb, r2, State );
        }

        // Render Selection Text
        if( m_iSelection != -1 )
        {
            r2.Translate( 1, -2 );
            m_pManager->RenderText( m_Font, r2, ui_font::h_center|ui_font::v_center, TextColor2, m_Items[m_iSelection].Label );
            r2.Translate( -1, -1 );
            m_pManager->RenderText( m_Font, r2, ui_font::h_center|ui_font::v_center, TextColor1, m_Items[m_iSelection].Label );
        }

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}

//=========================================================================

void ui_combo::OnCursorEnter( ui_win* pWin )
{
    ui_control::OnCursorEnter( pWin );

    if( m_pParent )
        m_pParent->OnNotify( m_pParent, this, WN_COMBO_SELCHANGE, (void*)m_iSelection );
}

//=========================================================================

void ui_combo::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // Pass up chain
    if( m_pParent )
        m_pParent->OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void ui_combo::OnPadSelect( ui_win* pWin )
{
    // Move Forward in List
    OnPadShoulder( pWin, 1 );
}

//=========================================================================

void ui_combo::OnPadShoulder( ui_win* pWin, s32 Direction )
{
    (void)pWin;

    // Apply movement
    if( Direction != 0 )
    {
        s32 OldSelection = m_iSelection;

        m_iSelection += Direction;
        if( m_iSelection < 0 )
            m_iSelection = m_Items.GetCount()-1;
        if( m_iSelection >= m_Items.GetCount() )
            m_iSelection = 0;
        if( m_Items.GetCount() == 0 )
            m_iSelection = -1;

        if( (m_iSelection != OldSelection) && m_pParent )
        {
            m_pParent->OnNotify( m_pParent, this, WN_COMBO_SELCHANGE, (void*)m_iSelection );
            audio_Play( SFX_FRONTEND_CURSOR_MOVE_02,AUDFLAG_CHANNELSAVER );
        }
    }
}

//=========================================================================

void ui_combo::SetLabelWidth( s32 Width )
{
    m_LabelWidth = Width;
}

//=========================================================================

s32 ui_combo::AddItem( const xwstring& Label, s32 Data1, s32 Data2 )
{
    item& Item = m_Items.Append();
    Item.Label = Label;
    Item.Enabled = TRUE;
    Item.Data[0] = Data1;
    Item.Data[1] = Data2;
    return m_Items.GetCount()-1;
}

//=========================================================================

s32 ui_combo::AddItem( const xwchar* Label, s32 Data1, s32 Data2 )
{
    item& Item = m_Items.Append();
    Item.Label = Label;
    Item.Enabled = TRUE;
    Item.Data[0] = Data1;
    Item.Data[1] = Data2;
    return m_Items.GetCount()-1;
}

//=========================================================================

void ui_combo::SetItemEnabled( s32 iItem, xbool State )
{
    (void)iItem;
    (void)State;

    m_Items[iItem].Enabled = State;
}

//=========================================================================

void ui_combo::DeleteAllItems( void )
{
    m_iSelection = -1;
    m_Items.Delete( 0, m_Items.GetCount() );

    if( m_pParent )
        m_pParent->OnNotify( m_pParent, this, WN_COMBO_SELCHANGE, (void*)m_iSelection );
}

//=========================================================================

void ui_combo::DeleteItem( s32 iItem )
{
    s32 OldSelection = m_iSelection;

    if( iItem < m_iSelection ) m_iSelection--;
    if( m_iSelection < 0 ) m_iSelection = 0;
    if( m_iSelection > m_Items.GetCount() ) m_iSelection = m_Items.GetCount() - 1;
    if( m_Items.GetCount() == 0 ) m_iSelection = -1;
    m_Items.Delete( iItem );

    if( (m_iSelection != OldSelection) && m_pParent )
        m_pParent->OnNotify( m_pParent, this, WN_COMBO_SELCHANGE, (void*)m_iSelection );
}

//=========================================================================

s32 ui_combo::GetItemCount( void ) const
{
    return m_Items.GetCount();
}

//=========================================================================

const xwstring& ui_combo::GetItemLabel( s32 iItem ) const
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    return m_Items[iItem].Label;
}

//=========================================================================

s32 ui_combo::GetItemData( s32 iItem, s32 Index ) const
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );
    ASSERT( (Index >= 0) && (Index < COMBO_DATA_FIELDS) );

    return m_Items[iItem].Data[Index];
}

//=========================================================================

const xwstring& ui_combo::GetSelectedItemLabel( void ) const
{
    ASSERT( (m_iSelection >= 0) && (m_iSelection < m_Items.GetCount()) );

    return m_Items[m_iSelection].Label;
}

//=========================================================================

s32 ui_combo::GetSelectedItemData( s32 Index ) const
{
    ASSERT( (m_iSelection >= 0) && (m_iSelection < m_Items.GetCount()) );
    ASSERT( (Index >= 0) && (Index < COMBO_DATA_FIELDS) );

    return m_Items[m_iSelection].Data[Index];
}

//=========================================================================

s32 ui_combo::FindItemByLabel( const xwstring& Label )
{
    s32     i;
    s32     iFound = -1;

    for( i=0 ; i<m_Items.GetCount() ; i++ )
    {
        if( m_Items[i].Label == Label )
        {
            iFound = i;
            break;
        }
    }

    return iFound;
}

//=========================================================================

s32 ui_combo::FindItemByData( s32 Data, s32 Index )
{
    ASSERT( (Index >= 0) && (Index < COMBO_DATA_FIELDS) );

    s32     i;
    s32     iFound = -1;

    for( i=0 ; i<m_Items.GetCount() ; i++ )
    {
        if( m_Items[i].Data[Index] == Data )
        {
            iFound = i;
            break;
        }
    }

    return iFound;
}

//=========================================================================

s32 ui_combo::GetSelection( void ) const
{
    return m_iSelection;
}

//=========================================================================

void ui_combo::SetSelection( s32 iSelection )
{
    ASSERT( (iSelection >= -1) && (iSelection < m_Items.GetCount()) );

    m_iSelection = iSelection;

    if( m_pParent )
        m_pParent->OnNotify( m_pParent, this, WN_COMBO_SELCHANGE, (void*)m_iSelection );
}

//=========================================================================

void ui_combo::ClearSelection( void )
{
    m_iSelection = -1;
}

//=========================================================================

void ui_combo::OnLBDown ( ui_win* pWin )
{
    (void)pWin;
#ifdef TARGET_PC
    // Throw up default config selection dialog
    irect r = GetPosition();
    r.Translate( -r.l, -r.t + r.GetHeight() );
    LocalToScreenCreate( r );

    // Scale the list box to a resonable size.
    s32 Items = m_Items.GetCount()-1;
    if( Items < 3 )
        Items = 3;
    r.b = r.t + 26+18*(Items);
    s32 x, y;
    eng_GetRes( x, y );
    
    // Keep the list box inside the viewport.
    if( r.b > y )
    {
        s32 diff = r.b - y;
        r.Translate( 0, -diff );
    }

    // Open a list box with all the items listed in it.
    ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
    ui_listbox* pList = pListDialog->GetListBox();
    pListDialog->SetResultPtr( &m_iSelection );

    // Set the items label.
    for( s32 i=0 ; i < m_Items.GetCount(); i++ )
    {
        pList->AddItem( m_Items[i].Label, i );
    }

    pList->SetSelection( 0 );
#endif
}

//=========================================================================