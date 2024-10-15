#include "WONAPI.h"
#include "WONStatus.h"
#include "WONCommon/StringUtil.h"
#include "WONCommon/Completion.h"
#include "WONCommon/WONConsole.h"
#include "WONRouting/SmartRoutingConnection.h"
#include "WONRouting/AllRoutingOps.h"
#include "WONDir/GetDirOp.h"
#include "WONMisc/GetMOTDOp.h"
#include <stdarg.h>

using namespace std;
using namespace WONAPI;

Console gConsole;

WONAPICoreEx gAPI;
AuthContextPtr gAuthContext;
ServerContextPtr gDirServers;
ServerContextPtr gAuthServers;
SmartRoutingConnectionPtr gConnection;
string gServerName;
bool gRegistered = false;
int gGroupId = -1;
string gGroupName;
bool gDone = false;

std::string gDirServerName;
std::wstring gDirServerPath = L"/API2Sample";

static DWORD COLOR_USER			= 0;
static DWORD COLOR_BROADCAST	= 0;
static DWORD COLOR_WHISPER		= 0;
static DWORD COLOR_EMOTE		= 0;
static DWORD COLOR_MESSAGE		= 0;
static DWORD COLOR_ERROR		= 0;
static DWORD COLOR_SYSTEM		= 0;
static DWORD COLOR_MOTD			= 0;
static DWORD COLOR_INVITE		= 0;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void printf_flush(const char *format,...)
{
	char buf[1024];
	va_list anArgPtr;
	va_start(anArgPtr, format);
	if(format)
		vsprintf(buf, format, anArgPtr);

	printf("%s",buf);
	fflush(stdout);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int read_int()
{
	string aStr = gConsole.ReadLine();
	int aNum = 0;
	sscanf(aStr.c_str(),"%d",&aNum);
	return aNum;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinCompletion(AsyncOpPtr theOp)
{
	RoutingClientJoinedGroupOp *anOp = (RoutingClientJoinedGroupOp*)theOp.get();
	if(anOp->GetConnection()!=gConnection.get())
		return;

	if(anOp->GetGroupId()!=gGroupId || anOp->GetClientId()==gConnection->GetMyClientId())
		return;

	RoutingClientInfoPtr aClient = gConnection->GetClientRef(anOp->GetClientId());
	printf("%s has entered.\n",WStringToString(aClient->mName).c_str());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LeaveCompletion(AsyncOpPtr theOp)
{
	RoutingClientLeftGroupOp *anOp = (RoutingClientLeftGroupOp*)theOp.get();
	if(anOp->GetConnection()!=gConnection.get())
		return;

	if(anOp->GetGroupId()!=gGroupId || anOp->GetClientId()==gConnection->GetMyClientId())
		return;

	RoutingClientInfoPtr aClient = gConnection->GetClientRef(anOp->GetClientId());
	printf("%s has left.\n",WStringToString(aClient->mName).c_str());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AppendChat(const std::string &theChat, DWORD theColor = 0)
{
	printf("%s",theChat.c_str());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatCompletion(AsyncOpPtr theOp)
{
	RoutingPeerChatOp *anOp = (RoutingPeerChatOp*)theOp.get();
	if(anOp->GetConnection()!=gConnection.get())
		return;

	RoutingClientInfoPtr aSendClient = gConnection->GetClientRef(anOp->GetSenderId());
	if(aSendClient.get()==NULL)
		return;

	const RoutingRecipientList &aList = anOp->GetRecipients();
	if(aList.empty())
		return;

	int aDestId = aList.front();

	if(anOp->IsEmote())
	{
		AppendChat(WStringToString(aSendClient->mName),COLOR_USER);
		AppendChat(WStringToString(anOp->GetText()),COLOR_EMOTE);
	}		
	else if(anOp->IsWhisper())
	{
		if(aSendClient->mId==gConnection->GetMyClientId()) // Whisper from me
		{
			RoutingClientInfoPtr aDestClient = gConnection->GetClientRef(aDestId);
			if(aDestClient.get()==NULL)
				return;

			AppendChat("You whisper to ",COLOR_USER);
			AppendChat(WStringToString(aDestClient->mName),COLOR_USER);
			AppendChat(": ");
			AppendChat(WStringToString(anOp->GetText()),COLOR_WHISPER);
		}
		else
		{
			AppendChat(WStringToString(aSendClient->mName),COLOR_USER);
			AppendChat(" whispers to you: ",COLOR_USER);
			AppendChat(WStringToString(anOp->GetText()),COLOR_WHISPER);
		}
	}
	else
	{
		AppendChat(WStringToString(aSendClient->mName),COLOR_USER);
		AppendChat(": ");
		AppendChat(WStringToString(anOp->GetText()),COLOR_BROADCAST);
	}

	AppendChat("\n");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Init()
{
	gAuthServers = new ServerContext;

	gDirServers = new ServerContext;
	if(gDirServerName.empty())
	{
		gDirServers->AddAddress("wontest.west.won.net:15101");
		gDirServers->AddAddress("wontest.central.won.net:15101");
		gDirServers->AddAddress("wontest.east.won.net:15101");
	}
	else
		gDirServers->AddAddress(gDirServerName);

	GetMOTDOpPtr aGetMOTD = new GetMOTDOp("API2Sample");
	aGetMOTD->RunAsync(30000);

	GetDirOpPtr aGetDir = new GetDirOp(gDirServers);
	aGetDir->SetPath(L"/TitanServers/Auth");
	aGetDir->SetFlags(DIR_GF_DECOMPSERVICES | DIR_GF_SERVADDNETADDR);
	aGetDir->RunAsync(30000);

	printf_flush("Getting MOTD (hit 'c' to cancel)... ");
	while(aGetMOTD->Pending())
	{
		if(gConsole.kbhit() && gConsole.getch()=='c')
			break;

		gAPI.Pump(20);
	}

	printf("%s\n",WONStatusToString(aGetMOTD->GetStatus()));
	if(aGetMOTD->Succeeded())
	{
		printf("System Message: %s\n",aGetMOTD->GetSysMOTD()->data());
		printf("Game Message: %s\n\n",aGetMOTD->GetGameMOTD()->data());
	}

	printf_flush("Getting AuthServer addressess... ");
	while(aGetDir->Pending())
		gAPI.Pump(20);

	printf("%s\n\n",WONStatusToString(aGetDir->GetStatus()));

	if(aGetDir->Succeeded())
		gAuthServers->AddAddressesFromDir(aGetDir->GetDirEntityList());
	else
	{
		gAuthServers->AddAddress("wontest.west.won.net:15200");
		gAuthServers->AddAddress("wontest.central.won.net:15200");
		gAuthServers->AddAddress("wontest.east.won.net:15200");
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Login()
{
	while(true)
	{
		printf_flush("login: ");
		string aName = gConsole.ReadLine();

		printf_flush("password: ");
		string aPassword = gConsole.ReadPassword();

		gAuthContext = new AuthContext(StringToWString(aName), L"WON", StringToWString(aPassword));
		gAuthContext->SetServerContext(gAuthServers);
		printf_flush("Logging in... ");
		
		WONStatus aStatus = gAuthContext->RefreshBlock();
		printf("%s\n\n",WONStatusToString(aStatus));

		if(aStatus==WS_Success)
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ShowMembers()
{
	if(gGroupId==-1)
	{
		printf("You're not in a room.\n");
		return;
	}

	RoutingGroupInfoPtr aGroup = gConnection->GetGroupRef(gGroupId);

	printf("%s members:\n",gGroupName.c_str());
	RoutingMemberMap::iterator anItr = aGroup->mMemberMap.begin();
	while(anItr!=aGroup->mMemberMap.end())
	{
		SmartRoutingClientInfo *aClient = (SmartRoutingClientInfo*)anItr->second->mClientInfo.get();
		printf("%s ",WStringToString(aClient->mName).c_str());
		if(aClient->mIgnored)
			printf("(ignored)");

		printf("\n");

		++anItr;
	}
	printf("\n");

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void KillConnection()
{
	if(gConnection.get()!=NULL)
	{
		printf("Disconnecting from %s\n",gServerName.c_str());
		gConnection->Kill();
		gConnection = NULL;
		gRegistered = false;
		gServerName = "";
		gGroupId = -1;
		gGroupName = "";
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CloseCompletion(AsyncOpPtr theOp)
{
	SocketOp *anOp = (SocketOp*)theOp.get();
	if(anOp->GetSocket()!=gConnection.get())
		return;

	KillConnection();		
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LeaveGroup()
{
	if(gGroupId!=-1)
	{
		printf("Leaving %s.\n",gGroupName.c_str());
		gConnection->LeaveGroup(gGroupId);
		gGroupId = -1;
		gGroupName = "";
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroup()
{
	if(gConnection.get()==NULL)
	{
		printf("You're not on a server.\n");
		return;
	}

	///////////////////////////////
	// Display group list
	const RoutingGroupMap &aMap = gConnection->GetGroupMapRef();
	RoutingGroupMap::const_iterator anItr = aMap.begin();
	int aCount = 0;
	printf("%2d: Cancel\n",0);
	while(anItr!=aMap.end())
	{
		RoutingGroupInfo *aGroup = anItr->second;
		if(aGroup->mFlags&RoutingGroupFlag_IsChatRoom)
		{
			++aCount;
			printf("%2d: %s ",aCount,WStringToString(aGroup->mName).c_str());
			if(aGroup->mFlags & RoutingGroupFlag_PasswordProtected)
				printf("(password)");
			if(aGroup->mFlags & RoutingGroupFlag_InviteOnly)
				printf("(invite)");
			
			int aNumMembers = aGroup->mMemberMap.size();
			printf(" - %d member",aGroup->mMemberMap.size());
			if(aNumMembers!=1)
				printf("s");
			printf(".\n");
		}

		++anItr;
	}

	///////////////////////////////
	// Choose group
	int aNum;
	do
	{
		printf_flush("\nSelect room: ");	
		aNum = read_int();
	} while(aNum<0 || aNum>aCount);

	if(aNum==0)
	{
		printf("Canceled.\n");
		return;
	}

	aCount = 0;
	anItr = aMap.begin();
	while(aCount < aNum)
	{
		if(anItr->second->mFlags&RoutingGroupFlag_IsChatRoom)
		{
			++aCount;
			if(aCount==aNum)
				break;
		}

		++anItr;
	}

	RoutingGroupInfo* aGroup = anItr->second;
	if(aGroup->mId==gGroupId)
	{
		printf("Already in %s.\n",gGroupName.c_str());
		return;
	}

	LeaveGroup();
	string aPassword;
	if(aGroup->mFlags&RoutingGroupFlag_PasswordProtected)
	{
		printf_flush("password: ");
		aPassword = gConsole.ReadPassword();
	}

	///////////////////////////////
	// Join new group
	string aGroupName = WStringToString(aGroup->mName);
	printf_flush("Joining %s (press 'c' to cancel)... ",aGroupName.c_str());

	RoutingJoinGroupOpPtr aJoinOp = new RoutingJoinGroupOp(gConnection);
	aJoinOp->SetGroupId(aGroup->mId);
	aJoinOp->SetGroupPassword(StringToWString(aPassword));
	aJoinOp->SetJoinFlags(RoutingJoinGroupFlag_GetMembersOfGroup);
	aJoinOp->RunAsync(OP_TIMEOUT_INFINITE);

	while(aJoinOp->Pending())
	{
		gAPI.Pump(20);
		if(gConsole.kbhit())
		{
			if(gConsole.getch()=='c')
				break;
		}
	}

	if(aJoinOp->Pending()) // canceling join
	{
		printf_flush("Canceling join.\n ");
		gConnection->CancelJoinGroup(aGroup->mId,true);
		gConnection->LeaveGroup(aGroup->mId);
		return;
	}


	printf("%s\n\n",WONStatusToString(aJoinOp->GetStatus()));
	if(!aJoinOp->Succeeded())
		return;

	gGroupId = aGroup->mId;
	gGroupName = aGroupName;
	ShowMembers();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinServer()
{
	///////////////////////////////
	// Get server list
	GetDirOpPtr anOp = new GetDirOp(gDirServers);
	anOp->SetPath(gDirServerPath);
	anOp->SetFlags(DIR_GF_DECOMPSERVICES | DIR_GF_ADDDISPLAYNAME | DIR_GF_SERVADDNETADDR);

	printf_flush("Getting chat servers... ");
	anOp->RunBlock(60000);

	printf("%s\n\n",WONStatusToString(anOp->GetStatus()));
	if(!anOp->Succeeded())
		return;

	///////////////////////////////
	// Choose server entry
	const DirEntityList &aList = anOp->GetDirEntityList();
	DirEntityList::const_iterator anItr = aList.begin();
	int aCount = 1;
	printf("%2d: Cancel\n",0);
	while(anItr!=aList.end())
	{
		printf("%2d: %s\n",aCount,WStringToString((*anItr)->mDisplayName).c_str());
		++aCount;
		++anItr;
	}

	int aNum;
	do
	{
		printf_flush("\nSelect server: ");	
		aNum = read_int();
	} while(aNum<0 || aNum>aList.size());

	if(aNum==0)
	{
		printf("Canceled.\n");
		return;
	}

	aCount = 1;
	anItr = aList.begin();
	while(aCount!=aNum)
	{
		++aCount;
		++anItr;
	}

	const DirEntity* anEntity = *anItr;
	IPAddr anAddr = anEntity->GetNetAddrAsIP();
	std::string aServerName = WStringToString(anEntity->mDisplayName);


	///////////////////////////////
	// Connect to new server
	KillConnection();
	printf_flush("Connecting to %s... ",aServerName.c_str());
	gConnection = new SmartRoutingConnection;
	gConnection->SetAuth(gAuthContext);
	gConnection->SetCloseCompletion(new OpCompletion(CloseCompletion));
	gConnection->SetAllowPartialNameMatch(false);
	gConnection->SetServerWideCommands(true);
	WONStatus aStatus = gConnection->ConnectBlock(anAddr);

	printf("%s\n\n",WONStatusToString(aStatus));
	if(aStatus!=WS_Success)
		return;

	///////////////////////////////
	// Register with server
	printf_flush("Registering with server... ");

	RoutingRegisterClientOpPtr aRegisterOp = new RoutingRegisterClientOp(gConnection, L"",L"",0,0,
		RoutingRegisterClientFlag_LoginTypeCertificate | 
		RoutingRegisterClientFlag_GetClientList,
		0);

	RoutingGetGroupListOpPtr aGetGroupListOp = new RoutingGetGroupListOp(gConnection,RoutingGetGroupList_IncludeAll);

	aRegisterOp->RunAsync(OP_TIMEOUT_INFINITE);
	aGetGroupListOp->RunAsync(OP_TIMEOUT_INFINITE);

	while(aRegisterOp->Pending())
		gAPI.Pump(20);

	printf("%s\n\n",WONStatusToString(aRegisterOp->GetStatus()));
	if(!aRegisterOp->Succeeded())
		return;

	while(aGetGroupListOp->Pending()) // get group list
		gAPI.Pump(20);

	gRegistered = true;
	gServerName = aServerName;
	gConnection->SetRoutingCompletion(RoutingOp_PeerChat, new OpCompletion(ChatCompletion));
	gConnection->SetRoutingCompletion(RoutingOp_ClientJoinedGroup, new OpCompletion(JoinCompletion));
	gConnection->SetRoutingCompletion(RoutingOp_ClientLeftGroup, new OpCompletion(LeaveCompletion));

	JoinGroup();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CreateGroup()
{
	if(gConnection.get()==NULL)
	{
		printf("You're not on a server.\n");
		return;
	}

	printf_flush("Enter room name: ");
	string aName = gConsole.ReadLine();

	string aPassword;

	while(true)
	{
		printf_flush("Enter password (blank = no password): ");
		aPassword = gConsole.ReadPassword();
		if(aPassword.length()!=0)
		{
			printf_flush("Confirm password: ");
			string aConfirm = gConsole.ReadPassword();
			if(aConfirm!=aPassword)
			{
				printf("Confirmation didn't match.\n");
				continue;
			}
		}
		break;
	}

	LeaveGroup();
	
	printf_flush("Creating room %s... ",aName.c_str());
	RoutingCreateGroupOpPtr aCreate = new RoutingCreateGroupOp(gConnection);
	aCreate->SetGroupId(RoutingId_Invalid); // create
	aCreate->SetGroupName(StringToWString(aName));
	aCreate->SetGroupPassword(StringToWString(aPassword));
	aCreate->SetGroupFlags(RoutingGroupFlag_IsChatRoom);
	aCreate->SetJoinFlags(RoutingCreateGroupJoinFlag_JoinAsPlayer);
	aCreate->RunAsync(OP_TIMEOUT_INFINITE);
	while(aCreate->Pending())
		gAPI.Pump(20);

	WONStatus aStatus = aCreate->GetStatus();
	printf("%s\n\n",WONStatusToString(aStatus));
	if(aStatus!=WS_Success)
		return;

	gGroupName = aName;
	gGroupId = aCreate->GetGroupId();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void HandleInput(char* theStr, int theLength)
{
	if(gGroupId!=-1)
	{
		bool handled = gConnection->HandleInput(StringToWString(theStr));
		if(handled)
			return;

		if(gConnection->GetLastInputError()!=SmartRoutingConnection::InputError_InvalidCommand)
			return;
	}

	if(theStr[0]!='/')
	{
		printf("You're not in a chat room.\n");
		return;
	}

	string aStr = StringToUpperCase(theStr+1);
	if(aStr=="LOGIN") Login();
	else if(aStr=="SERVERS") JoinServer();
	else if(aStr=="ROOMS") JoinGroup();
	else if(aStr=="MEMBERS") ShowMembers();
	else if(aStr=="CREATE") CreateGroup();
	else if(aStr=="LOCATION")
	{
		printf("Server: %s\n",gServerName.c_str());
		printf("Room: %s\n",gGroupName.c_str());
	}
	else if(aStr=="QUIT") 
	{
		KillConnection();
		gDone = true;
	}
	else if(aStr=="?")
	{
		printf("Commands:\n");
		printf("\t/Login - Login to AuthServer.\n");
		printf("\t/Servers - Join RoutingServer.\n");
		printf("\t/Rooms - Join group on RoutingServer.\n");
		printf("\t/Members - Display members of current group.\n");
		printf("\t/Create - Create new group.\n");
		printf("\t/Location - Display current server and room name.\n");
		printf("\t/Quit - Quit program.\n");
	}
	else 
		printf("Invalid command.\n");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	// Don't do threads so this sample will work on the Mac.  Haven't gotten to implementing
	// threads on the Mac yet.
	gAPI.SetDoSocketAndTimerThreads(false); 
	gAPI.Startup();

	if(argc==3)
	{
		gDirServerName = argv[1];
		gDirServerPath = StringToWString(argv[2]);
	}

	Init();
	Login();
	JoinServer();

	int aPos = 0;
	const int aStrSize = 1024;
	char aStr[aStrSize+1];
	char aChar;

	while(!gDone)
	{
		while(aPos>0 || gConsole.kbhit())
		{
			aChar = gConsole.getch();
			if (aChar == '\b')      // handle backspace
			{
				if(aPos>0) 
				{
					aPos--;
					if(aPos==0) printf_flush("\b\b\b   \b\b\b"); 
					else printf_flush("\b \b"); 

					aStr[aPos] = '\0';
				}
			}
			else if (aChar == '\n' || aChar=='\r')
			{
				printf("\n");
				if(aPos>0) 
				{
					HandleInput(aStr,aPos);
					aPos = 0;
				}

				break;
			}
			else
			{
				if(aPos<aStrSize) 
				{
					if(aPos==0) 
						printf_flush("> ");

					aStr[aPos] = aChar;
					aPos++;
					aStr[aPos] = 0;

					printf_flush("%c",aChar);
				}
			}
		}

		gAPI.Pump(20);
	}

	return 0;
}
