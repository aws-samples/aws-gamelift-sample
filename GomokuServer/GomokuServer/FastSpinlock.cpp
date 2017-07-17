#include "stdafx.h"
#include "FastSpinlock.h"
#include "ThreadLocal.h"

FastSpinlock::FastSpinlock() : mLockFlag(0)
{
}


FastSpinlock::~FastSpinlock()
{
}


void FastSpinlock::EnterWriteLock()
{
	while (true)
	{
		/// wait a writelock
		while (mLockFlag & LF_WRITE_MASK)
			YieldProcessor();

		if ((InterlockedAdd(&mLockFlag, LF_WRITE_FLAG) & LF_WRITE_MASK) == LF_WRITE_FLAG)
		{
			/// wait a readlock
			while (mLockFlag & LF_READ_MASK)
				YieldProcessor();

			return;
		}

		InterlockedAdd(&mLockFlag, -LF_WRITE_FLAG);
	}

}

void FastSpinlock::LeaveWriteLock()
{
	InterlockedAdd(&mLockFlag, -LF_WRITE_FLAG);
}

void FastSpinlock::EnterReadLock()
{
	while (true)
	{
		/// wait a writelock
		while (mLockFlag & LF_WRITE_MASK)
			YieldProcessor();

		if ((InterlockedIncrement(&mLockFlag) & LF_WRITE_MASK) == 0)
			return;

		InterlockedDecrement(&mLockFlag);
	}
}

void FastSpinlock::LeaveReadLock()
{
	InterlockedDecrement(&mLockFlag);
}