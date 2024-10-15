//==============================================================================
//
//  Disk.hpp
//
//==============================================================================

#ifndef DISK_HPP
#define DISK_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ObjectMgr\ObjectMgr.hpp"
#include "Shape\ShapeInstance.hpp"
#include "Linear.hpp"

//==============================================================================
//  TYPES
//==============================================================================

class disk : public linear
{

public:

    void        Setup           ( void );

    void        OnAcceptInit    ( const bitstream& BitStream );
    void        OnAdd           ( void );

    void        Initialize      ( const matrix4&   L2W, 
                                        u32        TeamBits,
                                        object::id OriginID );

    update_code Impact          ( vector3& Point, vector3& Normal );
    void        RenderCore      ( void );
    void        RenderEffects   ( void );

protected:
    shape_instance  m_Core; 
    shape_instance  m_Effects; 
};

//==============================================================================
//  FUNCTIONS
//==============================================================================

object*     DiskCreator( void );

//==============================================================================
#endif // DISK_HPP
//==============================================================================
