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
			// 하나의 소켓으로 RECV로 수신대기중인 상태에서
			// SEND로 데이터를 보내게 되면
			// RECV로 대기중이던 게 취소된다.
			if (keyValue != 0 && GetLastError() != ERROR_OPERATION_ABORTED)
			{
				// 소켓을 삭제 유저 삭제.
				if (bytesTransfer == 0)
				{
					// 클라이언트 접속 종료
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
