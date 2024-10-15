#include "Material.hpp"
#include "Texture.hpp"
#include "TextureManager.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Shape.hpp"
#include "PS2_Shape.hpp"    // For micro code and building dlists
#include "aux_Bitmap.hpp"
#include "ObjectMgr\collider.hpp"


// Need all versions of machine specific code for building display lists
#include "Shaders/D3D_Shape.hpp"
#include "MCode/PS2_Shape.hpp"




// For building dlists
#ifndef TARGET_PS2

#include "PS2\ps2_dlist.hpp" 
#include "PS2\ps2_vifgif.hpp"

// These are from C:\PROJECTS\T2\XCORE\3RDPARTY\PS2\Sony\sce\ee\include\eestruct.h
#define SCE_VIF1_SET_OFFSET(offset, irq)    ((u_int)(offset) | ((u_int)0x02 << 24) | ((u_int)(irq) << 31))
#define SCE_VIF1_SET_BASE(base, irq)        ((u_int)(base) | ((u_int)0x03 << 24) | ((u_int)(irq) << 31))
#define SCE_VIF1_SET_FLUSHE(irq)            (((u_int)0x10 << 24) | ((u_int)(irq) << 31))
#define SCE_VIF1_SET_MSCAL(vuaddr, irq)     ((u_int)(vuaddr) | ((u_int)0x14 << 24) | ((u_int)(irq) << 31))
#define SCE_VIF1_SET_NOP(irq)               ((u_int)(irq) << 31)
#define SCE_VIF1_SET_STMOD(stmod, irq)      ((u_int)(stmod) | ((u_int)0x05 << 24) | ((u_int)(irq) << 31))

#endif


//==============================================================================
// Static variables
//==============================================================================
#ifdef X_DEBUG
    s32     material::s_LoadSettings  = 0 ;
    s32     material::s_LoadColor     = 0 ;
    s32     material::s_LoadTex       = 0 ;
    s32     material::s_DListDraw     = 0 ;
#endif


//==============================================================================
// Useful macros
//==============================================================================

#define ASSERT_ALIGNED4(__addr__)    ASSERT(((s32)(__addr__) & 3) == 0) ;
#define ASSERT_ALIGNED16(__addr__)   ASSERT(((s32)(__addr__) & 15) == 0) ;


//==============================================================================
// Simple display list builder class
//==============================================================================

class dlist_builder
{
    byte* m_Addr;
    byte* m_StartAddr;
    byte* m_EndAddr;
    s32   m_Size;
  
public:
            dlist_builder   (void) ;
    void    Setup           ( byte* StartAddr, s32 Size ) ;
    void*   Alloc           ( s32 Size ) ;
    void    Align16         ( void ) ;
    byte*   GetStartAddr    ( void ) ;
	s32     GetLength       ( void ) ;
};

//==============================================================================

dlist_builder::dlist_builder( void )
{
    m_StartAddr = 0;
    m_Size      = 0;
    m_EndAddr   = 0;
    m_Addr      = 0;
}

//==============================================================================

void dlist_builder::Setup( byte* StartAddr, s32 aSize )
{
    m_StartAddr = StartAddr;
    m_Size      = aSize;
    m_EndAddr   = StartAddr + aSize;
    m_Addr      = StartAddr;

    ASSERT(((s32)StartAddr & 0xF) == 0) ;

}

//==============================================================================

void* dlist_builder::Alloc( s32 Size )
{
    byte* AllocAddr;

    //shape::printf("dlist_builder::Alloc(%d)\n", Size) ;
    
    AllocAddr = m_Addr;
    m_Addr += Size;
    ASSERT( m_Addr < m_EndAddr );
    return AllocAddr;
}

//==============================================================================

void dlist_builder::Align16( void )
{
    while( ((u32)m_Addr) & 0xF )
        *m_Addr++ = SCE_VIF1_SET_NOP(0) ;

    ASSERT( m_Addr < m_EndAddr );
    
    ASSERT((GetLength() & 0xF) == 0) ;
}

//==============================================================================

byte* dlist_builder::GetStartAddr( void )
{
    return m_StartAddr;
}

//==============================================================================

s32 dlist_builder::GetLength( void )
{
	return (s32)(m_Addr - m_StartAddr);
}

//==============================================================================


//==============================================================================
// Strip class
//==============================================================================

// Constructor/destructor
dlist_strip::dlist_strip()
{
    // Must be 16 byte aligned for shape_bin_file_class
    ASSERT((sizeof(dlist_strip) & 15) == 0) ;

    m_nVerts    = 0 ;       // Total verts in strip
    m_Pos       = NULL ;    // List of positions
    m_TexCoord  = NULL ;    // List of uv's
    m_Normal    = NULL ;    // List of normals
}

//==============================================================================

dlist_strip::~dlist_strip()
{
}

//==============================================================================

// Operators
dlist_strip&  dlist_strip::operator = ( dlist_strip& Strip )
{
    m_nVerts    = Strip.m_nVerts ;
    m_Pos       = Strip.m_Pos ;
    m_TexCoord  = Strip.m_TexCoord ;
    m_Normal    = Strip.m_Normal ;

    return *this ;
}

//==============================================================================

// Initialization
void dlist_strip::Setup( s32 nVerts, void* Pos, void* TexCoord, void* Normal)
{
    ASSERT(nVerts) ;
    m_nVerts    = nVerts ;
    m_Pos       = Pos ;
    m_TexCoord  = TexCoord ;
    m_Normal    = Normal ;
}

//==============================================================================

void dlist_strip::AddOffset( s32 Offset )
{
    m_Pos      = (void*)((s32)m_Pos      + Offset) ;    
    m_Normal   = (void*)((s32)m_Normal   + Offset) ;    

    // Tex co-ords are special case - they may not be any
    if (m_TexCoord != NULL)
        m_TexCoord = (void*)((s32)m_TexCoord + Offset) ;    
}

//==============================================================================

// Vert and poly access
s32 dlist_strip::GetVertCount( void )
{
    return m_nVerts ;
}

//==============================================================================

void dlist_strip::GetVert( s32 VertIndex, vector3& Pos, vector3& Norm, vector2& TexCoord )
{
    ASSERT(VertIndex >= 0) ;
    ASSERT(VertIndex < GetVertCount()) ;

#ifdef TARGET_PS2
    ps2_pos&        VertPos  = ((ps2_pos*)m_Pos)[VertIndex]; 
    Pos.X = (f32)VertPos.x ;
    Pos.Y = (f32)VertPos.y ;
    Pos.Z = (f32)VertPos.z ;

    ps2_norm&       VertNorm = ((ps2_norm*)m_Normal)[VertIndex]; 
    Norm.X = (f32)VertNorm.x * (1.0f / 127.0f) ;
    Norm.Y = (f32)VertNorm.y * (1.0f / 127.0f) ;
    Norm.Z = (f32)VertNorm.z * (1.0f / 127.0f) ;

    // Texture co-ords may not be present! eg. collision models
    if (m_TexCoord)
    {
        ps2_tex_coord&  VertTex  = ((ps2_tex_coord*)m_TexCoord)[VertIndex]; 
        TexCoord.X = (f32)VertTex.u ;
        TexCoord.Y = (f32)VertTex.v ;
    }
    else
    {
        TexCoord.X = 0 ;
        TexCoord.Y = 0 ;
    }
#endif

#ifdef TARGET_PC
    d3d_vert&   D3DVert = ((d3d_vert*)m_Pos)[VertIndex] ;

    Pos      = D3DVert.vPos ;
    Norm     = D3DVert.vNormal ;
    TexCoord = D3DVert.TexCoord ;
#endif
}

//==============================================================================

void dlist_strip::SetVert( s32 VertIndex, vector3& Pos, vector3& Norm, vector2& TexCoord )
{
    ASSERT(VertIndex >= 0) ;
    ASSERT(VertIndex < GetVertCount()) ;

#ifdef TARGET_PS2
    ps2_pos&        VertPos  = ((ps2_pos*)m_Pos)[VertIndex]; 
    ps2_norm&       VertNorm = ((ps2_norm*)m_Normal)[VertIndex]; 
    ps2_tex_coord&  VertTex  = ((ps2_tex_coord*)m_TexCoord)[VertIndex]; 
    
    VertPos.x = (s16)Pos.X ;
    VertPos.y = (s16)Pos.Y ;
    VertPos.z = (s16)Pos.Z ;

    VertNorm.x = (s8)Norm.X ;
    VertNorm.y = (s8)Norm.Y ;
    VertNorm.z = (s8)Norm.Z ;

    VertTex.u = (s16)TexCoord.X ;
    VertTex.v = (s16)TexCoord.Y ;
#endif

#ifdef TARGET_PC
    d3d_vert&   D3DVert = ((d3d_vert*)m_Pos)[VertIndex] ;

    D3DVert.vPos     = Pos ;
    D3DVert.vNormal  = Norm ;
    D3DVert.TexCoord = TexCoord ;
#endif
}

//==============================================================================

s32 dlist_strip::GetPolyCount( void )
{
    return m_nVerts-2 ;
}

//==============================================================================

xbool dlist_strip::GetPoly( s32 PolyIndex, vector3* pVert )
{
    ASSERT(PolyIndex >= 0) ;
    ASSERT(PolyIndex < GetPolyCount()) ;

#ifdef TARGET_PS2
    ps2_pos* Pos = &((ps2_pos*)m_Pos)[PolyIndex]; 
    
    // No kick?
    if( (Pos[2].adc & (1<<15)) != 0)
        return FALSE ;

    // Which orientation?
    if (Pos[2].adc & (1<<14))
    {
        pVert[0].X    = (f32)Pos[1].x ;
        pVert[0].Y    = (f32)Pos[1].y ;
        pVert[0].Z    = (f32)Pos[1].z ;

        pVert[1].X    = (f32)Pos[0].x ;
        pVert[1].Y    = (f32)Pos[0].y ;
        pVert[1].Z    = (f32)Pos[0].z ;

        pVert[2].X    = (f32)Pos[2].x ;
        pVert[2].Y    = (f32)Pos[2].y ;
        pVert[2].Z    = (f32)Pos[2].z ;
    }
    else
    {
        pVert[0].X    = (f32)Pos[0].x ;
        pVert[0].Y    = (f32)Pos[0].y ;
        pVert[0].Z    = (f32)Pos[0].z ;

        pVert[1].X    = (f32)Pos[1].x ;
        pVert[1].Y    = (f32)Pos[1].y ;
        pVert[1].Z    = (f32)Pos[1].z ;

        pVert[2].X    = (f32)Pos[2].x ;
        pVert[2].Y    = (f32)Pos[2].y ;
        pVert[2].Z    = (f32)Pos[2].z ;
    }
#endif

#ifdef TARGET_PC
    d3d_vert*   pD3DVert = &((d3d_vert*)m_Pos)[PolyIndex] ;

    // No kick (1st bit of y pos)?
    if ((*(u32*)&pD3DVert[2].vPos.Y) & 1)
        return FALSE ;

    // Which orientation? (1st bit of y pos)
    if ((*(u32*)&pD3DVert[2].vPos.X) & 1)
    {
        pVert[0] = pD3DVert[1].vPos;
        pVert[1] = pD3DVert[0].vPos;
        pVert[2] = pD3DVert[2].vPos;
    }
    else
    {
        pVert[0] = pD3DVert[0].vPos;
        pVert[1] = pD3DVert[1].vPos;
        pVert[2] = pD3DVert[2].vPos;
    }
#endif

    return TRUE ;
}

//==============================================================================

// Fast collision function - 
// Integer version (currently only used on PS2)
s32 dlist_strip::ApplyCollision( collider& Collider, const matrix4& DListL2W, const dlist_strip::ibbox& LocalBBox)
{
    s32     PolysSent = 0 ;

#ifdef TARGET_PS2
    // Locals
    vector3                 Verts[3] ;
    s32                     V ;

    // Get poly count and first vert
    s32        PolyCount = GetPolyCount() ;
    ps2_pos*   pPos      = (ps2_pos*)m_Pos ;
    
    // Loop over all polys
    while(PolyCount--)
    {
        // No kick?
        if( (pPos[2].adc & (1<<15)) != 0) goto SKIP_POLY ;

        // Trivial rejection - check integer bbox
        V = pPos[0].x;
        if( pPos[1].x < V ) V = pPos[1].x;
        if( pPos[2].x < V ) V = pPos[2].x;
        if( LocalBBox.Max.X < V ) goto SKIP_POLY ;
        V = pPos[0].x;
        if( pPos[1].x > V ) V = pPos[1].x;
        if( pPos[2].x > V ) V = pPos[2].x;
        if( LocalBBox.Min.X > V ) goto SKIP_POLY ;
        V = pPos[0].z;
        if( pPos[1].z < V ) V = pPos[1].z;
        if( pPos[2].z < V ) V = pPos[2].z;
        if( LocalBBox.Max.Z < V ) goto SKIP_POLY ;
        V = pPos[0].y;
        if( pPos[1].y > V ) V = pPos[1].y;
        if( pPos[2].y > V ) V = pPos[2].y;
        if( LocalBBox.Min.Y > V ) goto SKIP_POLY ;
        V = pPos[0].y;
        if( pPos[1].y < V ) V = pPos[1].y;
        if( pPos[2].y < V ) V = pPos[2].y;
        if( LocalBBox.Max.Y < V ) goto SKIP_POLY ;
        V = pPos[0].z;
        if( pPos[1].z > V ) V = pPos[1].z;
        if( pPos[2].z > V ) V = pPos[2].z;
        if( LocalBBox.Min.Z > V ) goto SKIP_POLY ;

        // Which orientation?
        if (pPos[2].adc & (1<<14))
        {
            Verts[0].X    = (f32)pPos[1].x ;
            Verts[0].Y    = (f32)pPos[1].y ;
            Verts[0].Z    = (f32)pPos[1].z ;

            Verts[1].X    = (f32)pPos[0].x ;
            Verts[1].Y    = (f32)pPos[0].y ;
            Verts[1].Z    = (f32)pPos[0].z ;

            Verts[2].X    = (f32)pPos[2].x ;
            Verts[2].Y    = (f32)pPos[2].y ;
            Verts[2].Z    = (f32)pPos[2].z ;
        }
        else
        {
            Verts[0].X    = (f32)pPos[0].x ;
            Verts[0].Y    = (f32)pPos[0].y ;
            Verts[0].Z    = (f32)pPos[0].z ;

            Verts[1].X    = (f32)pPos[1].x ;
            Verts[1].Y    = (f32)pPos[1].y ;
            Verts[1].Z    = (f32)pPos[1].z ;

            Verts[2].X    = (f32)pPos[2].x ;
            Verts[2].Y    = (f32)pPos[2].y ;
            Verts[2].Z    = (f32)pPos[2].z ;
        }

        // Transform into world space and do full collision
        PolysSent++;
        DListL2W.Transform( Verts, Verts, 3 );
        Collider.ApplyNGon( Verts, 3 ) ;

        // Early out?
        if (Collider.GetEarlyOut())
            return PolysSent ;

        // Next poly (simply goto next vert in strip)
        SKIP_POLY:
            pPos++ ;
    }

#else

    // To stop warnings
    (void)Collider ;
    (void)DListL2W ;
    (void)LocalBBox ;

#endif

    // Tell caller how many polys went to the collider
    return PolysSent ;
}

//==============================================================================

// Fast collision function - 
// Floating point version (currently only used on PC)
s32 dlist_strip::ApplyCollision( collider& Collider, const matrix4& DListL2W, const bbox& LocalBBox)
{
    s32     PolysSent = 0 ;

#ifdef TARGET_PC
    // Locals
    vector3                 Verts[3] ;
    f32                     V ;
    
    // Get poly count and first vert
    s32                     PolyCount = GetPolyCount() ;
    d3d_vert*  pD3DVert  = (d3d_vert*)m_Pos ;
    
    // Loop over all polys
    while(PolyCount--)
    {
        // No kick (1st bit of y pos)?
        if( ((*(u32*)&pD3DVert[2].vPos.Y) & 1) ) goto SKIP_POLY;

        // Trivial rejection - check floating point bbox
        V = pD3DVert[0].vPos.X;
        if( pD3DVert[1].vPos.X < V ) V = pD3DVert[1].vPos.X;
        if( pD3DVert[2].vPos.X < V ) V = pD3DVert[2].vPos.X;
        if( LocalBBox.Max.X < V ) goto SKIP_POLY ;
        V = pD3DVert[0].vPos.X;
        if( pD3DVert[1].vPos.X > V ) V = pD3DVert[1].vPos.X;
        if( pD3DVert[2].vPos.X > V ) V = pD3DVert[2].vPos.X;
        if( LocalBBox.Min.X > V ) goto SKIP_POLY ;
        V = pD3DVert[0].vPos.Z;
        if( pD3DVert[1].vPos.Z < V ) V = pD3DVert[1].vPos.Z;
        if( pD3DVert[2].vPos.Z < V ) V = pD3DVert[2].vPos.Z;
        if( LocalBBox.Max.Z < V ) goto SKIP_POLY ;
        V = pD3DVert[0].vPos.Y;
        if( pD3DVert[1].vPos.Y > V ) V = pD3DVert[1].vPos.Y;
        if( pD3DVert[2].vPos.Y > V ) V = pD3DVert[2].vPos.Y;
        if( LocalBBox.Min.Y > V ) goto SKIP_POLY ;
        V = pD3DVert[0].vPos.Y;
        if( pD3DVert[1].vPos.Y < V ) V = pD3DVert[1].vPos.Y;
        if( pD3DVert[2].vPos.Y < V ) V = pD3DVert[2].vPos.Y;
        if( LocalBBox.Max.Y < V ) goto SKIP_POLY ;
        V = pD3DVert[0].vPos.Z;
        if( pD3DVert[1].vPos.Z > V ) V = pD3DVert[1].vPos.Z;
        if( pD3DVert[2].vPos.Z > V ) V = pD3DVert[2].vPos.Z;
        if( LocalBBox.Min.Z > V ) goto SKIP_POLY ;

        // Which orientation? (1st bit of x pos)
        if ((*(u32*)&pD3DVert[2].vPos.X) & 1)
        {
            Verts[0] = pD3DVert[1].vPos;
            Verts[1] = pD3DVert[0].vPos;
            Verts[2] = pD3DVert[2].vPos;
        }
        else
        {
            Verts[0] = pD3DVert[0].vPos;
            Verts[1] = pD3DVert[1].vPos;
            Verts[2] = pD3DVert[2].vPos;
        }

        // Transform into world space and do full collision
        PolysSent++;
        DListL2W.Transform( Verts, Verts, 3 );
        Collider.ApplyNGon( Verts, 3 ) ;

        // Early out?
        if (Collider.GetEarlyOut())
            return PolysSent ;

        // Next poly (simply goto next vert in strip)
        SKIP_POLY:
            pD3DVert++ ;
    }

#else

    // To stop warnings
    (void)Collider ;
    (void)DListL2W ;
    (void)LocalBBox ;

#endif

    // Tell caller how many polys went to the collider
    return PolysSent ;
}

//==============================================================================

// File IO
void dlist_strip::ReadWrite   (shape_bin_file &File)
{
    File.ReadWrite(m_nVerts) ;
    File.ReadWrite(m_Pos) ;
    File.ReadWrite(m_TexCoord) ;
    File.ReadWrite(m_Normal) ;
}


//==============================================================================
// Material display list class
//==============================================================================

// Constructor
material_dlist::material_dlist()
{
    // Must be 16 byte aligned for shape_bin_file_class
    ASSERT((sizeof(material_dlist) & 15) == 0) ;

    m_Node      = -1 ;

    m_DList     = NULL ;
    m_DListSize = 0 ;

    m_nStrips   = 0 ;
    m_Strips    = NULL ;

    m_pVB       = NULL ;    // Vertex buffer
}

//==============================================================================

// Destructor
material_dlist::~material_dlist()
{
    if (m_DList)
        delete [] m_DList ;

    if (m_Strips)
        delete [] m_Strips ;

    if (m_pVB)
    {
        #ifdef TARGET_PC
        m_pVB->Release() ;
        #endif
    }
}

//==============================================================================

// Sets up display list
void material_dlist::Setup(s32 Node, s32 nPolys, s32 VertSize, byte *DList, s32 Size)
{
    s32 i ;

    #ifdef TARGET_PS2
        // Must be 16 byte aligned for DMA
        ASSERT((Size & 0xF) == 0) ;
    #endif

    ASSERT(nPolys) ;
	ASSERT(VertSize) ;
	ASSERT(DList) ;
    ASSERT(Size) ;

    // Create copy of dlist
    m_Node      = Node ;
	m_nPolys	= nPolys ;
	m_VertSize	= VertSize ;
    m_DList     = new byte[Size] ;
    ASSERT(m_DList) ;
    m_DListSize = Size ;
    x_memcpy(m_DList, DList, Size) ;

    // Convert dlist strip offsets into pointers
    for (i = 0 ; i < m_nStrips ; i++)
        m_Strips[i].AddOffset((s32)m_DList) ;
}

//==============================================================================

// Returns total memory used
s32 material_dlist::MemoryUsed()
{
    return sizeof(material_dlist) + m_DListSize ;
}

//==============================================================================

// On the PC this creates the vertex buffers
void material_dlist::InitializeForDrawing( void )
{
    #ifdef TARGET_PC

        // Already done?
        if (m_pVB)
            return ;

        // Locals
        dxerr   Error ;
        byte*   Buffer ;

        // Create vertex buffer
        Error = g_pd3dDevice->CreateVertexBuffer(m_DListSize,
                                                 D3DUSAGE_WRITEONLY, 
                                                 0, 
                                                 D3DPOOL_MANAGED, 
                                                 &m_pVB) ;
        ASSERT(Error == 0) ;
        ASSERT(m_pVB) ;

        // Copy all the verts (for PC they are at the m_Pos address)
        Error = m_pVB->Lock( 0, 0, &Buffer, 0);
        ASSERT(Error == 0);
        ASSERT(Buffer) ;
        ASSERT(m_DList) ;
        memcpy( Buffer, m_DList, m_DListSize ) ;
        m_pVB->Unlock();

    #endif
}

//==============================================================================

void material_dlist::ReadWrite(shape_bin_file &File)
{
    s32 i ;

    // Attributes
    File.ReadWrite(m_Node) ;
    File.ReadWrite(m_nPolys) ;
    File.ReadWrite(m_VertSize) ;

    // DList
    File.ReadWriteStructArray(m_DList, m_DListSize) ;

    // Turn pointers into offsets
    for (i = 0 ; i < m_nStrips ; i++)
        m_Strips[i].AddOffset(-(s32)m_DList) ;

    // Strips
    File.ReadWriteClassArray(m_Strips, m_nStrips) ;

    // Turn offset into pointers
    for (i = 0 ; i < m_nStrips ; i++)
        m_Strips[i].AddOffset((s32)m_DList) ;

    #ifdef TARGET_PS2
        // Must be 16 byte aligned for DMA
        ASSERT(((s32)m_DList & 0xF) == 0) ;
        ASSERT((m_DListSize & 0xF) == 0) ;
    #endif
}

//==============================================================================

// Adds a strip onto end of strip arrary
void material_dlist::AddStrip(dlist_strip &Strip)
{
    s32 i ;

    // Allocate space for new strips
    s32          nNewStrips = m_nStrips + 1 ;
    dlist_strip* NewStrips = new dlist_strip[nNewStrips] ;
    ASSERT(NewStrips) ;

    // Copy old strips
    for (i = 0 ; i < m_nStrips ; i++)
        NewStrips[i] = m_Strips[i] ;

    // Copy new strip
    NewStrips[m_nStrips] = Strip ;

    // Delete old strip data
    if (m_Strips)
    {
        ASSERT(m_nStrips) ;
        delete [] m_Strips ;
    }

    // Use new strip data
    m_nStrips = nNewStrips ;
    m_Strips  = NewStrips ;
}

//==============================================================================

s32 material_dlist::GetStripCount( void )
{
    return m_nStrips ;
}

//==============================================================================

dlist_strip& material_dlist::GetStrip( s32 StripIndex )
{
    ASSERT(StripIndex >= 0) ;
    ASSERT(StripIndex < m_nStrips ) ;

    return m_Strips[StripIndex] ;
}

//==============================================================================



//==============================================================================
// material class
//==============================================================================

// Constructor
material::material()
{
    // Must be 16 byte aligned for shape_bin_file_class
    ASSERT((sizeof(material) & 15) == 0) ;

    // Initialize to be valid
    x_strcpy(m_Name, "undefined") ;

    m_nStrips   = 0 ;
    m_Strips    = NULL ;

    m_nDLists   = 0 ;
    m_DLists    = NULL ;

    m_ID        = -1 ;

    m_NodeFlags = 0 ;

    // Clear texture indices
    for (s32 i = 0 ; i < TEXTURE_TYPE_TOTAL ; i++)
    {
        SetTextureInfo((texture_type)i, -1,0) ;
        SetTextureRef((texture_type)i,-1) ;
    }

    // Init flags
    SetUMirrorFlag      (FALSE) ;
    SetUWrapFlag        (FALSE) ;
    SetVMirrorFlag      (FALSE) ;
    SetVWrapFlag        (FALSE) ;
    SetSelfIllumFlag    (FALSE) ;
    SetHasAlphaFlag		(FALSE) ;
    SetAdditiveFlag		(FALSE) ;
    SetPunchThroughFlag	(FALSE) ;
}

//==============================================================================

// Destructor
material::~material()
{
    // Cleanup
    if (m_Strips)
        delete [] m_Strips ;

    if (m_DLists)
        delete [] m_DLists ;
}


//==============================================================================

void material::SetName(const char *Name)
{
    // If string length okay, just copy it
    if (x_strlen(Name) < MAX_NAME_LENGTH)
        x_strcpy(m_Name, Name) ;
    else
    {
        // Copy and truncate string
        x_memcpy(m_Name, Name, MAX_NAME_LENGTH-1) ;
        m_Name[MAX_NAME_LENGTH-1] = 0 ; // End string
    }
}

//==============================================================================

// Returns memory used
s32 material::MemoryUsed()
{
    s32 i ;
    s32 Size = sizeof(material) ;
    
    // Strips
    for (i = 0 ; i < m_nStrips ; i++)
        Size += m_Strips[i].MemoryUsed() ;

    // DLists
    for (i = 0 ; i < m_nDLists ; i++)
        Size += m_DLists[i].MemoryUsed() ;

    return Size ;
}

//==============================================================================

// Call after parsing ascii file
void material::CleanupUnusedMemory()
{
    // Make sure DList was built
    // ASSERT(m_nDLists) ;
    // TO DO - Fix up max exporter to delete any materials that have no geometry
    // For now don't worry about this anymore - some materials may not have any geometry attached
    
    // Free strips
    if (m_Strips)
    {
        delete [] m_Strips ;
        m_nStrips = 0 ;
        m_Strips  = NULL ;
    }
}

//==============================================================================

// Allocated strips
void material::SetStripCount(s32 Count)
{
    ASSERT(Count >= 0) ;

    // Free old strips
    if (m_Strips)
    {
        delete [] m_Strips ;
        m_Strips = NULL ;
    }

    // Allocate new strips
    if (Count)
    {
        m_Strips = new strip[Count] ;
        ASSERT(m_Strips) ;
    }

    m_nStrips = Count ;
}

//==============================================================================

// Returns total size of dlists
s32 material::GetDListSize()
{
    s32 Size = 0 ;
    for (s32 i = 0 ; i < m_nDLists ; i++)
        Size += m_DLists[i].GetDListSize() ;

    return Size ;
}

//==============================================================================

// On the PC this creates the vertex buffers
void material::InitializeForDrawing( void )
{
    // Prepare all display lists
    for (s32 i = 0 ; i < m_nDLists ; i++)
        m_DLists[i].InitializeForDrawing() ;
}

//==============================================================================

// Sends and draws geometry via VIF and VU1
void material::Draw(const material_draw_info& DrawInfo, s32 &CurrentLoadedNode)
{
    #ifdef TARGET_PC
        // Draw all dlists
        for (s32 i = 0 ; i < m_nDLists ; i++)
            m_DLists[i].Draw(*this, DrawInfo, CurrentLoadedNode) ;
    #endif

    #ifdef TARGET_PS2
        // Draw all dlists
        for (s32 i = 0 ; i < m_nDLists ; i++)
            m_DLists[i].Draw(DrawInfo, CurrentLoadedNode) ;

        // Wait for GS to finish
        vram_Sync() ;
    #endif
}

//==============================================================================

void material::CreateNewTextureName(char* NewName, const char* String, const texture& TEX)
{
    // Split up texture name
    char Drive[X_MAX_DRIVE];  
    char Dir  [X_MAX_DIR];  
    char FName[X_MAX_FNAME];  
    char Ext  [X_MAX_EXT];  
    x_splitpath( TEX.GetName(), Drive, Dir, FName, Ext) ;

    // Create new file name portion
    char NewFName[X_MAX_FNAME] ;
    x_strcpy(NewFName, String) ;
    x_strcat(NewFName, FName) ;

    // Finally rebuild into NewName
    x_makepath(NewName, Drive, Dir, NewFName, Ext) ;

    // Check
    ASSERT(x_strlen(NewName) < TEXTURE_MAX_FNAME) ;
}

//==============================================================================

void material::CreateNewTextureName(char* NewName, const texture& TEXA, const char* String, const texture& TEXB)
{
    // Get TEXA filename
    char FNameA[X_MAX_FNAME];  
    x_splitpath( TEXA.GetName(), NULL, NULL, FNameA, NULL) ;

    // Remove digits from end of name so that animated diffuse maps work!
    s32 i = x_strlen(FNameA)-1 ;
    while((i >= 0) && (FNameA[i] >= '0') && (FNameA[i] <= '9'))
        FNameA[i--] = 'X' ;

    // Split up texture B name
    char FNameB[X_MAX_FNAME];  
    char Drive[X_MAX_DRIVE];  
    char Dir  [X_MAX_DIR];  
    char Ext  [X_MAX_EXT];  
    x_splitpath( TEXB.GetName(), Drive, Dir, FNameB, Ext) ;

    // Create new file name portion
    char NewFName[X_MAX_FNAME] ;
    x_strcpy(NewFName, FNameA) ;
    x_strcat(NewFName, String) ;
    x_strcat(NewFName, FNameB) ;

    // Finally rebuild into NewName
    x_makepath(NewName, Drive, Dir, NewFName, Ext) ;

    // Check
    ASSERT(x_strlen(NewName) < TEXTURE_MAX_FNAME) ;
}

//==============================================================================

// Check for punch through alpha texture (ie. it only has 2 values!)
void material::CheckForPunchThrough( model&                     Model, 
                                     texture_manager&           TextureManager, 
                                     const shape_load_settings& LoadSettings )
{
    (void)Model ;
    xbool   AlphaBurned        = FALSE ;
    s32     NewDiffuseIndex = -1 ;

    // Loop through all frames!
    s32 Count = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Count ;
    for (s32 Frame = 0 ; Frame < Count ; Frame++)
    {
        // Get 1st texture
        s32 AlphaIndex   = GetTextureInfo(TEXTURE_TYPE_ALPHA).Index ;
        s32 DiffuseIndex = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Index ;
        s32 NewTexIndex     = -1 ;

        // Both present and not already burned this baby?
        // And textures aren't the same ie. alpha intensity required
        if ((AlphaIndex != -1) && (DiffuseIndex != -1) && (AlphaIndex != DiffuseIndex))
        {
            // Goto correct frame
            DiffuseIndex += Frame % GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Count ;
            AlphaIndex   += Frame % GetTextureInfo(TEXTURE_TYPE_ALPHA).Count ;

            // Get textures
            texture& AlphaTEX   = TextureManager.GetTexture(AlphaIndex) ;
            texture& DiffuseTEX = TextureManager.GetTexture(DiffuseIndex) ;

            // Get bitmaps
            xbitmap& AlphaBMP    = AlphaTEX.GetXBitmap() ;
            xbitmap& DiffuseBMP  = DiffuseTEX.GetXBitmap() ;

            // Get sizes
            s32 AlphaWidth    = AlphaBMP.GetWidth() ;
            s32 AlphaHeight   = AlphaBMP.GetHeight() ;
            s32 DiffuseWidth  = DiffuseBMP.GetWidth() ;
            s32 DiffuseHeight = DiffuseBMP.GetHeight() ;

            // Must be the same sizes
            if ((AlphaWidth == DiffuseWidth) && (AlphaHeight == DiffuseHeight))
            {
                // Clear used array
                char AlphaUsed[256] ;
                s32  nAlphaUsed = 0 ;
                for (s32 i = 0 ; i < 256 ; i++)
                    AlphaUsed[i] = 0 ;

                // Count how many alpha values are used
                for (s32 x = 0 ; x < AlphaWidth ; x++)
                {
                    for (s32 y = 0 ; y < AlphaHeight ; y++)
                    {
                        xcolor AlphaColor   = AlphaBMP.GetPixelColor(x,y) ;

                        if (!AlphaUsed[AlphaColor.A])
                        {
                            AlphaUsed[AlphaColor.A] = TRUE ;
                            nAlphaUsed++ ;
                        }
                    }
                }

                // If there are only 3 values, let's burn them into the diffuse and create a punch through texture
                if (nAlphaUsed <= 3)
                {
                    AlphaBurned = TRUE ;

                    // Flag material is punch through
                    SetPunchThroughFlag(TRUE) ;

                    // Create combined name: alphanameXX_PUNCH_diffusename
                    char NewName[X_MAX_FNAME] ;
                    CreateNewTextureName(NewName, AlphaTEX, "_PUNCH_", DiffuseTEX) ;

                    // Only create the texture if it's not already there...
                    NewTexIndex = TextureManager.GetTextureIndexByName(NewName) ;
                    if (NewTexIndex == -1)
                    {
                        // Create new texture
                        texture* NewTexture = new texture() ;
                        ASSERT(NewTexture) ;
                        NewTexture->SetName(NewName) ;

                        // Add to texture manager
                        NewTexIndex = TextureManager.AddTexture(NewTexture) ;
                        ASSERT(NewTexIndex != -1) ;

                        // Setup new bitmap
                        // (Create a new 32 bit combined bitmap so the quantizer kicks in)
                        xbitmap& NewBitmap = NewTexture->GetXBitmap() ;
                        byte*    PixelData = (byte *)x_malloc(DiffuseWidth * DiffuseHeight * 4) ;
                        ASSERT(PixelData) ;
                        NewBitmap.Setup(xbitmap::FMT_32_ARGB_8888,     // Format
                                             DiffuseWidth,                  // Width
                                             DiffuseHeight,                 // Height
                                             TRUE,                          // DataOwned
                                             PixelData,                     // PixelData
                                             FALSE,                         // Clut owned
                                             NULL) ;                        // Clut data

                        // Get middle of alpha range
                        s32 MiddleAlpha = 128 ;
                        if (LoadSettings.Target == shape_bin_file_class::PS2) // PS2 is special case!
                            MiddleAlpha = 64 ;

                        // Burn alpha + diffuse into new bitmap
                        for (s32 x = 0 ; x < DiffuseWidth ; x++)
                        {
                            for (s32 y = 0 ; y < DiffuseHeight ; y++)
                            {
                                xcolor AlphaColor   = AlphaBMP.GetPixelColor(x,y) ;
                                xcolor DiffuseColor = DiffuseBMP.GetPixelColor(x,y) ;

                                // Map to either 0 or 255 for punch through!
                                if (AlphaColor.A < MiddleAlpha)
                                    AlphaColor.A = 0 ;
                                else
                                    AlphaColor.A = 255 ;

                                // Grab alpha
                                DiffuseColor.A = AlphaColor.A ;

                                // Store new color
                                NewBitmap.SetPixelColor(DiffuseColor, x,y) ;
                            }
                        }

                        // Convert to a format with alpha
                        xbitmap::format NewFormat = xbitmap::FMT_NULL;
                        if (LoadSettings.Target == shape_bin_file_class::PC)
                            NewFormat = xbitmap::FMT_32_ARGB_8888 ;
                        else
                        if (LoadSettings.Target == shape_bin_file_class::PS2)
                        {
                            // Make sure it has alpha
                            switch( DiffuseBMP.GetFormatInfo().BPP )
                            {
                                case 32:
                                case 24:    NewFormat = xbitmap::FMT_32_ABGR_8888 ; break;
                                case 16:    NewFormat = xbitmap::FMT_16_ABGR_1555 ; break;
                                case 8:     NewFormat = xbitmap::FMT_P8_ABGR_8888 ; break;
                                case 4:     NewFormat = xbitmap::FMT_P4_ABGR_8888 ; break;
                            }
                        }

                        ASSERT( NewFormat != xbitmap::FMT_NULL );
                        NewBitmap.ConvertFormat( NewFormat );

                        // Put into native format
                        switch(LoadSettings.Target)
                        {
                            default:
                                ASSERT(0) ; // version not supported!
                                break ;

                            case shape_bin_file_class::PS2:
                                auxbmp_ConvertToPS2(NewBitmap) ;
                                break ;

                            case shape_bin_file_class::PC:
                                auxbmp_ConvertToD3D(NewBitmap) ;
                                break ;
                        }

                        // Create mips?
                        if (LoadSettings.BuildMips)
                            NewBitmap.BuildMips() ;

                        // Plop new diffuse bitmap back in vram
                        if (LoadSettings.Target == shape_bin_file_class::GetDefaultTarget())
                            vram_Register(NewBitmap) ;
                    }
                }
            }
        }

        // Update?
        if (AlphaBurned)
        {
            // Update material diffuse index
            if (Frame == 0)
            {
                ASSERT(NewTexIndex != -1) ;
                NewDiffuseIndex = NewTexIndex ;
            }
            else
            {
                ASSERT(NewTexIndex == (NewDiffuseIndex + Frame)) ;
            }
        }
    }

    // Remove alpha?
    if (AlphaBurned)
    {
        // NOTE: The diffuse and alpha textures are left around incase another diffuse texture
        //       may use it again...

        // Set new diffuse texture index
        ASSERT(NewDiffuseIndex != -1) ;
        m_TextureInfo[TEXTURE_TYPE_DIFFUSE].Index = NewDiffuseIndex ;

        // Finally, remove the alpha lookup since it is not needed
        SetTextureInfo(TEXTURE_TYPE_ALPHA, -1, 0) ;
        SetTextureRef (TEXTURE_TYPE_ALPHA, -1) ;
    }
}

//==============================================================================

void material::BurnAlphaIntoDiffuse( model&                     Model, 
                                     texture_manager&           TextureManager, 
                                     const shape_load_settings& LoadSettings )
{
    (void)Model ;

    xbool   AlphaBurned = FALSE ;
    s32     NewDiffuseIndex = -1 ;

    // Loop through all frames!
    s32 Count = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Count ;
    for (s32 Frame = 0 ; Frame < Count ; Frame++)
    {
        // Get 1st texture
        s32 AlphaIndex      = GetTextureInfo(TEXTURE_TYPE_ALPHA).Index ;
        s32 DiffuseIndex    = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Index ;
        s32 NewTexIndex        = -1 ;

        // Both present and not already burned this baby?
        if ((AlphaIndex != -1) && (DiffuseIndex != -1))
        {
            // Goto correct frame
            DiffuseIndex += Frame % GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Count ;
            AlphaIndex   += Frame % GetTextureInfo(TEXTURE_TYPE_ALPHA).Count ;

            // Get textures
            texture& AlphaTEX   = TextureManager.GetTexture(AlphaIndex) ;
            texture& DiffuseTEX = TextureManager.GetTexture(DiffuseIndex) ;

            // Get bitmaps
            xbitmap& AlphaBMP    = AlphaTEX.GetXBitmap() ;
            xbitmap& DiffuseBMP  = DiffuseTEX.GetXBitmap() ;

            // Simple case if diffuse and alpha are the same texture!
            if (DiffuseIndex == AlphaIndex)
            {
                // Setup name of new texture...
                AlphaBurned = TRUE ;

                // Create name "AI_diffusename"
                char NewName[X_MAX_FNAME] ;
                CreateNewTextureName(NewName, "AI_", DiffuseTEX) ;
                ASSERT(x_strlen(NewName) < TEXTURE_MAX_FNAME) ;

                // Only create the texture if it's not already there...
                NewTexIndex = TextureManager.GetTextureIndexByName(NewName) ;
                if (NewTexIndex == -1)
                {
                    // Create new texture
                    texture* NewTexture = new texture() ;
                    ASSERT(NewTexture) ;
                    NewTexture->SetName(NewName) ;

                    // Add to texture manager
                    NewTexIndex = TextureManager.AddTexture(NewTexture) ;
                    ASSERT(NewTexIndex != -1) ;

                    // Setup new bitmap
                    xbitmap& NewBitmap = NewTexture->GetXBitmap() ;
                    NewBitmap = DiffuseBMP ;
                    NewBitmap.BuildMips(0) ;
                    NewBitmap.MakeIntensityAlpha() ;

                    // Put into native format
                    switch(LoadSettings.Target)
                    {
                        default:
                            ASSERT(0) ; // version not supported!
                            break ;

                        case shape_bin_file_class::PS2:
                            auxbmp_ConvertToPS2(NewBitmap) ;
                            break ;

                        case shape_bin_file_class::PC:
                            auxbmp_ConvertToD3D(NewBitmap) ;
                            break ;
                    }

                    // Create mips?
                    if (LoadSettings.BuildMips)
                        NewBitmap.BuildMips() ;

                    // Put into vram
                    if (LoadSettings.Target == shape_bin_file_class::GetDefaultTarget())
                        vram_Register(NewBitmap) ;
                }
            }
            else
            if (LoadSettings.Target == shape_bin_file_class::PC) // Only burn in on the PC
            {
                // Setup name of new texture...
                AlphaBurned = TRUE ;

                // Create combined name: "alphanameXX_BURNT_diffusename"
                char NewName[X_MAX_FNAME] ;
                CreateNewTextureName(NewName, AlphaTEX, "_BURNT_", DiffuseTEX) ;

                // Only create the texture if it's not already there...
                NewTexIndex = TextureManager.GetTextureIndexByName(NewName) ;
                if (NewTexIndex == -1)
                {
                    // Create new texture
                    texture* NewTexture = new texture() ;
                    ASSERT(NewTexture) ;
                    NewTexture->SetName(NewName) ;

                    // Add to texture manager
                    NewTexIndex = TextureManager.AddTexture(NewTexture) ;
                    ASSERT(NewTexIndex != -1) ;

                    // Get sizes
                    s32 AlphaWidth    = AlphaBMP.GetWidth() ;
                    s32 AlphaHeight   = AlphaBMP.GetHeight() ;
                    s32 DiffuseWidth  = DiffuseBMP.GetWidth() ;
                    s32 DiffuseHeight = DiffuseBMP.GetHeight() ;

                    // Setup new bitmap
                    xbitmap& NewBitmap = NewTexture->GetXBitmap() ;
                    byte*    PixelData = (byte *)x_malloc(DiffuseWidth * DiffuseHeight * 4) ;
                    ASSERT(PixelData) ;
                    NewBitmap.Setup(xbitmap::FMT_32_ARGB_8888,     // Format
                                    DiffuseWidth,                  // Width
                                    DiffuseHeight,                 // Height
                                    TRUE,                          // DataOwned
                                    PixelData,                     // PixelData
                                    FALSE,                         // Clut owned
                                    NULL) ;                        // Clut data

                    // Okay - we need to go through pixel by pixel combing the diffuse color, and alpha

                    // Quick version
                    if (        (AlphaWidth  == DiffuseWidth)
                            &&  (AlphaHeight == DiffuseHeight) )
                    {
                        // Now setup alpha
                        for (s32 x = 0 ; x < DiffuseWidth ; x++)
                        {
                            for (s32 y = 0 ; y < DiffuseHeight ; y++)
                            {
                                xcolor AlphaColor   = AlphaBMP.GetPixelColor(x,y) ;
                                xcolor DiffuseColor = DiffuseBMP.GetPixelColor(x,y) ;
                                DiffuseColor.A      = AlphaColor.A ;
                                NewBitmap.SetPixelColor(DiffuseColor, x,y) ;
                            }
                        }
                    }
                    else
                    {
                        // Slow version...
                        for (s32 x = 0 ; x < DiffuseWidth ; x++)
                        {
                            for (s32 y = 0 ; y < DiffuseHeight ; y++)
                            {
                                xcolor AlphaColor   = AlphaBMP.GetBilinearColor( (f32)x/DiffuseWidth, (f32)y/DiffuseHeight, TRUE) ;
                                xcolor DiffuseColor = DiffuseBMP.GetPixelColor(x,y) ;
                                DiffuseColor.A      = AlphaColor.A ;
                                NewBitmap.SetPixelColor(DiffuseColor, x,y) ;
                            }
                        }
                    }

                    // Put into native format
                    switch(LoadSettings.Target)
                    {
                        default:
                            ASSERT(0) ; // version not supported!
                            break ;

                        case shape_bin_file_class::PS2:
                            auxbmp_ConvertToPS2(NewBitmap) ;
                            break ;

                        case shape_bin_file_class::PC:
                            auxbmp_ConvertToD3D(NewBitmap) ;
                            break ;
                    }

                    // Create mips?
                    if (LoadSettings.BuildMips)
                        NewBitmap.BuildMips() ;

                    // Put into vram
                    if (LoadSettings.Target == shape_bin_file_class::GetDefaultTarget())
                        vram_Register(NewBitmap) ;
                }
            }
        }

        // Update?
        if (AlphaBurned)
        {
            // Update material diffuse index
            if (Frame == 0)
            {
                ASSERT(NewTexIndex != -1) ;
                NewDiffuseIndex = NewTexIndex ;
            }
            else
            {
                ASSERT(NewTexIndex == (NewDiffuseIndex + Frame)) ;
            }
        }
    }

    // Remove alpha?
    if (AlphaBurned)
    {
        // NOTE: The diffuse and alpha textures are left around incase another diffuse texture
        //       may use it again...

        // Set new diffuse texture index
        ASSERT(NewDiffuseIndex != -1) ;
        m_TextureInfo[TEXTURE_TYPE_DIFFUSE].Index = NewDiffuseIndex ;

        // Finally, remove the alpha lookup since it is not needed
        SetTextureInfo(TEXTURE_TYPE_ALPHA, -1, 0) ;
        SetTextureRef (TEXTURE_TYPE_ALPHA, -1) ;
    }
}

//==============================================================================

// Prepares textures ready for using
void material::CompressTextures( model&                     Model, 
                                 texture_manager&           TextureManager, 
                                 const shape_load_settings& LoadSettings )
{
    // Only compress on the PS2
    if (LoadSettings.Target != shape_bin_file_class::PS2)
        return ;

    // Any diffuse texture(s)?
    s32 Count = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Count ;
    if (!Count)
        return ;

    // Skip punch through materials since they eat a lot of gs time due to the huge
    // leaf polys that are in trees
    if (m_Flags.PunchThrough)
        return ;

    // Check to see if the compressed textures have already been created
    {
        // Lookup 1st diffuse texture info
        s32      DiffuseIndex = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Index ;
        texture& DiffuseTEX   = TextureManager.GetTexture(DiffuseIndex) ;
        xbitmap& DiffuseBMP   = DiffuseTEX.GetXBitmap() ;

        // Can texture be compressed?
        // (smallest width/height is 8 pixels for ps2 and base map will be 4* smaller)
        if ( (DiffuseBMP.GetWidth() < (8*4)) || (DiffuseBMP.GetHeight() < (8*4)) )
            return ;

        // Skip alpha textures because they look like shit
        if (DiffuseBMP.HasAlphaBits())
            return ;

        // Skip 4bits per pixel formats since they will not compress!
        if (DiffuseBMP.GetBPP() <= 4)
            return ;

        // Already compressed?
        char FName[X_MAX_FNAME];  
        x_splitpath( DiffuseTEX.GetName(), NULL, NULL, FName, NULL) ;
        if (x_strncmp("BASE_", FName, 5) == 0)
            return ;

        // Create base name: "BASE_diffusename"
        char BaseName[X_MAX_FNAME] ;
        CreateNewTextureName(BaseName, "BASE_", DiffuseTEX) ;

        // Create lum name: "LUM_diffusename"
        char LumName[X_MAX_FNAME] ;
        CreateNewTextureName(LumName, "LUM_", DiffuseTEX) ;

        // Make material reference it (this will add it to the model only if it's not already there)
        SetTextureRef(TEXTURE_TYPE_LUMINANCE, Model.AddTextureRef(LumName)) ;

        // Already converted?
        s32 BaseTexIndex = TextureManager.GetTextureIndexByName(BaseName) ;
        s32 LumTexIndex  = TextureManager.GetTextureIndexByName(LumName) ;
        if ((BaseTexIndex != -1) && (LumTexIndex != -1))
        {
            // Just set indices to textures and we are done
            SetTextureInfo(TEXTURE_TYPE_DIFFUSE,   BaseTexIndex, Count) ;
            SetTextureInfo(TEXTURE_TYPE_LUMINANCE, LumTexIndex,  Count) ;
            return ;
        }
    }

    // Create the compressed textures

    // These will be the new texture indices once we are done
    s32 BaseTexIndex = TextureManager.GetTextureCount() ;
    s32 LumTexIndex  = TextureManager.GetTextureCount() + Count ;

    // Create the base textures
    s32 Frame ;
    for (Frame = 0 ; Frame < Count ; Frame++)
    {
        // Lookup diffuse texture info
        s32      DiffuseIndex = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Index + Frame ;
        texture& DiffuseTEX   = TextureManager.GetTexture(DiffuseIndex) ;

        // Create base name: "BASE_diffusename"
        char BaseName[X_MAX_FNAME] ;
        CreateNewTextureName(BaseName, "BASE_", DiffuseTEX) ;

        // Create the texture and add to texture manager
        texture* BaseTEX = new texture() ;
        ASSERT(BaseTEX) ;
        BaseTEX->SetName(BaseName) ;

        // Add to texture manager
        TextureManager.AddTexture(BaseTEX) ;
    }

    // Create the lum textures
    for (Frame = 0 ; Frame < Count ; Frame++)
    {
        // Lookup diffuse texture info
        s32      DiffuseIndex = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Index + Frame ;
        texture& DiffuseTEX   = TextureManager.GetTexture(DiffuseIndex) ;

        // Create lum name: "LUM_diffusename"
        char LumName[X_MAX_FNAME] ;
        CreateNewTextureName(LumName, "LUM_", DiffuseTEX) ;

        // Create the texture and add to texture manager
        texture* LumTEX = new texture() ;
        ASSERT(LumTEX) ;
        LumTEX->SetName(LumName) ;

        // Add to texture manager
        TextureManager.AddTexture(LumTEX) ;
    }

    // Compress the textures
    for (Frame = 0 ; Frame < Count ; Frame++)
    {
        // Lookup diffuse texture info
        s32      DiffuseIndex = GetTextureInfo(TEXTURE_TYPE_DIFFUSE).Index + Frame ;
        texture& DiffuseTEX   = TextureManager.GetTexture(DiffuseIndex) ;
        xbitmap& DiffuseBMP   = DiffuseTEX.GetXBitmap() ;
        
        // Lookup base texture info
        texture& BaseTEX      = TextureManager.GetTexture(BaseTexIndex + Frame) ;
        xbitmap& BaseBMP      = BaseTEX.GetXBitmap() ;
        
        // Lookup lum texture info
        texture& LumTEX       = TextureManager.GetTexture(LumTexIndex + Frame) ;
        xbitmap& LumBMP       = LumTEX.GetXBitmap() ;
        
        // Show info
        shape::printf("compressing texture: %s\n", DiffuseTEX.GetName()) ;

        // Subtractive compression! (Also builds mips!)
        auxbmp_CompressPS2( DiffuseBMP, BaseBMP, LumBMP, TRUE ) ;

        // Just incase we are doing a pc test
        if (LoadSettings.Target == shape_bin_file_class::PC)
        {
            auxbmp_ConvertToD3D (BaseBMP) ;
            auxbmp_ConvertToD3D (LumBMP) ;
        }

        // Register with vram
        if (LoadSettings.Target == shape_bin_file_class::GetDefaultTarget())
        {
            BaseTEX.Register(TEXTURE_TYPE_DIFFUSE) ;
            LumTEX.Register(TEXTURE_TYPE_LUMINANCE) ;
        }

        // TEST - BUILD AND SAVE OUT DECOMPRESSED VERSIONS
        #if 0
        {
            xbitmap OutBMP ;

            // Create a 32 bit bitmap the same size of the source
            s32     OutWidth  = LumBMP.GetWidth() ;
            s32     OutHeight = LumBMP.GetHeight() ;
            byte*   OutData   = (byte *)x_malloc(OutWidth * OutHeight * 4) ;
            ASSERT(OutData) ;
            OutBMP.Setup(xbitmap::FMT_32_ARGB_8888,  // Format
                         OutWidth,                   // Width
                         OutHeight,                  // Height
                         TRUE,                       // DataOwned
                         OutData,                    // PixelData
                         FALSE,                      // Clut owned
                         NULL) ;                     // Clut data
            s32 BlockSize = OutWidth / DiffuseBMP.GetWidth() ;

            // Create output
            for( s32 by=0; by<OutHeight/BlockSize; by++ )
            for( s32 bx=0; bx<OutWidth/BlockSize; bx++ )
            {
                // Setup block
                for( s32 y=0; y<BlockSize; y++ )
                for( s32 x=0; x<BlockSize; x++ )
                {
                    // Get base map color
                    f32 u = (f32)((bx*BlockSize)+x) / (f32)OutWidth ;
                    f32 v = (f32)((by*BlockSize)+y) / (f32)OutHeight ;
                    xcolor BaseCol = BaseBMP.GetBilinearColor(u,v,TRUE) ;

                    // Lookup actual color
                    xcolor LumCol = LumBMP.GetPixelColor(bx*BlockSize+x, by*BlockSize+y) ;
                    #if 0
                        // Multiplicative
                        s32 R = (((s32)BaseCol.R * (s32)LumCol.R) >> 7) ;
                        s32 G = (((s32)BaseCol.G * (s32)LumCol.G) >> 7) ;
                        s32 B = (((s32)BaseCol.B * (s32)LumCol.B) >> 7) ;
                        s32 A = (((s32)BaseCol.A * (s32)LumCol.A) >> 7) ;
                    #else
                        // Subtractive
                        s32 R = ((s32)BaseCol.R - (s32)LumCol.R) ;
                        s32 G = ((s32)BaseCol.G - (s32)LumCol.G) ;
                        s32 B = ((s32)BaseCol.B - (s32)LumCol.B) ;
                        s32 A = ((s32)BaseCol.A - (s32)LumCol.A) ;
                    #endif

                    xcolor OutCol ;
                    OutCol.R = (u8)MAX(0, MIN(R,255)) ;
                    OutCol.G = (u8)MAX(0, MIN(G,255)) ;
                    OutCol.B = (u8)MAX(0, MIN(B,255)) ;
                    OutCol.A = (u8)MAX(0, MIN(A,255)) ;

                    // Set it!
                    OutBMP.SetPixelColor(OutCol,bx*BlockSize+x, by*BlockSize+y) ;
                }
            }

            // Save
            char    DRIVE   [X_MAX_DRIVE];
            char    DIR     [X_MAX_DIR];
            char    FNAME   [X_MAX_FNAME];
            char    EXT     [X_MAX_EXT];
            char    OutName [X_MAX_PATH];
            x_splitpath( DiffuseTEX.GetName(), DRIVE, DIR, FNAME, EXT) ;
            x_makepath ( OutName, NULL, NULL, FNAME, NULL) ;
            OutBMP.SaveTGA(xfs("%s_COMP.tga",OutName)) ;

            DiffuseBMP.SaveTGA(xfs("%s_ORIG.tga",OutName)) ;
            BaseBMP.SaveTGA(xfs("%s_P1_BASE.tga",OutName)) ;
            LumBMP.SaveTGA(xfs("%s_P1_LUM.tga",OutName)) ;
        }
        #endif
    }

    // Finally, make the material use the new textures
    SetTextureInfo(TEXTURE_TYPE_DIFFUSE,   BaseTexIndex, Count) ;
    SetTextureInfo(TEXTURE_TYPE_LUMINANCE, LumTexIndex,  Count) ;
}

//==============================================================================

// Prepares textures ready for using
void material::CheckForBurningTextures( model&                      Model, 
                                        texture_manager&            TextureManager, 
                                        const shape_load_settings&  LoadSettings )
{
    // Punch through special case
    CheckForPunchThrough(Model, TextureManager, LoadSettings) ;

    // Burn alpha into diffuse texture special cases
    BurnAlphaIntoDiffuse(Model, TextureManager, LoadSettings) ;

    // Compress the textures for PS2 target
    // (do after punch through since they are skipped)
    if (LoadSettings.CompressTextures)
        CompressTextures(Model, TextureManager, LoadSettings) ;
}

//==============================================================================

// Loads all textures
void material::LoadTextures( model&                     Model, 
                             texture_manager&           TextureManager, 
                             const shape_load_settings& LoadSettings )
{
    s32 Type ;

    // PS doesn't support bump mapping
    if (LoadSettings.Target == shape_bin_file_class::PS2)
    {
        // Clear all references to the bumpmap
        SetTextureInfo(TEXTURE_TYPE_BUMP,-1,0) ;
        SetTextureRef(TEXTURE_TYPE_BUMP,-1) ;
    }

    // Loop through all textures in material
    for (Type = 0 ; Type < TEXTURE_TYPE_TOTAL ; Type++)
    {
        // Is a texture in this slot?
        s32 TexRefIndex = GetTextureRef((texture_type)Type) ;
        if (TexRefIndex != -1)
        {
            // Lookup texture name
            const char* TextureName = Model.GetTextureRef(TexRefIndex).GetName() ;

            // Remap texture index into texture manager index
            s32 Index = -1 ;
            s32 Count = TextureManager.AddAndLoadTextures(TextureName,
                                                          LoadSettings,
                                                          (texture_type)Type, 
                                                          Index,
                                                          GetTextureInfo((texture_type)Type).Count) ;

            // Texture is not found, so remove reference from shape
            // (so that .shp will not try to load it again...)
            if (Count == 0)
                SetTextureRef((texture_type)Type, -1) ;

            // Update material to use texture manager index
            SetTextureInfo((texture_type)Type, Index, Count) ;
        }
    }

    // Burn textures (for when reading from ascii)
    if (LoadSettings.BurnTextures)
        CheckForBurningTextures(Model, TextureManager, LoadSettings) ;

    // Increase the reference counts of textures that are used ready for saving xbmps
    for (Type = 0 ; Type < TEXTURE_TYPE_TOTAL ; Type++)
    {
        // Any textures of this type?
        s32 Count = GetTextureInfo((material::texture_type)Type).Count ;
        s32 Index = GetTextureInfo((material::texture_type)Type).Index ;
        if (Count)
        {
            // Get texture
            ASSERT(Index != -1) ;
            texture& Tex = TextureManager.GetTexture(Index) ;

            // Get model texture ref
            s32 TexRefIndex = GetTextureRef((texture_type)Type) ;
            ASSERT(TexRefIndex != -1) ;
            shape_texture_ref&  TexRef = Model.GetTextureRef(TexRefIndex) ;

            // Update reference to use texture name so that no searching is performed when loading .shp file
            TexRef.SetName(Tex.GetName()) ;

            // Loop through all frames this type
            for (s32 Frame = 0 ; Frame < Count ; Frame++)
            {
                // Get 1st texture index
                s32 TexIndex   = GetTextureInfo((material::texture_type)Type).Index ;

                // Goto correct frame
                TexIndex += Frame % GetTextureInfo((material::texture_type)Type).Count ;

                // Flag it's referenced
                texture& Texture = TextureManager.GetTexture(TexIndex) ;
                Texture.IncReferenceCount() ;

                // Show debug info
                char    DRIVE   [X_MAX_DRIVE];
                char    DIR     [X_MAX_DIR];
                char    FNAME   [X_MAX_FNAME];
                char    EXT     [X_MAX_EXT];
                char    Name    [X_MAX_PATH];
                x_splitpath( Model.GetName(), DRIVE, DIR,  FNAME, EXT) ;
                x_makepath ( Name,            NULL,  NULL, FNAME, EXT) ;
                shape::printf("   Model:%s ", Name) ;

                x_splitpath( Texture.GetName(), DRIVE, DIR,  FNAME, EXT) ;
                x_makepath ( Name,              NULL,  NULL, FNAME, EXT) ;
                shape::printf(" references texture %s\n", Name) ;
            }
        }
    }
}

//==============================================================================

// Returns the ID to use for the shape material list and sets up the textures that are going to be used
s32 material::LookupTextures( const material_draw_info&     DrawInfo,
                                    texture_draw_info&      TexDrawInfo )
{
    // Start with material base ID
    s32 DrawListID = m_DrawListID ;
    s32 FrameBits  = 0 ;

    // Setup texture lookup infos (one for each texture type)
    for (s32 Type = 0 ; Type < TEXTURE_TYPE_TOTAL ; Type++)
    {
        // Lookup texture and frame
        s32         Count = m_TextureInfo[Type].Count ;
        s32         Frame ;
        texture*    Texture ;

        // No textures?
        if (Count == 0)
        {
            // Quick case - no texture
            Frame   = 0 ;
            Texture = NULL ;
        }
        else
        {
            // Get start index
            s32 Index = m_TextureInfo[Type].Index ;
            ASSERT(Index != -1) ;

            // Single texture?
            if (Count == 1)
            {
                Frame   = 0 ;
                Texture = &(DrawInfo.TextureManager->GetTexture(Index)) ;
            }
            else
            {
                // Take frame into account
                switch(DrawInfo.PlaybackType)
                {
                    // Repeat sequence
                    case PLAYBACK_TYPE_LOOP:
                        Frame = DrawInfo.Frame % Count ;
                        break ;

                    // Ping pong sequence
                    case PLAYBACK_TYPE_PING_PONG:
                        {
                            s32 TotalFrames = Count<<1 ;
                            s32 ModFrame = DrawInfo.Frame % TotalFrames ;
                            if (ModFrame < Count)
                                Frame = ModFrame ;
                            else
                                Frame = TotalFrames - ModFrame - 1 ;
                        }
                        break ;

                    // Play sequence just once
                    case PLAYBACK_TYPE_ONCE_ONLY:
                        Frame = MIN(DrawInfo.Frame, Count-1) ;
                        break ;

                    default:
                        // Not supported!
                        ASSERT(0) ;
                        Frame = 0 ;
                        break ;
                }

                // Make sure code works!
                ASSERT(Frame >= 0) ;
                ASSERT(Frame < Count) ;

                // Lookup texture from frame
                Texture = &(DrawInfo.TextureManager->GetTexture(Index + Frame)) ;
            }
        }

        // Keep texture pointer
        TexDrawInfo.Texture[Type] = Texture ;

        // Adjust draw list ID so material lands in correct place in draw list
        DrawListID += Frame << FrameBits ;
        FrameBits  += m_TextureInfo[Type].FrameBits ;
    }

    // Setup detail texture lookup
    TexDrawInfo.DetailTexture    = DrawInfo.DetailTexture ;
    TexDrawInfo.DetailClutHandle = DrawInfo.DetailClutHandle ;

    // Turn off passes?
    if (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_ALPHA_PASS)
        TexDrawInfo.Texture[TEXTURE_TYPE_ALPHA] = NULL ;
    if (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_DIFFUSE_PASS)
        TexDrawInfo.Texture[TEXTURE_TYPE_DIFFUSE] = NULL ;
    if (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_LUMINANCE_PASS)
        TexDrawInfo.Texture[TEXTURE_TYPE_LUMINANCE] = NULL ;
    if (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_REFLECT_PASS)
        TexDrawInfo.Texture[TEXTURE_TYPE_REFLECT] = NULL ;
    if (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_BUMP_PASS)
        TexDrawInfo.Texture[TEXTURE_TYPE_BUMP] = NULL ;
    if (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_DETAIL_PASS)
        TexDrawInfo.DetailTexture = NULL ;

    // Turn off detail on self illum materials?
    if ((m_Flags.SelfIllum) && (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_SELF_ILLUM_MAT_DETAIL))
        TexDrawInfo.DetailTexture = NULL ;

    // Is this an animated texture?
    if (m_TextureInfo[TEXTURE_TYPE_DIFFUSE].Count > 1)
    {
        // Turn off detail texture for animated material?
        if (!(DrawInfo.Flags & shape::DRAW_FLAG_TURN_ON_ANIM_MAT_DETAIL))
            TexDrawInfo.DetailTexture    = NULL ;
    }
    else
    {
        // Turn off detail texture for static material?
        if (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_STATIC_MAT_DETAIL)
            TexDrawInfo.DetailTexture = NULL ;
    }

    // Return the draw list ID to use in the shape material list
    return DrawListID ;
}


//==============================================================================
//
// Shared code
//
//==============================================================================

// Prepares for rendering with material
void material::Activate ( const material_draw_info& DrawInfo,
                                s32&                CurrentLoadedNode )
{
    // Lookup textures to draw with
    texture_draw_info TexDrawInfo ;
    LookupTextures(DrawInfo, TexDrawInfo) ;

    // Activate
    Activate(DrawInfo, TexDrawInfo, CurrentLoadedNode) ;
}

//==============================================================================

// Prepares for rendering with material and textures
void material::Activate( const material_draw_info& DrawInfo, 
                         const texture_draw_info&  TexDrawInfo,
                               s32&                CurrentLoadedNode )
{
    // Activate common data
    ActivateSettings(DrawInfo, TexDrawInfo) ;

    // Activate textures
    ActivateTextures(DrawInfo, TexDrawInfo) ;

    // Write material to z buffer only?
    if (DrawInfo.Flags & shape::DRAW_FLAG_PRIME_Z_BUFFER)
        PrimeZBuffer(DrawInfo, TexDrawInfo, CurrentLoadedNode) ;

    // Actiavte material
    ActivateColor(DrawInfo, TexDrawInfo) ;
}

//==============================================================================


//==============================================================================
//
// PS2 Specific code
//
//==============================================================================

#ifdef TARGET_PS2

//==============================================================================
// material display list class
//==============================================================================

// Draws the material
void material_dlist::Draw(const material_draw_info& DrawInfo, s32 &CurrentLoadedNode)
{
    struct node_pack
    {
        dmatag DMA ;
    } ;

    // Skip if node is not visible
    if ( (m_Node < 32) && (!(DrawInfo.NodeVisibleFlags & (1<<m_Node))) )
        return ;

    // Performing animated visibility draw?
    if (DrawInfo.Flags & shape::DRAW_FLAG_VISIBILITY_ANIMATED)
    {
        // If drawing transparent visibility pass, then skip opaque nodes
        if (DrawInfo.Flags & shape::DRAW_FLAG_VISIBILITY_PASS_TRANSPARENT)
        {
            // If this node is opaque, then skip
            if (DrawInfo.RenderNodes[m_Node].Light.Amb.W == 1)
                return ;

            // Totally transparent?
            if (DrawInfo.RenderNodes[m_Node].Light.Amb.W == 0)
                return ;
        }

        // If drawing opqaue visibility pass, then skip transparent nodes
        if (DrawInfo.Flags & shape::DRAW_FLAG_VISIBILITY_PASS_OPAQUE)
        {
            // If this node is transparent, then skip
            if (DrawInfo.RenderNodes[m_Node].Light.Amb.W != 1)
                return ;
        }
    }

    #ifdef TARGET_PS2
        // Wait for GS to finish
        vram_Sync() ;
    #endif

    // Load a new node?
    if (CurrentLoadedNode != m_Node)
    {
        // Flag this node is loaded
        CurrentLoadedNode = m_Node ;

        // Must have already called BuildDList!
        ASSERT(m_Node != -1) ;
        ASSERT(m_DList) ;
        ASSERT(m_DListSize) ;

        // Transfer the node data to vu
        node_pack *Pack = DLStruct(node_pack) ;
        Pack->DMA.SetRef(NODE_SIZE*16, (s32) &DrawInfo.RenderNodes[m_Node]) ;
        Pack->DMA.PAD[0] = SCE_VIF1_SET_STCYCL(4, 4, 0) ;
        Pack->DMA.PAD[1] = VIF_Unpack(SHAPE_NODE_ADDR,  // DestVUAddr
                                      NODE_SIZE,        // NVectors
                                      VIF_V4_32,        // Format
                                      FALSE,            // Signed
                                      FALSE,            // Masked
                                      TRUE) ;           // AbsoluteAddress
        ASSERT( (((u32)&DrawInfo.RenderNodes[m_Node]) & 0xF) == 0) ;
    }

    // Simply reference dlist to draw the dlist
    dmatag *DMA = DLStruct(dmatag) ;
    DMA->SetRef(m_DListSize, (u32)m_DList) ;

    #ifdef X_DEBUG
        material::s_DListDraw++ ;
    #endif
}


//==============================================================================
// material class
//==============================================================================


// Draws the material into the z buffer only (frame buffer is untouched)
void material::PrimeZBuffer(const material_draw_info&   DrawInfo,
                            const texture_draw_info&    TexDrawInfo,
                                  s32&                  CurrentLoadedNode)
{
    (void)TexDrawInfo ;

    //======================================================================
    // Setup material used by every strip
    //======================================================================
    ps2_material *pPS2Material = PS2Material_Begin(DrawInfo, 1) ;
    ASSERT(pPS2Material) ;

    // To speed up micro code
    pPS2Material->Flags |= MATERIAL_FLAG_SELF_ILLUM ;

    //======================================================================
    // Setup and draw pass
    //======================================================================

    // Setup pass info
    ps2_pass_info   PassInfo ;
    PassInfo.BMP            = NULL ;                    // Bitmap
    PassInfo.ClutHandle     = -1 ;                      // Clut handle
    PassInfo.Context        = 0 ;                       // Context
    PassInfo.AlphaBlendMode = ALPHA_BLEND_OFF ;         // No blending
    PassInfo.FixedAlpha     = 0 ;                       // Not used
    PassInfo.FBMask         = 0xFFFFFFFF ;              // Do not write to frame buffer
    PassInfo.GifTag.Build2(4, 0, GIF_PRIM_TRIANGLESTRIP, 0) ; // no fog, alpha. or texture!
    PassInfo.GifTag.Reg(GIF_REG_NOP, GIF_REG_NOP, GIF_REG_NOP, GIF_REG_XYZ2) ; // only pos needed

    // Add pass
    PS2Material_AddPass(pPS2Material, PassInfo) ;

    // Done
    PS2Material_End(pPS2Material, 1) ;

    //======================================================================
    // Setup gs registers
    //======================================================================

    // Set z buffer test
    gsreg_Begin() ;
    gsreg_SetZBufferTest(ZBUFFER_TEST_GEQUAL) ;
    gsreg_SetZBuffer(TRUE) ;
    gsreg_SetZBufferUpdate(TRUE) ;
    gsreg_End() ;

    //======================================================================
    // Draw pass
    //======================================================================

    // Draw material into z buffer
    Draw(DrawInfo, CurrentLoadedNode) ;

    // Force new node load since reflection matrix is not setup and might be needed
    CurrentLoadedNode = -1 ;
}

//==============================================================================

void material::ActivateSettings(const material_draw_info& DrawInfo, const texture_draw_info& TexDrawInfo)
{
    (void)TexDrawInfo ;

    // Setup mip-mapping for diffuse texture
    gsreg_Begin() ;
    
    eng_PushGSContext(0);
    vram_SetMipEquation(DrawInfo.MipK) ;
    gsreg_SetClamping( (GetUWrapFlag() == FALSE), (GetVWrapFlag() == FALSE) ) ;
    eng_PopGSContext();

    // Setup mip-mapping for reflection map
    eng_PushGSContext(1);
    vram_SetMipEquation(DrawInfo.MipK) ;
    eng_PopGSContext();

    gsreg_End() ;

    #ifdef X_DEBUG
         s_LoadSettings++ ;
    #endif
}

//==============================================================================

void material::ActivateColor(const material_draw_info& DrawInfo, const texture_draw_info& TexDrawInfo)
{
    s32             AlphaBlendMode ;
    s32             FixedAlpha = 0 ;
    s32             GIFPrim ;
    u32             GifFlags ;
    ps2_pass_info   PassInfo PS2_ALIGNMENT(16);

    //======================================================================
    // Lookup textures
    //======================================================================
    texture* DiffuseTEX         = TexDrawInfo.Texture[TEXTURE_TYPE_DIFFUSE] ;
    texture* LuminanceTEX       = TexDrawInfo.Texture[TEXTURE_TYPE_LUMINANCE] ;
    texture* AlphaTEX           = TexDrawInfo.Texture[TEXTURE_TYPE_ALPHA] ;
    texture* ReflectTEX         = TexDrawInfo.Texture[TEXTURE_TYPE_REFLECT] ;
    texture* DetailTEX          = TexDrawInfo.DetailTexture ;
    s32      DetailClutHandle   = TexDrawInfo.DetailClutHandle ;

    // If there's no diffuse, and a luminance, then make the luminance be the diffuse 
    // just to make this setup code easier!
    if ((!DiffuseTEX) && (LuminanceTEX))
    {
        DiffuseTEX   = LuminanceTEX ;
        LuminanceTEX = NULL ;
    }

    //======================================================================
    // Count passes
    //======================================================================
    s32 PassCount = 1 ;     // Always a diffuse pass!
    if (LuminanceTEX)   PassCount++ ;
    if (AlphaTEX)       PassCount++ ;
    if (ReflectTEX)     PassCount++ ;
    if (DetailTEX)      PassCount++ ;

    //======================================================================
    // Setup material used by every strip
    //======================================================================
    ps2_material *pPS2Material = PS2Material_Begin(DrawInfo, PassCount) ;
    ASSERT(pPS2Material) ;

    //======================================================================
    // Setup material color
    //======================================================================
    if (DiffuseTEX)
    {
        // Just use texture map as color source
        pPS2Material->Col[0] = DrawInfo.Color[0] ;
        pPS2Material->Col[1] = DrawInfo.Color[1] ;
        pPS2Material->Col[2] = DrawInfo.Color[2] ;
        pPS2Material->Col[3] = m_Color[3] * DrawInfo.Color[3] ;
    }
    else
    {
        // Use diffuse color of material
        pPS2Material->Col[0] = m_Color[0] * DrawInfo.Color[0] ;
        pPS2Material->Col[1] = m_Color[1] * DrawInfo.Color[1] ;
        pPS2Material->Col[2] = m_Color[2] * DrawInfo.Color[2] ;
        pPS2Material->Col[3] = m_Color[3] * DrawInfo.Color[3] ;
    }

	// Take self illum color also?
	if ((m_Flags.SelfIllum) || (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_LIGHTING))
	{
		pPS2Material->Col[0] *= DrawInfo.SelfIllumColor[0] ;
		pPS2Material->Col[1] *= DrawInfo.SelfIllumColor[1] ;
		pPS2Material->Col[2] *= DrawInfo.SelfIllumColor[2] ;
		pPS2Material->Col[3] *= DrawInfo.SelfIllumColor[3] ;
	}

    //======================================================================
    // Setup alpha blending mode and fixed alpha for diffuse map
    //======================================================================

    if (DiffuseTEX)
    {
        // Punch through?
        if (m_Flags.PunchThrough)
            AlphaBlendMode = ALPHA_BLEND_OFF ;
        else
        // Use additive?
        if (m_Flags.Additive)
            AlphaBlendMode = ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) ;
        else
        // Use texture alpha?
        if (m_Flags.HasAlpha)
        {
            // Have reflection map that's going to be per pixel controlled by the alpha?
            if ( (ReflectTEX) && (DiffuseTEX->HasAlpha() || (AlphaTEX)) )
                AlphaBlendMode = ALPHA_BLEND_OFF ;
            else
            {
                // Use alpha in frame buffer (from alpha texture) or diffuse texture?
                if (AlphaTEX)
                    AlphaBlendMode = ALPHA_BLEND_MODE(C_SRC, C_DST, A_DST, C_DST) ; // (Src-Dst)*ADst + Dst
                else
                // Use alpha in texture?
                if (DiffuseTEX->HasAlpha())
                    AlphaBlendMode = ALPHA_BLEND_INTERP ;  // Use texture alpha: (Src-Dst)*ASrc + Dst
                else
                {
                    // FixedAlpha of material
                    AlphaBlendMode = ALPHA_BLEND_MODE(C_SRC, C_DST, A_FIX, C_DST) ; // Mat alpha: // (Src-Dst)*AFix + Dst
                    FixedAlpha     = (u32)(pPS2Material->Col[3] * 128.0f) ;
                }
            }
        }
        else
        // Use material alpha?
        if (pPS2Material->Col[3] != 1.0f)
        {
            // Use material alpha?
            //AlphaBlendMode = ALPHA_BLEND_MODE(C_SRC, C_DST, A_FIX, C_DST) ; // (Src-Dst)*AFix + Dst
            //FixedAlpha     = (u32)(pPS2Material->Col[3] * 128.0f) ;

            AlphaBlendMode = ALPHA_BLEND_INTERP ;  // Use texture alpha: (Src-Dst)*ASrc + Dst ;
            FixedAlpha     = (u32)(pPS2Material->Col[3] * 128.0f) ;
        }
        else
        {
            // Put fog color in vertex alpha so that luminance/reflection map pass fades out
            // correctly with fogging
            pPS2Material->Col[3] = 1.0f - (DrawInfo.FogColor.A*(1.0f/255.0f)) ;

            // No alpha
            AlphaBlendMode = ALPHA_BLEND_OFF ;
        }
    }
    else
    {
        // Use additive?
        if (m_Flags.Additive)
            AlphaBlendMode = ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) ;  // Add
        else
        // Use alpha?
        if (pPS2Material->Col[3] != 1.0f)
        {
            //AlphaBlendMode = ALPHA_BLEND_MODE(C_SRC, C_DST, A_FIX, C_DST) ; // Mat alpha: // (Src-Dst)*AFix + Dst
            //FixedAlpha     = (u32)(pPS2Material->Col[3] * 128.0f) ;

            AlphaBlendMode = ALPHA_BLEND_INTERP ;  // Use texture alpha: (Src-Dst)*ASrc + Dst ;
            FixedAlpha     = (u32)(pPS2Material->Col[3] * 128.0f) ;
        }
        else
            AlphaBlendMode = ALPHA_BLEND_OFF ;  // No alpha
    }

    //======================================================================
    // Setup constant GS registers for all passes
    //======================================================================

    gsreg_Begin() ;

    // Setup punch through mode?
    if (m_Flags.PunchThrough)
    {
        // Turn on source alpha test, turn on z buffer test
        gsreg_SetAlphaAndZBufferTests( TRUE,                    // AlphaTest
                                       ALPHA_TEST_GEQUAL,       // AlphaTestMethod
                                       64,                      // AlphaRef
                                       ALPHA_TEST_FAIL_KEEP,    // AlphaTestFail
                                       FALSE,                   // DestAlphaTest
                                       DEST_ALPHA_TEST_0,       // DestAlphaTestMethod
                                       TRUE,                    // ZBufferTest
                                       ZBUFFER_TEST_GEQUAL ) ;  // ZBufferTestMethod

        // Update the Z buffer as normal...
        gsreg_SetZBufferUpdate(TRUE) ;
    }
    else
    {
        // Turn off alpha test, turn on z buffer test
        gsreg_SetAlphaAndZBufferTests( FALSE,                   // AlphaTest
                                       ALPHA_TEST_ALWAYS,       // AlphaTestMethod
                                       64,                      // AlphaRef
                                       ALPHA_TEST_FAIL_KEEP,    // AlphaTestFail
                                       FALSE,                   // DestAlphaTest
                                       DEST_ALPHA_TEST_0,       // DestAlphaTestMethod
                                       TRUE,                    // ZBufferTest
                                       ZBUFFER_TEST_GEQUAL ) ;  // ZBufferTestMethod

        // Update Z buffer only on opaque passes
        gsreg_SetZBufferUpdate(AlphaBlendMode == ALPHA_BLEND_OFF) ;
    }

    // Fog?
    if (DrawInfo.FogColor.A != 0)
    {
        gsreg_Set(SCE_GS_FOGCOL, SCE_GS_SET_FOGCOL(DrawInfo.FogColor.R, DrawInfo.FogColor.G, DrawInfo.FogColor.B)) ;
        gsreg_Set(SCE_GS_FOG, SCE_GS_SET_FOG(255 - DrawInfo.FogColor.A)) ;      // for ps2: 0=max fog, 255=min fog
    }

    gsreg_End() ;


    //======================================================================
    // Setup gif prim and flags
    //======================================================================

    // Setup primitive to draw
    if (shape::s_DrawSettings.WireFrame)
        GIFPrim = GIF_PRIM_LINESTRIP ;
    else
        GIFPrim = GIF_PRIM_TRIANGLESTRIP ;

    // Setup gif flags
    GifFlags = GIF_FLAG_SMOOTHSHADE | GIF_FLAG_TEXTURE ;

    // Alpha blend?
    if (AlphaBlendMode != ALPHA_BLEND_OFF)
        GifFlags |= GIF_FLAG_ALPHA ;

    // Fog?
    if (DrawInfo.FogColor.A != 0)
        GifFlags |= GIF_FLAG_FOG ;

    //======================================================================
    // Setup default pass info
    //======================================================================

    // Context switching
    PassInfo.Context        = 0 ;

    // Alpha blending
    PassInfo.AlphaBlendMode = AlphaBlendMode ;
    PassInfo.FixedAlpha     = FixedAlpha ;

    // Frame buffer masking
    PassInfo.FBMask         = 0 ;

    //======================================================================
    // Always draw diffuse pass
    //======================================================================

    // Textured?
    if (DiffuseTEX)
    {
        // Setup pass info
        PassInfo.BMP        = &DiffuseTEX->GetXBitmap() ;   // Bitmap
        PassInfo.ClutHandle = -1 ;                          // Clut handle
        PassInfo.GifTag.Build2(4, 0, GIFPrim, GifFlags) ;
        PassInfo.GifTag.Reg   (GIF_REG_ST, GIF_REG_NOP, GIF_REG_RGBAQ, GIF_REG_XYZ2) ;

        // Add to material passes
        PS2Material_AddPass(pPS2Material, PassInfo) ;
    }
    else
    {
        // Setup pass info
        PassInfo.AlphaBlendMode = AlphaBlendMode ;
        PassInfo.BMP            = NULL ;    // Bitmap
        PassInfo.ClutHandle     = -1 ;      // Clut handle
        PassInfo.GifTag.Build2(4, 0, GIFPrim, GifFlags & ~(GIF_FLAG_TEXTURE)) ;
        PassInfo.GifTag.Reg   (GIF_REG_ST, GIF_REG_NOP, GIF_REG_RGBAQ, GIF_REG_XYZ2) ;

        // Add to material passes
        PS2Material_AddPass(pPS2Material, PassInfo) ;
    }

    //======================================================================
    // Luminance pass?
    //======================================================================

    if (LuminanceTEX)
    {
        // Setup bitmap
        PassInfo.BMP        = &LuminanceTEX->GetXBitmap() ; // Bitmap
        PassInfo.ClutHandle = -1 ;                          // Clut handle

        // Just write RGB
        PassInfo.FBMask = 0xff000000 ;  // xRGB

        // Multiplicative - can't alpha and fog though 8(
        //PassInfo.AlphaBlendMode = ALPHA_BLEND_MODE(C_DST, C_ZERO, A_SRC, C_ZERO) ;  // (Dst-0)*SrcA + 0
        
        // Subtractive - where (SrcA = 1 - see auxbitmap_CompressPS2)
        PassInfo.AlphaBlendMode = ALPHA_BLEND_MODE(C_ZERO, C_SRC, A_SRC, C_DST) ;  // (0-Src)*SrcA + Dst

        PassInfo.GifTag.Build2(4, 0, GIFPrim, GIF_FLAG_SMOOTHSHADE | GIF_FLAG_TEXTURE | GIF_FLAG_ALPHA) ;
        PassInfo.GifTag.Reg   (GIF_REG_ST, GIF_REG_NOP, GIF_REG_RGBAQ, GIF_REG_XYZ2) ;

        // Add to material passes
        PS2Material_AddPass(pPS2Material, PassInfo) ;

        // Turn on writing of all ARGB
        PassInfo.FBMask = 0x00000000 ;  // ARGB
    }

    //======================================================================
    // Alpha pass?
    //======================================================================
    
    if (AlphaTEX)
    {
        // Setup bitmap
        PassInfo.BMP        = &AlphaTEX->GetXBitmap(),      // Bitmap
        PassInfo.ClutHandle = -1 ;                          // Clut handle

        // Setup pass info to just blit texture into frame buffer
        PassInfo.AlphaBlendMode = ALPHA_BLEND_OFF ;        // No blending
        PassInfo.FBMask         = 0x00ffffff ;             // Axxx

        PassInfo.GifTag.Build2(4, 0, GIFPrim, GIF_FLAG_TEXTURE | GIF_FLAG_SMOOTHSHADE) ;
        PassInfo.GifTag.Reg   (GIF_REG_ST, GIF_REG_NOP, GIF_REG_RGBAQ, GIF_REG_XYZ2) ;

        // Add to material passes
        PS2Material_AddPass(pPS2Material, PassInfo) ;

        // Turn on writing of all rgba
        PassInfo.FBMask = 0x00000000 ;  // ARGB
    }

    //======================================================================
    // Reflection pass (used context1 mode is already setup)
    //======================================================================
    if (ReflectTEX)
    {
        // Setup bitmap
        PassInfo.BMP        = &ReflectTEX->GetXBitmap() ;  // Bitmap
        PassInfo.ClutHandle = -1 ;                         // Clut handle

        // Flag reflection uv's to get calculated
        pPS2Material->Flags  |= MATERIAL_FLAG_CALC_REFLECT_UVS ;

        // World space reflection?
        if (DrawInfo.Flags & shape::DRAW_FLAG_REF_WORLD_SPACE) 
            pPS2Material->Flags |= MATERIAL_FLAG_REFLECT_IN_WORLD_SPACE ;
        else
        // Random?
        if (DrawInfo.Flags & shape::DRAW_FLAG_REF_RANDOM)
            pPS2Material->Flags |= MATERIAL_FLAG_REFLECT_RANDOM ;

        // Setup pass info
        PassInfo.Context = 1 ;

        // Setup per pixel mode?
        if ( (AlphaTEX) || ((DiffuseTEX) && (DiffuseTEX->HasAlpha())) )
        {
            // Frame buffer alpha is the per pixel control, so setup: (Src-0)*DstA + Dst
            PassInfo.AlphaBlendMode = ALPHA_BLEND_MODE(C_SRC, C_ZERO, A_DST, C_DST) ;
        }            
        else
        {
            // Add to frame buffer, but fade out with fog color and vertex alpha
            PassInfo.AlphaBlendMode = ALPHA_BLEND_ADD ;
            PassInfo.FixedAlpha     = (u32)((255 - DrawInfo.FogColor.A)*((128.0f * pPS2Material->Col[3])/255.0f)) ;
        }

        // Setup gif tag (NLOOPS will be setup by micro code)
        PassInfo.GifTag.Build2(4, 0, GIFPrim, GIF_FLAG_TEXTURE | GIF_FLAG_SMOOTHSHADE | GIF_FLAG_CONTEXT | GIF_FLAG_ALPHA) ;
        PassInfo.GifTag.Reg(GIF_REG_NOP, GIF_REG_ST, GIF_REG_RGBAQ, GIF_REG_XYZ2) ;

        // Add to material passes
        PS2Material_AddPass(pPS2Material, PassInfo) ;
    }

    //======================================================================
    // Detail pass? (always the last stage)
    //======================================================================
    
    if (DetailTEX)
    {
        // Setup bitmap
        PassInfo.BMP        = &DetailTEX->GetXBitmap() ;    // Bitmap
        PassInfo.ClutHandle = DetailClutHandle ;            // Clut handle

        // Setup pass info
        PassInfo.Context = 0 ;

        PassInfo.AlphaBlendMode = ALPHA_BLEND_INTERP ;  // Use texture alpha: (Src-Dst)*ASrc

        PassInfo.GifTag.Build2(4, 0, GIFPrim, GifFlags | GIF_FLAG_ALPHA) ;
        PassInfo.GifTag.Reg   (GIF_REG_ST, GIF_REG_NOP, GIF_REG_RGBAQ, GIF_REG_XYZ2) ;

        // Add to material passes
        PS2Material_AddPass(pPS2Material, PassInfo) ;
    }

    //======================================================================
    // Special cases
    //======================================================================

    // End the material
    PS2Material_End(pPS2Material, PassCount) ;

    // Self illum does not get multiplied by the light in vu code, so we do the multiply here
    // (after everything has used the color! eg.reflection map for fading out)
    if ((m_Flags.SelfIllum) || (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_LIGHTING))
    {
        pPS2Material->Flags |= MATERIAL_FLAG_SELF_ILLUM ;
        pPS2Material->Col[0] *= 127.0f ;
        pPS2Material->Col[1] *= 127.0f ;
        pPS2Material->Col[2] *= 127.0f ;
        pPS2Material->Col[3] *= 128.0f ;
    }

    #ifdef X_DEBUG
        s_LoadColor++ ;
    #endif
}

//==============================================================================

#endif  //#ifdef TARGET_PS2


xbool material::ActivateTextures(const material_draw_info& DrawInfo, const texture_draw_info& TexDrawInfo)
{
// PC is easy...
#ifdef TARGET_PC

    (void)DrawInfo ;
    (void)TexDrawInfo ;
    return TRUE ;

#else

    s32     i ;
    s32     Attempt   = 0 ;
    xbool   KickedOut = FALSE ;
    xbool   Activated = FALSE ;

    // Lookup detail texture
    texture* DetailTexture    = TexDrawInfo.DetailTexture ;
    s32      DetailClutHandle = TexDrawInfo.DetailClutHandle ;

    // Mip map corruption fix - tell vram ready for texture load
    eng_PushGSContext(0);
    vram_SetMipEquation(DrawInfo.MipK) ;
    eng_PopGSContext();
    eng_PushGSContext(1);
    vram_SetMipEquation(DrawInfo.MipK) ;
    eng_PopGSContext();

    // Repeat incase texture get kicked out on first try
    do
    {
        // Activate all textures
        for (i = material::TEXTURE_TYPE_START ; i <= material::TEXTURE_TYPE_END ; i++)
        {
            // Lookup texture
            texture* Texture  = TexDrawInfo.Texture[i] ;
            if (Texture)
            {
                ASSERT(Texture->IsValid()) ;

                // Update stats
                #ifdef X_DEBUG
                    s_LoadTex++ ;
                #endif

                // Flag a texture was activated
                Activated = TRUE ;

                // Activate which texture?
                switch(i)
                {
                    case material::TEXTURE_TYPE_ALPHA:
                    case material::TEXTURE_TYPE_DIFFUSE:
                    case material::TEXTURE_TYPE_LUMINANCE:

                        #ifdef TARGET_PS2
                            // Load alpha texture into vram
                            eng_PushGSContext(0);
                            Texture->ActivateMips(DrawInfo.MinLOD, DrawInfo.MaxLOD) ;
                            eng_PopGSContext();
                        #endif

                        break ;

                    case material::TEXTURE_TYPE_REFLECT:

                        #ifdef TARGET_PS2
                            // Load reflection texture into vram
                            eng_PushGSContext(1);
                            //Texture->ActivateMips(DrawInfo.MinLOD, DrawInfo.MaxLOD) ;
                            // Temp for now - looks like mips on context 1 do not work...
                            Texture->ActivateMips(-100,100) ;
                            eng_PopGSContext();
                        #endif
                        
                        break ;

                    case material::TEXTURE_TYPE_BUMP:

                        #ifdef TARGET_PS2
                            // No bump mapping on the PS2
                        #endif
                        
                        break ;

                }
            }
        }

        // Activate detail texture?
        if (DetailTexture)
        {
            ASSERT(DetailTexture) ;
            ASSERT(DetailTexture->IsValid()) ;

            #ifdef TARGET_PS2

                // Load detail texture into vram
                eng_PushGSContext(0);

                //DetailTexture->ActivateMips(DrawInfo.MinLOD, DrawInfo.MaxLOD) ;
                DetailTexture->Activate() ;

                // Load clut into vram    
                if (DetailClutHandle != -1)
                    vram_ActivateClut( DetailClutHandle ) ;

                eng_PopGSContext();
            #endif
        }

        #ifdef TARGET_PS2
            // Check to see if any textures got kicked out
            KickedOut = FALSE ;
            for (i = material::TEXTURE_TYPE_START ; i <= material::TEXTURE_TYPE_END ; i++)
            {
                // Lookup texture
                texture* Texture  = TexDrawInfo.Texture[i] ;
    
                // Is there a texture that is not active?
                if ((Texture) && (!Texture->IsActive(DrawInfo.MinLOD, DrawInfo.MaxLOD)))
                {
                    // Flag a texture got kicked out, and increase the attempt
                    KickedOut = TRUE ;
                    break ;
                }
            }

            // Was detail texture kicked out?
            if (DetailTexture)
            {
                // Was detail texture kicked out?
                if (!DetailTexture->IsActive())
                    KickedOut = TRUE ;

                // Was detail clut kicked out?
                if ( (DetailClutHandle != -1) && (!vram_IsClutActive( DetailClutHandle )) )
                    KickedOut = TRUE ;
            }

        #endif
        
        // Was a texture kicked out?
        if (KickedOut)
        {
            // Okay - time for emergency action - cleanout vram and try again
            if (Attempt == 0)
                vram_Flush();

            // Try again
            Attempt++ ;
        }

    }
    while((KickedOut) && (Attempt < 2)) ;   // Keep going unless we fail twice...

    // Ran out of VRAM?
    if (Attempt >= 2)
    {
        // Show info
        u32 TotalSize=0;
        x_DebugMsg("Material:%s - Not all textures fit in VRAM together!!!\n", m_Name) ;
        for (i = material::TEXTURE_TYPE_START ; i <= material::TEXTURE_TYPE_END ; i++)
        {
            // Lookup texture
            texture* Texture = TexDrawInfo.Texture[i] ;
            if (Texture)
            {
                xbitmap& BMP = Texture->GetXBitmap() ;
                x_DebugMsg("   Texture:%s\n Width:%d Height:%d BPP:%d BPC:%d DataSize:%d ClutSize:%d\n", 
                          Texture->GetName(),
                          BMP.GetWidth(),
                          BMP.GetHeight(),
                          BMP.GetBPP(),
                          BMP.GetBPC(),
                          BMP.GetDataSize(),
                          BMP.GetClutSize()) ;
                TotalSize += BMP.GetDataSize() ;
                TotalSize += BMP.GetClutSize() ;
            }
        }

        // Detail texture?
        if (DetailTexture)
        {
            xbitmap& BMP = DetailTexture->GetXBitmap() ;
            x_DebugMsg("   Texture:%s\n Width:%d Height:%d BPP:%d BPC:%d DataSize:%d ClutSize:%d\n", 
                      DetailTexture->GetName(),
                      BMP.GetWidth(),
                      BMP.GetHeight(),
                      BMP.GetBPP(),
                      BMP.GetBPC(),
                      BMP.GetDataSize(),
                      BMP.GetClutSize()) ;
            TotalSize += BMP.GetDataSize() ;
            TotalSize += BMP.GetClutSize() ;
        }

        x_DebugMsg("   TotalSize:%d\n", TotalSize) ;

        ASSERTS(0, "Make your textures smaller buddy!\n\n") ;
    }

    // Return TRUE if any textures were activated
    return Activated ;

#endif

}

//==============================================================================

// Builds one single strip with all the verts in it
void material::PS2_BuildBigSingleStrip(s32          nNode,
                                       s32          nModelVerts,
                                       strip_vert*  &StripVerts,
                                       s32          &nStripVerts)
{
    s32 i,j, Saves=0 ;

    // Stop warning
    (void)nModelVerts ;

    // Count max total verts
    s32 TotalVerts = 0 ;
    for (i = 0 ; i < m_nStrips ; i++)
    {
        // Is this strip connected to this node?
        if (m_Strips[i].GetNode() == nNode)
            TotalVerts += m_Strips[i].GetVertCount() ;
    }
    
    // Allocate max
    StripVerts  = new strip_vert[TotalVerts] ;
    ASSERT(StripVerts) ;
    nStripVerts = 0 ;

    // Loop through all the strips
    s32     VertIndex=-1, VertNoKick =-1, VertOrient;
    s32     LastVertIndex=-1, LastVertNoKick=-1 ;
    for (i = 0 ; i < m_nStrips ; i++)
    {
        strip &S = m_Strips[i] ;
        if (S.GetNode() == nNode)
        {
            // Loop through all the verts in the strip
            for (j = 0 ; j < S.GetVertCount() ; j++)
            {
                xbool   SkipVert = FALSE ;

                // Get vert info
                VertIndex  = S.GetVert(j).GetVertIndex() ;
                VertNoKick = S.GetVert(j).GetNoKick() ;
                VertOrient = S.GetVert(j).GetOrient() ;

                ASSERT(VertIndex >= 0) ;
                ASSERT(VertIndex < nModelVerts) ;

                // If the last and current vert are equal, and the last vert was drawn,
                // but the next vert is not, then we can skip this vert!
                if ((VertIndex == LastVertIndex) && (!LastVertNoKick) && (VertNoKick))
                    SkipVert = TRUE ;

                // Keep this vert?
                if (!SkipVert)
                {
                    StripVerts[nStripVerts].SetVertIndex(VertIndex) ;
                    StripVerts[nStripVerts].SetNoKick(VertNoKick) ;
                    StripVerts[nStripVerts].SetOrient(VertOrient) ;
                    nStripVerts++ ;
                }
                else
                {
                    Saves++ ;
                }

                // Next vert
                LastVertIndex  = VertIndex ;
                LastVertNoKick = VertNoKick ;
            }
        }
    }
    
    // Make sure we built everything!
    ASSERT(nStripVerts <= TotalVerts) ;

    // Show info
    shape::printf("   finished building PS2 combined strip with %d verts\n", nStripVerts) ;
}

//==============================================================================

// Builds material display list so that it can be referenced by cpu
void material::PS2_BuildDList(vert       ModelVerts[], 
                              s32        nModelVerts, 
                              s32        Node,
                              strip_vert StripVerts[],
                              s32        nStripVerts,
                              byte*      &DList,
                              s32        &DListSize,       
                              s32        &nDListVerts, 
                              s32        &nDListPolys)
{
    // Strip transfer pack
    struct strip_transfer_pack
    {
        s32 BASE ;
        s32 OFFSET ;
    } ;

    // Params used by vu code
    struct params
    {
        u16 nStripVerts ;
        u16 NodeAddr ;
        u16 nTransVerts ;
        u16 Unused ;
    } ;

    // Params pack
    struct params_pack
    {
        s32     UNPACK ;
        params  PARAMS ;
    } ;

    // UV's
    struct vert_uv_pack
    {
        s16 s,t ;
    } ;

    // Normals
    struct vert_norm_pack
    {
        s8 nx,ny,nz ;
    } ;

    // Positions
    struct vert_pos_pack
    {
        s16 x,y,z,adc_addr ;
    } ;

    // Stop warning
    (void)nModelVerts ;

    s32     i ;
    params  Params ;


#define VU_BUFFERS  3

    // Setup vertex buffer addresses
    s32 Buffer = 0 ;
    s32 BaseAddr   = SHAPE_BASE_ADDR(1); //nNodes) ;
    s32 BufferSize = SHAPE_BUFFER_SIZE(BaseAddr, VU_BUFFERS) ;
    s32 VUBufferAddr[VU_BUFFERS] ;
    s32 VUBufferSize[VU_BUFFERS] ;
    for (i = 0 ; i < VU_BUFFERS ; i++)
    {
        VUBufferAddr[i] = BaseAddr + (BufferSize*i) ;
        VUBufferSize[i] = BufferSize ;
    }
    // Top up last buffer with extra verts incase vu memory is not a multiple of the buffer size
    VUBufferSize[VU_BUFFERS-1] += 1024 - (VUBufferAddr[VU_BUFFERS-1] + VUBufferSize[VU_BUFFERS-1]) ;

    // Show buffer info
    for (i = 0 ; i < VU_BUFFERS ; i++)
        shape::printf("   vert cache size %d\n", VUBufferSize[i]) ;

    // Allocate build buffer
    byte *BuildBuffer = (byte *)x_malloc(1024*256) ;
    ASSERT(BuildBuffer) ;

    // Make sure start is aligned
    s32 DListStart = ALIGN_16((s32)BuildBuffer) ;

    // Construct dlist builder from it
    dlist_builder DLBuilder ;
    DLBuilder.Setup((byte *)DListStart, 1024*256) ;

    // Macros to access builder class
    #define DL_BUILD_ALLOC(__x__)     DLBuilder.Alloc(__x__)
    #define DL_BUILD_STRUCT(__s__)    (__s__ *)DL_BUILD_ALLOC(sizeof(__s__))
    #define DL_BUILD_GET_START_ADDR() DLBuilder.GetStartAddr()
    #define DL_BUILD_GET_LENGTH()     DLBuilder.GetLength()
    #define DL_BUILD_SET_ADDR(__x__)  DLBuilder.SetAddr(__x__)
    #define DL_BUILD_ALIGN16()        DLBuilder.Align16()

    // Make sure start is aligned!
    ASSERT_ALIGNED16(DL_BUILD_GET_START_ADDR()) ;

    // Show info
    shape::printf("building material display list...\n") ;

    // Get vert structure info
    s32 UVOffset, NormOffset, PosOffset, VertSize ;
    s32 MSCAL_LoadNode ;
    s32 MSCAL_Draw ;
    s32 VertSTCYCL ;
    
    // Load node command
    MSCAL_LoadNode = SCE_VIF1_SET_MSCAL(SHAPE_MICRO_CODE_LOAD_NODE,0) ;

    // Textured diffuse + textured reflect
    UVOffset   = VERT_UV ;
    NormOffset = VERT_NORM ;
    PosOffset  = VERT_POS ;
    VertSize   = VERT_SIZE ;

    // Draw command
    MSCAL_Draw = SCE_VIF1_SET_MSCAL(SHAPE_MICRO_CODE_DRAW,0) ;

    // Vert skip size (since components are written independently)
    VertSTCYCL = VIF_SkipWrite(1, VertSize-1) ;

    // Get total verts to process
    s32 TotalVerts = nStripVerts ;

    // Join strips together
    s32 CurrentVert = 0 ;
    s32 VertIndex, NoKick, Orient ;
    s32 LoadedNode = -1 ;
    s32 CachedVerts = 0 ;

    s32 TotalVertsAdded = 0 ;
    s32 TotalPolysAdded = 0 ;

    // Loading of node will have changed the cycle write settings so reset it ready for transfering verts
    *(DL_BUILD_STRUCT(s32)) = VertSTCYCL ;

    // Put all verts into strips
    while(CurrentVert < TotalVerts)
    {
        // These should be at least 3 verts to get here!
        ASSERT((TotalVerts - CurrentVert) >= 3) ;

        // Get max strip length
        s32 MaxStripLength = (VUBufferSize[Buffer]-SHAPE_VERTS_ADDR) / VertSize ; // leave room for giftag+clip buff

        // Let's count the verts in this combined strip...

        // Get strip length
        s32 nVerts = TotalVerts-CurrentVert ;
        if (nVerts > MaxStripLength)
            nVerts = MaxStripLength ;

        // Remove no kick verts from end of strip
        while(StripVerts[CurrentVert+nVerts-1].GetNoKick())
            nVerts-- ;
      
        // Should be enough for 1 strip!
        ASSERT(nVerts >= 3) ;

        // Keep copy of strip start
        s32 StripStartVert = CurrentVert ;
        s32 StripEndVert   = CurrentVert + nVerts ;

        (void)StripStartVert;   // Surpress a warning.

        // Setup strip transfer
        s32 BufferAddr  = VUBufferAddr[Buffer] ;
        s32 VertAddr    = SHAPE_VERTS_ADDR ;       // skip gif tag

        // Add begin strip transfer pack to set buffer address for verts transfer
        s32 AddBeginStripPack = nVerts ;
        strip_transfer_pack *StripTransferPack = (strip_transfer_pack *)DL_BUILD_ALLOC(sizeof(strip_transfer_pack)) ;
        ASSERT_ALIGNED4(StripTransferPack) ;
        StripTransferPack->BASE   = SCE_VIF1_SET_BASE(BufferAddr, 0) ;
        StripTransferPack->OFFSET = SCE_VIF1_SET_OFFSET(0, 0) ;

        // Loop through all verts in this combined strip...
        s32 CombinedStripVert = 0 ;
        while(nVerts)
        {
            // Get the current vert info
            ASSERT(CurrentVert >= StripStartVert) ;
            ASSERT(CurrentVert < StripEndVert) ;

            VertIndex = StripVerts[CurrentVert].GetVertIndex() ;

            // Find out how many verts attached to the same node are in this transform loop
            s32 nTransVerts = 0 ;
            while((CurrentVert + nTransVerts) < StripEndVert)
                nTransVerts++ ;

            // Must be some verts!
            ASSERT( (CurrentVert + nTransVerts) <= StripEndVert) ;
            ASSERT(nTransVerts) ;
            ASSERT(nTransVerts <= nVerts) ;

            // Force loading of new node
            s32 AddLoadNodePack = -1 ;
            if (LoadedNode != Node)
            {
                // Add load bone pack
                LoadedNode      = Node ;
                AddLoadNodePack = LoadedNode ;
            }

            // UV's
            vert_uv_pack *VertUVPack = NULL ;
            if (m_TextureInfo[TEXTURE_TYPE_DIFFUSE].Index != -1)
            {
                //VIF_Mask((u32 *)DL_BUILD_ALLOC(sizeof(s32)*2),             // STMASK
                         //VIF_ASIS, VIF_ASIS, VIF_ROW, VIF_ROW,      // x0,y0,z0,w0
                         //VIF_ASIS, VIF_ASIS, VIF_ROW, VIF_ROW,      // x1,y1,z1,w1
                         //VIF_ASIS, VIF_ASIS, VIF_ROW, VIF_ROW,      // x2,y2,z2,w2
                         //VIF_ASIS, VIF_ASIS, VIF_ROW, VIF_ROW) ;    // x3,y3,z3,w3

                *DL_BUILD_STRUCT(u32) = VIF_Unpack(VertAddr + UVOffset, // DestVUAddr
                                                   nTransVerts,         // NVectors
                                                   VIF_V2_16,           // Format
                                                   TRUE,                // Signed 
                                                   FALSE,//TRUE,                // Masked
                                                   FALSE) ;             // AbsoluteAddress

                VertUVPack = (vert_uv_pack *)DL_BUILD_ALLOC(sizeof(vert_uv_pack) * nTransVerts) ;
                ASSERT_ALIGNED4(VertUVPack) ;
            }

            // Normals
            VIF_Mask((u32 *)DL_BUILD_ALLOC(sizeof(s32)*2),              // STMASK
                     VIF_ASIS, VIF_ASIS, VIF_ASIS, VIF_ROW,      // x0,y0,z0,w0
                     VIF_ASIS, VIF_ASIS, VIF_ASIS, VIF_ROW,      // x1,y1,z1,w1
                     VIF_ASIS, VIF_ASIS, VIF_ASIS, VIF_ROW,      // x2,y2,z2,w2
                     VIF_ASIS, VIF_ASIS, VIF_ASIS, VIF_ROW) ;    // x3,y3,z3,w3

            *DL_BUILD_STRUCT(s32) = VIF_Unpack(VertAddr + NormOffset,   // DestVUAddr
                                               nTransVerts,             // NVectors
                                               VIF_V3_8,                // Format
                                               TRUE,                    // Signed
                                               TRUE,                    // Masked (puts 0 in W component)
                                               FALSE) ;                 // AbsoluteAddress
            
            vert_norm_pack *VertNormPack = (vert_norm_pack *)DL_BUILD_ALLOC(ALIGN_4(sizeof(vert_norm_pack) * nTransVerts)) ;
            ASSERT_ALIGNED4(VertNormPack) ;
           
            // Positions
            *DL_BUILD_STRUCT(s32) = SCE_VIF1_SET_STMOD(1,0) ;   // Turn on VIF add mode
            *DL_BUILD_STRUCT(s32) = VIF_Unpack(VertAddr + PosOffset,    // DestVUAddr
                                               nTransVerts,             // NVectors
                                               VIF_V4_16,               // Format
                                               TRUE,                    // Signed
                                               FALSE,                   // Masked
                                               FALSE) ;                 // AbsoluteAddress

            vert_pos_pack *VertPosPack = (vert_pos_pack *)DL_BUILD_ALLOC(sizeof(vert_pos_pack) * nTransVerts) ;
            ASSERT_ALIGNED4(VertPosPack) ;
            *DL_BUILD_STRUCT(s32) = SCE_VIF1_SET_STMOD(0,0) ;   // Turn off VIF add mode

            // Update totals
            TotalVertsAdded += nTransVerts ;

            // Add strip to current dlist
            dlist_strip DListStrip ;
            s32 PosOffset, UVOffset, NormOffset ;
            PosOffset  = (s32)VertPosPack  - DListStart ;
            NormOffset = (s32)VertNormPack - DListStart ;
            if (VertUVPack)
                UVOffset = (s32)VertUVPack - DListStart ;
            else
                UVOffset = 0 ;    // Makes pointer become NULL
            DListStrip.Setup(nTransVerts, (void*)PosOffset, (void*)UVOffset, (void*)NormOffset) ; 
            m_DLists[m_nDLists].AddStrip(DListStrip) ;

            // Setup verts to transform
            for (i = 0 ; i < nTransVerts ; i++)
            {
                ASSERT(CurrentVert < TotalVerts) ;

                // Get next vert to add
                Orient    = StripVerts[CurrentVert].GetOrient() ;
                VertIndex = StripVerts[CurrentVert].GetVertIndex() ;
                NoKick    = StripVerts[CurrentVert].GetNoKick() ;
                NoKick |= (CombinedStripVert < 2) ;
                
                ASSERT(VertIndex >= 0) ;
                ASSERT(VertIndex < nModelVerts) ;
                vert &V = ModelVerts[VertIndex] ;

                // Update display list stats
                nDListVerts++ ;
                if (!NoKick)
                    TotalPolysAdded++ ;

                // Setup UV;s
                if (VertUVPack)
                {
                    // UV's must be in range
                    //ASSERTS((V.GetTexCoord().X >= -8) && (V.GetTexCoord().X <= 7), "PS2 wrap error: U tex co-ord if out of range -8 to +7") ;
                    //ASSERTS((V.GetTexCoord().Y >= -8) && (V.GetTexCoord().Y <= 7), "PS2 wrap error: V tex co-ord if out of range -8 to +7") ;
                    if ((V.GetTexCoord().X < -8) || (V.GetTexCoord().X > 7))
                        shape::printf("WARNING: PS2 wrap error: U (%f) tex co-ord is out of range -8 to +7\n", V.GetTexCoord().X) ;

                    if ((V.GetTexCoord().Y < -8) || (V.GetTexCoord().Y > 7))
                        shape::printf("WARNING: PS2 wrap error: V (%f) tex co-ord is out of range -8 to +7\n", V.GetTexCoord().Y) ;

                    // Setup UV's
                    VertUVPack[i].s = (s16)(V.GetTexCoord().X * (f32)(1<<12)) ;
                    VertUVPack[i].t = (s16)(V.GetTexCoord().Y * (f32)(1<<12)) ;
                }

                // Setup normal
                VertNormPack[i].nx = (s8)(V.GetNormal().X*127.0f) ;
                VertNormPack[i].ny = (s8)(V.GetNormal().Y*127.0f) ;
                VertNormPack[i].nz = (s8)(V.GetNormal().Z*127.0f) ;

                // Setup pos
                VertPosPack[i].x = (s16)V.GetPos().X ;
                VertPosPack[i].y = (s16)V.GetPos().Y ;
                VertPosPack[i].z = (s16)V.GetPos().Z ;
                ASSERT((VertAddr & (1<<15)) == 0) ;
                VertPosPack[i].adc_addr  = (s16)(NoKick<<15) ;
                VertPosPack[i].adc_addr |= (s16)(Orient<<14) ;

                // VU code no longer uses any cache address
                #if 0
                    // Is this vert cached?
                    // NOTE: the (CurrentVert-1) is there because we can't cache the last vert since
                    //       the vu code is interleaved and it is not written before the next vert read!
                    s32 CachedVert      = StripStartVert ;
                    s32 CachedVertAddr  = 1 ;
                    while(CachedVert < (CurrentVert-1))
                    {
                        // Already transformed in this strip?
                        if (StripVerts[CachedVert].GetVertIndex() == VertIndex)
                        {
                            VertPosPack[i].adc_addr = (s16)((CachedVertAddr+BufferAddr) | (NoKick<<15)) ;
                            CachedVerts++ ;
                            break ;   
                        }

                        // Check next vert
                        CachedVert++ ;
                        CachedVertAddr += VertSize ;
                    }
                #endif

                // Next vert
                CurrentVert++ ;
                CombinedStripVert++ ;
                nVerts-- ;
                VertAddr += VertSize ;
                ASSERT(VertAddr < 1024) ;
            }

            // Wait for vu to finish before wiping over params
            *DL_BUILD_STRUCT(s32) = SCE_VIF1_SET_FLUSHE(0) ;

            // Transfer the next set of params        
            params_pack *ParamsPack = DL_BUILD_STRUCT(params_pack) ;
            ASSERT_ALIGNED4(ParamsPack) ;
            ParamsPack->UNPACK = VIF_Unpack(SHAPE_PARAMS_ADDR,  // DestVUAddr
                                            1,                  // NVectors
                                            VIF_V4_16,          // Format
                                            FALSE,              // Signed
                                            FALSE,              // Masked
                                            TRUE) ;             // AbsoluteAddress

            // Begging of new strip?
            if (AddBeginStripPack != -1)
            {
                Params.nStripVerts = AddBeginStripPack | 0x8000 ;   // (0x8000 is the GIF EOP)
                //*DL_BUILD_STRUCT(s32) = MSCAL_BeginStrip ;
                AddBeginStripPack = -1 ;
            }

            // Load a new node?
            if (AddLoadNodePack != -1)
            {
                Params.NodeAddr = SHAPE_NODE_ADDR ;
                //Params.NodeAddr = SHAPE_NODE_ADDR + (Node * NODE_SIZE) ;
                *DL_BUILD_STRUCT(u32) = MSCAL_LoadNode ;
                AddLoadNodePack = -1 ;
            }

            // Add transform verts and draw pack (must be after load node pack)
            Params.nTransVerts = nTransVerts ;
            *DL_BUILD_STRUCT(s32) = MSCAL_Draw ;

            // Setup the params
            ParamsPack->PARAMS = Params ;
        }
    
        // Draw the combined strip!        
        //*DL_BUILD_STRUCT(s32) = MSCAL_Draw ;

        // Validate 
        ASSERT(CurrentVert >= StripStartVert) ;
        ASSERT(CurrentVert == StripEndVert) ;

        // Any more verts to go at start of next strip?
        if (CurrentVert < TotalVerts)
        {
            // If next vert is a kick, then we also need to grab the previous 2 verts so the tri is not missed out
            if (StripVerts[CurrentVert].GetNoKick() == 0)
                CurrentVert -= 2 ;
            else
            // If next vert is not a kick, but the one after it is, we need one more vert!
            // (the stripper may have placed a single no kick vert in the middle of a strip)
            if (    (StripVerts[CurrentVert].GetNoKick() == 1) &&
                    (StripVerts[CurrentVert+1].GetNoKick() == 0) )
                CurrentVert -= 1 ;

            // There has got to be at least 3 more verts!
            ASSERT((TotalVerts - CurrentVert) >= 3) ;

            // Validate again...
            ASSERT(CurrentVert >= StripStartVert) ;
            ASSERT(CurrentVert <= StripEndVert) ;
        }

        // Goto next buffer...
        Buffer++ ;
        if (Buffer == VU_BUFFERS)
            Buffer = 0 ;
    }

    // Get end of dlist
    DL_BUILD_ALIGN16() ;

    // Allocate and setup new dlist
    DListSize = DL_BUILD_GET_LENGTH() ;
    DList     = new byte[DListSize] ;
    ASSERT(DList) ;
    x_memcpy(DList, DL_BUILD_GET_START_ADDR(), DL_BUILD_GET_LENGTH()) ;

    // Update dlist count
    nDListPolys += TotalPolysAdded ;

    // Show info
    shape::printf("finished dlist Size:%d bytes (%dk) TotalVerts:%d CachedVerts:%d\n\n",
               DListSize, (DListSize/1024)+1, TotalVertsAdded, CachedVerts) ;

    // Free mem used by dlist
    x_free(BuildBuffer) ;
}

//==============================================================================




//==============================================================================
//
// PC Specific code
//
//==============================================================================

#ifdef TARGET_PC

//==============================================================================
// material display list class
//==============================================================================


// Draws the material
void material_dlist::Draw(const material& Material, const material_draw_info& DrawInfo, s32 &CurrentLoadedNode)
{
    // Skip if node is not visible
    if ( (m_Node < 32) && (!(DrawInfo.NodeVisibleFlags & (1<<m_Node))) )
        return ;

    // Performing animated visibility draw?
    if (DrawInfo.Flags & shape::DRAW_FLAG_VISIBILITY_ANIMATED)
    {
        // If drawing transparent visibility pass, then skip opaque nodes
        if (DrawInfo.Flags & shape::DRAW_FLAG_VISIBILITY_PASS_TRANSPARENT)
        {
            // If this node is opaque, then skip
            if (DrawInfo.RenderNodes[m_Node].Light.Amb.W == 1)
                return ;

            // Totally transparent?
            if (DrawInfo.RenderNodes[m_Node].Light.Amb.W == 0)
                return ;
        }

        // If drawing opqaue visibility pass, then skip transparent nodes
        if (DrawInfo.Flags & shape::DRAW_FLAG_VISIBILITY_PASS_OPAQUE)
        {
            // If this node is transparent, then skip
            if (DrawInfo.RenderNodes[m_Node].Light.Amb.W != 1)
                return ;
        }
    }

    // Load a new node?
    if (CurrentLoadedNode != m_Node)
    {
        render_node &RenderNode = DrawInfo.RenderNodes[m_Node] ;

        // Flag this node is loaded
        CurrentLoadedNode = m_Node ;

        // Must have already called BuildDList!
        ASSERT(m_Node != -1) ;
		ASSERT(m_nPolys) ;
		ASSERT(m_VertSize) ;
        ASSERT(m_DList) ;
        ASSERT(m_DListSize) ;

        // Get current view
        const view* View = eng_GetView() ;
        ASSERT(View) ;

        // Setup the shader constants
        d3d_vs_consts   VSConsts ;

        // Setup useful constants
        VSConsts.Consts[0] = 0.0f ;
        VSConsts.Consts[1] = 1.0f ;
        VSConsts.Consts[2] = -1.0f ;
        VSConsts.Consts[3] = 2.0f ;
        
        // Setup local to clip space matrix
        //VSConsts.L2C = View->GetW2C() * RenderNode.L2W ; // THERES SOMETHING WRONG IN THE XFILES!!!
        VSConsts.L2C = View->GetV2C() * View->GetW2V() * RenderNode.L2W ;
        VSConsts.L2C.Transpose() ;

        // Put light into object space
        vector3 LocalLight(-RenderNode.Light.Dir.X, -RenderNode.Light.Dir.Y, -RenderNode.Light.Dir.Z) ;
        LocalLight = RenderNode.L2W.InvRotateVector(LocalLight) ;
        LocalLight.Normalize() ;

        // Get material color
        const vector4& MatColor = Material.GetColor() ;

	    // Setup color
        vector4 Col ;
        if (Material.GetTextureInfo(material::TEXTURE_TYPE_DIFFUSE).Count)
        {
            // Use texture and color multiplier
            Col.X = DrawInfo.Color[0] ;
            Col.Y = DrawInfo.Color[1] ;
            Col.Z = DrawInfo.Color[2] ;
            Col.W = MatColor.W * DrawInfo.Color[3] ; 
        }
        else
        {
            // User material color and color multiplier
            Col.X = MatColor.X * DrawInfo.Color[0] ; 
            Col.Y = MatColor.Y * DrawInfo.Color[1] ; 
            Col.Z = MatColor.Z * DrawInfo.Color[2] ; 
            Col.W = MatColor.W * DrawInfo.Color[3] ; 
        }

        // Setup lighting
        if ((Material.GetSelfIllumFlag()) || (DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_LIGHTING))
        {
			// Take self illum color also?
			Col.X *= DrawInfo.SelfIllumColor[0] ;
			Col.Y *= DrawInfo.SelfIllumColor[1] ;
			Col.Z *= DrawInfo.SelfIllumColor[2] ;
			Col.W *= DrawInfo.SelfIllumColor[3] ;

            // Trick vertex shader into using no diffuse, but full ambient
            VSConsts.LightDir.Zero() ;
            VSConsts.LightDiffuseCol.Zero() ;
            VSConsts.LightAmbientCol = Col ;

            VSConsts.LightAmbientCol.W *= RenderNode.Light.Amb.W ; // vertex shaders use ambient w as vertex alpha!
        }
        else
        {
            // Use regular lighting
            VSConsts.LightDir.Set(LocalLight.X, LocalLight.Y, LocalLight.Z, 0) ;
            VSConsts.LightDiffuseCol = RenderNode.Light.Col ;
            VSConsts.LightAmbientCol = RenderNode.Light.Amb ; // W component is node visiblity! 

            // Mult by material color so we don't have to do it in the vertex shader...
            VSConsts.LightDiffuseCol.X *= Col.X ;
            VSConsts.LightDiffuseCol.Y *= Col.Y ;
            VSConsts.LightDiffuseCol.Z *= Col.Z ;

            VSConsts.LightAmbientCol.X *= Col.X ;
            VSConsts.LightAmbientCol.Y *= Col.Y ;
            VSConsts.LightAmbientCol.Z *= Col.Z ;
            VSConsts.LightAmbientCol.W *= Col.W ;    // vertex shaders use ambient w as vertex alpha!
        }

        // Setup fog
        VSConsts.FogCol.X = (f32)DrawInfo.FogColor.R * (1.0f / 255.0f) ;
        VSConsts.FogCol.Y = (f32)DrawInfo.FogColor.G * (1.0f / 255.0f) ;
        VSConsts.FogCol.Z = (f32)DrawInfo.FogColor.B * (1.0f / 255.0f) ;
        VSConsts.FogCol.W = 1.0f - ((f32)DrawInfo.FogColor.A * (1.0f / 255.0f)) ;

		// Load the reflection matrix for stage 1?
        if (        (Material.GetTextureInfo(material::TEXTURE_TYPE_REFLECT).Count)
                &&  (!(DrawInfo.Flags & shape::DRAW_FLAG_TURN_OFF_REFLECT_PASS)) )
        {
            matrix4 Reflect ;
            if (DrawInfo.Flags & shape::DRAW_FLAG_REF_RANDOM)
            {
                // Random reflection
                Reflect.Identity() ;
                Reflect.SetRotation(radian3(x_frand(-R_180, R_180), x_frand(-R_180, R_180), x_frand(-R_180, R_180))) ;
            }
            else
            {
                // Get look direction in view space
                vector3 LookOffset  = View->GetW2V() * RenderNode.L2W.GetTranslation() ;
                LookOffset.Normalize() ;
                vector3 UpOffset    = v3_Cross(LookOffset, vector3(1,0,0)) ;
                vector3 RightOffset = v3_Cross(UpOffset, LookOffset) ;

                // Setup look matrix (used to rotate co-ords so that environment map moves when object does)
                matrix4 Look ;
                Look(0, 0) = RightOffset.X; Look(0, 1) = UpOffset.X;  Look(0, 2) = LookOffset.X;  Look(0, 3) = 0.0f;
                Look(1, 0) = RightOffset.Y; Look(1, 1) = UpOffset.Y;  Look(1, 2) = LookOffset.Y;  Look(1, 3) = 0.0f;
                Look(2, 0) = RightOffset.Z; Look(2, 1) = UpOffset.Z;  Look(2, 2) = LookOffset.Z;  Look(2, 3) = 0.0f;
                Look(3, 0) = Look(3, 1) = Look(3, 2) = Look(3, 3) = 0.0f ;

                // World space?
                if (DrawInfo.Flags & shape::DRAW_FLAG_REF_WORLD_SPACE)
                {
                    // World space reflection
		            Reflect = Look * RenderNode.L2W ;
                }
                else
                {
                    // View space reflection
                    Reflect = Look * View->GetW2V() * RenderNode.L2W ;
                }

                // Make it so uv's go from 0->1
                // NOTE: the - sign is so that the relfection map orientates like the map itself
                // (that way you can use a sky texture etc)
                Reflect.Orthogonalize() ;
                Reflect.Scale(-0.5f) ;
                Reflect.SetTranslation(vector3(0.5f, 0.5f, 0.5f)) ;
            }

            Reflect.Transpose() ;
            VSConsts.Ref = Reflect ;
        }

        // Setup pixel shader constants
        d3d_ps_consts   PSConsts ;

        // Get light dir - need to bias and scale, because pixel shader will clamp these values
        PSConsts.LightDir        = VSConsts.LightDir ;
        PSConsts.LightDiffuseCol = VSConsts.LightDiffuseCol ;
        PSConsts.LightAmbientCol = VSConsts.LightAmbientCol ;

        // Setup H vector = 0.5 * (L + V)
        vector3 ViewDir  = -View->GetViewZ() ;
        vector3 LightDir(-RenderNode.Light.Dir.X, -RenderNode.Light.Dir.Y, -RenderNode.Light.Dir.Z) ;
        vector3 H = LightDir + ViewDir ;
        H = RenderNode.L2W.InvRotateVector(H) ;
        H.Normalize() ;
        //H *= 0.0f ; // half as bright!
        PSConsts.SpecDir.Set(H.X, H.Y, H.Z, 1) ;
        PSConsts.SpecCol.Set(1.0f, 1.0f, 1.0f, 0.0f) ;

        // Set vertex shader constants into hardware
        g_pd3dDevice->SetVertexShaderConstant(0, &VSConsts, sizeof(VSConsts)/16 ) ;

        // Set pixel shader constants into hardware
        g_pd3dDevice->SetPixelShaderConstant(0, &PSConsts, sizeof(PSConsts)/16 ) ;
    }

    // Use vertex buffers?
    if (m_pVB)
    {
        // Draw from vertex buffer
        g_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(d3d_vert) );

        // Without this check, D3D can crash when the device is lost (due to alt+tab)
        if (g_pd3dDevice->TestCooperativeLevel() == D3D_OK)
            g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    // PrimitiveType,
                                        0,                      // Start vertex
                                        m_nPolys) ;             // PrimitiveCount
    }
    else
    {
        // Draw strip from main memory

        // Without this check, D3D can crash when the device is lost (due to alt+tab)
        if (g_pd3dDevice->TestCooperativeLevel() == D3D_OK)
            g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,  // PrimitiveType,
                                          m_nPolys,             // PrimitiveCount,
                                          (void *)m_DList,      // verts
                                          m_VertSize) ;			// vert stride
    }

    #ifdef X_DEBUG
        material::s_DListDraw++ ;
    #endif
}


//==============================================================================
// material class
//==============================================================================

// Draws the material into the z buffer only (frame buffer is untouched)
void material::PrimeZBuffer(const material_draw_info&   DrawInfo,
                            const texture_draw_info&    TexDrawInfo,
                                  s32&                  CurrentLoadedNode)
{
    // Turn off updating the frame buffer
    //g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0) ;

    // Use diffuse shader
    d3deng_ActivateVertexShader( shape::s_hDifVertexShader );
    d3deng_ActivatePixelShader ( shape::s_hDifPixelShader  ) ;

    // Trick card into not changing the frame buffer, but update the z buffer
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ; 
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // Misc settings
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE) ;
	g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE) ;
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE) ;

    // Setup clipping
    if (DrawInfo.Flags & shape::DRAW_FLAG_CLIP)
        g_pd3dDevice->SetRenderState( D3DRS_CLIPPING, TRUE) ;
    else
        g_pd3dDevice->SetRenderState( D3DRS_CLIPPING, FALSE) ;

    // Draw material
    Draw(DrawInfo, CurrentLoadedNode) ;

    // Restore settings
    ActivateSettings(DrawInfo, TexDrawInfo) ;
    ActivateTextures(DrawInfo, TexDrawInfo) ;

    //g_pd3dDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA |
                                                          //D3DCOLORWRITEENABLE_BLUE  |
                                                          //D3DCOLORWRITEENABLE_GREEN |
                                                          //D3DCOLORWRITEENABLE_RED );

    // Force new node load since reflection matrix is not setup and might be needed
    CurrentLoadedNode = -1 ;
}

//==============================================================================

// Converts a FLOAT to a DWORD for use in SetRenderState() calls
DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

// Activate settings ready for drawing
void material::ActivateSettings(const material_draw_info& DrawInfo, const texture_draw_info& TexDrawInfo)
{
    // Lookup textures
    texture* AlphaTEX   = TexDrawInfo.Texture[TEXTURE_TYPE_ALPHA] ;
    texture* DiffuseTEX = TexDrawInfo.Texture[TEXTURE_TYPE_DIFFUSE] ;
    texture* ReflectTEX = TexDrawInfo.Texture[TEXTURE_TYPE_REFLECT] ;
    texture* BumpTexture    = TexDrawInfo.Texture[TEXTURE_TYPE_BUMP] ;

    // Setup reflection texture stage settings
	if (!ReflectTEX)
	{		
        // Activate textures
        g_pd3dDevice->SetTexture( 1, NULL ) ;

        // Use textured diffuse shaders
        if (DiffuseTEX)
        {
            d3deng_ActivateVertexShader( shape::s_hTexDifVertexShader );
            d3deng_ActivatePixelShader ( shape::s_hTexDifPixelShader  ) ;
        }
        else
        {
            d3deng_ActivateVertexShader( shape::s_hDifVertexShader );
            d3deng_ActivatePixelShader ( shape::s_hDifPixelShader  ) ;
        }
	}
	else
	{
        if (BumpTexture)
        {
            // Stage0=Diffuse, Stage1=Bump, Stage2=Reflect, Stage3=Detail

            // Use diffuse, reflection, bump shaders
            if (shape::s_UseDot3)
            {
                d3deng_ActivateVertexShader( shape::s_hTexDifDot3VertexShader );
                d3deng_ActivatePixelShader ( shape::s_hTexDifDot3PixelShader  ) ;
            }
            else
            {
                d3deng_ActivateVertexShader( shape::s_hTexDifRefBumpVertexShader );
                d3deng_ActivatePixelShader ( shape::s_hTexDifRefBumpPixelShader  ) ;
            }
            
            // Setup bump texture stage
            g_pd3dDevice->SetTexture( 1, vram_GetSurface(BumpTexture->GetXBitmap()) ) ;
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_NONE ) ;
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );

            static f32 mat00=0.5f ;
            static f32 mat01=0.0f ;
            static f32 mat10=0.0f ;
            static f32 mat11=0.5f ;
            static f32 scale=1.0f ;
            static f32 offset=0.0f ;

            //static f32 t=0 ;

            //mat00 = 0.5f + (0.5f*x_sin(t)) ;
            //mat11 = 0.5f + (0.5f*x_sin(t)) ;

            //t += R_1*0.1f ;

            //x_printfxy(1,1,"%f", mat00) ;

            g_pd3dDevice->SetTextureStageState( 2, D3DTSS_BUMPENVMAT00, F2DW(mat00) );
            g_pd3dDevice->SetTextureStageState( 2, D3DTSS_BUMPENVMAT01, F2DW(mat01) );
            g_pd3dDevice->SetTextureStageState( 2, D3DTSS_BUMPENVMAT10, F2DW(mat10) );
            g_pd3dDevice->SetTextureStageState( 2, D3DTSS_BUMPENVMAT11, F2DW(mat11) );
            g_pd3dDevice->SetTextureStageState( 2, D3DTSS_BUMPENVLSCALE, F2DW(scale) );
            g_pd3dDevice->SetTextureStageState( 2, D3DTSS_BUMPENVLOFFSET, F2DW(offset) );

            // Setup reflect texture stage
            g_pd3dDevice->SetTexture( 2, vram_GetSurface(ReflectTEX->GetXBitmap()) ) ;
		    g_pd3dDevice->SetTextureStageState( 2, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 2, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
		    g_pd3dDevice->SetTextureStageState( 2, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );

		    // Terminate texture stages
            g_pd3dDevice->SetTexture( 3, NULL ) ;
        }
        else
        {

            // Stage0=Diffuse, Stage1=Reflect, Stage2=Detail

            // Use diffuse, reflection shaders
            d3deng_ActivateVertexShader( shape::s_hTexDifRefVertexShader );
            d3deng_ActivatePixelShader ( shape::s_hTexDifRefPixelShader  ) ;

            // Setup reflection stage
            g_pd3dDevice->SetTexture( 1, vram_GetSurface(ReflectTEX->GetXBitmap()) ) ;
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR  );
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,  D3DTADDRESS_WRAP );
		    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,  D3DTADDRESS_WRAP );

		    // Terminate texture stages
            g_pd3dDevice->SetTexture( 2, NULL ) ;
	    }
    }

    // Setup diffuse texture settings
    if (DiffuseTEX)
    {
        // Activate textures
        g_pd3dDevice->SetTexture( 0, vram_GetSurface(DiffuseTEX->GetXBitmap()) ) ;

	    // Set bilinear mode
	    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
	    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );

	    // Turn on the trilinear blending
	    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR  );

	    // Set the wraping behavier
	    if (m_Flags.UMirror)
		    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_MIRROR );
	    else
	    if (m_Flags.UWrap)
		    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
	    else
		    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );

	    if (m_Flags.VMirror)
		    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_MIRROR );
	    else
	    if (m_Flags.VWrap)
		    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );
	    else
		    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
    }
    else
    {
        // Turn off texturing
        g_pd3dDevice->SetTexture( 0, NULL ) ;
    }

    // Setup final alpha blending

    // Set additive alpha blend?
    if (m_Flags.Additive)
    {
        g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
    }
    else
    {
        g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ;
        g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ;
    }

    // turn on z buffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE) ; 

    #ifdef X_DEBUG
        s_LoadSettings++ ;
    #endif
}

//==============================================================================

// Activates color etc for material, given new draw info
void material::ActivateColor(const material_draw_info& DrawInfo, const texture_draw_info& TexDrawInfo)
{ 
    //======================================================================
    // Setup fog
    //======================================================================
    
    // Setup fog color
    if (DrawInfo.FogColor.A != 0)
    {
    	g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE) ;
        g_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, DrawInfo.FogColor) ;
    }
    else
    {
    	g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE) ;
    }


    //======================================================================
    // Setup D3D material
    //======================================================================
	// Setup alpha
    f32 Alpha ;
    if (TexDrawInfo.Texture[TEXTURE_TYPE_DIFFUSE])
        Alpha = DrawInfo.Color[3] ; 
    else
        Alpha = m_Color[3]  * DrawInfo.Color[3] ; 

    // Use alpha blend?
    xbool UseAlphaBlend ;
    if (m_Flags.PunchThrough)
        UseAlphaBlend = FALSE ;
    else
    if (m_Flags.Additive)
        UseAlphaBlend = TRUE ;
    else
    if ((m_Flags.HasAlpha) || (Alpha != 1.0f))   // texture need alpha blending?
        UseAlphaBlend = TRUE ;
    else
        UseAlphaBlend = FALSE ;

    // Setup alpha compare test
    if (m_Flags.PunchThrough)
    {
        g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE) ;           
        g_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL) ;
        g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 128) ;
    }
    else
    {
        g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE) ;           
    }

    // Setup alpha enable and z write enable
    if (UseAlphaBlend)
    {
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ; 
        g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,     FALSE) ;
    }
    else
    {
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) ; 
        g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,     TRUE) ;
    }

    // Setup clipping
    if (DrawInfo.Flags & shape::DRAW_FLAG_CLIP)
        g_pd3dDevice->SetRenderState( D3DRS_CLIPPING, TRUE) ;
    else
        g_pd3dDevice->SetRenderState( D3DRS_CLIPPING, FALSE) ;

    #ifdef X_DEBUG
        s_LoadColor++ ;
    #endif
}

#endif  //#ifdef TARGET_PC

//==============================================================================

// Builds one single strip with all the verts in it
void material::PC_BuildBigSingleStrip(s32 nNode, 
                                      s32 nModelVerts, 
                                      strip_vert* &StripVerts, 
                                      s32 &nStripVerts)
{
    s32 Pass, s,i,v ;

    (void)nModelVerts;    // Surpress a warning.

    // Incase of fail
    StripVerts  = NULL ;
    nStripVerts = 0 ;

    // 1st pass counts total verts, 2nd fills in
    for (Pass = 0 ; Pass < 2 ; Pass++)
    {
        // Count verts in section   
	    s32 nVerts = 0 ;

        // Process all strips
        for (s = 0 ; s < m_nStrips ; s++)
        {
            strip &Strip = m_Strips[s] ;
            if (Strip.GetNode() == nNode)
            {
                // Loop through all verts in this strip
                for (v = 0 ; v < Strip.GetVertCount() ; v++)
                {
                    // Grab vert
                    strip_vert &Vert = Strip.GetVert(v) ;
                    s32 VertIndex    = Vert.GetVertIndex() ;
                    s32 VertNoKick   = Vert.GetNoKick() ;
                    s32 VertOrient   = Vert.GetOrient() ;

                    // If no kick, copy the last vert again
                    if (VertNoKick)
                    {
                        if (Pass)
                        {
                            s32 LastVertIndex ;
                            if (nVerts)
                                LastVertIndex = StripVerts[nVerts-1].GetVertIndex() ;
                            else
                                LastVertIndex = Strip.GetVert(0).GetVertIndex() ;

                            // Setup big strip vert
                            ASSERT(nVerts < nStripVerts) ;
                            StripVerts[nVerts].SetVertIndex(LastVertIndex) ;
                            StripVerts[nVerts].SetOrient(VertOrient) ;
                            StripVerts[nVerts].SetNoKick(TRUE) ;
                            nVerts++ ;
                        }
                        else
                            nVerts++ ;
                    }

                    // Copy vert
                    if (Pass)
                    {
                        ASSERT(nVerts < nStripVerts) ;
                        StripVerts[nVerts].SetVertIndex(VertIndex) ;
                        StripVerts[nVerts].SetOrient(VertOrient) ;
                        StripVerts[nVerts].SetNoKick(VertNoKick) ;
                        nVerts++ ;
                    }
                    else
                        nVerts++ ;
                }
            }
        }

        // Allocate strip verts?
        if (!Pass)
        {
            StripVerts  = new strip_vert[nVerts] ;
            nStripVerts = nVerts ;
            for (i = 0 ; i < nVerts ; i++)
                StripVerts[i].SetVertIndex(-1) ;
        }
        else
        {
            // Check strips!
            ASSERT(nVerts == nStripVerts) ;
            for (i = 0 ; i < nVerts ; i++)
            {
                ASSERT(StripVerts[i].GetVertIndex() >= 0) ;
                ASSERT(StripVerts[i].GetVertIndex() < nModelVerts) ;
            }
        }
    }

    // Show info
    shape::printf("   finished building PC combined strip with %d verts\n", nStripVerts) ;
}

//==============================================================================

// Builds material display list so that it can be referenced by cpu
void material::PC_BuildDList(vert       ModelVerts[], 
                             s32        nModelVerts, 
                             s32        Node,
                             strip_vert StripVerts[],
                             s32        nStripVerts,
                             byte*      &DList,
                             s32        &DListSize,       
                             s32        &nDListVerts, 
                             s32        &nDListPolys)
{
    (void)Node ;
    (void)nModelVerts;

    // Update stats
    nDListVerts += nStripVerts ;
    nDListPolys += nStripVerts-2 ;

	// Create new display list
	DListSize   = sizeof(d3d_vert) * nStripVerts ;
	DList       = new byte[DListSize] ;

	// Create and setup a new list of d3d verts
	d3d_vert *D3DVerts = (d3d_vert *)DList ;
	ASSERT(D3DVerts) ;
	for (s32 v = 0 ; v < nStripVerts ; v++)
	{
		s32 VertIndex = StripVerts[v].GetVertIndex() ;
		ASSERT(VertIndex >= 0) ;
		ASSERT(VertIndex < nModelVerts) ;

		vert&   Vert = ModelVerts[VertIndex] ;
		D3DVerts[v].vPos        = Vert.GetPos() ;
		D3DVerts[v].vNormal     = Vert.GetNormal() ;
		D3DVerts[v].TexCoord    = Vert.GetTexCoord() ;
        
        // Lowest bit of pos X co-ord is the cull direction!
        u32* Orient = (u32*)(&D3DVerts[v].vPos.X) ;
        if (StripVerts[v].GetOrient())
            *Orient |= 1 ;
        else
            *Orient &= ~1 ;

        // Lowest bit of pos Y co-ord is the no kick flag
        u32* NoKick = (u32*)(&D3DVerts[v].vPos.Y) ;
        if (StripVerts[v].GetNoKick())
            *NoKick |= 1 ;
        else
            *NoKick &= ~1 ;
	}

    // Add strip to current dlist
    dlist_strip DListStrip ;
    DListStrip.Setup(nStripVerts, NULL, NULL, NULL) ; // All relative
    m_DLists[m_nDLists].AddStrip(DListStrip) ;
}

//==============================================================================

// Builds material display list so that it can be referenced by cpu
void material::BuildDLists(s32                          nTotalPolys, 
                           vert                         ModelVerts[], 
                           s32                          nModelVerts, 
                           s32                          nNodes, 
                           s32&                         nDListVerts, 
                           s32&                         nDListPolys,
                           shape_bin_file_class::target Target)
{
    s32 i,j ;

    // Stop warning
    (void)nTotalPolys ;

    // Clear node flags
    m_NodeFlags = 0 ;

    // Calculate how many nodes have this material - we need a dlist for each node
    s32 nDLists = 0 ;
    for (i = 0 ; i < nNodes ; i++)
    {
        // Loop through strips to see if any are attached to this node
        for (j = 0 ; j < m_nStrips ; j++)
        {
            // Node have a vert attached?
            if (m_Strips[j].GetNode() == i)
            {
                // Increment the dlists needed and goto next node
                nDLists++ ;
                break ;
            }
        }
    }

    // Allocate number of dlists needed
    m_DLists    = new material_dlist[nDLists] ;
    m_nDLists   = 0 ;
    ASSERT(m_DLists) ;

    // Loop through all the nodes
    for (i = 0 ; i < nNodes ; i++)
    {
        strip_vert *StripVerts ;
        s32         nStripVerts ;

        // Build a single strip for this node
        switch(Target)
        {
            case shape_bin_file_class::PC:
                PC_BuildBigSingleStrip(i, nModelVerts, StripVerts, nStripVerts) ;
                break ;

            case shape_bin_file_class::PS2:
                PS2_BuildBigSingleStrip(i, nModelVerts, StripVerts, nStripVerts) ;
                break ;

            default:
                ASSERTS(0, "Target not supported!") ;
                break ;
        }

        // Any geometry in this material attached to this node?
        if (nStripVerts)
        {
            byte *DList    = NULL;
            s32  DListSize = 0 ;
			s32	 nVerts    = 0 ;
			s32	 nPolys    = 0 ;

            // Flags material is on this node
            if (i < 32)     // Only 32 nodes supported currently!!!
                m_NodeFlags |= 1 << i ;
            else
                m_NodeFlags = 0xFFFFFFFF ; // Currently no control over materials on nodes >= 32

            // Build display list for material and node
            switch(Target)
            {
                case shape_bin_file_class::PC:
                    PC_BuildDList(ModelVerts, 
                                  nModelVerts, 
                                  i,
                                  StripVerts,
                                  nStripVerts,
                                  DList,
                                  DListSize,
                                  nVerts, 
                                  nPolys) ;
                    break ;

                case shape_bin_file_class::PS2:
                    PS2_BuildDList(ModelVerts, 
                                   nModelVerts, 
                                   i,
                                   StripVerts,
                                   nStripVerts,
                                   DList,
                                   DListSize,
                                   nVerts, 
                                   nPolys) ;
                    break ;

                default:
                    ASSERTS(0, "Target not supported!") ;
                    break ;
            }

            // Update stats
			nDListVerts += nVerts ;
			nDListPolys += nPolys ;

            ASSERT(DList) ;
            ASSERT(DListSize) ;

            // Setup the next available dlist
            m_DLists[m_nDLists++].Setup(i, nPolys, DListSize / nVerts, DList, DListSize) ;

            // Free memory used to build display list
            delete [] DList ;
            delete [] StripVerts ;
        }
    }


    // Make sure we set all the dlists up!
    ASSERT(m_nDLists == nDLists) ;
}
        
//==============================================================================

// Read/Write to binary file
void material::ReadWrite(shape_bin_file &File)
{
    // Write attributes
    File.ReadWrite(m_Name) ;
    File.ReadWrite(m_Color) ;
    File.ReadWrite(m_Flags) ;
    File.ReadWrite(m_NodeFlags) ;

    // Write texture ref + info indices
    File.ReadWrite(m_TextureRefs) ;
    File.ReadWrite(m_TextureInfo) ;

    // DLists
    File.ReadWriteClassArray(m_DLists, m_nDLists) ;
}

//==============================================================================
