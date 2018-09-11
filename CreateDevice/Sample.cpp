#include "xCore.h"

struct TOutput								//������� ������ ������ �˼��ִ�.
{
	DXGI_OUTPUT_DESC				desc;
	std::vector<DXGI_MODE_DESC>		m_pModeList;
	std::vector<IDXGIOutput*>		m_pOutputList;
};

struct TAdapter									//�׷���ī��
{
	IDXGIAdapter*				m_pAdapter;
	DXGI_ADAPTER_DESC			desc;
	std::vector<TOutput>		m_OutputList;	//�׷���ī�忡 ����� �����
};

class Sample : public xCore
{
public:
	bool GamePreInit()
	{
		HRESULT hr;

		TDevice::CreateDX11GIFactory();
		std::vector<TAdapter> m_pTAdapterList;			//�׷���ī�� ������ ������ �˼��ִ�.

		for (int iAdapter = 0; ; iAdapter++)
		{
			IDXGIAdapter* pAdapter = NULL;
			TAdapter tAdapter;
			hr = m_pDXGIFactory->EnumAdapters(iAdapter, &pAdapter);

			if (FAILED(hr))	break;

			pAdapter->GetDesc(&tAdapter.desc);
			tAdapter.m_pAdapter = pAdapter;
			m_pTAdapterList.push_back(tAdapter);
		}

		for (int iAdapter = 0; iAdapter < m_pTAdapterList.size(); iAdapter++)
		{
			DXGI_MODE_DESC	desc[512];
			for (int iOutput = 0; ; iOutput++)		//�׷���ī�� ���� ����� ����͸� ã�´�.
			{
				TOutput Output;
				IDXGIOutput* pOutput = NULL;

				hr = m_pTAdapterList.at(iAdapter).m_pAdapter->EnumOutputs(iOutput, &pOutput);

				if (pOutput == NULL || FAILED(hr))	break;

				if (pOutput != NULL)
				{
					pOutput->GetDesc(&Output.desc);
					UINT NumModes = 512;

					hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
						DXGI_ENUM_MODES_SCALING, &NumModes, desc);

					Output.m_pOutputList.push_back(pOutput);

					m_pTAdapterList.at(iAdapter).m_OutputList.push_back(Output);

					for (int iMode = 0; iMode < 512; iMode++)
					{
						if (desc[iMode].Width >= 800 && desc[iMode].Width <= 1920 &&
							desc[iMode].Height >= 600 && desc[iMode].Height <= 1080)
						{
							m_pTAdapterList.at(iAdapter).m_OutputList[iOutput].m_pModeList.push_back(desc[iMode]);
						}
					}
				}
			}
		}
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("CreateDevice", 800, 600);