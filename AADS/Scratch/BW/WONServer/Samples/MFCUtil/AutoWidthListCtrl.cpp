#include "stdafx.h"
#include "AutoWidthListCtrl.h"

// CAutoWidthListCtrl::InsertColumn
// Inserts the column as CListCtrl::InsertColumn would.  If nWidth (aka LV_ITEM.cx) is 0, 
// the column width will be set to fit the column title, otherwise the width value 
// that is provided will be used
int CAutoWidthListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{ 
	// grow column vector if necessary
	if (mColumns.size() < nCol+1)
		mColumns.resize(nCol+1);
	
	// adjust column width if requested
	if (nWidth == 0)
		nWidth = GetStringWidth(lpszColumnHeading) + EXTRA_WIDTH;

	if ((nCol == 0) && (GetImageList(LVSIL_SMALL)))
		nWidth += ICON_WIDTH;


	// keep track of the column width
	mColumns[nCol] = nWidth;
	
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

// CAutoWidthListCtrl::SetColumn
// Sets the column as CListCtrl::SetColumn would.  If the new column title is wider
// than the current column, the column will be expanded to fit it.
BOOL CAutoWidthListCtrl::SetColumn(int nCol, const LV_COLUMN* pColumn)
{
	// adjust column width if needed
	int aNecessaryWidth = GetStringWidth(pColumn->pszText) + EXTRA_WIDTH;
	if ((nCol == 0) && (GetImageList(LVSIL_SMALL)))
		aNecessaryWidth += ICON_WIDTH;

	if (mColumns[nCol] < aNecessaryWidth)
		SetColumnWidth(nCol, aNecessaryWidth);
	
	return CListCtrl::SetColumn(nCol, pColumn);
}

// CAuthWidthListCtrl::SetColumnWidth
// Sets the column's width as CListCtrl::SetColumnWidth would.  Saves the new
// width in the column vector.
BOOL CAutoWidthListCtrl::SetColumnWidth(int nCol, int cx)
{
	// grow column vector if necessary
	if (mColumns.size() < nCol+1)
		mColumns.resize(nCol+1);

	mColumns[nCol] = cx;

	return CListCtrl::SetColumnWidth(nCol, cx);
}

// CAutoWidthListCtrl::InsertItem
// Inserts an item as CListCtrl::InsertItem would.  If column is not wide enough to hold
// the inserted value, it will be widened to fit.
int CAutoWidthListCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
{
	// you can just start adding stuff to a single column, headerless listctrl.  You don't
	// need to call InsertColumn if there's only one column.
	if (mColumns.size() == 0)
		InsertColumn(0, "", 0, 0, 0);
	
	// adjust column width if needed
	int aNecessaryWidth = EXTRA_WIDTH;
	if(nMask & LVIF_TEXT) 
		aNecessaryWidth += GetStringWidth(lpszItem);
	
	if (GetImageList(LVSIL_SMALL))
		aNecessaryWidth += ICON_WIDTH;

	if (mColumns[0] < aNecessaryWidth)
		SetColumnWidth(0, aNecessaryWidth);
	
	return CListCtrl::InsertItem(nMask, nItem, lpszItem, nState, nStateMask, nImage, lParam);
}

// CAutoWidthListCtrl::SetItem
// Sets a subitem as CListCtrl::SetItem would.  If column is not wide enough to hold
// the value, it will be widened to fit.
BOOL CAutoWidthListCtrl::SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam)
{
	// adjust column width if needed
	int aNecessaryWidth = EXTRA_WIDTH;
	if(nMask & LVIF_TEXT) 
		aNecessaryWidth += GetStringWidth(lpszItem); 

	if (nSubItem==0 && GetImageList(LVSIL_SMALL))
		aNecessaryWidth += ICON_WIDTH;

	if (mColumns[nSubItem] < aNecessaryWidth)
		SetColumnWidth(nSubItem, aNecessaryWidth);

	
	return CListCtrl::SetItem(nItem, nSubItem, nMask, lpszItem, nImage, nState, nStateMask, lParam);
}

// CAutoWidthListCtrl::SetImageList
// Sets an image list for the control. If the small image list is set, adjusts the width
// of first column if needed
CImageList* CAutoWidthListCtrl::SetImageList(CImageList* pImageList, int nImageList)
{
	if (nImageList == LVSIL_SMALL)
	{
		// Note: This isn't really correct.  If there aren't any rows in the list or
		//       if all of the items in the column are smaller than the title, this
		//       will add too much width.  However, it lets us avoid computing/keeping
		//       track of the widest non-title string in the column.  Really what we
		//       want is:
		//           if (mLargestNonTitleRow[0] + ICON_WIDTH > mColumns[0])
		//               SetColumnWidth(0, mLargestNonTitleRow[0] + ICON_WIDTH);
		if (mColumns.size() > 0)
			SetColumnWidth(0, mColumns[0] + ICON_WIDTH);
	}

	return CListCtrl::SetImageList(pImageList, nImageList);
}

// CAutoWidthListCtrl::DeleteAllItems
// Deletes all items as CListCtrl::DeleteAllItems would.  Sets width of all columns
// as necessary to hold column title text.
BOOL CAutoWidthListCtrl::DeleteAllItems()
{
	const int BUFFER_SIZE = 256;
	static char aBuffer[BUFFER_SIZE];
	
	for (int iCol = 0; iCol < mColumns.size(); iCol++)
	{
		LV_COLUMN aColumn;
		aColumn.mask = LVCF_TEXT;
		aColumn.pszText = aBuffer;
		aColumn.cchTextMax = BUFFER_SIZE;
		BOOL bSuccess = GetColumn(iCol, &aColumn);
		SetColumnWidth(iCol, bSuccess ? (GetStringWidth(aColumn.pszText) + EXTRA_WIDTH) : 0);
	}

	return CListCtrl::DeleteAllItems();
}
