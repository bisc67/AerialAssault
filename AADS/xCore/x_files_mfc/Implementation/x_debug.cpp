//==============================================================================
//  
//  x_debug.cpp
//  
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_DEBUG_HPP
#include "../x_debug.hpp"
#endif

#ifndef X_PLUS_HPP
#include "../x_plus.hpp"
#endif

#ifndef X_STDIO_HPP
#include "../x_stdio.hpp"
#endif

#ifndef X_STRING_HPP
#include "../x_string.hpp"
#endif

#ifndef X_FILES_PRIVATE_HPP
#include "x_files_private.hpp"
#endif

//==============================================================================
//  VARIABLES
//==============================================================================
static void s_DefaultLogHandler(const char *pString);

volatile    s32     DDBZ;
static      rtf_fn  s_DefaultRTFHandler;
static      rtf_fn* s_pRTFHandler = s_DefaultRTFHandler;
static      log_fn* s_pLogHandler = s_DefaultLogHandler;
static      xbool   s_Asserting   = FALSE;
static      char    s_DebugVersion[128]=__DATE__ __TIME__;
static  x_debug_crash_fn *s_DebugCrashFunction=NULL;

//==============================================================================
//  FUNCTIONS
//==============================================================================

//==============================================================================
//  Debugger Message

#ifdef TARGET_PC
#include <windows.h>    // OutputDebugString()
#endif

#ifdef TARGET_PS2
#include <stdio.h>      // printf()
#endif

//==============================================================================
void x_DebugMsg( const char* pFormatStr, ... )
{
    (void)pFormatStr;

//#ifdef X_DEBUG

    x_va_list   Args;
    x_va_start( Args, pFormatStr );

    xvfs XVFS( pFormatStr, Args );

    #ifdef TARGET_PC
        OutputDebugString( XVFS );
    #endif

    #ifdef TARGET_PS2
    #ifndef TARGET_DVD
        printf( XVFS );
    #endif
    #endif

//#endif
}
    
//==============================================================================
void x_DebugLog( const char* pFormatStr, ... )
{
    (void)pFormatStr;

    x_va_list   Args;
    x_va_start( Args, pFormatStr );

    xvfs XVFS( pFormatStr, Args );

    if( s_pRTFHandler )
    {
        s_pLogHandler(XVFS);
    }
    else
    {
        s_DefaultLogHandler(XVFS);
    }

}

//==============================================================================
void x_DebugSetVersionString(const char *pString)
{
    x_strncpy(s_DebugVersion,pString,sizeof(s_DebugVersion));
}

//==============================================================================
const char *x_DebugGetVersionString(void)
{
    return s_DebugVersion;
}

//==============================================================================
void        x_DebugSetCrashFunction(x_debug_crash_fn *Callback)
{
    s_DebugCrashFunction = Callback;
}

//==============================================================================
x_debug_crash_fn *x_DebugGetCrashFunction(void)
{
    return s_DebugCrashFunction;
}

//==============================================================================

xbool RTFHandler( const char* pFileName,      
                  s32         LineNumber,     
                  const char* pExprString,    
                  const char* pMessageString )
{
    if( !s_pRTFHandler )
        return( TRUE );
    else
        return( s_pRTFHandler( pFileName, 
                               LineNumber, 
                               pExprString, 
                               pMessageString ) );
}

//==============================================================================

void x_SetRTFHandler( rtf_fn* pRTFHandler )
{
    if( pRTFHandler == NULL )
        s_pRTFHandler = s_DefaultRTFHandler;
    else
        s_pRTFHandler = pRTFHandler;

}

//==============================================================================

rtf_fn* x_GetRTFHandler( void )
{
    return( s_pRTFHandler );
}

//==============================================================================
void x_SetLogHandler(log_fn *pLogHandler)
{
    if (pLogHandler == NULL)
    {
        s_pLogHandler = s_DefaultLogHandler;
    }
    else
    {
        s_pLogHandler = pLogHandler;
    }
}

//==============================================================================
static void s_DefaultLogHandler(const char *pString)
{

    X_FILE *fp;

    fp = x_fopen("debuglog.txt","wat");
    if (fp)
    {
        x_fwrite(pString,x_strlen(pString),1,fp);
        x_fclose(fp);
    }
    else
    {
        x_DebugMsg("Cannot open Log: %s\n",pString);
    }
}

//==============================================================================

//------------------------------------------------------------------------------
#ifdef TARGET_PC
//------------------------------------------------------------------------------

#define DEFAULT_RTF_HANDLER_DEFINED

#include <windows.h>    // OutputDebugString()
#include <stdio.h>      // printf()

inline void DebugPrintString( const char* pString )
{
    OutputDebugString( pString );
    printf( pString );
}

static
xbool s_DefaultRTFHandler( const char* pFileName,      
                           s32         LineNumber,     
                           const char* pExprString,    
                           const char* pMessageString )
{
    // Watch out for "Oops! I forgot to call x_Init()."
    if( x_GetInitialized() == 0 )
    {
        DebugPrintString( "***\n*** ERROR: x_files not initialized.\n***\n" );
        BREAK;
    }

    // Watch out for infinite assertion failures.
    if( s_Asserting )
        BREAK;

    s_Asserting = TRUE;

    DebugPrintString( "***\n*** RUNTIME FAILURE\n" );

    if( pFileName )         DebugPrintString( xfs( "*** File: %s on line %d\n", pFileName, LineNumber ) );
    else                    DebugPrintString( xfs( "*** File: <unknown> on line %d\n", LineNumber ) );
    if( pExprString )       DebugPrintString( xfs( "*** Expr: %s\n", pExprString ) );
    if( pMessageString )    DebugPrintString( xfs( "*** Msg : %s\n", pMessageString ) );
    
    DebugPrintString( "***\n" );

    s_Asserting = FALSE;
    return( TRUE );
}

//------------------------------------------------------------------------------
#endif // TARGET_PC
//------------------------------------------------------------------------------

//==============================================================================

//------------------------------------------------------------------------------
#ifdef TARGET_PS2
//------------------------------------------------------------------------------

#define DEFAULT_RTF_HANDLER_DEFINED

static
xbool s_DefaultRTFHandler( const char* pFileName,      
                           s32         LineNumber,     
                           const char* pExprString,    
                           const char* pMessageString )
{
    // Watch out for infinite assertion failures.
    if( s_Asserting )
        BREAK;

    s_Asserting = TRUE;

    x_printf( "***\n*** RUNTIME FAILURE\n" );
    if( pFileName )         x_printf( "*** File: %s on line %d\n", pFileName, LineNumber );
    else                    x_printf( "*** File: <unknown> on line %d\n", LineNumber );
    if( pExprString )       x_printf( "*** Expr: %s\n", pExprString );
    if( pMessageString )    x_printf( "*** Msg : %s\n", pMessageString );
    x_printf( "***\n" );

    x_DebugMsg( "***\n*** RUNTIME FAILURE\n" );
    if( pFileName )         x_DebugMsg( "*** File: %s on line %d\n", pFileName, LineNumber );
    else                    x_DebugMsg( "*** File: <unknown> on line %d\n", LineNumber );
    if( pExprString )       x_DebugMsg( "*** Expr: %s\n", pExprString );
    if( pMessageString )    x_DebugMsg( "*** Msg : %s\n", pMessageString );
    x_DebugMsg( "***\n" );

    s_Asserting = FALSE;
    return( TRUE );
}

//------------------------------------------------------------------------------
#endif // TARGET_PS2
//------------------------------------------------------------------------------

//==============================================================================

//------------------------------------------------------------------------------
#ifndef DEFAULT_RTF_HANDLER_DEFINED
//------------------------------------------------------------------------------

static
xbool s_DefaultRTFHandler( const char* pFileName,      
                           s32         LineNumber,     
                           const char* pExprString,    
                           const char* pMessageString )
{
    // Watch out for infinite assertion failures.
    if( s_Asserting )
        BREAK;

    s_Asserting = TRUE;

    x_printf( "***\n*** RUNTIME FAILURE\n" );

    if( pFileName )         x_printf( "*** File: %s on line %d\n", pFileName, LineNumber );
    else                    x_printf( "*** File: <unknown> on line %d\n", LineNumber );
    if( pExprString )       x_printf( "*** Expr: %s\n", pExprString );
    if( pMessageString )    x_printf( "*** Msg : %s\n", pMessageString );
    
    x_printf( "***\n" );

    s_Asserting = FALSE;
    return( TRUE );
}

//------------------------------------------------------------------------------
#endif // !DEFAULT_RTF_HANDLER_DEFINED
//------------------------------------------------------------------------------

//==============================================================================
