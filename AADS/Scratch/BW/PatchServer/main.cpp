#include "x_files.hpp"
#include "entropy.hpp"
#include "masterserver/masterserver.hpp"

net_address         LocalAddress;
net_address         RemoteAddress;
net_address         MasterServer;
byte                PacketBuffer[1024];

//xarray<net_address> PatchList;

//-------------------------------------------------------------------------------------------------
void SendRegistrationPacket(void)
{
    bytestream Bytestream;

    Bytestream.Init(PacketBuffer,sizeof(PacketBuffer));
    //
    // Send a registration request to the master server
    //
    Bytestream.PutByte(MC_SMALL_MESSAGE);               // Class
    Bytestream.PutWord(ST_DIRECTORY_SERVER);            // ServiceType
    Bytestream.PutWord(MT_ADD_SERVICE_EX);              // MessageType
    Bytestream.PutByte( DIR_UF_OVERWRITE|
                        DIR_UF_SERVRETURNADDR|
                        DIR_UF_SERVGENNETADDR|
                        DIR_UF_DIRNOTUNIQUE );          // Entity Flags
    Bytestream.PutWideString(xwstring("/Tribes2PS2")); // Path
    Bytestream.PutWideString(xwstring("")); // Name

    Bytestream.PutByte(2);                          // Address length (0 forces server to fill it for us)
    Bytestream.PutNetWord(LocalAddress.Port);

    Bytestream.PutWideString(xwstring(""));               // Display name
    Bytestream.PutLong(5*60);           // Life span

    // 1 Entity, server version # 1168
    Bytestream.PutWord(1);

    Bytestream.PutByte(1);
    Bytestream.PutByte('V');
    Bytestream.PutWord(4);
    Bytestream.PutBytes((byte*)"1168",4);

    Bytestream.PutWord(0);

//  x_printf( "%2.02f: Sent registration packet\n", x_GetTimeSec() );
    net_Send( LocalAddress, MasterServer, Bytestream.GetDataPtr(), Bytestream.GetBytesUsed() );
}

//-------------------------------------------------------------------------------------------------

void PatchClient( u8* pData, void* pDest, s32 Length )
{
    s32 i;
    // Let's send them a bogus packet!

    bytestream BS;

    BS.PutByte(MC_SMALL_MESSAGE);           // type
    BS.PutWord(ST_DIRECTORY_SERVER);        // class
    BS.PutWord(MT_MULTI_ENTITY_REPLY);      // type
    BS.PutWord(0);                          // status
    BS.PutByte(0);                          // Sequence #
    BS.PutLong(DIR_GF_ADDDATAOBJECTS|DIR_GF_ADDDODATA);                     // flags
    BS.PutWord(1);                          // Entities
    BS.PutWord(1);                          // number of data objects

    BS.PutWord(56+Length);
    // Pad out some space until we get to the strcpy addresses
    // on the client stack
    for (i=0;i<7;i++)
    {
        BS.PutLong(i);
    }
    // Send the destination address
    BS.PutLong((s32)pDest);
    // Dummy long
    BS.PutLong(0xff00ff00);
    // Now send the address for the SOURCE of the x_strcpy
#if 1
    BS.PutLong(0x01ffe35c);         // Standard machine
#else
    BS.PutLong(0x07ffe35c);         // Devkit
#endif

    for (i=0;i<Length;i++)
    {
        BS.PutByte(*pData++);
    }

    net_Send(LocalAddress,RemoteAddress,BS.GetDataPtr(),BS.GetBytesUsed());
}

//-------------------------------------------------------------------------------------------------

void ParseLookupRequest(lookup_request* pRequest)
{
    char ipstr[64];
    char Buddies[16*10];

    // Break out buddies from packet
    s32     nBuddies    = 0;
    s32     Index       = 0;
    char*   p = pRequest->BuddySearchString;

    if( x_strlen( p ) > 0 )
    {
        while( (nBuddies < 10) )
        {
            s32 c = *p++;
            if( c == 0 )
            {
                Buddies[nBuddies*16+Index] = 0;
                Index = 0;
                nBuddies++;
                ASSERT( nBuddies <= 10 );
                break;
            }
            else if( c == 1 )
            {
                Buddies[nBuddies*16+Index] = 0;
                Index = 0;
                nBuddies++;
                ASSERT( nBuddies <= 10 );
            }
            else
            {
                Buddies[nBuddies*16+Index] = c;
                Index++;
                ASSERT( Index < 16 );
            }
        }
    }

    xbool ControlsPatched = FALSE;

    // Run through all buddies
    for( s32 iBuddy = 0; iBuddy < nBuddies; iBuddy++ )
    {
        char* pBuddy = &Buddies[16*iBuddy];
        s32 BuddyLen = x_strlen( pBuddy );
        x_strtoupper( pBuddy );

        // Skip check if buddy string length is 0
        if( BuddyLen == 0 )
            continue;

        //
        // Experimental auto aim distance reduction.
        //
        if( x_strstr( pBuddy,"RRR" ) )
        {
            static byte Range100[] = { 0xC8, 0x42 };

            PatchClient( Range100, (void*)0x0034F7E2, 2 ); // 2nd half of MaxHelpDist @ 0034F7E0

            net_IPToStr( RemoteAddress.IP, ipstr );
            x_printf( "%2.02f: Patching client (RRR) at %s:%d\n", x_GetTimeSec(), ipstr, RemoteAddress.Port );

            ControlsPatched = TRUE;
        }

        //
        // Classic CHAT-TEST, but upgraded to fix the Help.
        //
        if( x_strstr( pBuddy,"CHAT-TEST" ) && !ControlsPatched )
        {
            static byte BindVoiceChat[]  = { 0x13, 0x00 };
            static byte ZeroTargetLock[] = { 0x00, 0x00 };

            PatchClient( BindVoiceChat,  (void*)0x0030DCA0, 2 ); // fegl.WarriorSetups[0].ControlInfo.PS2_VOICE_MENU.MainGadget
            PatchClient( ZeroTargetLock, (void*)0x0030DD48, 2 ); // fegl.WarriorSetups[0].ControlInfo.PS2_TARGET_LOCK.MainGadget
            PatchClient( BindVoiceChat,  (void*)0x0030E000, 2 ); // fegl.WarriorSetups[0].ControlEditData[11].GadgetID
            PatchClient( ZeroTargetLock, (void*)0x0030E09C, 2 ); // fegl.WarriorSetups[0].ControlEditData[24].GadgetID            

            net_IPToStr( RemoteAddress.IP, ipstr );
            x_printf( "%2.02f: Patching client (CHAT-TEST) at %s:%d\n", x_GetTimeSec(), ipstr, RemoteAddress.Port );

            ControlsPatched = TRUE;
        }

        //
        // New "expert" configurations.
        //

        if( x_strstr( pBuddy,"EXPERT-" ) && !ControlsPatched )
        {
            static byte Expert_A[]       = "Expert A";
            static byte Expert_B[]       = "Expert B";

            static byte BindShift[]      = { 0x11, 0x00 };
            static byte BindVoiceChat[]  = { 0x13, 0x00 };
            static byte BindUsePack[]    = { 0x1A, 0x00 };
            static byte BindDropWeapon[] = { 0x19, 0x00 };
            static byte BindDropPack[]   = { 0x1B, 0x00 };
            static byte BindDropFlag[]   = { 0x18, 0x00 };
            static byte BindSuicide[]    = { 0x14, 0x00 };
                                        
            static byte Shift[]          = { 0x01, 0x00 };

            switch( pBuddy[7] )
            {
            case 'A':
                {
                    PatchClient( Expert_A,       (void*)0x00392358, 9 );

                    PatchClient( BindShift,      (void*)0x00310A98, 2 ); // DefaultControls[0].Controls[ 0].GadgetID
                    PatchClient( BindVoiceChat,  (void*)0x00310B10, 2 ); // DefaultControls[0].Controls[10].GadgetID
                    PatchClient( BindUsePack,    (void*)0x00310B34, 2 ); // DefaultControls[0].Controls[13].GadgetID
                    PatchClient( BindDropWeapon, (void*)0x00310B70, 2 ); // DefaultControls[0].Controls[18].GadgetID
                    PatchClient( BindDropPack,   (void*)0x00310B7C, 2 ); // DefaultControls[0].Controls[19].GadgetID
                    PatchClient( BindDropFlag,   (void*)0x00310B94, 2 ); // DefaultControls[0].Controls[21].GadgetID
                    PatchClient( BindSuicide,    (void*)0x00310BA0, 2 ); // DefaultControls[0].Controls[22].GadgetID
                                                           
                    PatchClient( Shift,          (void*)0x00310B38, 2 ); // DefaultControls[0].Controls[13].IsShifted
                    PatchClient( Shift,          (void*)0x00310B74, 2 ); // DefaultControls[0].Controls[18].IsShifted
                    PatchClient( Shift,          (void*)0x00310B80, 2 ); // DefaultControls[0].Controls[19].IsShifted
                    PatchClient( Shift,          (void*)0x00310B98, 2 ); // DefaultControls[0].Controls[21].IsShifted
                    PatchClient( Shift,          (void*)0x00310BA4, 2 ); // DefaultControls[0].Controls[22].IsShifted
                    PatchClient( Shift,          (void*)0x00310BBC, 2 ); // DefaultControls[0].Controls[24].IsShifted

                    ControlsPatched = TRUE;

                    break;
                }

            case 'B':
                {
                    PatchClient( Expert_B,       (void*)0x00392368, 9 );

                    PatchClient( BindShift,      (void*)0x00310BEC, 2 ); // DefaultControls[1].Controls[ 0].GadgetID
                    PatchClient( BindVoiceChat,  (void*)0x00310C64, 2 ); // DefaultControls[1].Controls[10].GadgetID
                    PatchClient( BindUsePack,    (void*)0x00310C88, 2 ); // DefaultControls[1].Controls[13].GadgetID
                    PatchClient( BindDropWeapon, (void*)0x00310CC4, 2 ); // DefaultControls[1].Controls[18].GadgetID
                    PatchClient( BindDropPack,   (void*)0x00310CD0, 2 ); // DefaultControls[1].Controls[19].GadgetID
                    PatchClient( BindDropFlag,   (void*)0x00310CE8, 2 ); // DefaultControls[1].Controls[21].GadgetID
                    PatchClient( BindSuicide,    (void*)0x00310CF4, 2 ); // DefaultControls[1].Controls[22].GadgetID
                                                                                             
                    PatchClient( Shift,          (void*)0x00310C8C, 2 ); // DefaultControls[1].Controls[13].IsShifted
                    PatchClient( Shift,          (void*)0x00310CC8, 2 ); // DefaultControls[1].Controls[18].IsShifted
                    PatchClient( Shift,          (void*)0x00310CD4, 2 ); // DefaultControls[1].Controls[19].IsShifted
                    PatchClient( Shift,          (void*)0x00310CEC, 2 ); // DefaultControls[1].Controls[21].IsShifted
                    PatchClient( Shift,          (void*)0x00310CF8, 2 ); // DefaultControls[1].Controls[22].IsShifted
                    PatchClient( Shift,          (void*)0x00310D10, 2 ); // DefaultControls[1].Controls[24].IsShifted

                    ControlsPatched = TRUE;

                    break;
                }

            default:
                break;
            }

            net_IPToStr( RemoteAddress.IP, ipstr );
            x_printf( "%2.02f: Patching client (%s) at %s:%d\n", x_GetTimeSec(), pBuddy, ipstr, RemoteAddress.Port );
        }

        if( x_strstr( pBuddy,"MSG-PLUS" ) )
        {
            static u32 MsgPlusPatch[]=
            {
                // Note: Previous instructions read an xcolor type to 0x0(sp).
                0x27b000f0,             // addiu    s0,sp,0xf0
                0x0240202d,             // dmove    a0,s2
                0x0c056ad0,             // jal      bitstream::ReadWString
                0x0200282d,             // dmove    a1,s0
                0x3c020032,             // lui      v0,0x32             (load ptr to tgl.pHUDManager)
                0x0200382d,             // dmove    a3,s0
                0x8c448e94,             // lw       a0,0x8e94(v0)
                0x24050001,             // addiu    a1,zero,1           (chat_area_player)
                0x0c09a1c8,             // jal      hud_manager::Popup
                0x8e06ff10,             // lw       a2,-0xf0(s0)        (load player index - cannot use 0(sp) as that would terminate too early)
                0x00c63022,             // sub      a2,a2,a2
                0x00c63022,             // sub      a2,a2,a2
            };

            PatchClient( (byte*)MsgPlusPatch, (void*)0x00132320, sizeof(MsgPlusPatch) );

            net_IPToStr( RemoteAddress.IP, ipstr );
            x_printf( "%2.02f: Patching client (MSG-PLUS) at %s:%d\n", x_GetTimeSec(), ipstr, RemoteAddress.Port );
        }
    }

    /*
    //
    // Always send the patch for MSG-PLUS.
    //
    {
        static u32 MsgPlusPatch[]=
        {
            // Note: Previous instructions read an xcolor type to 0x0(sp).
            0x27b000f0,             // addiu    s0,sp,0xf0
            0x0240202d,             // dmove    a0,s2
            0x0c056ad0,             // jal      bitstream::ReadWString
            0x0200282d,             // dmove    a1,s0
            0x3c020032,             // lui      v0,0x32             (load ptr to tgl.pHUDManager)
            0x0200382d,             // dmove    a3,s0
            0x8c448e94,             // lw       a0,0x8e94(v0)
            0x24050001,             // addiu    a1,zero,1           (chat_area_player)
            0x0c09a1c8,             // jal      hud_manager::Popup
            0x8e06ff10,             // lw       a2,-0xf0(s0)        (load player index - cannot use 0(sp) as that would terminate too early)
            0x00c63022,             // sub      a2,a2,a2
            0x00c63022,             // sub      a2,a2,a2
        };

        PatchClient( (byte*)MsgPlusPatch, (void*)0x00132320, sizeof(MsgPlusPatch) );

        net_IPToStr( RemoteAddress.IP, ipstr );
        x_printf( "%2.02f: Patching client (MSG-PLUS) at %s:%d\n", x_GetTimeSec(), ipstr, RemoteAddress.Port );
    }
    */
}

//-------------------------------------------------------------------------------------------------
s32 main(s32 argc,char* argv[])
{
    xtimer Timer;
    xtimer Timeout;
    xbool  ok;
    s32    Length;
    bytestream Bytestream;
    char ipstr[64];

    x_Init();
    net_Init();

    VERIFY(net_Bind(LocalAddress,28500));

    while (1)
    {
        while (MasterServer.IP == 0)
        {
            MasterServer.IP = net_ResolveName("tribes2.m1.sierra.com");
            MasterServer.Port = 15101;
            x_DelayThread(250);
        }

        SendRegistrationPacket();
        Timeout.Reset();
        Timeout.Start();
        Timer.Reset();
        Timer.Start();
        while (Timer.ReadSec() < 4.0f*60.0f)
        {

            // If we receive a response from the master server on a lookup registration,
            // then we just turn off the Timeout timer.
            if (Timeout.ReadSec() > 5.0f)
                break;
            // Check for lookup request

            Length = sizeof(PacketBuffer);
            x_memset(PacketBuffer,0,sizeof(PacketBuffer));
            ok = net_Receive(LocalAddress,RemoteAddress,PacketBuffer,Length);
            if (ok)
            {
                net_IPToStr(RemoteAddress.IP,ipstr);
            //  x_printf("Packet received from %s:%d, %d bytes.\n",ipstr,RemoteAddress.Port,Length);
                s32 MessageClass;
                s32 ServiceType;
                s32 MessageType;
                s32 Status;

                Bytestream.Init(PacketBuffer,Length);

                MessageClass = Bytestream.GetByte();
                ServiceType  = Bytestream.GetWord();
                MessageType  = Bytestream.GetWord();
                Status       = Bytestream.GetWord();

                if ( (MessageClass == MC_SMALL_MESSAGE) &&
                     (ServiceType  == ST_DIRECTORY_SERVER) )
                {
                    // All we're interested in is whether or not we got registered with
                    // the master server.
                    if (Status >= 0)
                    {
                        switch(MessageType)
                        {
                        case MT_STATUS_REPLY:
                            s32 Port,IP;
                            Bytestream.GetByte();
                            Port = Bytestream.GetNetWord();
                            IP   = Bytestream.GetLong();
                            net_IPToStr(IP,ipstr);
                        //  x_printf("%2.02f: Received register response. IP = %s:%d\n", x_GetTimeSec(),ipstr,Port);
                            Timeout.Stop();
                            break;
                        }
                    }
                    else
                    {
                    //  x_printf("%2.02f: Got response %d from registration\n",x_GetTimeSec(),Status);
                    }
                }
                else
                {
                    // Could be a net lookup request (this is when we can
                    // hijack the patch in)
                    base_info *pIncoming;
                    pIncoming = (base_info*)PacketBuffer;
                    switch (pIncoming->Type)
                    {
                        //-----------------------------------------------------------------------------
                        case PKT_LOOKUP:
                        case PKT_RESPONSE:
                        case PKT_NET_RESPONSE:
                            break;
                        case PKT_NET_LOOKUP:
                            ParseLookupRequest((lookup_request*)pIncoming);
                            break;
                        default:
                            break;
                    }
                    
                }

            }
            else
            {
                x_DelayThread(100);
            }
        }
    }
    x_Kill();
    return 0;
}