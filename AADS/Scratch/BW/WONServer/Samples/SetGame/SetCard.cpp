#include "stdafx.h"
#include "SetCard.h"
#include "Resource.h"
#include <algorithm>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetCard::Init()
{
	mSelected = false;
	mSelectColor = RGB(0,0,255);
	mId = -1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SetCard::SetCard() 
{
	Init();
	SetAttributes(0,0,0,0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SetCard::SetCard(int theNumber, int theColor, int theShape, int theTexture)
{
	Init();
	SetAttributes(theNumber, theColor, theShape, theTexture);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetCard::SetAttributes(int theNumber, int theColor, int theShape, int theTexture)
{
	mNumber = theNumber;
	mColor = theColor;
	mShape = theShape;
	mTexture = theTexture;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool SetCard::ToggleSelection()
{
	mSelected = !mSelected;
	if(!mSelected)
		mSelectColor = RGB(0,0,255);

	return mSelected;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool SetCard::SetSelection(bool isSelected)
{
	if(isSelected==mSelected)
		return false;
	else
	{
		ToggleSelection();
		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static double rad(double ang) 
{
    return ang * 3.1415926535 / 180.0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int round(double x)
{
	return ceil(x-0.5);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetCard::DrawLine(int x1, int y1, int x2, int y2)
{
	MoveToEx(mDC,x1,y1,NULL);
	SetPixel(mDC,x1,y1,mColorRef);
	LineTo(mDC,x2,y2);
	SetPixel(mDC,x2,y2,mColorRef);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetCard::MyEllipse(int cx,int cy,int a,int b, int start, int end, int fill) 
{
	double ang = 0;
	int x, y, ox, oy,oox;

	if(end<start) 
		std::swap(start,end);

	oox = ox = round(cx+a*cos(rad(start)));
	oy = round(cy-b*sin(rad(start)));

	for(ang=start; ang<end; ang++) 
	{
		x = round(cx+a*cos(rad(ang)));
		y = round(cy-b*sin(rad(ang))); 
		DrawLine(ox,oy,x,y);
		if(fill==0 || fill>0 && x%fill==0)
		{
			DrawLine(x,y,x,cy);
			oox = x;
		}
	
		ox = x;
		oy = y;   
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetCard::MyRect(int cx,int cy,int a, int b, int fill) 
{
	int x0, x1, y0, y1, n, x,y,ox,oy,oox;
	double slope;

	x0 = cx - a;
	y0 = cy;

	x1 = cx;
	y1 = cy - b;

	slope = (double)(y1-y0)/(double)(x1-x0);
	oy = y0;
	oox =  ox = x0;

	for(n=0; n<=x1-x0; n++) 
	{
		x = n + x0;
		y = (int)(slope*(double)n) + y0;
		DrawLine(ox,oy,x,y);
		DrawLine(cx + (cx - ox),oy,cx + (cx - x),y);
		DrawLine(ox, cy + (cy - oy),x,cy + (cy - y));
		DrawLine(cx + (cx - ox),cy + (cy - oy),cx + (cx - x),cy + (cy - y));
		if(fill==0 || fill>0 && x%fill==0)
		{
			DrawLine(x,y,x,cy + (cy - y));
			DrawLine(cx + (cx - x),y,cx + (cx - x),cy + (cy - y));
			oox = x;
		}

		ox = x;
		oy = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetCard::MySquiggle(int cx, int cy,int a1, int b,int fill) 
{
	int a = 2*a1/3;   

	MyEllipse(cx-a/2.0,cy,a,b,0,180,fill);
	MyEllipse(cx+a/2.0,cy,a,b,180,360,fill);
	DrawLine(cx-a/2-a,cy,cx-a/2,cy);
	DrawLine(cx+a/2,cy,cx+a/2+a,cy);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetCard::Draw(HDC theDC)
{
	mDC = theDC;

	HANDLE anOldPen = SelectObject(theDC, GetStockObject(BLACK_PEN));
	HANDLE anOldBrush = SelectObject(theDC, GetStockObject(WHITE_BRUSH));
	Rectangle(theDC, left, top, right, bottom);
	if(mNumber==0)
		return;

	if(mSelected)
	{
		HANDLE aPen = CreatePen(PS_SOLID,0,mSelectColor);
		HANDLE anOldPen = SelectObject(theDC,aPen);
		for(int i=0; i<4; i++)
			Rectangle(theDC,left+i,top+i,right-i,bottom-i);

		SelectObject(theDC, anOldPen);
		DeleteObject(aPen);
	}

	mColorRef = RGB(0,0,0);
	switch(mColor)
	{
		case 1: mColorRef = RGB(255,0,0); break;
		case 2: mColorRef = RGB(0,150,0); break;
		case 3: mColorRef = RGB(175,0,255); break;
	}

	HANDLE aPen = CreatePen(PS_SOLID,0,mColorRef);
	SelectObject(theDC, aPen);

	int x,y,a,b,n;

	a = Width()/4;
	b = Height()/8;
	x = left+Width()/2;
	y = top;

	int fill = 0;
	switch(mTexture)
	{
		case 1: fill = -1; break;
		case 2: fill = 3; break;
		case 3: fill = 0; break;
	}


	switch(mNumber)
	{
		case 1: y += Height()/2; break;
		case 2: y += Height()/3; break;
		case 3: y += Height()/6; break;
	}
    
	for(n=0; n<mNumber; n++) 
	{
		switch(mShape) 
		{
			case 1: MyRect(x,y,a,b,fill); break;
			case 2: MyEllipse(x,y,a,b,0,360,fill); break;
			case 3: MySquiggle(x,y,a,b,fill); break;
		}
		
		y += Height()/3; 
	}


	SelectObject(theDC, anOldPen);
	SelectObject(theDC, anOldBrush);
	DeleteObject(aPen);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int SetCard::GetAttribute(int theAttribute)
{
	switch(theAttribute)
	{
		case 0:	return mNumber;
		case 1: return mColor;
		case 2: return mShape;
		case 3: return mTexture;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
void SetCard::Draw(HDC theDC)
{
	HANDLE anOldPen = SelectObject(theDC, GetStockObject(BLACK_PEN));
	HANDLE anOldBrush = SelectObject(theDC, GetStockObject(WHITE_BRUSH));
	Rectangle(theDC, left, top, right, bottom);
	if(mNumber==0)
		return;

	COLORREF aColor = RGB(0,0,0);
	switch(mColor)
	{
		case 1: aColor = RGB(255,0,0); break;
		case 2: aColor = RGB(0,255,0); break;
		case 3: aColor = RGB(255,0,255); break;
	}

	HANDLE aPen = CreatePen(PS_SOLID,0,aColor);
	SelectObject(theDC, aPen);

	HANDLE aBrush = NULL;
	switch(mTexture)
	{
		case 1: SelectObject(theDC,GetStockObject(NULL_BRUSH)); break;
		case 2: 
			SelectObject(theDC,mHatchBrush); 
			SetBkColor(theDC,RGB(255,255,255));
			SetTextColor(theDC,aColor);
			break;
		case 3: aBrush = CreateSolidBrush(aColor); SelectObject(theDC,aBrush); break;
	}


	int aXPos = left + Width()/2;
	int aYPos = top;
	switch(mNumber)
	{
		case 1: aYPos += Height()/2; break;
		case 2: aYPos += Height()/3; break;
		case 3: aYPos += Height()/6; break;
	}

	int aShapeHeight2 = Height()/8;
	int aShapeWidth2 = Width()/4;
    
	for(int n=0; n<mNumber; n++) 
	{
		switch(mShape) 
		{
		case 1: 
		{
			POINT aPoint[4] = {{aXPos-aShapeWidth2,aYPos},{aXPos,aYPos-aShapeHeight2}, {aXPos+aShapeWidth2,aYPos},{aXPos,aYPos+aShapeHeight2}};
			Polygon(theDC,aPoint,4);
			break;
		}
        case 2: 
			Ellipse(theDC,aXPos-aShapeWidth2,aYPos-aShapeHeight2,aXPos+aShapeWidth2,aYPos+aShapeHeight2);
			break;
        case 3:
			Chord(theDC,aXPos-aShapeWidth2,aYPos-aShapeHeight2,aXPos+aShapeWidth2/2,aYPos+aShapeHeight2,
				aXPos+aShapeWidth2/2,aYPos,aXPos-aShapeWidth2,aYPos);
			Chord(theDC,aXPos-aShapeWidth2/2,aYPos-aShapeHeight2,aXPos+aShapeWidth2,aYPos+aShapeHeight2,
				aXPos-aShapeWidth2/2,aYPos,aXPos+aShapeWidth2,aYPos);
//			mySquiggle(g,x,y,a,b,c,t);
			break;
		}
		
		aYPos += Height()/3; 
	}


	SelectObject(theDC, anOldPen);
	SelectObject(theDC, anOldBrush);
	DeleteObject(aBrush);
	DeleteObject(aPen);
}
*/
