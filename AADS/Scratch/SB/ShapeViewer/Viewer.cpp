//=========================================================================
//
// TEST0.CPP
//
//=========================================================================
#include "Entropy.hpp"

#include "Shape/Shape.hpp"
#include "Shape/ShapeInstance.hpp"
#include "Config.hpp"
#include "Entropy.hpp"
#include "../support/netlib/netlib.hpp"
#include "e_netfs.hpp"


//=========================================================================
// File server defines
//=========================================================================

#ifdef TARGET_PS2_CLIENT
    #define USE_FILE_SERVER
#endif



//=========================================================================
// Defines
//=========================================================================

// PS2 CONTROLLER
#define PS2_TOGGLE_HELP_KEY         INPUT_PS2_BTN_CROSS
#define PS2_TOGGLE_STATS_KEY        INPUT_PS2_BTN_CIRCLE
#define PS2_RESET_CAMERA_KEY        INPUT_PS2_BTN_SQUARE
#define PS2_TOGGLE_ROOT_TRANS_KEY   INPUT_PS2_BTN_TRIANGLE

#define PS2_CAMERA_LEFT_KEY         INPUT_PS2_BTN_L_LEFT
#define PS2_CAMERA_RIGHT_KEY        INPUT_PS2_BTN_L_RIGHT
#define PS2_CAMERA_UP_KEY           INPUT_PS2_BTN_L_UP
#define PS2_CAMERA_DOWN_KEY         INPUT_PS2_BTN_L_DOWN
#define PS2_CAMERA_FORWARD_KEY      INPUT_PS2_BTN_L1
#define PS2_CAMERA_BACKWARD_KEY     INPUT_PS2_BTN_L2

#define PS2_CAMERA_ROT_Z_KEY        INPUT_PS2_STICK_RIGHT_X        
#define PS2_CAMERA_ROT_X_KEY        INPUT_PS2_STICK_RIGHT_Y        
#define PS2_CAMERA_ROT_Y_KEY        INPUT_PS2_STICK_LEFT_X        
#define PS2_CAMERA_ROT_X2_KEY       INPUT_PS2_STICK_LEFT_Y        

#define PS2_PREV_MODEL_KEY          INPUT_PS2_BTN_R1
#define PS2_NEXT_MODEL_KEY          INPUT_PS2_BTN_R2
#define PS2_RELOAD_ALL_KEY          INPUT_PS2_BTN_START
#define PS2_RELOAD_TEXTURES_KEY     INPUT_PS2_BTN_SELECT

#define PS2_CAMERA_SNAPSHOT_KEY     INPUT_PS2_BTN_L_STICK

#define PS2_NEXT_TEXTURE_KEY        INPUT_PS2_BTN_R_STICK



// KEYBOARD
#define KBD_TOGGLE_HELP_KEY         INPUT_KBD_H
#define KBD_TOGGLE_STATS_KEY        INPUT_KBD_S
#define KBD_RESET_CAMERA_KEY        INPUT_KBD_C
#define KBD_TOGGLE_ROOT_TRANS_KEY   INPUT_KBD_R

#define KBD_CAMERA_LEFT_KEY         INPUT_KBD_LEFT
#define KBD_CAMERA_RIGHT_KEY        INPUT_KBD_RIGHT
#define KBD_CAMERA_UP_KEY           INPUT_KBD_UP
#define KBD_CAMERA_DOWN_KEY         INPUT_KBD_DOWN
#define KBD_CAMERA_FORWARD_KEY      INPUT_KBD_8
#define KBD_CAMERA_BACKWARD_KEY     INPUT_KBD_2

#define KBD_CAMERA_ROT_Z_KEY        INPUT_KBD_Z
#define KBD_CAMERA_ROT_X_KEY        INPUT_KBD_X
#define KBD_CAMERA_ROT_Y_KEY        INPUT_KBD_Y
#define KBD_CAMERA_ROT_X2_KEY       INPUT_KBD_W

#define KBD_PREV_MODEL_KEY          INPUT_KBD_PRIOR
#define KBD_NEXT_MODEL_KEY          INPUT_KBD_NEXT
#define KBD_RELOAD_ALL_KEY          INPUT_KBD_RETURN
#define KBD_RELOAD_TEXTURES_KEY     INPUT_KBD_T

#define KBD_QUIT_KEY                INPUT_KBD_ESCAPE

#define KBD_NEXT_TEXTURE_KEY        INPUT_KBD_SPACE




static xbool Quit = FALSE ;
static f32   NextTextureKeyTime=0 ;
static xbool AnimateTextures=FALSE ;

//=========================================================================
// Variables
//=========================================================================

// Misc vars
static s32 Frame = 0 ;

// Camera vars
static view         g_View;
static quaternion   g_qCamera ;
static vector3      g_vCamera ;


#ifdef TARGET_PS2

void    eng_GetStats(s32 &Count, f32 &CPU, f32 &GS, f32 &INT, f32 &FPS) ;

#endif




//=========================================================================

void InitView( void )
{
    // Init view
    g_View.SetZLimits(0.01f, 1000.0f) ;
    g_qCamera.Identity() ;
    g_vCamera = vector3(0.0f, 0.0f, -100.0f) ;
}

//=========================================================================

void Update( f32 DeltaTime )
{
    s32 i ;

    // Process pad
    f32 Speed = 2.0f ;

    while( input_UpdateState() )
    {
        #ifdef TARGET_PC

            if (input_IsPressed( KBD_QUIT_KEY ))
                Quit = TRUE ;

            if ( input_WasPressed(INPUT_MSG_EXIT))
                Quit = TRUE ;
        #endif

        if (g_nInstances)
        {
            // Next model?
            if ( (input_WasPressed(PS2_NEXT_MODEL_KEY)) || (input_WasPressed(KBD_NEXT_MODEL_KEY)) )
            {
                do
                {
                    g_CurrentInstance++ ;
                    if (g_CurrentInstance >= g_nInstances)
                        g_CurrentInstance = 0 ;
                }
                while(g_Instances[g_CurrentInstance] == NULL) ;
            }

            // Previous model?
            if ( (input_WasPressed(PS2_PREV_MODEL_KEY)) || (input_WasPressed(KBD_PREV_MODEL_KEY)) )
            {
                do
                {
                    g_CurrentInstance-- ;
                    if (g_CurrentInstance < 0)
                        g_CurrentInstance = g_nInstances-1 ;
                }
                while(g_Instances[g_CurrentInstance] == NULL) ;
            }

            // Update texture key pressed time
            if ( (input_IsPressed(PS2_NEXT_TEXTURE_KEY)) || (input_IsPressed(KBD_NEXT_TEXTURE_KEY)) )
                NextTextureKeyTime += DeltaTime ;
            else
                NextTextureKeyTime = 0 ;

            // Turn on animating textures if key is held down?
            AnimateTextures = (NextTextureKeyTime > 1) ;

            // Next texture?
            if (!AnimateTextures)
            {
                // Next texture?
                if ( (input_WasPressed(PS2_NEXT_TEXTURE_KEY)) || (input_WasPressed(KBD_NEXT_TEXTURE_KEY)) )
                    g_TextureFrame++ ;
            }
        }

        // Process pad
        if ( (input_WasPressed(PS2_RELOAD_ALL_KEY)) || (input_WasPressed(KBD_RELOAD_ALL_KEY)) )   // reload everything?
        {
            // Wait for drawing to finish
            eng_PageFlip();
            eng_PageFlip();
            eng_PageFlip();

            // Reload model and textures
            ConfigParseFile() ;
        }
        else
        if ( (input_WasPressed(PS2_RELOAD_TEXTURES_KEY)) || (input_WasPressed(KBD_RELOAD_TEXTURES_KEY)) )   // reload textures?
        {
            // Reload backdrop
            g_BackdropTexture.Load(g_BackdropTexture.GetName(), FALSE, FALSE, shape_bin_file_class::GetDefaultTarget()) ;

            // Loop through all models
            for (i = 0 ; i < g_nShapes ; i++)
            {
                if (g_Shapes[i])
                    g_Shapes[i]->LoadTextures(FALSE,    // BuildMips
                                              FALSE,    // BuildXBMS 
                                              shape_bin_file_class::GetDefaultTarget(), // Target
                                              TRUE) ;   // BurnTextures

            }

            // Clear screen of text
            for (i = 0 ; i < 100 ; i++)
                x_printf("\n") ;

            // Display missing textures
            for (i = 0 ; i < g_TextureManager.GetMissingTextureCount() ; i++)
                x_printf("MissingTexture:\n%s\n\n", g_TextureManager.GetMissingTexture(i)) ;
        }
        else
        if ( (input_WasPressed(PS2_TOGGLE_HELP_KEY)) || (input_WasPressed(KBD_TOGGLE_HELP_KEY)) )
        {
            // Toggle help
            g_ShowHelp ^= 1 ;

            // Clear screen of text
            for (i = 0 ; i < 100 ; i++)
                x_printf("\n") ;
        }
        else
        if ( (input_WasPressed(PS2_TOGGLE_STATS_KEY)) || (input_WasPressed(KBD_TOGGLE_STATS_KEY)) )
        {
            // Toggle stats
            g_ShowStats ^= 1 ;
        }
        else
        if ( (input_WasPressed(PS2_RESET_CAMERA_KEY)) || (input_WasPressed(KBD_RESET_CAMERA_KEY)) )
        {
            // Reset camera
            InitView() ;
        }
        else
        if ( (input_WasPressed(PS2_TOGGLE_ROOT_TRANS_KEY)) || (input_WasPressed(KBD_TOGGLE_ROOT_TRANS_KEY)) )
        {
            // Toggle root translation
            g_RemoveRootTrans ^= TRUE ;
        }
        else
        if( input_WasPressed(PS2_CAMERA_SNAPSHOT_KEY) )
        {
            eng_ScreenShot( );
        }

        else
        {
            quaternion qRot ;

            if ( (input_IsPressed(PS2_CAMERA_LEFT_KEY)) || (input_IsPressed(KBD_CAMERA_LEFT_KEY)) )
                g_vCamera += vector3(-Speed*DeltaTime*60, 0, 0) ;

            if ( (input_IsPressed(PS2_CAMERA_RIGHT_KEY)) || (input_IsPressed(KBD_CAMERA_RIGHT_KEY)) )
                g_vCamera += vector3(Speed*DeltaTime*60, 0, 0) ;

            if ( (input_IsPressed(PS2_CAMERA_UP_KEY)) || (input_IsPressed(KBD_CAMERA_UP_KEY)) )
                g_vCamera += vector3(0, -Speed*DeltaTime*60, 0) ;

            if ( (input_IsPressed(PS2_CAMERA_DOWN_KEY)) || (input_IsPressed(KBD_CAMERA_DOWN_KEY)) )
                g_vCamera += vector3(0, Speed*DeltaTime*60, 0) ;

            if ( (input_IsPressed(PS2_CAMERA_FORWARD_KEY)) || (input_IsPressed(KBD_CAMERA_FORWARD_KEY)) )
                g_vCamera += vector3(0, 0, Speed*DeltaTime*60) ;

            if ( (input_IsPressed(PS2_CAMERA_BACKWARD_KEY)) || (input_IsPressed(KBD_CAMERA_BACKWARD_KEY)) )
                g_vCamera += vector3(0, 0, -Speed*DeltaTime*60) ;

            float X,Y ;

            if (input_IsPressed(KBD_CAMERA_ROT_Z_KEY))
                X = 1 ;
            else
                X = input_GetValue(PS2_CAMERA_ROT_Z_KEY) ;
            qRot.Identity() ;
            qRot.RotateZ(-(float)X*0.015f*5.0f*DeltaTime*60) ;
            g_qCamera *= qRot ;

            if (input_IsPressed(KBD_CAMERA_ROT_X_KEY))
                Y = 1 ;
            else
                Y = input_GetValue(PS2_CAMERA_ROT_X_KEY) ;
            qRot.Identity() ;
            qRot.RotateX( -(float)Y*0.015f*5.0f*DeltaTime*60) ;
            g_qCamera *= qRot ;

            if (input_IsPressed(KBD_CAMERA_ROT_Y_KEY))
                X = 1 ;
            else
                X = input_GetValue(PS2_CAMERA_ROT_Y_KEY) ;
            qRot.Identity() ;
            qRot.RotateY(-(float)X*0.015f*5.0f*DeltaTime*60) ;
            g_qCamera *= qRot ;

            if (input_IsPressed(KBD_CAMERA_ROT_X2_KEY))
                Y = 1 ;
            else
                Y = input_GetValue(PS2_CAMERA_ROT_X2_KEY) ;
            qRot.Identity() ;
            qRot.RotateX(-(float)Y*0.015f*5.0f*DeltaTime*60) ;
            g_qCamera *= qRot ;
        }
    }

    xbool LButton=FALSE ;
    xbool RButton=FALSE ;
    xbool MButton=FALSE ;
    f32   X=0.0f ;
    f32   Y=0.0f ;
    f32   W=0.0f ;

    // Mouse input        
    LButton = (input_IsPressed(INPUT_MOUSE_BTN_L) != 0.0f) ;
    RButton = (input_IsPressed(INPUT_MOUSE_BTN_R) != 0.0f) ;
    MButton = (input_IsPressed(INPUT_MOUSE_BTN_C) != 0.0f) ;
    X       = input_GetValue(INPUT_MOUSE_X_REL) ;
    Y       = input_GetValue(INPUT_MOUSE_Y_REL) ;
    W       = input_GetValue(INPUT_MOUSE_WHEEL_REL) ;

    // Process mouse
    if (LButton && RButton)
    {
        InitView() ;
    }
    else
    {
#define MOUSE_SQR(a)     ( (a) < 0 ? -((a)*(a)) : ((a)*(a)) )
#define MOUSE_CUBE(a)    ( (a) * (a) * (a) )

        //X = MOUSE_SQR(X) ;
        //Y = MOUSE_SQR(Y) ;

        Speed = 0.2f * DeltaTime * 60 ;
        
        if (MButton)
            g_vCamera += vector3((float)X*Speed, (float)Y*Speed, 0) ;
        else
            g_vCamera += vector3(0.0f, 0.0f, (float)W*8.0f*DeltaTime*60) ;

        Speed = 0.0080f * DeltaTime * 60 ;

        if (LButton)
        {
            quaternion qRot ;
            qRot.Identity() ;
            qRot.RotateY( -(float)X*Speed) ;
            g_qCamera *= qRot ;
           
            qRot.Identity() ;
            qRot.RotateX( (float)Y*Speed) ;
            g_qCamera *= qRot ;
        }

        if (RButton)
        {
            quaternion qRot ;
            
            qRot.Identity() ;
            qRot.RotateZ( -(float)X*Speed) ;
            g_qCamera *= qRot ;

            qRot.Identity() ;
            qRot.RotateX( (float)Y*Speed) ;
            g_qCamera *= qRot ;
        }
    }

    // Update view with new camera settings
    g_qCamera.Normalize() ;

    matrix4 mRot, mTrans, mWorldOrient ;
    mRot.Identity() ;
    mRot.SetRotation(g_qCamera) ;
    
    mTrans.Identity() ;
    mTrans.Translate(g_vCamera) ;

    mWorldOrient = mRot * mTrans ;
    g_View.SetV2W(mWorldOrient) ;

    // Since we are moving the camera, but want the lighting constant -
    // rotate the light normal against the camera rotation
    matrix4 W2V ;
    W2V = g_View.GetW2V() ;
    W2V.Transpose() ;
    vector3 vRotLightDir = W2V.RotateVector(g_vLightDir) ;
    shape::SetLightDirection(vRotLightDir, TRUE) ;
}

//=========================================================================

void Render( f32 DeltaTime )
{
    static s32 NFrames=0;

    // Print some stats
    NFrames++;

    // Force reloading of textures
    vram_Flush();


    // Begin draw
    eng_Begin();

    // Tell shape library we are about to render a frame
    shape::BeginFrame() ;

    // Set current view
    eng_MaximizeViewport( g_View) ;
    eng_SetView(g_View, 0) ;
    eng_ActivateView ( 0 );

    // Draw background bitmap
    g_BackdropTexture.Draw(0,0) ;

    // Prepare for drawing models (loads microcode etc)
    shape::BeginDraw() ;

    // Render instance
    shape_instance *pInstance = g_Instances[g_CurrentInstance] ;
    if (pInstance)
    {
        shape *pShape = pInstance->GetShape() ;
        if (pShape)
        {
            // Show stats?
            if (g_ShowStats)
            {
                // Show frame stats
                anim_state  &AnimState = pInstance->GetCurrentAnimState() ;
                anim        *pAnim     = AnimState.GetAnim() ;
                if (pAnim)
                {
                    x_printfxy(0,0,"%3d/%d %s", 
                               (s32)AnimState.GetFrame(),
                               pAnim->GetFrameCount(),
                               pShape->GetModel(0).GetName()) ;
                }

                #ifdef TARGET_PS2
                    // Show timer stats
                    s32 Count ;
                    f32 CPU, GS, INT, FPS ;
                    eng_GetStats(Count, CPU, GS, INT, FPS) ;

                    x_printfxy(0,23,"CPU:%4.1f  GS:%4.1f  INT:%4.1f  FPS:%4.1f", CPU, GS, INT, FPS) ;
                #endif
            }

            // Show help
            if (g_ShowHelp)
            {
                #ifdef TARGET_PS2
                    s32 x = 0, y=5 ;
                    x_printfxy(x,y,"        PS2 Shape Viewer v%s         ", g_Version) ;  y++ ;
                    x_printfxy(x,y,"Select       = Reload textures        ") ;  y++ ;
                    x_printfxy(x,y,"Start        = Reload everything      ") ;  y++ ;
                    x_printfxy(x,y,"R1 R2        = Toggle models          ") ;  y++ ;
                    x_printfxy(x,y,"Cross        = Toggle help            ") ;  y++ ;
                    x_printfxy(x,y,"Circle       = Toggle stats           ") ;  y++ ;
                    x_printfxy(x,y,"Triangle     = Toggle root translation") ;  y++ ;
                    x_printfxy(x,y,"Square       = Reset camera           ") ;  y++ ;
                    x_printfxy(x,y,"Arrows       = Move camera            ") ;  y++ ;
                    x_printfxy(x,y,"L1 L2        = Zoom camera            ") ;  y++ ;
                    x_printfxy(x,y,"Sticks       = Rotate camera          ") ;  y++ ;
                    x_printfxy(x,y,"L-Stick-Btn  = Snapshot               ") ;  y++ ;
                    x_printfxy(x,y,"R-Stick-Btn  = Next/animate textures  ") ;  y++ ;
                #endif

                #ifdef TARGET_PC
                    s32 x = 0, y=5 ;
                    x_printfxy(x,y,"        PS2 Shape Viewer v%s         ", g_Version) ;  y++ ;
                    x_printfxy(x,y,"T            = Reload textures        ") ;  y++ ;
                    x_printfxy(x,y,"Return       = Reload everything      ") ;  y++ ;
                    x_printfxy(x,y,"PageUp/Down  = Toggle models          ") ;  y++ ;
                    x_printfxy(x,y,"H            = Toggle help            ") ;  y++ ;
                    x_printfxy(x,y,"S            = Toggle stats           ") ;  y++ ;
                    x_printfxy(x,y,"R            = Toggle root translation") ;  y++ ;
                    x_printfxy(x,y,"C            = Reset camera           ") ;  y++ ;
                    x_printfxy(x,y,"Arrows       = Move camera            ") ;  y++ ;
                    x_printfxy(x,y,"8 and 2      = Zoom camera            ") ;  y++ ;
                    x_printfxy(x,y,"XYZW         = Rotate camera          ") ;  y++ ;
                    x_printfxy(x,y,"Space        = Next/animate textures  ") ;  y++ ;
                    x_printfxy(x,y,"Escape       = Quit                   ") ;  y++ ;
                #endif
            }

            // Calc fog
            xcolor  FogColor = shape::GetFogColor() ;
            FogColor.A = (u8)(255.0f * pInstance->CalculateLinearFog(0.75f)) ;
            pInstance->SetFogColor(FogColor) ;

            // Set texture frame
            if (AnimateTextures)
                pInstance->SetTextureAnimFPS(30) ;
            else
            {
                pInstance->SetTextureAnimFPS(0) ;
                pInstance->SetTextureFrame((f32)g_TextureFrame) ;
            }

            // Draw instance
            pInstance->SetLightDirection    (shape::GetLightDirection()) ;
            pInstance->SetLightColor        (shape::GetLightColor()) ;
            pInstance->SetLightAmbientColor (shape::GetLightAmbientColor()) ;
            pInstance->Draw(shape::DRAW_FLAG_CLIP);// | shape::DRAW_FLAG_FOG) ;
            //pInstance->Draw(shape::DRAW_FLAG_CLIP | shape::DRAW_FLAG_REF_WORLD_SPACE) ;

            // Animate instance
            pInstance->Advance(g_PlaybackSpeed * DeltaTime) ;

        }
    }
    shape::EndDraw() ;


    // Tell shape library render frame is done
    shape::EndFrame() ;

    // End draw
    eng_End() ;

    // Finished rendering, do pageflip
    eng_PageFlip() ;
}

//=========================================================================

void AppMain( s32 argc, char* argv[] )
{
    (void)argc ;
    (void)argv ;

    // Init engine in default mode
    eng_Init();

    #ifdef USE_FILE_SERVER

        // Print server message
        //s32 x = 0, y=5 ;
        //x_printfxy(x,y,"PS2 Shape Viewer v%s", g_Version) ;  y++ ;
        //x_printfxy(x,y,"Searching for file server...") ; y++ ;
        //eng_PageFlip() ;
        //eng_PageFlip() ;

        // Setup file server
        net_Init();
        netfs_Init() ;

        // Found!
        //x_printfxy(x,y,"Found file server!...") ;
        //eng_PageFlip() ;
        //eng_PageFlip() ;
    #endif

    #ifdef TARGET_PC
        d3deng_SetMouseMode(MOUSE_MODE_BUTTONS) ;
    #endif

    // Init view
    InitView() ;

    // Prepare confog
    ConfigInit() ;

    // Load model and textures on first frame
    ConfigParseFile() ;

    // Okay - lets enter the main loop
    while( !Quit )
    {
        // Set background color
        xcolor C(125,125,255) ;
        C.R = (u8) (g_BackgroundCol.X * 255.0f) ;
        C.G = (u8) (g_BackgroundCol.Y * 255.0f) ;
        C.B = (u8) (g_BackgroundCol.Z * 255.0f) ;
        eng_SetBackColor(C) ;

        #ifdef TARGET_PC
            f32 FPS = eng_GetFPS() ;
        #else
            f32 FPS=60.0f ;
        #endif

        f32 DeltaTime ;
        if (FPS > 0)
            DeltaTime = 1.0f / FPS ;
        else
            DeltaTime = 0 ;

        Update(DeltaTime);
        Render(DeltaTime);

        // Show stats every 60 frames
        //if ((Frame % 60) == 0)
            //eng_PrintStats() ;

        // Next frame
        Frame++ ;
    }

    // Delete all textures
    g_TextureManager.DeleteAllTextures() ;
    g_BackdropTexture.Kill() ;

    // Shutdown engine
    //eng_Kill();
}

//=========================================================================
