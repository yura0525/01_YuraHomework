#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
//패킷은 1바이트로 패킹
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
///   아이템 생성 패킷
typedef struct {
	byte    szName[13];
	int		iIndex;
	int		iData[10];
}ITEM_INFO;
//1바이트로 패킹 된것을 다시 원래의 값으로 되돌려준다.
#pragma pack(pop)



//////////////  패킷의 타입들 /////////
#define PACKET_CHAT_MSG    1000
#define PACKET_GAME_START    2000
#define PACKET_GAME_END    3000
#define PACKET_CREATE_CHARACTER    4000
#define PACKET_MOVE_CHARACTER    5000
