#pragma once
#include "TBKObject.h"

const int g_iMaxLevel = 10;

enum TGameState
{
	GAME_SCENE_LOBBY,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};
class TScene
{
public:
	int			m_iSceneID;

	bool		m_bNextSceneStart;
	bool		m_bEndSceneStart;

	virtual bool Init()		{	return true;	}
	virtual bool Frame()	{	return true;	}
	virtual bool Render()	{	return true;	}
	virtual bool Release()	{	return true;	}

	virtual bool Reset()	{	return true;	}
	virtual bool IsDead()	{	return false;	}
public:
	TScene();
	virtual ~TScene();
};

