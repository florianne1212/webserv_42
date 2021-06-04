#include "ASocket.hpp"

ASocket::ASocket(void){}

ASocket::ASocket(int fd) : _fd(fd), _readStatus(false), _writeStatus(false)
{
}

ASocket::~ASocket(){}

ASocket::ASocket(const ASocket & other) :
	_fd(other._fd),
	_readStatus(other._readStatus),
	_writeStatus(other._writeStatus)
{}

ASocket & ASocket::operator=(const ASocket & other){
	if (this != &other)
	{
		_fd = other._fd;
		_readStatus = other._readStatus;
		_writeStatus = other._writeStatus;
	}
	return (*this);
}

bool ASocket::getReadStatus(void) const {
	return (_readStatus);
}

bool ASocket::getWriteStatus(void) const {
	return (_writeStatus);
}
void ASocket::setReadStatus(bool status) {
	_readStatus =status;
}

void ASocket::setWriteStatus(bool status) {
	_writeStatus = status;
}