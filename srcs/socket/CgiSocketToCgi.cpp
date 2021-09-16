#include "CgiSocketToCgi.hpp"

CgiSocketToCgi::CgiSocketToCgi(int fd, Request & request, ClientSocket * client): ASocket(fd, ""), _request(request), _client(client) {
	clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
	_pollFD.fd = fd;
	_pollFD.events = POLLOUT;
	std::cout << "ON VIENT DE CREER SOCKET TO CGI le sfd est " << _fd <<"\n";
}

CgiSocketToCgi::~CgiSocketToCgi(){
	// std::cout << "TROP TARD\n";
}

int	CgiSocketToCgi::getFd(void) const{
	return (_fd);
}

void CgiSocketToCgi::read(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
}

void CgiSocketToCgi::write(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
		// std::cout << "ON PASSE DANS LE WRITE\n";
	size_t writeResult;
	if ((writeResult = ::write(_fd, _request.getBody().c_str(), _request.getBody().length())) < 0)
		throw std::runtime_error("error while writing to CGI");
	// if (writeResult)
	// 	_cgiState = TO_CGI_IN_PROGRESS;
	// if (writeResult == 0 && _cgiState == TO_CGI_IN_PROGRESS) //plus rien a ecrire
	// {
	// 	close(_client->getCgiFdValue(2));
	// 	close(_client->getCgiFdValue(3));
	// 	_client->getListFD()->rmSocket(_fd);
	// 	_client->setCgiFd(2, 0);
	// 	_client->setCgiFd(3, 0);


		// std::cout << "ON est PASSE DANS LE WRITE\n";
	// }
	//voir avec Florianne si chunked Body
}

bool CgiSocketToCgi::getTimeout()
{
	// struct timespec act;
	// double time_taken;

	// clock_gettime(CLOCK_MONOTONIC, &act);
	// time_taken = (act.tv_sec - _lastInterTime.tv_sec) * 1e9;
    // time_taken = (time_taken + (act.tv_nsec - _lastInterTime.tv_nsec)) * 1e-9;
	// return (time_taken > 30);
	return (false);
}

void CgiSocketToCgi::setTime()
{
		clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
}