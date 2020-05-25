#include "stdafx.h"
#include "Log.h"

#include <iostream>


ConsoleLog::ConsoleLog(const char* filename)
{
	mLogFileStream.open(filename, std::ofstream::out | std::ofstream::app);
}

ConsoleLog::~ConsoleLog()
{
	mLogFileStream.close();
}

void ConsoleLog::PrintOut(bool fileOut, const char* fmt, ...)
{
	FastSpinlockGuard lock(mLock);

	char buf[512] = {};

	va_list args;
	va_start(args, fmt);
	vsprintf_s(buf, fmt, args);
	va_end(args);

	printf_s("%s", buf);

	if (fileOut)
	{
		mLogFileStream << buf;
		mLogFileStream.flush();
	}
}

std::unique_ptr<ConsoleLog> GConsoleLog(nullptr);

namespace LoggerUtil
{
	LogEvent gLogEvents[MAX_LOG_SIZE];
	__int64 gCurrentLogIndex = 0;

	void EventLogDumpOut(std::ostream& ost)
	{
		__int64 count = gCurrentLogIndex < MAX_LOG_SIZE ? gCurrentLogIndex : MAX_LOG_SIZE;

		ost << "===== Recent Sequential Event Log =====" << std::endl;

		for (int i = 1; i <= count ; ++i)
		{
			const LogEvent& log = gLogEvents[(gCurrentLogIndex - i) % MAX_LOG_SIZE];
			ost << "TID[" << log.mThreadId << "] MSG[ " << log.mMessage << " ] INFO [" << log.mAdditionalInfo << "]" << std::endl;
		}

		ost << "===== End of Event Log =====" << std::endl;
	}
}
