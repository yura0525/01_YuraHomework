#pragma once
#include "TServerObj.h"

class TDebugString : public TSingleton<TDebugString>, public TServerObj
{
private:
	friend class TSingleton<TDebugString>;
public:
	HWND		m_hList;
public:
	void		Print(const char* fmt, ...);
public:
	bool		Init();
	bool		Release();
	WCHAR*		GetMbToWcs(const char* strMsg);
	char*		GetwcsToMbs(const WCHAR* strMsg);
	HRESULT		ConvertWideStringToAnsiCh(char* msg, const WCHAR* strMsg, int iRen);
	HRESULT		ConvertAnsiStringToWideCh(WCHAR* msg, const char* strMsg, int iRen);
	void		T_ERROR(bool bPrint = false);

public:
	TDebugString();
	virtual ~TDebugString();
};

#define I_Debug TDebugString::GetInstance()