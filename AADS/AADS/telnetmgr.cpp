#include "x_files.hpp"
#include "telnetmgr.hpp"


telnet_manager g_TelnetManager;
// We only want to do this on the PC since we'll be using some additional windows specific calls to
// deal with a tcp port connection
#if !defined(TARGET_PC)

#error This is a PC only file. Please exclude it from the build rules

#endif

void telnet_manager::Init(void)
{
	s32 StartPort;

	m_TelnetPort.Clear();

	StartPort = TELNET_LISTEN_PORT;
	// Open the telnet port. TCP packet type
    struct sockaddr_in addr;
    SOCKET sd_dg;

    // create an address and bind it
    x_memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family = PF_INET;
    addr.sin_port = htons(TELNET_LISTEN_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // create a socket
    sd_dg = socket( PF_INET, SOCK_STREAM, 0 );

    // attempt to bind to the port
    while ( bind( sd_dg, (struct sockaddr *)&addr, sizeof(addr) ) == SOCKET_ERROR )
    {
        // increment port if that port is assigned
        if ( WSAGetLastError() == WSAEADDRINUSE )
        {
            StartPort++;
            addr.sin_port = htons( StartPort );
        }
        // if some other error, nothing we can do...abort
        else
        {
            return;
            //StartPort = net_NOSLOT;
            //break;
        }
    }

#if 1
    // set this socket to non-blocking, so we can poll it
    u_long  dwNoBlock = TRUE;
    ioctlsocket( sd_dg, FIONBIO, &dwNoBlock );
#endif
	listen(sd_dg,TELNET_MAX_CLIENTS);

    // fill out the slot structure
    // first, determine local IP address
    char tbuf[128];
    hostent* pHe;
    struct in_addr in_IP;

    gethostname( tbuf, 127 );
    pHe = gethostbyname( tbuf );

    in_IP = *(struct in_addr *)(pHe->h_addr);

    m_TelnetPort.IP = in_IP.s_addr;
    m_TelnetPort.Port = StartPort;//htons( StartPort );
    m_TelnetPort.Socket = (u32)sd_dg;


}

void telnet_manager::Kill(void)
{
	s32 i;

	for (i=0;i<TELNET_MAX_CLIENTS;i++)
	{
		if (m_Clients[i])
		{
			m_Clients[i]->Kill();
			delete m_Clients[i];
			m_Clients[i] = NULL;
		}
	}
}

void telnet_manager::Update(f32 DeltaTime)
{
	s32 i;
    struct sockaddr addr;
	s32 size;
	net_address incoming;


	// Has a new connection been established?
	size = sizeof(addr);
	incoming.Socket = accept(m_TelnetPort.Socket,&addr,&size);

	if (incoming.Socket != INVALID_SOCKET)
	{
		incoming.IP = -1;
		incoming.Port = -1;

		for (i=0;i<TELNET_MAX_CLIENTS;i++)
		{
			if (!m_Clients[i])
			{
				m_Clients[i] = new telnet_client;
				m_Clients[i]->Init(incoming);
				break;
			}
		}
		if (i==TELNET_MAX_CLIENTS)
		{
			closesocket(incoming.Socket);
		}
	}

	for (i=0;i<TELNET_MAX_CLIENTS;i++)
	{
		if (m_Clients[i])
		{
			if (m_Clients[i]->Update(DeltaTime))
			{
				m_Clients[i]->Kill();
				delete m_Clients[i];
				m_Clients[i] = NULL;
			}
		}
	}
}

void telnet_manager::AddDebugText(const char* pString)
{
	s32 i;

	for (i=0;i<TELNET_MAX_CLIENTS;i++)
	{
		if (m_Clients[i])
		{
			m_Clients[i]->AddLogText((f32)x_GetTimeSec(),pString);
		}
	}
}

void telnet_manager::AddDebugText(const xwchar* pString)
{
	s32 i;
	char Buffer[80];
	char*pBuffer;

	// Convert a wide string to narrow and exclude special characters
	pBuffer = Buffer;

	i=0;
	while (*pString && (i<sizeof(Buffer)) )
	{
		if ( (*pString >=0) && (*pString<128) )
		{
			*pBuffer++=(char)*pString;
			i++;
		}
		pString++;
	}
	*pBuffer=0x0;
	AddDebugText(Buffer);
}

