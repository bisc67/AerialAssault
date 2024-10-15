//=========================================================================
//
// Config.CPP
//
//=========================================================================
#include "config.hpp"
#include "Entropy.hpp"
#include "Tokenizer/Tokenizer.hpp"



//=========================================================================
// Variables
//=========================================================================

const char      *g_Version = "5.0" ;
const char      *g_ConfigFilename = "ShapeViewer.cfg" ;

// temp for sony visit!
//const char      *g_ConfigFilename = "P:/Art/sony visit/ShapeViewer.cfg" ;


shape           *g_Shapes[256] ;
s32             g_nShapes         = 0 ;
shape_instance  *g_Instances[256] ;
s32             g_nInstances      = 0 ;
s32             g_CurrentInstance = 0 ;
f32             g_PlaybackSpeed   = 1.0f ;
s32             g_ShowHelp        = 1 ;
s32             g_ShowStats       = 1 ;
texture         g_BackdropTexture ;
s32             g_RemoveRootTrans = TRUE ;
vector3         g_BackgroundCol = vector3(125.0f/255.0f, 125.0f/255.0f, 255.0f/255.0f) ;
NetworkPath     g_NetworkPaths[256] ;
s32             g_nNetworkPaths = 0 ;   
vector3         g_vLightDir = vector3(0,0,-1) ;
s32             g_BuildMips = 0 ;
texture_manager g_TextureManager ;
s32             g_TextureFrame = 0 ;


//=========================================================================
// Loads config file


// Cleans up memory
void ConfigCleanup()
{
    s32 i ;

    // Clear screen of text
    for (i = 0 ; i < 100 ; i++)
        x_printf("\n") ;

    // Delete old models and instances
    for (i = 0 ; i < g_nShapes ; i++)
    {
        if (g_Shapes[i])
        {
            delete g_Shapes[i] ;
            g_Shapes[i] = NULL ;
        }
    }
    g_nShapes = 0 ;

    // Delete old instances
    for (i = 0 ; i < g_nInstances ; i++)
    {
        if (g_Instances[i])
        {
            delete g_Instances[i] ;
            g_Instances[i] = NULL ;
        }
    }
    g_nInstances = 0 ;

    // Clear network paths
    g_nNetworkPaths = 0 ;   

    // Clear background pic
    g_BackdropTexture.Kill() ;

    // Delete all textures
    g_TextureManager.DeleteAllTextures() ;

    // Clear texture paths
    g_TextureManager.DeleteAllSearchPaths() ;
}



// Parses config file, then re-loads the model
void ConfigParseFile()
{
    s32 i ;

    // Delete current models and instances
    ConfigCleanup() ;
       
    // Read config file
    token_stream	Tok ; 
	if (Tok.OpenFile(g_ConfigFilename))
    {
        // Success
        x_DebugMsg("Parsing config file %s\n", g_ConfigFilename) ;   

        // 1st pass extracts paths etc
        for (s32 Pass = 0 ; Pass < 2 ; Pass++)
        {
            // Jump to front of file
            Tok.Rewind() ;

            // Done yet?
            while(Tok.Type() != token_stream::TOKEN_EOF)
            {
                // Get next token
                Tok.Read() ;
                char *Token = Tok.String() ;

                // Which pass?
                switch(Pass)
                {
                    // Grab important paths etc
                    case 0:

                        // Texture path?
                        if (x_strcmp(Token, "TexturePath") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            // Grab strings
                            g_TextureManager.AddSearchPath(Tok.ReadLine()) ;
                        }
                        else
                        // Network path? (NOTE - THIS MUST BE FIRST BEFORE OPENING OTHER FILES!!!)
                        if (x_strcmp(Token, "NetworkPath") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            // Grab strings
                            x_strcpy(g_NetworkPaths[g_nNetworkPaths].Drive, Tok.ReadSymbol()) ;
                            x_strcpy(g_NetworkPaths[g_nNetworkPaths].Path, Tok.ReadLine()) ;

                            // Make upper ready for string compare
                            x_strtoupper(g_NetworkPaths[g_nNetworkPaths].Drive) ;
                            x_strtoupper(g_NetworkPaths[g_nNetworkPaths].Path) ;

                            // Update total
                            g_nNetworkPaths++ ;
                        }
                        else
                        // BuildMips?
                        if (x_strcmp(Token, "BuildMips") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            g_BuildMips = Tok.ReadInt() ;
                        }
                        break ;

                    // Load data
                    case 1:

                        // ModelFile?
                        if (x_strcmp(Token, "ModelFile") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            // Load new model (no local, build mips, no load xbm file, load MDL file)
                            shape *pShape = new shape() ;
                            ASSERT(pShape) ;

                            pShape->SetTextureManager(&g_TextureManager) ;

                            char *ModelFiles[2] ;
                            char *AnimFiles [2] ;

                            ModelFiles[0] = AnimFiles[0] = Tok.ReadLine() ;
                            ModelFiles[1] = NULL ;
                            AnimFiles [1] = NULL ;

                            // Let's create a path from the model file
                            char    Drive     [X_MAX_DRIVE];
                            char    Dir       [X_MAX_DIR];
                            char    SearchPath[X_MAX_PATH];
                            x_splitpath(ModelFiles[0],Drive,Dir,NULL,NULL);
                            x_makepath(SearchPath, Drive, Dir, NULL, NULL) ;
                            g_TextureManager.AddSearchPath(SearchPath) ;

                            // Loads shape, models, and animations
	                        pShape->LoadFromAsciiFiles((const char**)ModelFiles, 
                                                       (const char**)AnimFiles,
                                                       NULL,    // LabelSet
                                                       NULL,    // ShapeFile
                                                       FALSE,   // BuildMips
                                                       FALSE) ; // Check for xbm
                            // Add to list
                            if (pShape)
                                g_Shapes[g_nShapes++] = pShape ;
                        }
                        else                            
                        // PlaybackSpeed?
                        if (x_strcmp(Token, "PlaybackSpeed") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            g_PlaybackSpeed = Tok.ReadFloat() ;
                        }
                        else
                        // ShowHelp?
                        if (x_strcmp(Token, "ShowHelp") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            g_ShowHelp = Tok.ReadInt() ;
                        }
                        else
                        // ShowStats?
                        if (x_strcmp(Token, "ShowStats") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            g_ShowStats = Tok.ReadInt() ;
                        }
                        else
                        // BackgroundPic?
                        if (x_strcmp(Token, "BackgroundPic") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            // Try load bitmap
                            g_BackdropTexture.SetName(Tok.ReadLine()) ;
                            g_BackdropTexture.Load(g_BackdropTexture.GetName(),
                                                   FALSE,       // no mips
                                                   FALSE,       // no xbmp
                                                   shape_bin_file_class::GetDefaultTarget()) ; // target
                        }
                        else
                        // RemoveRootTrans?
                        if (x_strcmp(Token, "RemoveRootTrans") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            g_RemoveRootTrans = Tok.ReadInt() ;
                        }
                        else
                        // LightDir?
                        if (x_strcmp(Token, "LightDir") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            g_vLightDir = vector3(0,0,1) ;
                            g_vLightDir.X = Tok.ReadFloat() ;
                            g_vLightDir.Y = Tok.ReadFloat() ;
                            g_vLightDir.Z = Tok.ReadFloat() ;
                            g_vLightDir.Normalize() ;
                        }
                        else
                        // LightCol?
                        if (x_strcmp(Token, "LightCol") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            vector4 V=vector4(1,1,1,1) ;
                            V.X = Tok.ReadFloat() ;
                            V.Y = Tok.ReadFloat() ;
                            V.Z = Tok.ReadFloat() ;
                            V.W = Tok.ReadFloat() ;
                            shape::SetLightColor(V) ;
                        }
                        else
                        // LightAmb?
                        if (x_strcmp(Token, "LightAmb") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            vector4 V=vector4(0.3f, 0.3f, 0.3f, 0.0f) ;
                            V.X = Tok.ReadFloat() ;
                            V.Y = Tok.ReadFloat() ;
                            V.Z = Tok.ReadFloat() ;
                            V.W = Tok.ReadFloat() ;
                            shape::SetLightAmbientColor(V) ;
                        }
                        else
                        // BackgroundCol?
                        if (x_strcmp(Token, "BackgroundCol") == 0)
                        {
                            VERIFY(Tok.Read() == token_stream::TOKEN_DELIMITER) ; // Skip =

                            g_BackgroundCol.X = 1.0f ;
                            g_BackgroundCol.Y = 1.0f ;
                            g_BackgroundCol.Z = 1.0f ;

                            g_BackgroundCol.X = Tok.ReadFloat() ;
                            g_BackgroundCol.Y = Tok.ReadFloat() ;
                            g_BackgroundCol.Z = Tok.ReadFloat() ;
                        }
                        break ;

                }
            }
        }

        // Close the file
        Tok.CloseFile() ;

        // Any shapes?
        if (g_nShapes)
        {
            // Find biggest shape
            f32 Radius = 0 ;
            for (i = 0 ; i < g_nShapes ; i++)
                Radius = MAX(Radius, g_Shapes[i]->GetModel(0).GetBounds().GetRadius()) ;

            // Scale all instances so most of viewport is used up
            f32 Scale = 30.0f / Radius ;
            //Scale=25.0f;

            // Create an instance per shape
            for (i = 0 ; i < g_nShapes ; i++)
            {
                // Create new instance
                shape_instance *pInstance = new shape_instance(g_Shapes[i]) ;
                if (pInstance)                    
                {
                    pInstance->SetScale(vector3(Scale, Scale, Scale)) ;
                    pInstance->SetRot(radian3(0,R_180,0)) ;
                    pInstance->SetPos(vector3(0,-1*Scale,0)) ;
                    //pInstance->SetPos(-pShape->GetModel(0).GetBounds().GetCenter() * Scale ) ;
                    g_Instances[g_nInstances++] = pInstance ;
                }
            }
        }
    }
    else
    {
        // Error!        
        x_DebugMsg("Config file %s not found!\n", g_ConfigFilename) ;   
    }

    // for now until i find the reflection bug!
    //shape::SetDrawListEnabled(FALSE) ;

    // Assign material id's ready for drawing
    shape::AssignMaterialIDs(g_Shapes, g_nShapes) ;

    // Clear screen of text
    for (i = 0 ; i < 100 ; i++)
        x_printf("\n") ;

    // Display missing textures
    for (i = 0 ; i < g_TextureManager.GetMissingTextureCount() ; i++)
        x_printf("MissingTexture:\n%s\n\n", g_TextureManager.GetMissingTexture(i)) ;
}

//=========================================================================

// Patches filename with network name if found since UHC filenames do not work!
open_fn*  OpenFnPtr = NULL ;
X_FILE* VIEWER_Open( const char* pFilename, const char* pMode )
{
    char PatchedFilename[256] ;
    
    // Copy filename and make uppercase ready for testing with
    x_strcpy(PatchedFilename, pFilename) ;
    x_strtoupper(PatchedFilename) ;

    // Check all network paths
    for (s32 i = 0 ; i < g_nNetworkPaths ; i++)
    {
        NetworkPath &NP = g_NetworkPaths[i] ;
        s32 PathLength = x_strlen(NP.Path) ;

        // Found match?
        if (x_strncmp(PatchedFilename, NP.Path, PathLength) == 0)
        {
            // Copy drive letter
            x_strcpy(PatchedFilename, NP.Drive) ;

            // Copy rest of filename, skipping drive path
            x_strcat(PatchedFilename, &pFilename[PathLength]) ;

            // Found!
            break ;
        }
    }

    // Finally call function to do the work
    ASSERT(OpenFnPtr) ;
    return OpenFnPtr(PatchedFilename, pMode) ;
}


//=========================================================================


//typedef xbool rtf_fn( const char* pFileName,
                      //s32         TokenNumber,
                      //const char* pExprString,
                      //const char* pMessageString );


xbool ViewAbortVector( const char* pFile, 
                         s32   Token, 
                         const char* pExpStr, 
                         const char* pMsgStr )
{
    x_printf( "ASSERT FAIL!\n" );

    if( pFile ) 
        x_printf( "File: %s(%d)\n", pFile, Token );

    if( pExpStr )   
        x_printf( "Expr: %s\n", pExpStr );

    if( pMsgStr )   
        x_printf( "Msg : %s\n", pMsgStr );

    x_printf( "Press X or Space to attempt to continue...\n\n") ;

    x_printf("\n") ;

    // Draw printf
    eng_PageFlip() ;
    eng_PageFlip() ;
    eng_PageFlip() ;

    // Wait for keys to be pressed...
    while(1)
    {
        // Ready?
        while(input_UpdateState())
        {
            // Continue?
            if ((input_WasPressed(INPUT_PS2_BTN_CROSS)) || (input_WasPressed(INPUT_KBD_SPACE))) 
                return FALSE ;
        }
    }

    // Should never get here...
    return( TRUE );
}


//=========================================================================

void ShapeViewerPrintf( const char* String )
{
    static s32 s_UpdateScreenCount = 0 ;

    // Print to screen
    x_printf(String) ;

    // Update screen every x printfs
    if (++s_UpdateScreenCount == 20)
    {
        s_UpdateScreenCount = 0 ;
        eng_PageFlip() ;
    }
}

//=========================================================================


void ConfigInit()
{
    // Override fopen so we can patch in the network name
    close_fn    *CloseFnPtr ;
    read_fn     *ReadFnPtr ;
    write_fn    *WriteFnPtr ;
    seek_fn     *SeekFnPtr ;
    tell_fn     *TellFnPtr ;
    flush_fn    *FlushPtr ;
    eof_fn      *EOFFnPtr ;
    length_fn   *LengthPtr ;

    x_GetFileIOHooks  (OpenFnPtr,
                       CloseFnPtr,
                       ReadFnPtr,
                       WriteFnPtr,
                       SeekFnPtr,
                       TellFnPtr,
                       FlushPtr,
                       EOFFnPtr,
                       LengthPtr) ;

    x_SetFileIOHooks  (VIEWER_Open,
                       CloseFnPtr,
                       ReadFnPtr,
                       WriteFnPtr,
                       SeekFnPtr,
                       TellFnPtr,
                       FlushPtr,
                       EOFFnPtr,
                       LengthPtr) ;

    #ifdef TARGET_PS2_CLIENT
        // Override assert function to get more info...
        x_SetRTFHandler( ViewAbortVector ) ;
    #endif


    // Set text to use full screen
    s32             ScreenWidth ;
    s32             ScreenHeight ;
    s32             XBorderWidth ;
    s32             YBorderWidth ;
    s32             CharacterWidth ;
    s32             CharacterHeight ;
    s32             NScrollLines ;
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

    // Redirect output
    shape::SetPrintfFunc(ShapeViewerPrintf) ;

    // Draw printf
    eng_PageFlip() ;
    eng_PageFlip() ;
    eng_PageFlip() ;
}


//=========================================================================
