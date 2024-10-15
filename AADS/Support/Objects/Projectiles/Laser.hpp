//==============================================================================
//
//  Laser.hpp
//
//==============================================================================

#ifndef LASER_HPP
#define LASER_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ObjectMgr\ObjectMgr.hpp"
#include "Shape\ShapeInstance.hpp"
#include "Linear.hpp"

//==============================================================================
//  FUNCTIONS
//==============================================================================

object*     LaserCreator        ( void ); 

//==============================================================================
//  TYPES
//==============================================================================

class laser : public object
{

//------------------------------------------------------------------------------
//  Public Functions

public:

    update_code OnAdvanceLogic  ( f32 DeltaTime );
    void        OnAcceptInit    ( const bitstream& BitStream );
    void        OnProvideInit   (       bitstream& BitStream );
    void        OnAdd           ( void );

    void        Initialize      ( const vector3&   Start,
                                        f32        Energy,
                                        u32        TeamBits,
                                        object::id OriginID );

    void        Render          ( void );

    void        DrawBeam        ( f32      UVStart,
                                  s32      UVLen,
                                  xcolor   Color0,
                                  xcolor   Color1,
                                  f32      Radius,
                                  s32      TWidth );

static void     Init           ( void );
static void     Kill           ( void );
static void     BeginRender    ( void );
static void     EndRender      ( void );


//------------------------------------------------------------------------------
//  Private Data

    vector3     m_Start;
    vector3     m_End;
    vector3     m_ColNorm;      // Collision normal
    vector3     m_Dir;          // normalized direction vector
    s32         m_Zoom;         // zoom factor (for texture stretching)
    f32         m_Age;
    f32         m_Energy;
    xbool       m_Hit;
    xbool       m_WasHeadShot;  // did we snipe someone in the head?
    f32         m_Length;
    object::id  m_VictimID;
    object::id  m_OriginID;
    object::id  m_ExcludeID;
};

//==============================================================================
#endif // LASER_HPP
//==============================================================================
