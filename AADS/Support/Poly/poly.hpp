//=============================================================================
//
//  Polygon Renderer
//
//=============================================================================

#ifndef POLY_HPP
#define POLY_HPP

//=============================================================================
//  Includes
//=============================================================================

#include "Entropy.hpp"

//=============================================================================
//  Defines
//=============================================================================

#define POLY_USE_ALPHA      ( 1 << 0 )
#define POLY_DRAW_SPRITES   ( 1 << 1 )
#define POLY_NO_TEXTURE     ( 1 << 2 )

//=============================================================================
//  Types
//=============================================================================

#ifdef TARGET_PC

struct ps2color
{
    u8 R, G, B, A;

    // for compatibility with PS2 class
    void Set( u8 vR, u8 vG, u8 vB, u8 vA = 255 )    { R=vR; G=vG; B=vB; A=vA; }
};

#endif

//=============================================================================
//  Functions
//=============================================================================

void poly_Initialize( void );
void poly_Begin     ( u32 Flags = 0 );
void poly_End       ( void );
void poly_Render    ( vector3* pVerts, vector2* pTex, ps2color* pColor, s32 nVerts );
void poly_SetZBias  ( s32 Bias );

#endif
