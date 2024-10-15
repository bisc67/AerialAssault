#include "RoutingDeleteDataObjectOp.h"
using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
RoutingDeleteDataObjectOp::RoutingDeleteDataObjectOp(RoutingConnection *theConnection) : RoutingOp(theConnection)
{
	mLinkId = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void RoutingDeleteDataObjectOp::SendRequest()
{
	InitSendMsg(RoutingDeleteDataObjectRequest);
	mSendMsg.AppendShort(mLinkId);
	mSendMsg.AppendString(mDataType,1);
	mSendMsg.AppendWString(mDataName,1);
	SendMsg();
	AddOp();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
WONStatus RoutingDeleteDataObjectOp::HandleReply(unsigned char theMsgType, ReadBuffer &theMsg)
{
	if(theMsgType!=RoutingStatusReply)
		return WS_RoutingOp_DontWantReply;

	WONStatus aStatus = (WONStatus)theMsg.ReadShort();
	return aStatus;
}
