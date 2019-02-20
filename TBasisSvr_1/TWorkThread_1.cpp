#include "TWorkThread_1.h"
#include "TServerIOCP_1.h"
#include "TServer_1.h"
#include "TDebugString_2.h"

bool TWorkThread_1::Run()
{
	DWORD			bytesTransfer;
	ULONG_PTR		keyValue;
	BOOL			bReturn;
	LPOVERLAPPED	overlapped;
	while (m_bLoop)
	{
		bReturn = ::GetQueuedCompletionStatus(I_ServerIOCP.m_hIOCP, &bytesTransfer, &keyValue, &overlapped, INFINITE);
		
		TUser* pSession = (TUser*)keyValue;
		if (pSession == 0) continue;

		if (bReturn == TRUE && bytesTransfer != 0 && overlapped != 0)
		{
			pSession->Dispatch(bytesTransfer, overlapped);
		}
		else
		{
			// �ϳ��� �������� RECV�� ���Ŵ������ ���¿���
			// SEND�� �����͸� ������ �Ǹ�
			// RECV�� ������̴� �� ��ҵȴ�.
			if (keyValue != 0 && GetLastError() != ERROR_OPERATION_ABORTED)
			{
				// ������ ���� ���� ����.
				if (bytesTransfer == 0)
				{
					if (bReturn == FALSE)
						I_DebugStr.DisplayText("%s%s\r\n",
							pSession->m_Name.c_str(), "�񵿱� ���� ����.");
					// Ŭ���̾�Ʈ ���� ����
					I_Server.DelUser(pSession->m_iEvent);
					continue;
				}
			}
		}
		I_Server.ProcessPacket();
	}
	return true;
}

TWorkThread_1::TWorkThread_1()
{
	m_bLoop = true;
}


TWorkThread_1::~TWorkThread_1()
{
	m_bLoop = false;
}
