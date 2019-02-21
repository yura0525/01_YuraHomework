#include "TWorkThread.h"
#include "TServerIOCP.h"
#include "TServer.h"
#include "TDebugString.h"

bool TWorkThread::Run()
{
	DWORD			bytesTransfer;
	ULONG_PTR		keyValue;
	LPOVERLAPPED	overlapped;
	BOOL			bReturn;

	while (m_bLoop)
	{
		bReturn = ::GetQueuedCompletionStatus(I_ServerIOCP.m_hIOCP, &bytesTransfer, &keyValue, &overlapped, INFINITE);
		TUser* pSession = (TUser*)keyValue;

		if (pSession == NULL)
			continue;

		if (bReturn == TRUE && bytesTransfer != 0 && overlapped != NULL)
		{
			{
				TSynchronize sync(this);
				pSession->Dispatch(bytesTransfer, overlapped);
			}
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
					// Ŭ���̾�Ʈ ���� ����
					I_Server.DelUser(pSession->m_Socket);
					continue;
				}
			}
		}
		I_Server.ProcessPacket();
	}
	return true;
}

TWorkThread::TWorkThread()
{
	m_bLoop = true;
}


TWorkThread::~TWorkThread()
{
	m_bLoop = false;
}
