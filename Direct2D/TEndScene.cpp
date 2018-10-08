#include "TEndScene.h"
#include "xInput.h"
#include "TCollision.h"


bool TEndScene::Init()
{
	m_bNextSceneStart = false;
	m_iSceneID = GAME_SCENE_END;

	m_BackGround.Init();
	m_BackGround.SetPosition((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Create(g_pd3dDevice, L"vertexshader.txt", L"../../data/RAINBOW.bmp");


	m_BtnStart.Init();
	m_BtnStart.SetPosition((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 334, 79);
	m_BtnStart.Create(g_pd3dDevice, L"vertexshader.txt", L"../../data/end.bmp");
	/*m_BtnStart.LoadUI(L"../../data/end.bmp", NULL, 0);
	m_BtnStart.LoadUI(L"../../data/end.bmp", NULL, 1);
	m_BtnStart.LoadUI(L"../../data/end.bmp", NULL, 2);
	m_BtnStart.LoadUI(L"../../data/end.bmp", NULL, 3);*/

	return true;
}
bool TEndScene::Frame()
{
	m_BackGround.Frame();
	if (I_Input.m_MouseState.rgbButtons[0] &&
		TCollision::RectInPoint(m_BtnStart.m_rtCollision, g_pMousePos))
	{
		DestroyWindow(g_hWnd);
	}

	m_BtnStart.Frame();
	return true;
}
bool TEndScene::Render()
{
	m_BackGround.Render();
	m_BtnStart.Render();
	return true;
}
bool TEndScene::Release()
{
	m_BackGround.Release();
	m_BtnStart.Release();
	return true;
}

TEndScene::TEndScene()
{
	m_iSceneID = GAME_SCENE_END;
	m_bNextSceneStart = false;
}


TEndScene::~TEndScene()
{
}
