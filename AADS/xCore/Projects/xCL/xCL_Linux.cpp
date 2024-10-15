//==============================================================================
//
//  xCL_Linux.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES    
//==============================================================================

#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <windows.h>

#include "xCL.hpp"
#include "Inevitable.hpp"
#include "xCL_Linux.hpp"

//==============================================================================
//  LOCAL STORAGE
//==============================================================================

static cmd_line_data&   C           = g_CmdLineData;
static token_list       ObjLib;
static token_list       Section;
static char*            pBase;

//==============================================================================

//==============================================================================
//  PRIVATE FUNCTIONS
//==============================================================================

static 
bool PrepareEnvironment( void )
{
    char  Buffer[ 32784 ];
    char* pPath = NULL;

    //
    // Extra verbage.  Show the current directory.
    // 

    if( g_Verbose )
    {
        OpenSection( "Current Working Directory" );
        if( _getcwd( Buffer, 32784 ) == NULL )
            strcpy( Buffer, "<<< ERROR: Could not read current working directory. >>>" );
        else
            printf( "%s\n", Buffer );
        CloseSection();
    }

    //
    // We need to find the "xCore" directory we are going to use.  The xTB 
    // defines an environment variable "X" with the correct path.  Set its
    // path into pBase.
    //

    pBase = getenv( "X" );

    if( !pBase )
    {
        printf( "xCL -- ERROR: Unable to locate xCore directory to use.\n" );
        return( false );
    }

	char Line[256];
	sprintf(Line,"X_LIB=%s/3rdparty/linux/lib",pBase);
	_putenv(Line);
    //
    // Extra verbage.  Show the xCore directory to be used.
    // 
    if( g_Verbose )
    {
        OpenSection( "xCore Directory In Use" );
        printf( "%s\n", pBase );
        CloseSection();
    }

    // We need to prepend some directories to the path.  Even though the
    // user may have manually added all desired and needed paths, sometimes
    // DevStudio ends up with shorted paths...

    //
    // Add the SN and Nintendo tools directories to the path.
    //                                 
    {
        // Add the SN and Nintendo paths.
        sprintf( Buffer, "PATH=%s\\3rdParty\\Linux\\bin;", 
                          pBase, pBase);

        // Add the original path.
        pPath = getenv( "PATH" );
        if( pPath )
            strcat( Buffer, pPath );

        // Export the new path.
        _putenv( Buffer );
    }

    //
    // Extra verbage.  Show the search path.
    // 

    if( g_Verbose )
    {
        char* p = Buffer;
        char* q = Buffer;

        OpenSection( "Path for Linux Operation" );
        strcpy( Buffer, getenv( "PATH" ) );
        while( *p )
        {   
            if( *p == ';' )
            {
                *p = '\0';
                printf( "%s\n", q );
                q = p+1;
            }
            p++;
        }
        if( *q )
            printf( "%s\n", q );
        CloseSection();
    }

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

    return( true );
}

//==============================================================================

static 
int CompileSourceCode( void )
{
    int     Result = EXIT_SUCCESS;
    int     i;

	char	ResponseFile[32768];	// Command line which would have gone to the response file
    char    Message [ 32784 ];      // Message to show to user

    char    NewCmdLn[ 32784 ];      // New command line to execute
    char    VC6CmdLn[ 32784 ];      // VC6 command line to execute

    char    VC6RName[ _MAX_PATH  ]; // VC6 response file name

    FILE*   pVC6RFile;              // VC6 response file pointer

    //--------------------------------------------------------------------------
    // Setup local and temporary variables.
    //--------------------------------------------------------------------------

    // Start with an empty message.  Build it as we go.
    strcpy( Message, "xCL --" );

    //--------------------------------------------------------------------------
    // Build the SN response file.
    //--------------------------------------------------------------------------
	char Line[256];
	sprintf(Line,"GCC_INC=-I %s/3rdparty/linux/include",pBase);
   
    // Add the include paths.
    for( i = 0; i < C.m_IncludeDir.GetCount(); i++ )
        sprintf(Line+strlen(Line)," -I %s",UseOnlySlashes(C.m_IncludeDir[i]) );

    // Add current directory as an include directory.
    strcat(Line," -I." );

	_putenv(Line);
	_putenv("GCC_OPT=-fno-common"
		   " -finline-functions"
		   " -funsigned-char"
		   " -fno-exceptions"
		   " -Wall"
		   " -Wno-unused"
		   " -W"
		   " -Wno-switch"
		   " -Wno-non-template-friend");
    // Warnings.

    // Add the defines to the response file.
	sprintf(Line,"GCC_DEFS=");
    for( i = 0; i < C.m_Define.GetCount(); i++ )
        sprintf(Line+strlen(Line)," -D%s", C.m_Define[i] );

	// Add the XCORE_PATH macro.
	sprintf(Line+strlen(Line)," -DXCORE_PATH=\"%s\"", UseOnlySlashes(pBase) );

	_putenv(Line);

    // Open the response file.
	memset(ResponseFile,0,sizeof(ResponseFile));

    // Add some standard compile options.
    sprintf(ResponseFile+strlen(ResponseFile)," %%GCC_INC%% %%GCC_OPT%% %%GCC_DEFS%%" );        // Put un-init data in BSS
	sprintf(ResponseFile+strlen(ResponseFile)," -specs=%%X_LIB%%/linux.specs",pBase);

	

    // Add some standard compile options.
    sprintf(ResponseFile+strlen(ResponseFile), " -c" );   // Compile only; don't link

    //?? With GNU compiler, all debugging information is stored outside of 
    //?? the object files.  So, no harm comes from generating the debug 
    //?? information, even if it was not specifically requested.
    sprintf(ResponseFile+strlen(ResponseFile), " -g" );
    strcat( Message, " DebugInfo[ON]" );



    // Add compile optimization specification.
    {
        char* List[] = { "0", "1", "2", "3" };
        char* pOpt;

        // Args from Microsoft:         Map to GNU arg:
        //  -Od = "debug"                "0"  
        //  -O1 = "minimize size"        "1"  
        //  -Ot = "default"              "2"  
        //  -O2 = "maximize speed"       "3"  

        switch( C.m_Optimization )
        {
            case 'd':   pOpt = List[0];  break;
            case '1':   pOpt = List[1];  break;
            case 't':   pOpt = List[2];  break;
            case '2':   pOpt = List[3];  break;
            default :   pOpt = List[2];  break;
        }

        // Add to message string.
        strcat( Message, " Optimize[" );
        strcat( Message, pOpt );
        strcat( Message, "]" );

        // Add to response file.
        sprintf(ResponseFile+strlen(ResponseFile)," -O%s", pOpt );
    }


    // Normally we would add the system includes here with code like that shown
    // below.  But, the system includes are handled by the SN.INI file.
    //{
    //  char Path[] = "3rdParty/Linux";
    //  fprintf( pNewRFile, " -I\"%s/%s/include\" \n", pBase, Path );
    //}


    // Add the defines to the message for the user.
    strcat( Message, "\nxCL -- Defines[ " );
    for( i = 0; i < C.m_Define.GetCount(); i++ )
    {
        if( i != 0 )  strcat( Message, ", " );
        strcat( Message, C.m_Define[i] );
    }
    strcat( Message, " ]" );

    // We built a helpful message string for the user as we went along.
    // Might as well go ahead and display it now.
    printf( "%s\n.\n", Message );
    fflush( stdout );

    //--------------------------------------------------------------------------
    // Build the VC6 response file.
    //--------------------------------------------------------------------------

    // Determine response file name.
    sprintf( VC6RName, "%s\\_VC6_ResponseFile.txt", C.m_OutputPath );

    // Open the response file.
    pVC6RFile = fopen( VC6RName, "wt" );
    if( pVC6RFile == NULL )
    {
        printf( "xCL -- ERROR: Unable to open VC6 response file: %s\n", VC6RName );
        return( EXIT_FAILURE );
    }

    // Add some standard compile options.
    fprintf( pVC6RFile, " /c /nologo /W3 /FD\n" );

    // Just pre-process.  Good enough for dependency information!
    //
    // Also...
    //
    // Update: I was undefining _WIN32, but this caused problems when 
    // compiling STL on Linux during the VC6 phase.  So, I've stopped
    // undefining _WIN32 for now until I find out the exact reason I
    // did it in the first place.  Current large PS2 projects compile
    // fine regardless of _WIN32.
    // **** REMOVED
    // ** We have to use Microsoft's preprocessor to get our dependency 
    // ** information.  Their preprocessor introduces some extra defines.
    // ** And this can mess things up sometimes.  So, UNDEFINE them!
    // **** REMOVED "/U _WIN32"
    fprintf( pVC6RFile, " /P \n" );

    // Miscellaneous options.
    if( C.m_SBRPath[0] )      fprintf( pVC6RFile, " /FR\"%s/\"\n", C.m_SBRPath );
    if( C.m_PDBPath[0] )      fprintf( pVC6RFile, " /Fd\"%s/\"\n", C.m_PDBPath );
    if( C.m_Preprocess )      fprintf( pVC6RFile, " /P /C\n" );

    // In order to get the code to preprocess in VC, a few macros and such
    // must be defined.  Macros with arguments cannot be defined on the 
    // command line.  So, we will force all source files to include a tiny 
    // header file with the macros.    
    {
        unsigned long Written;
        HANDLE        Handle;
        FILETIME      FileTime;
        char          Name[ _MAX_PATH ];
        char          Buffer[] = "#define __attribute__(a)\r\n"
                                 "#define __IEEE_LITTLE_ENDIAN\r\n"
                                 "typedef unsigned int u_long128;\r\n";

        // Make the name.
        sprintf( Name, "%s/_SN_in_VC.h", C.m_OutputPath );

        // Create the file.
        Handle = CreateFile( Name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
        if( Handle == INVALID_HANDLE_VALUE )
        {
            printf( "xCL -- ERROR: Unable to open file for write: %s\n", Name );
            return( EXIT_FAILURE );
        }
        
        // Write desired content.
        WriteFile( Handle, Buffer, strlen(Buffer), &Written, NULL );

        // Set the time back to avoid triggering dependencies.
        GetFileTime( Handle, &FileTime, &FileTime, &FileTime );
        FileTime.dwHighDateTime = 28000000;
        SetFileTime( Handle, &FileTime, &FileTime, &FileTime );

        // That's it!
        CloseHandle( Handle ); 

        // Force the include.
        fprintf( pVC6RFile, " /FI \"%s\"\n", Name );
    }

    // Add the include paths.
    for( i = 0; i < C.m_IncludeDir.GetCount(); i++ )
        fprintf( pVC6RFile, " /I \"%s\" \n", C.m_IncludeDir[i] );

    // Add the Linux system includes.
    {
        char Path[] = "3rdParty/Linux";

        fprintf( pVC6RFile, " -I\"%s/%s/include\" \n", pBase, Path );
    }

    // We need an extra include path to find the "forced include" header file.
    //
    // NOTE:  The forced include is something like "/FI Debug\_File.h".  This
    // only has meaning from within the current directory (which is the project 
    // directory).  However, the file we are compiling may be in a different 
    // directory.  Includes check the directory with the source file, then all 
    // paths added on the command line.  If the source file is in another 
    // directory, using "/I Debug" won't work.  Add the current directory to the
    // path (/I .), and this will resolve to ".\Debug\_File.h" regardless of the
    // location of the source file since the current directory does not change.
    //
    // Using "/FI _File.h" and "/I Debug" has a flaw, too.  But I can't remember
    // what it is right now.

    fprintf( pVC6RFile, " /I . \n" );

    // Add the defines to the response file.
    for( i = 0; i < C.m_Define.GetCount(); i++ )
        fprintf( pVC6RFile, " /D %s \n", C.m_Define[i] );

    // Add the XCORE_PATH macro.
    fprintf( pVC6RFile, " /D XCORE_PATH=\\\"%s\\\"", UseOnlySlashes(pBase) );

    // That's it.
    fclose( pVC6RFile );

    //--------------------------------------------------------------------------
    // Execute!
    //--------------------------------------------------------------------------

    //
    // For every file we need to compile, we must run it through VC6 
    // (preprocess only) and then through the SN compiler.
    //

    for( i = 0; i < C.m_SourceCode.GetCount(); i++ )
    {   
        char SrcFName[ _MAX_FNAME ];

        _splitpath( C.m_SourceCode[i], NULL, NULL, SrcFName, NULL );

        //-------//
        //  VC6  //
        //-------//
        {
            sprintf( VC6CmdLn, "_cl @\"%s\" \"%s\"", VC6RName, C.m_SourceCode[i] );

            // Extra verbage?
            if( g_Verbose || g_EchoCmdLine )
                DumpCommandLineAndResponseFile( "VC6 Compile",
                                                VC6CmdLn,
                                                VC6RName );

            // Execute command for VC6 to compute dependencies.
            system( VC6CmdLn );

            // Delete .i file from the VC preprocessor.
            if( !C.m_Preprocess )
            {
                sprintf( VC6CmdLn, "del %s.i > nul", SrcFName );
                system ( VC6CmdLn );
            }
        }

        //------//
        //  SN  //
        //------//
        {
            sprintf( NewCmdLn, "gcc-linux.exe %s \"%s\" -o \"%s\\%s.obj\"", 
                                ResponseFile, 
                                UseOnlySlashes(C.m_SourceCode[i]),
                                UseOnlySlashes(C.m_OutputPath),
                                SrcFName );

            // Extra verbage?
            if( g_Verbose || g_EchoCmdLine )
			{
				char* env;
				env = getenv("GCC_OPT");
				if (env) 
				{
					printf("GCC_OPT=%s\n",env);
				}

				env=getenv("GCC_INC");
				if (env)
				{
					printf("GCC_INC=%s\n",env);
				}

				env = getenv("GCC_DEFS");
				if (env) 
				{
					printf("GCC_DEFS=%s\n",env);
				}

                DumpCommandLineAndResponseFile( "Linux Compile",
                                                NewCmdLn, 
                                                NULL );
			}

            // Execute command for SN compiler.
            Result |= system( NewCmdLn );
            //Result |= WinExec( NewCmdLn, SW_SHOWNORMAL);
        }
    }

    //--------------------------------------------------------------------------
    //  We're done!
    //--------------------------------------------------------------------------
    return( Result );
}

//==============================================================================

static
int Compile_s_File( int i )
{
    char    FName   [ _MAX_FNAME ];
    char    Ext     [ _MAX_EXT   ];
    char    CmdLine [ 32784 ];
    char    Buffer  [ 32784 ];
    int     Result = EXIT_SUCCESS;
    int     j;

    // Pick the name apart.
    _splitpath( C.m_SourceOther[i], NULL, NULL, FName, Ext );

    // Start building the command line.
    sprintf( CmdLine, "gcc-linux.exe -g -xassembler-with-cpp -I." );

    // Add the include paths.
    for( j = 0; j < C.m_IncludeDir.GetCount(); j++ )
    {
        sprintf( Buffer, " -I\"%s\"", UseOnlySlashes(C.m_IncludeDir[j]) );
        strcat ( CmdLine, Buffer );
    }

    // Add the defines to the command line.
    for( j = 0; j < C.m_Define.GetCount(); j++ )
    {
        sprintf( Buffer, " -D%s", C.m_Define[j] );
        strcat ( CmdLine, Buffer );
    }

    // Add the XCORE_PATH macro.
    sprintf( Buffer, " -DXCORE_PATH=\\\"%s\\\"", UseOnlySlashes(pBase) );
    strcat ( CmdLine, Buffer );

    // Add the file names.
    sprintf( Buffer, " -c \"%s\" -o \"%s\\%s.obj\"",
                      UseOnlySlashes(C.m_SourceOther[i]),
                      UseOnlySlashes(C.m_OutputPath),
                      FName );
    strcat ( CmdLine, Buffer );

    // Verbose?
    if( g_Verbose || g_EchoCmdLine )
    {
	    DumpCommandLineAndResponseFile( "Linux Assemble",
                                        CmdLine,
                                        NULL );
    }

    // Feedback for the user.
    printf( "%s%s\n", FName, Ext );
    fflush( stdout );

    // Execute command for SN assembler.
    Result = system( CmdLine );
    return( Result );
}

//==============================================================================

static 
int CompileSourceOther( void )
{
    char    Ext[ _MAX_EXT ];
    int     Result = EXIT_SUCCESS;
    int     i;

    for( i = 0; i < C.m_SourceOther.GetCount(); i++ )
    {
        _splitpath( C.m_SourceOther[i], NULL, NULL, NULL, Ext );

        if( stricmp( ".s", Ext ) == 0 )
        {
            Result |= Compile_s_File( i );
        }
        else
        {
            printf( ".\n" );
            printf( "xCL -- ERROR: Do not know how to process file: %s\n", C.m_SourceOther[i] );
            printf( ".\n" );
            fflush( stdout );
        }
    }

    return( Result );
}

//==============================================================================

static 
int Compiler( void )
{
    int Result = EXIT_SUCCESS;

    if( C.m_SourceCode. GetCount() > 0 )  Result |= CompileSourceCode();
    if( C.m_SourceOther.GetCount() > 0 )  Result |= CompileSourceOther();

    return( Result );
}

//==============================================================================

static 
int Linker( void )
{
    int     i;
    int     Result;
    char    Map     [ _MAX_PATH  ];
    char    Drive   [ _MAX_DRIVE ];
    char    Dir     [ _MAX_DIR   ];
    char    FName   [ _MAX_FNAME ];
	char	LinkScriptName[_MAX_FNAME];
    char    Ext     [ _MAX_EXT   ];
    char    CmdLine [ 32784 ];
    char    ResponseFile[32768];
    FILE*   pLRF    = NULL;
    FILE*   pARF    = NULL;

    // Determine response file name and map name.
    _splitpath( C.m_OutputFile, Drive, Dir, FName,          Ext        );
    _makepath ( LinkScriptName, Drive, Dir, FName,			".ld"     );
    _makepath ( Map,            Drive, Dir, FName,          ".map.txt" );

    memset(ResponseFile,0,sizeof(ResponseFile));
    // Open the response file.
    pLRF = fopen( LinkScriptName, "wt" );
    if( pLRF == NULL )
    {
        printf( "xCL -- ERROR: Unable to open file for write: %s\n", LinkScriptName );
        return( 0 );
    }

	fprintf(pLRF,"SEARCH_DIR(%s/3rdparty/linux/lib)\n",pBase);
    // Add library paths.
    for( i = 0; i < C.m_LibraryDir.GetCount(); i++ )                       
        fprintf(pLRF,"SEARCH_DIR(%s)\n", UseOnlySlashes(C.m_LibraryDir[i]) );

	fprintf(pLRF,"GROUP(\n");
	for (i = 0; i < C.m_LibraryFile.GetCount(); i++ )
	{
		fprintf(pLRF,"      %s\n",C.m_LibraryFile[i]);
	}
	fprintf(pLRF,"      %s/3rdparty/linux/lib/crti.o\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/crtbegin.o\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/crtend.o\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/crtn.o\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/crt1.o\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/libm.a\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/libstdc++.a\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/libgcc.a\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/libc.a\n",pBase);
	fprintf(pLRF,"      %s/3rdparty/linux/lib/libpthread.a\n",pBase);

#if 0
	fprintf(pLRF,")\n");

	fprintf(pLRF,"INPUT(\n");
#endif
	for (i = 1; i < C.m_ObjectFile.GetCount(); i++ )
	{
		fprintf(pLRF,"      %s\n",UseOnlySlashes(C.m_ObjectFile[i]));
	}

	fprintf(pLRF,")\n");

	fprintf(pLRF, "INCLUDE linux.ld");

    //
    // Write stuff to the response file.
    //
    // Add standard library paths.
	//sprintf(ResponseFile+strlen(ResponseFile)," -specs=%%X_LIB%%/linux.specs");
	//sprintf(ResponseFile+strlen(ResponseFile)," -nostdlib");
	sprintf(ResponseFile+strlen(ResponseFile)," ");
	sprintf(ResponseFile+strlen(ResponseFile)," -T %s -nostdlib",LinkScriptName);
    // Add any custom link options.
    if( C.m_LinkOptions[0] != '\0' )
        sprintf(ResponseFile+strlen(ResponseFile), " %s", C.m_LinkOptions );


    
    sprintf(ResponseFile+strlen(ResponseFile), " -Wl,-Map=map.txt -o %s", C.m_OutputFile);

	sprintf(ResponseFile+strlen(ResponseFile)," %s",C.m_ObjectFile[0]);

    //
    // Done with response file.
    //
    sprintf( CmdLine, "gcc-linux.exe %s\n", ResponseFile );

    fclose(pLRF);
    // Extra verbage?
    if( g_Verbose || g_EchoCmdLine )
    {
	    DumpCommandLineAndResponseFile( "Linux Link",
                                        CmdLine,
                                        LinkScriptName );
    }

    fflush( stdout );

   // Execute the command line.
    Result = system( CmdLine );

    // We're done.
    return( Result );
}

//==============================================================================

static 
int Librarian( void )
{
    int     Result;
    int     i;
    FILE*   pLRF;
    char    Name     [ _MAX_PATH  ];
    char    Drive    [ _MAX_DRIVE ];
    char    Dir      [ _MAX_DIR   ];
    char    FName    [ _MAX_FNAME ];
    char    Ext      [ _MAX_EXT   ];
    char    CmdLine  [ 32784      ];

    // We must delete the library before we create the new one.
    sprintf( CmdLine, "if exist %s del %s > nul", 
                      C.m_OutputFile, 
                      C.m_OutputFile );
    system ( CmdLine );

    // Determine response file name and map name.
    _splitpath( C.m_OutputFile, Drive, Dir, FName,         Ext    );
    _makepath ( Name,           Drive, Dir, "_Linux_Lib", ".txt" );

    // Build the command line.
    sprintf( CmdLine, "ar -M < \"%s\"", Name );

    // Open the response file.
    pLRF = fopen( Name, "wt" );
    if( pLRF == NULL )
    {
        printf( "xCL -- ERROR: Unable to open file for write: %s\n", Name );
        return( 0 );
    }

    //
    // Write stuff to the response file.
    //
    
    // Create the archive.
    fprintf( pLRF, "create %s\n", UseOnlySlashes( C.m_OutputFile ) );

    // Add all object modules to it.
    for( i = 0; i < C.m_ObjectFile.GetCount(); i++ )
    {
        fprintf( pLRF, "addmod %s\n", UseOnlySlashes( (char*)C.m_ObjectFile[i] ) );
    }

    // Add all libraries to it.
    for( i = 0; i < C.m_LibraryFile.GetCount(); i++ )
    {
        fprintf( pLRF, "addlib %s\n", UseOnlySlashes( (char*)C.m_LibraryFile[i] ) );
    }

    // That's it for the Librarian Response File.
    fprintf( pLRF, "save\n" );
    fprintf( pLRF, "end \n" );

    // Close the response file.
    fclose( pLRF );

    //
    // Done with response file.
    //

    // Extra verbage?
    if( g_Verbose || g_EchoCmdLine )
    {
	    DumpCommandLineAndResponseFile( "Linux Librarian",
                                        CmdLine,
                                        Name );
    }

    // A little more standard verbage...
    printf( "%s%s\n", FName, Ext );
    fflush( stdout );

    // Execute the command line.
    Result = system( CmdLine );

    // We're done.
    return( Result );
}

//==============================================================================

int Process_Linux( void )
{
    int Result = EXIT_FAILURE;

    // Warn user about any unknown parameters.
    {   
        int i;
        for( i = 0; i < C.m_UnknownOption.GetCount(); i++ )
            printf( "xCL -- Warning: Option not processed: %s\n", C.m_UnknownOption[i] );
    }
    
    // Self explanitory.
    if( !PrepareEnvironment() )
        return( Result );

    // Based on the desired tool, call a function to deal with it.
    switch( C.m_Tool )
    {
    case TOOL_COMPILER:     Result = Compiler ();  break;
    case TOOL_LINKER:       Result = Linker   ();  break;
    case TOOL_LIBRARIAN:    Result = Librarian();  break;
    default:                
        printf( "xCL -- ERROR: Unable to determine tool for Linux operation.\n" );
        break;
    }

    // We're done.
    return( Result );
}

//==============================================================================
