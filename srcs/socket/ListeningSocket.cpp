#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int fd) : ASocket(fd){}

ListeningSocket::~ListeningSocket(){}

ListeningSocket::ListeningSocket(const ListeningSocket & other) : ASocket(){
	*this = other;
}

ListeningSocket & ListeningSocket::operator=(const ListeningSocket & other){
	if (this != &other)
	{
		ASocket::operator=(other);
	}
	return (*this);
}

int	ListeningSocket::getFd(void) const{
	return (_fd);
}

void ListeningSocket::read(Config *datas, FDList *listFD)
{
	(void)datas;
	socklen_t i = 0;

	std::cout << "adding client" << std::endl;
	int fd = accept(_fd, NULL, &i);
	ClientSocket *tmp = new ClientSocket(fd);
	listFD->addSocket(tmp);
}

void ListeningSocket::write(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
}