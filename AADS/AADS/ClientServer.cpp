//=========================================================================
//
//  ClientServer.cpp
//
//=========================================================================

#include "x_stdio.hpp"
#include "netlib/bitstream.hpp"
#include "clientserver.hpp"
#include "ServerVersion.hpp"

//=========================================================================
//=========================================================================
//=========================================================================

struct cs_header
{
    // Data sent as header
    u32         Type;
    s32         Sequence;
    s32         LastSequenceReceived;
    s16         nTimesSent;
	s16			Checksum;
};

struct cs_packet
{
    // Data used privately
    cs_packet*  pNext;
    cs_packet*  pPrev;
    s32         BufferSize;
    s32         PAD[1];

    cs_header   Header;
};

struct cs_connection
{
    net_address Address;
    s32         Sequence;
    s32         LastSeqReceived;
    cs_packet*  pSentPacket;
    cs_packet*  pReceivedPacket;
    s32         nSentPackets;
    s32         nReceivedPackets;
};

#define MAX_CONNECTIONS 32
#define SHIP_FREQUENCY (0.25f)
static xbool        s_Inited=FALSE;
static f32          s_TimeTillShip;
static net_address  s_Address;

static cs_connection s_Connection[ MAX_CONNECTIONS ];

//=========================================================================


//=========================================================================

void  cs_Init   ( net_address& Address )
{
    ASSERT( !s_Inited );
    s_Inited = TRUE;

    for( s32 i=0; i<MAX_CONNECTIONS; i++ )
    {
        s_Connection[i].LastSeqReceived = -1;
        s_Connection[i].pSentPacket = NULL;
        s_Connection[i].pReceivedPacket = NULL;
        s_Connection[i].Sequence = -1;
        s_Connection[i].nSentPackets = 0;
        s_Connection[i].nReceivedPackets = 0;
    }
    
    s_TimeTillShip = SHIP_FREQUENCY;
    s_Address = Address;

    ASSERT( (sizeof(cs_header)%128) == 0 );
}

//=========================================================================

void  cs_Kill   ( void )
{
    ASSERT( s_Inited );
    s_Inited = FALSE;

    // Kill all unused packets
    for( s32 i=0; i<MAX_CONNECTIONS; i++ )
    if( s_Connection[i].Address.IP != 0 )
        cs_CloseConnection( s_Connection[i].Address );
}

//=========================================================================

void cs_OpenConnection( net_address& DstAddress )
{
    s32 i;

    // Check if already used
    for( i=MAX_CONNECTIONS-1; i>=0; i-- )
    {
        if( s_Connection[i].Address == DstAddress )
            return;

        if( s_Connection[i].Address.IP == 0 )
            break;
    }

    ASSERT( i!=-1 );

    s_Connection[i].Address = DstAddress;
    s_Connection[i].LastSeqReceived = -1;
    s_Connection[i].pSentPacket = NULL;
    s_Connection[i].pReceivedPacket = NULL;
    s_Connection[i].Sequence = -1;
    s_Connection[i].nSentPackets = 0;
    s_Connection[i].nReceivedPackets = 0;
}

//=========================================================================

void cs_CloseConnection( net_address& DstAddress )
{
    s32 i;

    // Find
    for( i=0; i<MAX_CONNECTIONS; i++ )
    if( s_Connection[i].Address == DstAddress )
        break;
    ASSERT( i!=MAX_CONNECTIONS );

    cs_packet* pP = s_Connection[i].pSentPacket;
    while( pP )
    {
        cs_packet* pNext = pP->pNext;
        x_free( pP );
        pP = pNext;
    }

    pP = s_Connection[i].pReceivedPacket;
    while( pP )
    {
        cs_packet* pNext = pP->pNext;
        x_free( pP );
        pP = pNext;
    }

    s_Connection[i].Address = DstAddress;
    s_Connection[i].LastSeqReceived = -1;
    s_Connection[i].pSentPacket = NULL;
    s_Connection[i].pReceivedPacket = NULL;
    s_Connection[i].Sequence = -1;
    s_Connection[i].nSentPackets = 0;
    s_Connection[i].nReceivedPackets = 0;
}

//=========================================================================
/*

void  cs_HandleIncoming ( const bitstream& aBS, net_address& Src )
{
    // Find channel
    s32 i;
    for( i=0; i<MAX_CONNECTIONS; i++ )
    if( s_Connection[i].Address == Src )
        break;
    if( i==MAX_CONNECTIONS )
    {
        x_DebugMsg("Packet from unknown IP\n");
        return;
    }

    // Get sequence numbers
    cs_header* pHeader = (cs_header*)aBS.GetDataPtr();
    s32 Seq = pHeader->Sequence;
    s32 LastSeqRecv = pHeader->LastSequenceReceived;
    x_DebugMsg("csHandleIncoming( %1d, %1d )\n",Seq,LastSeqRecv);

    // Check if we've already received this packet
    if( Seq > s_Connection[i].LastSeqReceived )
    {
        // Allocate a received packet
        s32 TotalSize = aBS.GetNBytesUsed();
        s32 BufferSize = TotalSize - sizeof(cs_header);

        // Check if this is just a header.
        if( BufferSize > 0 )
        {
            cs_packet* pPacket = (cs_packet*)x_malloc(sizeof(cs_packet)+BufferSize);
            x_memcpy( &pPacket->Header, aBS.GetDataPtr(), TotalSize );

            // Hook packet into list
            if( s_Connection[i].pReceivedPacket )
                s_Connection[i].pReceivedPacket->pPrev = pPacket;
            pPacket->pNext = s_Connection[i].pReceivedPacket;
            pPacket->pPrev = NULL;
            pPacket->BufferSize = TotalSize - sizeof(cs_header);

            s_Connection[i].nReceivedPackets++;
        }
    }
    else
    {
        x_DebugMsg("Received duplicate packet\n");
    }

    // Clear out sent packets that have been received
    cs_packet* pPacket = s_Connection[i].pSentPacket;
    while( pPacket )
    {
        cs_packet* pNext = pPacket->pNext;
        
        // If dest has received the packet then remove from list
        if( pPacket->Header.Sequence <= LastSeqRecv )
        {
            if( pPacket->pNext )
                pPacket->pNext->pPrev = pPacket->pPrev;
            if( pPacket->pPrev )
                pPacket->pPrev->pNext = pPacket->pNext;
            if( s_Connection[i].pSentPacket == pPacket )
                s_Connection[i].pSentPacket = pPacket->pNext;

            s_Connection[i].nSentPackets--;

            x_free( pPacket );
        }

        pPacket = pNext;
    }
}

//=========================================================================

void  cs_Update ( f32 DeltaTime )
{
    // Is it time to ship packets again?
    s_TimeTillShip -= DeltaTime;
    if( s_TimeTillShip <= 0.0f )
    {
        s_TimeTillShip += SHIP_FREQUENCY;

        // Loop through channels
        s32 i;
        for( i=0; i<MAX_CONNECTIONS; i++ )
        if( s_Connection[i].Address.IP != 0 )
        {
            // Are there any packets to send?
            if( s_Connection[i].pSentPacket )
            {
                // Get last packet
                cs_packet* pPacket = s_Connection[i].pSentPacket;
                while( pPacket->pNext != NULL )
                    pPacket = pPacket->pNext;

                // We are the first outgoing packet for this dest address
                pPacket->Header.LastSequenceReceived = s_Connection[i].LastSeqReceived;
				// We don't need to bother checksumming here since it'll be checksummed when
				// it is sent with cs_Send
                net_Send( s_Address, s_Connection[i].Address, &pPacket->Header, pPacket->BufferSize+sizeof(cs_header) );

                // Remember we sent it
                pPacket->Header.nTimesSent++;

                x_DebugMsg("Shipping %1d/%1d\n",pPacket->BufferSize, pPacket->Header.nTimesSent);
            }
            else
            {
                // Send just a header
                cs_header Header;
                Header.LastSequenceReceived = s_Connection[i].LastSeqReceived;
                Header.nTimesSent   = 0;
                Header.Sequence     = -1;
                Header.Type         = 0;
                net_Send( s_Address, s_Connection[i].Address, &Header, sizeof(cs_header) );

                x_DebugMsg("Sending blank header\n");
            }

        }
    }
}

//=========================================================================

void  cs_Send   ( const bitstream& BS, net_address& Dst )
{
    // Find the channel
    s32 i;
    for( i=0; i<MAX_CONNECTIONS; i++ )
    if( s_Connection[i].Address == Dst )
        break;
    ASSERT( i!=MAX_CONNECTIONS );

    // Alloc a packet
    cs_packet* pPacket;
    byte* pByte = (byte*)x_malloc(sizeof(cs_packet)+BS.GetNBytesUsed());
    ASSERT( pByte );
    pPacket = (cs_packet*)pByte;

    pPacket->pNext      = NULL;
    pPacket->pPrev      = NULL;
    pPacket->BufferSize = BS.GetNBytesUsed();

    pPacket->Header.Type       = 0;//CONN_PACKET_TYPE_CS;
    pPacket->Header.Sequence   = s_Connection[i].Sequence;
    pPacket->Header.nTimesSent = 0;
    pPacket->Header.LastSequenceReceived = 0;
    s_Connection[i].Sequence++;

    // Copy data over
    pByte += sizeof(cs_packet);
    x_memcpy( pByte, BS.GetDataPtr(), pPacket->BufferSize );

	pPacket->Header.Checksum = net_Checksum(pByte,pPacket->BufferSize);
    // Add packet to list
    if( s_Connection[i].pSentPacket )
        s_Connection[i].pSentPacket->pPrev = pPacket;
    pPacket->pNext = s_Connection[i].pSentPacket;
    pPacket->pPrev = NULL;

    s_Connection[i].nSentPackets++;
}

//=========================================================================

xbool cs_Receive( bitstream& BS, net_address& Src )
{
    // Find the channel
    s32 i;
	xbool valid;

    for( i=0; i<MAX_CONNECTIONS; i++ )
    if( s_Connection[i].Address == Src )
        break;
    ASSERT( i!=MAX_CONNECTIONS );

    // Find last packet
    cs_packet* pPacket = s_Connection[i].pReceivedPacket;
    if( pPacket == NULL )
        return FALSE;
    while( pPacket->pNext != NULL )
        pPacket = pPacket->pNext;

	valid = (net_Checksum((u8*)(pPacket+1),pPacket->BufferSize) == pPacket->Header.Checksum);
    // Copy buffer over
    x_memcpy( BS.GetDataPtr(), pPacket+1, pPacket->BufferSize );

    // Remove from list
    if( pPacket->pNext )
        pPacket->pNext->pPrev = pPacket->pPrev;
    if( pPacket->pPrev )
        pPacket->pPrev->pNext = pPacket->pNext;
    if( s_Connection[i].pReceivedPacket == pPacket )
        s_Connection[i].pReceivedPacket = pPacket->pNext;

    s_Connection[i].nReceivedPackets--;

    // Deallocate 
    x_free( pPacket );

	if (!valid)
	{
		x_DebugMsg("cs_Receive: Packet received with bad checksum\n");
		x_printf("cs_Receive: Bad packet. Please log in bug database\n");
	}
    return valid;
}
*/
//=========================================================================

void cs_DisplayStats( void )
{
    s32 i;

    for( i=0; i<MAX_CONNECTIONS; i++ )
    {
        x_printfxy(0,i+4,"%2d] S:%2d R:%2d Seq:%1d",
            i,
            s_Connection[i].nSentPackets,
            s_Connection[i].nReceivedPackets,
            s_Connection[i].Sequence );
    }
}

//=========================================================================
//=========================================================================
//=========================================================================

void cs_ReadLoginReq ( cs_login_req& Login, const bitstream& BS )
{
    x_memset( &Login, 0, sizeof(cs_login_req) );

    BS.ReadS32      ( Login.ServerVersion );
    BS.ReadRangedS32( Login.NPlayers, 1, 2 );
    BS.ReadS32      ( Login.SystemId );
    BS.ReadWString  ( Login.Password );

    ASSERT( (Login.NPlayers>=1) && (Login.NPlayers<=2) );

    for( s32 i=0; i<Login.NPlayers; i++ )
    {
        BS.ReadWString( Login.Data[i].Name );
		ASSERT(x_wstrlen(Login.Data[i].Name) <= 16);
        BS.ReadS32( Login.Data[i].CharacterType, 8 );
        BS.ReadS32( Login.Data[i].SkinType, 8 );
        BS.ReadS32( Login.Data[i].VoiceType, 8 );
    }   

    BS.ReadF32( Login.TVRefreshRate ) ;
}

//=========================================================================

void cs_WriteLoginReq( const cs_login_req& Login, bitstream& BS )
{
    ASSERT( (Login.NPlayers>=1) && (Login.NPlayers<=2) );

    BS.WriteS32      ( Login.ServerVersion );
    BS.WriteRangedS32( Login.NPlayers, 1, 2 );
    BS.WriteS32      ( Login.SystemId );
    BS.WriteWString  ( Login.Password );

    for( s32 i=0; i<Login.NPlayers; i++ )
    {
		ASSERT(x_wstrlen(Login.Data[i].Name) <= 16 ); 
        BS.WriteWString( Login.Data[i].Name );
        BS.WriteS32( Login.Data[i].CharacterType, 8 );
        BS.WriteS32( Login.Data[i].SkinType, 8 );
        BS.WriteS32( Login.Data[i].VoiceType, 8 );
    }   

    BS.WriteF32( Login.TVRefreshRate ) ;
}

//=========================================================================

void cs_ReadLoginResp( cs_login_resp& Login, const bitstream& BS )
{
    x_memset( &Login, 0, sizeof(cs_login_resp) );

    BS.ReadS32( Login.RequestResp );
    BS.ReadS32( Login.SessionID );
    BS.ReadWString( Login.ServerName );
    BS.ReadS32( Login.NPlayers, 8 );

    ASSERT( (Login.NPlayers>=0) && (Login.NPlayers<=2) );

    for( s32 i=0; i<Login.NPlayers; i++ )
    {
        BS.ReadWString( Login.Data[i].Name );
		ASSERT(x_wstrlen(Login.Data[i].Name) <= 16 ); 
        BS.ReadS32( Login.Data[i].PlayerIndex, 16 );
    }   
}

//=========================================================================

void cs_WriteLoginResp( const cs_login_resp& Login, bitstream& BS )
{
    ASSERT( (Login.NPlayers>=0) && (Login.NPlayers<=2) );

    BS.WriteS32( Login.RequestResp );
    BS.WriteS32( Login.SessionID );
    BS.WriteWString( Login.ServerName );
    BS.WriteS32( Login.NPlayers, 8 );

    ASSERT( (Login.NPlayers>=0) && (Login.NPlayers<=2) );

    for( s32 i=0; i<Login.NPlayers; i++ )
    {
		ASSERT(x_wstrlen( Login.Data[i].Name ) <= 16 );
        BS.WriteWString( Login.Data[i].Name );
        BS.WriteS32( Login.Data[i].PlayerIndex, 16 );
    }   
}

//=========================================================================

void cs_ReadEndMission( cs_end_mission& EndMission, const bitstream& BS )
{
    BS.ReadS32( EndMission.Round );
    BS.ReadS32( EndMission.SessionID );
}

//=========================================================================

void cs_WriteEndMission( const cs_end_mission& EndMission, bitstream& BS )
{
    BS.WriteS32( EndMission.Round );
    BS.WriteS32( EndMission.SessionID );
}

//=========================================================================

void cs_ReadMissionResp( cs_mission_resp& Mission, const bitstream& BS )
{
    BS.ReadString( Mission.MissionName );
    BS.ReadS32( Mission.GameType, 5 );
    BS.ReadS32( Mission.Round );
}

//=========================================================================

void cs_WriteMissionResp( const cs_mission_resp& Mission, bitstream& BS )
{
    BS.WriteString( Mission.MissionName );
    BS.WriteS32( Mission.GameType, 5 );
    BS.WriteS32( Mission.Round );
}

//=========================================================================

void cs_ReadDisconnect (       cs_disconnect& Disconnect, const bitstream& BS )
{
    BS.ReadS32( Disconnect.Reason );
}

//=========================================================================

void cs_WriteDisconnect( const cs_disconnect& Disconnect,       bitstream& BS )
{
    BS.WriteS32( Disconnect.Reason );
}

//=========================================================================

void cs_ReadInSync (       cs_insync& InSync, const bitstream& BS )
{
    BS.ReadS32( InSync.Reason );
}

//=========================================================================

void cs_WriteInSync( const cs_insync& InSync,       bitstream& BS )
{
    BS.WriteS32( InSync.Reason );
}

//=========================================================================



