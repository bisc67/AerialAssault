//=========================================================================
//
// TEST0.CPP
//
//=========================================================================
//#define INCLUDE_NET
#include "entropy.hpp"
#include "e_draw.hpp"
#include "e_input.hpp"
#include "e_threads.hpp"
#include "vm_util.hpp"
#include "libsn.h"

//=========================================================================

view          View;

//=========================================================================

s32 flashcount=0;                              
byte databuffer[1024];

vector3 Target;
void Render( void )
{
    static s32 NFrames=0;
    static vector3 corner1=vector3(480,0,0);
    static vector3 corner2=vector3(0,0,480);

    // Set current view
    eng_SetView(View,0);
    eng_ActivateView(0);

    // Print some stats
    NFrames++;

    // Render random bboxes
    if( 1 )
    {
        x_srand(13);
        eng_Begin();
        draw_ClearL2W();
        draw_Grid(vector3(-240,0,-240),vector3(480,0,0),vector3(0,0,480),xcolor(0,128,0),16);
        for( s32 i=0; i<20; i++ )
        {
            vector3 MN;
            vector3 MX;
            vector3 P;
            vector3 W;
            xcolor   C;

            P.X = x_frand(-30,30)*8;
            P.Y = 0;
            P.Z = x_frand(-30,30)*8;
            W.X = x_frand(1,2.5f)*4;
            W.Y = x_frand(1,2.5f)*4;
            W.Z = x_frand(1,2.5f)*4;
            C.Random();

            MN = P;
            MX = P+W;
            if ( !((i==0) && (flashcount & 8)) )
                draw_BBox(bbox(MN,MX),C);

            if (i==0)
            {
                Target = P;
            }
        }
        eng_End();
    }
    flashcount++;
}

//=========================================================================

void Update( void )
{
    f32 S     = 1.0f;
    f32 Speed = S;
    f32 Rot   = S*R_1*1.5f;
    radian      Pitch,Yaw;
    f32         analog_x,analog_y;


    if( input_IsPressed(INPUT_PS2_BTN_R1) )     View.Translate(vector3(0,Speed,0),view::VIEW);
    if( input_IsPressed(INPUT_PS2_BTN_R2) )     View.Translate(vector3(0,-Speed,0),view::VIEW);

    View.GetPitchYaw(Pitch,Yaw);

    if( input_IsPressed(INPUT_PS2_BTN_TRIANGLE) )   Pitch+=Rot;
    if( input_IsPressed(INPUT_PS2_BTN_CROSS) ) Pitch-=Rot;
    if( input_IsPressed(INPUT_PS2_BTN_SQUARE) ) Yaw+=Rot;
    if( input_IsPressed(INPUT_PS2_BTN_CIRCLE) )Yaw-=Rot;

    analog_x = -input_GetValue(INPUT_PS2_STICK_RIGHT_X);
    analog_y = input_GetValue(INPUT_PS2_STICK_RIGHT_Y);
    
    View.Translate(vector3(analog_x,0,analog_y),view::VIEW);
    analog_x = -input_GetValue(INPUT_PS2_STICK_LEFT_X);
    analog_y = input_GetValue(INPUT_PS2_STICK_LEFT_Y);
    
    Pitch -=analog_y/(R_360*4.0f);
    Yaw+=analog_x/(R_360*4.0f);

    View.SetRotation(radian3(Pitch,Yaw,0));
}

void AppMain( int argc,char **argv )
{
    xtimer          timer;
    xtimer          DeltaTimer;
    f32             DeltaTime;

    (void)argc;         // Just to get rid of some compiler warnings!
    (void)argv;

    snDebugInit();
    // Init engine in default mode
    eng_Init();

    View.SetViewport(0,0,512,448);
    View.SetPosition(vector3(-206.0f,0.0f,0.0f));
    View.SetRotation(radian3(0,-R_90,0));
    View.SetZLimits(1.0f,10000.0f);
    View.SetXFOV(R_60);

    DeltaTimer.Stop();
    DeltaTimer.Reset();

    vm_Debug_DumpTLBs();
    vm_Init();
    vm_Debug_DumpTLBs();
    *(u32 *)0x00 = 0xdeadbeef;

    while (1) 
    {
        //-----------------------------------------------------------------------------
        while (input_UpdateState());
        Update();
        Render();
        DeltaTime = DeltaTimer.ReadSec();
        if (DeltaTime > 1.0f) DeltaTime = 1.0f;
        DeltaTimer.Reset();
        DeltaTimer.Start();


        if (input_WasPressed(INPUT_PS2_BTN_L2))
        {
            View.SetPosition(vector3(-206.0f,0.0f,0.0f));
            View.SetRotation(radian3(0,-R_90,0));
        }

        if (input_WasPressed(INPUT_PS2_BTN_L1))
        {
        }

        timer.Stop();
        eng_PageFlip();

    }

    // Shutdown engine
    eng_Kill();
}

//=========================================================================
