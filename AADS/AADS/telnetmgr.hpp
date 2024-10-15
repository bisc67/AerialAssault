#ifndef __TELNETMGR_HPP
#define __TELNETMGR_HPP

#include "netlib/netlib.hpp"
#include "x_threads.hpp"
#include "telnetclient.hpp"

#define TELNET_LISTEN_PORT 49152
#define TELNET_MAX_CLIENTS	2

class telnet_manager
{
public:
	void			Init			(void);
	void			Kill			(void);
	void			Update			(f32 DeltaTime);
	void			AddDebugText	(const xwchar* pString);
	void			AddDebugText	(const char* pString);
private:
	telnet_client*	m_Clients[TELNET_MAX_CLIENTS];
	net_address		m_Local;
	net_address		m_TelnetPort;
};

extern telnet_manager g_TelnetManager;

#endif