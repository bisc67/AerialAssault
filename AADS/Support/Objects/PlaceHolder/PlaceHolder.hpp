//==============================================================================
//
//  PlaceHolder.hpp
//
//==============================================================================

#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ObjectMgr\ObjectMgr.hpp"

//==============================================================================
//  TYPES
//==============================================================================

class place_holder : public object
{

public:

    update_code OnAdvanceLogic  ( f32 DeltaTime );
    update_code OnAcceptUpdate  ( const bitstream& BitStream, f32 SecInPast );
    void        OnProvideUpdate ( bitstream& BitStream, u32& DirtyBits, f32 Priority );
    void        OnAcceptInit    ( const bitstream& BitStream );
    void        OnProvideInit   (       bitstream& BitStream );
    void        OnCollide       ( u32 AttrBits, collider& Collider );
                                                         
    void        Initialize      ( const vector3& WorldPos,
                                  const radian3& WorldRot,
                                  const char*    pLabel );

    void        DebugRender     ( void );
    void        Render          ( void );

protected:
    matrix4     m_L2W; 
    f32         m_LabelTimer;
    char        m_Label[64];
};

//==============================================================================
//  FUNCTIONS
//==============================================================================

object*     PlaceHolderCreator( void );

//==============================================================================
#endif // PLACEHOLDER_HPP
//==============================================================================
