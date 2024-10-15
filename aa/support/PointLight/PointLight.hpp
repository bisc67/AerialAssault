//=========================================================================
//
// POINTLIGHT.HPP
//
//=========================================================================
#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "x_math.hpp"
#include "x_color.hpp"

//=========================================================================

#define MAX_FCACHE_VERTS        3

typedef u32 fcache_key;

struct fcache_face
{
    fcache_key  Key;
    plane       Plane;
    bbox        BBox;
    s32         NVerts;
    vector3     Pos[ MAX_FCACHE_VERTS ];
    vector2     OrthoST[ MAX_FCACHE_VERTS ];
    vector3     OrthoS;
    vector3     OrthoT;
    s32         Next;
    s32         Prev;
};

void fcache_ConstructKey ( fcache_key& Key, s32  ObjectSlot, s32  PrivateInfo );
void fcache_DestructKey  ( fcache_key  Key, s32& ObjectSlot, s32& PrivateInfo );

//=========================================================================

void ptlight_Init       ( void );
void ptlight_Kill       ( void );

// For fire and forget lights
void ptlight_Create     ( const vector3& Pos,
                                f32      Radius,
                                xcolor   Color,
                                f32      RampUpTime,
                                f32      HoldTime,
                                f32      RampDownTime,
                                xbool    IncludeBldgs = TRUE );

// For dynamically updated lights
s32  ptlight_Create     ( const vector3& Pos, f32 Radius, xcolor Color, xbool IncludeBldgs = TRUE );
void ptlight_Destroy    ( s32 Handle );
void ptlight_SetPosition( s32 Handle, const vector3& Pos );
void ptlight_SetRadius  ( s32 Handle, f32 Radius );
void ptlight_SetColor   ( s32 Handle, xcolor Color );

void ptlight_Render         ( void );
void ptlight_AdvanceLogic   ( f32 DeltaSec );

void AddBulletHole      ( const vector3& Pos, const vector3& Normal );

void shadow_Render      ( const vector3& SunDir );

void AddDummy           ( const vector3& Pos );

//=========================================================================
#endif
//=========================================================================
