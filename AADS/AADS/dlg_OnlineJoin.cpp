//=========================================================================
//
//  dlg_online_join.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_Globals.hpp"
#include "serverman.hpp"
#include "gamemgr/gamemgr.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_dlg_vkeyboard.hpp"
#include "dlg_OnlineJoin.hpp"
#include "ui_joinlist.hpp"

#include "masterserver/masterserver.hpp"

#include "data\ui\ui_strings.h"
#include "StringMgr\StringMgr.hpp"
#include "Demo1/SpecialVersion.hpp"
#include "fe_colors.hpp"

#ifdef X_DEBUG
//#define SHOW_ADDRESS
#endif
//==============================================================================

extern server_manager* pSvrMgr;

extern xwchar       g_JoinPassword[FE_MAX_ADMIN_PASS];

xbool               s_JoinPasswordEntered;
xbool               s_JoinPasswordOk;
static f32			s_TimeSinceLastRefresh = 0.0f;

xcolor S_RED   ( 255,128, 96,224 );
xcolor S_YELLOW( 230,230,  0,230 );
xcolor S_GREEN ( 128,255, 96,224 );
xcolor S_GRAY  ( 170,170,170,224 );


enum oj_sort_key
{
    OJ_SORT_NONE = 0,
    OJ_SORT_SERVER_ASC,
    OJ_SORT_SERVER_DES,
    OJ_SORT_MAP_ASC,
    OJ_SORT_MAP_DES,
    OJ_SORT_TARGETLOCK_ASC,
    OJ_SORT_TARGETLOCK_DES,
    OJ_SORT_QUALITY_ASC,
    OJ_SORT_PLAYER_COUNT_ASC,
    OJ_SORT_PLAYER_COUNT_DES,
    OJ_SORT_COMPLETE_ASC,
    OJ_SORT_LAST,
};

//==============================================================================
// Online Dialog
//==============================================================================

enum controls
{
    IDC_SERVERLIST,
    IDC_ICON_TEXT1,       
    IDC_ICON_TEXT2,       
    IDC_ICON_TEXT3,       
    IDC_ICON_TEXT4,       
    IDC_SERVER_LIST_TEXT,
    IDC_PLAYER_LIST_TEXT,
    IDC_MAP_LIST_TEXT,   
    IDC_NET_ADDRESS,     
    IDC_REFRESH_LIST,    
    IDC_BACK,            
    IDC_JOIN,            
    IDC_SORT,
    IDC_INFO_FRAME,      
    IDC_PLAYERS_TEXT,    
    IDC_BOTS_TEXT,       
    IDC_COMPLETE_TEXT,   
    IDC_RESPONSE_TEXT,   
    IDC_PASSWORD_TEXT,   
	IDC_MODEM_TEXT,
    IDC_BUDDY_TEXT,      
	IDC_CLIENTS_TEXT,
	IDC_TARGETLOCK_TEXT,
    IDC_INFO_PLAYERS,   
    IDC_INFO_BOTS,      
    IDC_INFO_SERVERNAME,
    IDC_INFO_COMPLETE,  
    IDC_INFO_RESPONSE,  
    IDC_INFO_PASSWORD,  
    IDC_INFO_BUDDY,     
	IDC_INFO_TARGETLOCK,
	IDC_INFO_CLIENTS,
	IDC_INFO_MODEM,
	IDC_SERVERCOUNT_TEXT,
	IDC_SERVERCOUNT_BOX,
	IDC_SERVERCOUNT,
};

#define TOP_BOX_X       4
#define TOP_BOX_Y       0
#define TOP_BOX_W       456
#define TOP_BOX_H       168

#define BTM_BOX_X       4
#define BTM_BOX_Y       198
#define BTM_BOX_W       456
#define BTM_BOX_H       96

#define BTM_BTN_W		74

#define BUTTON_X        4
#define BUTTON_Y        281
#define BUTTON_W        140
#define BUTTON_H        24

// SPECIAL_VERSION

ui_manager::control_tem OnlineJoinControls[] =
{
    // Listbox components for the server list - these need to be first in the list because on a tabbed
    // dialog, the first entry within the destination dialog is what will be activated.
    { IDC_SERVERLIST,       IDS_NULL,           "joinlist",	TOP_BOX_X+0,				TOP_BOX_Y+24*1, TOP_BOX_W+0,    TOP_BOX_H,      0,  0,  2,  1, ui_win::WF_VISIBLE },
    // Lower region buttons
    { IDC_REFRESH_LIST,		IDS_OJ_REFRESH_LIST,  "button", BUTTON_X+0,					BUTTON_Y+0,     BUTTON_W+22,    BUTTON_H,       0,  1,  1,  1, ui_win::WF_VISIBLE },
    { IDC_SORT,             IDS_SF_SORTBY,      "combo",    BUTTON_X+294,				BUTTON_Y+ 2,    BUTTON_W+22,    BUTTON_H-4,     1,  1,  1,  1, ui_win::WF_VISIBLE },

    { IDC_BACK,             IDS_OJ_BACK,        "button",   BUTTON_X+0,					BUTTON_Y+28,    BUTTON_W+22,    BUTTON_H,       0,  2,  1,  1, ui_win::WF_VISIBLE },
    { IDC_JOIN,             IDS_OJ_JOIN,        "button",   BUTTON_X+294,				BUTTON_Y+28,    BUTTON_W+22,    BUTTON_H,       1,  2,  1,  1, ui_win::WF_VISIBLE },

    // Titles for listbox
    { IDC_ICON_TEXT1,       IDS_OJ_ICONS1,      "text",     TOP_BOX_X+8,				TOP_BOX_Y+0,    BUTTON_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_ICON_TEXT2,       IDS_OJ_ICONS2,      "text",     TOP_BOX_X+21,				TOP_BOX_Y+0,    BUTTON_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_ICON_TEXT3,       IDS_OJ_ICONS3,      "text",     TOP_BOX_X+34,				TOP_BOX_Y+0,    BUTTON_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_ICON_TEXT4,       IDS_OJ_ICONS4,      "text",     TOP_BOX_X+49,				TOP_BOX_Y+0,    BUTTON_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_SERVER_LIST_TEXT, IDS_OJ_SERVER_NAME, "text",     TOP_BOX_X+59+8,				TOP_BOX_Y+0,    BUTTON_W*2,     BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_PLAYER_LIST_TEXT, IDS_OJ_PLAYERS,     "text",     TOP_BOX_X+180+33,			TOP_BOX_Y+0,    BUTTON_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_MAP_LIST_TEXT,    IDS_OJ_MAP_NAME,    "text",     TOP_BOX_X+246+33,			TOP_BOX_Y+0,    BUTTON_W*3,     BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},

    // Frame layout for additional server information
    { IDC_INFO_FRAME,       IDS_OJ_INFO_TITLE,  "frame",    BTM_BOX_X,					BTM_BOX_Y+18*1, BTM_BOX_W,      BTM_BOX_H-18*2, 0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_INFO_SERVERNAME,  IDS_OJ_QUALITY,     "text",     BTM_BOX_X+16,				BTM_BOX_Y - 4,  BTM_BOX_W,      BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},

	//----- INFO BOX TITLE TEXT
    // Left part of server information box
    { IDC_BUDDY_TEXT,       IDS_OJ_BUDDY,       "text",     BTM_BOX_X+BTM_BTN_W*0+22,	BTM_BOX_Y+18*1, BTM_BTN_W,		BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_TARGETLOCK_TEXT,  IDS_OJ_TARGETLOCK,  "text",     BTM_BOX_X+BTM_BTN_W*0+22,	BTM_BOX_Y+18*2, BTM_BTN_W,      BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_BOTS_TEXT,        IDS_OJ_BOTS,        "text",     BTM_BOX_X+BTM_BTN_W*0+22,	BTM_BOX_Y+18*3, BTM_BTN_W,      BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},

	// Center part of server information box
    { IDC_PASSWORD_TEXT,    IDS_OJ_PASSWORD,    "text",     BTM_BOX_X+BTM_BTN_W*2+12,	BTM_BOX_Y+18*1, BTM_BTN_W,      BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
	{ IDC_MODEM_TEXT,		IDS_OJ_MODEM,		"text",     BTM_BOX_X+BTM_BTN_W*2+12,	BTM_BOX_Y+18*2, BTM_BTN_W,      BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_COMPLETE_TEXT,    IDS_OJ_COMPLETE,    "text",     BTM_BOX_X+BTM_BTN_W*2+12,	BTM_BOX_Y+18*3, BTM_BTN_W,      BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},

    // Right part of server information box
    { IDC_PLAYERS_TEXT,     IDS_OJ_PLAYERS,     "text",     BTM_BOX_X+BTM_BTN_W*4+12,    BTM_BOX_Y+18*1, BTM_BTN_W,     BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_CLIENTS_TEXT,		IDS_OJ_CLIENTS,		"text",     BTM_BOX_X+BTM_BTN_W*4+12,	BTM_BOX_Y+18*2, BTM_BTN_W,      BUTTON_H,        0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},
    { IDC_RESPONSE_TEXT,    IDS_OJ_QUALITY,     "text",     BTM_BOX_X+BTM_BTN_W*4+12,	BTM_BOX_Y+18*3, BTM_BTN_W,      BUTTON_H,        0,  0,  0,  0, ui_win::WF_VISIBLE | ui_win::WF_STATIC},

	{ IDC_SERVERCOUNT_BOX,	IDS_OJ_SERVERCOUNT,	"frame",	BUTTON_X+168,				BUTTON_Y+2,		118,		BUTTON_H*2+2,			0,  0,  0,  0, ui_win::WF_VISIBLE|ui_win::WF_STATIC},
	{ IDC_SERVERCOUNT_TEXT,	IDS_OJ_SERVERCOUNT,	"text",		BUTTON_X+170,				BUTTON_Y+6,		118,		BUTTON_H,			0,  0,  0,  0, ui_win::WF_VISIBLE|ui_win::WF_STATIC},
	{ IDC_SERVERCOUNT,		IDS_OJ_SERVERCOUNT, "text",		BUTTON_X+170,				BUTTON_Y+24,	118,		BUTTON_H,			0,  0,  0,  0, ui_win::WF_VISIBLE|ui_win::WF_STATIC},	
	//----- ACTUAL INFO BOXES
    // Left part of server information box
    { IDC_INFO_BUDDY,       IDS_OJ_BUDDY,       "text",     BTM_BOX_X+BTM_BTN_W*1+30,	BTM_BOX_Y+18*1, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
	{ IDC_INFO_TARGETLOCK,	IDS_OJ_TARGETLOCK,	"text",		BTM_BOX_X+BTM_BTN_W*1+30,	BTM_BOX_Y+18*2,	BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
    { IDC_INFO_BOTS,        IDS_OJ_BOTS,        "text",     BTM_BOX_X+BTM_BTN_W*1+30,	BTM_BOX_Y+18*3, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
    
    // Center part of server information box
    { IDC_INFO_PASSWORD,    IDS_OJ_PASSWORD,    "text",     BTM_BOX_X+BTM_BTN_W*3+18,	BTM_BOX_Y+18*1, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
    { IDC_INFO_MODEM,		IDS_OJ_MODEM,       "text",     BTM_BOX_X+BTM_BTN_W*3+18,	BTM_BOX_Y+18*2, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
    { IDC_INFO_COMPLETE,    IDS_OJ_COMPLETE,    "text",     BTM_BOX_X+BTM_BTN_W*3+18,	BTM_BOX_Y+18*3, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},

    // Center right part of server information box
	{ IDC_INFO_PLAYERS,     IDS_OJ_PLAYERS,     "text",     BTM_BOX_X+BTM_BTN_W*5+18,	BTM_BOX_Y+18*1, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
	{ IDC_INFO_CLIENTS,		IDS_OJ_CLIENTS,		"text",		BTM_BOX_X+BTM_BTN_W*5+18,	BTM_BOX_Y+18*2, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
    { IDC_INFO_RESPONSE,    IDS_OJ_QUALITY,     "text",     BTM_BOX_X+BTM_BTN_W*5+18,	BTM_BOX_Y+18*3, BTM_BTN_W,       BUTTON_H,       0,  0,  0,  0, ui_win::WF_VISIBLE},
	// Right part of server information box
};

ui_manager::dialog_tem OnlineJoinDialog =
{
    IDS_NULL,
    2, 3,
    sizeof(OnlineJoinControls)/sizeof(ui_manager::control_tem),
    &OnlineJoinControls[0],
    0
};

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================
s32 g_ForceServerListRefresh = 0;
xbool g_ServerListChanged = FALSE;
static s32	s_LastSortKey=-1;

//=========================================================================
//  Registration function
//=========================================================================

void dlg_online_join_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "onlinejoin", &OnlineJoinDialog, &dlg_online_join_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_online_join_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_online_join* pDialog = new dlg_online_join;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_online_join
//=========================================================================

dlg_online_join::dlg_online_join( void )
{
}

//=========================================================================

dlg_online_join::~dlg_online_join( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_online_join::Create( s32                        UserID,
                               ui_manager*                pManager,
                               ui_manager::dialog_tem*    pDialogTem,
                               const irect&               Position,
                               ui_win*                    pParent,
                               s32                        Flags,
                               void*                      pUserData )
{
    xbool   Success = FALSE;

    (void)pUserData;

    ASSERT( pManager );

    s_JoinPasswordEntered = FALSE;
    s_JoinPasswordOk      = FALSE;

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    // Fields for the server list listbox
    m_pServerList     = (ui_joinlist*)FindChildByID( IDC_SERVERLIST );

    m_pBack           = (ui_button*) FindChildByID( IDC_BACK );
    m_pJoin           = (ui_button*) FindChildByID( IDC_JOIN );
    m_pSort           = (ui_combo*)  FindChildByID( IDC_SORT );
    m_pRefresh        = (ui_button*) FindChildByID( IDC_REFRESH_LIST );
	m_pServerCount	  = (ui_text*)	 FindChildByID( IDC_SERVERCOUNT );
    
    ((ui_text*)FindChildByID(IDC_ICON_TEXT1         ))->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_ICON_TEXT2         ))->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_ICON_TEXT3         ))->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_ICON_TEXT4         ))->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_SERVER_LIST_TEXT   ))->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_PLAYER_LIST_TEXT   ))->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_MAP_LIST_TEXT      ))->SetLabelFlags(ui_font::h_left|ui_font::v_center);

    ((ui_text*)FindChildByID(IDC_ICON_TEXT1         ))->SetLabelColor(xcolor(0xc0,0xc0,0xc0));
    ((ui_text*)FindChildByID(IDC_ICON_TEXT2         ))->SetLabelColor(xcolor(0xc0,0xc0,0xc0));
    ((ui_text*)FindChildByID(IDC_ICON_TEXT3         ))->SetLabelColor(xcolor(0xc0,0xc0,0xc0));
    ((ui_text*)FindChildByID(IDC_ICON_TEXT4         ))->SetLabelColor(xcolor(0xc0,0xc0,0xc0));
    ((ui_text*)FindChildByID(IDC_SERVER_LIST_TEXT   ))->SetLabelColor(xcolor(0xc0,0xc0,0xc0));
    ((ui_text*)FindChildByID(IDC_PLAYER_LIST_TEXT   ))->SetLabelColor(xcolor(0xc0,0xc0,0xc0));
    ((ui_text*)FindChildByID(IDC_MAP_LIST_TEXT      ))->SetLabelColor(xcolor(0xc0,0xc0,0xc0));

    ((ui_text*)FindChildByID(IDC_PLAYERS_TEXT       ))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_BOTS_TEXT          ))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_COMPLETE_TEXT      ))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_RESPONSE_TEXT      ))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_PASSWORD_TEXT      ))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_MODEM_TEXT			))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_BUDDY_TEXT         ))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_CLIENTS_TEXT		))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    ((ui_text*)FindChildByID(IDC_TARGETLOCK_TEXT    ))->SetLabelFlags(ui_font::h_right|ui_font::v_center);
    
    m_pSort->AddItem  ( "NO SORTING",         OJ_SORT_NONE             );
    m_pSort->AddItem  ( "SERVER NAME \024",   OJ_SORT_SERVER_ASC       );
    m_pSort->AddItem  ( "SERVER NAME \023",   OJ_SORT_SERVER_DES       );
    m_pSort->AddItem  ( "MAP NAME \024",      OJ_SORT_MAP_ASC          );
    m_pSort->AddItem  ( "MAP NAME \023",      OJ_SORT_MAP_DES          );
    m_pSort->AddItem  ( "TARGET LOCK \024",   OJ_SORT_TARGETLOCK_ASC   );
    m_pSort->AddItem  ( "TARGET LOCK \023",   OJ_SORT_TARGETLOCK_DES   );
    m_pSort->AddItem  ( "QUALITY",            OJ_SORT_QUALITY_ASC      );
    m_pSort->AddItem  ( "PLAYER COUNT \024",  OJ_SORT_PLAYER_COUNT_ASC );
    m_pSort->AddItem  ( "PLAYER COUNT \023",  OJ_SORT_PLAYER_COUNT_DES );
    m_pSort->AddItem  ( "% COMPLETE",         OJ_SORT_COMPLETE_ASC     );
    m_pSort->SetSelection( 0 );
    if ( (fegl.SortKey >= 0) && (fegl.SortKey < m_pSort->GetItemCount()) )
    {
        m_pSort->SetSelection ( fegl.SortKey );
    }

    m_pInfoPlayers      =   (ui_text*)FindChildByID(IDC_INFO_PLAYERS       );
    m_pInfoBots         =   (ui_text*)FindChildByID(IDC_INFO_BOTS          );
    m_pInfoServerName   =   (ui_text*)FindChildByID(IDC_INFO_SERVERNAME    );
    m_pInfoHasBuddy     =   (ui_text*)FindChildByID(IDC_INFO_BUDDY         );
    m_pInfoHasPassword  =   (ui_text*)FindChildByID(IDC_INFO_PASSWORD      );
    m_pInfoResponse     =   (ui_text*)FindChildByID(IDC_INFO_RESPONSE      );
    m_pInfoComplete     =   (ui_text*)FindChildByID(IDC_INFO_COMPLETE      );
	m_pInfoTargetLock	=	(ui_text*)FindChildByID(IDC_INFO_TARGETLOCK    );
	m_pInfoClients		=	(ui_text*)FindChildByID(IDC_INFO_CLIENTS	   );
	m_pInfoModem		=	(ui_text*)FindChildByID(IDC_INFO_MODEM		   );

    m_pInfoPlayers      ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoBots         ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoServerName   ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoHasBuddy     ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoHasPassword  ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoResponse     ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoComplete     ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoTargetLock   ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    m_pInfoClients      ->SetLabelFlags(ui_font::h_left|ui_font::v_center);
	m_pInfoModem		->SetLabelFlags(ui_font::h_left|ui_font::v_center);
//	m_pServerCount		->SetLabelFlags(ui_font::h_left|ui_font::v_center);
    
    m_pServerCount->SetLabel( xwstring("0") );

    m_pInfoServerName   ->SetLabelColor(xcolor(0xc0,0xc0,0xc0));
    m_LocalServerCount = 0;
	PopulateServerInfo(NULL);

	s_LastSortKey = -1;
    UpdateControls();
    if (pSvrMgr)
    {
        pSvrMgr->SetAsServer(FALSE);
        pSvrMgr->Reset();
        pSvrMgr->SetIsInGame(FALSE);
    }
	g_ForceServerListRefresh=2;

    // Return success code
    return Success;
}

//=========================================================================
void dlg_online_join::Purge(void)
{
	tgl.pMasterServer->AcquireDirectory(FALSE);
	g_ForceServerListRefresh = 1;
	m_SortList.Clear();
	m_pServerList->DeleteAllItems();

}
//=========================================================================

void dlg_online_join::Destroy( void )
{
	Purge();
    ui_dialog::Destroy();
}

//=========================================================================

const char* GetGameTypeString( const server_info* pServer )
{
    const char* pString = "";

    switch(pServer->ServerInfo.GameType)
    {
    case GAME_NONE:
        pString = "";
        break;
    case GAME_CTF:
        pString = "CTF:";
        break;
    case GAME_CNH:
        pString = "CNH:";
        break;
    case GAME_DM:
        pString = "DM:";
        break;
    case GAME_TDM:
        pString = "TDM:";
        break;
    case GAME_HUNTER:
        pString = "Hunter:";
        break;
    case GAME_CAMPAIGN:
        pString = "Campaign:";
        break;
    }

    return pString;
}

s32 y_offset = 0;
//=========================================================================

void dlg_online_join::Render( s32 ox, s32 oy )
{
    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
		if (g_ForceServerListRefresh)
		{
			g_ForceServerListRefresh = 2;
		}
//        // Fade out background
//        const irect& ru = m_pManager->GetUserBounds( m_UserID );
//        m_pManager->RenderRect( ru, xcolor(0,0,0,64), FALSE );

		s32 w,h;
		eng_GetRes(w,h);
		if (h>448)
		{
			y_offset = 32;
		}
		else
		{
			y_offset = 0;
		}
        // Get window rectangle
        irect   r;
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        irect   rb = r;
        rb.Deflate( 1, 1 );
		rb.Translate(0,y_offset);

        // Render frame
        if( m_Flags & WF_BORDER )
        {
            // Render background color
            if( m_BackgroundColor.A > 0 )
            {
                m_pManager->RenderRect( rb, m_BackgroundColor, FALSE );
            }

            // Render Title Bar Gradient
            rb.SetHeight( 40 );
            xcolor c1 = FECOL_TITLE1;
            xcolor c2 = FECOL_TITLE2;
            m_pManager->RenderGouraudRect( rb, c1, c1, c2, c2, FALSE );

            // Render the Frame
            m_pManager->RenderElement( m_iElement, r, 0 );

            // Render Title
            rb.Deflate( 16, 0 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_WHITE, m_Label );
        }

        {
            irect rb=m_pInfoServerName->GetPosition();
            rb.SetHeight(18);
            rb.Translate(-16,3+y_offset);
            xcolor c1 = FECOL_TITLE1;
            xcolor c2 = FECOL_TITLE2;

            LocalToScreen(rb);
            
            irect rect = rb;

            m_pManager->RenderGouraudRect( rect, c1, c1, c2, c2, FALSE );

            rb= m_pInfoServerName->GetPosition();
            rb.SetHeight(18);
            rb.Translate(-16,3+y_offset);
            LocalToScreen(rb);

            rect = rb;
            
            m_pManager->RenderGouraudRect( rect, c1, c1, c2, c2, FALSE );
        }

        {
            irect rb=m_pServerList->GetPosition();
            rb.Translate(0,-20+y_offset);
            rb.SetHeight(18);
            xcolor c1 = FECOL_TITLE1;
            xcolor c2 = FECOL_TITLE2;

            LocalToScreen(rb);

            irect rect = rb;

            m_pManager->RenderGouraudRect( rect, c1, c1, c2, c2, FALSE );

            rb= m_pServerList->GetPosition();
            rb.Translate(0,-20+y_offset);
            rb.SetHeight(18);
            LocalToScreen(rb);

            rect = rb;
            
            m_pManager->RenderGouraudRect( rect, c1, c1, c2, c2, FALSE );
        }
        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
//    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_online_join::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // If at top of dialog then go back to parent tabbed dialog
    if( (Code == ui_manager::NAV_UP) && (m_NavY == 0) )
    {
        if( m_pParent )
        {
            ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        }
    }
    else
    {
        ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
    }
}

//=========================================================================

void dlg_online_join::OnPadShoulder( ui_win* pWin, s32 Direction )
{
    if( m_pServerList )
        m_pServerList->OnPadShoulder( pWin, Direction );
}

//=========================================================================

void dlg_online_join::OnPadShoulder2( ui_win* pWin, s32 Direction )
{
    if( m_pServerList )
        m_pServerList->OnPadShoulder2( pWin, Direction );
}

//=========================================================================

void dlg_online_join::OnPadSelect( ui_win* pWin )
{
    if( pWin == (ui_win*)m_pBack )
    {
        // Close dialog and step back
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
    else if( pWin == (ui_win*)m_pJoin )
    {
        fegl.GameHost = FALSE;

        s32         iSelection = m_pServerList->GetSelection();
        // Server Selection within Range?
        if( (iSelection >= 0) && (iSelection < m_pServerList->GetItemCount() ) )
        {
            const server_info* pCurrentServer = (server_info*)m_pServerList->GetSelectedItemData();

			if (pCurrentServer==NULL) return;

            tgl.UserInfo.MyName[0] = '\0';
            x_wstrcpy( tgl.UserInfo.Server, pCurrentServer->ServerInfo.Name );

            tgl.UserInfo.IsServer = FALSE;

            tgl.UserInfo.ServerAddr.IP = pCurrentServer->Info.IP;
            tgl.UserInfo.ServerAddr.Port = pCurrentServer->Info.Port;

            // Open Password dialog
            if( pCurrentServer->ServerInfo.Flags & SVR_FLAGS_HAS_PASSWORD )
            {
                audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );

                irect   r = m_pManager->GetUserBounds( m_UserID );
                ui_dlg_vkeyboard* pVKeyboard = (ui_dlg_vkeyboard*)m_pManager->OpenDialog( m_UserID, "ui_vkeyboard", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
                pVKeyboard->SetLabel( StringMgr( "ui", IDS_ENTER_PASSWORD ) );
                pVKeyboard->ConnectString( &m_JoinPassword, FE_MAX_ADMIN_PASS-1 );
                pVKeyboard->SetReturn( &s_JoinPasswordEntered, &s_JoinPasswordOk );

            }
            else
            {
                // Start joining
                tgl.UserInfo.UsingConfig = TRUE;

                // Setup Mission Loading Dialog
                audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
                ui_dialog* pDialog = tgl.pUIManager->OpenDialog( fegl.UIUserID, "mission load", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
				Purge();

                (void)pDialog;
                ASSERT( pDialog );
            }

        }
    }
    else if ( pWin == (ui_win*)m_pRefresh )
    {
		if (s_TimeSinceLastRefresh > 2.5f)
		{
			ForceRefresh();
			s_TimeSinceLastRefresh = 0.0f;
		}

		if (s_TimeSinceLastRefresh < 0.0f)
		{
			s_TimeSinceLastRefresh = 0.0f;
		}
    }

}

//=========================================================================
void dlg_online_join::ForceRefresh(void)
{
    tgl.pMasterServer->ClearFilters();
    
    // Build the buddy string
    char    BuddyString[16*10];
    s32     Index = 0;
    xstring Buddy;
    x_memset( BuddyString, 0, 16*10 );
    for( s32 i=0 ; i<10 ; i++ )
    {
        Buddy = fegl.WarriorSetups[0].Buddies[i];
        s32 Len = Buddy.GetLength();
        if( Len > 0 )
        {
            if( (Index + Len + 1) < 160 )
            {
                x_memcpy( &BuddyString[Index], (const char*)Buddy, Len );
                Index += Len;
                BuddyString[Index] = 1;
                Index++;
            }
        }
    }
    pSvrMgr->SetSearchString( BuddyString );

	m_SortList.Clear();
	m_pServerList->DeleteAllItems();
    tgl.pMasterServer->AcquireDirectory(TRUE);
}
//=========================================================================

void dlg_online_join::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Go back to Tabbed Dialog parent
    if( m_pParent )
    {
        ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
}

//=========================================================================

void dlg_online_join::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    UpdateControls();

    // If X pressed in Server List goto the host control
    if( (pSender == (ui_win*)m_pServerList) && (Command == WN_LIST_ACCEPTED) )
    {
        GotoControl( (ui_control*)m_pJoin );
    }
}

//=========================================================================

void dlg_online_join::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;

    if( (m_Flags & WF_VISIBLE)==0 )
		return;

	if (g_ForceServerListRefresh==2)
	{
		g_ForceServerListRefresh = 0;
		ForceRefresh();
	}

	s_TimeSinceLastRefresh += DeltaTime;

    FillServerList();
    fegl.SortKey = m_pSort->GetSelection();

    // Check for password dialog setting up a password to join
    if( s_JoinPasswordEntered )
    {
        s_JoinPasswordEntered = FALSE;
        if( s_JoinPasswordOk )
        {
            s_JoinPasswordOk = FALSE;

            x_wstrncpy( g_JoinPassword, (const xwchar*)m_JoinPassword, FE_MAX_ADMIN_PASS-1 );
            g_JoinPassword[FE_MAX_ADMIN_PASS-1] = 0;

            // Start joining
            tgl.UserInfo.UsingConfig = TRUE;

			Purge();
            // Setup Mission Loading Dialog
            audio_Play( SFX_FRONTEND_SELECT_01_OPEN, AUDFLAG_CHANNELSAVER );
            ui_dialog* pDialog = tgl.pUIManager->OpenDialog( fegl.UIUserID, "mission load", irect( 16, 16, 512-16, 448-16 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL|ui_win::WF_RENDERMODAL|ui_win::WF_BORDER );
            (void)pDialog;
            ASSERT( pDialog );
        }
    }
}

//=========================================================================

void dlg_online_join::UpdateControls( void )
{
}

//=========================================================================
// Returns 0 if item 1 == item 2
// Returns -1 if item 1 < item 2
// Returns +1 if item 1 > item 2
//
static s32 sort_compare(const void* item1, const void* item2)
{
    const server_info *pItem1;
    const server_info *pItem2;
    s32 result;

    result = 0;

    pItem1 = *(server_info**)item1;
    pItem2 = *(server_info**)item2;


    switch (fegl.SortKey)
    {
    //---------------------------------------------------------------------
    case OJ_SORT_NONE:
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_MAP_ASC:
        {
            xstring MapName1 = GetGameTypeString( pItem1 );
            xstring MapName2 = GetGameTypeString( pItem2 );
            MapName1 += pItem1->ServerInfo.LevelName;
            MapName2 += pItem2->ServerInfo.LevelName;
            result = x_strcmp( MapName1, MapName2 );
        }
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_MAP_DES:
        {
            xstring MapName1 = GetGameTypeString( pItem1 );
            xstring MapName2 = GetGameTypeString( pItem2 );
            MapName1 += pItem1->ServerInfo.LevelName;
            MapName2 += pItem2->ServerInfo.LevelName;
            result = x_strcmp( MapName2, MapName1 );
        }
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_TARGETLOCK_ASC:
		if (pItem1->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK)
		{
			if (pItem2->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK)
				result = 0;
			else
				result = 1;
		}
		else
		if (pItem2->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK)
		{
			result = -1;
		}
		else
		{
			result = 0;
		}
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_TARGETLOCK_DES:
		if (pItem1->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK)
		{
			if (pItem2->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK)
				result = 0;
			else
				result = -1;
		}
		else
		if (pItem2->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK)
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_SERVER_ASC:
        result = x_wstrcmp(pItem1->ServerInfo.Name,pItem2->ServerInfo.Name);
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_SERVER_DES:
        result = x_wstrcmp(pItem2->ServerInfo.Name,pItem1->ServerInfo.Name);
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_QUALITY_ASC:
        if (pItem1->PingTime < pItem2->PingTime)    result = -1;
        else if (pItem1->PingTime > pItem2->PingTime)    result = 1;
        else result = 0;
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_PLAYER_COUNT_ASC:
        result = pItem1->ServerInfo.CurrentPlayers - pItem2->ServerInfo.CurrentPlayers;
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_PLAYER_COUNT_DES:
        result =pItem2->ServerInfo.CurrentPlayers - pItem1->ServerInfo.CurrentPlayers;
        break;
    //---------------------------------------------------------------------
    case OJ_SORT_COMPLETE_ASC:
        if( pItem1->ServerInfo.AmountComplete < pItem2->ServerInfo.AmountComplete )
            result =  -1;
        else if( pItem1->ServerInfo.AmountComplete > pItem2->ServerInfo.AmountComplete )
            result = 1;
        else
            result = 0;
        break;
    default:
        break;
    }

	if ( (result==0) || (fegl.SortKey == OJ_SORT_NONE) )
	{
		if (pItem1->ServerInfo.Flags & SVR_FLAGS_HAS_BUDDY)
		{
			if (pItem2->ServerInfo.Flags & SVR_FLAGS_HAS_BUDDY)
			{
				result = 0;
			}
			else
			{
				result = -1;
			}
		}
		else
		{
			if (pItem2->ServerInfo.Flags & SVR_FLAGS_HAS_BUDDY)
			{
				result = 1;
			}
			else
			{
				result = 0;
			}
		}
	}
    return result;
}

//=========================================================================

xbool dlg_online_join::ApplyFilterRules    ( const server_info *pServerInfo )
{
    xbool Matched;

    (void)pServerInfo;

    Matched = TRUE;

    return Matched;
}

//=========================================================================
static s32 s_EntityCount;
s32 insort;
s32 count;
s32 isel;
server_info* si_entry;

void dlg_online_join::FillServerList( void )
{
    s32         i;
    s32         iSelection = m_pServerList->GetSelection();
    update_state    State;

    State = tgl.pMasterServer->GetCurrentState();

    if ( (State == US_WAIT_DIRECTORY) || (s_TimeSinceLastRefresh < 2.5f) )
        m_pRefresh->SetLabel( "WORKING..." );
    else
        m_pRefresh->SetLabel( "REFRESH LIST" );
    
	if ( (g_ServerListChanged==FALSE) && (fegl.SortKey == s_LastSortKey) )
	{
		m_pServerList->     SetSelection( iSelection );
		if (iSelection >= 0)
		{
			count = m_pServerList->GetItemCount();
			PopulateServerInfo(	(server_info*)m_pServerList->GetSelectedItemData());
		}
		else
		{
			PopulateServerInfo(NULL);
		}
		return;
	}

    // x_DebugMsg("dlg_online_join::FillServerList - Refresh requested\n");
	g_ServerListChanged = FALSE;
	s_LastSortKey = fegl.SortKey;

    // Get List cursor offset
    s32 ListCursorOffset = m_pServerList->GetCursorOffset();

    // Clear the list
    m_pServerList->     DeleteAllItems();

    // Add all servers into list
    server_info *pCurrentInfo;

    m_LocalServerCount = pSvrMgr->GetCount();
    if (iSelection>=0)
    {
        pCurrentInfo = m_SortList[iSelection];
    }
    else
    {
        pCurrentInfo = NULL;
    }

	m_SortList.Clear();

	for( i=0; i<m_LocalServerCount; i++ )
	{
        if (ApplyFilterRules(pSvrMgr->GetServer(i)))
        {
            m_SortList.Append(pSvrMgr->GetServer(i));
        }
	}

    for (i=0;i<tgl.pMasterServer->GetEntityCount();i++)
    {
       if (ApplyFilterRules(tgl.pMasterServer->GetEntityInfo(i)))
       {
            m_SortList.Append(tgl.pMasterServer->GetEntityInfo(i));
        }
    }

    // This is where we sort the actual server list

    if ( m_SortList.GetCount() )
        x_qsort(m_SortList.GetData(),m_SortList.GetCount(),sizeof(server_info*),sort_compare);

	s32 PlayerCount=0;

    for (i=0;i<m_SortList.GetCount();i++)
    {
        if (pCurrentInfo == m_SortList[i])
        {
            iSelection = i;
        }
        AddServerToList(m_SortList[i],XCOLOR_WHITE);
		PlayerCount+= m_SortList[i]->ServerInfo.CurrentPlayers;
    }

    // Validate selection
    if( iSelection <  0 ) iSelection = 0;
    if( iSelection >= m_pServerList->GetItemCount() ) iSelection = m_pServerList->GetItemCount()-1;
    if (m_pServerList->GetItemCount() == 0)
        iSelection = -1;

    if (iSelection >=0)
    {
        PopulateServerInfo(m_SortList[iSelection]);
    }
    else
    {
        PopulateServerInfo(NULL);
    }
    m_pServerList->     SetSelectionWithOffset( iSelection, ListCursorOffset );
#ifdef DEDICATED_SERVER
	xwstring count(xfs("%d/%d(%d)",m_pServerList->GetItemCount(),tgl.pMasterServer->GetServerCount(),PlayerCount));
#else
	xwstring count(xfs("%d",m_pServerList->GetItemCount()));
#endif

	m_pServerCount->	SetLabel((const xwchar*)count);

#ifdef TARGET_PS2
	extern void ps2_GetFreeMem(s32*,s32*,s32*);
	extern s32 s_MemFree;
	extern s32 s_LargestFree;
	extern s32 s_Fragments;
	ps2_GetFreeMem(&s_MemFree,&s_LargestFree,&s_Fragments);
	s_EntityCount = m_SortList.GetCount();
#endif
}

//=========================================================================
s32 PingToColor(f32 ping, xcolor& responsecolor)
{
    if      (ping > 500.0f)  { responsecolor = S_RED;   return 8;   }
    else if (ping > 400.0f)  { responsecolor = S_RED;   return 7;   }
    else if (ping > 300.0f)  { responsecolor = S_RED;   return 6;   }
    else if (ping > 250.0f)  { responsecolor = S_YELLOW;return 5;   }
    else if (ping > 200.0f)  { responsecolor = S_YELLOW;return 4;   }
    else if (ping > 150.0f)  { responsecolor = S_YELLOW;return 3;   }
    else if (ping > 125.0f)  { responsecolor = S_GREEN; return 2;   }
    else if (ping > 100.0f)  { responsecolor = S_GREEN; return 2;   }
    else if (ping > 75.0f)   { responsecolor = S_GREEN; return 1;   }
    else                     { responsecolor = S_GREEN; return 0;   }
}

s32 NoResponseCount;
void dlg_online_join::AddServerToList( const server_info *pServer, const xcolor &color )
{
    (void)color;

    xcolor      responsecolor = xcolor(0xc0,0xc0,0xc0);

    if( pServer->ServerInfo.GameType >= 0 )
    {
        PingToColor( pServer->PingTime,responsecolor );

        if( (pServer->ServerInfo.MaxClients == pServer->ServerInfo.ClientCount) ||
            (pServer->ServerInfo.CurrentPlayers == pServer->ServerInfo.MaxPlayers) )
            responsecolor = S_GRAY;

        s32 iItem = m_pServerList->AddItem( xwstring(pServer->ServerInfo.LevelName), (s32)pServer );
        m_pServerList->EnableItem  ( iItem, TRUE );
        m_pServerList->SetItemColor( iItem, responsecolor );
    }
	else
	{
		NoResponseCount++;
	}
}

//=========================================================================
void dlg_online_join::PopulateServerInfo(const server_info *pServerInfo)
{

    xcolor      responsecolor;

    xwchar buff[128];

    x_wstrcpy(buff, StringMgr("ui",IDS_OJ_INFO_TITLE));

    if (pServerInfo)
    {

        x_wstrcat(buff,xwstring(" for "));
        x_wstrcat(buff,pServerInfo->ServerInfo.Name);

        m_pInfoPlayers      ->SetLabel((xwstring)xfs("%d/%d",pServerInfo->ServerInfo.CurrentPlayers,pServerInfo->ServerInfo.MaxPlayers));
        m_pInfoBots         ->SetLabel((xwstring)xfs("%d",pServerInfo->ServerInfo.BotCount));
        m_pInfoComplete     ->SetLabel((xwstring)xfs("%d%%",pServerInfo->ServerInfo.AmountComplete));
        m_pInfoClients      ->SetLabel((xwstring)xfs("%d/%d",pServerInfo->ServerInfo.ClientCount,pServerInfo->ServerInfo.MaxClients));

		if (pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_TARGETLOCK)
		{
			m_pInfoTargetLock	->SetLabel(StringMgr("ui",IDS_OJ_YES));
		}
		else
		{
			m_pInfoTargetLock	->SetLabel(StringMgr("ui",IDS_OJ_NO));
		}

        if (pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_BUDDY)
        {
            m_pInfoHasBuddy     ->SetLabel(StringMgr("ui",IDS_OJ_YES));
        }
        else
        {
            m_pInfoHasBuddy     ->SetLabel(StringMgr("ui",IDS_OJ_NO));
        }

        if (pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_PASSWORD)
        {
            m_pInfoHasPassword  ->SetLabel(StringMgr("ui",IDS_OJ_YES));
        }
        else
        {
            m_pInfoHasPassword ->SetLabel(StringMgr("ui",IDS_OJ_NO));
        }

        if (pServerInfo->ServerInfo.Flags & SVR_FLAGS_HAS_MODEM)
        {
            m_pInfoModem		->SetLabel(StringMgr("ui",IDS_OJ_YES));
        }
        else
        {
            m_pInfoModem		->SetLabel(StringMgr("ui",IDS_OJ_NO));
        }

        s32 response;

        response = PingToColor(pServerInfo->PingTime,responsecolor);
        m_pInfoResponse     ->SetLabel( xwstring("llllllllll"+response) );
        m_pInfoResponse     ->SetLabelColor( responsecolor );

		if (pServerInfo->ServerInfo.CurrentPlayers == pServerInfo->ServerInfo.MaxPlayers)
		{
			m_pInfoPlayers	->SetLabelColor( S_RED );
		}
		else
		{
			m_pInfoPlayers	->SetLabelColor( XCOLOR_WHITE );
		}

		if (pServerInfo->ServerInfo.ClientCount == pServerInfo->ServerInfo.MaxClients)
		{
			m_pInfoClients	->SetLabelColor( S_RED );
		}
		else
		{
			m_pInfoClients	->SetLabelColor( XCOLOR_WHITE );
		}
    }
    else
    {
        m_pInfoPlayers      ->SetLabel(xwstring("---"));
        m_pInfoBots         ->SetLabel(xwstring("---"));
        m_pInfoHasBuddy     ->SetLabel(xwstring("---"));
        m_pInfoHasPassword  ->SetLabel(xwstring("---"));
        m_pInfoResponse     ->SetLabel(xwstring("---"));
        m_pInfoComplete     ->SetLabel(xwstring("---"));
        m_pInfoClients		->SetLabel(xwstring("---"));
        m_pInfoTargetLock   ->SetLabel(xwstring("---"));
        m_pInfoModem	    ->SetLabel(xwstring("---"));
		m_pInfoClients		->SetLabelColor( XCOLOR_WHITE );
		m_pInfoPlayers		->SetLabelColor( XCOLOR_WHITE );
		m_pInfoResponse		->SetLabelColor( XCOLOR_WHITE );
    }
    m_pInfoServerName   ->SetLabel(buff);
}





