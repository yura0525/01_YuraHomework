#pragma once
#include "xObject.h"
class TBKObject : public xObject
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

