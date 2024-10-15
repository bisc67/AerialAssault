//==============================================================================
//  
//  hud_font.hpp
//  
//==============================================================================

#ifndef HUD_FONT_HPP
#define HUD_FONT_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_TYPES_HPP
#include "x_types.hpp"
#endif

//==============================================================================
//  HUD_FONT
//==============================================================================

class hud_font
{
    friend class hud;

public:
    enum Flags
    {
        h_left   = 0x0001,
        h_center = 0x0002,
        h_right  = 0x0004,
        v_top    = 0x0008,
        v_center = 0x0010,
        v_bottom = 0x0020
    };

protected:
    struct Character
    {
        s32     X;
        s32     Y;
        s32     W;
    };

    xbitmap     m_Bitmap;
    s32         m_BmWidth;
    s32         m_BmHeight;
    s32         m_RowHeight;
    s32         m_Height;
    Character   m_Characters[256];

public:
    xbool   Load                ( const char* pPathName );
    void    Kill                ( void );
    void    TextSize            ( irect& Rect, const char* pString, s32 Count = -1 ) const;
    void    FormattedTextSize   ( irect& Rect, const char* pString, s32 Count = -1 ) const;

    void    SetupTextRender     ( void );
    void    DrawText            ( const irect& R, u32 Flags, xcolor Color, const char* pString ) const;
    void    DrawFormattedText   ( const irect& R, u32 Flags, xcolor Color, const char* pString ) const;
};

//==============================================================================
#endif // HUD_FONT_HPP
//==============================================================================
