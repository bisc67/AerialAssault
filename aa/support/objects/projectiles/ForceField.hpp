//==============================================================================
//
//  ForceField.hpp
//
//==============================================================================

#ifndef FORCEFIELD_HPP
#define FORCEFIELD_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ObjectMgr\ObjectMgr.hpp"
#include "Shape\ShapeInstance.hpp"
#include "x_files.hpp"

//==============================================================================
//  TYPES
//==============================================================================

class force_field : public object
{

public:

        update_code OnAdvanceLogic  ( f32 DeltaTime );
        void        OnCollide       ( u32 AttrBits, collider& Collider );
                                                         
        void        Initialize      ( const vector3& WorldPos,
                                      const radian3& WorldRot,
                                      const vector3& WorldScale,
                                            s32      Switch,
                                            s32      Power );

static  void        Init            ( void );
static  void        Kill            ( void );

        void        DebugRender     ( void );
        void        Render          ( void );

protected:

        s32         m_Switch;
        s32         m_Power;
        xbool       m_HasPower;
        f32         m_Slide;

static  xbitmap     m_Bitmap;
        vector3     m_Vertex[8];

// For shape version.

    shape_instance  m_Shape; 
    radian3         m_WorldRot;
    vector3         m_WorldScale;
};

//==============================================================================
//  FUNCTIONS
//==============================================================================

object*     ForceFieldCreator( void );

//==============================================================================
#endif // FORCEFIELD_HPP
//==============================================================================


