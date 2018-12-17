#pragma once
#include <windows.h>

//��Ŷ�� 1����Ʈ�� ��ŷ
#pragma pack(push, 1)

typedef struct
{
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER ph;
	char msg[2048];
}UPACKET, *P_UPACKET;

typedef struct
{
	byte szName[13];
	int iIndex;
	int iData[20];
}CHARACTER_INFO;

//�ٽ� ������ ������ �ǵ����ش�.
#pragma pack(pop)


#define PACKET_HEADER_SIZE 4

// TYPE
#define PACKET_CHAT_MSG 1000
#define PACKET_CREATE_CHARACTER 2000