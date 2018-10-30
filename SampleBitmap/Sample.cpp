#include "TCore.h"
#include "TBitmap.h"
#include "TCollision.h"
#include "TScene.h"
#include "TLobbyScene.h"
#include "TGameScene.h"
#include "TEndScene.h"
#include <math.h>

using namespace std;

//TCoreLib의 빌드후 이벤트
//폴더내의 모든 헤더파일을 ../../include 경로로 복사한다
//폴더내의 모든 .lib 파일을 ../../lib 경로로 복사한다.
//copy "*.h" "../../include"
//copy "*.lib" "../../lib"
const float g_NPC_REGENTIME = 4.0f;
const float g_TOTAL_GAMETIME = 300.0f;
class Sample : public TCore
{
public:
	shared_ptr<TScene>	m_pLobbyScene;
	shared_ptr<TScene>	m_pGameScene;
	shared_ptr<TScene>	m_pEndScene;

	TScene*				m_pCurrentScene;
	int					m_iLevel;
public:
	bool Init()
	{
		m_iLevel = 1;
		m_pLobbyScene = make_shared<TLobbyScene>();
		m_pGameScene = make_shared<TGameScene>();
		m_pEndScene = make_shared<TEndScene>();

		m_pCurrentScene = m_pLobbyScene.get();
		
		m_pLobbyScene->Init();
		m_pGameScene->Init();
		m_pEndScene->Init();
		return true;
	}

	bool Frame()
	{
		switch (m_pCurrentScene->m_iSceneID)
		{
		case GAME_SCENE_LOBBY:
			if (m_pCurrentScene->m_bNextSceneStart == true)
			{
				m_pCurrentScene->m_bNextSceneStart = false;
				m_pCurrentScene = m_pGameScene.get();
				m_pCurrentScene->m_bNextSceneStart = true;
			}
			break;
		case GAME_SCENE_PLAY:
			if (m_pCurrentScene->m_bNextSceneStart == true)
			{
				if ((m_pCurrentScene->m_bEndSceneStart) || (m_iLevel > 3))
				{
					m_pCurrentScene->SetNPCCount(g_iMaxNPCCount);
					m_pCurrentScene->Reset();
					m_iLevel = 1;
					m_pCurrentScene = m_pEndScene.get();
					break;
				}
				else
				{
					m_pCurrentScene = m_pGameScene.get();
					m_pCurrentScene->SetNPCCount(g_iMaxNPCCount);
					++m_iLevel;
					m_pCurrentScene->Reset();
					m_Timer.NPCRegenTime(g_NPC_REGENTIME);
					m_Timer.GameStartTime(g_TOTAL_GAMETIME);
				}
			}
			break;
		case GAME_SCENE_END:
			break;
		}

		m_pCurrentScene->Frame();
		return true;
	}
	bool Render()
	{
		m_pCurrentScene->Render();
		return true;
	}
	bool Release()
	{
		m_pLobbyScene->Release();
		m_pGameScene->Release();
		m_pEndScene->Release();

		return true;
	}

	void NPCRegenAlarm()
	{
		m_pGameScene->NPCRegenAlarm();
	}

	void GameEndTimeAlarm()
	{
		m_pCurrentScene->m_bNextSceneStart = true;
		m_pCurrentScene->m_bEndSceneStart = true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("SampleBitmap", 800, 600);