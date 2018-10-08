#pragma once
#include "xObject_2D.h"
class TBKObject : public xObject_2D
{

public:
	float	m_fScrollSpeed;

public:
	bool Frame()
	{
		return true;
	}
	bool Render();
	bool RenderScroll();

	virtual bool FadeOut() override;
	virtual bool FadeIn() override;

	TBKObject();
	virtual ~TBKObject();
};

