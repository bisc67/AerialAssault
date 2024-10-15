//==============================================================================
//
//  Bullet.hpp
//
//==============================================================================

#ifndef BULLET_HPP
#define BULLET_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ObjectMgr\ObjectMgr.hpp"
#include "Shape\ShapeInstance.hpp"
#include "Linear.hpp"

//==============================================================================
//  FUNCTIONS
//==============================================================================

object*     BulletCreator( void );

//==============================================================================
//  TYPES
//==============================================================================

class bullet : public linear
{
private:
    vector3     m_Tracer;

public:
    void        Setup           ( void );

    void        OnAcceptInit    ( const bitstream& BitStream );

    void        Initialize      ( const matrix4&   L2W, 
                                        u32        TeamBits,
                                        object::id OriginID,
                                        object::id ExcludeID = obj_mgr::NullID );

    void        Render          ( s32 Stage );
    
    update_code Impact          ( vector3& Point, vector3& Normal );

static  void        Init            ( void );
static  void        Kill            ( void );
static  void        BeginRender     ( s32 Stage );
static  void        EndRender       ( void );
};

//==============================================================================
#endif // BULLET_HPP
//==============================================================================
