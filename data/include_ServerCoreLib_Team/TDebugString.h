#pragma once
#include "TServerObj.h"
class TDebugString : public TSingleton<TDebugString>, public TServerObj
{
private:
	friend class TSingleton<TDebugString>;
public:
	T_STR_VEC		m_Message;
	HWND			m_hEdit;
	HWND			m_hWnd;
	HINSTANCE		m_hInst;

public:
	void			Set(HWND hWnd, HINSTANCE hInst)
	{
		m_hWnd = hWnd;
		m_hInst = hInst;
	}

	void		DisplayText(const char* fmt, ...);
	void		GetMbToWc(TCHAR* destMsg, const char* srcMsg);
	void		GetWcToMb(char* destMsg, WCHAR* srcMsg);
	HRESULT		ConvertAnsiStringToWideCh(WCHAR* wstrDest, const CHAR* strSrc, int chDestChar);
	HRESULT		ConvertWideStringToAnsiCh(CHAR* strDest, const WCHAR* wstrSrc, int chDestChar);
	void		T_ERROR(bool bPrint = true);

public:
	bool		Init();
	bool		Frame();
	bool		Release();

public:
	TDebugString();
	virtual ~TDebugString();
};

#define I_DebugStr TDebugString::GetInstance()

