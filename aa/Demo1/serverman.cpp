#include "entropy.hpp"
#include "x_types.hpp"
#include "netlib/netlib.hpp"
#include "serverman.hpp"
#include "gamemgr/gamemgr.hpp"
#include "masterserver/masterserver.hpp"
#include "fe_Globals.hpp"
#include "dlg_message.hpp"
#include "ui/ui_win.hpp"
#include "audiomgr/audio.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "sm_common.hpp"
#include "gameserver.hpp"
#include "SpecialVersion.hpp"

extern xbool g_ServerListChanged;
//#define TEST_SERVER_LIST
//-----------------------------------------------------------------------------
server_manager::server_manager(void)
{
    m_Broadcast.Clear();
	Reset();
	m_IsServer = FALSE;
    m_IsInGame = FALSE;
    m_Changed  = FALSE;
    m_Local.IP = 0;
    m_Local.Port = 0;
    m_Name[0] = '\0';
    x_memset( m_BuddySearchString, 0, SEARCH_STRING_LENGTH );
}

//-----------------------------------------------------------------------------
server_manager::~server_manager(void)
{
	net_Unbind(m_Broadcast);
}


//-----------------------------------------------------------------------------
void	server_manager::Reset(void)
{
	m_Servers.Clear();
	m_PollDelay		= 0.0f;
}

//-----------------------------------------------------------------------------
server_info *server_manager::GetServer(s32 index)
{
	if (index > m_Servers.GetCount())
		return NULL;

	return &m_Servers[index];
}

//-----------------------------------------------------------------------------
s32			server_manager::GetCount(void)
{
	return m_Servers.GetCount();
}

//-----------------------------------------------------------------------------
void		server_manager::SetAsServer(xbool on)
{
	m_IsServer = on;
}

//-----------------------------------------------------------------------------
void		server_manager::SetName(const xwchar *pName)
{
	x_wstrncpy(m_Name,pName,MAX_SERVER_NAME_LENGTH);
}

//-----------------------------------------------------------------------------
void        server_manager::SetSearchString( const char* pBuddyString)
{
    x_strncpy( m_BuddySearchString, pBuddyString, SEARCH_STRING_LENGTH-1 );
    m_BuddySearchString[SEARCH_STRING_LENGTH-1] = 0;
}

//-----------------------------------------------------------------------------
s32			server_manager::Refresh(f32 time_delta)
{
	s32             updated;
    char            RecvBuf[MAX_PACKET_SIZE];
    interface_info  info;
    s32             i,length;
    xbool           status;

    updated = FALSE;

    // Check to see if we have an attached interface yet
    net_GetInterfaceInfo(-1,info);
    if (!info.Address)
    {
        //
        // Clean up internal buffers just in case the network
        // interface just died
        //
        if (m_Broadcast.IP)
        {
            x_DebugMsg("server_manager: Network connection dropped, closed broadcast port\n");
            net_Unbind(m_Broadcast);
            m_Changed = TRUE;
			dlg_message* pMessage;

			if (tgl.pUIManager && !fegl.DeliberateDisconnect)
			{
				if (GameMgr.GameInProgress())
				{
					if (fegl.State == FE_STATE_ONLINE)
					{
						tgl.Playing = FALSE;
        
						GameMgr.EndGame();
						g_SM.NetworkDown = TRUE;
					}
					// Do something here to force it to quit game and give a message saying the
					// network connection was lost
				}
				else
				{
					if (!g_SM.MissionLoading)
					{
						pMessage = (dlg_message*)tgl.pUIManager->OpenDialog( fegl.UIUserID,
																   "message",
																   irect(0,0,260,160),
																   NULL,
																   ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER );
						ASSERT(pMessage);
						pMessage->Configure( xwstring("ERROR!"), 
											   NULL, 
											   xwstring("DONE"), 
											   xwstring("You have been disconnected from\n"
														 "the network. Please attempt to\n"
														 "connect again."), 
											   XCOLOR_WHITE
											   );
						audio_Play(SFX_FRONTEND_ERROR, AUDFLAG_CHANNELSAVER);
					}
					else
					{
						tgl.Playing = FALSE;
        
						GameMgr.EndGame();
						g_SM.NetworkDown = TRUE;
					}
				}
			}
			fegl.DeliberateDisconnect = FALSE;
        }
        m_Broadcast.Clear();
        return FALSE;
    }

    // If we don't have an address for our broadcast port, let's open it now
    if (!m_Broadcast.IP)
    {
		fegl.DeliberateDisconnect = FALSE;
        net_BindBroadcast(m_Broadcast,BROADCAST_PORT);
        m_Changed = TRUE;
        if (!m_Broadcast.IP)
        {
            return FALSE;
        }
        x_DebugMsg("server_manager: Network connection established, opened broadcast port\n");
    }

    for (i=0;i<m_Servers.GetCount();)
    {
        m_Servers[i].RefreshTimeout-=time_delta;
        if (m_Servers[i].RefreshTimeout <=0.0f)
        {
            m_Servers.Delete(i);
			m_Changed = TRUE;
        }
        else
        {
            i++;
        }
    }

    while (1)
    {
        status = net_Receive( m_Broadcast, m_Recipient, RecvBuf, length );
        if (!status)
            break;

        updated = ParsePacket(RecvBuf,m_Broadcast,m_Recipient);
    }

	//
	// Now send another broadcast packet if we need to (and if we're ready to)
    // We should only be sending broadcast packets when we are in the front end
    // proper (i.e. none sent during mission transition or load)
	//
	m_PollDelay -= time_delta;
	
	if ( (!m_IsInGame) && (m_Broadcast.IP) && (m_PollDelay < 0.0f) )
	{
		m_PollDelay = SERVER_POLL_INTERVAL;
        SendLookup(PKT_LOOKUP,m_Broadcast,net_address(-1,m_Broadcast.Port));
	}

    if( updated ) 
        m_Changed = TRUE;

	return m_Changed;
}

//-----------------------------------------------------------------------------
xbool   server_manager::ParsePacket(void *pBuff,const net_address &Local,const net_address &Remote)
{
    base_info   *pIncoming;

    pIncoming = (base_info*)pBuff;
	// Did we receive a request for server information?

	switch (pIncoming->Type)
	{
        //-----------------------------------------------------------------------------
        case PKT_NET_LOOKUP:
		case PKT_LOOKUP:
			if (m_IsServer)
			{
                ParseLookupRequest((lookup_request*)pIncoming,Local,Remote);
			}
			break;
        //-----------------------------------------------------------------------------
		case PKT_RESPONSE:
            return ParseLookupResponse((lookup_response*)pIncoming);
			break;
        //-----------------------------------------------------------------------------
		case PKT_NET_RESPONSE:
            return ParseNetLookupResponse((lookup_response*)pIncoming,Remote);
			break;

         //-----------------------------------------------------------------------------
		default:
            // Packet wasn't for us, so let's just pass it on to the next handler....
            return FALSE;
			break;
	}
    return TRUE;
}

//-----------------------------------------------------------------------------
void server_manager::ParseLookupRequest(lookup_request *pRequest,const net_address &Local, const net_address &Remote)
{
    lookup_response packet;

    x_memset( &packet, 0, sizeof(packet) );

    // ignore packets from different versions
    if ( pRequest->Info.Version != SERVER_CURRENT_VERSION )
        return;

	if (!tgl.pServer)
		return;

    if (pRequest->Info.Type==PKT_NET_LOOKUP)
    {
        packet.Info.Type = PKT_NET_RESPONSE;
    }
    else
    {
		packet.Info.Type = PKT_RESPONSE;
    }

	// This information *SHOULD* be in m_Recipient already but the PS2 libs have
	// a bug in them which causes a bad IP/Port to be placed in the UDP header
	// so we don't really know who it came from.
	m_Recipient.IP	 = pRequest->Info.IP;
	m_Recipient.Port = pRequest->BroadcastPort;

    ASSERT( m_Recipient.Port < 0xFFFF );

    //
	// This *SHOULD* be pointing to the data port for this server. THIS
	// IS JUST A TEST FOR NOW!
	//
    packet.Info.Version             = SERVER_CURRENT_VERSION;
    s32 Humans,Bots,Max;

    GameMgr.GetPlayerCounts(Humans,Bots,Max);
	//x_DebugMsg("server_manager::ParseLookupRequest - GameMgr reported %d humans, %d bots and %d max\n",Humans,Bots,Max);

    packet.Info.IP                  = m_Local.IP;
	packet.Info.Port                = m_Local.Port;
    packet.Info.SequenceNumber      = pRequest->Info.SequenceNumber;
    packet.ServerInfo.GameType      = (s8)GameMgr.GetGameType();
    packet.ServerInfo.AmountComplete= (s8)(GameMgr.GetGameProgress() * 100.0f);
    packet.ServerInfo.BotCount      = Bots;
    packet.ServerInfo.CurrentPlayers= Humans;
    packet.ServerInfo.MaxPlayers    = Max;
	packet.ServerInfo.MaxClients	= fegl.ServerSettings.MaxClients;
	packet.ServerInfo.ClientCount	= tgl.pServer->GetClientCount();
	packet.ServerInfo.Flags			= 0;

    x_strcpy ( packet.ServerInfo.LevelName, tgl.MissionName );
	x_wstrcpy( packet.ServerInfo.Name, m_Name );
    ASSERT(m_Recipient.IP);
    ASSERT(m_Broadcast.IP);
    // Now we go through and try and find any users within the buddy search string.
    // Each search entry is delimited by a null character

    const game_score& Score = GameMgr.GetScore();
    xstring PlayerName;

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

    // Run through all buddies
    xbool done = FALSE;
    for( s32 iBuddy=0 ; (iBuddy<nBuddies) && !done ; iBuddy++ )
    {
        char* pBuddy = &Buddies[16*iBuddy];
        s32 BuddyLen = x_strlen( pBuddy );

        // Skip check if buddy string length is 0
        if( BuddyLen > 0 )
        {
            // Upper case it
            x_strtoupper( pBuddy );

            // Run through all players
            for( s32 iPlayer=0 ; iPlayer<32 ; iPlayer++ )
            {
                // Connected human player?
                if( Score.Player[iPlayer].IsConnected &&
                    !Score.Player[iPlayer].IsBot )
                {
                    PlayerName = Score.Player[iPlayer].Name;
                    PlayerName.MakeUpper();
                    if( x_strstr( PlayerName, pBuddy ) )
                    {
                        packet.ServerInfo.Flags |= SVR_FLAGS_HAS_BUDDY;
                        done = TRUE;
                        break;
                    }
                }
            }
        }
    }

    if (x_wstrlen(fegl.ServerSettings.AdminPassword) > 0)
    {
        packet.ServerInfo.Flags |= SVR_FLAGS_HAS_PASSWORD;
    }

    if( fegl.ServerSettings.TargetLockEnabled )
    {
        packet.ServerInfo.Flags |= SVR_FLAGS_HAS_TARGETLOCK;
    }

	if (fegl.ModemConnection)
	{
		packet.ServerInfo.Flags |= SVR_FLAGS_HAS_MODEM;
	}

#if defined( bwatson ) || defined( RELEASE_CANDIDATE )
    char ipstr[64];
    char ipstr2[64];

    net_IPToStr(Remote.IP,ipstr);
    net_IPToStr(Local.IP,ipstr2);
    x_DebugMsg("Lookup response sent to %s:%d via %s:%d\n",ipstr,Remote.Port,ipstr2,Local.Port);
#endif
    net_Send( Local, Remote, &packet, sizeof(lookup_response) );
}

//-----------------------------------------------------------------------------
xbool server_manager::ParseNetLookupResponse(lookup_response *pResponse,const net_address& Remote)
{
    server_info*     pInfo    = NULL;

	//
	// We have received a response to a server lookup request. Now insert the new
	// server in to our list. We also know the ping time by default since we sent
    // out the timer information with the original packet
	//
	s32 found;
	s32 index;

	found = FALSE;
	for (index=0;index<tgl.pMasterServer->GetEntityCount();index++)
	{

        pInfo = tgl.pMasterServer->GetEntityInfo(index);
		if ( ((u32)pInfo->Info.IP == Remote.IP) &&
			 ((u32)pInfo->Info.Port == Remote.Port) )
		{
			found = TRUE;
            break;
		}
	}

    if (!found)
    {
        return FALSE;
    }

    // ignore packets from different versions
    if ( pResponse->Info.Version != SERVER_CURRENT_VERSION )
    {
        // Remove the server from the entity list
        tgl.pMasterServer->DeleteEntity(index);
        return FALSE;
    }

    pInfo->ServerInfo     = pResponse->ServerInfo;
	pInfo->Info.IP        = Remote.IP;
	pInfo->Info.Port      = Remote.Port;
    pInfo->RefreshTimeout = SERVER_LIFETIME;
    pInfo->PingTime       = GetSequenceTime(pResponse->Info.SequenceNumber);
	g_ServerListChanged = TRUE;

    return TRUE;
}

//-----------------------------------------------------------------------------
xbool server_manager::ParseLookupResponse(lookup_response *pResponse)
{
    // ignore packets from different versions
    if ( pResponse->Info.Version != SERVER_CURRENT_VERSION )
        return FALSE;
	//
	// We have received a response to a server lookup request. Now insert the new
	// server in to our list. We also know the ping time by default since we sent
    // out the timer information with the original packet
	//
	s32 found;
	s32 index;

	found = FALSE;
    server_info* pServer;

    pServer = NULL;

	for (index=0;index<m_Servers.GetCount();index++)
	{
		if ( (m_Servers[index].Info.IP == pResponse->Info.IP) &&
			 (m_Servers[index].Info.Port == pResponse->Info.Port) )
		{
			found = TRUE;
            pServer = &m_Servers[index];
            break;
		}
	}

    if (!found)
    {
        pServer = &m_Servers.Append();
    }

	g_ServerListChanged = TRUE;
    pServer->Info.IP        = pResponse->Info.IP;
	pServer->Info.Port      = pResponse->Info.Port;
    pServer->RefreshTimeout = SERVER_LIFETIME;
    pServer->PingTime       = GetSequenceTime(pResponse->Info.SequenceNumber);
    pServer->ServerInfo     = pResponse->ServerInfo;
#ifdef TEST_SERVER_LIST
	if (!found)
	{
		s32 i;

		for (i=0;i<20;i++)
		{
			server_info* pServer2 = &m_Servers.Append();
			*pServer2 = *pServer;
			pServer2->Info.Port =-
			pServer2->Info.Port--;
			pServer2->PingTime = (x_rand()%250)+50;
			pServer2->ServerInfo.Name[0] =xwchar('A'+x_rand()%32);
		}
	}
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
void server_manager::SendLookup(const s32 type,const net_address &Local,const net_address &Remote)
{
    lookup_request packet;

    if ( ( m_Local.IP == 0) || (m_Broadcast.IP == 0) )
        return;
    packet.Info.Type = type;
    packet.Info.IP   = m_Local.IP;
    packet.Info.Port = m_Local.Port;
    packet.Info.SequenceNumber = GetSequenceNumber();
    packet.Info.Version = SERVER_CURRENT_VERSION;
    packet.BroadcastPort = m_Broadcast.Port;
    ASSERT( x_strlen( m_BuddySearchString ) < SEARCH_STRING_LENGTH );
    x_strncpy( packet.BuddySearchString, m_BuddySearchString, SEARCH_STRING_LENGTH );
    packet.BuddySearchString[SEARCH_STRING_LENGTH-1] = 0;
    net_Send(Local,Remote,&packet,sizeof(lookup_request)-(SEARCH_STRING_LENGTH-x_strlen(packet.BuddySearchString))+1);
}

//-----------------------------------------------------------------------------
xbool server_manager::HasChanged(void)
{
    return m_Changed;
}

//-----------------------------------------------------------------------------
void server_manager::ClearChanged(void)
{
    m_Changed = FALSE;
}

//-----------------------------------------------------------------------------
s32 server_manager::GetSequenceNumber(void)
{
    s32 index;

    // We want a new sequence number, this can happen when a request is sent to
    // a server that is returned from the master server or from a local subnet
    // lookup. Since we only have a finite number of entries in the table, we
    // only want to advance when necessary. We should have enough slots for
    // one second worth of requests but this will typically be much longer than
    // a second since sequence advancements should occur less frequently than
    // every game cycle.
    if (smem_GetActiveID() != m_CurrentActiveId)
    {
        m_CurrentActiveId = smem_GetActiveID();
        // Stop the old timer
        index = (u16)m_SequenceNumber % MAX_SERVER_TIME_SEQUENCES;
        m_SequenceTimer[index].Stop();
        // Advance sequence number and start the timer for
        // that sequence.
        m_SequenceNumber++;
        index = (u16)m_SequenceNumber % MAX_SERVER_TIME_SEQUENCES;
        m_SequenceTimer[index].Reset();
        m_SequenceTimer[index].Start();
        m_TimerSequence[index] = m_SequenceNumber;
    }
    return m_SequenceNumber;
}

//-----------------------------------------------------------------------------
f32 server_manager::GetSequenceTime(s32 SequenceNumber)
{
    s32 index;

    index = (u16)SequenceNumber % MAX_SERVER_TIME_SEQUENCES;

    if (m_TimerSequence[index] != SequenceNumber)
    {
        return 0.0f;
    }

    return m_SequenceTimer[index].ReadMs();

}
