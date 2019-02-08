#pragma once
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

int PrintAddress(SOCKADDR* sa, int salen);
int FormatAddress(SOCKADDR* sa, int salen, char* addrbuf, int addrbuflen);
struct addrinfo* ResolveAddress(char* addr, const char* port, int af, int type, int proto);


