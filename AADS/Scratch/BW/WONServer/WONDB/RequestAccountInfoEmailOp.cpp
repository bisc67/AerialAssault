
#include "RequestAccountInfoEmailOp.h"

using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
RequestAccountInfoEmailOp::RequestAccountInfoEmailOp(ServerContext* theContext)
	: DBProxyOp(theContext),
	  mMsgType(14)
{
	Init();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
RequestAccountInfoEmailOp::RequestAccountInfoEmailOp(const IPAddr& theAddr)
	: DBProxyOp(theAddr),
	  mMsgType(14)
{
	Init();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
WONStatus RequestAccountInfoEmailOp::CheckResponse()
{
	// Call the base class implementation
	WONStatus result = DBProxyOp::CheckResponse();
	if (result != WS_Success)
		return result;
	
	if (mSubMessageReplyType != mMsgType+1)
		return InvalidReplyHeader();
	
	// Do extended unpack
	return WS_Success;

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void RequestAccountInfoEmailOp::RunHook()
{
	SetMessageType(DBProxyMsgType::DBProxyAccount);
	SetSubMessageType(mMsgType);

	// Pack the message data
	WriteBuffer requestData;
	requestData.AppendString(mEmail);

	// Pack the custom data (if any)
	mProfileData->PackRequestData(requestData, true);

	SetProxyRequestData(requestData.ToByteBuffer());

	DBProxyOp::RunHook();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void RequestAccountInfoEmailOp::Init()
{
	mLengthFieldSize = 4;
	mProfileData = new ProfileData;
}
