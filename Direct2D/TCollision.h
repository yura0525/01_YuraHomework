#pragma once
#include <Windows.h>

struct TSphere
{
	POINT pCenter;
	float fRadius;
};

class TCollision
{
public:
	static bool RectInRect(RECT rtDest, RECT rtSrc);
	static bool RectInPoint(RECT rtDest, POINT ptSrc);
	static bool SphereInSphere(RECT rtDest, RECT rtSrc);
	static bool SphereInPoint(RECT rtDest, POINT ptSrc);
	static bool HeroSpereInNPCSphere(RECT rtDest, RECT rtSrc);

public:
	TCollision();
	virtual ~TCollision();
};

