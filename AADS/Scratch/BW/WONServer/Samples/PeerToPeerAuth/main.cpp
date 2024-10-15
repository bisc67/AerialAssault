#include "WONAPI.h"
#include "WONStatus.h"
#include "WONAuth/AuthContext.h"
#include "WONAuth/PeerAuthOp.h"
#include "WONAuth/PeerAuthServerOp.h"
#include "WONSocket/BlockingSocket.h"
#include "WONCommon/StringUtil.h"
#include "WONCommon/WONConsole.h"

using namespace WONAPI;
using namespace std;

WONAPICore gAPI(true);
AuthContextPtr gAuthContext;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PrintCert(Auth2Certificate *theCert)
{
	printf("\nCertificate Data:\n");

	time_t aTime = theCert->GetIssueTime();
	printf("\tIssue Time:\t\t%s",asctime(localtime(&aTime)));
	aTime = theCert->GetExpireTime();
	printf("\tExpire Time:\t\t%s",asctime(localtime(&aTime)));
	printf("\tDuration:\t\t%d minutes\n",(theCert->GetExpireTime() - theCert->GetIssueTime())/60);
	printf("\tUser Id:\t\t%d\n",theCert->GetUserId());
	printf("\tUser name:\t\t%s\n",WStringToString(theCert->GetUserName()).c_str());

	printf("\tCommunity/Trust pairs:\n");
	const Auth2Certificate *theCert2 = (Auth2Certificate*)theCert;
	const CommunityTrustMap &aMap = theCert2->GetCommunityTrustMap();
	CommunityTrustMap::const_iterator anItr = aMap.begin();
	while(anItr!=aMap.end())
	{
		printf("\t\t%d --> %d\n",anItr->first,anItr->second);
		++anItr;
	}

	printf("\n");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PeerAuthCompletion(AsyncOpPtr theOp)
{
	PeerAuthServerOp *anOp = (PeerAuthServerOp*)theOp.get();
	BlockingSocket *aSocket = anOp->GetSocket();

	printf("Finished peer auth from %s: %s\n", aSocket->GetDestAddr().GetHostAndPortString().c_str(), WONStatusToString(anOp->GetStatus()));

	if(anOp->Succeeded())
	{
		printf("Displaying client certificate.\n");
		PrintCert(anOp->GetClientCertificate());
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AcceptCompletion(AsyncOpPtr theOp)
{
	AcceptOp *anOp = (AcceptOp*)theOp.get();
	if(!anOp->Succeeded())
	{
		printf("Accept failed: %s\n",WONStatusToString(anOp->GetStatus()));
		return;
	}

	BlockingSocket *aSocket = (BlockingSocket*)anOp->GetAcceptedSocket();
	printf("Accepted connection from %s.\n",aSocket->GetDestAddr().GetHostAndPortString().c_str());

	PeerAuthServerOpPtr aPeerAuthOp = new PeerAuthServerOp(gAuthContext, aSocket);
	aPeerAuthOp->SetCompletion(new OpCompletion(PeerAuthCompletion));
	aPeerAuthOp->RunAsync(30000);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Server(int theListenPort)
{
	printf("Executing server.\n");

	WONStatus aStatus;
	BlockingSocketPtr anAcceptSocket = new BlockingSocket;

	printf("Binding to listen port... ");
	aStatus = anAcceptSocket->Bind(theListenPort);
	printf("%s\n",WONStatusToString(aStatus));
	if(aStatus!=WS_Success)
		return;

	anAcceptSocket->Listen();
	anAcceptSocket->SetRepeatCompletion(new OpCompletion(AcceptCompletion));
	anAcceptSocket->SetRepeatOp(new AcceptOp);

	printf("Waiting for connections (press 'q' to quit)\n");
	Console aConsole;
	while(tolower(aConsole.getch()!='q'))
	{
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Client(const char *theServerAddr)
{
	printf("Executing client.\n");

	WONStatus aStatus;
	BlockingSocketPtr aSocket = new BlockingSocket;
	
	printf("Performing Peer to Peer Authentication... ");
	PeerAuthOpPtr aPeerAuthOp = new PeerAuthOp(theServerAddr, gAuthContext, AUTH_TYPE_PERSISTENT_NOCRYPT, aSocket);
	aStatus = aPeerAuthOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);
	printf("%s\n",WONStatusToString(aStatus));
	if(aStatus==WS_Success)
	{
		printf("Displaying server certificate.\n");
		PrintCert(aPeerAuthOp->GetServerCertificate());
	}
	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void main(int argc, char *argv[])
{
	if(argc!=6)
	{
		printf("Usage: %s <auth addr:port> <username> <password> [c | s] [server addr | listen port]\n", argv[0]);
		return;
	}

	gAuthContext = new AuthContext;
	gAuthContext->AddAddress(argv[1]);
	gAuthContext->SetUserName(StringToWString(argv[2]));
	gAuthContext->SetPassword(StringToWString(argv[3]));
	gAuthContext->SetCommunity(L"WON");

	printf("Getting certificate... ");
	WONStatus aStatus = gAuthContext->RefreshBlock();
	printf("%s\n",WONStatusToString(aStatus));
	if(aStatus!=WS_Success)
		return;
	
	gAuthContext->SetDoAutoRefresh(true);

	switch(tolower(argv[4][0]))
	{
		case 'c': Client(argv[5]); break;
		case 's': Server(atoi(argv[5])); break;
		default: printf("Invalid Client/Server choice (must be 'c' or 's').\n");
	}
}
