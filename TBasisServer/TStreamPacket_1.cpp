#include "TStreamPacket_1.h"
#include "TUser_1.h"

void TStreamPacket_1::Put(char* recvBuffer, int iRecvSize, TUser_1* pUser)
{
	{
		TSynchronize sync(this);
		// �޴� ������ �뷮�� �����ϸ�
		if (m_iWritePos + iRecvSize >= MAX_RECV_SIZE)
		{
			char strTemp[MAX_RECV_SIZE] = { 0, };
			memcpy(strTemp, &m_RecvBuffer[m_iStartPos], m_iReadPos);
			ZeroMemory(&m_RecvBuffer, sizeof(char) * MAX_RECV_SIZE);
			memcpy(&m_RecvBuffer, &strTemp, m_iReadPos);
			m_pPacket = (P_UPACKET)m_RecvBuffer;
			m_iStartPos = 0;
			m_iWritePos = m_iReadPos;
		}
		
		memcpy(&m_RecvBuffer[m_iWritePos], recvBuffer, iRecvSize);
		m_iWritePos += iRecvSize;
		m_iReadPos += iRecvSize;

		if (m_iReadPos < PACKET_HEADER_SIZE)
			return;

		//��Ŷ�� ����
		m_pPacket = (P_UPACKET)&m_RecvBuffer[m_iStartPos];
		// 1���� ��Ŷ �����ŭ �޾Ҵٸ�
		if (m_iReadPos >= m_pPacket->ph.len)
		{
			do
			{
				T_PACKET add;
				ZeroMemory(&add, sizeof(add));
				memcpy(&add.packet, &m_RecvBuffer[m_iStartPos], m_pPacket->ph.len);
				add.pUser = pUser;

				if (pUser != NULL)
				{
					pUser->AddPacket(add);
				}

				// 1���� ��Ŷ�� ó���ϰ� ���� �ܿ��� ũ��
				m_iReadPos -= m_pPacket->ph.len;
				// ��Ŷ�� ���� ��ġ
				m_iStartPos += m_pPacket->ph.len;

				// �ܿ����� ��Ŷ ������� ���� ���
				if (m_iReadPos < PACKET_HEADER_SIZE)
					break;
				m_pPacket = (P_UPACKET)&m_RecvBuffer[m_iStartPos];
			} while (m_iReadPos >= m_pPacket->ph.len);
		}
	}
}

TStreamPacket_1::TStreamPacket_1()
{
	ZeroMemory(m_RecvBuffer, sizeof(char) * MAX_RECV_SIZE);
	m_iStartPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	m_pPacket = NULL;
}


TStreamPacket_1::~TStreamPacket_1()
{
}
