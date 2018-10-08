#include "TCollision.h"
#include <math.h>


bool TCollision::RectInRect(RECT rtDest, RECT rtSrc)
{
	POINT cCenterDest, cCenterSrc;
	cCenterDest.x = (rtDest.left + rtDest.right) / 2;
	cCenterDest.y = (rtDest.top + rtDest.bottom) / 2;

	cCenterSrc.x = (rtSrc.left + rtSrc.right) / 2;
	cCenterSrc.y = (rtSrc.top + rtSrc.bottom) / 2;

	POINT radius;
	radius.x = abs(cCenterDest.x - cCenterSrc.x);
	radius.y = abs(cCenterDest.y - cCenterSrc.y);

	POINT destRadius, srcRadius;
	destRadius.x = (rtDest.right - rtDest.left) / 2;
	destRadius.y = (rtDest.bottom - rtDest.top) / 2;

	srcRadius.x = (rtSrc.right - rtSrc.left) / 2;
	srcRadius.y = (rtSrc.bottom - rtSrc.top) / 2;

	if (radius.x <= (destRadius.x + srcRadius.x) &&
		radius.y <= (destRadius.y + srcRadius.y))
		return true;

	return false;
}
bool TCollision::RectInPoint(RECT rtDest, POINT ptSrc)
{
	if ((rtDest.left <= ptSrc.x) && (ptSrc.x <= rtDest.right))
	{
		if ((rtDest.top <= ptSrc.y) && (ptSrc.y <= rtDest.bottom))
			return true;
	}

	return false;
}
bool TCollision::SphereInSphere(RECT rtDest, RECT rtSrc)
{
	TSphere sphereDest;
	sphereDest.pCenter.x = (rtDest.left + rtDest.right) / 2;
	sphereDest.pCenter.y = (rtDest.top + rtDest.bottom) / 2;
	
	long dwX = (rtDest.right - rtDest.left) / 2;
	long dwY = (rtDest.bottom - rtDest.top) / 2;
	sphereDest.fRadius = (dwX < dwY) ? dwY : dwX;

	TSphere sphereSrc;
	sphereSrc.pCenter.x = (rtSrc.left + rtSrc.right) / 2;
	sphereSrc.pCenter.y = (rtSrc.top + rtSrc.bottom) / 2;

	dwX = (rtSrc.right - rtSrc.left) / 2;
	dwY = (rtSrc.bottom - rtSrc.top) / 2;
	sphereSrc.fRadius = (dwX < dwY) ? dwY : dwX;

	float fDistance = sqrt((sphereDest.pCenter.x - sphereSrc.pCenter.x) * (sphereDest.pCenter.x - sphereSrc.pCenter.x) +
		(sphereDest.pCenter.y - sphereSrc.pCenter.y) * (sphereDest.pCenter.y - sphereSrc.pCenter.y));

	if (fDistance < (sphereDest.fRadius + sphereSrc.fRadius))
		return true;

	return false;
}
bool TCollision::SphereInPoint(RECT rtDest, POINT ptSrc)
{
	TSphere sphereDest;
	sphereDest.pCenter.x = (rtDest.left + rtDest.right) / 2;
	sphereDest.pCenter.y = (rtDest.top + rtDest.bottom) / 2;

	long dwX = (rtDest.right - rtDest.left) / 2;
	long dwY = (rtDest.bottom - rtDest.top) / 2;
	sphereDest.fRadius = (dwX < dwY) ? dwY : dwX;

	float fDistance = sqrt((sphereDest.pCenter.x - ptSrc.x) * (sphereDest.pCenter.x - ptSrc.x) +
		(sphereDest.pCenter.y - ptSrc.y) * (sphereDest.pCenter.y - ptSrc.y));

	if (fDistance < sphereDest.fRadius)
		return true;

	return false;
}
TCollision::TCollision()
{
}


TCollision::~TCollision()
{
}
