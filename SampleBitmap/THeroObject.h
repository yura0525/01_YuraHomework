#pragma once
#include "TPCObject.h"

class THeroObject : public TPCObject
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

