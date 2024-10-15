
#pragma warning (disable : 4786)
#include "AdDisplay.h"
#include "AdUtils.h"

using namespace WONAPI;

// constants
const int AdDisplay_DefaultAnimationDelay = 100; // Default animation delay if set to 0 in the frame

AdDisplay::AdDisplay() :
	hLoadTrigger(NULL),
	mFuncPaintCallback(NULL),
	mDataPaintP(NULL),
	mCurrentImpageP(NULL),
	mNumScenes(0),
	mCurrentSceneNum(0),
	mTimeToNextScene(INFINITE),
	mAllowSceneSkipping(true),
	mShownCurrentScene(false),
	mAdDisplayStopped(false),
	mIterationCount(0),
	ThreadBase()
{
    hLoadTrigger = CreateEvent(NULL, false, false, NULL);
	startThread();
}

AdDisplay::~AdDisplay()
{
	stopThread();
	LOG("AdDisplay::~AdDisplay Display thread stopped");
	if(hLoadTrigger) CloseHandle(hLoadTrigger); hLoadTrigger = NULL;
	for(RAW_SET::iterator aRawIt(mRawSet.begin()); aRawIt != mRawSet.end(); aRawIt++)
		delete [] *aRawIt;
	mRawSet.clear();
	for(BITMAPH_SET::iterator anBMPIt(mBitmapHSet.begin()); anBMPIt != mBitmapHSet.end(); anBMPIt++)
		DeleteObject(*anBMPIt);
	mBitmapHSet.clear();
}

// If compiled to use single-threaded libraries,
// startThread() will fail to link.
#ifdef _MT
void AdDisplay::startThread()
{
	ThreadBase::startThread();
	ResetEvent(hLoadTrigger);
}
#endif

void AdDisplay::stopThread()
{
	ThreadBase::stopThread();
}

//////////////////////////////////////////////////////////////////////
// Setting callbacks
//////////////////////////////////////////////////////////////////////
	
void
AdDisplay::SetPaintCallback(AdApi::PaintCallback thePaintCallback, void* theDataPaintP)
{
	mFuncPaintCallback = thePaintCallback;
	mDataPaintP = theDataPaintP;
}

//////////////////////////////////////////////////////////////////////
// Display functions
//////////////////////////////////////////////////////////////////////

// Used to get the current animation frame in bitmap form
bool 
AdDisplay::GetDisplayImage(HDC hDC, HBITMAP& theBitmapHR)
{
	// Crit section
	AdApiCommon::AutoCrit aCrit(mCrit);

	theBitmapHR = NULL;
	try
	{
		if(mCurrentImage.isValid())
		{
			Magick::Blob aBlob;
			mCurrentImage.write(&aBlob, "BMP");
			const void* aPtr = aBlob.data();

			BITMAPFILEHEADER* aFileHeaderP   = (BITMAPFILEHEADER*)aPtr;
			BITMAPINFOHEADER* anInfoHeaderP  = (BITMAPINFOHEADER*)(((BYTE*)aPtr) + sizeof(BITMAPFILEHEADER));
			BYTE*             aBitmapP       = ((BYTE*)(aPtr)) + aFileHeaderP->bfOffBits;

			theBitmapHR = CreateDIBitmap(hDC, anInfoHeaderP, CBM_INIT, aBitmapP, (BITMAPINFO*)(anInfoHeaderP), DIB_RGB_COLORS);
			mBitmapHSet.insert(theBitmapHR);
			mShownCurrentScene = true;
		}
		else
		{
			LOG("AdDisplay::GetDisplayImage Error: Image is invalid");
		}
	}
	catch(...)
	{
		if(theBitmapHR)
		{
			mBitmapHSet.erase(theBitmapHR);
			DeleteObject(theBitmapHR);
		}
		LOG("AdDisplay::GetDisplayImage Error: Exception caught unable to get image");
		return false;
	}
	return (theBitmapHR != NULL);
}

// Used to get the current animation frame in raw form
bool 
AdDisplay::GetDisplayImage(BYTE* &theRawPR, int& theWidthR, int& theHeightR)
{
	// Crit section
	AdApiCommon::AutoCrit aCrit(mCrit);

	theRawPR = NULL;
	try
	{
		if(mCurrentImage.isValid())
		{
			// Interpret ImageMajicks raw data
			MagickLib::Image* aCurrentImageP = mCurrentImage.image();
			theWidthR = mCurrentImage.size().width();
			theHeightR = mCurrentImage.size().height();
			theRawPR = new BYTE[theWidthR * theHeightR * 3];
			MagickLib::GetPixels(aCurrentImageP, 0, 0, theWidthR, theHeightR, "RGB", MagickLib::CharPixel, theRawPR);
			mRawSet.insert(theRawPR);
			mShownCurrentScene = true;
		}
	}
	catch(...)
	{
		if(theRawPR)
		{
			mRawSet.erase(theRawPR);
			delete [] theRawPR;
			theRawPR = NULL;
		}
		return false;
	}
	return (theRawPR != NULL);
}

void 
AdDisplay::ReleaseDisplayImage(HBITMAP theBitmapH)
{
	if(mBitmapHSet.erase(theBitmapH) > 0) 
	{
		DeleteObject(theBitmapH);
	}
}

void 
AdDisplay::ReleaseDisplayImage(BYTE* theRawP)
{
	if(mRawSet.erase(theRawP) > 0)
		delete [] theRawP;
}

int
AdDisplay::threadProcess()
{
	HANDLE aWaitArray[2] = {getStopEvent(), getLoadEvent()};
	while(true)
	{
		DWORD aResult = WaitForMultipleObjects(2, aWaitArray, false, mAdDisplayStopped ? INFINITE : mTimeToNextScene);
		if(aResult == WAIT_FAILED)
			return 1;

		switch(aResult - WAIT_OBJECT_0)
		{
		// Stop requested
		case 0:
			LOG("AdDisplay::threadProcess Stop thread requested.  Thread ID: " << getId());
			return 0;
			break;
		// Load Image
		case 1:
			if(mFuncPaintCallback)
				mFuncPaintCallback(mDataPaintP);
			break;
		// Timer
		default:
			if (!mAdDisplayStopped)
			{
				AdvanceScene();
				if(mFuncPaintCallback)
					mFuncPaintCallback(mDataPaintP);
			}
			break;
		}
	}
}

bool 
AdDisplay::LoadNewImage(const std::string& theFileName)
{
	// Crit section
	AdApiCommon::AutoCrit aCrit(mCrit);

	bool isNewLoad(false);
	try
	{
		Magick::Image anImage(theFileName);
		mBaseImage = anImage;
		mCurrentImage = mBaseImage;
		mCurrentImpageP = mBaseImage.image();
		mNumScenes = MagickLib::GetNumberScenes(mCurrentImpageP);
		mCurrentSceneNum = 0;
		isNewLoad = true;
	}
	catch(...)
	{
	}
	if(mCurrentImpageP && mCurrentImage.isValid() && mNumScenes > 1)
		mTimeToNextScene = mCurrentImpageP->delay ? mCurrentImpageP->delay * 10 : AdDisplay_DefaultAnimationDelay;
	else
		mTimeToNextScene = INFINITE;
	// Set the number of times to go through loop
	mIterationCount = 0;
	SetEvent(hLoadTrigger);		
	return isNewLoad;
}

// Stops and starts the displaying of ads.
void 
AdDisplay::StopAdDisplay(bool thePause)
{
	mAdDisplayStopped = thePause;
	if (!thePause)
	{
		SetEvent(hLoadTrigger); // ok time to wake up and display ads again
	}
}

bool 
AdDisplay::AdvanceScene(void)
{
	// Crit section
	AdApiCommon::AutoCrit aCrit(mCrit);

	if(mShownCurrentScene == false && mAllowSceneSkipping == false)
	{
		mTimeToNextScene = 100;
		return false;
	}

	mShownCurrentScene = false;
	if(mCurrentSceneNum > 0)
	{
		try
		{
			// We have exceeded the number of scenes, time to start over from the beginning or stop if max number of iterations reached
			if(mCurrentSceneNum == mNumScenes)
			{
				mIterationCount++;
				if (mBaseImage.image()->iterations == 0 || mIterationCount < mBaseImage.image()->iterations )
				{
					// Start over from the beginning
					mCurrentImpageP = mBaseImage.image();
					mCurrentImage = mBaseImage;
					mCurrentSceneNum = 1;
				}
			}
			// Composite the new image over the last image
			else
			{
				mCurrentImpageP = MagickLib::GetNextImage(mCurrentImpageP);
				mCurrentSceneNum++;
				Magick::Image aTempImage(MagickLib::CloneImage(mCurrentImpageP, mCurrentImpageP->columns, mCurrentImpageP->rows, true), mBaseImage.options());
				// Don't ask me why, but this fixes an odd display problem with some GIFs
				if(mCurrentImpageP->matte == 0)
					mCurrentImage.composite(aTempImage, mCurrentImpageP->page_info.x, mCurrentImpageP->page_info.y, Magick::ReplaceCompositeOp);
				else
					mCurrentImage.composite(aTempImage, mCurrentImpageP->page_info.x, mCurrentImpageP->page_info.y, Magick::OverCompositeOp);
			}
		}
		catch(...)
		{
		}
	}
	// First image is displayed as is
	else
		mCurrentSceneNum++;

	if (mBaseImage.image()->iterations == 0 || mIterationCount < mBaseImage.image()->iterations )
	{
		mTimeToNextScene = mTimeToNextScene = mCurrentImpageP->delay ? mCurrentImpageP->delay * 10 : AdDisplay_DefaultAnimationDelay;
	}
	else // we have animated as long as we are supposed to
	{
		mTimeToNextScene = INFINITE;
	}
	return true;
}
