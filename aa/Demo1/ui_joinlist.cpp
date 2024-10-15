//=========================================================================
//
//  ui_joinlist.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "AudioMgr\audio.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "gamemgr/gamemgr.hpp"

#include "ui\ui_listbox.hpp"
#include "ui\ui_manager.hpp"
#include "ui\ui_font.hpp"
#include "ui_joinlist.hpp"

#include "serverman.hpp"
#include "StringMgr\StringMgr.hpp"
#include "data\ui\ui_strings.h"

//=========================================================================
//  Defines
//=========================================================================

#define SPACE_TOP       4
#define SPACE_BOTTOM    4
#define LINE_HEIGHT     16
#define TEXT_OFFSET     -2

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Factory function
//=========================================================================

ui_win* ui_joinlist_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    ui_joinlist* pList = new ui_joinlist;
    pList->Create( UserID, pManager, Position, pParent, Flags );

    return (ui_win*)pList;
}

//=========================================================================
//  ui_listbox
//=========================================================================

ui_joinlist::ui_joinlist( void )
{
}

//=========================================================================

ui_joinlist::~ui_joinlist( void )
{
}

//=========================================================================

void ui_joinlist::Render( s32 ox, s32 oy )
{
    s32     i;

    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        xcolor  TextColor1  = XCOLOR_WHITE;
        xcolor  TextColor2  = XCOLOR_BLACK;
        s32     State       = ui_manager::CS_NORMAL;

        // Calculate rectangle
        irect   br;
        irect   r;
        irect   r2;
        br.Set( (m_Position.l+ox), (m_Position.t+oy), (m_Position.r+ox), (m_Position.b+oy) );
        r = br;
        r2 = r;
        r.r -= 19;
        r2.l = r.r;

        // Render appropriate state
        if( m_Flags & WF_DISABLED )
        {
            State = ui_manager::CS_DISABLED;
            TextColor1  = XCOLOR_GREY;
            TextColor2  = xcolor(0,0,0,0);
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_HIGHLIGHT )
        {
            State = ui_manager::CS_HIGHLIGHT;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_SELECTED )
        {
            State = ui_manager::CS_SELECTED;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == (WF_HIGHLIGHT|WF_SELECTED) )
        {
            State = ui_manager::CS_HIGHLIGHT_SELECTED;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else
        {
            State = ui_manager::CS_NORMAL;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }

        // Add Highlight to list
        if( m_Flags & WF_HIGHLIGHT )
            m_pManager->AddHighlight( m_UserID, br, !(m_Flags & WF_SELECTED) );

        // Render background color
        irect   rb = r;
        rb.Deflate( 1, 1 );
        m_pManager->RenderRect( rb, m_BackgroundColor, FALSE );

        // Render Text & Selection Marker
        irect rl = r;
        rl.SetHeight( LINE_HEIGHT );
        rl.Deflate( 2, 0 );
        rl.r -= 2;
        rl.Translate( 0, SPACE_TOP );

        m_pManager->PushClipWindow( r );

        for( i=0 ; i<m_nVisibleItems ; i++ )
        {
            s32 iItem = m_iFirstVisibleItem + i;

            if( (iItem >= 0) && (iItem < m_Items.GetCount()) )
            {
                // Render Selection Rectangle
                if( (iItem == m_iSelection)  && m_ShowBorders )
                {
                    if( m_Flags & (WF_SELECTED) )
                    {
                        m_pManager->RenderRect( rl, xcolor(0,100,160,192), FALSE );
                        if( m_Flags & WF_HIGHLIGHT )
                            m_pManager->AddHighlight( m_UserID, rl );
                    }
                    else
                        m_pManager->RenderRect( rl, xcolor(0,60,100,192), FALSE );
                }
#ifdef TARGET_PC
                // Let the hight light track the mouse cursor.
                if( iItem == m_TrackHighLight )
                {
                    m_pManager->AddHighlight( m_UserID, rl );
                }
#endif

                // Render Text
                xcolor c1 = m_Items[iItem].Color;
                xcolor c2 = TextColor2;
                if( !m_Items[iItem].Enabled )
                {
                    c1 = XCOLOR_GREY;
                    c2 = xcolor(0,0,0,0);
                }
                irect rl2 = rl;

/*
                // Darken text if listbox not selected
                if( !(m_Flags & WF_SELECTED) )
                {
                    c1.R = (u8)(c1.R * 0.85f);
                    c1.G = (u8)(c1.G * 0.85f);
                    c1.B = (u8)(c1.B * 0.85f);
                    c1.A = (u8)(c1.A * 0.85f);
                    c2.R = (u8)(c2.R * 0.85f);
                    c2.G = (u8)(c2.G * 0.85f);
                    c2.B = (u8)(c2.B * 0.85f);
                    c2.A = (u8)(c2.A * 0.85f);
                }
*/

//				m_pManager->PushClipWindow( rl2 );

                RenderItem( rl2, m_Items[iItem], c1, c2 );

				// Clear the clip window
//				m_pManager->PopClipWindow();

            }
            rl.Translate( 0, LINE_HEIGHT );
        }

        m_pManager->PopClipWindow();

        if (m_ShowBorders)
        {
            // Render Frame
            m_pManager->RenderElement( m_iElementFrame, r, 0 );
            irect r3 = r2;
            irect r4 = r2;
            r3.b = r3.t + 22;
            r4.t = r4.b - 22;
            r2.t = r3.b;
            r2.b = r4.t;

#ifdef TARGET_PC
            m_UpArrow = r3;
            m_DownArrow = r4;
#endif
            m_pManager->RenderElement( m_iElement_sb_container, r2, State );
            m_pManager->RenderElement( m_iElement_sb_arrowup,   r3, State );
            m_pManager->RenderElement( m_iElement_sb_arrowdown, r4, State );

            // Render Thumb
            r2.Deflate( 2, 2 );
			s32 itemcount;

			itemcount = 0;
			for (s32 i=0;i<m_Items.GetCount();i++)
			{
				if (m_Items[i].Enabled)
					itemcount++;
			}

            if( itemcount > m_nVisibleItems )
            {
                f32 t = (f32)m_iFirstVisibleItem / itemcount;
                f32 b = (f32)(m_iFirstVisibleItem + m_nVisibleItems) / itemcount;
                if( t < 0.0f ) t = 0.0f;
                if( b > 1.0f ) b = 1.0f;
                r2.Set( r2.l, r2.t + (s32)(r2.GetHeight() * t), r2.r, r2.t + (s32)(r2.GetHeight() * b) );
            }
			if( r2.GetHeight() < 8)
			{
				r2.SetHeight(8);
			}
//            if( r2.GetHeight() > 16 )
            m_pManager->RenderElement( m_iElement_sb_thumb,     r2, State );
#ifdef TARGET_PC
            m_ScrollBar = r2;
#endif
        }

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}

//=========================================================================

static const char* GetGameTypeString( const server_info* pServer )
{
    const char* pString = "";

    switch(pServer->ServerInfo.GameType)
    {
    case GAME_NONE:
        pString = "";
        break;
    case GAME_CTF:
        pString = "CTF:";
        break;
    case GAME_CNH:
        pString = "CNH:";
        break;
    case GAME_DM:
        pString = "DM:";
        break;
    case GAME_TDM:
        pString = "TDM:";
        break;
    case GAME_HUNTER:
        pString = "Hunter:";
        break;
    case GAME_CAMPAIGN:
        pString = ":";
        break;
    }

    return pString;
}

//=========================================================================

void ui_joinlist::RenderString( irect r, u32 Flags, const xcolor& c1, const xcolor& c2, const char* pString )
{
    m_pManager->RenderText( m_Font, r, Flags, c2, pString );
    r.Translate( -1, -1 );
    m_pManager->RenderText( m_Font, r, Flags, c1, pString );
}

void ui_joinlist::RenderItem( irect r, const item& Item, const xcolor& c1, const xcolor& c2 )
{
    r.Deflate( 4, 0 );
    r.Translate( 1, -2 );

    irect rIcons   = r;
    irect rName    = r;
    irect rPlayers = r;
    irect rMap     = r;

    rIcons.r   = r.l+ 48;
    rName.l    = r.l+ 61;
    rName.r    = r.l+176+32;
    rPlayers.l = r.l+176+32;
    rPlayers.r = r.l+176+48+28; //250;
    rMap.l     = r.l+242+32;

    const server_info* pServerInfo = (const server_info*)Item.Data;

    if( pServerInfo->ServerInfo.GameType >= 0 )
    {
        xstring Name;
        xstring Players;
        xstring Map;

        Name     = pServerInfo->ServerInfo.Name;
        Players  = xfs( "%d/%d", pServerInfo->ServerInfo.CurrentPlayers, pServerInfo->ServerInfo.MaxPlayers );
        Map      = GetGameTypeString( pServerInfo );
        Map     += pServerInfo->ServerInfo.LevelName;

        if( pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_BUDDY )
            RenderString( rIcons  , ui_font::h_left  |ui_font::v_center,                        c1, c2, "\027" );
        
        rIcons.l += 11;
        if( pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK )
            RenderString( rIcons  , ui_font::h_left  |ui_font::v_center,                        c1, c2, "\030" );

        rIcons.l += 15;
        if( pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_PASSWORD )
            RenderString( rIcons  , ui_font::h_left  |ui_font::v_center,                        c1, c2, "\026" );

        rIcons.l += 14;
        if( pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_MODEM )
			RenderString( rIcons  , ui_font::h_left  |ui_font::v_center,                        c1, c2, "\x84" );

        RenderString( rName   , ui_font::h_left  |ui_font::v_center|ui_font::clip_ellipsis, c1, c2, (const char*)Name );
        RenderString( rPlayers, ui_font::h_center|ui_font::v_center|ui_font::clip_ellipsis, c1, c2, (const char*)Players );
        RenderString( rMap    , ui_font::h_left  |ui_font::v_center|ui_font::clip_ellipsis, c1, c2, (const char*)Map );
    }
    else
    {
        xstring Status;

        switch( pServerInfo->ServerInfo.GameType )
        {
        case -1:
            Status = StringMgr( "ui", IDS_OJ_WAITING    ); break;
        case -2:
            Status = StringMgr( "ui", IDS_OJ_NORESPONSE ); break;
        default:
            Status = "Unknown response"; break;
        }

        RenderString( rMap    , ui_font::h_left  |ui_font::v_center|ui_font::clip_ellipsis, c1, c2, (const char*)Status );
    }
}

//=========================================================================
