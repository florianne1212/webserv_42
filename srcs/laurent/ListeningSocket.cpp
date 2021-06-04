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