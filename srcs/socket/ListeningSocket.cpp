#include "ListeningSocket.hpp"
#include "fcntl.h"

ListeningSocket::ListeningSocket(int fd, std::string serverName, sockaddr_in addr) : ASocket(fd, serverName), _addr(addr){}

ListeningSocket::~ListeningSocket(){}

ListeningSocket::ListeningSocket(const ListeningSocket & other) : ASocket(){
	*this = other;
}

ListeningSocket & ListeningSocket::operator=(const ListeningSocket & other){
	if (this != &other)
	{
		ASocket::operator=(other);
		_addr = other._addr;
	}
	return (*this);
}

int	ListeningSocket::getFd(void) const{
	return (_fd);
}

void ListeningSocket::read(Config *datas, FDList *listFD)
{
	(void)datas;

	std::cout << "adding client" << std::endl;

	struct sockaddr_in from;
	socklen_t addrlen = sizeof(_addr);
	std::string clientAddress;
	std::string clientPort;

	int newClientFd = accept(_fd, (struct sockaddr*)(&from), &addrlen);
	fcntl(newClientFd, F_SETFL, O_NONBLOCK);
	if (newClientFd != -1)
	{
		char buff[INET6_ADDRSTRLEN] = { 0 };

		clientAddress = inet_ntop(_addr.sin_family, (void*)&(_addr.sin_addr), buff, INET6_ADDRSTRLEN);
		std::cout << "Connexion de " << clientAddress << ":" << _addr.sin_port << std::endl;
	}
	listFD->addSocket(new ClientSocket(newClientFd, _serverName, clientAddress, clientPort, listFD));
}

void ListeningSocket::write(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
}

bool ListeningSocket::getTimeout()
{
	return (false);
}

void ListeningSocket::setTime()
{
}
