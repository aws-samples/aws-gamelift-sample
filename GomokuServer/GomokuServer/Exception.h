#pragma once

LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* exceptionInfo) ;

inline void CRASH_ASSERT(bool isOk)
{
	if ( isOk )
		return ;

	int* crashVal = 0 ;
	*crashVal = 0xDEADBEEF ;
}
