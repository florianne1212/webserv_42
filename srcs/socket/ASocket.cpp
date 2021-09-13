#include "ASocket.hpp"

ASocket::ASocket(void){}

ASocket::ASocket(int fd, std::string ServerName) : _fd(fd), _pollFD(), _serverName(ServerName), _cgiState(NO_CGI)
{
	_pollFD.fd = fd;
	_pollFD.events = POLLIN;
}

ASocket::~ASocket(){}

ASocket::ASocket(const ASocket & other) :
	_fd(other._fd),
	_pollFD(other._pollFD),
	_serverName(other._serverName),
	_cgiState(other._cgiState)
{}

ASocket & ASocket::operator=(const ASocket & other){
	if (this != &other)
	{
		_fd = other._fd;
		_pollFD = other._pollFD;
		_serverName = other._serverName;
		_cgiState = other._cgiState;
	}
	return (*this);
}

bool ASocket::getReadStatus(void) const {
	return (_pollFD.revents == POLLIN);
}

bool ASocket::getWriteStatus(void) const {
	return (_pollFD.revents == POLLOUT);
}

bool ASocket::endFromCgiStatus(void) const {
	return ((_cgiState == FROM_CGI_IN_PROGRESS || _cgiState == FROM_CGI_DONE) && (!(_pollFD.revents == POLLIN)));
}


struct pollfd ASocket::getPollFD(void) const{
	return (_pollFD);
}

std::string ASocket::getServerName() const{
	return _serverName;
}

void ASocket::setPollFD(struct pollfd toSet)
{
	_pollFD = toSet;
}

int ASocket::getcgiState()
{
	return (_cgiState);
}
void ASocket::setCgiState(int cgiState)
{
	_cgiState = cgiState;
}

void ASocket::prepareCgiEnd()
{}