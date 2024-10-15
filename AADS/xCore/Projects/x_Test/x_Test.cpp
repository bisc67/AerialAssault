
//==============================================================================
/* TEST xfs CLASS CODE

#include "x_files.hpp"

void Function( const char* pA )
{
    (void)pA;
}

int main( void )
{
    x_Init();
    x_printfxy( 10, 10, "A" );
    xstring Q = "123";

    {
        f32* p;
    
        p = new f32;
        delete p;

        p = new f32[10];
        delete [] p;

        p = (f32*)x_malloc ( 4 );
    }

    Function( Q );
    Function( xfs( "Like, %s", Q ) );

    ASSERTS( FALSE, xfs( "Like, %s", "OMG" ) );
    x_Kill();
    return( 0 );
}

*/
//==============================================================================

//==============================================================================
/* LINEAR COMBINATION CODE

#include "x_files.hpp"

void main( void )
{
    x_Init();

    while( 1 )
    {
        vector2 A( x_frand(-500,500), x_frand(-500,500) );
        vector2 B( x_frand(-500,500), x_frand(-500,500) );
        vector2 C( x_frand(-500,500), x_frand(-500,500) );
        vector2 D;
        vector2 E;

        f32     S, T;

        S = ((B.X * C.Y) - (B.Y * C.X)) / ((A.Y * B.X) - (B.Y * A.X));
        T = ((A.X * C.Y) - (A.Y * C.X)) / ((B.Y * A.X) - (A.Y * B.X));

        D = (S * A) + (T * B);
        E = C - D;
    }

    x_Kill();
}

*/
//==============================================================================

//==============================================================================
/* TEST BINARY SEARCH CODE

#include "x_files.hpp"

s32 Compare( const void* pItem1, const void* pItem2 )
{
    s32& Item1 = *(s32*)pItem1;
    s32& Item2 = *(s32*)pItem2;

    if( Item1 < Item2 )   return( -1 );
    if( Item1 > Item2 )   return(  1 );

    return( 0 );
}

s32 List[] = { 1, 3, 3, 5, 5, 5, 5, 5 };
s32 Count  = sizeof( List ) / 4;

void main( void )
{
    s32     Index;
    s32     Key = 4;
    void*   pAddr;
    void*   pLocation;

    pAddr = x_bsearch( &Key, List, Count, 4, Compare, pLocation );
    if( pAddr == NULL )
        Index = (s32*)pLocation - List;
    else
        Index = (s32*)pAddr - List;
}

*/
//==============================================================================

//==============================================================================
/* TEST x_printfxy IN WINDOWS CONSOLE

#include "x_files.hpp"

void main( void )
{
    x_Init();
    x_printfxy( 10, 10, "A" );
    x_Kill();
}
*/
//==============================================================================

//==============================================================================
/* TEST THE xstring CLASS

#include "x_files.hpp"

void main( void )
{
    x_Init();

    // Test set
    {
        xstring s1;
        xstring s2( 'A' );
        xstring s3( "abc" );
        xstring s4( s3 );

        s4.GetLength();
        s1.IsEmpty();
        s4.IsEmpty();
        s3.Clear();    
        s4.GetAt( 1 );
        s4.GetAt( 5 );
        s3.FreeExtra();
    }

    // Test set
    {
        s32 R, C, i;

        xstring s1;
        xstring s2 = 'A';
        xstring s3 = "abc";
        xstring s4 = "The quick brown fox\njumped over the lazy dog.";
        xstring s5 = s4;
        xstring s6;

        s4.FreeExtra();
        s4.IndexToRowCol(  3, R, C );
        s4.IndexToRowCol( 23, R, C );

        s6 = s4.Mid( 4, 5 );
        s6 = s4.Left ( 9 );
        s6 = s4.Right( 9 );

        s5.MakeUpper();
        s5.MakeLower();

        s4.Insert( 4, "very " );
        s5 = "very ";
        i = s4.Find( "lazy" );
        s4.Insert( i, s5 );
    }

    // Test set
    {
        s32 i;
        char s0[256];
        char c = 'X';

        xstring s1;
        xstring s2 = 'A';
        xstring s3 = "abc";
        xstring s4 = "The quick brown fox\njumped over the lazy dog.";
        xstring s5;

        s5.Format( "%d of %d", 1, 3 );
        s5.Format( "%s", s4 );
        s5.AddFormat( "\n[the end]%d", 4 );

        i = s5.Find( "very" );

        x_strcpy( s0, s5 );

        s5[0] = c;      // WRITE
        c     = s5[1];  // READ  -- This doesn't call expected function.
    }

    // Test set
    {
        xbool b;
        xstring s1;
        xstring s2 = 'A';
        xstring s3 = "ABC";     s3    = "A";
        xstring s4 = "ABC";     s4[1] = '\0';
        xstring s5 = "ABC";
        xstring s6 = "ABCDEFGH";

        b = s1 >  s2;
        b = s2 >  s3;
        b = s3 >  s4;
        b = s4 >  s5;
        b = s5 >  s6;

        s1  = s2 + s3 + s4 + s5 + s6;
        s1 += s1;

        s1.Dump( TRUE );
        s1.DumpHex();
        s6.DumpHex();

        x_printf( "\n" );
    }

    // Test set
    {
        xstring s1( 256 );
        s32     i;

        for( i = 0; i < 256; i++ )
            s1 += (char)i;

        s1.DumpHex();
    }

    // Test set
    {
        char  s[] = "XYZ";
        char* p   = s;

        xstring s1;
        xstring s2 = 'A';
        xstring s3 = "ABC";     s3    = "A";
        xstring s4 = "ABC";     s4[1] = '\0';
        xstring s5 = "ABC";
        xstring s6 = "ABCDEF";

        s1 += s2;
        s1 += s3;
        s1 += s4;

        s1 = s2 + s3;
        s1 = s3 + s4;
        s1 = s4 + s5;
        s1 = s5 + s6;

        s1 += 'A'   + s6;
        s1 += "ABC" + s6;
        s1 += s     + s6;
        s1 += p     + s6;
    }

    // Test set
    {
        xbool b;

        xstring s1 = 'A';
        xstring s2 = "ABC";     s2    = "A";
        xstring s3 = "ABC";     s3[1] = '\0';
        xstring s4 = "ABC";
        xstring s5 = "ABCDEF";

        char*   s6 = "A";
        char*   s7 = "ABC";

        b = s1 == s1;
        b = s1 == s2;
        b = s1 == s3;
        b = s1 == s4;
        b = s1 == s6;
        b = s1 == s7;
        b = s2 == s3;
        b = s2 == s4;
        b = s2 == s6;
        b = s2 == s7;
        b = s3 == s4;
        b = s3 == s6;
        b = s3 == s7;
        b = s4 == s6;
        b = s4 == s7;

        b = s2 == s1;
        b = s3 == s1;
        b = s4 == s1;
        b = s6 == s1;
        b = s7 == s1;
        b = s3 == s2;
        b = s4 == s2;
        b = s6 == s2;
        b = s7 == s2;
        b = s4 == s3;
        b = s6 == s3;
        b = s7 == s3;
        b = s6 == s4;
        b = s7 == s4;
    }

    // Test set
    {
        xstring String  = "Test";                            
        s32     Integer = 123;                               
        x_printf( "(%s)(%d)", (const char*)String, Integer );
        x_printf( "(%s)(%d)",              String, Integer );
        x_printf( "\n" );
    }

    x_Kill();
}
*/
//==============================================================================

//==============================================================================
/* TEST THE xarray CLASS.

#include "x_files.hpp"

int main( void )
{
    x_Init();
    {
        {
            vector3 v;

        }

        xarray<s32> A;
        xarray<s32> B;
        s32         i;

        A.Append( 1 );
        A.Append( 2 );
        A.Append( 3 );

        A.Append() = 5;

        A[1] = 7;
        i = A[1];

        B.Append( 10 );
        B.Append( 11 );
        B.Append( 12 );

        B.Delete( 1 );

        A.Append( B );
        A.Insert( 1, B );

        A.Delete( 1, 3 );

        B = A;

        A += 7;
        A += B;

        A = B + B;

        B.Delete( 1, 2 );

        xarray<s32> C = B;
        s32* p = C;

        p++;
        (void)p;

    //  i = C[10];
    //  i = C.GetAt( 10 );
    //  C.SetAt( 10, i );

        i = B.GetCount();
        i = B.GetCapacity();

        B.SetCapacity( 10 );
        B.SetCapacity(  5 );

        B.FreeExtra();

        i = A.Find( 3 );
        i = A.Find( 3, 4 );

        A.Clear();

        B.IsStatic();
        B.IsLocked();

        A.Delete( 0, A.GetCount() );
        B.Delete( 0, B.GetCount() );
        C.Delete( 0, C.GetCount() );
    }
    x_Kill();
    return( 0 );
}
*/
//==============================================================================

//==============================================================================
/* Test some xcolor stuff and some xbitmap stuff.

#include "x_files.hpp"
#include "..\Entropy\e_View.hpp"
#include <math.h>

xbool   auxbmp_Load     ( xbitmap& Bitmap, const char* pFileName );


int main( void )
{
    x_Init();

    {
        matrix4 M;
        M.Identity();
        M.RotateY( R_90 );

        vector3 V( 0, 0, 1 );
        V = M * V;

        f32 R  = R_90;
        f32 C1 = x_cos( R_90 );
        f32 C2 = (f32)cos( R_90 );
    }

    {
        view View;
        vector3 V;

        View.Translate( vector3(0,0,1) );
        View.RotateY( R_90 );

        V = View.ConvertW2V( vector3(1,0,1) );
        V = View.ConvertW2V( vector3(0,0,0) );
    }

    {
        f32 a = 10.0f;
        f32 b = 50.0f;
        s32 c = 1;
        radian Angle = R_15;

        x_abs       ( a );
        x_sqr       ( a );
        x_sqrt      ( a );
        x_1sqrt     ( a );
        x_floor     ( a );
        x_ceil      ( a );
        x_log       ( a );
        x_log2      ( a );
        x_log10     ( a );
        x_exp       ( a );
        x_pow       ( a,  b );
        x_fmod      ( a,  b );
        x_lpr       ( a,  b );
        x_round     ( a,  b );
        x_modf      ( a, &b );
        x_frexp     ( a, &c );
        x_ldexp     ( a,  c );

        x_sin           ( Angle   );
        x_cos           ( Angle   );
        x_tan           ( Angle   );
        x_asin          ( a );
        x_acos          ( a );
        x_atan          ( a );
        x_atan2         ( a, b );
        x_sincos        ( Angle, a, b );
        x_ModAngle      ( Angle );
        x_ModAngle2     ( Angle );
        x_MinAngleDiff  ( Angle, Angle );
    }

    {
        xcolor a( XCOLOR_BLUE );
        xcolor b( a );
        xcolor c( 0xFFFFFFFF );
        xcolor d( XCOLOR_RED );
        xcolor e;

        e = d;
        a.Set( XCOLOR_RED );
        b.Set( 0, 1, 2 );
        c.Set( 0, 1, 3, 4 );

        a.Random();
        a.Random( 127 );
        a.Random();
        a.Random();
    }

    {
        xcolor c1, c2, c3;
        xbitmap Bitmap;

        auxbmp_Load( Bitmap, "Test05.tga" );

        c1 = Bitmap.GetPixelColor(   0,   0 );
        c2 = Bitmap.GetPixelColor(   1,   0 );
        c3 = Bitmap.GetPixelColor(   2,   0 );

        Bitmap.DumpSourceCode( "TestA.cpp" );

        Bitmap.Save( "Test.xbmp" );
        auxbmp_Load( Bitmap, "Test.xbmp" );

        c1 = Bitmap.GetPixelColor(   0,   0 );
        c2 = Bitmap.GetPixelColor(   1,   0 );
        c3 = Bitmap.GetPixelColor(   2,   0 );

        Bitmap.DumpSourceCode( "TestB.cpp" );
    }

    x_Kill();
    return( 0 );
}
//*/
//==============================================================================

//==============================================================================
/*

#include "x_files.hpp"
#include "e_ScratchMem.hpp"

int main( void )
{
    x_Init();

    if( TRUE )
    {
        char Fill = 'a';

        smem_Init( 256 );

        while( TRUE )
        {
            x_memset( smem_BufferAlloc( 16 ), Fill++, 16 );

            smem_StackPushMarker();
            x_memset( smem_StackAlloc( 8 ), Fill++, 8 );
            x_memset( smem_StackAlloc( 8 ), Fill++, 8 );

            smem_StackPushMarker();
            x_memset( smem_StackAlloc( 8 ), Fill++, 8 );
            x_memset( smem_StackAlloc( 7 ), Fill++, 7 );

            x_memset( smem_BufferAlloc( 16 ), Fill++, 16 );

            smem_StackPopToMarker();

            x_memset( smem_BufferAlloc( 16 ), Fill++, 16 );

            smem_StackPopToMarker();

            smem_Toggle();
        }

        smem_Kill();
    }

    if( FALSE )
    {
        xtick T;
        //f32   Ms;
        f64   Sec;

        while( 1 )
        {
            T   = x_GetTime();
            //Ms  = x_TicksToMs( T );
            Sec = x_TicksToSec( T );

            x_printf( "T:%06I64d -- S:%05.10f\n", T, Sec );                
        }
    }

    if( TRUE )
    {
        xtick T;
        xtick LT = x_GetTime();
        f32   Ms;
        f64   Sec;

        while( 1 )
        {
            T   = x_GetTime();
            Ms  = x_TicksToMs ( T - LT );
            Sec = x_TicksToSec( T - LT );
            
            if( x_frand( 0.0f, 1.0f ) < 0.00001f )
                x_printf( " delta sec %5.10f    delta ms %5.10f\n", Sec, Ms );
            
            if( Sec > 1.0f )
            {
                x_printf( "********************************\n" );
                LT = T;
            }
            
            //x_printf( "T:%06I64d -- MS:%05.10f -- S:%05.10f\n", T, Ms, Sec );                
        }
    }

    x_Kill();
    return( 0 );
}

*/
//==============================================================================
/*

#include "x_files.hpp"
#include <conio.h>

int main( void )
{
    x_Init();

    xbool Finished = FALSE;

    xtimer T1, T2;
    f32    Trip, Stop;

    T1.Start();
    Trip = T1.TripSec();
    Stop = 0.0f;

    while( !Finished )
    {
        if( _kbhit() )
        {
            char Key = _getch();
            switch( Key )
            {
            case  27:   Finished = TRUE;        break;
            case 't':   Trip = T1.TripSec();    break;
            case 's':   Stop = T2.StopSec();    break;
            case 'g':   T2.Start();             break;
            case 'r':   T2.Reset();             break;
            default:                            break;
            }
        }

        x_printf( "Current:  %08.3f   Trip: %08.3f   Stop: %08.3f   Running: %08.3f\n", 
                  (f32)x_GetTimeSec(), Trip, Stop, T2.ReadSec() );
    }

    x_Kill();
    return( 0 );
}

*/  
//==============================================================================
/*

#include "x_files.hpp"

xbool auxbmp_Load( xbitmap& Bitmap, const char* pFileName );

int main( void )
{
    x_Init();

    if( FALSE )
    {
        xbitmap Bitmap;
        auxbmp_Load( Bitmap, "heavy_male00.BMP" );
        Bitmap.DumpSourceCode( "Default.cpp" );
        Bitmap.BuildMips( 10 );
    }

    if( FALSE )
    {   
        xbitmap Bitmap1, Bitmap2;
        auxbmp_Load( Bitmap1, "base.lfemale.bmp" );
        auxbmp_Load( Bitmap2, "emboss1.tga" );

        Bitmap1.ConvertFormat( xbitmap::FMT_32_ARGB_8888 );
        Bitmap2.ConvertFormat( xbitmap::FMT_32_ARGB_8888 );

        Bitmap1.Blit( 256, 92, 64, 64, 64, 64, Bitmap2 );
        Bitmap1.SaveTGA( "Blit01.tga" );
    }

    if( FALSE )
    {
        s32     Max, i;
        xbitmap Bitmap;
        char    FileName[] = "heavy_male00.BMP";

        auxbmp_Load( Bitmap, FileName );

        if( Bitmap.IsClutBased() )  Max = 72;
        else                        Max = 24;

        for( i = 1; i <= Max; i++ )
        {
            auxbmp_Load( Bitmap, FileName );
            Bitmap.ConvertFormat( (xbitmap::format)i );
            Bitmap.SaveTGA( xfs( "%s.tga", xbitmap::m_FormatInfo[i].String ) );
        }
    }

    if( TRUE )
    {
        xbitmap Bitmap;

        auxbmp_Load( Bitmap, "light_female00.xbmp" );
//      auxbmp_Load( Bitmap, "heavy_male00.xbmp" );  
        Bitmap.SaveTGA( "Original.tga" );
        Bitmap.ConvertFormat( (xbitmap::format)40 );
//      Bitmap.ConvertFormat( xbitmap::FMT_P8_URGB_8888 );
        Bitmap.BuildMips();
        Bitmap.SaveTGA( "Convert.tga"  );
        Bitmap.Save( "Convert.xbmp" );
        auxbmp_Load( Bitmap, "Convert.xbmp" );
        Bitmap.SaveTGA( "Convert2.tga" );
    }

    x_Kill();
    return( 0 );
}

*/  
//==============================================================================
/*
#include "x_files.hpp"

struct test
{
    test( void ) { };
    {    
            void(test::*pmfn)( void ) = (&test::~test);
    };

   ~test( void ) { };
    //int foo( char ) { };
};

int main( void )
{
    test A;
    A.test::test();
//  A.(&test::~test);


//  void (test::*pmfn)()     = (&test::test);
//  ((chiken2::state*)MyChicken2.m_pActiveState)->NewMessageMessage( "Hello" );

    return( 0 );
}
*/
//==============================================================================
/*
#include "x_files.hpp"

struct rita
{
    void* operator new( s32 Size ) { }
};

int main( void )
{
}
*/
//==============================================================================
/*
#include "x_files.hpp"

class verbose1
{
public:
                verbose1( void ) { x_printf( "verbose1::verbose1( void )\n" ); }
    virtual    ~verbose1( void ) { x_printf( "verbose1::~verbose1( void )\n" ); }
};

class verbose2 : public verbose1
{
public:
                verbose2( void ) { x_printf( "verbose2::verbose2( void )\n" ); }
    virtual    ~verbose2( void ) { x_printf( "verbose2::~verbose2( void )\n" ); }
};

void main( void )
{
    x_Init();
    x_printf( "BEGIN\n" );
    verbose2* pV = new verbose2[3];
    delete [] pV;
    x_printf( "END\n" );
    x_Kill();
}
*/
//==============================================================================
/*
#include "x_files.hpp"

class foobar
{
public:
    s32         List[10];

                foobar( void ) { for(s32 i=0;i<10;i++) List[i]=i; }
    virtual    ~foobar( void ) {  }
};


void main( void )
{
    x_Init();

    foobar  Foobar;
    foobar& FRef = *((foobar*)NULL);

    if( &FRef == NULL )
    {
        FRef = Foobar;
    }

    x_Kill();
}
*/
//==============================================================================

#include "x_files.hpp"

s32 Compare( const void* pA, const void* pB )
{
    s32* A = (s32*)pA;
    s32* B = (s32*)pB;

    if( *A < *B )   return( -1 );
    if( *A > *B )   return(  1 );
    else            return(  0 );
}

void main( void )
{
    x_Init();

    s32 i, j;
    s32 List[1002];

    List[   0] = 127;
    List[1001] = 127;

    for( i = 0; i < 500000; i++ )
    {
        x_printf( "[%06d]  ", i );

        x_srand( i );
        for( j = 1; j < 1001; j++ )
            List[j] = x_rand() & 0x000000FF;
        
        x_qsort( &List[1], 1000, sizeof(s32), Compare );

        ASSERT( List[   0] == 127 );
        ASSERT( List[1001] == 127 );
        for( j = 2; j < 1001; j++ )
            ASSERT( List[j-1] <= List[j] );
    }


    x_Kill();
}

//==============================================================================
