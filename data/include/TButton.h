#pragma once
#include "TObject.h"
enum Btn_State
{
	Btn_Normal,
	Btn_Click,
	Btn_Active,
	Btn_Hover,
	Btn_Count,
};

class TButton : public TObject
{
public:
	TBitmap* m_hColorBitmap[Btn_Count];
	TBitmap* m_hMaskBitmap[Btn_Count];
	TBitmap* m_hBtnCurrentBitmap;
public:
	bool Frame();
	bool Render();
	bool LoadUI(const TCHAR* pszColor, const TCHAR* pszMask, int iState);
	TButton();
	virtual ~TButton();
};

