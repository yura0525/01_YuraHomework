#include "TScene.h"

TScene::TScene()
{
	m_iSceneID = GAME_SCENE_LOBBY;
	m_bNextSceneStart = false;
	m_bEndSceneStart = false;
}


TScene::~TScene()
{
}
