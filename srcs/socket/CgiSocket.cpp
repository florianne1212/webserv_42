#include "CgiSocket.hpp"

CgiSocket::CgiSocket(int fd): ASocket(fd, "") {}

CgiSocket::~CgiSocket(){}

int	CgiSocket::getFd(void) const{
	return (_fd);
}

void CgiSocket::read(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
}

void CgiSocket::write(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
}

bool CgiSocket::getTimeout()
{
	return false;
}

void CgiSocket::setTime(){}