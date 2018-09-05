#pragma once
#include "TObject.h"

class THeroObject : public TObject
{
public:
	bool bFadeStart;
public:
	bool Frame();
	virtual bool Render() override;
	virtual bool FadeOut() override;
	virtual bool FadeIn() override;

public:
	void ProcessDamage(int damage);
	void SetMAXHP(int hp);
public:
	THeroObject();
	virtual ~THeroObject();
};

