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
			// 하나의 소켓으로 RECV로 수신대기중인 상태에서
			// SEND로 데이터를 보내게 되면
			// RECV로 대기중이던 게 취소된다.
			if (keyValue != 0 && GetLastError() != ERROR_OPERATION_ABORTED)
			{
				// 소켓을 삭제 유저 삭제.
				if (bytesTransfer == 0)
				{
					if (bReturn == FALSE)
						I_DebugStr.DisplayText("%s%s\r\n",
							pSession->m_Name.c_str(), "비동기 소켓 오류.");
					// 클라이언트 접속 종료
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
