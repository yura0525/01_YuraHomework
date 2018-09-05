#pragma once
#include "TObject.h"
class TBKObject : public TObject
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

