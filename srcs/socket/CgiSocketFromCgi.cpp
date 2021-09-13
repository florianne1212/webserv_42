#include "CgiSocketFromCgi.hpp"

CgiSocketFromCgi::CgiSocketFromCgi(int fd[2], ClientSocket * client, Response * response): ASocket(fd[0], ""), _state(0), _response(response), _client(client), _otherFdToClose(fd[1]) {
	clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
	_pollFD.fd = fd[0];
	_pollFD.events = POLLIN;
	_compteur = 0;
	std::cout << "ON VIENT DE CREER SOCKET FROM CGI le fd est " << _fd <<  "le fd du client associe est "<< _client->getFd() << "\n";
}

CgiSocketFromCgi::~CgiSocketFromCgi(){

		std::cout << "ON VIENT DE detruire SOCKET FROM CGI le fd est " << _fd <<  "le fd du client associe est "<< _client->getFd() << "\n";

}

int	CgiSocketFromCgi::getFd(void) const{
	return (_fd);
}

void CgiSocketFromCgi::read(Config *datas, FDList *listFD)
{
// 	if ( _compteur > 0)
// 		exit (1);
	_cgiState = FROM_CGI_IN_PROGRESS;
	(void)datas;
	(void)listFD;
	// std::cout << "ON PASSE DANS LE READ\n";
	std::string cgiResponse;
	// std::string cgiHeaders;
	char buf[80001] = {0};
		// char buf[100001] = {0};
	// std::cout << "\n||||||||||||||||||||||||||||||||| le compteur est a " << _compteur++ << "\n|||||||||||||||||||||||||\n\n";
	// _compteur++;
	ssize_t readResult;
	bool sended = false;
	readResult = ::read(_fd, buf, 80000);
	// std::cout << "readresult = " << readResult << " compteur : " << _compteur << "\n";
	// exit(1);
 	// if((readResult = ::read(_fd, buf, 2048)) > 0)//et snded = false a rtravailler ++
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
		// std::cout << "state = " << _state << "\n";
		if (_state == 4)///on vient juste de passer le header et la fin du buffer
		{
			_cgiHeaders = cgiResponseHeaderPreparation(_cgiHeaders);
			_state +=1;
			std::cout << "----------------------------------------------------COUCOU1\n";
			readResult = cgiResponse.length();
			cgiResponse = cgiResponseChunkedPreparation(cgiResponse, readResult);
			// std::cout << "le debut de la reponse est : \n" << _cgiHeaders + cgiResponse << "\n\n\n";
			_client->getResponse().setCgiResponse(_cgiHeaders + cgiResponse);
			// _response->setCgiResponse(_cgiHeaders + cgiResponse);
			// _response->_cgiResponse = true;
			//envoi du header et du 1er buffer
			_cgiHeaders.clear();
			sended = true;
			// exit(1);
		}
		else if (_state > 4) //le header a deja ete envoye
		{
			// exit(1);
						std::cout << "----------------------------------------------------COUCOU2\n";

			cgiResponse = cgiResponseChunkedPreparation(cgiResponse, readResult);
			// _response->setCgiResponse(cgiResponse);
			// _response->_cgiResponse = true;
			// std::cout << "la suite de la reponse est : \n" << cgiResponse << "\n\n\n";

			_client->getResponse().setCgiResponse(cgiResponse);
			// std::cout << "\ndans ma variable response, le cgi response est " << _client->getResponse().getCgiResponse() << "\n\n";
			sended = true;
		}
		else if (_state < 4)
			return;//le cgiHeaders n'est pas encore complet
	}
	if (sended == false) //ie le read = 0 ou error
	{
		cgiResponse = cgiResponseChunkedPreparation("", 0);
		std::cout << "-----------YOUPI le read est a 0 -----------------------------------\n";
		std::cout <<"on est dans cgisocketfromcgi avec le fd "<< _fd << " et on le ferme\n";

		// _response->setCgiResponse(cgiResponse);
		// _response->_cgiResponse = true;
		_client->getResponse().setCgiResponse(cgiResponse);
		// _client.getResponse()->_cgiResponse = true;
		close (_fd);
		close (_otherFdToClose);
		// std::cout << "--------------------MAMA MIA----------------------\n";
		_client->getListFD()->rmSocket(_fd);
		// std::cout <<"on est dans cgisocketfromcgi avec le fd "<< _fd << " et on le ferme\n";

	}
	// std::cout << "\n\n FIN DU READ DE CGISOCKET \n\n";
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
		std::cout << "on passe dans preparecgiend\n";
		_compteur = 0;
		_client->getResponse().setCgiResponse("0\r\n\r\n");
		// _client->setCgiState(CGI_DONE);
		_cgiState = FROM_CGI_DONE;
		_state = 0;

	}
	else
	{
		// _compteur++;
		// std::cout << " mon compteur est a " << _compteur << "\n\n";
		std::cout << "il faut detruire les cgisockets\n";
		// _client->destroyCgiSockets();
		// if (_compteur > 2)
			_cgiState = NO_CGI;
			_client->setCgiState(NO_CGI);
			// exit(1);
	}
}
