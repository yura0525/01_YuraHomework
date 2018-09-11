#include "xCore.h"


class Sample : public xCore
{
public:
	bool Init()
	{
		xCore::Init();
		IDXGISurface1*		pBackBuffer;
		
		HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
		m_Font.Set(m_hWnd, m_sd.BufferDesc.Width, m_sd.BufferDesc.Height, pBackBuffer);
		if (pBackBuffer != NULL)
			pBackBuffer->Release();

		//TTextArray s0, s1, s2;
		//s0.m_pText = L"aaa";
		//s1.m_pText = L"bbb";
		//s2.m_pText = L"ccc";

		//m_Font.m_TextList.push_back(s0);
		//m_Font.m_TextList.push_back(s1);
		//m_Font.m_TextList.push_back(s2);

		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		
		TCHAR	m_csBuffer[256];

		if (g_Input.bFront)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bFront!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bFront)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bFront!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bBack)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bBack!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bLeft)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bLeft!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bRight)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bRight!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bAttack)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bAttack!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bJump)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bJump!!!!!\n");
			OutputDebugString(m_csBuffer);
		}

		return true;
	}
	bool Render()
	{
		xCore::Render();

		RECT rt = g_rtClient;
		m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_Font.DrawText(rt, L"kgca!!!", D2D1::ColorF(1, 1, 1, 0.5f));

		TCHAR	m_csBuffer[256];
		_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f",
			m_Timer.m_iFramePerSecond, m_Timer.m_fGameTime, m_Timer.m_fSecPerFrame);


		rt.top += 50;
		m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_Font.DrawText(rt, m_csBuffer, D2D1::ColorF(1, 1, 1, 1.0f));

		return true;
	}
	bool Release()
	{
		xCore::Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("CreateDevice", 800, 600);