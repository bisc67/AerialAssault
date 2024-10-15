//==============================================================================
//
//  Bounds.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "Bounds.hpp"
#include "Entropy.hpp"
#include "Auxiliary\Bitmap\aux_Bitmap.hpp"
#include "Poly\Poly.hpp"



//#define BOUNDS_TIMER

//==============================================================================

static xbitmap s_Bitmap;

xbitmap BoundsBitmap;

f32 BoundsMaxQuadDist   = 200.0f;
f32 BoundsMaxEdgeLength = 100.0f;
f32 BoundsTime          =   0.0f;
f32 BoundsTile          =   4.0f;
f32 BoundsPulseLum      =   0.1f;
f32 BoundsPulseMul      =   0.5f;
f32 BoundsDistMul       =   0.25f;

s32 NumQuadsRendered;
s32 NumBoxFaces;

static xcolor WarnColor( 255, 220, 100 );
static xcolor KillColor( 200,  64,  64 );
static xcolor BoundsColor = WarnColor;

xcolor BoundsCol;

xbool DebugBounds = FALSE;
xbool PulseBounds = TRUE;

//==============================================================================

void bounds_SetFatal( xbool Fatal )
{
    if( Fatal )     BoundsColor = KillColor;
    else            BoundsColor = WarnColor;
}

//==============================================================================

void bounds_Init( void )
{
    auxbmp_Load( s_Bitmap, "Data\\bounds.bmp" ); 

    #ifdef TARGET_PC
    s_Bitmap.ConvertFormat( xbitmap::FMT_32_ARGB_8888 );
    #endif

    #ifdef TARGET_PS2
    s_Bitmap.ConvertFormat( xbitmap::FMT_P8_ABGR_8888 );
    s_Bitmap.PS2SwizzleClut();
    #endif

    //s_Bitmap.BuildMips();
    vram_Register( s_Bitmap );
}

//==============================================================================

void bounds_End( void )
{
    vram_Unregister( s_Bitmap );
    s_Bitmap.Kill();
}

//==============================================================================

s32 RenderTiledQuad( const vector3* pVert, const vector2* pUV, xcolor Color, f32 MaxQuadDist, f32 MaxEdgeLength )
{
    // Determine if quad is inside view frustrum
    const view* pView = eng_GetActiveView( 0 );
    bbox BBox( pVert, 4 );
    
    if( pView->BBoxInView( BBox ) == view::VISIBLE_NONE )
        return( 0 );

    // Get location of viewpoint in world
    vector3 CamPos = pView->GetPosition();
    
    // Determine distance from plane of quad
    plane Plane( pVert[0], pVert[1], pVert[2] );
    f32 Dist = Plane.Distance( CamPos );

    if( Dist > MaxQuadDist )
        return( 0 );

    // Get Edge vectors and compute dimensions of sub-quad
    vector3 EdgeX = pVert[1] - pVert[0];
    vector3 EdgeY = pVert[2] - pVert[1];

    f32 LenX = EdgeX.Length() / MaxEdgeLength;
    f32 LenY = EdgeY.Length() / MaxEdgeLength;
    
    // Scale sub-quad dimensions so that we have an integer number of sub-quads
    LenX = EdgeX.Length() / x_ceil( LenX );
    LenY = EdgeY.Length() / x_ceil( LenY );

    vector3 VX( EdgeX );
    vector3 VY( EdgeY );

    VX.Normalize();
    VY.Normalize();
    
    VX *= LenX;
    VY *= LenY;

    // Determine number of sub-quads needed horizontally and vertically
    s32 NumX = (s32)( (EdgeX.Length() / LenX) + 0.5f );
    s32 NumY = (s32)( (EdgeY.Length() / LenY) + 0.5f );
    
    // Setup variables needed for rendering
    vector3 Shared[256];
    vector3 Pts[4];
    
    #ifdef TARGET_PC
    xcolor  RGB[4];
    #endif
    
    #ifdef TARGET_PS2
    ps2color RGB[4];
    #endif

    RGB[0].R = Color.R; RGB[0].G = Color.G; RGB[0].B = Color.B; RGB[0].A = Color.A;
    RGB[1].R = Color.R; RGB[1].G = Color.G; RGB[1].B = Color.B; RGB[1].A = Color.A;
    RGB[2].R = Color.R; RGB[2].G = Color.G; RGB[2].B = Color.B; RGB[2].A = Color.A;
    RGB[3].R = Color.R; RGB[3].G = Color.G; RGB[3].B = Color.B; RGB[3].A = Color.A;
    
    // Initialize vertices of top edge
    ASSERT( NumX < 256 );
    vector3 P( pVert[0] );

    for( s32 X = 0; X <= NumX; X++, P += VX )
    {                      
        Shared[ X ] = P;
    }   

    s32 NumQuads = NumX * NumY;
    
    #ifdef TARGET_PS2
    vector3*  pV = (vector3*) smem_BufferAlloc( sizeof( vector3  ) * NumQuads * 6 );
    vector2*  pT = (vector2*) smem_BufferAlloc( sizeof( vector2  ) * NumQuads * 6 );
    ps2color* pC = (ps2color*)smem_BufferAlloc( sizeof( ps2color ) * NumQuads * 6 );

    vector3*  pVtx = pV;
    vector2*  pTex = pT;
    ps2color* pCol = pC;

    if( (!pV) || (!pT) || (!pC) )
    {
        x_DebugMsg( "Could not alloc buffers for bounds.\n" );
        return( 0 );
    }
    #endif

    NumQuads = 0;
    NumBoxFaces++;

    // Calculate sub-quad vertices and render them    
    for( s32 Y = 0; Y < NumY; Y++ )
    {
        for( s32 X = 0; X < NumX; X++ )
        {
            Pts[0] = Shared[ X ];
            Pts[1] = Shared[X+1];
            
            Shared[X] += VY;
            Pts[3] = Shared[ X ];
            Pts[2] = Shared[X+1] + VY;

            f32 MinDist = 100000000.0f;

            for( s32 i=0; i<4; i++ )
            {
                f32 Dist = (Pts[i] - CamPos).LengthSquared();
                if( Dist < MinDist )
                    MinDist = Dist;

                f32 D = MIN( Dist, MaxQuadDist * MaxQuadDist );
                f32 K = (1.0f - (D / (MaxQuadDist * MaxQuadDist)));
                
                RGB[i].A = (u8)( 255.0f * K );
            }
            
            //if( MinDist > (MaxQuadDist * MaxQuadDist) )
            //    continue;

            #ifdef TARGET_PC
            draw_Color ( RGB[0] );
            draw_UV    ( pUV[0] );
            draw_Vertex( Pts[0] );
            draw_Color ( RGB[3] );             
            draw_UV    ( pUV[3] );
            draw_Vertex( Pts[3] );
            draw_Color ( RGB[2] );
            draw_UV    ( pUV[2] );
            draw_Vertex( Pts[2] );

            draw_Color ( RGB[0] );             
            draw_UV    ( pUV[0] );
            draw_Vertex( Pts[0] );
            draw_Color ( RGB[2] );             
            draw_UV    ( pUV[2] );
            draw_Vertex( Pts[2] );
            draw_Color ( RGB[1] );
            draw_UV    ( pUV[1] );
            draw_Vertex( Pts[1] );
            #endif
            
            #ifdef TARGET_PS2
            pV[0] = Pts[0];
            pV[3] = Pts[0];
            pV[2] = Pts[2];
            pV[4] = Pts[2];
            pV[5] = Pts[1];
            pV[1] = Pts[3];
            
            pT[0] = pUV[0];
            pT[3] = pUV[0];
            pT[2] = pUV[2];
            pT[4] = pUV[2];
            pT[5] = pUV[1];
            pT[1] = pUV[3];
            
            pC[0] = RGB[0];
            pC[3] = RGB[0];
            pC[2] = RGB[2];
            pC[4] = RGB[2];
            pC[5] = RGB[1];
            pC[1] = RGB[3];

            pV += 6;
            pT += 6;
            pC += 6;
            #endif
            
            NumQuads++;
        }
        
        Shared[ NumX] += VY;
    }

    #ifdef TARGET_PS2
    poly_Render( pVtx, pTex, pCol, NumQuads * 6 );
    #endif
    
    return( NumQuads );
}

//=============================================================================

void GetBBoxFaces( const bbox& BBox, vector3* pVert )
{
    // Compute edge vectors
    vector3 X( BBox.Max.X - BBox.Min.X, 0.0f, 0.0f );
    vector3 Y( 0.0f, BBox.Max.Y - BBox.Min.Y, 0.0f );
    vector3 Corner[8];

    // Find the corner vertices of bounding box
    Corner[0] = BBox.Min;
    Corner[1] = Corner[0] + X;
    Corner[2] = Corner[1] + Y;
    Corner[3] = Corner[0] + Y;
    Corner[4] = BBox.Max;
    Corner[5] = Corner[4] - Y;
    Corner[6] = Corner[5] - X;
    Corner[7] = Corner[4] - X;
    
    // Setup corner vertices for all 6 faces of box
    pVert[ 0] = Corner[0];
    pVert[ 1] = Corner[1];
    pVert[ 2] = Corner[2];
    pVert[ 3] = Corner[3];

    pVert[ 4] = Corner[4];
    pVert[ 5] = Corner[5];
    pVert[ 6] = Corner[6];
    pVert[ 7] = Corner[7];

    pVert[ 8] = Corner[1];
    pVert[ 9] = Corner[2];
    pVert[10] = Corner[4];
    pVert[11] = Corner[5];

    pVert[12] = Corner[0];
    pVert[13] = Corner[3];
    pVert[14] = Corner[7];
    pVert[15] = Corner[6];

    pVert[16] = Corner[2];
    pVert[17] = Corner[3];
    pVert[18] = Corner[7];
    pVert[19] = Corner[4];

    pVert[20] = Corner[0];
    pVert[21] = Corner[1];
    pVert[22] = Corner[5];
    pVert[23] = Corner[6];
}

//=============================================================================

void bounds_Render( const bbox& a_BBox )
{
    vector3 Face[6][4];

    // Increase rendered height of game bounds so we cant get above it
    bbox BBox = a_BBox;
    BBox.Max.Y += BoundsMaxQuadDist * BoundsDistMul;

    GetBBoxFaces( BBox, Face[0] );

#ifdef BOUNDS_TIMER
    xtimer Time;
    Time.Reset();
    Time.Start();
#endif

    #ifdef TARGET_PC
    draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
    draw_ClearL2W();
    draw_SetTexture( s_Bitmap );
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    #endif

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE( C_SRC, C_ZERO, A_SRC, C_DST ) );
    gsreg_SetZBufferUpdate( FALSE );
    gsreg_SetClamping( FALSE );
    gsreg_End();
    poly_Begin( POLY_USE_ALPHA );
    vram_Activate( s_Bitmap );
    vram_ActivateClut( vram_GetClutHandle( s_Bitmap ) );
    #endif

    NumQuadsRendered = 0;
    NumBoxFaces      = 0;

    vector2 UVS[4];

    UVS[0] = vector2( -BoundsTile, -BoundsTile );
    UVS[1] = vector2(  BoundsTile, -BoundsTile );
    UVS[2] = vector2(  BoundsTile,  BoundsTile );
    UVS[3] = vector2( -BoundsTile,  BoundsTile );

    NumQuadsRendered += RenderTiledQuad( Face[0], UVS, BoundsCol, BoundsMaxQuadDist, BoundsMaxEdgeLength );
    NumQuadsRendered += RenderTiledQuad( Face[1], UVS, BoundsCol, BoundsMaxQuadDist, BoundsMaxEdgeLength );
    NumQuadsRendered += RenderTiledQuad( Face[2], UVS, BoundsCol, BoundsMaxQuadDist, BoundsMaxEdgeLength );
    NumQuadsRendered += RenderTiledQuad( Face[3], UVS, BoundsCol, BoundsMaxQuadDist, BoundsMaxEdgeLength );

    #ifdef TARGET_PC
    draw_End();
    #endif

    #ifdef TARGET_PS2
    poly_End();
    gsreg_Begin();
    gsreg_SetZBufferUpdate( TRUE );
    gsreg_End();
    #endif
    
#ifdef BOUNDS_TIMER
    Time.Stop();
#endif

    if( DebugBounds == TRUE )
    {
#ifdef BOUNDS_TIMER
        x_printfxy( 2, 18, "Time  = %f\n", Time.ReadMs()    );
#endif
        x_printfxy( 2, 19, "Quads = %d\n", NumQuadsRendered );
        x_printfxy( 2, 20, "Faces = %d\n", NumBoxFaces      );
    }
}

//=============================================================================

void bounds_Update( f32 DeltaTime )
{
    BoundsTime += DeltaTime;

    f32 T = 1.0f;

    if( PulseBounds == TRUE )
        T = BoundsPulseLum + (x_sin( 2.0f * PI * x_fmod( BoundsTime * BoundsPulseMul, 1.0f ) ) * BoundsPulseLum);

    xcolor C( BoundsColor );

    BoundsCol.R = (u8)(C.R * T);
    BoundsCol.G = (u8)(C.G * T);
    BoundsCol.B = (u8)(C.B * T);
}

//=============================================================================

