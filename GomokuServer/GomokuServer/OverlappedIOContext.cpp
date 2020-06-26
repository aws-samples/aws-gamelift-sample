#include "stdafx.h"
#include "Exception.h"
#include "OverlappedIOContext.h"
#include "Session.h"

OverlappedIOContext::OverlappedIOContext(SessionPtr owner, IOType ioType)
: mSessionObject(owner), mIoType(ioType)
{
	memset(&mOverlapped, 0, sizeof(OVERLAPPED));
	memset(&mWsaBuf, 0, sizeof(WSABUF));
}



void DeleteIoContext(OverlappedIOContext* context)
{
	if (nullptr == context)
		return;

	/// ObjectPool's operate delete dispatch
	switch (context->mIoType)
	{
	case IO_SEND:
		delete static_cast<OverlappedSendContext*>(context);
		break;

	case IO_RECV_ZERO:
		delete static_cast<OverlappedPreRecvContext*>(context);
		break;

	case IO_RECV:
		delete static_cast<OverlappedRecvContext*>(context);
		break;

	default:
		CRASH_ASSERT(false);
	}


}

