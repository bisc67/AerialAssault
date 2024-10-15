//==============================================================================
//
//  xCL_Thru.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xCL.hpp"
#include "xCL_Thru.hpp"
#include "Inevitable.hpp"

//==============================================================================
//  VARIABLES
//==============================================================================

static cmd_line_data&  C = g_CmdLineData;

//==============================================================================
//  FUNCTIONS
//==============================================================================

int PassThru( void )
{
    int   i;
    bool  Compiler = false;
    int   Length   = 0;
    char* pCmdLn   = NULL;

    // We are going to call the original cl.exe or link.exe.  We need to do a 
    // few things:
    //  (1) We must replace "cl" with "_cl", or "link" with "_link".
    //  (2) We must place quotes around any argument with spaces.
    //  (3) We must define XCORE_PATH on compile command lines.

    // Sum up the size of the original command line.
    for( i = 0; i < C.m_argc; i++ )
    {
        Length += strlen( C.m_argv[i] );
    }

    // Allow enough room for: all possible edits, spaces between args, and 
    // addition of XCORE_PATH macro.
    Length += (4 * C.m_argc);
    Length += _MAX_PATH + 11;

    // Allocate the command line space.
    pCmdLn = (char*)malloc( Length );
    pCmdLn[0] = 0;

    // First, take care of argument 0, which is the cl or link invokation.
    {
        bool  AddQuotes = false;
        char  Whole[ _MAX_PATH   ];
        char  Dir  [ _MAX_DIR    ];
        char  Drive[ _MAX_DRIVE  ];
        char  FName[ _MAX_FNAME  ];
        char  Ext  [ _MAX_EXT    ];
        char* pNewFName = FName;

        // Is there a space anywhere in argument 0?  If so, we must quote it.
        if( strchr( C.m_argv[0], ' ' ) )
            AddQuotes = true;

        // Break argument 0 into its various parts.
        _splitpath( C.m_argv[0], Drive, Dir, FName, Ext );

        // Special case check...
        {
            //
            // If the FName is 'xCL' then we have a recursive call!
            // We're probably debugging xCL or something like that.
            //
            if( stricmp( FName, "xCL" ) == 0 )
                return( 0 );
        }

        // Change "CL" to "_cl".
        if( stricmp( FName, "CL" ) == 0 )
        {
            pNewFName = "_cl";
            Compiler  = true;
        }

        // Change "LINK" to "_link".
        if( stricmp( FName, "LINK" ) == 0 )
            pNewFName = "_link";

        // And now for a special case...  
        // Change "DUMPBIN" to "_link".
        if( stricmp( FName, "DUMPBIN" ) == 0 )
            pNewFName = "_link";

        // Put it all back together.
        _makepath( Whole, Drive, Dir, pNewFName, Ext );

        //
        // Put the modified version of argument 0 in the command line.
        //

        if( AddQuotes )
            strcat( pCmdLn, "\"" );

        strcat( pCmdLn, Whole );

        if( AddQuotes )
            strcat( pCmdLn, "\"" );
    }       

    //
    // Add a macro definition on the command line for XCORE_PATH.
    //
    if( Compiler )
    {
        char* pX = getenv( "X" );
        if( pX )
        {
            char Buffer[ _MAX_PATH ];
            sprintf( Buffer, " /D XCORE_PATH=\\\"%s\\\"", UseOnlySlashes(pX) );
            strcat( pCmdLn, Buffer );
        }
    }

    //
    // For each argument, copy it over to the new command line adding 
    // quotes where needed.
    //      
    for( i = 1; i < C.m_argc; i++ )
    {
        bool AddQuotes = false;
      
        if( strchr( C.m_argv[i], ' ' ) )
            AddQuotes = true;

        strcat( pCmdLn, " " );

        if( AddQuotes )
            strcat( pCmdLn, "\"" );

        strcat( pCmdLn, C.m_argv[i] );

        if( AddQuotes )
            strcat( pCmdLn, "\"" );
    }

/*
    //
    // Extra verbage.  Show the environment variables.
    //
    if( g_Verbose )
    {
        system( "set > \"C:/Environment Variables.txt\"" );
        OpenSection( "Environment Variables" );
        DumpTextFileContent( "C:/Environment Variables.txt" );
        CloseSection();
        system( "del \"C:/Environment Variables.txt\"" );
    }
*/

    // Extra verbage?  Show the xCore directory to be used.
    if( g_Verbose )
    {
        OpenSection( "xCore Directory In Use" );
        printf( "%s\n", getenv( "X" ) );
        CloseSection();
    }

    // Extra verbage?
    if( g_Verbose || g_EchoCmdLine )
    {
        DumpCommandLineAndResponseFile( "PC Pass Thru",
                                        pCmdLn,
                                        NULL );
    }

    //
    // Now that we have our command line, invoke it!
    //
    {
        int Result;
        Result = system( pCmdLn );
        return( Result );    
    }
}

//==============================================================================
