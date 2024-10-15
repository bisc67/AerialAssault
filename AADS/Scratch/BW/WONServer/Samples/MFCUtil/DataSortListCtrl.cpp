// DataSortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DataSortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DataSortListCtrl
DataSortListCtrl::DataSortListCtrl()
{
	mLess = true;

	mSortCol = 0;
	mSortFunc = NULL;
	mTextFunc = NULL;
	mImageFunc = NULL;
	mParam = NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DataSortListCtrl::~DataSortListCtrl()
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void DataSortListCtrl::UpdateItem(void *theData, int thePos, bool sort)
{
	int anImage = -1;
	if(mImageFunc!=NULL)
		anImage = mImageFunc(theData,mParam);

	CString aStr;

	if(mTextFunc!=NULL)
		aStr = mTextFunc(theData,0,mParam);

	LV_ITEM aItem;
	aItem.iItem = thePos;
	aItem.iSubItem = 0;
	aItem.iImage = anImage;
	aItem.pszText = (char*)(const char*)aStr;
	aItem.mask = LVIF_TEXT;
	if(anImage!=-1)
		aItem.mask |= LVIF_IMAGE; 

	SetItem(&aItem);

	if(mTextFunc==NULL)
		return;

	int aNumItems = GetHeaderCtrl()->GetItemCount();
	for(int i=1; i<aNumItems; i++)
	{
		aStr = mTextFunc(theData,i,mParam);
		SetItemText(thePos,i,(char*)(const char*)aStr);
	}

	if(sort)
		Sort();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int DataSortListCtrl::GetIndex(void *theData)
{
	LPARAM aData = (LPARAM)theData;
	for(int i=0; i<GetItemCount(); i++)
	{
		if(aData==GetItemData(i))
			return i;
	}

	return -1;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool DataSortListCtrl::AddItem(void *theData, bool sort)
{
	if(GetIndex(theData)!=-1)
		return false;

	LV_ITEM aItem;
	aItem.iItem = 0;
	aItem.iSubItem = 0;
	aItem.mask =  LVIF_PARAM;
	aItem.lParam = (DWORD)theData;
	int aPos = InsertItem(&aItem);
	
	UpdateItem(theData, aPos, sort);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool DataSortListCtrl::UpdateItem(void *theData, bool sort)
{
	int aPos = GetIndex(theData);
	if(aPos==-1)
		return false;

	UpdateItem(theData, aPos, sort);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool DataSortListCtrl::RemoveItem(void *theData)
{
	int aPos = GetIndex(theData);
	if(aPos==-1)
		return false;

	DeleteItem(aPos);
	return true;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int DataSortListCtrl::StaticSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM that)
{
	DataSortListCtrl *aCtrl = (DataSortListCtrl*)that;
	int aSortCol = aCtrl->mSortCol;
	bool aLess = aCtrl->mLess;
	SortFunc aFunc = aCtrl->mSortFunc;

	int aRet = aFunc((void*)lParam1, (void*)lParam2, aSortCol, aCtrl->mParam);
	if(!aLess)
		aRet*=-1;

	return aRet;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void DataSortListCtrl::Sort()
{
	if(mSortFunc==NULL)
		return;

	SortItems(StaticSortFunc, (LPARAM)this);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(DataSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(DataSortListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DataSortListCtrl message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void DataSortListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int aCol = pNMListView->iSubItem;
	if(aCol==mSortCol)
		mLess = !mLess;
	else
		mLess = true;

	mSortCol = aCol;
	Sort();

	*pResult = 0;
}
