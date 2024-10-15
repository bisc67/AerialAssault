//==============================================================================
//
//  Projector.hpp
//
//==============================================================================

#ifndef PROJECTOR_HPP
#define PROJECTOR_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ObjectMgr\ObjectMgr.hpp"
#include "Shape\ShapeInstance.hpp"

//==============================================================================
//  TYPES
//==============================================================================

class projector : public object
{

public:

    update_code OnAdvanceLogic  ( f32 DeltaTime );
    void        OnCollide       ( u32 AttrBits, collider& Collider );
                                                         
    void        Initialize      ( const vector3& WorldPos,
                                  const radian3& WorldRot,
                                        s32      Switch );

    void        DebugRender     ( void );
    void        Render          ( void );

protected:
    radian3         m_WorldRot;
    radian3         m_LogoRot;
    s32             m_Switch;
    shape_instance  m_Shape; 
    shape_instance  m_Logo; 
};

//==============================================================================
//  FUNCTIONS
//==============================================================================

object*     ProjectorCreator( void );

//==============================================================================
#endif // PROJECTOR_HPP
//==============================================================================
