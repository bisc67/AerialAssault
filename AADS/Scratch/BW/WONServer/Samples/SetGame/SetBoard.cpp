#include "stdafx.h"
#include "SetBoard.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SetBoard::SetBoard()
{
	mDC = NULL;
	mBitmap = NULL;
	mOldBitmap = NULL;

	mNumCols = 4;
	mNumRows = 3;

	mActive = false;

	for(int i=0; i<21; i++)
		mIdToPos[i] = -1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SetBoard::~SetBoard()
{
	FreeGDI();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SetBoard::GetPos(int theId)
{
	if(theId<0)
		return -1;

	return mIdToPos[theId];
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::FreeGDI()
{
	if(mDC!=NULL)
	{
		SelectObject(mDC,mOldBitmap);
		DeleteDC(mDC);
	}

	if(mBitmap!=NULL)
		DeleteObject(mBitmap);

	mDC = NULL;
	mBitmap = NULL;

	Clear(false);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::Clear(bool redraw)
{
	for(int i=0; i<mCardVector.size(); i++)
	{
		delete mCardVector[i];
		mIdToPos[i] = -1;
	}

	mCardVector.resize(0);
	if(redraw)
		Redraw();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SetBoard::GetNumCards()
{
	int aVal = 0;
	for(int i=0; i<mCardVector.size(); i++)
		if(mCardVector[i]!=NULL)
			aVal++;

	return aVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SetBoard::GetSelection(SetCard *c[3])
{
	int aNumSelected = 0;
	for(int i=0; i<mCardVector.size(); i++)
	{
		SetCard *aCard = mCardVector[i];
		if(aCard!=NULL && aCard->IsSelected())
		{
			c[aNumSelected++] = aCard;
			if(aNumSelected==3)
				break;
		}
	}

	return aNumSelected;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool SetBoard::IsSet(SetCard *c[3])
{
	int attrib[3];
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
			attrib[j] = c[j]->GetAttribute(i);

		bool allSame = attrib[0]==attrib[1] && attrib[1]==attrib[2];
		bool allDifferent = attrib[0]!=attrib[1] && attrib[0]!=attrib[2] && attrib[1]!=attrib[2];

		if(!allSame && !allDifferent)
			return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SetBoard::CheckSelection()
{
	SetCard *c[3];
	int aNumSelected = GetSelection(c);
	if(aNumSelected!=3)
		return 0;

	bool isSet = IsSet(c);
	COLORREF aColor = isSet?RGB(0,255,0):RGB(255,0,0);
	for(int i=0; i<3; i++)
	{
		c[i]->SetSelectColor(aColor);
		c[i]->Draw(mDC);
	}

	if(isSet)
		return 1;
	else
		return -1;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::AddCard(int theId, SetCard *theCard)
{
	int aPos = GetPos(theId);
	if(aPos==-1)
	{
		for(int i=0; i<mCardVector.size(); i++)
			if(mCardVector[i]==NULL)
				aPos = i;

		if(aPos==-1)
			aPos = mCardVector.size();
	}


	if(aPos>=mCardVector.size())
		mCardVector.resize(aPos+1,NULL);

	RemoveCard(theId);
	mCardVector[aPos] = theCard;
	theCard->mId = theId;
	mIdToPos[theId] = aPos;
	ReshapeCard(aPos);
	theCard->Draw(mDC);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::RemoveCard(int theId)
{
	int aPos = GetPos(theId);
	if(aPos<mCardVector.size())
	{
		SetCard *aCard = mCardVector[aPos];
		if(aCard!=NULL)
		{
			ClearArea(aCard->left,aCard->top,aCard->Width(),aCard->Height(),false);
			delete aCard;
		}
		mCardVector[aPos] = NULL;
	}

	mIdToPos[theId] = -1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::ReshapeCard(int thePos)
{
	int aSpaceX = 4;
	int aSpaceY = 4;

	int aHorzPad = 4;
	int aVertPad = 4;

	double aCardWidth = (Width()-aHorzPad-aSpaceX*(mNumCols-1))/(double)mNumCols;
	double aCardHeight = (Height()-aVertPad-aSpaceY*(mNumRows-1))/(double)mNumRows;

	int aCorrectHeight = aCardWidth*4/3;
	if(aCardHeight<aCorrectHeight)
		aHorzPad += (aCorrectHeight-aCardHeight)*mNumCols*3/4;
	else 
		aVertPad += (aCardHeight-aCorrectHeight)*mNumRows*4/3;

	aCardWidth = (Width()-aHorzPad-aSpaceX*(mNumCols-1))/(double)mNumCols;
	aCardHeight = (Height()-aVertPad-aSpaceY*(mNumRows-1))/(double)mNumRows;
	
	double aWidth = aCardWidth+aSpaceX;
	double aHeight = aCardHeight+aSpaceY;

	int x = aHorzPad/2 + (thePos%mNumCols)*aWidth;
	int y = aVertPad/2 + (thePos/mNumCols)*aHeight;

	SetCard *aCard = mCardVector[thePos];
	if(aCard!=NULL)
		aCard->SetRect(x,y,x+aCardWidth,y+aCardHeight);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::ReshapeCards()
{

	for(int i=0; i<mCardVector.size(); i++)
		ReshapeCard(i);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::RepackCards()
{
	int i;
	int aFreePos = mCardVector.size();
	for(i=0; i<mCardVector.size(); i++)
	{
		if(mCardVector[i]==NULL)
		{
			aFreePos = i++;
			break;
		}
	}

	for(; i<mCardVector.size(); i++)
	{
		if(mCardVector[i]!=NULL)
		{
			int aPos = aFreePos++;
			mCardVector[aPos] = mCardVector[i];
			mIdToPos[mCardVector[aPos]->mId] = aPos;
		}
	}

	mCardVector.resize(aFreePos);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::SetGrid(int theNumRows, int theNumCols)
{
	if(mNumRows==theNumRows && mNumCols==theNumCols)
		return;

	mNumRows = theNumRows;
	mNumCols = theNumCols;
	RepackCards();
	ReshapeCards();
	Redraw();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::ClearArea(int x, int y, int width, int height, bool drawBorder)
{
	HANDLE aNewPen = CreatePen(PS_SOLID,0,drawBorder?RGB(0,0,0):RGB(130,132,130));
	HANDLE aNewBrush = CreateSolidBrush(RGB(130,132,130)); 
/*	if(mActive)
		aNewBrush = GetStockObject(LTGRAY_BRUSH);*/

	HANDLE anOldPen = SelectObject(mDC, aNewPen);
	HANDLE anOldBrush = SelectObject(mDC, aNewBrush);
	Rectangle(mDC, x, y, x+width, y+height);

	SelectObject(mDC,anOldPen);
	SelectObject(mDC,anOldBrush);

	DeleteObject(aNewPen);
	DeleteObject(aNewBrush);

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::SetRect(HDC theDC, int x1, int y1, int x2, int y2)
{
	CRect::SetRect(x1,y1,x2,y2);

	FreeGDI();
	mDC = CreateCompatibleDC(theDC);
	mBitmap = CreateCompatibleBitmap(theDC,Width(),Height());
	mOldBitmap = (HBITMAP)SelectObject(mDC,mBitmap);

	ReshapeCards();
	Redraw();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::Redraw()
{
	ClearArea(0,0,Width(),Height(),true);
	for(int i=0; i<mCardVector.size(); i++)
	{
		SetCard *aCard = mCardVector[i];
		if(aCard!=NULL)
			aCard->Draw(mDC);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::Draw(HDC theDC)
{
	BitBlt(theDC,left,top,Width(),Height(),mDC,0,0,SRCCOPY);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SetBoard::FindCardPos(CPoint &thePoint)
{
	if(!PtInRect(thePoint))
		return -1;

	CPoint aPoint(thePoint.x-left, thePoint.y-top);
	for(int i=0; i<mCardVector.size(); i++)
	{
		SetCard *aCard = mCardVector[i];
		if(aCard!=NULL && aCard->PtInRect(aPoint))
			return i;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SetCard* SetBoard::FindCard(CPoint &thePoint)
{
	int aPos = FindCardPos(thePoint);
	if(aPos==-1)
		return NULL;
	else
		return mCardVector[aPos];
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool SetBoard::ToggleCardState(int theId)
{
	int aPos = GetPos(theId);
	if(aPos==-1)
		return false;

	SetCard *aCard = mCardVector[aPos];
	if(aCard==NULL)
		return false;

	aCard->ToggleSelection();
	aCard->Draw(mDC);
	return true;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SetBoard::ButtonDown(CPoint &thePoint)
{
	if(!mActive)
		return -1;

	SetCard *aCard = FindCard(thePoint);
	if(aCard==NULL)
		return -1;

	ToggleCardState(aCard->mId);
	return aCard->mId;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool SetBoard::MouseOver(CPoint &thePoint)
{
	if(!mActive)
		return false;

	SetCard *aCard = FindCard(thePoint);
	if(aCard!=NULL)
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::Activate(bool isActive)
{
	mActive = isActive;
//	Redraw();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetBoard::ResetSelection(bool remove)
{
	for(int i=0; i<mCardVector.size(); i++)
	{
		SetCard *aCard = mCardVector[i];
		if(aCard!=NULL && aCard->IsSelected())
		{
			if(remove)
				RemoveCard(aCard->mId);
			else
			{
				aCard->ToggleSelection();
				aCard->Draw(mDC);
			}
		}
	}
}
