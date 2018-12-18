#include "stdafx.h"
#include "TWorkThread.h"
#include "XUser.h"
#include "TSvrIOCP.h"
#include "TLogError.h"

void TWorkThread::Run()
{
	bool ret;
	LPOVERLAPPED ov;
	DWORD bytes;
	ULONG_PTR key;

	while (1)
	{
		ret = ::GetQueuedCompletionStatus(I_IOCP.m_hIOCP, &bytes, &key, &ov, INFINITE);
		if (ret == TRUE)
		{
			LPOVERLAPPED_EX ovex = (OVERLAPPED_EX*)ov;
			XUser* pUser = (XUser*)key;
			if (pUser == NULL)
			{
				I_LOG.T_ERROR();
				continue;
			}

			if (bytes != 0 && ovex != NULL)
			{
				pUser->Dispatch(bytes, ovex);
			}
			else
			{
				//ERROR_OPERATION_ABORTED : 1���� �������� ���Ŵ������ ���¿���
				//Send�����͸� �����ԵǸ� Recv������� ���°� ��ҵȴ�.
				//1���� �������δ� ���ÿ� �� �۾��� �Ҽ��ִ�.
				if (ERROR_OPERATION_ABORTED != GetLastError() && bytes == 0)
				{
					I_User.DelUser(pUser->m_clientSocket);
				}
			}
		}
		else
		{
			I_LOG.T_ERROR();
		}
	}
}
TWorkThread::TWorkThread()
{
}


TWorkThread::~TWorkThread()
{
}
