#pragma once
#include <Windows.h>
class CRegistry
{
public:
	BOOL Open(HKEY rootKey, LPCTSTR subKey);
	BOOL Close(VOID);
	BOOL CreateKey(HKEY rootKey, LPCTSTR subKey);
	BOOL DeleteKey(HKEY rootKey, LPCTSTR subKey);

	BOOL SetValue(LPCTSTR valueName, LPCTSTR value);
	BOOL SetValue(LPCTSTR valueName, DWORD value);
	BOOL SetValueForMultiSz(LPCTSTR valueName, DWORD value, DWORD byteLength);

	BOOL GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength);
	BOOL GetValue(LPCTSTR valueName, LPDWORD value);

	BOOL DeleteValue(LPCTSTR valueName);
public:
	CRegistry();
	virtual ~CRegistry();
public:
	HKEY m_RootKey;
	BOOL m_IsOpened;
};

