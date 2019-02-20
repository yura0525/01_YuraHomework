#pragma once
#include "TServerObj_1.h"
class TDebugString_2 : public TSingleton<TDebugString_2>, public TServerObj_1
{
private:
	friend class TSingleton<TDebugString_2>;
public:
	T_STR_VEC		m_Message;
	HWND			m_hEdit;
	HWND			m_hWnd;
	HINSTANCE		m_hInst;
	void		Set(HWND hWnd, HINSTANCE hInst)
	{
		m_hWnd = hWnd;
		m_hInst = hInst;
	}
	void		DisplayText(char* fmt, ...);
	TCHAR*		GetMbtoWcs(const char* srcmsg);
	char*		GetWcstoMbs(WCHAR* srcmsg);
	HRESULT		ConvertAnsiStringToWideCh(WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar);
	HRESULT		ConvertWideStringToAnsiCh(CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar);
public:
	bool		Init();
	bool		Frame();
	bool		Release();

	void		T_ERROR(bool bPrint = true);

public:
	TDebugString_2();
	~TDebugString_2();
};

#define I_DebugStr TDebugString_2::GetInstance()