#include "TStreamPacket_2.h"
#include "TDebugString_1.h"

void TStreamPacket_2::Put(char* recvBuffer, int iRecvSize)
{
	memcpy(&m_strBuffer[m_iWritePos], recvBuffer, sizeof(char)*iRecvSize);
	m_iWritePos += iRecvSize;
	// �ּҰ� ����
	m_pPacketData = (P_UPACKET)m_strBuffer;

	// ��Ŷ ũ�⺸�ٴ� ���� �����Ͱ� ũ�ų� ����.
	if (m_iWritePos >= m_pPacketData->ph.len)
	{
		static int iCount = 0;

		do {
			iCount++;
			// ���1:���� �����Ͱ� 1���� ��Ŷ���� ���� ���.
			// ���2:�������� ��Ŷ�� �����͸� �޾��� ���.
			// ���1 & ���2 = �޵�� ������ ��Ŷ �����͸� �����ؾ� �Ѵ�.
			memcpy(&m_PacketData, m_strBuffer, m_pPacketData->ph.len);

			UPACKET  addPacket = { 0, };
			ZeroMemory(&addPacket, sizeof(UPACKET));
			addPacket.ph = m_PacketData.ph;
			memcpy(&addPacket.msg, &m_PacketData.msg, m_PacketData.ph.len - PACKET_HEADER_SIZE);
			m_PacketList.push_back(addPacket);

			ZeroMemory(&m_PacketData, sizeof(UPACKET));

			m_iWritePos -= m_pPacketData->ph.len;
			// ��Ȯ�� ũ�Ⱑ ���ͼ� 1���� ��Ŷ�� �ϼ����� ��� ���� �̷� ���� ����.
			if (addPacket.ph.len == iRecvSize) break;

			m_iRecvPos += addPacket.ph.len;
			m_pPacketData = (P_UPACKET)&m_strBuffer[m_iRecvPos];//addPacket.ph.len];

		} while (m_iWritePos >= m_pPacketData->ph.len);
		I_DebugStr.DisplayText(const_cast<char*>("\n%d\r\n"), iCount);
		iCount = 0;
		TCHAR strTmp[256] = { 0, };
		memcpy(strTmp, &m_strBuffer[m_iRecvPos], m_iWritePos);
		ZeroMemory(&m_strBuffer, sizeof(char) * 2048);
		memcpy(&m_strBuffer, &strTmp, m_iWritePos);
		m_pPacketData = (P_UPACKET)m_strBuffer;
		m_iRecvPos = 0;
	}
	else
	{
		//���3:�ϼ��� ��Ŷ�� ����� ������ �����Ͱ� ���� ���.
		I_DebugStr.DisplayText(const_cast<char*>("\n%d\r\n"), m_iWritePos);
	}
}

TStreamPacket_2::TStreamPacket_2()
{
	m_iWritePos = 0;
	m_iRecvPos = 0;
}


TStreamPacket_2::~TStreamPacket_2()
{
}
