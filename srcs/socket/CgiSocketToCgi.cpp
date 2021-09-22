#include "CgiSocketToCgi.hpp"

CgiSocketToCgi::CgiSocketToCgi(int fd, Request & request, ClientSocket * client): ASocket(fd, ""), _request(request), _client(client) {
	clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
	_pollFD.fd = fd;
	_pollFD.events = POLLOUT;
}

CgiSocketToCgi::~CgiSocketToCgi(){
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

	try
	{
		size_t writeResult;
		if ((writeResult = ::write(_fd, _request.getBody().c_str(), _request.getBody().length())) < 0)
			throw std::runtime_error("error while writing to CGI");
		if (_client->getCloseToCgi())
		{
			close(_client->getCgiFdValue(2));
			close(_client->getCgiFdValue(3));
			_client->setCgiFd(2, 0);
			_client->setCgiFd(3, 0);
			_client->setCloseToCgi(false);
			_client->getListFD()->rmSocket(_fd);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
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