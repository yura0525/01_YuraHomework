#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TServerStd.h"

typedef basic_string<TCHAR> T_STR;

class TDebugString_1 : public TSingleton<TDebugString_1>
{
private:
	friend class TSingleton<TDebugString_1>;

public:
	CRITICAL_SECTION	m_CS;
	std::list<T_STR>	m_Msg;
public:
	TCHAR_STRING_VECTOR	m_Message;
	HWND			m_hEdit;

public:
	void		DisplayText(char* fmt, ...);
	TCHAR*		GetMbtoWcs(const char* srcmsg);
	HRESULT		ConvertWideStringToAnsiCh(CHAR* strDestination, const WCHAR* wstrSource,
		int cchDestChar);
	HRESULT		ConvertAnsiStringToWideCh(WCHAR* wstrDestination,
		const CHAR* strSource,
		int cchDestChar);
public:
	bool		Init();
	bool		Frame();
	bool		Release();
	void		T_ERROR(bool bPrint = true);
public:
	TDebugString_1();
	~TDebugString_1();
};

#define I_DebugStr TDebugString_1::GetInstance()
