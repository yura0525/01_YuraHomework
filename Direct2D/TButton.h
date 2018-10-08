#pragma once
#include "xObject.h"
enum Btn_State
{
	Btn_Normal,
	Btn_Click,
	Btn_Hover,
	Btn_Disable,
	Btn_Count,
};
class TButton : public xObject
{
public:
	//TBitmap * m_pColorBitmap[Btn_Count];
	//TBitmap * m_pMaskBitmap[Btn_Count];
	//TBitmap * m_pCurrentBitmap;
public:
	bool Frame();
	bool Render();

	bool LoadUI(const TCHAR* pszColor, const TCHAR* pszMask, int iState);
public:
	TButton();
	virtual ~TButton();
};

