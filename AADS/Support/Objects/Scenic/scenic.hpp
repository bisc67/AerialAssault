//==============================================================================
//
//  Scenic.hpp
//
//==============================================================================

#ifndef SCENIC_HPP
#define SCENIC_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ObjectMgr\ObjectMgr.hpp"
#include "Shape\ShapeInstance.hpp"
#include "..\AADS\Globals.hpp"

//==============================================================================
//  ENUMS/DEFINES
//==============================================================================

//==============================================================================
//  TYPES
//==============================================================================

class scenic : public object
{

//------------------------------------------------------------------------------
//  Public data types

public:

//------------------------------------------------------------------------------
//  Public functions

public:
    
                scenic          ( void );

    void        Initialize      ( const vector3& Position,                                   
                                  const radian3& Rot,
                                  const vector3& Scale,
                                  const char*    pName);

    void        OnCollide       ( u32 AttrBits, collider& Collider );
    update_code OnAdvanceLogic  ( f32 DeltaTime );

    void        Render          ( void );

    s32         GetShapeType    ( void ) ;


//------------------------------------------------------------------------------
//  Private data

protected:
    radian3         m_WorldRot;
    shape_instance  m_ShapeInstance;
    bbox            m_CollBBox ;
    xbool           m_HasShape;
    zone_set        m_ZoneSet;
};

//==============================================================================
#endif // SCENIC_HPP
//==============================================================================
