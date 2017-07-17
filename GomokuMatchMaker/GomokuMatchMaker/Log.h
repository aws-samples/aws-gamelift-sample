#pragma once

#include <fstream>
#include "Exception.h"
#include "ThreadLocal.h"
#include "FastSpinlock.h"

class ConsoleLog
{
public:
	ConsoleLog(const char* filename);
	~ConsoleLog();

	void PrintOut(bool fileOut, const char* fmt, ...);

private:
	FastSpinlock mLock;
	std::ofstream mLogFileStream;
};

extern std::unique_ptr<ConsoleLog> GConsoleLog;

namespace LoggerUtil
{

	struct LogEvent
	{
		int mThreadId = -1;
		int	mAdditionalInfo = 0;
		const char* mMessage = nullptr; 
	};

	#define MAX_LOG_SIZE  65536   ///< Must be a power of 2

	extern LogEvent gLogEvents[MAX_LOG_SIZE];
	extern __int64 gCurrentLogIndex;

	inline void EventLog(const char* msg, int info)
	{
		__int64 index = _InterlockedIncrement64(&gCurrentLogIndex) - 1;
		LogEvent& event = gLogEvents[index & (MAX_LOG_SIZE - 1)];
		event.mThreadId = LWorkerThreadId;
		event.mMessage = msg;
		event.mAdditionalInfo = info;
	}

	void EventLogDumpOut(std::ostream& ost = std::cout);

}

#define EVENT_LOG(x, info) LoggerUtil::EventLog(x, info)

