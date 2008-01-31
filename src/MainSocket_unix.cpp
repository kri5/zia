#include "MainSocket_unix.h"

MainSocket::MainSocket(int queue, std::string adress, int port) : Socket()
{
	bind(adress, port);
	listen(queue);
	_port = port;
	_adress = adress;
}

MainSocket::~MainSocket()
{
	this->close(true);
}

void	MainSocket::bind(std::string adress, int port) const
{
	struct sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(adress.c_str());
	service.sin_port = htons(port);

	if (::bind(listenSocket, (struct sockaddr *)&service, sizeof(service)) == SOCKET_ERROR)
	{
		throw 0;
	}
}

void	MainSocket::listen(int queue) const
{
	if (::listen(listenSocket, queue) == SOCKET_ERROR)
	{
		throw 0;
	}
}

ClientSocket*	MainSocket::accept()
{
	int acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == SOCKET_ERROR)
	{
		throw 0;
	}
	ClientSocket *ret = new ClientSocket(acceptSocket);
	return (ret);
}

int		MainSocket::recv(char *buf, int length) const
{
	; //FIXME
}


