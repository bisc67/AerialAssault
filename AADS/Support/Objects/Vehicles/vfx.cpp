//==============================================================================
//
//  Vfx.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================
#include "..\Demo1\Globals.hpp"
#include "Entropy.hpp"
#include "tokenizer\tokenizer.hpp"
#include "poly\poly.hpp"

#include "vfx.hpp"


//#define VFX_DEBUG


//==============================================================================
//  STATIC STORAGE FOR GLOW BITMAP
//==============================================================================

static xbitmap      s_GlowBmp ;
static s32          s_GlowInit = -1;
static xbitmap      s_ThrustBmp ;
static s32          s_ThrustInit = -1;

//==============================================================================
//  FUNCTIONS
//==============================================================================

//==============================================================================
// ctor
vfx_geom::vfx_geom( )
{
    // initialize the pointers to NULL
    x_memset( m_pGeom, 0, sizeof( geom* ) * MAX_GEOMS );
}

//==============================================================================
// dtor             
vfx_geom::~vfx_geom( )
{

#if 0
    s32 i;
    // find and destroy any allocated geom data
    for ( i = 0; i < MAX_GEOMS; i++ )
    {
        // free the individual allocations for this slot
        if ( m_pGeom[i] )
        {
            if ( m_pGeom[i]->m_pVerts   )     x_free( m_pGeom[i]->m_pVerts  ) ;
            if ( m_pGeom[i]->m_pTVerts  )     x_free( m_pGeom[i]->m_pTVerts ) ;
            if ( m_pGeom[i]->m_pFaces   )     x_free( m_pGeom[i]->m_pFaces  ) ;
            if ( m_pGeom[i]->m_pTFaces  )     x_free( m_pGeom[i]->m_pTFaces ) ;
        }
        
        // and now the struct itself
        x_free( m_pGeom[i] );
    }
#endif
    x_free(m_pData);
}

//==============================================================================
// initialization
void vfx_geom::CreateFromFile( const char* pPath )
{
    token_stream    Geom;
    xbool           r;
    char*           pToken;
    geom            NewGeom;
    s32             MemNeeded;
    byte*           pData = NULL;
    s32             pass;

    r = Geom.OpenFile( pPath );
    ASSERT( r );

    MemNeeded = 0;

    for (pass=0;pass<2;pass++)
    {
        // There are 2 passes, pass 1 determines memory requirements
        // pass 2 will assign allocated memory
        if (pass)
        {
            m_pData = (byte *)x_malloc(MemNeeded);
            ASSERT(m_pData);
            pData = m_pData;
        }

        Geom.Rewind();
        // read the data (very simple format)    
        Geom.Read();
        // begin
        ASSERT( x_stricmp( Geom.String(), "BEGIN" ) == 0 );

        // loop thru the file
        Geom.Read();
        pToken = Geom.String();
        
        while ( x_stricmp( pToken, "DONE" ) != 0 )
        {
            //====================================================================
            // TYPE
            //====================================================================
            if ( x_stricmp( pToken, "TYPE" ) == 0 )
            {
                NewGeom.m_Type = (vfx_geom::type)Geom.ReadInt();
            }
        
            //====================================================================
            // ID
            //====================================================================
            else if ( x_stricmp( pToken, "ID" ) == 0 )
            {
                x_strcpy( NewGeom.ID, Geom.ReadString() );
            }

            //====================================================================
            // VERTEX DATA
            //====================================================================
            else if ( x_stricmp( pToken, "VERTS" ) == 0 )
            {
                matrix4 MaxToEng ;
                MaxToEng.Identity();
                MaxToEng.RotateX(DEG_TO_RAD(-90.0f)) ;
                MaxToEng.RotateY(DEG_TO_RAD(180.0f)) ;
            
                // read in the # of verts
                NewGeom.m_nVerts = Geom.ReadInt();
        
                // alloc storage
                if (pass)
                {
                    NewGeom.m_pVerts = (vector3*)pData;pData+=sizeof(vector3) * NewGeom.m_nVerts;
                    ASSERT( NewGeom.m_pVerts );
                }
                else
                {
                    MemNeeded += sizeof(vector3)*NewGeom.m_nVerts;
                }


                // read in each vert
                for ( s32 i = 0; i < NewGeom.m_nVerts; i++ )
                {
                    vector3 n;

                    n.X = Geom.ReadFloat();
                    n.Y = Geom.ReadFloat();
                    n.Z = Geom.ReadFloat();
                    if (pass)
                    {
                        // re-orient this vertex from MAX to engine
                        NewGeom.m_pVerts[i] = MaxToEng * n; 
                    }

                }
            }

            //====================================================================
            // FACE DATA
            //====================================================================
            else if ( x_stricmp( pToken, "FACES" ) == 0 )
            {
                // read in the # of faces
                NewGeom.m_nFaces = Geom.ReadInt();
        
                if (pass)
                {
                    // alloc storage
                    NewGeom.m_pFaces = (face*)pData;pData+= sizeof(face) * NewGeom.m_nFaces;
                }
                else
                {
                    MemNeeded += sizeof(face)*NewGeom.m_nFaces;
                }

                // read in each face
                for ( s32 i = 0; i < NewGeom.m_nFaces; i++ )
                {
                    face f;

                    f.A = Geom.ReadInt();
                    f.B = Geom.ReadInt();
                    f.C = Geom.ReadInt();

                    if (pass)
                    {
                        NewGeom.m_pFaces[i] = f;
                    }
                }
            }

            //====================================================================
            // TEXTURE VERTS
            //====================================================================
            else if ( x_stricmp( pToken, "TVERTS" ) == 0 )
            {
                // read in the # of verts
                NewGeom.m_nTVerts = Geom.ReadInt();
        
                // alloc storage
                if (pass)
                {
                    NewGeom.m_pTVerts = (vector2*)pData;pData+= sizeof(vector2) * NewGeom.m_nTVerts;
                }
                else
                {
                    MemNeeded+= sizeof(vector2)*NewGeom.m_nTVerts;
                }

                // read in each vert
                for ( s32 i = 0; i < NewGeom.m_nTVerts; i++ )
                {
                    vector2 v;

                    v.X = Geom.ReadFloat();
                    v.Y = Geom.ReadFloat();
                    if (pass)
                    {
                        NewGeom.m_pTVerts[i] = v;
                    }
                }
            }

            //====================================================================
            // TEXTURE FACES
            //====================================================================
            else if ( x_stricmp( pToken, "TFACES" ) == 0 )
            {
                // alloc storage
                if (pass)
                {
                    NewGeom.m_pTFaces = (face*)pData;pData+= sizeof(face) * NewGeom.m_nFaces;
                }
                else
                {
                    MemNeeded = sizeof(face)*NewGeom.m_nFaces;
                }

                // read in each face
                for ( s32 i = 0; i < NewGeom.m_nFaces; i++ )
                {
                    face f;

                    f.A = Geom.ReadInt();
                    f.B = Geom.ReadInt();
                    f.C = Geom.ReadInt();
                    if (pass)
                    {
                        NewGeom.m_pTFaces[i] = f;
                    }
                }
            }

            //====================================================================
            // GLOWY POINT SPRITE CENTER
            //====================================================================
            else if ( x_stricmp( pToken, "CENTER" ) == 0 )
            {
                ASSERT( NewGeom.m_Type == GLOWY_PT_SPRITE );
                vector3 v;

                v.X = Geom.ReadFloat();
                v.Y = Geom.ReadFloat();
                v.Z = Geom.ReadFloat();

                if (pass)
                {
                    matrix4 MaxToEng ;
                    MaxToEng.Identity();
                    MaxToEng.RotateX(DEG_TO_RAD(-90.0f)) ;
                    MaxToEng.RotateY(DEG_TO_RAD(180.0f)) ;

                    NewGeom.m_nVerts = 1;
                    NewGeom.m_pVerts = (vector3*)pData;pData+=sizeof(vector3) * 2;
                    ASSERT( NewGeom.m_pVerts );

                    // read in the vertex
                    // re-orient to Entropy-space
                    NewGeom.m_pVerts[0] = MaxToEng * v;
                }
                else
                {
                    MemNeeded += sizeof(vector3)*2;
                }
            }

            //====================================================================
            // GLOWY POINT SPRITE CENTER
            //====================================================================
            else if ( x_stricmp( pToken, "SIZE" ) == 0 )
            {
                ASSERT( NewGeom.m_Type == GLOWY_PT_SPRITE );

                // read in the size...yes, it's kludgy, but it will work just fine.
                if (pass)
                {
                    NewGeom.m_pVerts[1].X = Geom.ReadFloat();
                }
                else
                {
                    Geom.ReadFloat();
                }
            }

            //====================================================================
            // END_GEOM
            //====================================================================
            else if ( x_stricmp( pToken, "END_GEOM" ) == 0 )
            {
                s32 i;

                if (pass)
                {
                    // all read in, store it
                    for ( i = 0; i < MAX_GEOMS; i++ )
                    {
                        // is this slot available?
                        if ( m_pGeom[i] == NULL )
                        {
                            // allocate it
                            m_pGeom[i] = (geom*)pData;pData+=sizeof(geom);
                            ASSERT( m_pGeom[i] );

                            // assign it
                            *m_pGeom[i] = NewGeom;

                            // clear NewGeom for next entry
                            x_memset( &NewGeom, 0, sizeof(geom) );

                            // done, next
                            break;
                        }
                    }
                    // make sure we had a slot for it
                    ASSERT( i != MAX_GEOMS );
                }
                else
                {
                    MemNeeded+=sizeof(geom);
                }
            }

            // read in the next token
            Geom.Read();
            pToken = Geom.String();
        }
    }
       

    ASSERT(m_pData+MemNeeded == pData);

    // Close it
    Geom.CloseFile();
}

//==============================================================================
// database
s32 vfx_geom::FindGeomByID( const char* pID )
{
    // find the index represented by ID
    for ( s32 i = 0; i < MAX_GEOMS; i++ )
    {
        if ( m_pGeom[i] )
        {
            if ( x_stricmp( m_pGeom[i]->ID, pID ) == 0 )
                return i;
        }
        else
            // not found (first chance exit)
            return -1;
    }
    
    // not found
    return -1;
}

//==============================================================================
// render glowy effect
void vfx_geom::RenderGlowyEffect( const matrix4& L2W, s32 ID, f32 Alpha )
{
    s32         i, j;
    geom*       pGeom;
    ps2color    tmpColor;

    vector3*    s_Scratch;
    vector3*    s_Verts;
    vector2*    s_UV;
    ps2color*   s_Colors;

    // safety checks
    // if you hit any of these asserts, alert John
    ASSERT( ID < MAX_GEOMS );
    ASSERT( ID >= 0 );
    ASSERT( m_pGeom[ID] );
    ASSERT( m_pGeom[ID]->m_Type == vfx_geom::GLOWY_SPOT );
    ASSERT( m_pGeom[ID]->m_nTVerts );

    pGeom = m_pGeom[ID];

    // step 1: allocate scratch memory
    s_Scratch = (vector3*)smem_BufferAlloc( sizeof(vector3) * pGeom->m_nVerts );
    s_Verts =   (vector3*)smem_BufferAlloc( sizeof(vector3) * (pGeom->m_nFaces * 3) ); 
    s_UV =      (vector2*)smem_BufferAlloc( sizeof(vector2) * (pGeom->m_nFaces * 3) );    
    s_Colors =  (ps2color*)smem_BufferAlloc( sizeof(ps2color) * (pGeom->m_nFaces * 3) );  
    
    if ( (!s_Scratch) || (!s_Verts) || (!s_UV) || (!s_Colors) )
        return;

    // step 2: transform the verts

    // make sure the scratch area can hold all the verts
    for ( i = 0; i < pGeom->m_nVerts; i++ )
    {
        // store them in our scratch area to avoid transforming them twice
        s_Scratch[i] = ( L2W * pGeom->m_pVerts[i] );
    }
        
    // step 3: go thru the faces and build a raw triangle list
    tmpColor.Set( 255, 255, 166, (u8)(255 * Alpha) );

    j = 0;
    for ( i = 0; i < pGeom->m_nFaces; i++ )
    {
        // vertex 1
        s_Colors[ j ] = tmpColor;
        s_UV[ j ] = pGeom->m_pTVerts[ pGeom->m_pTFaces[i].A ];
        s_Verts[ j++ ] = s_Scratch[ pGeom->m_pFaces[i].A ];        
        
        // vertex 2
        s_Colors[ j ] = tmpColor;
        s_UV[ j ] = pGeom->m_pTVerts[ pGeom->m_pTFaces[i].B ];
        s_Verts[ j++ ] = s_Scratch[ pGeom->m_pFaces[i].B ];        

        // vertex 3
        s_Colors[ j ] = tmpColor;
        s_UV[ j ] = pGeom->m_pTVerts[ pGeom->m_pTFaces[i].C ];
        s_Verts[ j++ ] = s_Scratch[ pGeom->m_pFaces[i].C ];        
    }

    // step 4: call the poly soup renderer
    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_End();
    #endif

    poly_Begin(POLY_USE_ALPHA);

    // one-time instantiation of the glow bitmap
    if ( s_GlowInit == -1 )
    {
        s_GlowInit = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName( "[A]GlowFX" );
        s_GlowBmp = tgl.GameShapeManager.GetTextureManager().GetTexture( s_GlowInit ).GetXBitmap();
        vram_Register( s_GlowBmp );
    }

    // activate the glow bitmap
    vram_Activate( s_GlowBmp );

    poly_Render( s_Verts, s_UV, s_Colors, j );
    poly_End();

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_OFF );
    gsreg_SetZBufferUpdate(TRUE);
    gsreg_End();
    #endif    

#ifdef VFX_DEBUG
    for ( i = 0; i < pGeom->m_nVerts-1; i++ )
    {
        draw_Line( s_Scratch[i], s_Scratch[i+1], XCOLOR_WHITE );
    }
#endif

}    

//==============================================================================
// render extrusion effect
void vfx_geom::RenderExtrusion( const matrix4& L2W, 
                                  s32 ID_Start,
                                  s32 ID_End, 
                                  f32 Parametric, 
                                  f32 AlphaStart, 
                                  f32 AlphaEnd      )
{
    s32         i, j, k;
    geom*       pGeom1;
    geom*       pGeom2;
    ps2color    tmpColor1;
    ps2color    tmpColor2;

    vector3*    s_Scratch1;
    vector3*    s_Scratch2;
    vector3*    s_Verts;
    vector2*    s_UV;
    ps2color*   s_Colors;

    // safety checks
    // if you hit any of these asserts, alert John
    ASSERT( ID_Start < MAX_GEOMS );
    ASSERT( ID_Start >= 0 );
    ASSERT( m_pGeom[ID_Start] );
    ASSERT( m_pGeom[ID_Start]->m_Type == vfx_geom::EXTRUSION_START );

    ASSERT( ID_End < MAX_GEOMS );
    ASSERT( ID_End >= 0 );
    ASSERT( m_pGeom[ID_End] );
    ASSERT( m_pGeom[ID_End]->m_Type == vfx_geom::EXTRUSION_END );

    // additional sanity
    ASSERT( m_pGeom[ID_Start]->m_nVerts == m_pGeom[ID_End]->m_nVerts );

    pGeom1 = m_pGeom[ID_Start];
    pGeom2 = m_pGeom[ID_End];

    // step 1: allocate scratch memory
    s32 size = pGeom1->m_nVerts * 2;        // size = number of triangles

    s_Scratch1 = (vector3*)smem_BufferAlloc( sizeof(vector3) * pGeom1->m_nVerts );
    s_Scratch2 = (vector3*)smem_BufferAlloc( sizeof(vector3) * pGeom2->m_nVerts );
    s_Verts =   (vector3*)smem_BufferAlloc( sizeof(vector3) * size * 3 ); 
    s_UV =      (vector2*)smem_BufferAlloc( sizeof(vector2) * size * 3 );    
    s_Colors =  (ps2color*)smem_BufferAlloc( sizeof(ps2color) * size * 3 );
    
    if ( (!s_Scratch1) ||(!s_Scratch2) || (!s_Verts) || (!s_UV) || (!s_Colors) )
        return;

    // step 2: transform the verts for both ends
    for ( i = 0; i < pGeom1->m_nVerts; i++ )
    {
        // store them in our scratch area to avoid transforming them twice
        s_Scratch1[i] = ( L2W * pGeom1->m_pVerts[i] );
    }

    for ( i = 0; i < pGeom2->m_nVerts; i++ )
    {
        // same for the end piece, but we need to slide them back towards start
        s_Scratch2[i] = ( L2W * pGeom2->m_pVerts[i] );
        
        vector3 tmp = (s_Scratch2[i] - s_Scratch1[i]) * Parametric;
        s_Scratch2[i] = s_Scratch1[i] + tmp;        
    }

    // step 3: all transformed, now build the triangle list
    tmpColor1.Set( 255, 255, 166, (u8)(255 * AlphaStart) );
    tmpColor2.Set( 255, 255, 166, (u8)(255 * AlphaEnd) );

    if ( (AlphaStart == 0.0f) && (AlphaEnd == 0.0f) )
        return;

    j = 0;
    for ( i = 0; i < pGeom1->m_nVerts; i++ )
    {
        // render 2 triangles for each node
        k = i + 1;
        if ( k == pGeom1->m_nVerts )
            k = 0;

        // triangle 1 vertex 1
        s_Colors[ j ] = tmpColor1;
        s_UV[ j ] = vector2( 0.0f, 0.0f );
        s_Verts[ j++ ] = s_Scratch1[ i ];

        // triangle 1 vertex 2
        s_Colors[ j ] = tmpColor2;
        s_UV[ j ] = vector2( 1.0f, 0.0f );
        s_Verts[ j++ ] = s_Scratch2[ i ];

        // triangle 1 vertex 3
        s_Colors[ j ] = tmpColor1;
        s_UV[ j ] = vector2( 0.0f, 1.0f );
        s_Verts[ j++ ] = s_Scratch1[ k ];
        
        // triangle 2 vertex 1
        s_Colors[ j ] = tmpColor2;
        s_UV[ j ] = vector2( 1.0f, 0.0f );
        s_Verts[ j++ ] = s_Scratch2[ i ];

        // triangle 2 vertex 2
        s_Colors[ j ] = tmpColor1;
        s_UV[ j ] = vector2( 0.0f, 1.0f );
        s_Verts[ j++ ] = s_Scratch1[ k ];

        // triangle 2 vertex 3
        s_Colors[ j ] = tmpColor2;
        s_UV[ j ] = vector2( 1.0f, 1.0f );
        s_Verts[ j++ ] = s_Scratch2[ k ];
    }

    // step 4: call the poly soup renderer
    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_End();
    #endif

    poly_Begin(POLY_USE_ALPHA);

    // one-time instantiation of the thrust bitmap
    if ( s_ThrustInit == -1 )
    {
        s_ThrustInit = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName( "[A]ThrustFX" );
        s_ThrustBmp = tgl.GameShapeManager.GetTextureManager().GetTexture( s_ThrustInit ).GetXBitmap();
        vram_Register( s_ThrustBmp );
    }

    // activate the glow bitmap
    vram_Activate( s_ThrustBmp );

    poly_Render( s_Verts, s_UV, s_Colors, j );
    poly_End();

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_OFF );
    gsreg_SetZBufferUpdate(TRUE);
    gsreg_End();
    #endif                                      

#ifdef VFX_DEBUG
    for ( i = 0; i < pGeom1->m_nVerts; i++ )
    {
        draw_Line( s_Scratch1[i], s_Scratch2[i], XCOLOR_WHITE );
    }
#endif

}

//==============================================================================
// render glowy point sprite
void vfx_geom::RenderGlowyPtSpr( const matrix4& L2W, s32 ID, f32 Alpha )
{
    geom*       pGeom;
    xcolor      tmpColor;

    // safety checks
    // if you hit any of these asserts, alert John
    ASSERT( ID < MAX_GEOMS );
    ASSERT( ID >= 0 );
    ASSERT( m_pGeom[ID] );
    ASSERT( m_pGeom[ID]->m_Type == vfx_geom::GLOWY_PT_SPRITE );

    pGeom = m_pGeom[ID];

    // step 1: transform the verts
    vector3 Pt = L2W * pGeom->m_pVerts[0];
    
    // step 2: get the size
    f32     Size = pGeom->m_pVerts[1].X;
       
    // step 3: set the color
    tmpColor.Set( 255, 255, 166, (u8)(255 * Alpha) );

    // step 4: render the point sprite
    draw_Begin( DRAW_SPRITES, DRAW_TEXTURED | DRAW_USE_ALPHA );
    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_End();
    #endif

    // one-time instantiation of the glow bitmap
    if ( s_GlowInit == -1 )
    {
        s_GlowInit = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName( "[A]GlowFX" );
        s_GlowBmp = tgl.GameShapeManager.GetTextureManager().GetTexture( s_GlowInit ).GetXBitmap();
        vram_Register( s_GlowBmp );
    }

    // activate the glow bitmap
    vram_Activate( s_GlowBmp );

    draw_Sprite( Pt, vector2( Size, Size ), tmpColor );

    draw_End();

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_OFF );
    gsreg_SetZBufferUpdate(TRUE);
    gsreg_End();
    #endif    
}