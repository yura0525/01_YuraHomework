#include "TChatAccept.h"
#include "TChatSvr.h"

void TChatAccept::AddUser(SOCKET socket, SOCKADDR_IN address)
{
	TAcceptor::AddUser(socket, address);
	I_Server.AddUser(socket, address);
}

TChatAccept::TChatAccept()
{
}


TChatAccept::~TChatAccept()
{
}
