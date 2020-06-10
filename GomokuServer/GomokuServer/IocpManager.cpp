#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "Log.h"
#include "IocpManager.h"
#include "IOThread.h"
#include "PlayerSession.h"


std::unique_ptr<IocpManager> GIocpManager(nullptr);


IocpManager::IocpManager() : mCompletionPort(NULL), mListenSocket(NULL)
{
	memset(mIoWorkerThread, 0, sizeof(mIoWorkerThread));
}


IocpManager::~IocpManager()
{
}

bool IocpManager::Initialize(int& listenPort)
{
	/// winsock initializing
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	/// Create I/O Completion Port
	mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mCompletionPort == NULL)
		return false;

	/// create TCP socket
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket == INVALID_SOCKET)
		return false;

	int opt = 1;
	setsockopt(mListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

	/// bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(static_cast<u_short>(listenPort));
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(mListenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
		return false;

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	int len = sizeof(serveraddr);
	if (SOCKET_ERROR == getsockname(mListenSocket, (SOCKADDR*)&serveraddr, &len))
		return false;

	listenPort = ntohs(serveraddr.sin_port);

	return true;
}


bool IocpManager::StartIoThreads()
{
	/// create I/O Thread
	for (int i = 0; i < MAX_WORKER_THREAD; ++i)
	{
		DWORD dwThreadId;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, IoWorkerThread, reinterpret_cast<LPVOID>(i), CREATE_SUSPENDED, (unsigned int*)&dwThreadId);
		if (hThread == NULL)
			return false;

		mIoWorkerThread[i] = new IOThread(hThread, mCompletionPort);
	}

	/// start!
	for (int i = 0; i < MAX_WORKER_THREAD; ++i)
	{
		ResumeThread(mIoWorkerThread[i]->GetHandle());
	}

	return true;
}


void IocpManager::StartAccept()
{
	CRASH_ASSERT(LThreadType == THREAD_MAIN);

	/// listen
	if (SOCKET_ERROR == listen(mListenSocket, SOMAXCONN))
	{
		GConsoleLog->PrintOut(true, "[DEBUG] listen error\n");
		return;
	}
		
	/// accept loop
	while (true)
	{
		SOCKET acceptedSock = accept(mListenSocket, NULL, NULL);
		if (acceptedSock == INVALID_SOCKET)
		{
			GConsoleLog->PrintOut(true, "accept: invalid socket\n");
			continue;
		}

		/// accepting a new player session 
		auto newSession = std::make_shared<PlayerSession>(acceptedSock);
		
		/// connection establishing and then issuing recv
		if (false == newSession->OnConnect())
		{
			newSession->Disconnect(DR_ONCONNECT_ERROR);
		}
	}


}


void IocpManager::Finalize()
{
	for (int i = 0; i < MAX_WORKER_THREAD; ++i)
	{
		CloseHandle(mIoWorkerThread[i]->GetHandle());
	}

	CloseHandle(mCompletionPort);

	/// winsock finalizing
	WSACleanup();

}

unsigned int WINAPI IocpManager::IoWorkerThread(LPVOID lpParam)
{
	LThreadType = THREAD_IO_WORKER;
	LWorkerThreadId = reinterpret_cast<int>(lpParam);
	
	return GIocpManager->mIoWorkerThread[LWorkerThreadId]->Run();
}

