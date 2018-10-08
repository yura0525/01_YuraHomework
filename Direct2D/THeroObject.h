#pragma once
#include "xObject_2D.h"

class THeroObject : public xObject_2D
{
public:
	bool	bFadeStart;
public:
	bool Frame();
	virtual bool Render() override;
	virtual bool FadeOut() override;
	virtual bool FadeIn() override;

public:
	THeroObject();
	virtual ~THeroObject();
};

