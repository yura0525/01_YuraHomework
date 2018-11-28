#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>

#pragma comment(lib, "ws2_32")

#define MAX_WORK_THREAD 4
#define RECV 1000
#define SEND 2000

struct OVERLAPPED_EX : OVERLAPPED
{
	DWORD flag;
};

struct TUser
{
	SOCKET sock;
	WSABUF buffer;
	char buf[2048];
	OVERLAPPED_EX ov;
};

TUser user;
HANDLE m_hIOCP;

void main()
{

}