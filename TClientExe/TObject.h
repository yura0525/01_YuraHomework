#pragma once
#include "TBasisStd.h"
#include "TImage.h"

class TObject
{
public:
	float		m_fAngle;
	TImage*		m_phColor;
	TImage*		m_phMask;
	TImage*		m_hBackScreen;

	T_STR	m_strColor;
	T_STR	m_strMask;
	RECT			m_rtCollision;

	RECT			m_rtLeftAnim[3];
	RECT			m_rtRightAnim[3];
	RECT			m_Rect;
	void			SetRect(RECT rect)
	{
		m_Rect = rect;
	}
	bool	RotateBlt();
	int		m_dwColorID;
	int		m_dwMaskID;
	void			SetBackScreen(TImage*		pBackScreen,
		int dwColorID, int dwMaskID = -1)
	{
		m_hBackScreen = pBackScreen;
		m_dwColorID = dwColorID;
		m_dwMaskID = dwMaskID;
	}
	void			SetBackScreen(TImage*		pBackScreen,
									T_STR color,
									T_STR mask)
	{
		m_hBackScreen = pBackScreen;
		m_strColor = color;
		m_strMask = mask;
	}


	float			m_fPosX;
	float			m_fPosY;
	void			MoveObject(int x, int y)
	{
		m_fPosX = x;
		m_fPosY = y;
	}
	void		MoveLeft();
	void		MoveRight();
	void		MoveUp();
	void		MoveDown();
	bool		Init();
	bool		Render();
	bool		Frame();
	bool		Release();
	bool		RenderMap();

public:
	TObject(void);
	~TObject(void);
};
