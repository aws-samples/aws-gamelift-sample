#pragma once

#include "ObjectPool.h"

enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO
};

enum DisconnectReason
{
	DR_NONE,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_IO_REQUEST_ERROR,
	DR_COMPLETION_ERROR,
	DR_SENDFLUSH_ERROR,
	DR_SENDBUFFER_ERROR,
	DR_UNAUTH,
	DR_LOGOUT
};



class Session;
typedef std::shared_ptr<Session> SessionPtr;

struct OverlappedIOContext
{
	OverlappedIOContext(SessionPtr owner, IOType ioType);

	OVERLAPPED	mOverlapped;
	SessionPtr	mSessionObject;
	IOType		mIoType;
	WSABUF		mWsaBuf;

};


struct OverlappedSendContext : public OverlappedIOContext, public ObjectPool<OverlappedSendContext>
{
	OverlappedSendContext(SessionPtr owner) : OverlappedIOContext(owner, IO_SEND)
	{
	}
};

struct OverlappedRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedRecvContext>
{
	OverlappedRecvContext(SessionPtr owner) : OverlappedIOContext(owner, IO_RECV)
	{
	}
};

struct OverlappedPreRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedPreRecvContext>
{
	OverlappedPreRecvContext(SessionPtr owner) : OverlappedIOContext(owner, IO_RECV_ZERO)
	{
	}
};


void DeleteIoContext(OverlappedIOContext* context);

