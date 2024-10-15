#include "x_files.hpp"
#include "entropy.hpp"
#include "masterserver/masterserver.hpp"

net_address			LocalAddress;
net_address			RemoteAddress;
net_address			MasterServer;
byte				PacketBuffer[1024];
xarray<net_address> PatchList;

struct lookup_client
{
	net_address		RemoteAddress;
	xtimer			Timeout;
	s32				Retries;
	xbool			IsValid;
	s32				Version;
	f32				RetryTime;
	base_server		Info;
};

xarray<lookup_client> LookupList;
s32 TotalServers;

//-------------------------------------------------------------------------------------------------
void BeginLookupRequest(void)
{
	bytestream Bytestream;

	char ipstr[64];
	net_IPToStr(MasterServer.IP,ipstr);
	TotalServers = 0;
	x_DebugMsg("BeginLookupRequest: Request sent to master server %s:%d\n",ipstr,MasterServer.Port);

    Bytestream.Init(PacketBuffer,sizeof(PacketBuffer));
    Bytestream.PutByte(MC_SMALL_MESSAGE);				// Class
    Bytestream.PutWord(ST_DIRECTORY_SERVER);			// ServiceType
    Bytestream.PutWord(MT_GET_DIRECTORY_EX);			// MessageType
    Bytestream.PutLong(
	        DIR_GF_ADDDOTYPE	 |
            DIR_GF_ADDTYPE       |
	        DIR_GF_DECOMPSERVICES|
	        DIR_GF_SERVADDNETADDR|
	        DIR_GF_ADDDODATA	 |
	        DIR_GF_ADDDATAOBJECTS);						// GetFlags
    Bytestream.PutWord(20);								// MaxEntitiesPerReply
    Bytestream.PutWideString(xwstring("/Tribes2PS2"));   // DirectoryPath
    Bytestream.PutWord(0);								// NumDataObjectTypes
    net_Send(LocalAddress,MasterServer,Bytestream.GetDataPtr(),Bytestream.GetBytesUsed());
	LookupList.Clear();
}

//-------------------------------------------------------------------------------------------------
// Send a lookup request to an individual client
void SendLookupRequest(lookup_client& Client)
{
    lookup_request packet;
	char			ipstr[64];

	net_IPToStr(Client.RemoteAddress.IP,ipstr);
	x_printf("%2.2f: Sending lookup request to %s:%d, attempt %d\n",(f32)x_GetTimeSec(),ipstr,Client.RemoteAddress.Port,Client.Retries);
    packet.Info.Type = PKT_NET_LOOKUP;
    packet.Info.IP   = LocalAddress.IP;
    packet.Info.Port = LocalAddress.Port;
    packet.Info.SequenceNumber = 0;
    packet.Info.Version = Client.Version;
    packet.BroadcastPort = 27999;
	packet.BuddySearchString[0]=0x0;
    net_Send(LocalAddress,Client.RemoteAddress,&packet,sizeof(lookup_request)-(SEARCH_STRING_LENGTH-x_strlen(packet.BuddySearchString))+1);
	Client.Timeout.Reset();
	Client.Timeout.Start();
}


//-------------------------------------------------------------------------------------------------
void ParseLookupResponse(net_address& Remote, lookup_response* pRequest)
{
	s32 i;

	for (i=0;i<LookupList.GetCount();i++)
	{
		if ( (Remote.IP == LookupList[i].RemoteAddress.IP) &&
			 (Remote.Port == LookupList[i].RemoteAddress.Port) )
		{
			LookupList[i].Info = pRequest->ServerInfo;
			LookupList[i].Version = pRequest->Info.Version;
			LookupList[i].IsValid = TRUE;
			LookupList[i].Timeout.Stop();
			break;
		}
	}
}

//-------------------------------------------------------------------------------------------------
void ResolveMasterServer(void)
{
	s32 RetriesRemaining;

	//x_printf("Resolving Master Server IP...\n");
	// Resolve the master server IP
	RetriesRemaining = 3;
	while (MasterServer.IP == 0)
	{
		MasterServer.IP = net_ResolveName("tribes2.m1.sierra.com");
		MasterServer.Port = 15101;
		x_DelayThread(250);
		RetriesRemaining--;
		if (RetriesRemaining < 0)
		{
			x_printf("Error: Could not resolve master server ip\n");
			exit(-1);
		}
	}
}

//-------------------------------------------------------------------------------------------------
void AcquireServerList(void)
{
	xbool		LastPacket;
	xtimer		Timeout;
	s32			RetriesRemaining;
	xbool		ok;
	bytestream	Bytestream;
	s32			Length;
	char		ipstr[64];
	xwchar		buffer[64];
	s32			MessageClass;
	s32			MessageType;
	s32			Status;
	s32			SequenceNumber;
	s32			Flags;
	s32			ServiceType;
	s32			EntityCount;
	s32			i;
	s32			Type;
	s32			Count;

	//x_printf("Waiting for server list...\n");
	LastPacket = FALSE;
	RetriesRemaining = 3;
	Timeout.Reset();
	Timeout.Start();
	BeginLookupRequest();

	while (!LastPacket)
	{
		if (Timeout.ReadSec() > 5.0f)
		{
			RetriesRemaining--;
			if (RetriesRemaining >= 0)
			{
				BeginLookupRequest();
			}
			else
			{
				Timeout.Stop();
				break;
			}
		}
	
		ok = net_Receive(LocalAddress,RemoteAddress,PacketBuffer,Length);
		if (ok)
		{
			net_IPToStr(RemoteAddress.IP,ipstr);
			x_DebugMsg("Packet received from %s:%d, %d bytes.\n",ipstr,RemoteAddress.Port,Length);

			Bytestream.Init(PacketBuffer,Length);

			MessageClass = Bytestream.GetByte();
			ServiceType  = Bytestream.GetWord();
			MessageType  = Bytestream.GetWord();
			Status       = Bytestream.GetWord();

			if ( (MessageClass == MC_SMALL_MESSAGE) &&
				 (ServiceType  == ST_DIRECTORY_SERVER) && 
				 (Status >=0) && 
				 (MessageType == MT_MULTI_ENTITY_REPLY)
				 )
			{
				Timeout.Trip();
				SequenceNumber	= (u8)Bytestream.GetByte();
				Flags			= Bytestream.GetLong();
				EntityCount		= Bytestream.GetWord();

				for (i=0;i<EntityCount;i++)
				{
					// Parse each entity in turn and then start lookup requests to it
					lookup_client& Client = LookupList.Append();
					Client.RemoteAddress = RemoteAddress;
					Client.Retries = 3;
					Client.IsValid = FALSE;
					if (Flags & DIR_GF_ADDTYPE)
					{
						Type = Bytestream.GetByte();
					}

					if (Type =='S' )
					{
						if (Flags & DIR_GF_SERVADDPATH)
						{
							Bytestream.GetWideString(buffer);
						}
						if (Flags & DIR_GF_SERVADDNAME)
						{
							Bytestream.GetWideString(buffer);
						}

						if (Flags & DIR_GF_SERVADDNETADDR)
						{
							s32 length;
							length = Bytestream.GetByte();
							if (length == 6)
							{
								Client.RemoteAddress.Port = Bytestream.GetNetWord();
								Client.RemoteAddress.IP   = Bytestream.GetLong();
							}
							else
							{
								ASSERT(length==0);
								while (length)
								{
									Bytestream.GetByte();
									length--;
								}
							}
						}
					}

					if (Type == 'D' )
					{
						if (Flags & DIR_GF_DIRADDPATH)
						{
							Bytestream.GetWideString(buffer);
						}
						if (Flags & DIR_GF_DIRADDNAME)      
						{
							Bytestream.GetWideString(buffer);
						}
						if (Flags & DIR_GF_DIRADDREQUNIQUE) 
						{
							Bytestream.GetByte();
						}
					}

					if (Flags & DIR_GF_ADDDISPLAYNAME)	Bytestream.GetWideString(buffer);

					if (Flags & DIR_GF_ADDLIFESPAN)         Bytestream.GetLong();
					if (Flags & DIR_GF_ADDCREATED)          Bytestream.GetLong();
					if (Flags & DIR_GF_ADDTOUCHED)          Bytestream.GetLong();
					if (Flags & DIR_GF_ADDCRC)              Bytestream.GetLong();

					if (Flags & DIR_GF_ADDDATAOBJECTS)
					{
						s32 j;
						Count = Bytestream.GetWord();               // Number of data objects
						for (j=0;j<Count;j++)
						{
							s32 length;
							char value[64];
							char name[64];

							x_memset(value,0,sizeof(value));
							x_memset(name,0,sizeof(name));
							if (Flags & DIR_GF_ADDDOTYPE)
							{
								length = Bytestream.GetByte();              // Length of data object type, byte
								Bytestream.GetBytes((byte*)name,length);      // Name of data object type
							}

							if (Flags & DIR_GF_ADDDODATA)
							{
								length = Bytestream.GetWord();              // Length of data value
								Bytestream.GetBytes((byte*)value,length);     // Name of data value
							}

							if (x_strcmp(name,"V")==0)
							{
								Client.Version = x_atoi(value);
							}
						}
					}

					if (Flags & DIR_GF_ADDACLS) ASSERT(FALSE);
				}

				if (SequenceNumber & 0x80)
				{
					LastPacket = TRUE;
				}
			}
		}
		else
		{
			x_DelayThread(250);
		}
	}
	x_printf("Got %d servers from the master server.\n",LookupList.GetCount());
}

//-------------------------------------------------------------------------------------------------
void GetServerResponses(void)
{
	xbool	Finished;
	s32		RetriesRemaining;
	xtimer	Timeout;
	s32		Length;
	xbool	ok;
	char	ipstr[64];
	s32		i;

	RetriesRemaining = 3;
	Finished = FALSE;
	Timeout.Reset();
	Timeout.Start();
	f32 RetryTime = 0.0f;

	for (i=0;i<LookupList.GetCount();i++)
	{
		LookupList[i].RetryTime = RetryTime;
		LookupList[i].Timeout.Trip();
		RetryTime += 0.10f;
	}

	TotalServers = LookupList.GetCount();

	// Now get the info from each server in the list
	while (!Finished)
	{
		// Check for lookup request
		Length = sizeof(PacketBuffer);
		x_memset(PacketBuffer,0,sizeof(PacketBuffer));
		ok = net_Receive(LocalAddress,RemoteAddress,PacketBuffer,Length);
		if (ok)
		{
			Timeout.Trip();
			net_IPToStr(RemoteAddress.IP,ipstr);
			//x_printf("Packet received from %s:%d, %d bytes.\n",ipstr,RemoteAddress.Port,Length);
			// We should have received a PKT_NET_RESPONSE to the lookup request
			// we sent to a machine so it could be that!
			base_info *pIncoming;
			pIncoming = (base_info*)PacketBuffer;
			if (pIncoming->Type==PKT_NET_RESPONSE)
			{
				ParseLookupResponse(RemoteAddress,(lookup_response*)pIncoming);
			}
		}
		else
		{
			x_DelayThread(100);
		}

		s32 ValidCount=0;

		for (i=0;i<LookupList.GetCount();i++)
		{
			if (!LookupList[i].IsValid)
			{
				if (LookupList[i].Timeout.ReadSec() > LookupList[i].RetryTime)
				{
					LookupList[i].RetryTime = 3.0f;
					if (LookupList[i].Retries)
					{
						LookupList[i].Retries--;
						SendLookupRequest(LookupList[i]);
					}
					else
					{
						LookupList.Delete(i);
					}
				}
			}
			else
			{
				ValidCount++;
			}
		}

		if ( (ValidCount == LookupList.GetCount()) || (Timeout.ReadSec() > 10.0f) )
		{
			Finished = TRUE;
		}
	}
}
//-------------------------------------------------------------------------------------------------
extern "C" s32 main(s32 argc,char* argv[])
{
	char		ipstr[64];
	s32			i;

	x_Init();
	net_Init();

	VERIFY(net_Bind(LocalAddress));

	ResolveMasterServer();
	AcquireServerList();
	GetServerResponses();
	s32 TotPlayers;
	s32 TotClients;
	s32 TotServers;

	TotClients=0;
	TotPlayers=0;
	TotServers=0;

	x_printf("   SERVER NAME   CLNTS PLYRS BOTS  VERS ADDRESS\n");
	for (i=0;i<LookupList.GetCount();i++)
	{
		net_IPToStr(LookupList[i].RemoteAddress.IP,ipstr);
		if (LookupList[i].IsValid)
		{
			x_printf("%15s %4d  %4d  %4d   %4d %s:%d\n",
						(const char*)xstring(LookupList[i].Info.Name),
						LookupList[i].Info.ClientCount,
						LookupList[i].Info.CurrentPlayers,
						LookupList[i].Info.BotCount,
						LookupList[i].Version,
						ipstr,
						LookupList[i].RemoteAddress.Port);
			TotPlayers+= LookupList[i].Info.CurrentPlayers;
			TotClients+= LookupList[i].Info.ClientCount;
			TotServers++;
		}

	}
	x_printf("\nTOTALS %4d/%-4d %3d  %4d\n",TotServers, TotalServers, TotClients,TotPlayers);
	// Now dump out the list of obtained server information
	x_Kill();
	return 0;
}