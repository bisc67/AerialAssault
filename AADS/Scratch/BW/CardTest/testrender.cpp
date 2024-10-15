#include "x_files.hpp"
#include "entropy.hpp"

view          View;
s32             flashcount=0;

vector3 Target;
void TestRenderInit(void)
{
    View.SetViewport(0,0,512,448);
    View.SetPosition(vector3(0,0,0));
    View.SetRotation(radian3(0,R_45,0));
    View.SetZLimits(1.0f,10000.0f);
    View.SetXFOV(R_60);
}

void TestRenderRender( void )
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
            if ( !((i==0) && (flashcount & 16)) )
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

void TestRenderUpdate( void )
{
    f32 S     = 1.0f;
    f32 Speed = S;
    f32 Rot   = S*R_1*1.5f;
    radian      Pitch,Yaw;


    if( input_IsPressed(INPUT_PS2_BTN_L_UP) )   View.Translate(vector3(0,Speed,0),view::VIEW);
    if( input_IsPressed(INPUT_PS2_BTN_L_DOWN) ) View.Translate(vector3(0,-Speed,0),view::VIEW);
    if( input_IsPressed(INPUT_PS2_BTN_L_LEFT) ) View.Translate(vector3(-Speed,0,0),view::VIEW);
    if( input_IsPressed(INPUT_PS2_BTN_L_RIGHT) )View.Translate(vector3(Speed,0,0),view::VIEW);
    if( input_IsPressed(INPUT_PS2_BTN_R1) )     View.Translate(vector3(0,0,Speed),view::VIEW);
    if( input_IsPressed(INPUT_PS2_BTN_R2) )     View.Translate(vector3(0,0,-Speed),view::VIEW);

    View.GetPitchYaw(Pitch,Yaw);

    if( input_IsPressed(INPUT_PS2_BTN_TRIANGLE) )   Pitch+=Rot;
    if( input_IsPressed(INPUT_PS2_BTN_CROSS) ) Pitch-=Rot;
    if( input_IsPressed(INPUT_PS2_BTN_SQUARE) ) Yaw+=Rot;
    if( input_IsPressed(INPUT_PS2_BTN_CIRCLE) )Yaw-=Rot;
    View.SetRotation(radian3(Pitch,Yaw,0));
}

