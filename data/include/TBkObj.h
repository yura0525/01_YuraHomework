#pragma once
#include "TObject.h"

class TBkObj : public TObject
{
public:
	bool Frame();
	bool Render();
	bool RenderScroll();
	virtual bool FadeOut() override;
	virtual bool FadeIn() override;
public:
	TBkObj();
	~TBkObj();
};

