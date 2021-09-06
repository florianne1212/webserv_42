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
	struct timespec act;
	double time_taken;

	clock_gettime(CLOCK_MONOTONIC, &act);
	time_taken = (act.tv_sec - _lastInterTime.tv_sec) * 1e9;
    time_taken = (time_taken + (act.tv_nsec - _lastInterTime.tv_nsec)) * 1e-9;
	return (time_taken > 30);
}

void CgiSocket::setTime()
{
		clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
}