/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.4.2
 */
/*
 *                     INET Library
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                         libnet - libnet.h
 *
 *
 * $Id: libnet.h,v 1.17 2001/10/25 07:05:15 ksh Exp $
 */

#ifndef _LIBNET_H_DEFS
#define _LIBNET_H_DEFS

#ifndef _MSIFRPC_H_DEFS
#include <libmrpc.h>
#endif

/*
 * inet.h
 */

#if defined(__cplusplus)
extern "C" {
#endif

/* << INET API >> */

typedef struct sceInetAddress {
	int reserved;	/* must be zero */
	char data[12];	/* IP address (4 bytes) + reserved (8 bytes) */
} sceInetAddress_t;

typedef struct sceInetParam {
	int type;
	int local_port;
	struct sceInetAddress remote_addr;
	int remote_port;
	int reserved[9];	/* must be zero */
} sceInetParam_t;

/* type for struct InetParam */
#define sceINETT_DGRAM		0x0	/* UDP */
#define sceINETT_CONNECT	0x1	/* TCP, Active-Open */
#define sceINETT_LISTEN		0x2	/* TCP, Passive-Open */
#define sceINETT_RAW		0x3	/* IP */

/* port for struct InetParam */
#define sceINETP_AUTO		-1
#define sceINETP_ANY		0

typedef struct sceInetInfo {
	int cid;
	int proto;
	int recv_queue_length;
	int send_queue_length;
	struct sceInetAddress local_adr;
	int local_port;
	struct sceInetAddress remote_adr;
	int remote_port;
	int state;
	int reserved[4];
} sceInetInfo_t;

/* proto for sceInetInfo */
#define sceINETI_PROTO_TCP	1	/* CONNECT,LISTEN */
#define sceINETI_PROTO_UDP	2	/* DGRAM */
#define sceINETI_PROTO_IP	3	/* RAW */

/* state for sceInetInfo */
#define sceINETI_STATE_UNKNOWN		0	/* all */
#define sceINETI_STATE_CLOSED		1	/* all */
#define sceINETI_STATE_CREATED		2	/* UDP */
#define sceINETI_STATE_OPENED		3	/* UDP,RAW */
#define sceINETI_STATE_LISTEN		4	/* CONNECT,LISTEN */
#define sceINETI_STATE_SYN_SENT		5	/* CONNECT,LISTEN */
#define sceINETI_STATE_SYN_RECEIVED	6	/* CONNECT,LISTEN */
#define sceINETI_STATE_ESTABLISHED	7	/* CONNECT,LISTEN */
#define sceINETI_STATE_FIN_WAIT_1	8	/* CONNECT,LISTEN */
#define sceINETI_STATE_FIN_WAIT_2	9	/* CONNECT,LISTEN */
#define sceINETI_STATE_CLOSE_WAIT	10	/* CONNECT,LISTEN */
#define sceINETI_STATE_CLOSING		11	/* CONNECT,LISTEN */
#define sceINETI_STATE_LAST_ACK		12	/* CONNECT,LISTEN */
#define sceINETI_STATE_TIME_WAIT	13	/* CONNECT,LISTEN */

int sceInetName2Address(sceSifMClientData *cd, u_int *buf, int flags,
	struct sceInetAddress *paddr, char *name, int ms, int nretry);
int sceInetAddress2String(sceSifMClientData *cd, u_int *buf, char *name, int len, struct sceInetAddress *paddr);
int sceInetAddress2Name(sceSifMClientData *cd, u_int *buf, int flags, char *name, int len, struct sceInetAddress *paddr, int ms, int nretry);
int sceInetCreate(sceSifMClientData *cd, u_int *buf, struct sceInetParam *param);
int sceInetOpen(sceSifMClientData *cd, u_int *buf, int cid, int ms);
int sceInetClose(sceSifMClientData *cd, u_int *buf, int cid, int ms);
int sceInetRecv(sceSifMClientData *cd, u_int *buf, int cid, void *ptr, int count, int *pflags, int ms);
int sceInetRecvFrom(sceSifMClientData *cd, u_int *buf, int cid, void *ptr, int count, int *pflags, struct sceInetAddress *remote_adr, int *premote_port, int ms);
int sceInetSend(sceSifMClientData *cd, u_int *buf, int cid, void *ptr, int count, int *pflags, int ms);
int sceInetSendTo(sceSifMClientData *cd, u_int *buf, int cid, void *ptr, int count, int *pflags, struct sceInetAddress *remote_adr, int remote_port, int ms);
int sceInetAbort(sceSifMClientData *cd, u_int *buf, int cid, int flags);
int sceInetControl(sceSifMClientData *cd, u_int *buf, int cid, int code, void *ptr, int len);

/* flags for InetRecv/InetSend */
#define sceINETF_PSH		0x01	/* TCP Push   (Recv-Out, Send-In) */
#define sceINETF_URG		0x02	/* TCP Urgent (Recv-Out, Send-In) */
#define sceINETF_FIN		0x04	/* TCP Fin    (Recv-Out) */
#define sceINETF_TIM		0x08	/* Timeout    (Recv-Out, Send-Out) */
#define sceINETF_ONE		0x10	/* One-Byte   (Recv-In) */
#define sceINETF_ABT		0x20	/* Abort      (Recv-Out, Send-Out) */

/* flags for sceInetName2Address() */
#define sceInetN2AF_NoDot	0x01	/* disable dot format conversion */
#define sceInetN2AF_NoHosts	0x02	/* disable refer to hosts table */
#define sceInetN2AF_NoDNS	0x04	/* disable using DNS */

/* code for sceInetControl() */
#define sceINETC_CODE_GET_INFO	1
#define sceINETC_CODE_GET_FLAGS	2
#define sceINETC_CODE_SET_FLAGS	3

/* flags for sceINETC_{SET,GET}_FLAGS */
#define sceINETC_FLAGS_NODELAY	0x01	/* TCP no delay */

/* error codes - InetXXX() */
#define sceINETE_OK				0
#define sceINETE_TIMEOUT			-500
#define sceINETE_ABORT				-501
#define sceINETE_BUSY				-502
#define sceINETE_LINK_DOWN			-503
#define sceINETE_INSUFFICIENT_RESOURCES		-504
#define sceINETE_LOCAL_SOCKET_UNSPECIFIED	-505
#define sceINETE_FOREIGN_SOCKET_UNSPECIFIED	-506
#define sceINETE_CONNECTION_ALREADY_EXISTS	-507
#define sceINETE_CONNECTION_DOES_NOT_EXIST	-508
#define sceINETE_CONNECTION_CLOSING		-509	/* TCP */
#define sceINETE_CONNECTION_RESET		-510	/* TCP */
#define sceINETE_CONNECTION_REFUSED		-511	/* TCP */
#define sceINETE_INVALID_ARGUMENT		-512
#define sceINETE_INVALID_CALL			-513
#define sceINETE_NO_ROUTE			-514

/* << INET Control I/F >> */

typedef struct sceInetRoutingEntry {
	struct sceInetAddress dstaddr;
	struct sceInetAddress gateway;
	struct sceInetAddress genmask;
	int flags;
	int mss;
	int window;
	char interface[8 + 1];
} sceInetRoutingEntry_t;

/* flags of struct sceInetRoutingEntry */
#define sceInetRoutingF_Up		0x01	/* route is up */
#define sceInetRoutingF_Host		0x02	/* target is host */
#define sceInetRoutingF_Gateway		0x04	/* use gateway */
#define sceInetRoutingF_Dynamic		0x08	/* dynamically installed */
#define sceInetRoutingF_Modified	0x10	/* modified */

/* code of sceInetInterfaceControl() */
#define sceInetCC_GetInterfaceName	0x00000000	/* interface */
#define sceInetCC_GetModuleName		0x00000001	/* module_name */
#define sceInetCC_GetVendorName		0x00000002	/* vendor_name */
#define sceInetCC_GetDeviceName		0x00000003	/* device_name */
#define sceInetCC_GetBusType		0x00000004	/* bus_type */
#define sceInetCC_GetBusLoc		0x00000005	/* bus_loc */
#define sceInetCC_GetProt		0x00000006	/* prot */
#define sceInetCC_GetImpl		0x00000007	/* impl */
#define sceInetCC_GetFlags		0x00000008	/* flags */
#define sceInetCC_GetAddress		0x00000009	/* ip_addr */
#define sceInetCC_GetNetmask		0x0000000a	/* ip_mask */
#define sceInetCC_GetBroadcast		0x0000000b	/* broad_adr */
#define sceInetCC_GetMTU		0x0000000c	/* MTU */
#define sceInetCC_GetHWaddr		0x0000000d	/* hw_addr */
#define sceInetCC_GetDHCPHostName	0x0000000e	/* DHCP hostname */
#define sceInetCC_GetDHCPFlags		0x0000000f	/* dhcp_flags */

#define sceInetCC_NETDEV		0x80000000
#define sceInetCC_MODEM			0x40000000

/*
 0x00000000 - 0x7fffffff  INET layer
 0x80000000 - 0x8fffffff  NETDEV layer (common)
 0x90000000 - 0xbfffffff  NETDEV layer (depends by implementation) 
 0xc0000000 - 0xcfffffff  MODEM layer (common)
 0xd0000000 - 0xffffffff  MODEM layer (depends by implementation) 
*/

int sceInetGetInterfaceList(sceSifMClientData *cd, u_int *buf, int *interface_id_list, int n);
int sceInetInterfaceControl(sceSifMClientData *cd, u_int *buf, int interface_id, int code, void *ptr, int len);
int sceInetGetRoutingTable(sceSifMClientData *cd, u_int *buf, struct sceInetRoutingEntry *p, int n);
int sceInetGetNameServers(sceSifMClientData *cd, u_int *buf, struct sceInetAddress *paddr, int n);
int sceInetChangeThreadPriority(sceSifMClientData *cd, u_int *buf, int prio);
int sceInetGetLog(sceSifMClientData *cd, u_int *net_buf, char *buf, int len, int ms);
int sceInetAbortLog(sceSifMClientData *cd, u_int *buf);

#if defined(__cplusplus)
}
#endif

/*
 * in.h
 */

#define IPADR_BROAD		0xffffffffL	/* 255.255.255.255 */
#define IPADR_ANY		0x00000000L	/* 0.0.0.0 */

#define PORT_ANY		0
#define PORT_ECHO		7
#define PORT_DISCARD		9
#define PORT_DAYTIME		13
#define PORT_CHARGEN		19
#define PORT_FTP_DATA		20
#define PORT_FTP		21
#define PORT_TELNET		23
#define PORT_DOMAIN		53
#define PORT_BOOTPS		67
#define PORT_BOOTPC		68
#define PORT_TFTP		69
#define PORT_USER		5000
#define PORT_MAX		65535

#define dump_byte(p, size)  ({ \
    int i; \
	for(i = 0; i < (size); i++) \
	printf("%02x", *((char *)(p) + i) & 0xff); \
	printf("\n"); \
	})

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN

#ifndef ntohl
#define ntohl(v)	(v)
#endif
#ifndef ntohs
#define ntohs(v)	(v)
#endif
#ifndef ntohf
#define ntohf(v)	(v)
#endif

#else

#ifndef ntohs
#define ntohs(sv)	({			\
	u_short _sv = (sv);			\
	(u_short)((_sv << 8) | (_sv >> 8));	\
})
#endif

#ifndef ntohl
#define ntohl(lv)	(u_int)({		\
	u_int _lv = (lv);			\
	(ntohs(_lv) << 16) | ntohs(_lv >> 16);	\
})
#endif

#ifndef ntohf
#define ntohf(f) ({ \
	u_int l; \
	l = ntohl(*(u_int *)&(f)); \
	memcpy(&(f), &l, sizeof(float)); \
	})
#endif

#endif

#ifndef htonl
#define htonl(v)	ntohl(v)
#endif
#ifndef htons
#define htons(v)	ntohs(v)
#endif
#ifndef htonf
#define htonf(v)	ntohf(v)
#endif

#define put2byte(p, v)	({ 		\
	u_char *_p = (u_char *)(p); 	\
	u_short _v = (u_short)(v); 	\
	*(_p + 0) = _v >>  8; 		\
	*(_p + 1) = _v >>  0; 		\
	_p + 2; 			\
})

#define put4byte(p, v)	({ 		\
	u_char *_p = (u_char *)(p);	\
	u_int _v = (u_int)(v);	\
	*(_p + 0) = _v >> 24;		\
	*(_p + 1) = _v >> 16;		\
	*(_p + 2) = _v >>  8;		\
	*(_p + 3) = _v >>  0;		\
	_p + 4;				\
})

#define get2byte(p)	({		\
	u_char *_p = (u_char *)(p);	\
	(*(_p + 0) <<  8)		\
		| (*(_p + 1) <<  0);	\
})

#define get4byte(p)	({		\
	u_char *_p = (u_char *)(p);	\
	(*(_p + 0) << 24)		\
		| (*(_p + 1) << 16)	\
		| (*(_p + 2) <<  8)	\
		| (*(_p + 3) <<  0);	\
})

#if defined(iop)

#define get4u(p)	({		\
	u_char *_p = (u_char *)(p);	\
	(*(_p + 0) << 0)		\
		| (*(_p + 1) <<  8)	\
		| (*(_p + 2) << 16)	\
		| (*(_p + 3) << 24);	\
})

#define put4u(p, v)	({ 		\
	u_char *_p = (u_char *)(p);	\
	u_int _v = (u_int)(v);	\
	*(_p + 0) = _v >>  0;		\
	*(_p + 1) = _v >>  8;		\
	*(_p + 2) = _v >> 16;		\
	*(_p + 3) = _v >> 24;		\
	_p + 4;				\
})

#else

#define get4u(p)	(*(u_int *)(p))
#define put4u(p, v)	(*(u_int *)(p) = (v))
	
#endif

#if defined(__cplusplus)
}
#endif

/*
 * netdev.h
 */

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct sceInetPkt {
	struct sceInetPkt *forw, *back;
	int reserved[2];
	u_char *rp, *wp;
} sceInetPkt_t;

typedef struct sceInetPktQ {
	struct sceInetPkt *head, *tail;
} sceInetPktQ_t;

typedef struct sceInetDevOps {
	struct sceInetDevOps *forw, *back;	/* links for INET layer */
	char interface[8 + 1];			/* interface name */
	char *module_name;			/* module name */
	char *vendor_name;			/* vendor name of device */
	char *device_name;			/* device name of device */
	u_char bus_type;			/* bus type */
	u_char bus_loc[31];			/* bus location */
	u_short prot_ver;			/* protocol version */
	u_short impl_ver;			/* implement version */
	void *priv;				/* private for NETDEV layer */
	int flags;				/* various flags */
	int evfid;				/* event flag ID */
	struct sceInetPktQ rcvq;		/* receive queue */
	struct sceInetPktQ sndq;		/* send queue */
	int (*start)(void *priv, int flags);	/* start function */
	int (*stop)(void *priv, int flags);	/* stop function */
	int (*xmit)(void *priv, int flags);	/* transmit function */
	int (*control)(void *priv, int code, void *ptr, int len);
						/* control function */
	u_long ip_addr;				/* IP address */
	u_long ip_mask;				/* IP subnet mask */
	u_long broad_addr;			/* IP broadcast address */
	u_long gw_addr;				/* gateway address */
	u_long ns_addr1;			/* primary DNS address */
	int mtu;				/* MTU */
	u_char hw_addr[16];			/* hardware address */
	u_char dhcp_hostname[256];		/* host name for DHCP */
	int dhcp_hostname_len;			/* length of host name */
	int dhcp_flags;				/* flags for DHCP */
	void *reserved[4];			/* reserved */
	u_long ns_addr2;			/* secondary DNS address */
	void *pppoe_priv;			/* private for PPPoE */
} sceInetDevOps_t;

/* Bus type */
#define sceInetBus_Unknown	0
#define sceInetBus_USB		1
#define sceInetBus_1394		2	/* -> reserved only */
#define sceInetBus_PCMCIA	3	/* -> reserved only */
#define sceInetBus_PSEUDO	4
#define sceInetBus_NIC		5

/* prot_ver */
#define sceInetDevProtVer	2	/* current protocol version */
	/* 0->1: add dhcp_hostname,dhcp_hostname_len */
	/* 1->2: rename ns_addr to ns_addr1, add ns_addr2, dhcp_flags */

/* flags */
#define sceInetDevF_Up		0x0001	/* interface is up */
#define sceInetDevF_Running	0x0002	/* interface is running */
#define sceInetDevF_Broadcast	0x0004	/* broadcast address is valid */
#define sceInetDevF_ARP		0x0010	/* ARP */
#define sceInetDevF_DHCP	0x0020	/* DHCP */
#define sceInetDevF_PPP		0x0040	/* PPP */
#define sceInetDevF_NIC		0x0080	/* NIC */
#define sceInetDevF_Error	0x0100	/* Error */

/* Event Flags Patterns */
#define sceInetDevEFP_StartDone	0x00000001	/* start process complete */
#define sceInetDevEFP_PlugOut	0x00000002	/* device is plug-out */
#define sceInetDevEFP_Recv	0x00000004	/* packet received */
#define sceInetDevEFP_Error	0x00000010	/* error */
#define sceInetDevEFP_TimeOut	0x00000020	/* timeout error */
#define sceInetDevEFP_InetUse	0xffff0000	/* used by INET layer */

/* DHCP flags (dhcp_flags) */
#define sceInetDevDHCP_RelOnStop	0x00000001	/* release on stop */

#if defined(__cplusplus)
}
#endif

/* code for control() */
/*   (I/F independent) */
#define	sceInetNDCC_GET_THPRI	0x80000000	/* get thread priority */
#define	sceInetNDCC_SET_THPRI	0x81000000	/* set thread priority */
#define	sceInetNDCC_GET_IF_TYPE	0x80000100	/* get I/F type */
#define	sceInetNDIFT_GENERIC	0x00000000	/* generic */
#define	sceInetNDIFT_ETHERNET	0x00000001	/* Ethernet */
#define	sceInetNDIFT_PPP	0x00000002	/* PPP */
#define	sceInetNDCC_GET_RX_PACKETS	0x80010000	/* rx packets */
#define	sceInetNDCC_GET_TX_PACKETS	0x80010001	/* tx packets */
#define	sceInetNDCC_GET_RX_BYTES	0x80010002	/* rx bytes */
#define	sceInetNDCC_GET_TX_BYTES	0x80010003	/* tx bytes */
#define	sceInetNDCC_GET_RX_ERRORS	0x80010004	/* rx errors */
#define	sceInetNDCC_GET_TX_ERRORS	0x80010005	/* tx errors */
#define	sceInetNDCC_GET_RX_DROPPED	0x80010006	/* rx dropped */
#define	sceInetNDCC_GET_TX_DROPPED	0x80010007	/* tx dropped */

/*   (Ethernet I/F) */
#define sceInetNDCC_GET_MULTICAST	0x80011000	/* multicast */
#define sceInetNDCC_GET_COLLISIONS	0x80011001	/* collisions */
#define sceInetNDCC_GET_RX_LENGTH_ER	0x80011002	/* rx length errors */
#define sceInetNDCC_GET_RX_OVER_ER	0x80011003	/* rx over errors */
#define sceInetNDCC_GET_RX_CRC_ER	0x80011004	/* rx crc errors */
#define sceInetNDCC_GET_RX_FRAME_ER	0x80011005	/* rx frame errors */
#define sceInetNDCC_GET_RX_FIFO_ER	0x80011006	/* rx fifo errors */
#define sceInetNDCC_GET_RX_MISSED_ER	0x80011007	/* rx missed errors */
#define sceInetNDCC_GET_TX_ABORTED_ER	0x80011008	/* tx aborted errors */
#define sceInetNDCC_GET_TX_CARRIER_ER	0x80011009	/* tx carrier errors */
#define sceInetNDCC_GET_TX_FIFO_ER	0x8001100a	/* tx fifo errors */
#define sceInetNDCC_GET_TX_HEARTBEAT_ER	0x8001100b	/* tx heartbeat errors */
#define sceInetNDCC_GET_TX_WINDOW_ER	0x8001100c	/* tx window errors */
#define sceInetNDCC_GET_NEGO_MODE	0x80020000	/* get nego mode */

/* Nego mode for Ethernet I/F */
#define sceInetNDNEGO_10	0x0001	/* 10BaseT Half-Duplex */
#define sceInetNDNEGO_10_FD	0x0002	/* 10BaseT Full-Duplex */
#define sceInetNDNEGO_TX	0x0004	/* 100BaseTX Half-Duplex */
#define sceInetNDNEGO_TX_FD	0x0008	/* 100BaseTX Full-Duplex */
#define sceInetNDNEGO_PAUSE	0x0040	/* Enable Flow Control */
#define sceInetNDNEGO_AUTO	0x0080	/* Enable Auto Nego */

/*
 * libnet original functions
 */
#if defined(__cplusplus)
extern "C" {
#endif

void libnet_init(sceSifMClientData *cd, unsigned int buffersize, unsigned int stacksize, int priority);
void libnet_term(sceSifMClientData *cd);
int reg_handler(sceSifMClientData *cd, u_int *buf);
int unreg_handler(sceSifMClientData *cd, u_int *buf);
int load_set_conf(sceSifMClientData *cd, u_int *buf, char *fname, char *usr_name);
int load_set_conf_only(sceSifMClientData *cd, u_int *buf, char *fname, char *usr_name);
int wait_get_addr(sceSifMClientData *cd, u_int *buf, int *if_id, struct sceInetAddress *addr);
int wait_get_addr_only(sceSifMClientData *cd, u_int *buf, int *if_id, struct sceInetAddress *addr);
int get_interface_id(sceSifMClientData *cd, u_int *buf, int *if_id);
int up_interface(sceSifMClientData *cd, u_int *buf, int id);
int down_interface(sceSifMClientData *cd, u_int *buf, int id);

#if defined(__cplusplus)
}
#endif

#endif	/* _LIBNET_H_DEFS */

/*** End of file ***/

