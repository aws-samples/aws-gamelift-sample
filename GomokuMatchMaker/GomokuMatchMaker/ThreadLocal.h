#pragma once

#define MAX_WORKER_THREAD	2

enum THREAD_TYPE
{
	THREAD_MAIN,
	THREAD_IO_WORKER
};


extern __declspec(thread) int LThreadType;
extern __declspec(thread) int LWorkerThreadId;



