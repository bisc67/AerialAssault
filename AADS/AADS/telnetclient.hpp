#ifndef __TELNET_CLIENT_HPP
#define __TELNET_CLIENT_HPP

#include "telnetdisplay.hpp"
#include "x_threads.hpp"
#include "support/gamemgr/gamemgr.hpp"
#include "gameserver.hpp"

#define TELNET_LOG_HISTORY 20


class telnet_client
{
public:
	void			Init			(const net_address& Local);
	void			Kill			(void);
	void			Main			(void);
	xbool			Update			(f32 DeltaTime);
	void			Refresh			(void);
	void			AddLogText		(const f32 time, const char* pString);
private:

	enum disp_mode
	{
		DISP_MODE_PLAYERS_1=0,
		DISP_MODE_PLAYERS_2,
		DISP_MODE_CLIENTS_1,
		DISP_MODE_CLIENTS_2,
		DISP_MODE_SERVER,

		DISP_MODE_LAST,
	};

	net_address		m_Local;
	net_address		m_Remote;
	xthread*		m_pThread;

	// Authentication
	xbool			m_Authenticated;
	s32				m_Retries;
	char			m_Username[TELNET_LINE_LENGTH];
	char			m_Password[TELNET_LINE_LENGTH];

	xbool			m_Exit;

	// Display
	f32				m_UpdateInterval;
	f32				m_RefreshInterval;
	xtimer			m_RefreshTimer;
	telnet_display	m_Display;
	s32				m_DisplayMode;
	s32				m_LastDisplayMode;
	telnet_display::window* m_pInfoWindow;
	telnet_display::window* m_pInputWindow;
	telnet_display::window* m_pLogWindow;

	s32				m_ClientCount;
	s32				m_LastBytesReceived;
	s32				m_LastBytesSent;
	s32				m_LastPacketsReceived;
	s32				m_LastPacketsSent;
	s32				m_LastBytesIn[MAX_CLIENTS];
	s32				m_LastBytesOut[MAX_CLIENTS];

	char			m_LogText[TELNET_LOG_HISTORY][80];
	f32				m_LogTextTime[TELNET_LOG_HISTORY];
	s32				m_LogTextIndex;

	// Control
	char			WaitForCommand	(const char* Label, 
									 const char* Commands,
									 const char** Help,
									 const telnet_color foreground=WHITE,
									 const telnet_color background=BLACK);
	s32				GetValue		(const char* pLabel);
	void			Stop			(void);
	// Command processing functions
	void			KickPlayer		(void);
	void			SwitchTeam		(void);
	void			SwitchMap		(void);
	void			SetClientCount	(void);
	void			SetPlayerCount	(void);
	void			SetBotCount		(void);
	void			SetDuration		(void);
	void			ShutdownServer	(void);

	xbool			RefreshPlayers	(s32 PlayerIndex);
	xbool			RefreshClients	(s32 ClientIndex);
	void			RefreshServer	(void);
	void			SampleGameState	(void);
	void			RefreshLogText	(void);
	s32				Dialog			(const char* pTitle, const char** pOptions);


friend telnet_display;
};



#endif