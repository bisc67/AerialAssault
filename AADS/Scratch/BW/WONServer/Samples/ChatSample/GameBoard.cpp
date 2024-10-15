// GameBoard.cpp : implementation file
//

#include "stdafx.h"
#include "ChatSample.h"
#include "GameBoard.h"
#include "CancelDialog.h"
#include "ChatSampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace WONAPI;

/////////////////////////////////////////////////////////////////////////////
// GameBoard dialog

GameBoard::GameBoard(CWnd* pParent /*=NULL*/)
	: CDialog(GameBoard::IDD, pParent)
{
	//{{AFX_DATA_INIT(GameBoard)
	//}}AFX_DATA_INIT
	mApp = CChatSampleDlg::mInstance;

	mWidth = 0;
	mHeight = 0;
	mMouseX = 0;
	mMouseY = 0;

	mNeedDrawBoard = false;
	mWinDialog = NULL;
}


void GameBoard::NeedDrawBoard()
{
	if(!mNeedDrawBoard)
	{
		mNeedDrawBoard = true;
		Invalidate(FALSE);
	}
}

void GameBoard::ResetGame()
{
	mDraggingPiece = false;
	for(int i=0; i<NUM_ROWS; i++)
		for(int j=0; j<NUM_COLS; j++)
			mBoard[i][j] = 0;

	mTurn = 0;
	mTurnCount = 0;

	m_StartGameButton.EnableWindow(GetAmCaptain() && GetNumPlayers()==2);

	NeedDrawBoard();
}


void GameBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GameBoard)
	DDX_Control(pDX, IDC_CHATOUTPUT, m_ChatOutput);
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	DDX_Control(pDX, IDC_GAME_PIECE_AREA, m_GamePieceArea);
	DDX_Control(pDX, IDC_PLAYER2_LABEL, m_Player2Label);
	DDX_Control(pDX, IDC_PLAYER1_LABEL, m_Player1Label);
	DDX_Control(pDX, IDC_GAME_BOARD, m_GameBoard);
	DDX_Control(pDX, IDC_START_GAME, m_StartGameButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GameBoard, CDialog)
	//{{AFX_MSG_MAP(GameBoard)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_LEAVE_GAME, OnLeaveGame)
	ON_BN_CLICKED(IDC_SEND_CHAT, OnSendChat)
	ON_BN_CLICKED(IDC_START_GAME, OnStartGame)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GameBoard message handlers
void GameBoard::DrawBoard()
{
	CRect aRect(0,0,mWidth,mHeight);
	CBrush aBrush(RGB(0,175,255));
	mDrawImageDC.FillRect(&aRect,&aBrush);

	aBrush.DeleteObject();
	aBrush.CreateSolidBrush(RGB(255,255,0));

	aRect = mBoardRect;
		
		//aRect.DeflateRect((mWidth-aWidth)/2,(mHeight-aHeight)/2);
	mDrawImageDC.FillRect(&aRect,&aBrush);
	mBoardRect = aRect;

	aBrush.DeleteObject();
	aBrush.CreateSolidBrush(RGB(0,100,255));
	CBrush *anOldBrush = mDrawImageDC.SelectObject(&aBrush);

	CBrush aRedBrush(RGB(255,0,0));
	CBrush aBlackBrush(RGB(0,0,0));

	CPen aPen(PS_NULL,0,RGB(0,0,0));
	CPen *anOldPen = mDrawImageDC.SelectObject(&aPen);

	for(int i=0; i<NUM_COLS; i++)
	{
		for(int j=0; j<NUM_ROWS; j++)
		{
			int x = aRect.left + i*mColSize + 5;
			int y = aRect.top + j*mRowSize + 5;

			switch(mBoard[j][i])
			{
				case 0: mDrawImageDC.SelectObject(&aBrush); break;
				case 1: mDrawImageDC.SelectObject(&aRedBrush); break;
				case 2: mDrawImageDC.SelectObject(&aBlackBrush); break;
			}

			mDrawImageDC.Ellipse(x,y,x+mColSize-10,y+mRowSize-10);
		}
	}

	mDrawImageDC.SelectObject(anOldPen)->DeleteObject();
	aPen.CreatePen(PS_SOLID,0,RGB(0,0,0));
	mDrawImageDC.SelectObject(&aPen);

	mDrawImageDC.SetBkMode(TRANSPARENT);
	string aPlayer1Str = "Player1: " + mPlayer[0].mName;
	string aPlayer2Str = "Player2: " + mPlayer[1].mName;

	mDrawImageDC.TextOut(mPlayer1LabelRect.left,mPlayer1LabelRect.top,aPlayer1Str.c_str());
	mDrawImageDC.TextOut(mPlayer2LabelRect.left,mPlayer2LabelRect.top,aPlayer2Str.c_str());

	mDrawImageDC.SelectObject(anOldBrush);

	aBrush.DeleteObject();
	aBrush.CreateSolidBrush(RGB(200,225,255));
	mDrawImageDC.SelectObject(&aBrush);
	mDrawImageDC.Rectangle(&mGamePieceAreaRect);

/*	mDrawImageDC.SelectObject(anOldBrush)->DeleteObject();
	aBrush.CreateSolidBrush(RGB(255,0,0));
	mDrawImageDC.SelectObject(&aBrush);
	mDrawImageDC.Ellipse(&mGamePieceRect);*/
	DrawGamePiece();

	mDrawImageDC.SelectObject(anOldBrush);
	mDrawImageDC.SelectObject(anOldPen);
	aBrush.DeleteObject();
	aRedBrush.DeleteObject();
	aBlackBrush.DeleteObject();
	aPen.DeleteObject();
	Invalidate(FALSE);

}

void GameBoard::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(cx>0 && cy>0)
	{
		mWidth = cx;
		mHeight = cy;
	}
}

void GameBoard::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(mNeedDrawBoard)
	{
		DrawBoard();
		mNeedDrawBoard = false;
	}

	dc.BitBlt(0,0,mWidth,mHeight,&mDrawImageDC,0,0,SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void GameBoard::DrawGamePiece()
{
	mGamePieceBGDC.BitBlt(0,0,mGamePieceRect.Width(),mGamePieceRect.Height(),&mDrawImageDC,mGamePieceRect.left,mGamePieceRect.top,SRCCOPY);
	if(mTurn==0)
		return;

	CPen *anOldPen;
	CBrush *anOldBrush;
	
	CBrush aBrush(mTurn==1?RGB(255,0,0):RGB(0,0,0));
	CPen aPen(PS_SOLID,0,RGB(0,0,0));
	
	anOldBrush = mDrawImageDC.SelectObject(&aBrush);
	anOldPen = mDrawImageDC.SelectObject(&aPen);

	mDrawImageDC.Ellipse(mGamePieceRect);

	mDrawImageDC.SelectObject(anOldBrush);
	mDrawImageDC.SelectObject(anOldPen);

	aBrush.DeleteObject();
	aPen.DeleteObject();
}

void GameBoard::RestoreGamePieceBG()
{
	mDrawImageDC.BitBlt(mGamePieceRect.left,mGamePieceRect.top,mGamePieceRect.Width(),mGamePieceRect.Height(),&mGamePieceBGDC,0,0,SRCCOPY);
}

BOOL GameBoard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ChatOutput.SetBackgroundColor(FALSE,RGB(0,0,0));
	CHARFORMAT aFormat;
	aFormat.cbSize = sizeof(aFormat);
	aFormat.dwMask = CFM_COLOR;
	aFormat.dwEffects = 0;
	aFormat.crTextColor = RGB(0,255,0);
	m_ChatOutput.SetDefaultCharFormat(aFormat);
	
	CDC *aDC = GetDC();
	mDrawImage.CreateCompatibleBitmap(aDC,mWidth,mHeight);
	mDrawImageDC.CreateCompatibleDC(aDC);
	mDrawImageDC.SelectObject(&mDrawImage);

	m_GameBoard.GetWindowRect(&mBoardRect);
	ScreenToClient(&mBoardRect);

	m_Player1Label.GetWindowRect(&mPlayer1LabelRect);
	ScreenToClient(&mPlayer1LabelRect);

	m_Player2Label.GetWindowRect(&mPlayer2LabelRect);
	ScreenToClient(&mPlayer2LabelRect);

	m_GamePieceArea.GetWindowRect(&mGamePieceAreaRect);
	ScreenToClient(&mGamePieceAreaRect);

	mColSize = mBoardRect.Width()/NUM_COLS;
	mRowSize = mBoardRect.Height()/NUM_ROWS;
	mPieceWidth = mColSize - 10;
	mPieceHeight = mRowSize - 10;

	ResetPiece();
	mGamePieceBG.CreateCompatibleBitmap(aDC,mPieceWidth,mPieceHeight);
	mGamePieceBGDC.CreateCompatibleDC(aDC);
	mGamePieceBGDC.SelectObject(&mGamePieceBG);

	mPlayerLabelFont.CreateFont(15,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"SansSerif");

	mDrawImageDC.SelectObject(&mPlayerLabelFont);


	ReleaseDC(aDC);
	NeedDrawBoard();

	m_ChatInput.SetFocus();	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GameBoard::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(mDraggingPiece)
	{
		RestoreGamePieceBG();
		InvalidateRect(&mGamePieceRect,FALSE);

		int dx = point.x - mMouseX;
		int dy = point.y - mMouseY;

		mGamePieceRect.left+=dx;
		mGamePieceRect.right+=dx;
		mGamePieceRect.top+=dy;
		mGamePieceRect.bottom+=dy;

		DrawGamePiece();
		InvalidateRect(&mGamePieceRect,FALSE);

		mDragPoint = point;
	}

	mMouseX = point.x;
	mMouseY = point.y;
	
	CDialog::OnMouseMove(nFlags, point);
}

BOOL GameBoard::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CChatSampleApp *pApp = (CChatSampleApp *)AfxGetApp();
	if(mGamePieceRect.PtInRect(CPoint(mMouseX,mMouseY)) && mTurn==GetMyTurn())
	{
		SetCursor(pApp->LoadCursor(IDC_HAND_CURSOR));
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void GameBoard::ResetPiece()
{
	int x = mGamePieceAreaRect.left + (mGamePieceAreaRect.Width()-mPieceWidth)/2;
	int y = mGamePieceAreaRect.top + (mGamePieceAreaRect.Height()-mPieceHeight)/2;

	mGamePieceRect.SetRect(x,y,x+mPieceWidth,y+mPieceHeight);
	NeedDrawBoard();
}

int GameBoard::CheckWin(int theRow, int theCol)
{	
	int dx[] = {1,0,1,-1};
	int dy[] = {0,1,1,1};
	int col, row, curPiece, curCount;

	int i,j;
	for(i=0; i<4; i++)
	{
		col = theCol-3*dx[i];
		row = theRow-3*dy[i];
		curPiece = curCount = 0;
		for(j=0; j<8; j++)
		{
			if(col>=0 && col<NUM_COLS && row>=0 && row<NUM_ROWS)
			{
				int aPiece = mBoard[row][col];
				if(aPiece!=0 && aPiece==curPiece)
				{
					curCount++;
					if(curCount==4)
						return curPiece;
				}
				else
				{
					curCount = 1;
					curPiece = aPiece;
				}
			}
				
			col+=dx[i];
			row+=dy[i];
		}	
	}
	
	if(mTurnCount==NUM_ROWS*NUM_COLS)
		return -1;
	else
		return 0;
}

void GameBoard::OtherPlayerDropPiece(int theCol)
{
	if(theCol<0 || theCol>=NUM_COLS)
		return;

	if(mBoard[0][theCol]!=0) // nowhere to put it
		return;

	mStoreCol = theCol;
	
	RestoreGamePieceBG();
	InvalidateRect(&mGamePieceRect,FALSE);

	int aWidth = mGamePieceRect.Width();
	int aHeight = mGamePieceRect.Height();
	mGamePieceRect.left = mBoardRect.left+mColSize*theCol+5;
	mGamePieceRect.top = mBoardRect.top-mRowSize/2;
	mGamePieceRect.right = mGamePieceRect.left+aWidth;
	mGamePieceRect.bottom = mGamePieceRect.top+aHeight;

	DrawGamePiece();
	InvalidateRect(&mGamePieceRect,FALSE);
	SetTimer(0,500,NULL);
}

void GameBoard::DropPiece(int theCol)
{
	for(int i=NUM_ROWS-1; i>=0; i--)
	{
		if(mBoard[i][theCol]==0)
		{
			if(mTurn==GetMyTurn())
			{
				WriteBuffer aBuf;
				aBuf.AppendByte(MSG_MOVE);
				aBuf.AppendByte(theCol);
				mApp->SendGameData(aBuf.ToByteBuffer());
			}

			mBoard[i][theCol]=mTurn;
			mTurnCount++;
			int aWinner = CheckWin(i,theCol);
			if(aWinner!=0)
			{
				mTurn = 0;
				NeedDrawBoard();
				mWinDialog = new CancelDialog;
				mWinDialog->m_Caption = "Game Over";
				mWinDialog->m_ButtonText = "Ok";
				switch(aWinner)
				{
					case -1: mWinDialog->m_Message = "It's a tie."; break;
					case 1: mWinDialog->m_Message = (mPlayer[0].mName + " wins.").c_str(); break;
					case 2: mWinDialog->m_Message = (mPlayer[1].mName + " wins.").c_str(); break;
				}

				int aResult = mWinDialog->DoModal();
				ResetGame();

				delete mWinDialog;
				mWinDialog = NULL;
				if(aResult==IDOK)
					StartGame();

				return;
			}
			
			if(mTurn==1) mTurn=2; else mTurn=1;
			break;
		}
	}
}

void GameBoard::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(mGamePieceRect.PtInRect(point) && mTurn==GetMyTurn())
	{
		SetCapture();
		mDraggingPiece = true;
		mDragPoint = point;
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void GameBoard::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(mDraggingPiece)
	{
		mDraggingPiece = false;
		ReleaseCapture();

		int aCol = (point.x - mBoardRect.left)/mColSize;
		if(aCol>=0 && aCol<NUM_COLS && point.y<mBoardRect.bottom)
			DropPiece(aCol);
			
		ResetPiece();
		NeedDrawBoard();
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void GameBoard::OnLeaveGame() 
{
	// TODO: Add your control notification handler code here
	mApp->LeaveGameGroup();	
}

void GameBoard::InitializeGroup(RoutingGroupInfo *theGroup)
{
	ResetGame();

	SetWindowText(WStringToString(theGroup->mName).c_str());
	mWaitingForGameState = false;

	for(int i=0; i<2; i++)
		mPlayer[i].Reset();

	mTurn = 0;
	mMyId = mApp->mRoutingConnection->GetMyClientId();

	mGroup = theGroup;
	RoutingMemberMap::iterator anItr = theGroup->mMemberMap.begin();
	while(anItr!=theGroup->mMemberMap.end())
	{
		RoutingMemberInfo *aMember = anItr->second;
		AddMember(aMember);

		if(aMember->mClientId==mMyId && aMember->mClientId==theGroup->mCaptainId)
			SetAmCaptain(true);

		++anItr;
	}

	NeedDrawBoard();
	if(mApp->mObserveGame)
	{
		WriteBuffer aBuf;
		aBuf.AppendByte(MSG_GET_GAME_STATE);
		mApp->SendGameData(aBuf.ToByteBuffer());
		mWaitingForGameState = true;
	}
}


int GameBoard::GetMyTurn()
{
	for(int i=0; i<2; i++)
		if(mPlayer[i].mId==mMyId)
			return i+1;

	return -1;
}

int GameBoard::GetNumPlayers()
{
	int aCount = 0;
	for(int i=0; i<2; i++)
		if(mPlayer[i].Valid())
			aCount++;
		
	return aCount;
}

bool GameBoard::GetAmCaptain()
{
	return mMyId==mPlayer[0].mId;
}

void GameBoard::AddMember(RoutingMemberInfo *theMember)
{
	if(!(theMember->mFlags&RoutingMemberFlag_IsObserver))
	{
		int anIdx;
		if(theMember->mClientId==mGroup->mCaptainId)
			anIdx = 0;
		else
			anIdx = 1;

		mPlayer[anIdx].mId = theMember->mClientId;
		mPlayer[anIdx].mName = WStringToString(theMember->mClientInfo->mName);

		m_StartGameButton.EnableWindow(GetAmCaptain() && GetNumPlayers()==2);
		NeedDrawBoard();
	}
}

void GameBoard::RemoveMember(unsigned short theId)
{
	for(int i=0; i<2; i++)
	{
		if(theId==mPlayer[i].mId)
		{
			mPlayer[i].Reset();
			m_StartGameButton.EnableWindow(FALSE);
			ResetGame();
			NeedDrawBoard();
			break;
		}
	}
}

void GameBoard::SetAmCaptain(bool amCaptain)
{
	if(mMyId==mPlayer[0].mId && !amCaptain)
		swap(mPlayer[0],mPlayer[1]);
	else if(mMyId==mPlayer[1].mId && amCaptain)
		swap(mPlayer[0],mPlayer[1]);

	m_StartGameButton.EnableWindow(amCaptain && GetNumPlayers()==2);
	NeedDrawBoard();
}

void GameBoard::OnSendChat() 
{
	// TODO: Add your control notification handler code here
	CString aChatStr;
	m_ChatInput.GetWindowText(aChatStr);
	mApp->HandleInput(aChatStr,true);
	m_ChatInput.SetWindowText("");
		
}

void GameBoard::StartGame()
{
	if(mWinDialog!=NULL)
	{
		mWinDialog->EndDialog(IDOK);
		return;
	}

	mTurn = 1;
	NeedDrawBoard();

	m_StartGameButton.EnableWindow(FALSE);
	if(GetAmCaptain())
	{
		WriteBuffer aBuf;
		aBuf.AppendByte(MSG_START_GAME);
		mApp->SendGameData(aBuf.ToByteBuffer());
	}
}

void GameBoard::HandleMessage(const ByteBuffer *theMsg, unsigned short theSenderId)
{
	ReadBuffer aMsg(theMsg->data(),theMsg->length());
	try
	{
		unsigned char aMsgType = aMsg.ReadByte();
		if(mWaitingForGameState && aMsgType!=MSG_GAME_STATE)
			return;

		switch(aMsgType)
		{
			case MSG_START_GAME:
			{
				if(mPlayer[0].mId==theSenderId)
					StartGame();
			}
			break;

			case MSG_MOVE:
			{
				unsigned char aCol = aMsg.ReadByte();
				int anIdx = mTurn-1;
				if(anIdx>=0 && anIdx<2 && mPlayer[anIdx].mId==theSenderId)
					OtherPlayerDropPiece(aCol);
			}
			break;

			case MSG_GET_GAME_STATE:
			{
				if(GetAmCaptain())
				{				
					WriteBuffer aReply;
					aReply.AppendByte(MSG_GAME_STATE);
					aReply.AppendByte(mTurn);
					aReply.AppendByte(mTurnCount);
					for(int i=0; i<NUM_ROWS; i++)
						for(int j=0; j<NUM_COLS; j++)
							aReply.AppendByte(mBoard[i][j]);

					mApp->SendGameDataTo(aReply.ToByteBuffer(),theSenderId);
				}

			}
			break;

			case MSG_GAME_STATE:
			{
				if(mWaitingForGameState && theSenderId==mPlayer[0].mId)
				{
					mWaitingForGameState = false;
					mTurn = aMsg.ReadByte();
					mTurnCount = (unsigned char)aMsg.ReadByte();
					for(int i=0; i<NUM_ROWS; i++)
						for(int j=0; j<NUM_COLS; j++)
							mBoard[i][j] = aMsg.ReadByte();

					NeedDrawBoard();
				}
			}
			break;


		}
	}
	catch(ReadBufferException&)
	{
	}
}


void GameBoard::OnStartGame() 
{
	// TODO: Add your control notification handler code here
	StartGame();	
}

void GameBoard::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	DropPiece(mStoreCol);			
	ResetPiece();
	NeedDrawBoard();
		
	CDialog::OnTimer(nIDEvent);
}
