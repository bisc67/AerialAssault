#include "StdAfx.h"
#include "ChatManager.h"
#include "JoinRequestDialog.h"
#include "WindowsMessages.h"

using namespace WONAPI;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
ChatManager::ChatManager()
{
	mChatOutput = NULL;
	mChatInput = NULL;
	mUserList = NULL;
	mSendButton = NULL;

	COLOR_USER			= RGB(  0,255,  0);
	COLOR_BROADCAST		= RGB(255,255,255);
	COLOR_WHISPER		= RGB(255,255,  0);
	COLOR_EMOTE			= RGB(255,  0,255);
	COLOR_MESSAGE		= RGB(255,255,255);
	COLOR_ERROR			= RGB(255,  0,  0);
	COLOR_SYSTEM		= RGB(255,255,255);
	COLOR_MOTD			= RGB(255,255,  0);
	COLOR_INVITE		= RGB(  0,255,255);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
ChatManager::~ChatManager()
{
	RestoreChatInput();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::RestoreChatInput()
{
	if(mChatInput==NULL || mChatInput->m_hWnd==NULL)
		return;

	::SetWindowLong(mChatInput->m_hWnd, GWL_WNDPROC, (LONG) mOldChatInputProc);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::SetControls(HWND theMainWindow, CEdit *theChatInput, RicherEditCtrl *theChatOutput, 
							  DataSortListCtrl *theUserList, CButton *theSendButton)
{
	mMainWindow = theMainWindow;
	mChatInput = theChatInput;
	mChatOutput = theChatOutput;
	mUserList = theUserList;
	mSendButton = theSendButton;

	mInputBuffer.clear();
	mInputBufferIt = mInputBuffer.begin();

	if(mChatInput!=NULL)
	{
		SetWindowLong(mChatInput->m_hWnd, GWL_USERDATA, (LONG) this);
		mOldChatInputProc = (WNDPROC) ::SetWindowLong(mChatInput->m_hWnd, GWL_WNDPROC, (LONG) StaticChatInputProc);		
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
long FAR PASCAL ChatManager::StaticChatInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ChatManager *that = (ChatManager*)::GetWindowLong(hWnd, GWL_USERDATA);
	return that->ChatInputProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
long ChatManager::ChatInputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_GETDLGCODE:
		{
			long aRet = CallWindowProc(mOldChatInputProc, mChatInput->m_hWnd, message, wParam, lParam);
			if (lParam)
			{
				LPMSG aMsg = (LPMSG)lParam;
				if (aMsg->message==WM_KEYDOWN && aMsg->wParam==VK_RETURN)
					aRet |= DLGC_WANTMESSAGE;
			}
			return aRet;
		}
		break;


		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				case VK_DOWN:
					if(mInputBuffer.size() > 0)
					{
						if(mInputBufferIt != mInputBuffer.end())
							mInputBufferIt++;
						if(mInputBufferIt == mInputBuffer.end())
							mChatInput->SetWindowText("");
						else
						{
							mChatInput->SetWindowText(*mInputBufferIt);
							mChatInput->SetSel(mInputBufferIt->GetLength(), mInputBufferIt->GetLength());
						}

					}
					return 0;

				case VK_UP:
					if(mInputBuffer.size() > 0)
					{
						if(mInputBufferIt != mInputBuffer.begin())
							mInputBufferIt--;
						
						mChatInput->SetWindowText(*mInputBufferIt);
						mChatInput->SetSel(mInputBufferIt->GetLength(), mInputBufferIt->GetLength());
					}
					return 0;

				case VK_RETURN:
					HandleInput();
					return 0;
			}
		}
		break;

		case WM_CHAR:
			if(wParam==0x0d)
				return 0;

	}

	return CallWindowProc(mOldChatInputProc, mChatInput->m_hWnd, message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ChatManager::AppendChat(const std::string &theStr)
{
	if(mChatOutput!=NULL)
		mChatOutput->Append(theStr.c_str());
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ChatManager::AppendChat(const std::string &theStr, COLORREF theColor)
{
	if(mChatOutput!=NULL)
		mChatOutput->Append(theStr.c_str(), theColor);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::Init(WONAPI::SmartRoutingConnection *theConnection, unsigned short theGroupId)
{
	mRoutingConnection = theConnection;
	mGroupId = theGroupId;

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(mGroupId);
	if(aGroup.get()==NULL)
	{
		LeaveGroup();
		return;
	}

	if(mUserList!=NULL)
	{
		RoutingMemberMap::iterator anItr = aGroup->mMemberMap.begin();
		while(anItr!=aGroup->mMemberMap.end())
		{
			mUserList->AddItem(anItr->second,false);
			++anItr;
		}


		mUserList->Sort();
	}

	if(mSendButton!=NULL)
		mSendButton->EnableWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool ChatManager::LeaveGroup()
{
	if(mRoutingConnection.get()!=NULL)
	{
		mRoutingConnection->LeaveGroup(mGroupId);
		mRoutingConnection = NULL;

		if(mSendButton!=NULL)
			mSendButton->EnableWindow(FALSE);

		if(mUserList!=NULL)
			mUserList->DeleteAllItems();

		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleRoutingOp(WONAPI::RoutingOp *theOp)
{
	if(theOp->GetConnection()!=mRoutingConnection)
		return;

	switch(theOp->GetType())
	{
		case RoutingOp_PeerChat:				HandlePeerChat(theOp);				break;
		case RoutingOp_ClientJoinedGroup:		HandleClientJoined(theOp);			break;
		case RoutingOp_ClientLeftGroup:			HandleClientLeft(theOp);			break;
		case RoutingOp_ClientFlagsChanged:		HandleClientFlagsChanged(theOp);	break;
		case RoutingOp_MemberFlagsChanged:		HandleMemberFlagsChanged(theOp);	break;
		case RoutingOp_GroupInvitation:			HandleInvitation(theOp);			break;
		case RoutingOp_GroupJoinAttempt:		HandleJoinRequest(theOp);			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleClientJoined(RoutingOp *theOp)
{
	if(mUserList==NULL)
		return;

	RoutingClientJoinedGroupOp *anOp = (RoutingClientJoinedGroupOp*)theOp;
	unsigned short aGroupId = anOp->GetGroupId();
	unsigned short aClientId = anOp->GetClientId();

	if(aGroupId!=mGroupId)
		return;

	RoutingMemberInfo *aMember = mRoutingConnection->GetMemberRef(aClientId,aGroupId);
	if(aMember!=NULL)
		mUserList->AddItem(aMember);
	
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleClientLeft(RoutingOp *theOp)
{
	if(mUserList==NULL)
		return;

	RoutingClientLeftGroupOp *anOp = (RoutingClientLeftGroupOp*)theOp;
	if(anOp->GetGroupId()!=mGroupId)
		return;

	unsigned short aClientId = anOp->GetClientId();
	RoutingMemberInfo *aMember = mRoutingConnection->GetMemberRef(aClientId,mGroupId);
	if(aMember!=NULL)
		mUserList->RemoveItem(aMember);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleClientFlagsChanged(RoutingOp *theOp)
{
	if(mUserList==NULL)
		return;

	RoutingClientFlagsChangedOp *anOp = (RoutingClientFlagsChangedOp*)theOp;
	RoutingMemberInfo *aMember = mRoutingConnection->GetMemberRef(anOp->GetClientId(),mGroupId);
	if(aMember!=NULL)
		mUserList->UpdateItem(aMember);

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleMemberFlagsChanged(RoutingOp *theOp)
{
	if(mUserList==NULL)
		return;

	RoutingMemberFlagsChangedOp *anOp = (RoutingMemberFlagsChangedOp*)theOp;
	int aGroupId = anOp->GetGroupId();
	if(aGroupId!=mGroupId)
		return;

	RoutingMemberInfo *aMember = mRoutingConnection->GetMemberRef(anOp->GetClientId(),aGroupId);
	if(aMember!=NULL)
		mUserList->UpdateItem(aMember);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleInvitation(RoutingOp *theOp)
{
	if(mChatInput==NULL)
		return;

	RoutingGroupInvitationOp *anOp = (RoutingGroupInvitationOp*)theOp;

	int aGroupId = anOp->GetGroupId();
	int aClientId = anOp->GetCaptainId();

	RoutingClientInfoPtr aClient = mRoutingConnection->GetClientRef(aClientId);
	if(aClient.get()==NULL)
		return;

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(aGroupId);
	if(aGroup.get()==NULL)
		return;

	AppendChat(WStringToString(aClient->mName),COLOR_USER);
	if(anOp->GetAmInvited())
		AppendChat(" invites you to join ",COLOR_INVITE);
	else
		AppendChat(" uninvites you from ",COLOR_INVITE);

	AppendChat(WStringToString(aGroup->mName),COLOR_INVITE);
	if(anOp->GetComment().empty())
		AppendChat(".");
	else
	{
		AppendChat(": ");
		AppendChat(WStringToString(anOp->GetComment()),COLOR_WHISPER);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleJoinRequest(RoutingOp *theOp)
{
	RoutingGroupJoinAttemptOp *anOp = (RoutingGroupJoinAttemptOp*)theOp;
	if(mGroupId!=anOp->GetGroupId())
		return;

	RoutingClientInfoPtr aClient = mRoutingConnection->GetClientRef(anOp->GetClientId());
	if(aClient.get()==NULL)
		return;

	string aStr = "Allow " + WStringToString(aClient->mName) + " to join game?";

	JoinRequestDialog aDlg;
	aDlg.m_JoinText = aStr.c_str();
	int aResult = aDlg.DoModal();

	if(mRoutingConnection.get()==NULL)
		return;

	mRoutingConnection->AcceptClient(anOp->GetGroupId(),anOp->GetClientId(),aResult==IDOK, StringToWString((const char*)aDlg.m_Comment));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::HandlePeerChat(RoutingOp *theOp)
{
	if(mChatOutput==NULL)
		return;

	RoutingPeerChatOp *anOp = (RoutingPeerChatOp*)theOp;

	RoutingClientInfoPtr aSendClient = mRoutingConnection->GetClientRef(anOp->GetSenderId());
	if(aSendClient.get()==NULL)
		return;

	const RoutingRecipientList &aList = anOp->GetRecipients();
	if(aList.empty())
		return;

	int aDestId = aList.front();
	if(!anOp->IsWhisper() && aDestId!=mGroupId)
		return;

	if(anOp->IsEmote())
	{
		AppendChat(WStringToString(aSendClient->mName),COLOR_USER);
		AppendChat(WStringToString(anOp->GetText()),COLOR_EMOTE);
	}		
	else if(anOp->IsWhisper())
	{
		if(aSendClient->mId==mRoutingConnection->GetMyClientId()) // Whisper from me
		{
			RoutingClientInfoPtr aDestClient = mRoutingConnection->GetClientRef(aDestId);
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

	AppendChat("\r\n");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ChatManager::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(mRoutingConnection.get()==NULL)
		return;

	if(message==WM_COMMAND)
	{
		if(HIWORD(wParam)==BN_CLICKED)
		{
			if(mSendButton!=NULL && mSendButton->m_hWnd==(HWND)lParam)
			{
				HandleInput();
			}
		}
	}
	else if(message==MSG_UPDATE_CLIENT)
	{
		if(mUserList!=NULL)
		{
			RoutingMemberInfo *aMember = mRoutingConnection->GetMemberRef((unsigned short)lParam, mGroupId);
			if(aMember!=NULL)
				mUserList->UpdateItem(aMember);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ChatManager::HandleInput()
{
	if(mRoutingConnection.get()==NULL)
		return;

	if(mChatInput==NULL)
		return;

	// Get input string
	CString anInputStr;
	mChatInput->GetWindowText(anInputStr);
	if(anInputStr.GetLength()==0)
		return;

	// Deal with input buffer
	if(mInputBufferIt==mInputBuffer.end())
		mInputBuffer.push_back(anInputStr);
	else if(*mInputBufferIt!=anInputStr)
	{
		mInputBuffer.push_back(anInputStr);
		mInputBufferIt = mInputBuffer.end();
	}
	else 
		mInputBufferIt++; // make it so pushing up-arrow will get back to same string

	if(mInputBuffer.size()>20)
		mInputBuffer.pop_front();

	// Clear text
	mChatInput->SetWindowText("");

	// Examine command
	wstring anInput = StringToWString((const char*)anInputStr);
	if(mRoutingConnection->HandleInput(anInput,mGroupId))
	{
		SmartRoutingConnection::InputCommand aCommand = mRoutingConnection->GetLastInputCommand();
		if(aCommand==SmartRoutingConnection::InputCommand_Ignore)
		{
			RoutingClientInfo *aClient = mRoutingConnection->GetLastTargetClientRef();
			if(aClient!=NULL)
				PostMessage(mMainWindow,MSG_UPDATE_CLIENT,0,aClient->mId);
		}
	}
	else
	{
		switch(mRoutingConnection->GetLastInputError())
		{
			case SmartRoutingConnection::InputError_ClientNotFound:
				AppendChat("Client not found: ",COLOR_ERROR);
				AppendChat(WStringToString(mRoutingConnection->GetLastMatchName()));
				AppendChat("\r\n");
				break;

			case SmartRoutingConnection::InputError_ClientAmbiguous:
				AppendChat("Name could match multiple clients: ",COLOR_ERROR);
				AppendChat(WStringToString(mRoutingConnection->GetLastMatchName()));
				AppendChat("\r\n");
				break;

			case SmartRoutingConnection::InputError_InvalidCommand:
				AppendChat("InvalidCommand.\r\n",COLOR_ERROR);
				break;

			case SmartRoutingConnection::InputError_BadTime:
				AppendChat("Bad time specification.\r\n",COLOR_ERROR);
				break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int ChatManager::UserListImage(void *theMember, void *theParam)
{
	RoutingMemberInfo *aMember = (RoutingMemberInfo*)theMember;
	SmartRoutingClientInfo *aClient = (SmartRoutingClientInfo*)aMember->mClientInfo.get();

	bool muted = aMember->IsMuted();
	bool moderator = (aClient->mFlags&RoutingClientFlag_IsModerator)?true:false;
	bool away = (aClient->mFlags&RoutingClientFlag_IsAway)?true:false;
	bool ignored = aClient->mIgnored;

	if(ignored) return UserListImage_Ignored;
	else if(muted) return UserListImage_Muted;
	else if(away) return 	UserListImage_Away;
	else if(moderator) return UserListImage_Moderator;
	else return UserListImage_Normal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CString ChatManager::UserListText(void *theMember, int theIndex, void *theParam)
{
	RoutingMemberInfo *aMember = (RoutingMemberInfo*)theMember;
	return WStringToString(aMember->mClientInfo->mName).c_str();	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int ChatManager::UserListSort(void *theM1, void *theM2, int theIndex, void *theParam)
{
	RoutingMemberInfo *m1 = (RoutingMemberInfo*)theM1;
	RoutingMemberInfo *m2 = (RoutingMemberInfo*)theM2;

	return m1->mClientInfo->mName.compare(m2->mClientInfo->mName);
}
