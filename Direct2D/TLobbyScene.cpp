#include "TLobbyScene.h"
#include "xInput.h"
#include "TCollision.h"

bool TLobbyScene::Init()
{
	m_bNextSceneStart = false;
	
	m_BackGround.Init();
	m_BackGround.SetTexUV(800, 600);
	m_BackGround.SetPosition((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Create(g_pd3dDevice, L"vertexshader.txt", L"../../data/lobby.jpg");


	m_BtnStart.Init();
	m_BtnStart.SetTexUV(332, 82);
	m_BtnStart.SetPosition((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 332, 82);
	m_BtnStart.Create(g_pd3dDevice, L"vertexshader.txt", L"../../data/main_start_nor.bmp");
	/*m_BtnStart.LoadUI(L"../../data/main_start_nor.bmp", NULL, 0);
	m_BtnStart.LoadUI(L"../../data/main_start_pus.bmp", NULL, 1);
	m_BtnStart.LoadUI(L"../../data/main_start_sel.bmp", NULL, 2);
	m_BtnStart.LoadUI(L"../../data/main_start_dis.bmp", NULL, 3);*/

	return true;
}
bool TLobbyScene::Frame()
{
	m_BackGround.Frame();

	if (I_Input.m_CurrentMouseState.rgbButtons[0] == KEY_PUSH && 
		TCollision::RectInPoint(m_BtnStart.m_rtCollision, g_pMousePos))
	{
		m_bNextSceneStart = true;
	}
	m_BtnStart.Frame();
	return true;
}
bool TLobbyScene::Render()
{
	m_BackGround.Render();
	m_BtnStart.Render();
	return true;
}
bool TLobbyScene::Release()
{
	m_BackGround.Release();
	m_BtnStart.Release();
	return true;
}

TLobbyScene::TLobbyScene()
{
	m_iSceneID = GAME_SCENE_LOBBY;
}

TLobbyScene::~TLobbyScene()
{
}
