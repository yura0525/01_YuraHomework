#pragma once
#include <windows.h>
struct TPoint
{
	float x;
	float y;
};

struct TSphere
{
	POINT pCenter;
	float fRadius;
};

class TCollision
{
public:
	static bool RectInRect(RECT rtDest, RECT rtSrc);
	static bool RectInPoint(RECT rtDest, POINT pt);
	static bool RectInPoint(RECT rtDest, TPoint pt);

	static bool SphereInSphere(RECT rtDest, RECT rtSrc);
	static bool SphereInPoint(RECT rtDest, POINT rtSrc);
public:
	TCollision();
	virtual ~TCollision();
};

