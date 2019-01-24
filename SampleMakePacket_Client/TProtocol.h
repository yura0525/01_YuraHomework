#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
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
	int iData[50];
}CHARACTER_INFO;
///   ������ ���� ��Ŷ
typedef struct {
	byte    szName[13];
	int		iIndex;
	int		iData[10];
}ITEM_INFO;
//1����Ʈ�� ��ŷ �Ȱ��� �ٽ� ������ ������ �ǵ����ش�.
#pragma pack(pop)



//////////////  ��Ŷ�� Ÿ�Ե� /////////
#define PACKET_CHAT_MSG    1000
#define PACKET_GAME_START    2000
#define PACKET_GAME_END    3000
#define PACKET_CREATE_CHARACTER    4000
#define PACKET_MOVE_CHARACTER    5000
