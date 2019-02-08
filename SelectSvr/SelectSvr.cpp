// define this before include winsock2.h to up the allowed size
#define FD_SET_SIZE 1024

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "TSelect.h"

#define DEFAULT_BUFFER_SIZE 4096

int gAddressFamily = AF_UNSPEC,
gSocketType = SOCK_STREAM,
gProtocol = IPPROTO_TCP,
gBufferSize = DEFAULT_BUFFER_SIZE;

char* gBindAddr = NULL;
const char* gBindPort = "10000";

typedef struct _BUFFER_OBJ
{
	char*				buf;
	int					buflen;
	SOCKADDR_STORAGE	addr;
	int					addrlen;
	struct _BUFFER_OBJ	* next;
}BUFFER_OBJ;

typedef struct _SOCKET_OBJ
{
	SOCKET s;
	int listening;
	int closing;
	SOCKADDR_STORAGE addr;
	int addrlen;
	BUFFER_OBJ* pending, *pendingtail;

	struct _SOCKET_OBJ *next, *prev;
}SOCKET_OBJ;

SOCKET_OBJ * gSocketList = NULL, *gSocketListEnd = NULL;
int gSocketCount = 0;

volatile LONG gBytesRead = 0,
gBytesSent = 0,
gStartTime = 0,
gBytesReadLast = 0,
gBytesSentLast = 0,
gStartTimeLast = 0,
gCurrentConnections = 0;

void usage(char *progname)
{
	fprintf(stderr, "usage: %s [-a 4|6] [-e port] [-l local-addr] [-p udp|tcp]\n",
		progname);
	fprintf(stderr, "  -a 4|6     Address family, 4 = IPv4, 6 = IPv6 [default = IPv4]\n"
		"  -b size    Buffer size for send/recv [default = %d]\n"
		"  -e port    Port number [default = %s]\n"
		"  -l addr    Local address to bind to [default INADDR_ANY for IPv4 or INADDR6_ANY for IPv6]\n"
		"  -p tcp|udp Which protocol to use [default = TCP]\n",
		gBufferSize,
		gBindPort
	);
	ExitProcess(-1);
}

BUFFER_OBJ* GetBufferObj(int buflen)
{
	BUFFER_OBJ * newobj = NULL;
	newobj = (BUFFER_OBJ*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BUFFER_OBJ));

	if(newobj == NULL)
	{
		fprintf(stderr, "GetBufferObj: HeapAlloc failed: %d\n", GetLastError());
		ExitProcess(-1);
	}

	newobj->buf = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BYTE) * buflen);
	if (newobj->buf == NULL)
	{
		fprintf(stderr, "GetBufferObj: HeapAlloc failed: %d\n", GetLastError());
		ExitProcess(-1);
	}
	newobj->buflen = buflen;
	newobj->addrlen = sizeof(newobj->addr);
	return newobj;
}


void FreeBufferObj(BUFFER_OBJ *obj)
{
	HeapFree(GetProcessHeap(), 0, obj->buf);
	HeapFree(GetProcessHeap(), 0, obj);
}

SOCKET_OBJ* GetSocketObj(SOCKET s, int listening)
{
	SOCKET_OBJ * sockobj = NULL;
	sockobj = (SOCKET_OBJ*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SOCKET_OBJ));

	if (sockobj == NULL)
	{
		fprintf(stderr, "GetSocketObj: HeapAlloc failed: %d\n", GetLastError());
		ExitProcess(-1);
	}

	sockobj->s = s;
	sockobj->listening = listening;
	sockobj->addrlen = sizeof(sockobj->addr);

	return sockobj;
}

void FreeSocketObj(SOCKET_OBJ *obj)
{
	BUFFER_OBJ *ptr = NULL, *tmp = NULL;
	ptr = obj->pending;

	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;

		FreeBufferObj(tmp);
	}

	HeapFree(GetProcessHeap(), 0, obj);
}

void InsertSocketObj(SOCKET_OBJ* sock)
{
	sock->next = sock->prev = NULL;
	if (gSocketList == NULL)
	{
		gSocketList = gSocketListEnd = sock;
	}
	else
	{
		sock->prev = gSocketListEnd;
		gSocketListEnd->next = sock;
		gSocketListEnd = sock;
	}

	gSocketCount++;
}

void RemoveSocketObj(SOCKET_OBJ * sock)
{
	if (sock->prev)
	{
		sock->prev->next = sock->next;
	}
	if (sock->next)
	{
		sock->next->prev = sock->prev;
	}
	if (gSocketList == sock)
		gSocketList = sock->next;
	if (gSocketListEnd == sock)
		gSocketListEnd = sock->prev;

	gSocketCount--;
}

void EnqueueBufferObj(SOCKET_OBJ* sock, BUFFER_OBJ* obj, BOOL AtHead)
{
	if (sock->pending == NULL)
	{
		sock->pending = sock->pendingtail = obj;
	}
	else if(AtHead == FALSE)
	{
		sock->pendingtail->next = obj;
		sock->pendingtail = obj;
	}
	else
	{
		obj->next = sock->pending;
		sock->pending = obj;
	}
}

BUFFER_OBJ* DequeueBufferObj(SOCKET_OBJ* sock)
{
	BUFFER_OBJ* ret = NULL;
	if (sock->pendingtail != NULL)
	{
		ret = sock->pending;

		sock->pending = sock->pending->next;
		if (sock->pendingtail == ret)
		{
			sock->pendingtail = NULL;
		}
	}

	return ret;
}

void ValidateArgs(int argc, char **argv)
{
	int     i;

	for (i = 1; i < argc; i++)
	{
		if (((argv[i][0] != '/') && (argv[i][0] != '-')) || (strlen(argv[i]) < 2))
			usage(argv[0]);
		else
		{
			switch (tolower(argv[i][1]))
			{
			case 'a':               // address family - IPv4 or IPv6
				if (i + 1 >= argc)
					usage(argv[0]);
				if (argv[i + 1][0] == '4')
					gAddressFamily = AF_INET;
				else if (argv[i + 1][0] == '6')
					gAddressFamily = AF_INET6;
				else
					usage(argv[0]);
				i++;
				break;
			case 'b':               // buffer size for send/recv
				if (i + 1 >= argc)
					usage(argv[0]);
				gBufferSize = atol(argv[++i]);
				break;
			case 'e':               // endpoint - port number
				if (i + 1 >= argc)
					usage(argv[0]);
				gBindPort = argv[++i];
				break;
			case 'l':               // local address for binding
				if (i + 1 >= argc)
					usage(argv[0]);
				gBindAddr = argv[++i];
				break;
			case 'p':               // protocol - TCP or UDP
				if (i + 1 >= argc)
					usage(argv[0]);
				if (_strnicmp(argv[i + 1], "tcp", 3) == 0)
				{
					gProtocol = IPPROTO_TCP;
					gSocketType = SOCK_STREAM;
				}
				else if (_strnicmp(argv[i + 1], "udp", 3) == 0)
				{
					gProtocol = IPPROTO_UDP;
					gSocketType = SOCK_DGRAM;
				}
				else
					usage(argv[0]);
				i++;
				break;
			default:
				usage(argv[0]);
				break;
			}
		}
	}
}

int ReceivePendingData(SOCKET_OBJ* sockobj)
{
	BUFFER_OBJ* buffobj = NULL;
	int rc, ret;

	buffobj = GetBufferObj(gBufferSize);
	ret = 0;

	if (gProtocol == IPPROTO_TCP)
	{
		rc = recv(sockobj->s, buffobj->buf, buffobj->buflen, 0);
	}
	else
	{
		rc = recvfrom(sockobj->s, buffobj->buf, buffobj->buflen, 0, (SOCKADDR*)&buffobj->addr, &buffobj->addrlen);
	}

	if (rc == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			// Socket connection has failed, close the socket
			fprintf(stderr, "recv(from) failed: %d\n", WSAGetLastError());

			closesocket(sockobj->s);

			ret = -1;
		}
		FreeBufferObj(buffobj);
	}
	else if (rc == 0)
	{
		// Graceful close
		if (gProtocol == IPPROTO_TCP)
		{
			FreeBufferObj(buffobj);
		}
		else
		{
			buffobj->buflen = 0;
			EnqueueBufferObj(sockobj, buffobj, FALSE);
		}

		sockobj->closing = TRUE;

		if (sockobj->pending == NULL)
		{
			// If no sends are pending, close the socket for good
			closesocket(sockobj->s);

			ret = -1;
		}
	}
	else
	{
		gBytesRead += rc;
		gBytesReadLast += rc;
		buffobj->buflen = rc;
		EnqueueBufferObj(sockobj, buffobj, FALSE);
	}
	return ret;
}

int SendPendingData(SOCKET_OBJ* sock)
{
	BUFFER_OBJ* bufobj = NULL;
	BOOL		breakouter;
	int			nleft, idx, ret, rc;

	ret = 0;
	while (bufobj = DequeueBufferObj(sock))
	{
		if (gProtocol == IPPROTO_TCP)
		{
			breakouter = FALSE;
			nleft = bufobj->buflen;
			idx = 0;

			while (nleft)
			{
				rc = send(sock->s, &bufobj->buf[idx], nleft, 0);
				if (rc == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSAEWOULDBLOCK)
					{
						BUFFER_OBJ * newbuf = NULL;
						newbuf = GetBufferObj(nleft);
						memcpy(newbuf->buf, &bufobj->buf[idx], nleft);

						EnqueueBufferObj(sock, newbuf, TRUE);
					}
					else
					{
						ret = -1;
					}
					breakouter = TRUE;
					break;
				}
				else
				{
					gBytesSent += rc;
					gBytesSentLast += rc;
					nleft -= rc;
					idx += rc;
				}
			}
			FreeBufferObj(bufobj);
			if (breakouter)
				break;
		}
		else
		{
			rc = sendto(sock->s, bufobj->buf, bufobj->buflen, 0, (SOCKADDR*)&bufobj->addr, bufobj->addrlen);
			if (rc == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					// If the send couldn't be made, put the buffer
					// back at the head of the queue
					EnqueueBufferObj(sock, bufobj, TRUE);

					ret = 0;
				}
				else
				{
					// Socket error occured so indicate the error to the caller
					ret = -1;
				}
				break;
			}
			else
			{
				FreeBufferObj(bufobj);
			}
		}
	}

	if ((sock->pending == NULL) && (sock->closing))
	{
		closesocket(sock->s);
		ret = -1;
		printf("Closing connection\n");
	}
	return ret;
}

void PrintStatistics()
{
	ULONG       bps, tick, elapsed;

	tick = GetTickCount();

	elapsed = (tick - gStartTime) / 1000;

	if (elapsed == 0)
		return;

	printf("\n");

	bps = gBytesSent / elapsed;
	printf("Average BPS sent: %lu [%lu]\n", bps, gBytesSent);

	bps = gBytesRead / elapsed;
	printf("Average BPS read: %lu [%lu]\n", bps, gBytesRead);

	elapsed = (tick - gStartTimeLast) / 1000;

	if (elapsed == 0)
		return;

	bps = gBytesSentLast / elapsed;
	printf("Current BPS sent: %lu\n", bps);

	bps = gBytesReadLast / elapsed;
	printf("Current BPS read: %lu\n", bps);

	printf("Current Connections: %lu\n", gCurrentConnections);

	InterlockedExchange(&gBytesSentLast, 0);
	InterlockedExchange(&gBytesReadLast, 0);

	gStartTimeLast = tick;
}


int main(int argc, char **argv)
{
	WSADATA wsd;
	SOCKET s;
	SOCKET_OBJ* sockobj = NULL, *sptr = NULL, *tmp = NULL;
	ULONG			lastprint = 0;
	int rc;
	struct fd_set fdread, fdwrite, fdexcept;
	struct timeval timeout;
	struct addrinfo* res = NULL, *ptr = NULL;


	ValidateArgs(argc, argv);

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		fprintf(stderr, "unable to load Winsock!\n");
		return -1;
	}

	printf("Local address: %s; Port: %s; Family: %d\n",
		gBindAddr, gBindPort, gAddressFamily);

	res = ResolveAddress(gBindAddr, gBindPort, gAddressFamily, gSocketType, gProtocol);
	if (res == NULL)
	{
		fprintf(stderr, "ResolveAddress failed to return any addresses!\n");
		return -1;
	}

	// For each local address returned, create a listening/receiving socket
	ptr = res;
	while (ptr)
	{
		PrintAddress(ptr->ai_addr, ptr->ai_addrlen); printf("\n");

		s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (s == INVALID_SOCKET)
		{
			fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
			return -1;
		}

		sockobj = GetSocketObj(s, (gProtocol == IPPROTO_TCP) ? TRUE : FALSE);

		InsertSocketObj(sockobj);

		// bind the socket to a local address and port
		rc = bind(sockobj->s, ptr->ai_addr, ptr->ai_addrlen);
		if (rc == SOCKET_ERROR)
		{
			fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
			return -1;
		}

		if (gProtocol == IPPROTO_TCP)
		{
			rc = listen(sockobj->s, 10000);
			if (rc == SOCKET_ERROR)
			{
				fprintf(stderr, "listen failed: %d\n", WSAGetLastError());
				return -1;
			}
		}

		ptr = ptr->ai_next;
	}
	freeaddrinfo(res);
	gStartTime = gStartTimeLast = lastprint = GetTickCount();

	while (1)
	{
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcept);

		sptr = gSocketList;

		while (sptr)
		{
			FD_SET(sptr->s, &fdread);
			FD_SET(sptr->s, &fdwrite);
			FD_SET(sptr->s, &fdexcept);

			sptr = sptr->next;
		}
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		rc = select(0, &fdread, &fdwrite, &fdexcept, &timeout);

		if (rc == SOCKET_ERROR)
		{
			fprintf(stderr, "select failed: %d\n", WSAGetLastError());
			return -1;
		}
		else if (rc == 0)
		{
			// timeout
			PrintStatistics();
		}
		else
		{
			sptr = gSocketList;
			while (sptr)
			{
				if (FD_ISSET(sptr->s, &fdread))
				{
					if (sptr->listening)
					{
						sockobj = GetSocketObj(INVALID_SOCKET, FALSE);
						s = accept(sptr->s, (SOCKADDR*)&sockobj->addr, &sockobj->addrlen);
						if (s == INVALID_SOCKET)
						{
							fprintf(stderr, "accept failed: %d\n", WSAGetLastError());
							return -1;
						}

						InterlockedIncrement(&gCurrentConnections);
						sockobj->s = s;

						InsertSocketObj(sockobj);
					}
					else
					{
						if (ReceivePendingData(sptr) != 0)
						{
							printf("ReceivePendingData indicated to remove obj\n");
							tmp = sptr;
							sptr = sptr->next;

							RemoveSocketObj(tmp);
							FreeSocketObj(tmp);

							if (sptr == NULL)
								continue;
						}
						if (SendPendingData(sptr) != 0)
						{
							tmp = sptr;
							sptr = sptr->next;

							RemoveSocketObj(tmp);
							FreeSocketObj(tmp);

							if (sptr == NULL)
								continue;
						}
					}
				}
				if (FD_ISSET(sptr->s, &fdwrite))
				{
					// Write is indicated so attempt to send the pending data
					if (SendPendingData(sptr) != 0)
					{
						tmp = sptr;
						sptr = sptr->next;

						RemoveSocketObj(tmp);
						FreeSocketObj(tmp);

						// At the end of the list
						if (sptr == NULL)
							continue;
					}
				}
				if (FD_ISSET(sptr->s, &fdexcept))
				{
					// Not handling OOB data so just close the connection
					tmp = sptr;
					sptr = sptr->next;

					RemoveSocketObj(tmp);
					FreeSocketObj(tmp);

					// At the end of the list
					if (sptr == NULL)
						continue;
				}
				sptr = sptr->next;
			}
		}

		// See if we should print statistics
		if ((GetTickCount() - lastprint) > 5000)
		{
			PrintStatistics();
			lastprint = GetTickCount();
		}
	}

	WSACleanup();
	return 0;
}
