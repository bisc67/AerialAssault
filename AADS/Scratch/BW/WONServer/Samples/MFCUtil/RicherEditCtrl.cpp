#include "stdafx.h"
#include "RicherEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void RicherEditCtrl::Append(const char *theStr)
{
	// Remember selection
	CHARRANGE aSelRange;
	GetSel(aSelRange);

	// Get the scroll bar position values
	bool needScroll = false;
	int aNumLines = GetLineCount();
	int aTopLine = GetFirstVisibleLine();
	int aNumVisibleLines = 0;

	TEXTMETRIC tm;
	CDC* aDC = GetDC();
	aDC->GetTextMetrics(&tm);
	ReleaseDC(aDC);
	CRect aRect; 
	GetRect(&aRect);
	if(tm.tmAscent>0)
	{
		aNumVisibleLines = aRect.Height()/tm.tmAscent;
		if(aNumLines>=aNumVisibleLines && aNumLines-aNumVisibleLines <= aTopLine+1)
			needScroll = true;
	}

	// Store the line count
	int nBeginLineCount = GetLineCount();

	// Freeze the display of the edit control
	SetRedraw(FALSE);	

	// Append the chat text
	SetSel(GetWindowTextLength(),GetWindowTextLength());
	SetSelectionCharFormat(mCharFormat);
	ReplaceSel(theStr);
	CHARFORMAT noFormat;
	noFormat.cbSize = sizeof(CHARFORMAT);
	noFormat.dwMask = 0;
	SetSelectionCharFormat(noFormat);

	// Restore Selection
	if (aSelRange.cpMin != aSelRange.cpMax)
		SetSel(aSelRange);
	
	int aNewNumLines = GetLineCount();
	int aNewTopLine = GetFirstVisibleLine();

	if(needScroll)
		LineScroll((aNumLines-aNewTopLine)-aNumVisibleLines);
	else
		LineScroll(aTopLine - aNewTopLine);


	// Resume the display of the edit control
	SetRedraw(TRUE);
	RedrawWindow();

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void RicherEditCtrl::Append(const char *theStr, COLORREF theColor)
{
	CHARFORMAT aFormat;
	aFormat.cbSize = sizeof(aFormat);
	aFormat.dwMask = CFM_COLOR;
	aFormat.dwEffects = 0;
	aFormat.crTextColor = theColor;
	mCharFormat = aFormat;

	Append(theStr);
}
