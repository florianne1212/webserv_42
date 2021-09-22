#include "CgiSocketFromCgi.hpp"

CgiSocketFromCgi::CgiSocketFromCgi(int fd[2], ClientSocket * client, Response * response): ASocket(fd[0], ""), _state(0), _response(response), _client(client), _otherFdToClose(fd[1]), _contentLengthPresent(false){
	clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
	_pollFD.fd = fd[0];
	_pollFD.events = POLLIN;
}

CgiSocketFromCgi::~CgiSocketFromCgi(){}

int	CgiSocketFromCgi::getFd(void) const{
	return (_fd);
}

void CgiSocketFromCgi::read(Config *datas, FDList *listFD)
{
	_cgiState = FROM_CGI_IN_PROGRESS;
	(void)datas;
	(void)listFD;

	std::string cgiResponse;
	char buf[80001] = {0};
	bool sended = false;

	ssize_t readResult = ::read(_fd, buf, 80000);
	if (readResult > 0)
	{
		int i = 0;
		char c;
		while (buf[i])
		{
			c = buf[i];
			if (_state < 4)
			{
				_state = checkHeaders(c, _state);//on cree le header
				_cgiHeaders += c;
			}
			else
				cgiResponse += c;
			i++;
		}
		if (_state == 4)///on vient juste de passer le header et la fin du buffer
		{
			_cgiHeaders = cgiResponseHeaderPreparation(_cgiHeaders);
			_state +=1;
			if (_contentLengthPresent == false)
			{
				readResult = cgiResponse.length();
				cgiResponse = cgiResponseChunkedPreparation(cgiResponse, readResult);
			}
			_client->getResponse().setCgiResponse(_cgiHeaders + cgiResponse);
			_cgiHeaders.clear();
			sended = true;
		}
		else if (_state > 4) //le header a deja ete envoye
		{
			if (_contentLengthPresent == false)
				cgiResponse = cgiResponseChunkedPreparation(cgiResponse, readResult);
			_client->getResponse().setCgiResponse(cgiResponse);
			sended = true;
		}
		else if (_state < 4)
			return;//le cgiHeaders n'est pas encore complet
	}
	if (sended == false) //ie le read = 0 ou error
	{
		cgiResponse = cgiResponseChunkedPreparation("", 0);
		_client->getResponse().setCgiResponse(cgiResponse);
		_client->setCgiState(NO_CGI);
		_contentLengthPresent = false;
		_client->destroyCgiSockets();
	}
}

void CgiSocketFromCgi::write(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
}

bool CgiSocketFromCgi::getTimeout()
{
	// struct timespec act;
	// double time_taken;

	// clock_gettime(CLOCK_MONOTONIC, &act);
	// time_taken = (act.tv_sec - _lastInterTime.tv_sec) * 1e9;
    // time_taken = (time_taken + (act.tv_nsec - _lastInterTime.tv_nsec)) * 1e-9;
	// return (time_taken > 30);
	return (false);
}

void CgiSocketFromCgi::setTime()
{
		clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
}

int CgiSocketFromCgi::checkHeaders(char c, int state)
{
	if ((c == '\r') && ((state % 2) == 0))
		state ++;
	else if ((c == '\n') && ((state % 2) == 1))
		state ++;
	else
		state = 0;
	return (state);
}

std::string CgiSocketFromCgi::cgiResponseHeaderPreparation(std::string & cgiHeaders)
{
	//on checke si HTTP
	size_t found;
	size_t found2;
	std::string firstLine;
	if (!(cgiHeaders.substr(0, 4) == "HTTP"))
		//on checke si status et on cree la premiere ligne
	{
		std::string status;
		if ((found = cgiHeaders.find("Status:")) != cgiHeaders.npos) //il existe une ligne status
		{
			found2 = cgiHeaders.find("\r\n", found);
			status = cgiHeaders.substr(found + 7, found2 - found - 7);
			cgiHeaders = cgiHeaders.erase(found, 9 + status.length());
		}
		firstLine = "HTTP/1.1" + status + "\r\n";
	}
	else
	{
		firstLine = cgiHeaders.substr(0, (cgiHeaders.find("\r\n") + 2));
		cgiHeaders = cgiHeaders.erase(0, firstLine.length());
	}
	// on check si Content-Length ou Transfer-Encoding
	//si non existant on cree un header Transfer-Encoding: chunked
	std::string encoding;
	if ((cgiHeaders.find("Content-Length") == cgiHeaders.npos) && (cgiHeaders.find("Transfer-Encoding") == cgiHeaders.npos))
		encoding = "Transfer-Encoding: chunked\r\n";
	if (cgiHeaders.find("Content-Length") != cgiHeaders.npos)
		_contentLengthPresent = true;
	return (firstLine + encoding + cgiHeaders);
}


std::string CgiSocketFromCgi::cgiResponseChunkedPreparation(std::string cgiResponse, int readResult)
{
	 return(itoaBase16(readResult) + "\r\n" + cgiResponse + "\r\n");
}


std::string CgiSocketFromCgi::itoaBase16(size_t num)
{
	std::string ret;
	if (num == 0)
		return ("0");
	while (num != 0)
	{
		size_t temp = num % 16;
		char unit = (temp > 9)? (temp - 10) + 'A' : temp + '0';
		ret.insert(ret.begin(), unit);
		num = num / 16;
	}
	return (ret);
}

void CgiSocketFromCgi::prepareCgiEnd()
{
	if (_cgiState == FROM_CGI_IN_PROGRESS)
	{
		_cgiState = FROM_CGI_DONE;
		_state = 0;
		if (_contentLengthPresent)
			return;
		_client->getResponse().setCgiResponse("0\r\n\r\n");
	}
	else
	{
		_cgiState = NO_CGI;
		_client->setCgiState(NO_CGI);
		_client->destroyCgiSockets();
	}
}
