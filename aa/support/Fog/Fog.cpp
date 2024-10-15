//=========================================================================
//
//  Fog.cpp
//
//=========================================================================

#include "Entropy.hpp"
#include "Fog.hpp"
#include "Demo1\Globals.hpp"

//=========================================================================

#define FOG_WIDTH   64
#define FOG_HEIGHT  64

//#define FOG_USE_BILINEAR      // Define to use bilinear fog color lookups (3 times slower)



//=========================================================================

struct debug_fog
{
    xbool   ShowFogMap;
    xbool   FlashBack;
    s32     ScaleFogMap;
    s32     SampleX, SampleY;
    xcolor  SampleColor;

    xbool   UseBilinear ;
    f32     Time  ;
    s32     Frame ;
};

static debug_fog g_DebugFog;

//=========================================================================

fog::fog( void )
{
    m_Initialized = FALSE;
    m_WorldMaxY   = 400.0f;
    m_WorldMinY   =   0.0f;
    g_DebugFog.ShowFogMap = FALSE;
    g_DebugFog.FlashBack  = FALSE;
    g_DebugFog.ScaleFogMap= 2;
    g_DebugFog.SampleX    = 64;
    g_DebugFog.SampleY    = 32;
    g_DebugFog.SampleColor= XCOLOR_WHITE;

    g_DebugFog.UseBilinear = TRUE ;
    g_DebugFog.Time = 0 ;
    g_DebugFog.Frame = -1 ;
}

//=========================================================================

fog::~fog( void )
{
    if( m_Initialized )
        Unload();
}

//=========================================================================

void fog::SaveSettings( const char* pFileName )
{
    X_FILE* fp;

    fp = x_fopen(pFileName,"wt");

    if( !fp )
    {
        x_printf("COULD NOT SAVE FOG SETTINGS\n");
        return;
    }

    x_fprintf(fp,"128 32 // Fog texture size\n");
    x_fprintf(fp,"%3d %3d %3d       // Fog Color\n",(s32)m_FogColor.R,(s32)m_FogColor.G,(s32)m_FogColor.B);
    x_fprintf(fp,"%3d %3d %3d       // Fog Color\n",(s32)m_FogColor.R,(s32)m_FogColor.G,(s32)m_FogColor.B);
    x_fprintf(fp,"%f  %f            // Start/Finish Distances\n",m_HazeStartDist,m_HazeFinishDist);
    x_fprintf(fp,"%f  %f            // Haze Start/Finish Angles\n",RAD_TO_DEG(m_HazeStartAngle),RAD_TO_DEG(m_HazeFinishAngle));
//  x_fprintf(fp,"%4.1f %4.1f         // Min/Max Band Alpha\n",m_MinBandAlpha,m_MaxBandAlpha);

    x_fprintf(fp,"%3d               // Number of fog volumes\n",m_NVolumes);
    for( s32 i=0; i<m_NVolumes; i++ )
    {
        x_fprintf(fp,"%f  %f  %f        // Volume %1d Min/Max Height, VisDist\n",m_Volume[i].MinY,m_Volume[i].MaxY,m_Volume[i].VisDistance);
    }

    x_fclose(fp);
}

//=========================================================================

void fog::LoadSettings( const char* pFileName )
{
    token_stream TOK;
    xbool status;
    status =  TOK.OpenFile( pFileName );

    if (!status) 
    {
        x_printf("Could not load fog settings\n");
        return;
    }

    Load( TOK );

    TOK.CloseFile();
}

//=========================================================================

void fog::Load( token_stream& TOK )
{
    s32    i ;
    xcolor Dummy;

    // Get through fog texture size
    TOK.ReadInt();
    TOK.ReadInt();

    // Get fog color
    m_FogColor.R = (s32)TOK.ReadFloat();
    m_FogColor.G = (s32)TOK.ReadFloat();
    m_FogColor.B = (s32)TOK.ReadFloat();
    m_FogColor.A = 255;

    // Get haze color
    Dummy.R = (s32)TOK.ReadFloat();
    Dummy.G = (s32)TOK.ReadFloat();
    Dummy.B = (s32)TOK.ReadFloat();

    // Get haze distances
    m_HazeStartDist  = TOK.ReadFloat();
    m_HazeFinishDist = TOK.ReadFloat();

    m_HazeStartAngle  = DEG_TO_RAD( TOK.ReadFloat() );
    m_HazeFinishAngle = DEG_TO_RAD( TOK.ReadFloat() );

    // Get band alpha limits
    m_MinBandAlpha = TOK.ReadFloat();
    m_MaxBandAlpha = TOK.ReadFloat();

    // Get number of fog bands
    m_NVolumes = TOK.ReadInt();

    // Read band parameters
    for( i = 0; i < m_NVolumes; i++ )
    {
        m_Volume[i].MinY = TOK.ReadFloat();
        m_Volume[i].MaxY = TOK.ReadFloat();
        m_Volume[i].VisDistance = TOK.ReadFloat();
        m_Volume[i].VisFactor   = 1.0f / m_Volume[i].VisDistance;
    }

    // Setup volumes
    for( i=m_NVolumes; i<MAX_FOG_VOLUMES; i++ )
    {
        m_Volume[i].MaxY = 0.0f;
        m_Volume[i].MinY = 0.0f;
        m_Volume[i].VisDistance = 0.0f;
        m_Volume[i].VisFactor   = 1.0f;
    }  

    if( m_NVolumes==0 )
    {
        m_MinVolumeY =   0.0f;
        m_MaxVolumeY = 300.0f;
    }
    else
    {
        m_MinVolumeY =  F32_MAX;
        m_MaxVolumeY = -F32_MAX;
        for( i=0; i<m_NVolumes; i++ )
        {
            m_MinVolumeY = MIN( m_MinVolumeY, m_Volume[i].MinY );
            m_MaxVolumeY = MAX( m_MaxVolumeY, m_Volume[i].MaxY );
        }
    }

    //
    // Setup fog map
    //

    if( !m_Initialized )
    {
        m_UVWidth  = FOG_WIDTH;
        m_UVHeight = FOG_HEIGHT;

        for( i=0; i<4; i++ )
        {

            #ifdef TARGET_PS2

                byte* pPixels = (byte*)x_malloc( m_UVWidth * m_UVHeight);
                ASSERT(pPixels) ;

                byte* pClut = (byte*)x_malloc(256*4) ;
                ASSERT(pClut) ;

                m_BMP[i].Setup( xbitmap::FMT_P8_ABGR_8888,
                                m_UVWidth,
                                m_UVHeight,
                                TRUE,
                                pPixels,
                                TRUE,
                                pClut ) ;

                // Setup the clut
                for (s32 j = 0 ; j < 256 ; j++)
                {
                    xcolor C = m_FogColor ;
                    C.A = j >> 1 ;
                    m_BMP[i].SetClutColor(C, j) ;
                }

                // Don't forget the olde ps2 palette swizzle!
                m_BMP[i].PS2SwizzleClut() ;

            #endif

            #ifdef TARGET_PC
            
                byte* pPixels = (byte*)x_malloc( m_UVWidth * m_UVHeight * 4 );
                ASSERT( pPixels  );

                m_BMP[i].Setup( xbitmap::FMT_32_ARGB_8888,
                                m_UVWidth,
                                m_UVHeight,
                                TRUE,
                                pPixels);
            #endif

            vram_Register( m_BMP[i] );
        }
        m_BMPIndex = 0;
    }

    m_Initialized = TRUE;
}

//=========================================================================

//
//  this is here only for my building viewer!
//

void fog::Load( const char* pFileName )
{
    ASSERT( FALSE );
    (void)pFileName;
/*
    s32    i;
    xcolor Dummy;

    token_stream TOK;

    // Load in file with fog info
    TOK.OpenFile( pFileName );

    // Get texture dimensions
    VERIFY(TOK.Find("FogMapSize",TRUE));
    m_UVWidth  = TOK.ReadInt();
    m_UVHeight = TOK.ReadInt();

    // Get colors
    VERIFY(TOK.Find("FogColor",TRUE));
    m_FogColor.R = (s32)TOK.ReadFloat();
    m_FogColor.G = (s32)TOK.ReadFloat();
    m_FogColor.B = (s32)TOK.ReadFloat();
    m_FogColor.A = 255;
    VERIFY(TOK.Find("HazeColor",TRUE));
    Dummy.R = (s32)TOK.ReadFloat();
    Dummy.G = (s32)TOK.ReadFloat();
    Dummy.B = (s32)TOK.ReadFloat();
    Dummy.A = 255;

    // Get haze distances
    VERIFY(TOK.Find("FogHazeStartDist",TRUE));
    m_HazeStartDist = TOK.ReadFloat();
    VERIFY(TOK.Find("FogHazeFinishDist",TRUE));
    m_HazeFinishDist = TOK.ReadFloat();

    // Get band alpha limits
    VERIFY(TOK.Find("FogMaxBandAlpha",TRUE));
    m_MaxBandAlpha = TOK.ReadFloat();
    VERIFY(TOK.Find("FogMinBandAlpha",TRUE));
    m_MinBandAlpha = TOK.ReadFloat();

    // Setup volumes
    for( i=0; i<MAX_FOG_VOLUMES; i++ )
    {
        m_Volume[i].MaxY = 0;
        m_Volume[i].MinY = 0;
        m_Volume[i].VisDistance = 0;
    }

    // Read fog volumes
    for( i=0; i<MAX_FOG_VOLUMES; i++ )
    {
        if( TOK.Find(xfs("FogBand%1d",i),TRUE) )
        {
            m_Volume[i].MaxY = TOK.ReadFloat();
            m_Volume[i].MinY = TOK.ReadFloat();
            m_Volume[i].VisDistance = TOK.ReadFloat();
        }
    }

    // Close fog info
    TOK.CloseFile();

    // Get min and max volume Y

    // Determine number of volumes of fog
    m_NVolumes   =  0;
    m_MinVolumeY =  F32_MAX;
    m_MaxVolumeY = -F32_MAX;
    for( i=0; i<MAX_FOG_VOLUMES; i++ )
    {
        if( m_Volume[i].VisDistance > 0 )
        {
            m_MinVolumeY = MIN( m_MinVolumeY, m_Volume[i].MinY );
            m_MaxVolumeY = MAX( m_MaxVolumeY, m_Volume[i].MaxY );
            m_NVolumes++;
        }
    }

    if( m_NVolumes == 0 )
    {
        m_MinVolumeY =   0.0f;
        m_MaxVolumeY = 300.0f;
    }

    //
    // Setup fog map
    //
    m_UVWidth  = FOG_WIDTH;
    m_UVHeight = FOG_HEIGHT;

    // Set up the shared palette.
    u32* pPalette = (u32*)x_malloc( 256 * 4 );
    for( i = 0; i < 256; i++ )
    {
        #ifdef TARGET_PS2   // format is ABGR_8888
        pPalette[i] = (i            << 24) || 
                      (m_FogColor.B << 16) ||
                      (m_FogColor.G <<  8) ||
                      (m_FogColor.R <<  0);
        #endif

        #ifdef TARGET_PC    // format is ARGB_8888
        pPalette[i] = (i            << 24) || 
                      (m_FogColor.R << 16) ||
                      (m_FogColor.G <<  8) ||
                      (m_FogColor.B <<  0);
        #endif
    }

    // Setup the 4 bitmaps.  They are 8 bit paletted.
    for( i=0; i<4; i++ )
    {
        byte* pData = (byte*)x_malloc( m_UVWidth * m_UVHeight );
        ASSERT( pData );

        #ifdef TARGET_PS2
        m_BMP[i].Setup( xbitmap::FMT_P8_ABGR_8888,
                        m_UVWidth,
                        m_UVHeight,
                        TRUE,
                        pData,
                        (i == 0),
                        (byte*)pPalette );
        #endif

        #ifdef TARGET_PC
        m_BMP[i].Setup( xbitmap::FMT_P8_ABGR_8888,
                        m_UVWidth,
                        m_UVHeight,
                        TRUE,
                        pData,
                        (i == 0),
                        (byte*)pPalette );
        #endif

        vram_Register( m_BMP[i] );
    }                   
    m_BMPIndex = 0;

    m_Initialized = TRUE;
*/    
}

//=========================================================================

void fog::Unload( void )
{
    for( s32 i=0; i<4; i++ )
    {
        vram_Unregister( m_BMP[i] );
        m_BMP[i].Kill();
    }
    m_Initialized = FALSE;
}

//=========================================================================

void fog::SetEyePos( const vector3& EyePos )
{
    if( !m_Initialized ) 
        return;

    // Advance index.
    m_BMPIndex = (m_BMPIndex+1)%4;

    // Record eye position.
    m_EyePos = EyePos;

    BuildFogMap();
}

//=========================================================================

static inline xcolor LookupFogColor( const xbitmap& BMP, f32 U, f32 V )
{
#ifdef X_DEBUG
    xcolor C ;

    xtimer Timer ;

    if (g_DebugFog.UseBilinear)
    {
        // Just use bilinear function...
        Timer.Start() ;
        C = BMP.GetBilinearColor(U,V,TRUE) ;
        Timer.Stop() ;
    }
    else
    {
        Timer.Start() ;

        // Convert to pixel co-ords
        U *= (f32)FOG_WIDTH ;
        V *= (f32)FOG_HEIGHT ;

        // Clamp U
        if (U < 0)
            U = 0 ;
        else
        if (U > (FOG_WIDTH-1))
            U = FOG_WIDTH-1 ;

        // Clamp V
        if (V < 0)
            V = 0 ;
        else
        if (V > (FOG_HEIGHT-1))
            V = FOG_HEIGHT-1 ;

        // Get color from bitmap
        C = BMP.GetPixelColor((s32)U, (s32)V) ;
        Timer.Stop() ;
    }

    g_DebugFog.Time += Timer.ReadMs() ;

    return C ;

#else   //#ifdef _X_DEBUG


#ifdef FOG_USE_BILINEAR

    // Just use bilinear function...
    return BMP.GetBilinearColor(U,V,TRUE) ;
   
#else
        
    // Convert to pixel co-ords
    U *= (f32)FOG_WIDTH ;
    V *= (f32)FOG_HEIGHT ;

    // Clamp U
    if (U < 0)
        U = 0 ;
    else
    if (U > (FOG_WIDTH-1))
        U = FOG_WIDTH-1 ;

    // Clamp V
    if (V < 0)
        V = 0 ;
    else
    if (V > (FOG_HEIGHT-1))
        V = FOG_HEIGHT-1 ;

    // Get color from bitmap
    return BMP.GetPixelColor((s32)U, (s32)V) ;

#endif  //#ifdef FOG_USE_BILINEAR


#endif  //#ifdef _X_DEBUG

}



xcolor fog::ComputeFog( f32 Dist, f32 DeltaY ) const
{
    if( !m_Initialized ) return XCOLOR_WHITE;
    f32 U, V;
    ComputeUVs( Dist, DeltaY, U, V );
    xcolor C = LookupFogColor( m_BMP[m_BMPIndex], U,V ) ;
    #ifdef TARGET_PS2
    s32    A = (s32)((C.A<<1)|(C.A&0x1));
    C.A = (u8)(A & 0x000000FF);
    #endif
    return C;
}

//=========================================================================

xcolor fog::ComputeFog( const vector3& Pos ) const
{
    if( !m_Initialized ) return XCOLOR_WHITE;
    f32 U, V;
    ComputeUVs( Pos, U, V );
    xcolor C = LookupFogColor( m_BMP[m_BMPIndex], U,V ) ;
    #ifdef TARGET_PS2
    s32    A = (s32)((C.A<<1)|(C.A&0x1));
    C.A = (u8)(A & 0x000000FF);
    #endif
    return C;
}

//=========================================================================

xcolor fog::ComputeParticleFog( const vector3& Pos ) const
{
    if( !m_Initialized ) return XCOLOR_WHITE;
    f32 U,V;
    f32 Dist;
    f32 Haze;

    Dist = (Pos - m_EyePos).Length();
    U = Dist  * m_UVDistConst0   + m_UVDistConst1;
    V = Pos.Y * m_UVDeltaYConst0 + m_UVDeltaYConst1;

    xcolor C = LookupFogColor( m_BMP[m_BMPIndex], U,V ) ;

    s32 A = (s32)((C.A<<1)|(C.A&0x1)) * 224 / 256;

    Haze = (Dist - m_HazeStartDist) / (m_HazeFinishDist - m_HazeStartDist);
    Haze = MAX(0,Haze);
    Haze = MIN(1,Haze);
    s32 Ahaze = (s32)(Haze * 256);

    A = MAX( A, Ahaze );
    C.A = (u8)(A & 0x000000FF);

    return C;
}

//=========================================================================

void fog::ComputeUVs( f32 Dist, f32 DeltaY, f32& U, f32& V ) const
{
    //U = (Dist - m_UVMinDist) / (m_UVMaxDist - m_UVMinDist);
    //V = (m_WorldMaxY - DeltaY) / (m_WorldMaxY - m_WorldMinY);

    U = Dist   * m_UVDistConst0   + m_UVDistConst1;
    V = DeltaY * m_UVDeltaYConst0 + m_UVDeltaYConst1;
}

//=========================================================================

void fog::ComputeUVs( const vector3& Pos, f32& U, f32& V ) const
{
    // DMT - Something is fishy here with Delta...

    vector3 Delta = Pos; // - m_EyePos;
    f32     Dist  = (Pos - m_EyePos).Length();

    //U = (Dist - m_UVMinDist) / (m_UVMaxDist - m_UVMinDist);
    //V = (m_WorldMaxY - Delta.Y) / (m_WorldMaxY - m_WorldMinY);

    U = Dist    * m_UVDistConst0   + m_UVDistConst1;
    V = Delta.Y * m_UVDeltaYConst0 + m_UVDeltaYConst1;
}

//=========================================================================

void fog::SetMinMaxY( f32 MinY, f32 MaxY )
{
    m_WorldMaxY  = MaxY + 5.0f;
    m_WorldMinY  = MinY - 5.0f;
}

//=========================================================================

void fog::GetFogConst( f32& Dist0, f32& Dist1, f32& DeltaY0, f32& DeltaY1 ) const
{
    DeltaY0 = m_UVDeltaYConst0;
    DeltaY1 = m_UVDeltaYConst1;
    Dist0   = m_UVDistConst0;
    Dist1   = m_UVDistConst1;
}

//=========================================================================

f32 fog::GetVisDistance( void ) const
{   
    return m_HazeFinishDist;
}

//=========================================================================

const xbitmap& fog::GetBMP( void ) const
{
    return m_BMP[m_BMPIndex];
}

//=========================================================================

xcolor  fog::GetHazeColor( void ) const
{
    return m_FogColor;
}

//=========================================================================

s32 fog::GetNVolumes( void ) const
{
    return m_NVolumes;
}

//=========================================================================

const fog::volume& fog::GetVolume( s32 Index ) const
{
    ASSERT( (Index>=0) && (Index<m_NVolumes) );
    return m_Volume[Index];
}

//=========================================================================

void fog::GetHazeAngles( radian& StartAngle, radian& FinishAngle ) const
{
    StartAngle  = m_HazeStartAngle;
    FinishAngle = m_HazeFinishAngle;
}

//=========================================================================
/*
void fog::BuildFogMap( void )
{
    if( !m_Initialized ) return;

    xtimer Timer;
    Timer.Start();

    #ifdef TARGET_PC
    vram_Unregister(m_BMP);
    #endif

    xcolor* C = (xcolor*)m_BMP.GetPixelData();

    s32 x,y;

    m_WorldMaxY = (m_MaxVolumeY-m_EyePos.Y) + 8.0f;
    m_WorldMinY = (m_MinVolumeY-m_EyePos.Y) - 8.0f;
    m_UVMinDist   = 0;
    m_UVMaxDist   = m_HazeFinishDist;
    
    // Compute constants
    m_UVDeltaYConst0 = -1.0f/(m_WorldMaxY-m_WorldMinY);
    m_UVDeltaYConst1 = m_WorldMaxY/(m_WorldMaxY-m_WorldMinY);
    m_UVDistConst0   = 1.0f/(m_UVMaxDist-m_UVMinDist);
    m_UVDistConst1   = -m_UVMinDist/(m_UVMaxDist-m_UVMinDist);

    for( y=0; y<m_UVHeight; y++ )
    {
        for( x=0; x<m_UVWidth; x++ )
        {
            f32 DeltaY;
            f32 Dist;
            f32 U = (f32)x/(f32)(m_UVWidth-1);
            f32 V = (f32)y/(f32)(m_UVHeight-1);

            Dist   = m_UVMinDist   + U*(m_UVMaxDist   - m_UVMinDist  );
            DeltaY = m_WorldMaxY + V*(m_WorldMinY - m_WorldMaxY);

            xcolor FC = ComputeFog(Dist,DeltaY);

            #ifdef TARGET_PS2
            FC.R ^= FC.B;
            FC.B ^= FC.R;
            FC.R ^= FC.B;
            FC.A >>= 1;
            #endif

            *C = FC;
            C++;
        }
    }

    #ifdef TARGET_PC
    vram_Register(m_BMP);
    #endif

    Timer.Stop();
    x_printfxy(0,0,"FogMap: %1.1f",Timer.ReadMs());
}
*/
//=========================================================================
/*
struct my_xcolor
{
    union
    {
        struct
        {
            u8 B,G,R,A;
        } split;
        s32 whole;
    };
};

void fog::BuildFogMap( void )
{
    // WARNING: This is highly optimized for the PS2
    // and will probably not work right on a system with
    // an RGB format different than ps2.
    if( !m_Initialized ) return;

    struct foginfo
    {
        f32 HazeValue;
        f32 DistValue;
        f32 VolumeValue;
        f32 BlendVolumeValue;
        f32 HazeR;
        f32 HazeG;
        f32 HazeB;
        f32 HazeFogInterp;
    };

    xtimer Timer;
    Timer.Start();

    #ifdef TARGET_PC
    static foginfo pFogInfo[128];
    vram_Unregister(m_BMP[m_BMPIndex]);
    #endif

    #ifdef TARGET_PS2
    foginfo* pFogInfo = (foginfo*)0x70000000;
    #endif

    s32 x,y;
    my_xcolor* C = (my_xcolor*)m_BMP[m_BMPIndex].GetPixelData();
//    xcolor* C = (xcolor*)m_BMP[m_BMPIndex].GetPixelData();

    f32 HazeR = m_HazeColor.R;
    f32 HazeG = m_HazeColor.G;
    f32 HazeB = m_HazeColor.B;
    f32 FogR  = m_FogColor.R;
    f32 FogG  = m_FogColor.G;
    f32 FogB  = m_FogColor.B;

    // Compute constants
    //m_WorldMaxY = (430.0f);//(247.75f);//m_MaxVolumeY-m_EyePos.Y) + 8.0f;
    //m_WorldMinY = (200.0f);//(50.0f);//m_MinVolumeY-m_EyePos.Y) - 8.0f;
    m_UVMinDist   = 0;
    m_UVMaxDist   = m_HazeFinishDist;

    m_UVDeltaYConst0 = -1.0f/(m_WorldMaxY-m_WorldMinY);
    m_UVDeltaYConst1 = m_WorldMaxY/(m_WorldMaxY-m_WorldMinY);
    m_UVDistConst0   = 1.0f/(m_UVMaxDist-m_UVMinDist);
    m_UVDistConst1   = -m_UVMinDist/(m_UVMaxDist-m_UVMinDist);

    // Build haze and dist values
    for( x=0; x<m_UVWidth; x++ )
    {
        f32 U = (f32)x/(f32)(m_UVWidth-1);
        f32 Dist;
        f32 Haze;
        
        Dist = m_UVMinDist + U*(m_UVMaxDist - m_UVMinDist);
        //Dist = MAX(Dist,m_HazeStartDist);
        //Dist = MIN(Dist,m_HazeFinishDist);
        Haze = (Dist - m_HazeStartDist) / (m_HazeFinishDist - m_HazeStartDist);
        Haze = MAX(0,Haze);
        Haze = MIN(1,Haze);

        if( FOG_NO_HAZE )
            Haze = 0;

        pFogInfo[x].HazeValue       = Haze;
        pFogInfo[x].DistValue       = Dist;
        pFogInfo[x].HazeR           = Haze*HazeR;
        pFogInfo[x].HazeG           = Haze*HazeG;
        pFogInfo[x].HazeB           = Haze*HazeB;
        pFogInfo[x].HazeFogInterp   = 1-Haze*Haze*Haze;
    }

    
    // Build volume values
    for( y=0; y<m_UVHeight; y++ )
    {
        band*   pBand;
        f32     Ht;
        s32     NBands;
        f32     Slope;
        f32     VolumeHaze;
        f32     PartialHt;

        f32     V = (f32)y/(f32)(m_UVHeight-1);
        f32     DeltaY = m_WorldMaxY + V*(m_WorldMinY - m_WorldMaxY);

        // ANDY
        DeltaY -= m_EyePos.Y;

        // Compute delta and distance
        Ht    = x_abs(DeltaY);
        Slope = 1.0f / Ht;

        VolumeHaze = 0;

        // Decide which bands to loop through
        if( DeltaY > 0 )
        {
            pBand  = m_PosBand;
            NBands = m_NPosBands;
        }
        else
        {
            pBand  = m_NegBand;
            NBands = m_NNegBands;
        }

        // Loop through the bands and accumulate fog
        while( NBands-- )
        {
            PartialHt   = MIN(Ht,pBand->Height);
            VolumeHaze += PartialHt * pBand->Factor;
            Ht         -= PartialHt;
            Ht          = MAX(0,Ht);
            pBand++;
        }

        if( FOG_NO_BANDS )
            VolumeHaze = 0;

        pFogInfo[y].VolumeValue = VolumeHaze*Slope;
    }


    {
        for( y=0; y<m_UVHeight; y++ )
        {
            f32 T=0;
            s32 C=0;
            s32 W=0;

            for( s32 i=-FOG_BLEND_RANGE; i<=FOG_BLEND_RANGE; i++ )
            {
                s32 y2 = y+i;
                if( (y2>=0) && (y2<m_UVHeight) )
                {
                    s32 w = FOG_BLEND_RANGE - ABS(i) + 1;
                    T += pFogInfo[y2].VolumeValue*w;
                    W+=w;
                    C++;
                }
            }

            pFogInfo[y].BlendVolumeValue = T / (f32)W;
        }

        for( y=0; y<m_UVHeight; y++ )
            pFogInfo[y].VolumeValue = pFogInfo[y].BlendVolumeValue;
    }

    if( FOG_USETEST )
    {
        for( s32 i=0; i<m_UVHeight; i++ )
        for( s32 j=0; j<m_UVWidth; j++ )
        {
            C->split.R = 128;
            C->split.G = 128;
            C->split.B = 128;
            C->split.A = 255;
#if 0
            C->R = (i*25 + j*19 +  64)%256;
            C->G = (i*7  + j*20 + 128)%256;
            C->B = (i*13 + j*17 + 200)%256;
            C->A = 128;
#endif
            #ifdef TARGET_PS2
            C->split.R ^= C->split.B;
            C->split.B ^= C->split.R;
            C->split.R ^= C->split.B;
            C->split.A >>= 1;
            #endif

            C++;
        }
    }
    else
    {
        register f32 MaxBand,MinBand,Const_001,Const_127,Const_1,Const_1e23;
        register s32 UVHeight,UVWidth;

        Const_001   = 0.001f;
        Const_127   = 127.0f;
        Const_1     = 1.0f;
        MaxBand     = m_MaxBandAlpha;
        MinBand     = m_MinBandAlpha;
        UVHeight    = m_UVHeight;
        UVWidth     = m_UVWidth;
        Const_1e23  = 1<<23;

        for( y=0; y<UVHeight; y++ )
        {
            f32 VolumeHaze = pFogInfo[y].VolumeValue;
            foginfo* pF = pFogInfo;

            for( x=0; x<UVWidth; x++ )
            {
                f32 FA;
                f32 NFA;
                f32 HA;
                f32 TA;

                // Compute fog alpha
                FA = pF->DistValue * VolumeHaze;
                FA = MIN(MaxBand,FA);
                FA = MAX(MinBand,FA);

                // Get haze alpha
                HA = pF->HazeValue;
            
                // Compute color to interpolate towards
                NFA = FA*pF->HazeFogInterp;

                if( NFA+HA > Const_001 )
                {
                    f32 OneOverTotal = Const_1 / (NFA+HA);

                    // Compute alpha
                    TA     = FA+HA;
                    TA     = MIN(Const_1,TA);

                    u8 r,g,b,a;

                    union
                    {
                        f32 f;
                        s32 i;
                    } x;

                    // Faster float to int method from Game Programming Gems 2, pp168-170
                    x.f = Const_127*TA + Const_1e23;
                    a = x.i & 0xff;

                    x.f = ((((FogR)*NFA + pF->HazeR)) * OneOverTotal) + Const_1e23;
                    r = x.i & 0xff;

                    x.f = ((((FogG)*NFA + pF->HazeG)) * OneOverTotal) + Const_1e23;
                    g = x.i & 0xff;

                    x.f = ((((FogB)*NFA + pF->HazeB)) * OneOverTotal) + Const_1e23;
                    b = x.i & 0xff;
                    C->whole = (a<<24)|(b<<16)|(g<<8)|r;
                }
                else
                {
                    C->whole    = (0<<24)|((u8)FogB<<16)|((u8)FogG<<8)|(u8)FogR;
                }
                C++;
                pF++;
            }
        }
    }

    #ifdef TARGET_PC
    vram_Register(m_BMP[m_BMPIndex]);
    #endif

    Timer.Stop();

    x_printfxy(0,0,"FogMap: %1.1f",Timer.ReadMs());

}
*/

//=========================================================================

void fog::BuildFogMap( void )
{
    //xtimer Timer;
    //Timer.Start();

    s32 x ;

    s32 x1,x2, y, i;

    m_UVMinDist      = 0;
    m_UVMaxDist      = m_HazeFinishDist;

    m_UVDeltaYConst0 =        -1.0f / (m_WorldMaxY - m_WorldMinY);
    m_UVDeltaYConst1 =  m_WorldMaxY / (m_WorldMaxY - m_WorldMinY);
    m_UVDistConst0   =         1.0f / (m_UVMaxDist - m_UVMinDist);
    m_UVDistConst1   = -m_UVMinDist / (m_UVMaxDist - m_UVMinDist);

    #ifdef TARGET_PC
    vram_Unregister( m_BMP[m_BMPIndex] );
    #endif

    u8* pData        = (u8*)m_BMP[m_BMPIndex].GetPixelData();
    f32 WorldStepY   = (m_WorldMaxY - m_WorldMinY) / m_UVHeight;
    f32 DistanceStep = m_HazeFinishDist / m_UVWidth;
    f32 HazeScaler   = 255.0f / (m_HazeFinishDist - m_HazeStartDist) ;

    // Pixels before haze
    x1 = 1 + (s32)(m_HazeStartDist / DistanceStep) ;
    x1 = MIN(x1, m_UVWidth) ;

    // Pixels after haze
    x2 = m_UVWidth - x1 ;

    // Setup distance (ready for after haze loop)
    f32 Distance         = x1 * DistanceStep ;
    f32 DistanceFogStart = (Distance - m_HazeStartDist) * HazeScaler ;
    f32 DistanceFogDelta = DistanceStep * HazeScaler ;

    // Make sure haze fog is at least 255 on the last pixel
    if ((x2-1) > 0)
        DistanceFogDelta = (255.0f - DistanceFogStart) / (f32)(x2-1) ;
    else
    {
        // Just 1 pixel of haze - make it solid
        DistanceFogStart = 255.0f ;
        DistanceFogDelta = 255.0f ;
    }        
    DistanceFogStart += 0.5f ;

    // Y Loop
    for( y = 0; y < m_UVHeight; y++ )
    {
        f32 Y = m_WorldMaxY - (WorldStepY * y);

        // For the current value of y, decide how much fog is encountered by
        // moving vertically through the various fog bands to the eye position.

        f32 LayerFog = 0.0f;
        f32 Span;

        for( i = 0; i < m_NVolumes; i++ )
        {
            if( Y > m_EyePos.Y )
            {
                // Try to rule out the volume.
                if( m_EyePos.Y > m_Volume[i].MaxY ) continue;
                if(          Y < m_Volume[i].MinY ) continue;

                // OK, we have spanned some of the volume.  But how much?
                Span = MIN( m_Volume[i].MaxY,          Y ) -
                       MAX( m_Volume[i].MinY, m_EyePos.Y );

                // Add in the attenuation.
                LayerFog += Span * m_Volume[i].VisFactor;
            }
            else
            {
                // Try to rule out the volume.
                if(          Y > m_Volume[i].MaxY ) continue;
                if( m_EyePos.Y < m_Volume[i].MinY ) continue;

                // OK, we have spanned some of the volume.  But how much?
                Span = MIN( m_Volume[i].MaxY, m_EyePos.Y ) -
                       MAX( m_Volume[i].MinY,          Y );

                // Add in the attenuation.
                LayerFog += Span * m_Volume[i].VisFactor;
            }
        }

        // Scale the layer fog according to the distance from eye
        LayerFog *= 255.0f ;
        LayerFog /= ABS( m_EyePos.Y - Y );

        // Setup layer fog
        f32 Fog      = 0 ;
        f32 FogDelta = LayerFog * DistanceStep ;

        // Before haze start loop
        x = x1 ;
        while(x--)
        {
            // Store
            #ifdef TARGET_PS2
            {
                if (Fog <= 255.0f)
                    *pData++ = (u8)Fog ;
                else
                    *pData++ = 255 ;
            }
            #endif

            #ifdef TARGET_PC
            {
                *pData++ = m_FogColor.B;
                *pData++ = m_FogColor.G;
                *pData++ = m_FogColor.R;
                if (Fog <= 255.0f)
                    *pData++ = (u8)Fog ;
                else
                    *pData++ = 255 ;
            }
            #endif

            // Next distance
            Fog += FogDelta ;
        }

        // After haze start loop
        if (x2)
        {
            // Setup distance fog start
            f32 DistanceFog = DistanceFogStart ;
            
            // Take most dense fog
            f32 FinalFog = DistanceFog ;

            // Fill
            x = x2 ;
            while(x--)
            {
                // Take most dense fog
                FinalFog = MAX( Fog, DistanceFog );

                // Store
                #ifdef TARGET_PS2
                {
                    if (FinalFog <= 255.0f)
                        *pData++ = (u8)FinalFog ;
                    else
                        *pData++ = 255 ;
                }
                #endif

                #ifdef TARGET_PC
                {
                    *pData++ = m_FogColor.B;
                    *pData++ = m_FogColor.G;
                    *pData++ = m_FogColor.R;
                    if (FinalFog <= 255.0f)
                        *pData++ = (u8)FinalFog ;
                    else
                        *pData++ = 255 ;
                }
                #endif

                // Next distance
                Fog         += FogDelta ;
                DistanceFog += DistanceFogDelta ;
            }

            // Check to make sure last pixel is fully fogged!
            ASSERT(FinalFog >= 255.0f) ;
        }
    }

    #ifdef TARGET_PC
    vram_Register( m_BMP[m_BMPIndex] );
    #endif


    //Timer.Stop() ;
    //x_printfxy(2,20, "BuildFogMap:%fms", Timer.ReadMs() ) ;
}

//=========================================================================

void fog::RenderFogMap( void ) const
{
    if( !m_Initialized ) return;
    if( !g_DebugFog.ShowFogMap ) return;

    xcolor Back = XCOLOR_WHITE;
    if( g_DebugFog.FlashBack )
        Back.Random();

    draw_Rect( irect( 16, 
                      16, 
                      15 + m_UVWidth  * g_DebugFog.ScaleFogMap,
                      15 + m_UVHeight * g_DebugFog.ScaleFogMap ), 
               Back, 
               FALSE );

    draw_SetTexture( m_BMP[m_BMPIndex] );
    draw_Begin( DRAW_SPRITES, DRAW_2D|DRAW_TEXTURED|DRAW_USE_ALPHA );
    draw_Sprite( vector3( 16, 16, 0 ), 
                 vector2( (f32)(m_UVWidth  * g_DebugFog.ScaleFogMap),
                          (f32)(m_UVHeight * g_DebugFog.ScaleFogMap) ),
                 XCOLOR_WHITE );

    //g_DebugFog.SampleColor = m_BMP[m_BMPIndex].GetPixelColor( g_DebugFog.SampleX, 
                                                              //g_DebugFog.SampleY );

    draw_End();   

    x_printfxy(2,10, "LookupFogColorTime: %f", g_DebugFog.Time) ;
    if (tgl.NRenders != g_DebugFog.Frame)
    {
        g_DebugFog.Frame = tgl.NRenders ;
        g_DebugFog.Time  = 0 ;
    }
}

//=========================================================================






