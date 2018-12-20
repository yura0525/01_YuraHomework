#pragma once
#include <windows.h>

class CMiniDump
{
private:
	static LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter;

public:
	static BOOL Begin(VOID);
	static BOOL End(VOID);

public:
	CMiniDump();
	virtual ~CMiniDump();
};

