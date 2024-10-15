//=========================================================================
//
// TEST0.CPP
//
//=========================================================================
//#define INCLUDE_NET
#include "entropy.hpp"
#include "e_draw.hpp"
#include "e_input.hpp"
#include "e_netfs.hpp"
#include "netlib/netlib.hpp"
#include "ps2/iop/iop.hpp"
#include "e_threads.hpp"
#include "masterserver/masterserver.hpp"

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

f32 f1sqrt(f32 v)
{
    f32 r;
    f32 one;

    one = 1.0f;

    asm("rsqrt.s %0,%1,%2":"=f"(r):"f"(one),"f"(v));
    return r;
}
void AppMain( int argc,char **argv )
{
    interface_info  Info;
    net_address     Port;
    xtimer          timer;
    xtimer          DeltaTimer;
    xbool           Bound;
    s32             Delay;
    master_server   Master;
    single_entity   *pEntity;
    s32             i,x,y;
    char            textbuff[128];
    f32             DeltaTime;
    xbool           PacketReceived;
    net_address     IncomingPort;
    s32             Length;

    (void)argc;         // Just to get rid of some compiler warnings!
    (void)argv;

    // Init engine in default mode
    eng_Init();

    DeltaTime = f1sqrt(x_GetTime());
    View.SetViewport(0,0,512,448);
    View.SetPosition(vector3(-206.0f,0.0f,0.0f));
    View.SetRotation(radian3(0,-R_90,0));
    View.SetZLimits(1.0f,10000.0f);
    View.SetXFOV(R_60);

    net_Init();
    Delay = 0;
    Bound = FALSE;


    {
        xtimer t;
        f32 v;
        t.Reset();t.Start();
        v=2.1;
        for (i=0;i<1000000;i++)
        {
            v = v + sqrtf(v);
        }

        t.Stop();
        x_DebugMsg("Method 1 took %2.2f ms\n",t.ReadMs());
        x_DebugMsg("v=%2.2f\n",v);
    }

    {
        xtimer t;
        f32 v;
        t.Reset();t.Start();
        v=2.1;
        for (i=0;i<1000000;i++)
        {
            v = v + x_sqrt(v);
        }

        t.Stop();
        x_DebugMsg("Method 2 took %2.2f ms\n",t.ReadMs());
        x_DebugMsg("v=%2.2f\n",v);
    }

    Master.Init();
    Master.SetPath("/Tribes2PS2");
#ifdef bwatson
    Master.SetName("Biscuits test");
#else
#ifdef mtraub
    Master.SetName("Traub test");
#else
    Master.SetName("Other server");
#endif
#endif
    DeltaTimer.Stop();
    DeltaTimer.Reset();
    Master.AcquireDirectory(TRUE);
//    Master.SetPath("/");               // Test only. Get master directory list
    Master.AddMasterServer("wontest.west.won.net",15101);
    Master.AddMasterServer("wontest.central.won.net",15101);
    Master.AddMasterServer("wontest.east.won.net",15101);
    Master.SetAttribute("Test1","This is test value 1");
    Master.SetAttribute("test2","This is test 2");
    Master.SetAttribute("AllowKeyboard","T");
    Master.SetAttribute("MaximumPlayers","32");
    Master.SetAttribute("MinimumPlayers","0");

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
        net_GetInterfaceInfo(-1,Info);

        if (Info.Address)
        {
            if (!Bound)
            {
                net_Bind(Port);
                Master.SetAddress(Port);
                Master.SetServer(FALSE);
                Bound = TRUE;
                Master.AcquireDirectory(TRUE);
            }
        }
        else
        {
            if (Bound)
            {
                net_Unbind(Port);
                Bound = FALSE;
                Port.Clear();
                Master.SetAddress(Port);
            }
        }

                // Now do the polling of the port to see if anything useful came back
        if (Bound)
        {
            PacketReceived = net_Receive(Port,IncomingPort,databuffer,Length);
            if (PacketReceived)
            {
                Master.ProcessPacket(databuffer,Length);
            }
        }

        Master.Update(DeltaTime);
        static s32 count=0;

        net_DebugLog("This is test cycle %d\n",count++);

        x_printfxy(0,0,"Entity count=%d\n",Master.GetEntityCount());
        x=0;
        y=1;
        for (i=0;i<Master.GetEntityCount();i++)
        {
            pEntity = Master.GetEntity(i);
            *textbuff=0x0;
            x_printfxy(x,y,"%s%s,%s",pEntity->GetPath(),pEntity->GetName(),pEntity->GetDisplayName());
            if (pEntity->GetAddress().IP)
            {
                net_IPToStr(pEntity->GetAddress().IP,textbuff);
                x_printfxy(x+5,y+1,"%s:%d\n",textbuff,pEntity->GetAddress().Port);

            }
            y+=2;
            if (y>25)
            {
                break;
                y=0;
                x+=20;
            }
        }

        if (input_WasPressed(INPUT_PS2_BTN_L2))
        {
            Master.Reset();
            Master.SetServer(FALSE);
            Master.AcquireDirectory(TRUE);
            View.SetPosition(vector3(-206.0f,0.0f,0.0f));
            View.SetRotation(radian3(0,-R_90,0));
        }

        if (input_WasPressed(INPUT_PS2_BTN_L1))
        {
            Master.SetServer(TRUE);
            Master.AcquireDirectory(TRUE);
        }

        timer.Stop();
        eng_PageFlip();

    }
    Master.Kill();
    net_Kill();

    // Shutdown engine
    eng_Kill();
}

//=========================================================================
