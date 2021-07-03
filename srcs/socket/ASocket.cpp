#include "ASocket.hpp"

ASocket::ASocket(void){}

ASocket::ASocket(int fd) : _fd(fd), _pollFD()
{
	_pollFD.fd = fd;
	_pollFD.events = POLLIN;
}

ASocket::~ASocket(){}

ASocket::ASocket(const ASocket & other) :
	_fd(other._fd),
	_pollFD(other._pollFD)
{}

ASocket & ASocket::operator=(const ASocket & other){
	if (this != &other)
	{
		_fd = other._fd;
		_pollFD = other._pollFD;
	}
	return (*this);
}

bool ASocket::getReadStatus(void) const {
	return (_pollFD.revents == POLLIN);
}

bool ASocket::getWriteStatus(void) const {
	return (_pollFD.revents == POLLOUT);
}

struct pollfd ASocket::getPollFD(void) const{
	return (_pollFD);
}

void ASocket::setPollFD(struct pollfd toSet)
{
	_pollFD = toSet;
}