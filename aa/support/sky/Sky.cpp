//=========================================================================
//
// SKY.CPP
//
//=========================================================================

#include "Entropy.hpp"
#include "Sky.hpp"
#include "aux_Bitmap.hpp"
#include "poly\poly.hpp"

//=========================================================================

sky::sky( void )
{
    m_Yaw = 0.0f;
    m_Fog = NULL;
}

//=========================================================================

sky::~sky( void )
{
}

//=========================================================================

static void LoadTexture( xbitmap& BMP, char* pFileName )
{
    xbool Result = auxbmp_LoadNative( BMP, pFileName );
    if( !Result )
    {
        //x_DebugMsg("Using default bitmap for %1s\n",pFileName);
    }
    
    vram_Register( BMP );
}

//=========================================================================

void sky::Kill( void )
{
    for( s32 i=0; i<5; i++ )
    {
        vram_Unregister( m_Material[i] );
        m_Material[i].Kill();
    }
}

//=========================================================================

void sky::Load( token_stream& TOK )
{
    // Loop through and get textures
    for( s32 i=0; i<5; i++ )
    {
        TOK.ReadSymbol();
        //x_DebugMsg( "Loading sky texture %s\n",TOK.String());
        LoadTexture( m_Material[i], TOK.String() );
    }
}

//=========================================================================

void sky::SetFog( const fog* pFog )
{
    m_Fog = pFog;
}

//=========================================================================

void sky::SetYaw( radian Yaw )
{
    m_Yaw = Yaw;
}

//=========================================================================

#define NUM_SKY_VERTS 8
#define NUM_SKY_TILES 5

static vector3 BoxVertex[NUM_SKY_VERTS] = 
{
    vector3( -100, -20, -100 ), // 0
    vector3( -100, -20, +100 ), // 1
    vector3( -100, +75, -100 ), // 2
    vector3( -100, +75, +100 ), // 3
    vector3( +100, -20, -100 ), // 4
    vector3( +100, -20, +100 ), // 5
    vector3( +100, +75, -100 ), // 6
    vector3( +100, +75, +100 ), // 7
};

static s16 BoxI[NUM_SKY_TILES*4] = 
{
    6,7,3,2, // Top
    7,5,1,3, // Front
    6,4,5,7, // Left
    2,0,4,6, // Back
    3,1,0,2, // Right
};

//=========================================================================

#ifdef TARGET_PS2

static vector2 s_Sky_Tex[6] PS2_ALIGNMENT(16) =
{
    vector2( 0, 0 ),
    vector2( 0, 1 ),
    vector2( 1, 1 ),
    vector2( 1, 1 ),
    vector2( 1, 0 ),
    vector2( 0, 0 )
};

static ps2color s_Sky_Col[6] PS2_ALIGNMENT(16) =
{
    ps2color( 128, 128, 128 ),
    ps2color( 128, 128, 128 ),
    ps2color( 128, 128, 128 ),
    ps2color( 128, 128, 128 ),
    ps2color( 128, 128, 128 ),
    ps2color( 128, 128, 128 )
};

#endif

//=========================================================================

volatile xbool RENDER_SKY     = TRUE;
volatile xbool RENDER_SKYBOX  = TRUE;
volatile xbool RENDER_FOGDOME = TRUE;

void sky::Render( void )
{
    if( RENDER_SKY == FALSE )
        return;

    s32 i;
    const view* pView = eng_GetView(0);
    vector3 BoxV[NUM_SKY_VERTS];
    u32 ClipMask[NUM_SKY_VERTS];

    SeedZBuffer();

    m_EyePos = pView->GetPosition();

    // Yaw box verts
    {
        matrix4 M;
        M.Identity();
        M.RotateY( m_Yaw );
        M.Transform( BoxV, BoxVertex, NUM_SKY_VERTS );
    }

    // Get clipping planes
    const plane* P = pView->GetViewPlanes();
    for( i=0; i<NUM_SKY_VERTS; i++ )
    {
        vector3* BV = &BoxV[i];
        ClipMask[i] = 0;
        if( (BV->X*P[0].Normal.X + BV->Y*P[0].Normal.Y + BV->Z*P[0].Normal.Z) < 0 ) ClipMask[i] |= (1<<0);
        if( (BV->X*P[1].Normal.X + BV->Y*P[1].Normal.Y + BV->Z*P[1].Normal.Z) < 0 ) ClipMask[i] |= (1<<1);
        if( (BV->X*P[2].Normal.X + BV->Y*P[2].Normal.Y + BV->Z*P[2].Normal.Z) < 0 ) ClipMask[i] |= (1<<2);
        if( (BV->X*P[3].Normal.X + BV->Y*P[3].Normal.Y + BV->Z*P[3].Normal.Z) < 0 ) ClipMask[i] |= (1<<3);
        if( (BV->X*P[4].Normal.X + BV->Y*P[4].Normal.Y + BV->Z*P[4].Normal.Z) < 0 ) ClipMask[i] |= (1<<4);
    }

    #ifdef TARGET_PC
        // Setup texture pixel blending
	    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
	    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
	    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR  );
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
    #endif

    if( RENDER_SKYBOX == TRUE )
    {
    poly_Begin();

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetClamping(TRUE);
    gsreg_SetZBuffer(FALSE);
    gsreg_End();
    vram_SetMipEquation(0,TRUE);
    #endif

    s32 NSides=0;

    // Loop through materials
    for( i=0; i<NUM_SKY_TILES; i++ )
    {
        // Get indices to 
        s16* pI = &BoxI[i*4];

        // Decide if view can see tile
        if( ClipMask[pI[0]] & ClipMask[pI[1]] & ClipMask[pI[2]] & ClipMask[pI[3]] )
            continue;

        // Set texture and render tile
        NSides++;
        
        #ifdef TARGET_PS2

        vector3* pts = (vector3*)smem_BufferAlloc( sizeof(vector3) * 6 );
        if( !pts )
            break;
        
        pts[0] = BoxV[pI[0]] + m_EyePos;
        pts[1] = BoxV[pI[1]] + m_EyePos;
        pts[2] = BoxV[pI[2]] + m_EyePos;
        pts[3] = BoxV[pI[2]] + m_EyePos;
        pts[4] = BoxV[pI[3]] + m_EyePos;
        pts[5] = BoxV[pI[0]] + m_EyePos;
    
        vram_Activate( m_Material[i] );
        
        poly_Render( pts, s_Sky_Tex, s_Sky_Col, 6 );
        
        #else
        
        draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED|DRAW_NO_ZBUFFER );
            draw_SetTexture( m_Material[i] );
            draw_Color(1,1,1,1);
            draw_UV(0,0); draw_Vertex( BoxV[pI[0]] + m_EyePos );
            draw_UV(0,1); draw_Vertex( BoxV[pI[1]] + m_EyePos );
            draw_UV(1,1); draw_Vertex( BoxV[pI[2]] + m_EyePos );
            draw_UV(1,1); draw_Vertex( BoxV[pI[2]] + m_EyePos );
            draw_UV(1,0); draw_Vertex( BoxV[pI[3]] + m_EyePos );
            draw_UV(0,0); draw_Vertex( BoxV[pI[0]] + m_EyePos );
        draw_End();
        
        #endif
    }

    poly_End();
    }

    // Render fog dome
    if( RENDER_FOGDOME )
        RenderFogDome();

    #ifdef TARGET_PC
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );
    #endif

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetClamping(FALSE);
    gsreg_SetZBuffer(TRUE);
    gsreg_End();
    vram_SetMipEquation(-16,FALSE);
    #endif
}

//=========================================================================
void sky::RenderFogDome( void )
{
    s32 i;
    f32 Radius      = 500.0f;

    // Read fog sphere info
    m_Fog->GetHazeAngles( m_HazeStartAngle, m_HazeFinishAngle );

    // Clear fog sphere values
    f32 TopAlpha     = 0;
    f32 TopBandAlpha = 0;
    f32 BotBandAlpha = 1;
    f32 BotAlpha     = 1;
    f32 TopBandAngle = 0;
    f32 BotBandAngle = 0;

    // Deal with fog volumes

    if( m_Fog->GetNVolumes() > 0 )
    {
        const fog::volume& Volume = m_Fog->GetVolume(0);
        f32 DepthInFog = Volume.MaxY - m_EyePos.Y;

        if( DepthInFog > 0 )
        {
            f32 VisDist = Volume.VisDistance;

            //x_printfxy(0,L++,"Inside fog %f %f\n",DepthInFog,VisDist);

            // Are we completely submerged
            if( DepthInFog >= VisDist )
            {
                //x_printfxy(0,L++,"No visible sky");
                TopAlpha = 1;
                BotAlpha = 1;
                TopBandAlpha = 1;
                BotBandAlpha = 1;
                BotBandAngle = 0;
                TopBandAngle = R_90;
            }
            else
            {
                f32 Horiz;
                //x_printfxy(0,L++,"Partial fogging");

                // Compute top alpha
                TopAlpha = (DepthInFog / VisDist);
                //x_printfxy(0,L++,"TopAlpha %f %f",TopAlpha,(DepthInFog / VisDist));

                // Compute angle of complete fogging
                Horiz = x_sqrt( VisDist*VisDist - DepthInFog*DepthInFog );
                BotBandAngle = x_atan2( DepthInFog, Horiz );
                BotBandAlpha = 1;
                //x_printfxy(0,L++,"BotBandAlpha %f",BotBandAlpha);

                // Compute angle where fog is midway
                f32 DesiredAlpha = BotBandAlpha + 0.9f*(TopAlpha-BotBandAlpha);
                f32 Dist = VisDist*DesiredAlpha;

                ASSERT( Dist>=DepthInFog );
                Horiz = x_sqrt( Dist*Dist - DepthInFog*DepthInFog );
                TopBandAngle = x_atan2( DepthInFog, Horiz );
                TopBandAlpha = DesiredAlpha;
                //x_printfxy(0,L++,"TopBandAlpha %f",TopBandAlpha);
            }
        }
    }


    // Peg angles 
    if( BotBandAngle < m_HazeFinishAngle )
    {
        BotBandAngle = m_HazeFinishAngle;
        BotBandAlpha = 1;
    }

    if( TopBandAngle < m_HazeStartAngle )
    {
        TopBandAngle = m_HazeStartAngle;
        //TopBandAlpha = 0;
    }

    // Setup alpha for colors
    TopAlpha     *= 255;
    TopBandAlpha *= 255;
    BotBandAlpha *= 255;
    BotAlpha     *= 255;

    // Build top and bottom verts of sphere
    vector3 TopPos    = m_EyePos;
    vector3 BotPos    = m_EyePos;
    TopPos.Y += Radius;
    BotPos.Y -= Radius*0.25f;

    // Compute radius and height at top and bottom bands
    f32 TopBandRadius = Radius * x_cos( TopBandAngle );
    f32 TopBandY      = Radius * x_sin( TopBandAngle );
    f32 BotBandRadius = Radius * x_cos( BotBandAngle );
    f32 BotBandY      = Radius * x_sin( BotBandAngle );

#define NUM_SLICES 20

    //f32 ZMul[8] = {1,0.707f,0,-0.707f,-1,-0.707f,0,0.707f};
    //f32 XMul[8] = {0,0.707f,1,0.707f,0,-0.707f,-1,-0.707f};
    vector3 TopBandPos[NUM_SLICES];
    vector3 BotBandPos[NUM_SLICES];

    // Build the NUM_SLICES verts
    for( i=0; i<NUM_SLICES; i++ )
    {
        f32 c,s;
        x_sincos( R_360*i/(f32)(NUM_SLICES-1), s, c );

        TopBandPos[i].X = m_EyePos.X + s*TopBandRadius;
        TopBandPos[i].Y = m_EyePos.Y + TopBandY;
        TopBandPos[i].Z = m_EyePos.Z + c*TopBandRadius;

        BotBandPos[i].X = m_EyePos.X + s*BotBandRadius;
        BotBandPos[i].Y = m_EyePos.Y + BotBandY;
        BotBandPos[i].Z = m_EyePos.Z + c*BotBandRadius;
    }

#ifdef TARGET_PC
    draw_Begin( DRAW_TRIANGLES, DRAW_USE_ALPHA|DRAW_NO_ZBUFFER );
    {
        xcolor TC,TBC,BC,BBC;
        TC = TBC = BC = BBC = m_Fog->GetHazeColor();
        TC.A  = (s32)TopAlpha;
        TBC.A = (s32)TopBandAlpha;
        BC.A  = (s32)BotAlpha;
        BBC.A = (s32)BotBandAlpha;

        for( i=0; i<NUM_SLICES; i++ )
        {
            s32 n = (i+1)%NUM_SLICES;

            // Render top fan
            draw_Color(TC);     draw_Vertex(TopPos);
            draw_Color(TBC);    draw_Vertex(TopBandPos[i]);
                                draw_Vertex(TopBandPos[n]);

            // Render bot fan
            draw_Color(BC);     draw_Vertex(BotPos);
            draw_Color(BBC);    draw_Vertex(BotBandPos[i]);
                                draw_Vertex(BotBandPos[n]);

            // Render band
            draw_Color(TBC);    draw_Vertex(TopBandPos[i]);
                                draw_Vertex(TopBandPos[n]);
            draw_Color(BBC);    draw_Vertex(BotBandPos[i]);

            draw_Color(TBC);    draw_Vertex(TopBandPos[n]);
            draw_Color(BBC);    draw_Vertex(BotBandPos[n]);
                                draw_Vertex(BotBandPos[i]);
        }
    }
    draw_End();
#endif

#ifdef TARGET_PS2
    poly_Begin( POLY_USE_ALPHA | POLY_NO_TEXTURE );

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend(ALPHA_BLEND_INTERP);
    gsreg_SetZBuffer(FALSE);
    gsreg_End();
    #endif

    {
        ps2color TC,TBC,BC,BBC;
        xcolor HC = m_Fog->GetHazeColor();
        BBC.Set(HC.R,HC.G,HC.B,HC.A);
        TC = TBC = BC = BBC;
        
        // Map 0->255 to 0->128 for correct PS2 blender equation!
        TC.A  = (s32)(TopAlpha     * (128.0f / 255.0f)) ;
        TBC.A = (s32)(TopBandAlpha * (128.0f / 255.0f)) ;
        BC.A  = (s32)(BotAlpha     * (128.0f / 255.0f)) ;
        BBC.A = (s32)(BotBandAlpha * (128.0f / 255.0f)) ;

        //TC.A    >>=1;
        //TBC.A   >>=1;
        //BC.A    >>=1;
        //BBC.A   >>=1;

        s32 NVerts = NUM_SLICES * 12;
        vector3* pV = (vector3*)smem_BufferAlloc(sizeof(vector3)*NVerts);
        ps2color* pC = (ps2color*)smem_BufferAlloc(sizeof(ps2color)*NVerts);

        if (pV && pC)
        {
            s32 j=0;
            for( i=0; i<NUM_SLICES; i++ )
            {
                s32 n = i+1;
                if( n==NUM_SLICES ) n = 0;

                pC[j] = TC;   pV[j++] = TopPos;
                pC[j] = TBC;  pV[j++] = TopBandPos[i];
                pC[j] = TBC;  pV[j++] = TopBandPos[n];

                pC[j] = BC;   pV[j++] = BotPos;
                pC[j] = BBC;  pV[j++] = BotBandPos[i];
                pC[j] = BBC;  pV[j++] = BotBandPos[n];

                pC[j] = TBC;  pV[j++] = TopBandPos[i];
                pC[j] = TBC;  pV[j++] = TopBandPos[n];
                pC[j] = BBC;  pV[j++] = BotBandPos[i];

                pC[j] = TBC;  pV[j++] = TopBandPos[n];
                pC[j] = BBC;  pV[j++] = BotBandPos[n];
                pC[j] = BBC;  pV[j++] = BotBandPos[i];
            }

            poly_Render( pV, (vector2*)pV, pC, NVerts );
        }
    }

    poly_End();

    #ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend(ALPHA_BLEND_OFF);
    gsreg_SetZBuffer(TRUE);
    gsreg_End();
    #endif
#endif
}

//=========================================================================

f32 ZSEED_VALUE = 0;

void sky::SeedZBuffer( void )
{
    const view* View = eng_GetActiveView(0);

    f32     NearZ,FarZ;
    vector3 EyePos;
    vector3 ViewDir;
    rect    Viewport;
    vector3 Corner[4];

    View->GetZLimits( NearZ, FarZ );
    EyePos  = View->GetPosition();
    ViewDir = View->GetViewZ();
    View->GetViewport( Viewport );

    Corner[0] = View->RayFromScreen( Viewport.Min.X, Viewport.Min.Y );
    Corner[1] = View->RayFromScreen( Viewport.Min.X, Viewport.Max.Y );
    Corner[2] = View->RayFromScreen( Viewport.Max.X, Viewport.Max.Y );
    Corner[3] = View->RayFromScreen( Viewport.Max.X, Viewport.Min.Y );

    f32 ZDist = m_Fog->GetVisDistance() - ZSEED_VALUE;
    if( ZDist > FarZ )
        ZDist = FarZ;
    if( ZDist < NearZ )
        ZDist = NearZ;

    for( s32 i=0; i<4; i++ )
    {
        f32 D = ViewDir.Dot( Corner[i] );
        Corner[i] = EyePos + Corner[i]*(1.0f/D) * (ZDist);
    }
#ifdef TARGET_PC
    draw_Begin( DRAW_TRIANGLES );
    draw_Color( XCOLOR_RED );
    draw_Vertex( Corner[0] );
    draw_Vertex( Corner[1] );
    draw_Vertex( Corner[2] );
    draw_Vertex( Corner[0] );
    draw_Vertex( Corner[2] );
    draw_Vertex( Corner[3] );
    draw_End();
#endif

#ifdef TARGET_PS2
    vector3* pV = (vector3*)smem_BufferAlloc(sizeof(vector3)*6);
    ps2color* pC = (ps2color*)smem_BufferAlloc(sizeof(ps2color)*6);
    
    if (pV && pC)
    {
        ps2color C(255,0,0,128);
        poly_Begin( POLY_NO_TEXTURE );
        gsreg_Begin();
        gsreg_SetFBMASK( 0xFFFFFFFF );
        gsreg_End();

        pV[0] = Corner[0];
        pV[1] = Corner[1];
        pV[2] = Corner[2];
        pV[3] = Corner[0];
        pV[4] = Corner[2];
        pV[5] = Corner[3];
        pC[0] = C;
        pC[1] = C;
        pC[2] = C;
        pC[3] = C;
        pC[4] = C;
        pC[5] = C;
        poly_Render( pV, (vector2*)pV, pC, 6 );
        gsreg_Begin();
        gsreg_SetFBMASK( 0x00000000 );
        gsreg_End();
        poly_End();
    }
#endif
}

//=========================================================================
