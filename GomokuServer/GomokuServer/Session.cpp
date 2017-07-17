#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "Log.h"

#include "OverlappedIOContext.h"
#include "Session.h"
#include "IocpManager.h"



Session::Session(SOCKET sock, size_t sendBufSize, size_t recvBufSize) 
: mSocket(sock), mSendBuffer(sendBufSize), mRecvBuffer(recvBufSize), mConnected(0), mSendPendingCount(0)
{
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
}

bool Session::OnConnect()
{
	CRASH_ASSERT(LThreadType == THREAD_MAIN);

	int addrlen = sizeof(mClientAddr);
	getpeername(mSocket, (SOCKADDR*)&mClientAddr, &addrlen);

	/// make socket non-blocking
	u_long arg = 1;
	ioctlsocket(mSocket, FIONBIO, &arg);

	/// turn off nagle
	int opt = 1;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int));

	opt = 0;
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
	{
		GConsoleLog->PrintOut(true, "[DEBUG] SO_RCVBUF change error: %d\n", GetLastError());
		return false;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)mSocket, GIocpManager->GetComletionPort(), (ULONG_PTR)this, 0);
	if (handle != GIocpManager->GetComletionPort())
	{
		GConsoleLog->PrintOut(true, "[DEBUG] CreateIoCompletionPort error: %d\n", GetLastError());
		return false;
	}

	InterlockedExchange(&mConnected, 1);

	GConsoleLog->PrintOut(true, "[DEBUG] Client Connected: IP=%s, PORT=%d\n", inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port));

	return PreRecv();
}


void Session::Disconnect(DisconnectReason dr)
{
	/// already disconnected or disconnecting...
	if (0 == InterlockedExchange(&mConnected, 0))
		return;

	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

	/// no TCP TIME_WAIT
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)))
	{
		GConsoleLog->PrintOut(true, "[DEBUG] setsockopt linger option error: %d\n", GetLastError());
	}

	GConsoleLog->PrintOut(true, "[DEBUG] Client Disconnected: Reason=%d IP=%s, PORT=%d \n", dr, inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port));

	OnDisconnect(dr);

	closesocket(mSocket);

}


bool Session::PreRecv()
{
	if (!IsConnected())
		return false;

	OverlappedPreRecvContext* recvContext = new OverlappedPreRecvContext(shared_from_this());

	DWORD recvbytes = 0;
	DWORD flags = 0;
	recvContext->mWsaBuf.len = 0;
	recvContext->mWsaBuf.buf = nullptr;

	/// start async recv
	if (SOCKET_ERROR == WSARecv(mSocket, &recvContext->mWsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)recvContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(recvContext);
			GConsoleLog->PrintOut(true, "Session::PreRecv Error : %d\n", GetLastError());
			return false;
		}
	}

	return true;
}

bool Session::PostRecv()
{
	if (!IsConnected())
		return false;

	if (0 == mRecvBuffer.GetFreeSpaceSize())
		return false;

	OverlappedRecvContext* recvContext = new OverlappedRecvContext(shared_from_this());

	DWORD recvbytes = 0;
	DWORD flags = 0;
	recvContext->mWsaBuf.len = (ULONG)mRecvBuffer.GetFreeSpaceSize();
	recvContext->mWsaBuf.buf = mRecvBuffer.GetBuffer();


	/// start real recv
	if (SOCKET_ERROR == WSARecv(mSocket, &recvContext->mWsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)recvContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(recvContext);
			GConsoleLog->PrintOut(true, "Session::PostRecv Error : %d\n", GetLastError());
			return false;
		}

	}

	return true;
}


bool Session::PostSend(const char* data, size_t len)
{
	if (!IsConnected())
		return false;

	FastSpinlockGuard criticalSection(mSendBufferLock);

	if (false == mSendBuffer.Write(data, len))
		return false;

	if (mSendPendingCount > 0) ///< if not completed a previous completion
		return true;

	return FlushSend();
}


bool Session::FlushSend()
{
	OverlappedSendContext* sendContext = new OverlappedSendContext(shared_from_this());

	DWORD sendbytes = 0;
	DWORD flags = 0;
	sendContext->mWsaBuf.len = (ULONG)mSendBuffer.GetContiguiousBytes();
	sendContext->mWsaBuf.buf = mSendBuffer.GetBufferStart();

	/// start async send
	if (SOCKET_ERROR == WSASend(mSocket, &sendContext->mWsaBuf, 1, &sendbytes, flags, (LPWSAOVERLAPPED)sendContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(sendContext);
			GConsoleLog->PrintOut(true, "Session::FlushSend Error : %d\n", GetLastError());

			Disconnect(DR_SENDFLUSH_ERROR);
			return false;
		}

	}

	mSendPendingCount++;

	CRASH_ASSERT(mSendPendingCount == 1);

	return true ;
}

void Session::SendCompletion(DWORD transferred)
{
	FastSpinlockGuard criticalSection(mSendBufferLock);

	CRASH_ASSERT(mSendPendingCount > 0);

	mSendBuffer.Remove(transferred);

	mSendPendingCount--;

	CRASH_ASSERT(mSendPendingCount == 0);

	/// if data remain to send 
	if (mSendBuffer.GetContiguiousBytes() > 0)
	{
		FlushSend();
	}
}

void Session::RecvCompletion(DWORD transferred)
{
	mRecvBuffer.Commit(transferred);
	OnRead(transferred);
}




void Session::EchoBack()
{
	size_t len = mRecvBuffer.GetContiguiousBytes();

	if (len == 0)
		return;

	if (false == PostSend(mRecvBuffer.GetBufferStart(), len))
		return;

	mRecvBuffer.Remove(len);

}

