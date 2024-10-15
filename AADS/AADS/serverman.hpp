#ifndef SERVERMAN_HPP
#define SERVERMAN_HPP

#include "serverversion.hpp"
#include "netlib/netlib.hpp"
#include "x_time.hpp"
#include "objects/player/playerobject.hpp"
#include "specialversion.hpp"

#define SERVER_CURRENT_VERSION  ( SERVER_VERSION )

#define PKT_LOOKUP              (0)
#define PKT_RESPONSE            (1)
#define PKT_NET_LOOKUP          (2)
#define PKT_NET_RESPONSE        (3)
#define PKT_EXT_LOOKUP          (4)
#define PKT_EXT_RESPONSE        (5)

#define MAX_SERVER_NAME_LENGTH  (16)
#define SERVER_POLL_INTERVAL    (8.0f)
#define BROADCAST_PORT          (27999)
#define MIN_USER_PORT           (28000)
#define MAX_USER_PORT           (32767)
#define SERVER_LIFETIME         (SERVER_POLL_INTERVAL * 4.0f)
#define SERVER_GAME_NAME_LENGTH (16)
#define PLAYER_NAME_LENGTH      (32)
#define SEARCH_STRING_LENGTH    (160)
#define MAX_PLAYER_COUNT        (16)
#define MAX_SERVER_TIME_SEQUENCES (32)

// Flags field bitdefs
#define SVR_FLAGS_HAS_BUDDY         (1<<0)  // Has a match on the player search string
#define SVR_FLAGS_HAS_PASSWORD      (1<<1)  // Has a password set on this server
#define SVR_FLAGS_HAS_TARGETLOCK    (1<<2)  // Has target lock set on this server
#define SVR_FLAGS_IS_DEDICATED      (1<<3)  // Server is dedicated
#define SVR_FLAGS_HAS_MODEM			(1<<4)	// Server is connected on a modem

// Base information sent in a response
struct base_info
{
    s16         Type;
    s16         Version;
    s16         SequenceNumber;         // Sequence number of this packet. This will be used as an index to xtimer structs
    s16         Port;
    s32         IP;
};

// Basic information about a server sent in a response and stored locally
struct base_server
{
    xwchar      Name[MAX_SERVER_NAME_LENGTH];
    char        LevelName[16];          // Current level number playing
    s8          MaxPlayers;             // Max numbers of players that can connect
    s8          CurrentPlayers;         // Current count of players connected
    s8          BotCount;               // Number of bots playing
    s8          GameType;               // game_type enum converted to byte
    s8          AmountComplete;         // Parametric converted to 0..100
    u8          Flags;                  // Various flags, as yet undefined.
	s8			MaxClients;				// Max # of clients that can connect
	s8			ClientCount;			// Current client count connected
};

struct server_info
{
    base_info   Info;
    base_server ServerInfo;
    f32         PingTime;
    f32         RefreshTimeout;
	s32			Retries;
};

struct lookup_request
{
    base_info   Info;
    s32         BroadcastPort;
    char        BuddySearchString[SEARCH_STRING_LENGTH];
};

struct lookup_response
{
    base_info   Info;
    base_server ServerInfo;
};

struct extended_lookup_response
{
    base_info   Info;
    base_server ServerInfo;
    s8          PlayerNames[1][PLAYER_NAME_LENGTH];
};

class server_manager
{
public:
                    server_manager(void);
                    ~server_manager(void);

    void            Reset                   ( void );
    server_info*    GetServer               ( s32 index );
    s32             GetCount                ( void );
    s32             Refresh                 ( f32 time_delta );
    void            SetAsServer             ( xbool on );
    void            SetName                 ( const xwchar* pName );
    xwchar*         GetName                 ( void )                 { return m_Name; }
    xbool           HasChanged              ( void );
    void            ClearChanged            ( void );
    void            SetLocalAddr            ( net_address& Addr )   { m_Local = Addr; }
    net_address     GetLocalAddr            ( void )                { return m_Local; }
    s32             GetSequenceNumber       (void);
    f32             GetSequenceTime         ( s32 SequenceNumber);
    void            SetIsInGame             ( xbool Tmp )           { m_IsInGame = Tmp; }
    xbool           GetIsInGame             ( void )                { return m_IsInGame; }
    void            SendLookup              ( const s32 type,const net_address &Local,const net_address &Remote);
    xbool           ParsePacket             ( void* pBuff , const net_address &Local, const net_address &Remote);
    void            SetSearchString         ( const char* pBuddyString);
    const char*     GetSearchString         ( void )                { return m_BuddySearchString;}
	s32				GetLookupRequests		( void )				{ return m_LookupRequests;}
	s32				GetUniqueCount			( void )				{ return m_UniqueList.GetCount();}
private:
    xarray<server_info> m_Servers;
    f32                 m_PollDelay;
    net_address         m_Broadcast;
    net_address         m_Local;
    net_address         m_Recipient;
    xbool               m_IsServer;
    xbool               m_Changed;
    xbool               m_IsInGame;
    xwchar              m_Name[MAX_SERVER_NAME_LENGTH];
    s16                 m_SequenceNumber;
    s32                 m_CurrentActiveId;
    xtimer              m_SequenceTimer[MAX_SERVER_TIME_SEQUENCES];
    s16                 m_TimerSequence[MAX_SERVER_TIME_SEQUENCES];
    char                m_BuddySearchString[SEARCH_STRING_LENGTH];
	xarray<net_address>	m_UniqueList;
	s32					m_LookupRequests;

    void            ParseLookupRequest      (lookup_request *pRequest,const net_address &Local, const net_address &Remote);
    xbool           ParseLookupResponse     (lookup_response *pResponse);
    xbool           ParseNetLookupResponse  (lookup_response *pResponse,const net_address& Remote);
};

#endif
