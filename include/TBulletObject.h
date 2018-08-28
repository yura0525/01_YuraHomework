#pragma once
#include "TNPCObject.h"
#include <vector>

typedef std::vector<RECT> RECT_ARRAY;

class TBulletObject : public TNPCObject
{
public:
	int		m_iCurrentSprite;
	float	m_fOffSet;

	int		m_iIndexSprite;
	std::vector<RECT_ARRAY> m_rtSpriteList;
public:
	void SetSpriteList(vector<RECT_ARRAY>& rtSpriteList);
	void SetBulletDirectionSpeed(int dirX, int dirY, float speed);
	bool Init();
	bool Frame();
public:
	TBulletObject();
	virtual ~TBulletObject();
};

