//==============================================================================
//
//  ForceField.cpp
//
//==============================================================================

// TO DO: Optimize the collision and the rendering with more data.

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ForceField.hpp"
#include "Entropy.hpp"
#include "GameMgr\GameMgr.hpp"
#include "NetLib\BitStream.hpp"
#include "..\AADS\Globals.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "Objects\Projectiles\Bounds.hpp"

//==============================================================================
//  PROTOTYPES
//==============================================================================

xbool IntersectsOBB( const bbox& BBox, const vector3* pOBBVerts );

//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   ForceFieldCreator;

obj_type_info   ForceFieldTypeInfo( object::TYPE_FORCE_FIELD, 
                                    "ForceField", 
                                    ForceFieldCreator, 
                                    object::ATTR_FORCE_FIELD |
                                    object::ATTR_SOLID_DYNAMIC );

/* DEDICATED_SERVER
xbitmap         force_field::m_Bitmap;
DEDICATED_SERVER */

//==============================================================================
//  FUNCTIONS
//==============================================================================

object* ForceFieldCreator( void )
{
    return( (object*)(new force_field) );
}

//==============================================================================

object::update_code force_field::OnAdvanceLogic( f32 DeltaTime )
{   
    xbool NewPowered;
    m_TeamBits = GameMgr.GetSwitchBits( m_Switch );
    NewPowered = GameMgr.GetPower( m_Power );

    if( NewPowered && !m_HasPower )
    {
        // The force field has just been activated.  Woe to anything within!
        object* pObject;
        ObjMgr.Select( object::ATTR_DAMAGEABLE, m_WorldBBox );
        while( (pObject = ObjMgr.GetNextSelected()) )
        {
            if( IntersectsOBB( pObject->GetBBox(), m_Vertex ) )
            {
                ObjMgr.InflictPain( pain::MISC_FORCE_FIELD, 
                                    pObject,
                                    pObject->GetPosition(),
                                    m_ObjectID,
                                    pObject->GetObjectID() );
            }           
        }
        ObjMgr.ClearSelection();
    }

    m_HasPower = NewPowered;

    m_Slide += DeltaTime;
    if( m_Slide > 1.0f )
        m_Slide -= 1.0f;

    return( NO_UPDATE );
}

//==============================================================================

void force_field::Initialize( const vector3& WorldPos,
                              const radian3& WorldRot,
                              const vector3& WorldScale,
                                    s32      Switch, 
                                    s32      Power )
{   
    s32 i;

    /* DEDICATED_SERVER

    // Keep info
    m_WorldPos   = WorldPos;
    m_WorldRot   = WorldRot;
    m_WorldScale = WorldScale;

    // Setup shape
    vector3 Offset = m_WorldScale * 0.5f;
    Offset.X = -Offset.X;
    Offset.Rotate( WorldRot );

    m_Shape.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_MISC_FORCE_FIELD ) );
    m_Shape.SetScale( m_WorldScale );
    m_Shape.SetRot  ( m_WorldRot );
    m_Shape.SetPos  ( m_WorldPos + Offset );

    // Since instance never moves, declare it static for some speedup.
    m_Shape.SetFlag( shape_instance::FLAG_IS_STATIC, TRUE );

    DEDICATED_SERVER */

    // Setup local to world
    matrix4 L2W;
    L2W.Identity();
    L2W.Setup( WorldScale, WorldRot, WorldPos );

    // Setup verts
    m_Vertex[0](  0.0f, 1.0f, 1.0f );
    m_Vertex[1](  0.0f, 1.0f, 0.0f );
    m_Vertex[2]( -1.0f, 1.0f, 0.0f );
    m_Vertex[3]( -1.0f, 1.0f, 1.0f );
    m_Vertex[4](  0.0f, 0.0f, 1.0f );
    m_Vertex[5](  0.0f, 0.0f, 0.0f );
    m_Vertex[6]( -1.0f, 0.0f, 0.0f );
    m_Vertex[7]( -1.0f, 0.0f, 1.0f );
    L2W.Transform( m_Vertex, m_Vertex, 8 );

    // BUGFIX - Being able to walk through force fields -
    // 1) Grab bounds from verts incase of float inaccuracy
    // 2) The bounds must contain whatever tris are fed to 
    //    the collider which is this vertex array and not the shape.
    //    The shape bounds are slightly smaller than this transformed
    //    vertex list - therefore was messing up the collision detection
    //    and letting the player through.
    m_WorldBBox.Clear();
    for( i = 0; i < 8; i++ )
        m_WorldBBox += m_Vertex[i];

    // Setup game stuff
    m_Switch    = Switch;
    m_Power     = Power;
    m_HasPower  = GameMgr.GetPower( m_Power );

    m_Slide     = 0.0f;
}

//==============================================================================

void force_field::DebugRender( void )
{
    /* DEDICATED_SERVER

    draw_ClearL2W() ;
    for( s32 i = 0; i < 8; i++ )
    {
        draw_Point( m_Vertex[i], XCOLOR_BLUE );
    }
    draw_BBox( m_WorldBBox, XCOLOR_RED );

    matrix4 L2W;
    L2W.Identity();
    L2W.SetTranslation( m_WorldBBox.GetCenter() );
    draw_SetL2W( L2W );
    draw_Axis( 10.0f );
    draw_ClearL2W();

    DEDICATED_SERVER */
}

//==============================================================================

void force_field::OnCollide( u32 AttrBits, collider& Collider )
{
    (void)AttrBits;

    // No power, no collision.
    if( !GameMgr.GetPower( m_Power ) )
        return;

    // Force fields do not affect players which have a matching team bit.
    {
        object* pObject = (object*)Collider.GetMovingObject();
        if( (pObject) &&
            (pObject->GetAttrBits() & ATTR_PLAYER) && 
            (pObject->GetTeamBits() & m_TeamBits) )
            return;
    }

    // Well, if we are here, then this thing is solid.  Collide.

    plane Plane;

    Plane.Setup      ( m_Vertex[0], m_Vertex[1], m_Vertex[2] );
    Collider.ApplyTri( m_Vertex[0], m_Vertex[1], m_Vertex[2], Plane );
    Collider.ApplyTri( m_Vertex[2], m_Vertex[3], m_Vertex[0], Plane );

    Plane.Setup      ( m_Vertex[6], m_Vertex[5], m_Vertex[4] );
    Collider.ApplyTri( m_Vertex[6], m_Vertex[5], m_Vertex[4], Plane );
    Collider.ApplyTri( m_Vertex[4], m_Vertex[7], m_Vertex[6], Plane );

    Plane.Setup      ( m_Vertex[0], m_Vertex[4], m_Vertex[5] );
    Collider.ApplyTri( m_Vertex[0], m_Vertex[4], m_Vertex[5], Plane );
    Collider.ApplyTri( m_Vertex[5], m_Vertex[1], m_Vertex[0], Plane );

    Plane.Setup      ( m_Vertex[1], m_Vertex[5], m_Vertex[6] );
    Collider.ApplyTri( m_Vertex[1], m_Vertex[5], m_Vertex[6], Plane );
    Collider.ApplyTri( m_Vertex[6], m_Vertex[2], m_Vertex[1], Plane );

    Plane.Setup      ( m_Vertex[2], m_Vertex[6], m_Vertex[7] );
    Collider.ApplyTri( m_Vertex[2], m_Vertex[6], m_Vertex[7], Plane );
    Collider.ApplyTri( m_Vertex[7], m_Vertex[3], m_Vertex[2], Plane );

    Plane.Setup      ( m_Vertex[3], m_Vertex[7], m_Vertex[4] );
    Collider.ApplyTri( m_Vertex[3], m_Vertex[7], m_Vertex[4], Plane );
    Collider.ApplyTri( m_Vertex[4], m_Vertex[0], m_Vertex[3], Plane );
}

//==============================================================================

void force_field::Init( void )
{
    /* DEDICATED_SERVER

    // Load the xbitmap.

    #ifdef TARGET_PC
    VERIFY( m_Bitmap.Load( "Data\\Projectiles\\PC\\ForceField.xbmp" ) );
    #endif

    #ifdef TARGET_PS2
    VERIFY( m_Bitmap.Load( "Data\\Projectiles\\PS2\\ForceField.xbmp" ) );
    #endif

    vram_Register( m_Bitmap );

    DEDICATED_SERVER */

}

//==============================================================================

void force_field::Kill( void )
{
    /* DEDICATED_SERVER

    // Unload the xbitmap.
    vram_Unregister( m_Bitmap );
    m_Bitmap.Kill();

    DEDICATED_SERVER */
}

//==============================================================================

static s32 s_BBoxEdgeList[12][2] =
{
    { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, { 4, 5 }, { 5, 6 },
    { 6, 7 }, { 7, 4 }, { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
};

//==============================================================================

xbool IntersectsOBB( const bbox& BBox, const vector3* pOBBVerts )
{
    const s32 NumBoxPlanes =  6;
    const s32 NumBoxVerts  =  8;
    const s32 NumBoxEdges  = 12;

    //
    // Setup vertex list for axis aligned bounding box.
    //

    vector3 AABBVerts[ NumBoxVerts ];
    
    vector3 VX = vector3( BBox.Max.X - BBox.Min.X, 0, 0 );
    vector3 VY = vector3( 0, BBox.Max.Y - BBox.Min.Y, 0 );
    vector3 VZ = vector3( 0, 0, BBox.Max.Z - BBox.Min.Z );

    AABBVerts[0] = BBox.Min;
    AABBVerts[1] = BBox.Min + VX;
    AABBVerts[2] = BBox.Max - VY;
    AABBVerts[3] = BBox.Min + VZ;
    AABBVerts[4] = BBox.Min + VY;
    AABBVerts[5] = BBox.Max - VZ;
    AABBVerts[6] = BBox.Max;
    AABBVerts[7] = BBox.Max - VX;
    
    //
    // Setup planes for oriented bounding box.
    //
    
    plane Planes[ NumBoxPlanes ];
    
    Planes[0].Setup( pOBBVerts[0], pOBBVerts[1], pOBBVerts[2] );
    Planes[1].Setup( pOBBVerts[6], pOBBVerts[5], pOBBVerts[4] );
    Planes[2].Setup( pOBBVerts[0], pOBBVerts[4], pOBBVerts[5] );
    Planes[3].Setup( pOBBVerts[1], pOBBVerts[5], pOBBVerts[6] );
    Planes[4].Setup( pOBBVerts[2], pOBBVerts[6], pOBBVerts[7] );
    Planes[5].Setup( pOBBVerts[3], pOBBVerts[7], pOBBVerts[4] );

    //
    // Test vertices of axis aligned box to see if any are inside the
    // oriented bounding box.
    //
    {
        s32 NumOut = 0;
    
        for( s32 i=0; i<NumBoxVerts; i++ )
        {
            s32 NumIn = 0;
            
            for( s32 j=0; j<NumBoxPlanes; j++ )
            {
                if( Planes[j].InBack( AABBVerts[i] ))
                    NumIn++;
                else
                    NumOut++;
            }

            // Test if point is inside all 6 planes.
            if( NumIn == NumBoxPlanes )
                return( TRUE );
        }

        // Test if all points are outside all the box planes.
        if( NumOut == ( NumBoxVerts * NumBoxPlanes ))
            return( FALSE );
    }

    //
    // Check edges of axis aligned box against oriented bounding box.
    //
    {
        for( s32 i=0; i<NumBoxEdges; i++ )
        {
            vector3& P0 = AABBVerts[ s_BBoxEdgeList[i][0] ];
            vector3& P1 = AABBVerts[ s_BBoxEdgeList[i][1] ];
            
            for( s32 j=0; j<NumBoxPlanes; j++ )
            {
                f32 t;
            
                if( Planes[j].Intersect( t, P0, P1 ))
                {
                    if(( t >= 0 ) && ( t <= 1.0f ))
                    {
                        //
                        // Test if point of intersection P is inside all other 
                        // planes .
                        //
    
                        vector3 N = P1 - P0;                
                        vector3 P = P0 + ( t * N );
                        
                        s32 PointIn = 0;
                    
                        for( s32 k=0; k<NumBoxPlanes; k++ )
                            if( k != j )
                                if( Planes[k].InBack( P ))
                                    PointIn++;
                        
                        //
                        // If point P is inside all the other planes, then the
                        // edge intersects the oriented bounding box.
                        //
                        
                        if( PointIn == ( NumBoxPlanes - 1 ))
                            return( TRUE );
                    }
                }
            }
        }    
    }

    //
    // Test vertices of oriented box to see if any are inside the axis
    // aligned bounding box.
    //
    {
        for( s32 i=0; i<NumBoxVerts; i++ )
            if( BBox.Intersect( pOBBVerts[i] ))
                return( TRUE );
    }
    
    return( FALSE );
}

//==============================================================================

static f32 HScalar = 0.0625f;
static f32 VScalar = 0.5000f;
static s32 Shift   = 2;

void force_field::Render( void )
{
}

/* DEDICATED_SERVER
   DEDICATED_SERVER
   DEDICATED_SERVER

//==============================================================================
#ifdef TARGET_PS2
//==============================================================================

void force_field::Render( void )
{
    // No power?
    if( !GameMgr.GetPower( m_Power ) )
        return;

    // Visible?
    s32 Visible = IsInViewVolume( m_WorldBBox );
    if( !Visible )
        return;

    // Allocate data.
    vector3*  pV = (vector3*)smem_BufferAlloc( sizeof( vector3 ) * 36 );
    vector2*  pT = (vector2*)smem_BufferAlloc( sizeof( vector2 ) * 36 );
    u32*      pC = (u32*)    smem_BufferAlloc( sizeof( u32     ) * 36 );

    if( (!pV) || (!pT) || (!pC) )
    {
        //x_DebugMsg( "Could not alloc buffers for force field.\n" );
        return;
    }

    // Decide on the color.

    xcolor Color = XCOLOR_YELLOW;

    if( m_TeamBits == 0x00000000 )        Color.Set( 127,   0,   0 );   
    else
    if( m_TeamBits == 0xFFFFFFFF )        Color.Set(   0, 255, 255 );   
    else
    {
        u32         ContextBits = 0x00;
        object::id  PlayerID( tgl.PC[tgl.WC].PlayerIndex, -1 );
        object*     pObject = ObjMgr.GetObjectFromID( PlayerID );
        if( pObject )
            ContextBits = pObject->GetTeamBits();

        if( ContextBits & m_TeamBits )    Color.Set(   0, 255,  0 );
        else                              Color.Set( 255,   0,  0 );  
    }   

    //----------------------------------------------------------------------

    xcolor   F( tgl.Fog.ComputeFog( (m_Vertex[0] + m_Vertex[6]) * 0.5f ) );
    ps2color C( Color.R, Color.G, Color.B, (255 - F.A) >> Shift );
    f32      H = x_frand( 0.0f, 1.0f ) - 8.0f;
    f32      V = m_Slide - 8.0f;
    f32      h, v;
    vector2  T[12];

    h = m_WorldScale.X * HScalar;
    v = m_WorldScale.Z * VScalar;

    T[ 0].X = H;              T[ 0].Y = V;
    T[ 1].X = H;              T[ 1].Y = V + v;
    T[ 2].X = H + h;          T[ 2].Y = V + v;
    T[ 3].X = H + h;          T[ 3].Y = V;

    h = m_WorldScale.Z * HScalar;
    v = m_WorldScale.Y * VScalar;

    T[ 4].X = H;              T[ 4].Y = V;
    T[ 5].X = H;              T[ 5].Y = V + v;
    T[ 6].X = H + h;          T[ 6].Y = V + v;                  
    T[ 7].X = H + h;          T[ 7].Y = V;

    h = m_WorldScale.X * HScalar;
    v = m_WorldScale.Y * VScalar;

    T[ 8].X = H;              T[ 8].Y = V;
    T[ 9].X = H;              T[ 9].Y = V + v;
    T[10].X = H + h;          T[10].Y = V + v;                  
    T[11].X = H + h;          T[11].Y = V;

    //----------------------------------------------------------------------

    s32 VMap[36] = {  0, 1, 2,  2, 3, 0,   4, 7, 6,  6, 5, 4,
                      5, 1, 0,  0, 4, 5,   6, 7, 3,  3, 2, 6,
                      4, 0, 3,  3, 7, 4,   5, 6, 2,  2, 1, 5, };
                     
    s32 TMap[36] = {  0, 1, 2,  2, 3, 0,   0, 3, 2,  2, 1, 0,
                      4, 5, 6,  6, 7, 4,   4, 7, 6,  6, 5, 4,
                      8, 9,10, 10,11, 8,   8,11,10, 10, 9, 8, };
                     
    //----------------------------------------------------------------------
    
    s32 i;
    
    for( i = 0; i < 36; i++ )   pV[i] = m_Vertex[ VMap[i] ];
    for( i = 0; i < 36; i++ )   pT[i] = T       [ TMap[i] ];
    for( i = 0; i < 36; i++ )   pC[i] = C;            
    
    //----------------------------------------------------------------------

    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE( C_SRC, C_ZERO, A_SRC, C_DST ) );
    gsreg_SetZBufferUpdate( FALSE );
    gsreg_SetClamping( FALSE );
    gsreg_End();

    poly_Begin( POLY_USE_ALPHA );
    vram_Activate( m_Bitmap );
    poly_Render( pV, pT, (ps2color*)pC, 36 );
    poly_End();

    gsreg_Begin();
    gsreg_SetZBufferUpdate( TRUE );
    gsreg_End();
}

//==============================================================================
#endif // TARGET_PS2
//==============================================================================

//==============================================================================
#ifdef TARGET_PC
//==============================================================================

void force_field::Render( void )
{
    // No power?
    if( !GameMgr.GetPower( m_Power ) )
        return;

    // Visible?
    s32 Visible = IsInViewVolume( m_WorldBBox );
    if( !Visible )
        return;

    // Decide on the color.

    xcolor Color = XCOLOR_YELLOW;

    if( m_TeamBits == 0x00000000 )        Color.Set( 255,   0, 127 );   
    else
    if( m_TeamBits == 0xFFFFFFFF )        Color.Set(   0, 255, 127 );   
    else
    {
        u32         ContextBits = 0x00;
        object::id  PlayerID( tgl.PC[tgl.WC].PlayerIndex, -1 );
        object*     pObject = ObjMgr.GetObjectFromID( PlayerID );
        if( pObject )
            ContextBits = pObject->GetTeamBits();

        if( ContextBits & m_TeamBits )    Color.Set(   0, 255,  0 );
        else                              Color.Set( 255,   0,  0 );  
    }   
            
    //----------------------------------------------------------------------

    f32      X = m_WorldScale.X * VScalar;
    f32      Y = m_WorldScale.Y * VScalar;
    f32      Z = m_WorldScale.Z * VScalar;
    f32      H = x_frand( 0.0f, 1.0f ) - 8.0f;
    f32      V = m_Slide - 8.0f;

    xcolor   F( tgl.Fog.ComputeFog( (m_Vertex[0] + m_Vertex[6]) * 0.5f ) );

    Color.A = (255 - F.A);

    //----------------------------------------------------------------------

    draw_Begin( DRAW_QUADS, DRAW_TEXTURED | DRAW_USE_ALPHA );
    draw_SetTexture( m_Bitmap );

    draw_Color( Color );

    draw_UV( H   , V   );          draw_Vertex( m_Vertex[0] );
    draw_UV( H   , V+Z );          draw_Vertex( m_Vertex[1] );
    draw_UV( H+X , V+Z );          draw_Vertex( m_Vertex[2] );
    draw_UV( H+X , V   );          draw_Vertex( m_Vertex[3] );
                         
    draw_UV( H+X , V   );          draw_Vertex( m_Vertex[7] );
    draw_UV( H+X , V+Z );          draw_Vertex( m_Vertex[6] );
    draw_UV( H   , V+Z );          draw_Vertex( m_Vertex[5] );
    draw_UV( H   , V   );          draw_Vertex( m_Vertex[4] );
                         
    draw_UV( H   , V   );          draw_Vertex( m_Vertex[0] );
    draw_UV( H   , V+Y );          draw_Vertex( m_Vertex[4] );
    draw_UV( H+Z , V+Y );          draw_Vertex( m_Vertex[5] );
    draw_UV( H+Z , V   );          draw_Vertex( m_Vertex[1] );
                         
    draw_UV( H   , V   );          draw_Vertex( m_Vertex[1] );
    draw_UV( H   , V+Y );          draw_Vertex( m_Vertex[5] );
    draw_UV( H+X , V+Y );          draw_Vertex( m_Vertex[6] );
    draw_UV( H+X , V   );          draw_Vertex( m_Vertex[2] );
                         
    draw_UV( H   , V   );          draw_Vertex( m_Vertex[2] );
    draw_UV( H   , V+Y );          draw_Vertex( m_Vertex[6] );
    draw_UV( H+Z , V+Y );          draw_Vertex( m_Vertex[7] );
    draw_UV( H+Z , V   );          draw_Vertex( m_Vertex[3] );
                          
    draw_UV( H   , V   );          draw_Vertex( m_Vertex[3] );
    draw_UV( H   , V+Y );          draw_Vertex( m_Vertex[7] );
    draw_UV( H+X , V+Y );          draw_Vertex( m_Vertex[4] );
    draw_UV( H+X , V   );          draw_Vertex( m_Vertex[0] );

    draw_End();  
}

//==============================================================================
#endif // TARGET_PC
//==============================================================================

DEDICATED_SERVER
DEDICATED_SERVER
DEDICATED_SERVER */