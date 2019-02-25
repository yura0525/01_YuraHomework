#include "TStreamPacket.h"
#include "TServer.h"

void TStreamPacket::Put(char* recvBuffer, int iRecvSize, TUser* pUser)
{
	{
		TSynchronize sync(this);
		// WSARecv ���۰� 1���� ��Ŷ ũ�⺸�� ����� ũ�� ��Ŷ�� �񵿱� ó���� 1���� ���´�.
		m_pPacket = (P_UPACKET)recvBuffer;
		if (m_pPacket->ph.iotype == 777 && m_pPacket->ph.len == iRecvSize)
		{
			T_PACKET	add;
			ZeroMemory(&add, sizeof(add));
			CopyMemory(&add.packet, recvBuffer, m_pPacket->ph.len);
			add.pUser = pUser;
			if (pUser != NULL)
			{
				I_Server.AddPacket(add);
			}
			return;
		}
		//return;
		// ������  ���� ��Ŷ�� ũ�⺸�� WSARecv ���۰� ������ �ɰ����� ���´�.
		// �޴� ������ �뷮�� �����ϸ�
		if ((m_iWritePos + iRecvSize) >= MAX_RECV_SIZE)
		{
			char strTemp[MAX_RECV_SIZE] = { 0, };
			memcpy(strTemp, &m_RecvBuffer[m_iStartPos], m_iReadPos);
			ZeroMemory(&m_RecvBuffer, sizeof(char)* MAX_RECV_SIZE);
			memcpy(&m_RecvBuffer, &strTemp, m_iReadPos);
			m_iStartPos = 0;
			m_iWritePos = m_iReadPos;
		}

		memcpy(&m_RecvBuffer[m_iWritePos], recvBuffer, iRecvSize);
		m_iWritePos += iRecvSize;
		m_iReadPos += iRecvSize;

		if (m_iReadPos < PACKET_HEADER_SIZE) return;

		// ��Ŷ�� ����
		m_pPacket = (P_UPACKET)&m_RecvBuffer[m_iStartPos];
		if (m_pPacket->ph.iotype != 777)
			return;

		// 1���� ��Ŷ �����ŭ �޾Ҵٸ�
		if (m_iReadPos >= m_pPacket->ph.len)
		{
			do {
				T_PACKET    add;
				ZeroMemory(&add, sizeof(add));
				memcpy(&add.packet, &m_RecvBuffer[m_iStartPos], m_pPacket->ph.len);
				add.pUser = pUser;

				if (pUser != NULL)
				{
					I_Server.AddPacket(add);
				}

				// 1���� ��Ŷ�� ó���ϰ� ���� �ܿ��� ũ��
				m_iReadPos -= m_pPacket->ph.len;
				// ��Ŷ�� ���� ��ġ
				m_iStartPos += m_pPacket->ph.len;

				// �ܿ����� ��Ŷ ������� ���� ���
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				m_pPacket = (P_UPACKET)&m_RecvBuffer[m_iStartPos];
				if (m_pPacket->ph.iotype != 777)
					return;

			} while (m_iReadPos >= m_pPacket->ph.len);
		}
	}
}

void TStreamPacket::Reset()
{
	ZeroMemory(m_RecvBuffer, sizeof(char) * MAX_RECV_SIZE);
	m_iStartPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	m_pPacket = NULL;
}
TStreamPacket::TStreamPacket()
{
	Reset();
}


TStreamPacket::~TStreamPacket()
{
}
