#include "TScene.h"


bool TScene::SetNPCCount(int iNPC)
{
	if (g_iMaxNPCCount <= iNPC)
	{
		m_iMaxNPCCount = g_iMaxNPCCount;
		return true;
	}

	m_iMaxNPCCount = iNPC;
	return true;
}

TScene::TScene()
{
	m_iSceneID = GAME_SCENE_LOBBY;
	m_bNextSceneStart = false;
	m_iMaxNPCCount = g_iLineMaxNPCCount;
	m_bEndSceneStart = false;
}


TScene::~TScene()
{
}
