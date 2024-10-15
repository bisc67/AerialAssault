#include "Entropy.hpp"
#include "ShapeManager.hpp"
#include "ShapeInstance.hpp"


//==============================================================================
// Globals
//==============================================================================

static X_FILE*  s_LogFile = NULL ;
static s32      s_UpdateScreenCount = 0 ;

//==============================================================================

void ShapeBuilderPrintf( const char* String )
{
    // Print to screen
    x_printf(String) ;

    // Update screen every x printfs
    if (++s_UpdateScreenCount == 40)
    {
        s_UpdateScreenCount = 0 ;
        eng_PageFlip() ;
    }

    // Print to log file
    if (s_LogFile)
        x_fprintf(s_LogFile, String) ;

    //x_MemSanity() ;

}

//==============================================================================

xbool ShapeBuilderAbortVector( const char*  pFile, 
                              s32           Line, 
                              const char*   pExpStr, 
                              const char*   pMsgStr )
{
    shape::printf( "ASSERT FAIL!\n" );

    if( pFile ) 
        shape::printf( "File: %s(%d)\n", pFile, Line );

    if( pExpStr )   
        shape::printf( "Expr: %s\n", pExpStr );

    if( pMsgStr )   
        shape::printf( "Msg : %s\n", pMsgStr );

    shape::printf( "\n\nPress A, or Esc, or Space to abort, press C to continue at your own risk!") ;

    // Wait for keys to be pressed...
    while(1)
    {
        // Show assert message
        eng_PageFlip() ;
        eng_PageFlip() ;

        // Ready?
        while(input_UpdateState())
        {
            // Abort?
            if (        (input_WasPressed(INPUT_KBD_A))
                    ||  (input_WasPressed(INPUT_KBD_ESCAPE))
                    ||  (input_WasPressed(INPUT_KBD_SPACE)) )
            {
                // Close log file
                if (s_LogFile)
                {
                    x_fclose(s_LogFile) ;
                    s_LogFile = NULL ;
                }

                // Quit program - so messed up shape binaries are not written out!!!
                exit(1) ;
            }

            // Continue?
            if (input_WasPressed(INPUT_KBD_C))
                return FALSE ;
        }
    }

    // Should never get here...
    return( TRUE );
}

//==============================================================================


void AppMain( s32 argc, char* argv[] )
{
    char            Drive   [X_MAX_DRIVE];
    char            Dir     [X_MAX_DIR];
    char            FName   [X_MAX_FNAME];
    char            Ext     [X_MAX_EXT];
    char            LogPath [X_MAX_PATH];

    s32             ScreenWidth ;
    s32             ScreenHeight ;
    s32             XBorderWidth ;
    s32             YBorderWidth ;
    s32             CharacterWidth ;
    s32             CharacterHeight ;
    s32             NScrollLines ;

    shape_manager   ShapeManager ;

    (void)argc ;


    // Init engine
    eng_Init();

    // Set text to use full screen
    text_GetParams(ScreenWidth,
                   ScreenHeight,
                   XBorderWidth,
                   YBorderWidth,
                   CharacterWidth,
                   CharacterHeight,
                   NScrollLines) ;

    eng_GetRes(ScreenWidth, ScreenHeight) ;

    text_SetParams(ScreenWidth,
                   ScreenHeight,
                   XBorderWidth,
                   YBorderWidth,
                   CharacterWidth,
                   CharacterHeight,
                   ScreenHeight / CharacterHeight) ;

    text_ClearBuffers();


#if 1
    
    // Redirect output
    shape::SetPrintfFunc(ShapeBuilderPrintf) ;

    // Redirect assert
    x_SetRTFHandler( ShapeBuilderAbortVector ) ;

    // Show file name
    char* TextPath = argv[1] ;  // skip [0] since that is the .exe

#else

    // Redirect output
    shape::SetPrintfFunc(shape::DefaultPrintf) ;

    //char* TextPath = "C:/Projects/T2/Demo1/Data/Shapes/BuildSingleShape.txt" ;
    //char* TextPath = "C:/Projects/T2/Demo1/Data/Shapes/GameShapes.txt" ;
    char* TextPath = "C:/Projects/T2/Demo1/Data/Shapes/LushShapes.txt" ;
    //char* TextPath = "C:/Projects/T2/Demo1/Data/Shapes/DesertShapes.txt" ;
    //char* TextPath = "C:/Projects/T2/Demo1/Data/Shapes/IceShapes.txt" ;
    //char* TextPath = "C:/Projects/T2/Demo1/Data/Shapes/LavaShapes.txt" ;

#endif

    // File there?
    if (TextPath)
    {
        // Create logging file
        x_splitpath ( TextPath, Drive, Dir, FName, Ext ) ;
        x_makepath  ( LogPath, Drive, Dir, FName, ".log" ) ;
        s_LogFile = x_fopen(LogPath, "wt") ;    

        // Show struc size info
        shape::printf("Class sizes\n") ;
#define PRINT_STRUC_SIZE(__s__) shape::printf("sizeof(%s) = %d bytes\n", #__s__, sizeof(__s__)) ;

        PRINT_STRUC_SIZE(shape_bin_file_class) ;
        PRINT_STRUC_SIZE(anim) ;
        PRINT_STRUC_SIZE(key_set) ;
        PRINT_STRUC_SIZE(hot_point) ;
        PRINT_STRUC_SIZE(material) ;
        PRINT_STRUC_SIZE(material_dlist) ;
        PRINT_STRUC_SIZE(model) ;
        PRINT_STRUC_SIZE(node) ;
        PRINT_STRUC_SIZE(shape) ;
        PRINT_STRUC_SIZE(shape_texture_ref) ;
        PRINT_STRUC_SIZE(shape_instance) ;
        shape::printf("\n\n") ;

        // Build PC
        shape::printf("Building PC shapes from %s\n", TextPath) ;
        ShapeManager.AddAndLoadShapes(TextPath,                             // Filename
                                      shape_manager::MODE_BUILD_BINARY,     // Mode
                                      shape_bin_file_class::PC) ;           // Target
        ShapeManager.DeleteAllShapes() ;


        // Build PS2
        shape::printf("\n\n\n\n\n\n\n\n\n\n") ;
        shape::printf("Building PS2 shapes from %s\n", TextPath) ;
        ShapeManager.AddAndLoadShapes(TextPath,                             // Filename
                                      shape_manager::MODE_BUILD_BINARY,     // Mode
                                      shape_bin_file_class::PS2) ;          // Target
        ShapeManager.DeleteAllShapes() ;


        // Close log file
        if (s_LogFile)
            x_fclose(s_LogFile) ;
    }
}

