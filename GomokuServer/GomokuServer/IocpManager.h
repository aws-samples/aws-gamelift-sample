#pragma once
#include "ThreadLocal.h"


class Session;
class IOThread;

struct OverlappedSendContext;
struct OverlappedPreRecvContext;
struct OverlappedRecvContext;


class IocpManager
{
public:
	IocpManager();
	~IocpManager();

	bool Initialize(int& listenPort);
	void Finalize();

	bool StartIoThreads();
	void StartAccept();


	HANDLE GetComletionPort()	{ return mCompletionPort; }

	SOCKET* GetListenSocket()  { return &mListenSocket;  }

private:

	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);

private:

	HANDLE	mCompletionPort;
	SOCKET	mListenSocket;

	IOThread* mIoWorkerThread[MAX_WORKER_THREAD];

};

extern std::unique_ptr<IocpManager> GIocpManager;

