//=========================================================================
//
//  hud_font.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "aux_bitmap.hpp"
#include "hud_font.hpp"

#ifdef TARGET_PS2
#include "ps2_misc.hpp"
#endif

//=========================================================================

#define OFFSET_X    (2048-(512/2))
#define OFFSET_Y    (2048-(512/2))
#define CHAR_WIDTH  13
#define CHAR_HEIGHT 18
#define XBORDER      8
#define YBORDER      8

//=========================================================================

#ifdef TARGET_PS2

struct header
{
	dmatag   DMA;       // DMA tag
	giftag   PGIF;      // GIF for setting PRIM register
	s64      Prim;      // PRIM register
	s64      Color;		// RGBAQ register
	giftag   GIF;		// GIF for actual primitives
};

struct char_info
{
    s64 T0;
    s64 P0;
    s64 T1;
    s64 P1;
};

#endif

//=========================================================================

#ifdef TARGET_PS2

static header*          s_pHeader;
static s32              s_NChars;
static giftag           s_GIF PS2_ALIGNMENT(16);
static giftag           s_PGIF PS2_ALIGNMENT(16);
//static xcolor           s_CharColor;

#endif

//=========================================================================
/*
static xbool LoadTexture( xbitmap& BMP, const char* pPathName )
{
    char    Path[X_MAX_PATH];
    char    Drive[X_MAX_DRIVE];
    char    Dir[X_MAX_DIR];
    char    FName[X_MAX_FNAME];
    char    Ext[X_MAX_EXT];

    xbool   Success = FALSE;

    x_splitpath( pPathName, Drive, Dir, FName, Ext );
#if defined TARGET_PS2
    x_strcat( Dir, "PS2/" );
#elif defined TARGET_PC
    x_strcat( Dir, "PC/" );
#else
    #error NO LOAD TEXTURE FUNCTION FOR TARGET
#endif
    x_makepath( Path, Drive, Dir, FName, Ext );

    if( !(Success = BMP.Load( xfs("%s.xbmp",Path) )) )
    {
        if( (Success = auxbmp_Load( BMP, xfs("%s.tga",pPathName) )) )
        {
            BMP.ConvertFormat( xbitmap::FMT_P8_RGBA_8888 );
#if defined TARGET_PS2
            auxbmp_ConvertToPS2( BMP );
#elif defined TARGET_PC
            auxbmp_ConvertToD3D( BMP );
#else
    #error NO LOAD TEXTURE FUNCTION FOR TARGET
#endif
            BMP.Save( xfs("%s.xbmp",Path) );
        }
    }

    return Success;
}
*/
//=========================================================================
//  HUD Font
//=========================================================================

xbool hud_font::Load( const char* pPathName )
{
    // Load font image
    VERIFY( auxbmp_LoadNative( m_Bitmap, xfs("%s.tga",pPathName) ) );

    // Setup info
    m_Height = m_Bitmap.GetHeight()-1;
    x_memset( &m_Characters, 0, sizeof(Character)*256 );

    // Get info from bitmap size
    m_BmWidth   = m_Bitmap.GetWidth();
    m_BmHeight  = m_Bitmap.GetHeight();

    // Scan through font building character map
    s32 y = 0;
    for( s32 Row=0 ; Row<6 ; Row++ )
    {
        // Clear registration mark
        m_Bitmap.SetPixelColor( xcolor(255,255,255,0), 0, y );

        // Initialize for character row
        s32 x1 = 0;
        for( s32 Col=0 ; Col<16 ; Col++ )
        {
            // Scan registration marks for character
            s32 x2 = x1+1;
            while( (x2 < m_BmWidth) && !(m_Bitmap.GetPixelColor( x2, y ).R == 0) )
                x2++;

            // Clear Registration mark if found
            if( x2 < m_BmWidth )
                m_Bitmap.SetPixelColor( xcolor(255,255,255,0), x2, 0 );

            // Add character
            m_Characters[32+Row*16+Col].X = x1;
            m_Characters[32+Row*16+Col].Y = y+1;
            m_Characters[32+Row*16+Col].W = x2-x1;

            // Set start of next character
            x1 = x2+1;
        }

        // Scan down to next row
        if( Row < 5 )
        {
            m_RowHeight = y;
            while( (y < m_BmHeight) && !(m_Bitmap.GetPixelColor( 0, y ).R == 0) )
                y++;
            m_RowHeight = y - m_RowHeight;
            m_Height    = m_RowHeight - 1;
        }
    }

    // Register the bitmap
    vram_Register( m_Bitmap );

    // Return success
    return TRUE;
}

//=========================================================================

void hud_font::Kill( void )
{
    // UnRegister the bitmap
    vram_Unregister( m_Bitmap );
    m_Bitmap.Kill();
}

//=========================================================================

void hud_font::TextSize( irect& Rect, const char* pString, s32 Count ) const
{
    Rect.Set( 0,0,0,m_Height );

    // Loop until end of string
    while( *pString && (Count != 0) )
    {
        // Add character to width
        s32 c = *pString++;
        Rect.r += m_Characters[c].W + 1;

        // Decrease character count
        Count--;
    }

    // Adjust width for last character
    Rect.r -= 1;
}

//=========================================================================

void hud_font::FormattedTextSize( irect& Rect, const char* pString, s32 Count ) const
{
    s32 w = 0;
    Rect.Set( 0,0,0,m_Height );

    // Loop until end of string
    while( *pString && (Count != 0) )
    {
        s32 c = *pString++;

        // Check for newline
        if( c == '\n' )
        {
            // Adjust width and set rectangle
            w -= 1;
            if( w > Rect.r )
                Rect.r = w;
            w = 0;

            // Add another line
            Rect.b += m_Height+1;
        }
        else
        {
            // Add character to width
            w += m_Characters[c].W + 1;
        }

        // Decrease character count
        Count--;
    }

    // Adjust width and set rectangle
    w -= 1;
    if( w > Rect.r )
        Rect.r = w;
}

//=========================================================================

void hud_font::SetupTextRender( void )
{
#ifdef TARGET_PS2
    vram_Activate( m_Bitmap );
    gsreg_Begin();
    gsreg_SetClamping( TRUE );
    gsreg_SetMipEquation( FALSE, 1.0f, 0, MIP_MAG_POINT, MIP_MIN_POINT );
    gsreg_End();
#endif
}

//=========================================================================

void hud_font::DrawText( const irect& Rect, u32 Flags, xcolor Color, const char* pString ) const
{
    s32 tx = Rect.l;
    s32 ty = Rect.t;

    // Adjust position for alignment flags
    if( Flags & h_center )
    {
        irect Size;
        TextSize( Size, pString );
        tx += (Rect.GetWidth() - Size.GetWidth()) / 2;
    }
    else if( Flags & h_right )
    {
        irect Size;
        TextSize( Size, pString );
        tx += (Rect.GetWidth() - Size.GetWidth());
    }

    if( Flags & v_center )
    {
        ty += (Rect.GetHeight() - m_Height + 4)/2;
    }
    else if( Flags & v_bottom )
    {
        ty += (Rect.GetHeight() - m_Height);
    }

#ifdef TARGET_PC
    vector2 size;
    vector2 uv0;
    vector2 uv1;

    // Draw characters
    draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );
    draw_SetTexture( m_Bitmap );

    f32 BmWidth  = (f32)m_BmWidth;
    f32 BmHeight = (f32)m_BmHeight;
    f32 Height   = (f32)m_Height;
    while( *pString )
    {
        s32     c  = *pString++;
        s32     x = m_Characters[c].X;
        s32     y = m_Characters[c].Y;
        s32     w = m_Characters[c].W;
        f32     u0 = (x) / BmWidth;
        f32     u1 = (x+w) / BmWidth;
        f32     v0 = (y) / BmHeight;
        f32     v1 = (y+Height) / BmHeight;

        size.Set( (f32)w, Height );
        uv0.Set( u0, v0 );
        uv1.Set( u1, v1 );
        draw_SpriteUV( vector3((f32)tx,(f32)ty,10.0f), size, uv0, uv1, Color );
        tx += w + 1;
    }
    draw_End();
#else
    //Setup Texture
    vram_Activate( m_Bitmap );
    gsreg_Begin();
    gsreg_SetClamping( TRUE );
    gsreg_SetMipEquation( FALSE, 1.0f, 0, MIP_MAG_POINT, MIP_MIN_POINT );
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC, C_DST, A_SRC, C_DST) );
    gsreg_End();

    // Build GIF Tags
    s_PGIF.Build( GIF_MODE_REGLIST, 2, 1, 0, 0, 0, 1 );
    s_PGIF.Reg( GIF_REG_PRIM, GIF_REG_RGBAQ ) ;
    s_GIF.Build( GIF_MODE_REGLIST, 4, 0, 0, 0, 0, 1 );
    s_GIF.Reg( GIF_REG_UV, GIF_REG_XYZ2, GIF_REG_UV, GIF_REG_XYZ2 );

    // Compute size of header and skip over
    s_pHeader = DLStruct(header);
    s_NChars = 0;

    // Do Chars
    while( *pString )
    {
        s32 C;
        s32 CX,CY;
	    s32 X0,Y0,X1,Y1;
        char_info* pCH;

        C  = (*pString++);
        CX = m_Characters[C].X;
        CY = m_Characters[C].Y;

	    X0 = (OFFSET_X<<4) + ((tx)<<4);
	    Y0 = (OFFSET_Y<<4) + ((ty)<<4);
	    X1 = (OFFSET_X<<4) + ((tx+m_Characters[C].W)<<4);
	    Y1 = (OFFSET_Y<<4) + ((ty+m_Height)<<4);

        pCH = DLStruct(char_info);
	    pCH->T0 = SCE_GS_SET_UV( (CX<<4), (CY<<4) );
	    pCH->P0 = SCE_GS_SET_XYZ(X0,Y0,0xFFFFFFFF);
	    pCH->T1 = SCE_GS_SET_UV( ((CX+m_Characters[C].W+1)<<4), ((CY+m_Height+1)<<4) );
	    pCH->P1 = SCE_GS_SET_XYZ(X1,Y1,0xFFFFFFFF);

        tx += m_Characters[C].W + 1;

        s_NChars++;
    }

    // Render
    s_pHeader->DMA.SetCont( sizeof(header) - sizeof(dmatag) + s_NChars*sizeof(char_info) );
    s_pHeader->DMA.MakeDirect();
    s_pHeader->PGIF      = s_PGIF;
    s_pHeader->GIF       = s_GIF;
    s_pHeader->GIF.NLOOP = s_NChars; 

    s_pHeader->Prim = SCE_GS_SET_PRIM(GIF_PRIM_SPRITE,    // type of primative (point, line, line-strip, tri, tri-strip, tri-fan, sprite)
                            0,    // shading method (flat, gouraud)
                            1,    // texture mapping (off, on)
                            0,    // fogging (off, on)
                            1,    // alpha blending (off, on)
                            0,    // 1 pass anti-aliasing (off, on)
                            1,    // tex-coord spec method (STQ, UV)
                            0,    // context (1 or 2)
                            0);  // fragment value control (normal, fixed)

	s_pHeader->Color = SCE_GS_SET_RGBAQ( Color.R>>1, Color.G>>1, Color.B>>1, Color.A>>1, 0x3f800000 );

#endif
}

//=========================================================================

void hud_font::DrawFormattedText( const irect& Rect, u32 Flags, xcolor Color, const char* pString ) const
{
    s32     tx      = Rect.l;
    s32     ty      = Rect.t;
    irect   Size;
    s32     iStart  = 0;
    s32     iEnd    = 0;

    // Get size for vertical positioning
    FormattedTextSize( Size, pString );

    // Position start vertically
    if( Flags & v_center )
    {
        ty += (Rect.GetHeight() - Size.GetHeight())/2;
    }
    else if( Flags & v_bottom )
    {
        ty += (Rect.GetHeight() - Size.GetHeight());
    }

    // Seperate into lines of text
    while( pString[iEnd] )
    {
        // Locate extents of line of text
        iEnd = iStart;
        while( (pString[iEnd] != 0) && (pString[iEnd] != '\n') )
            iEnd++;

        // Get size of that substring
        TextSize( Size, &pString[iStart], iEnd-iStart );

        // Adjust position for alignment flags
        if( Flags & h_center )
        {
            tx = Rect.l + (Rect.GetWidth() - Size.GetWidth()) / 2;
        }
        else if( Flags & h_right )
        {
            tx = Rect.l + (Rect.GetWidth() - Size.GetWidth());
        }

#ifdef TARGET_PC
        vector2 size;
        vector2 uv0;
        vector2 uv1;

        // Draw characters
        draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );
        draw_SetTexture( m_Bitmap );

        f32 BmWidth  = (f32)m_BmWidth;
        f32 BmHeight = (f32)m_BmHeight;
        f32 Height   = (f32)m_Height;
        for( ; iStart<iEnd ; iStart++ )
        {
            s32     c  = pString[iStart];
            s32     x = m_Characters[c].X;
            s32     y = m_Characters[c].Y;
            s32     w = m_Characters[c].W;
            f32     u0 = (x) / BmWidth;
            f32     u1 = (x+w) / BmWidth;
            f32     v0 = (y) / BmHeight;
            f32     v1 = (y+m_Height) / BmHeight;

            size.Set( (f32)w, Height );
            uv0.Set( u0, v0 );
            uv1.Set( u1, v1 );
            draw_SpriteUV( vector3((f32)tx,(f32)ty,10.0f), size, uv0, uv1, Color );
            tx += w + 1;
        }
        draw_End();
#else
        //Setup Texture
        vram_Activate( m_Bitmap );
        gsreg_Begin();
        gsreg_SetClamping( TRUE );
        gsreg_SetMipEquation( FALSE, 1.0f, 0, MIP_MAG_POINT, MIP_MIN_POINT );
        gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC, C_DST, A_SRC, C_DST) );
        gsreg_End();

        // Build GIF Tags
        s_PGIF.Build( GIF_MODE_REGLIST, 2, 1, 0, 0, 0, 1 );
        s_PGIF.Reg( GIF_REG_PRIM, GIF_REG_RGBAQ ) ;
        s_GIF.Build( GIF_MODE_REGLIST, 4, 0, 0, 0, 0, 1 );
        s_GIF.Reg( GIF_REG_UV, GIF_REG_XYZ2, GIF_REG_UV, GIF_REG_XYZ2 );

        // Compute size of header and skip over
        s_pHeader = DLStruct(header);
        s_NChars = 0;

        // Do Chars
        for( ; iStart<iEnd ; iStart++ )
        {
            s32 C;
            s32 CX,CY;
	        s32 X0,Y0,X1,Y1;
            char_info* pCH;

            C  = pString[iStart];
            CX = m_Characters[C].X;
            CY = m_Characters[C].Y;

	        X0 = (OFFSET_X<<4) + ((tx)<<4);
	        Y0 = (OFFSET_Y<<4) + ((ty)<<4);
	        X1 = (OFFSET_X<<4) + ((tx+m_Characters[C].W)<<4);
	        Y1 = (OFFSET_Y<<4) + ((ty+m_Height)<<4);

            pCH = DLStruct(char_info);
	        pCH->T0 = SCE_GS_SET_UV( (CX<<4), (CY<<4) );
	        pCH->P0 = SCE_GS_SET_XYZ(X0,Y0,0xFFFFFFFF);
	        pCH->T1 = SCE_GS_SET_UV( ((CX+m_Characters[C].W+1)<<4), ((CY+m_Height+1)<<4) );
	        pCH->P1 = SCE_GS_SET_XYZ(X1,Y1,0xFFFFFFFF);

            tx += m_Characters[C].W + 1;

            s_NChars++;
        }

        // Render
        s_pHeader->DMA.SetCont( sizeof(header) - sizeof(dmatag) + s_NChars*sizeof(char_info) );
        s_pHeader->DMA.MakeDirect();
        s_pHeader->PGIF      = s_PGIF;
        s_pHeader->GIF       = s_GIF;
        s_pHeader->GIF.NLOOP = s_NChars; 

        s_pHeader->Prim = SCE_GS_SET_PRIM(GIF_PRIM_SPRITE,    // type of primative (point, line, line-strip, tri, tri-strip, tri-fan, sprite)
                                0,    // shading method (flat, gouraud)
                                1,    // texture mapping (off, on)
                                0,    // fogging (off, on)
                                1,    // alpha blending (off, on)
                                0,    // 1 pass anti-aliasing (off, on)
                                1,    // tex-coord spec method (STQ, UV)
                                0,    // context (1 or 2)
                                0);  // fragment value control (normal, fixed)

	    s_pHeader->Color = SCE_GS_SET_RGBAQ( Color.R>>1, Color.G>>1, Color.B>>1, Color.A>>1, 0x3f800000 );
#endif

        // Set start of next line
        ty += m_Height+1;
        iStart = iEnd+1;
    }
}

//=========================================================================

