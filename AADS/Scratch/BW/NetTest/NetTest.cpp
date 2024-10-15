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
#include "netlib.hpp"
#include "ps2/iop/iop.hpp"
#include "x_threads.hpp"

#include "ps2/iop/usb.hpp"
#include "../../support/audiomgr/audio.hpp"
#include "../../support/cardmgr/cardmgr.hpp"
#include "../../support/cleantext/cleantext.hpp"
//=========================================================================

view          View;

//=========================================================================
char *StatusStrings[]=
{
    "AUDSTAT_FREE",            
    "AUDSTAT_IDLE",                   
    "AUDSTAT_PLAYING",                
    "AUDSTAT_DELAYING",               
    "AUDSTAT_PAUSING",                
    "AUDSTAT_PAUSED",                 
    "AUDSTAT_STOPPING",               
    "AUDSTAT_STOPPED",                
    "AUDSTAT_DELAYED",                
};

char *ModemStatusString[]=
{
        "STATUS_IDLE",
        "STATUS_NOTPRESENT",
        "STATUS_CONNECTED",
        "STATUS_DISCONNECTED",
        "STATUS_DIALING",
        "STATUS_NEGOTIATING",
        "STATUS_AUTHENTICATING",
        "STATUS_RETRYING",
        "STATUS_NODIALTONE",
        "STATUS_TIMEDOUT",
};

u8  KeyboardMap[]="`1234567890-=\\QWERTYUIOP[]ASDFGHJKL;'saZXCVBNM,./";

s32 EventMap[]=
{
    INPUT_KBD_GRAVE,INPUT_KBD_1,INPUT_KBD_2,INPUT_KBD_3,INPUT_KBD_4,INPUT_KBD_5,INPUT_KBD_6,INPUT_KBD_7,INPUT_KBD_8,INPUT_KBD_9,INPUT_KBD_0,INPUT_KBD_MINUS,INPUT_KBD_EQUALS,INPUT_KBD_BACKSLASH,0,
    INPUT_KBD_Q,INPUT_KBD_W,INPUT_KBD_E,INPUT_KBD_R,INPUT_KBD_T,INPUT_KBD_Y,INPUT_KBD_U,INPUT_KBD_I,INPUT_KBD_O,INPUT_KBD_P,INPUT_KBD_LBRACKET,INPUT_KBD_RBRACKET,0,
    INPUT_KBD_A,INPUT_KBD_S,INPUT_KBD_D,INPUT_KBD_F,INPUT_KBD_G,INPUT_KBD_H,INPUT_KBD_J,INPUT_KBD_K,INPUT_KBD_L,INPUT_KBD_SEMICOLON,INPUT_KBD_APOSTROPHE,0,
    INPUT_KBD_LSHIFT,INPUT_KBD_LMENU,INPUT_KBD_Z,INPUT_KBD_X,INPUT_KBD_C,INPUT_KBD_V,INPUT_KBD_B,INPUT_KBD_N,INPUT_KBD_M,INPUT_KBD_COMMA,INPUT_KBD_PERIOD,INPUT_KBD_SLASH,0,
    -1,
};

extern char ENGArgv0[64];
s32 flashcount=0;                              
#define MAX_SERVERS             8
#define MAX_SERVER_NAME_LENGTH  16

#define BROADCAST_PORT          8192
#define COMMUNICATION_PORT      8193

typedef struct s_ServerLookup
{
    u32 ClientIP;
    u32 ClientPort;
    s32 nServers;
    u32 ServerList[MAX_SERVERS];
} server_lookup;

typedef struct s_ServerResponse
{
    u32     ServerIP;
    u32     ServerPort;
    char    Name[MAX_SERVER_NAME_LENGTH];
} server_response;

vector3 Target;
void net_GetConfig   (interface_info &info);
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

void DispMouse(s32 mouse,s32 y)
{
    char            buttonstate[16];
    s32             i;

    i=0;
    if (input_IsPressed(INPUT_MOUSE_BTN_L,mouse))
    {
        buttonstate[i++]='L';
    }
    if (input_IsPressed(INPUT_MOUSE_BTN_C,mouse))
    {
        buttonstate[i++]='C';
    }

    if (input_IsPressed(INPUT_MOUSE_BTN_R,mouse))
    {
        buttonstate[i++]='R';
    }

    if (input_IsPressed(INPUT_MOUSE_BTN_0,mouse))
    {
        buttonstate[i++]='0';
    }

    if (input_IsPressed(INPUT_MOUSE_BTN_1,mouse))
    {
        buttonstate[i++]='1';
    }

    if (input_IsPressed(INPUT_MOUSE_BTN_2,mouse))
    {
        buttonstate[i++]='2';
    }

    if (input_IsPressed(INPUT_MOUSE_BTN_3,mouse))
    {
        buttonstate[i++]='3';
    }

    if (input_IsPressed(INPUT_MOUSE_BTN_4,mouse))
    {
        buttonstate[i++]='4';
    }

    buttonstate[i]=0x0;
    if (input_IsPressed(INPUT_PS2_QRY_MOUSE_PRESENT,mouse))
    {
        x_printfxy(1,y,"Mouse(%d) = (%2.f,%2.f,%2.f) btn=%s\n",mouse,
                            input_GetValue(INPUT_MOUSE_X_ABS,mouse),
                            input_GetValue(INPUT_MOUSE_Y_ABS,mouse),
                            input_GetValue(INPUT_MOUSE_WHEEL_ABS,mouse),buttonstate);
    }
    else
    {
        x_printfxy(1,y,"No mouse");
    }
}

void DispKeyboard(s32 kbd,s32 x,s32 y)
{
    char            buttonstate[16];
    s32             *pEventId;
    u8              *pKeycode;
    s32             i;

    if (input_IsPressed(INPUT_PS2_QRY_KBD_PRESENT,kbd))
    {

        pEventId = EventMap;
        pKeycode = KeyboardMap;
        i=0;
        while (1)
        {
            s32 j;
            if (*pEventId==-1)
                break;
            j=0;
            while (*pEventId)
            {
                if (input_IsPressed((enum input_gadget)*pEventId,kbd))
                {
                    buttonstate[j]=*pKeycode;
                }
                else
                {
                    buttonstate[j]=' ';
                }
                j++;
                pKeycode++;
                pEventId++;
            }
            buttonstate[j]=0x0;
            pEventId++;
            x_printfxy(x,i+y,buttonstate);
            i++;
        }
    }
    else
    {
        x_printfxy(x,y,"No keyboard");
    }
}

extern "C"
{
    void *ps2_SetStack(void *pStack);
}


char *TestWords[]=
{
    "FUCK you fucking dickless twat",
    "SHIT",
    "DICK",
    "TWAT",
    "ARSE",
    "OK",
    "BISCUIT",
    "TESTING",
    "DYNAMITE",
    NULL,
};

xbool s_Release=0;
xsema mutex(1,1);
s32     sema;

void thread(void)
{
    while (1)
    {
        mutex.Release();
    }
}

void thread2(void)
{
    while(1)
    {
        WaitSema(sema);
    }
}

//=========================================================================
void AppMain( int argc,char **argv )
{
    f32             analog_x,analog_y;
    s32             Effect1,Effect2,Effect3;

    net_address     CommunicationCaller,Recipient;
    net_address     BroadcastRecipient,BroadcastCaller;
    s32             status;
    char            buffer[512];
    s32             Length;
    u32             ip,port;
    s32             broadcast_delay;
    s32             broadcast_count=0;
    interface_info  info;
    s32             i;
    xtimer          timer;
    xtimer          sendtimer;
    xtimer          recvtimer;
    modem_status    ModemStatus;
    void            *sp;

    server_response ServerList[MAX_SERVERS];
    char            ResolvedName[MAX_SERVERS][128];
    union
    {
        server_lookup   Lookup;
        server_response Response;
    } Server;

    s32 ServerCount;
    s32 containerid;
    Effect2 = 0;

    (void)argc;         // Just to get rid of some compiler warnings!
    (void)argv;

#define LOOP 100000
    {
        s32 i;
        xtimer t;
        struct SemaParam p;
        xthread* th;

        p.initCount = 0;
        p.maxCount = 1;

        sema = CreateSema(&p);

        th = new xthread(thread2,"Test thread",8192,1);
        t.Reset();
        t.Start();

        for (i=0;i<LOOP;i++)
        {
            SignalSema(sema);
        }
        t.Stop();
        printf("Time for system signal/wait %2.2fms\n",t.ReadMs());
        delete th;
    }

    {
        xtimer t;
        xthread* th;

        t.Reset();
        t.Start();
        th = new xthread(thread,"Test Thread",8192,1);
        for (i=0;i<LOOP;i++)
        {
            mutex.Acquire();
        }
        t.Stop();
        printf("Time for internal acquire/release %2.2fms\n",t.ReadMs());
        delete th;
    }

    BREAK;
    // Init engine in default mode
    eng_Init();

    char **pWords;

    CleanLanguage Language;

    pWords = TestWords;
    while (*pWords)
    {
        xstring good;
        s32 status;

        //status = Language.WashYourMouth(xstring(*pWords),good);
        x_DebugMsg("Word %s checked to %s, status = %d\n",*pWords,(const char *)good,status);
        pWords++;
    }
    BREAK;
    sp = ps2_SetStack((void *)0x70004000);

    View.SetViewport(0,0,512,448);
    View.SetPosition(vector3(-206.0f,0.0f,0.0f));
    View.SetRotation(radian3(0,-R_90,0));
    View.SetZLimits(1.0f,10000.0f);
    View.SetXFOV(R_60);


    ps2_SetStack(sp);

    audio_Init();
    analog_x=0.5;analog_y=0.5;
    audio_LoadContainer("effects.pkg");
    audio_LoadContainer("music.pkg");
    containerid = audio_LoadContainer("voice.pkg");
    x_DebugMsg("Card error %d\n",CARD_ERR_GENERAL);
#ifdef INCLUDE_NET
    net_Init();

#endif
    ServerCount = 0;
#if 0

    netfs_Init();
    {
        X_FILE *fp;

        fp = NULL;
        while (1)
        {
            fp = x_fopen("demo1.ncb","rb");
            if (fp)
            {
                u8 *pBuffer;
                s32 Length;

                x_fseek(fp,0,X_SEEK_END);
                Length = x_ftell(fp);
                x_fseek(fp,0,X_SEEK_SET);
                ASSERT(Length);
                if (Length > 1048576)
                    Length = 1048576;

                pBuffer = (u8 *)x_malloc(Length);
                ASSERT(pBuffer);

                x_fread(pBuffer,Length,1,fp);
                x_fclose(fp);
                x_free(pBuffer);
                eng_PageFlip();
                ps2_DelayThread(250);
            }
        }
    }
#endif
    broadcast_delay = 0;
    while (1) 
    {

        timer.Reset();
        timer.Start();
        while (input_UpdateState());
        audio_Update();
        Update();
        Render();

#ifdef INCLUDE_NET
        net_GetInterfaceInfo(-1,info);
        if (info.Address)
        {
            xbool changed;

            changed = FALSE;

            if (!CommunicationCaller.IP)
            {
                net_Bind(CommunicationCaller,COMMUNICATION_PORT);
                changed = TRUE;
            }
            if (!BroadcastCaller.IP)
            {
                net_BindBroadcast(BroadcastCaller,BROADCAST_PORT);
                changed = TRUE;
            }

            if (changed)
            {
                net_AddressToStr(CommunicationCaller.IP,CommunicationCaller.Port,buffer);
                x_DebugMsg("%08x:%04x converted to %s\n",CommunicationCaller.IP,CommunicationCaller.Port,buffer);
                ip = net_ResolveName("root.inevitable.com");
                net_AddressToStr(ip,buffer);
                x_DebugMsg("root.inevitable.com resolved to %s\n",buffer);

                ip = net_ResolveName("biscuits-house.com");
                net_AddressToStr(ip,buffer);
                x_DebugMsg("biscuits-house.com resolved to %s\n",buffer);

                net_StrToAddress("biscuits-house.com:8000",ip,port);

                x_DebugMsg("StrToAddress returned ip=%08x,port=%04x\n",ip,port);

                net_AddressToStr(ip,port,buffer);

                x_DebugMsg("AddressToStr returned %s\n",buffer);
            }
        }
        else
        {
            if (CommunicationCaller.IP)
            {
                net_Unbind(CommunicationCaller);
                CommunicationCaller.Clear();
            }

            if (BroadcastCaller.IP)
            {
                net_Unbind(BroadcastCaller);
                BroadcastCaller.Clear();
            }
        }

        net_GetInterfaceInfo(-1,info);
        x_printfxy(1,12,"Addr=%08x, Broad=%08x, \nMask=%08x, Name=%08x,",
                    info.Address,
                    info.Broadcast,
                    info.Netmask,
                    info.Nameserver);

        recvtimer.Reset();
        recvtimer.Start();
        if (CommunicationCaller.IP)
        {
            status = net_Receive(CommunicationCaller,Recipient,buffer,Length);
        }
        else
        {
            status = FALSE;
        }
        recvtimer.Stop();

        if (status)
        {
            s32 i;
            net_IPToStr(Recipient.IP,buffer);
            x_DebugMsg("Received a %d byte packet from %s,time=%2.2f\n",Length,buffer,recvtimer.ReadMs());

            for (i=0;i<2;i++)
            {
                x_sprintf(buffer,"Counter %d\n",i);
                net_Send(CommunicationCaller,Recipient,buffer,x_strlen(buffer)+1);
            }
        }

        //
        // Did we receive a response from a server lookup?
        //
        recvtimer.Reset();
        recvtimer.Start();
        if (BroadcastCaller.IP)
        {
            status = net_Receive(BroadcastCaller,BroadcastRecipient,&Server,Length);
        }
        else
        {
            status = FALSE;
        }
        recvtimer.Stop();
        if (status)
        {
            char str[128];

            net_AddressToStr(BroadcastRecipient.IP,BroadcastRecipient.Port,str);
            x_DebugMsg("Got broadcast packet from %s, time=%2.2f\n",str,recvtimer.ReadMs());

        //
        // Did we receive a broadcast packet requesting a server lookup?
        //
            if (Length == sizeof(server_lookup))
            {
                status = FALSE;
                for (i=0;i<Server.Lookup.nServers;i++)
                {
                    if ( Server.Lookup.ServerList[i] == info.Address )
                    {
                        status = TRUE;
                    }
                }

                if (!status)
                {
                    // We didn't find ourselves in the list of excluded servers so we
                    // build a response packet and send it back. This will add us to
                    // the clients server list.

                    x_sprintf(Server.Response.Name,"ip=%08x",info.Address);

                    Recipient.IP = Server.Lookup.ClientIP;
                    Recipient.Port = Server.Lookup.ClientPort;

                    Server.Response.ServerIP   = BroadcastCaller.IP;
                    Server.Response.ServerPort = BroadcastCaller.Port;

                    net_Send(BroadcastCaller,Recipient,&Server.Response,sizeof(server_response));
                }
            }
        //
        // Did we receive a broadcast packet with a server response?
        //
            else if (Length == sizeof(server_response))
            {
                s32 i,found;

                found = FALSE;

                for (i=0;i<ServerCount;i++)
                {
                    if ( (ServerList[i].ServerIP == Server.Response.ServerIP) &&
                         (ServerList[i].ServerPort == Server.Response.ServerPort) )
                        found = TRUE;
                }

                if ( ( ServerCount < MAX_SERVERS ) && (!found) )
                {
                    ServerList[ServerCount] = Server.Response;
                    net_IPToStr(Server.Response.ServerIP,ResolvedName[ServerCount]);
                    ServerCount++;
                }
            }
            else
            {
                x_DebugMsg("Broadcast: Got %d bytes instead of %d. Ignored!\n",Length,sizeof(server_lookup));
            }
        }
        //
        // Is it time to send another lookup request?
        //
        if (broadcast_delay == 0)
        {
            char str[128];

            if (BroadcastCaller.IP)
            {
                BroadcastRecipient.IP = info.Broadcast;
                BroadcastRecipient.Port = BroadcastCaller.Port;
                x_sprintf(buffer,"Broadcast %d\n",broadcast_count);
                Server.Lookup.ClientIP = BroadcastCaller.IP;
                Server.Lookup.ClientPort = BroadcastCaller.Port;
                Server.Lookup.nServers = ServerCount;
                for (i=0;i<ServerCount;i++)
                {
                    Server.Lookup.ServerList[i] = ServerList[i].ServerIP;
                }
                sendtimer.Reset();
                sendtimer.Start();

                net_Send(BroadcastCaller,BroadcastRecipient,&Server.Lookup,sizeof(server_lookup));
                sendtimer.Stop();
                net_IPToStr(BroadcastRecipient.IP,str);
                x_DebugMsg("Sent broadcast to %s:%d, time = %2.2f\n",str,BroadcastRecipient.Port,sendtimer.ReadMs());
            }
            broadcast_delay = 60 * 2;
        }


        broadcast_delay--;

        for (i=0;i<ServerCount;i++)
        {
            net_AddressToStr(ServerList[i].ServerIP,ServerList[i].ServerPort,buffer);
            x_printfxy(1,11+i,"IP=%s, name=%s (%s)\n",buffer,ServerList[i].Name,ResolvedName[i]);
        }
        
        x_printfxy(1,4,"Position=(%2.2f,%2.2f)\n",analog_x,analog_y);
        x_printfxy(1,5,"Status 1=%s,%2.2f\n",StatusStrings[audio_GetStatus(Effect1) & 0x0f],audio_GetPlayPosition(Effect1));

        net_GetDialupStatus(ModemStatus);
        x_printfxy(1,6,"nRetries = %d, Timeout=%d, Speed=%x",
                        ModemStatus.nRetries,
                        ModemStatus.TimeoutRemaining,
                        ModemStatus.ConnectSpeed);
        x_printfxy(1,7,"Modem Status=%s\n",ModemStatusString[ModemStatus.Status]);
#endif        

        DispMouse(0,16);
        DispMouse(1,17);
        DispKeyboard(0,2,18);
        DispKeyboard(1,18,18);

        audio_SetEarView(&View);
        audio_SetPosition(Effect2,&Target);

        if (input_IsPressed(INPUT_PS2_BTN_L_DOWN))
        {
            if (audio_GetStatus(Effect3) == AUDSTAT_FREE)
            {
                Effect3 = audio_Play(0x04000006);
            }
        }
        else
        {
            if (Effect3)
            {
                audio_Stop(Effect3);
                Effect3=0;
            }
        }
        if (input_WasPressed(INPUT_PS2_BTN_START))
        {
            if (audio_GetStatus(Effect1) == AUDSTAT_PLAYING)
            {
                audio_Stop(Effect1);
            }
            else
            {
                Effect1 = audio_Play(0x04000000);
            }
        }

        if (Effect2 && (audio_GetStatus(Effect2) == AUDSTAT_FREE))
        {
            Effect2 =audio_Play(0x04000006,&Target);
        }
        if (input_WasPressed(INPUT_PS2_BTN_SELECT))
        {
            Effect2 =audio_Play(0x04000006,&Target);
            audio_SetVolume(Effect2,0.75);
        }

        if (input_IsPressed(INPUT_PS2_BTN_L1))
        {
            audio_SetPitch(Effect2,0.0f);
        }
        else
        {
            audio_SetPitch(Effect2,1.0f);
        }

        if (input_WasPressed(INPUT_PS2_BTN_L2))
        {
            ServerCount=0;
            View.SetPosition(vector3(-206.0f,0.0f,0.0f));
            View.SetRotation(radian3(0,-R_90,0));
            audio_StopAll();
            Effect2=0;
        }

        if (input_IsPressed(INPUT_PS2_BTN_R_STICK))
        {
                analog_x = (input_GetValue(INPUT_PS2_STICK_RIGHT_X)+1)/2;
                analog_y = (1-input_GetValue(INPUT_PS2_STICK_RIGHT_Y))/2;
                audio_SetPan(Effect2,analog_x);
                audio_SetRearPan(Effect2,analog_y);
        }
#ifdef INCLUDE_NET
        if (input_WasPressed(INPUT_PS2_BTN_R1))
        {
            net_SetDialupInfo("9,3811045","MSN/inevitable1","lemmings");
            net_StartDial(10,10);
        }

        if (input_WasPressed(INPUT_PS2_BTN_R2))
        {
            net_Disconnect();
        }
        if (input_WasPressed(INPUT_KBD_X))
        {
            ServerCount=0;
        }
#endif
        if (input_WasPressed(INPUT_PS2_BTN_CIRCLE))
        {
            input_Feedback(1.0f,(input_GetValue(INPUT_PS2_STICK_RIGHT_Y)+1)/2.0f);
        }

        if (input_WasPressed(INPUT_PS2_BTN_CIRCLE,1))
        {
            input_Feedback(1.0f,(input_GetValue(INPUT_PS2_STICK_RIGHT_Y,1)+1)/2.0f,1);
        }

        if (input_IsPressed(INPUT_PS2_BTN_TRIANGLE) )
        {
            input_Feedback(1.0f,(input_GetValue(INPUT_PS2_STICK_RIGHT_Y)+1)/2.0f);
        }

        timer.Stop();
        x_printfxy(1,15,"Time = %2.2f, cpu idle=%2.2f",timer.ReadMs(),iop_GetCpuUtilization());
        eng_PageFlip();

    }

    audio_UnloadContainer(containerid);

    net_Kill();

    // Shutdown engine
    eng_Kill();
}

