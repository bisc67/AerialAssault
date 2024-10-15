//==============================================================================
//  
//  x_color.hpp
//  
//==============================================================================

#ifndef X_COLOR_HPP
#define X_COLOR_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_TYPES_HPP
#include "x_types.hpp"
#endif

//==============================================================================
//  TYPES
//==============================================================================

struct xcolor
{

//------------------------------------------------------------------------------
//  Fields

    u8  B, G, R, A;     // Values are in [ 0, 255 ].

//------------------------------------------------------------------------------
//  Functions

                xcolor              ( void );
                xcolor              ( const xcolor& C );
                xcolor              ( u8 R, u8 G, u8 B, u8 A = 255 );
                xcolor              ( u32 ARGB );
                            
        void    Set                 ( u8 R, u8 G, u8 B, u8 A = 255 );
        void    Set                 ( u32 ARGB );
        void    Random              ( u8 A = 255 );

const   xcolor& operator =          ( const xcolor& C );
const   xcolor& operator =          ( u32 ARGB );

        u32     GetRGBA             ( void ) const;

                operator const u32  ( void ) const;

    // How about:
    //  c *  c
    //  c *= c
    //  c *  s
    //  s *  c
    //  c *= s
    //  division also
};                 

#include "Implementation/x_color_inline.hpp"

//==============================================================================

#define ARGB(a,r,g,b) (((u32)(a)<<24)|((u32)(r)<<16)|((u32)(g)<<8)|(u32)(b))

enum xcolors
{   //                  AARRGGBB
    XCOLOR_BLACK    = 0xFF000000,
    XCOLOR_WHITE    = 0xFFFFFFFF,
    XCOLOR_RED      = 0xFFFF0000,    
    XCOLOR_GREEN    = 0xFF00FF00,    
    XCOLOR_BLUE     = 0xFF0000FF,    
    XCOLOR_AQUA     = 0xFF00FFFF,    
    XCOLOR_PURPLE   = 0xFFFF00FF,
    XCOLOR_YELLOW   = 0xFFFFFF00,    
    XCOLOR_GREY     = 0xFF808080,
};                  

//==============================================================================
#endif // X_COLOR_HPP
//==============================================================================
