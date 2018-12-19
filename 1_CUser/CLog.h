#pragma once
#include <Windows.h>

class CLog
{
public:
	static BOOL WriteLog(LPTSTR data, ...);
public:
	CLog();
	virtual ~CLog();
};

