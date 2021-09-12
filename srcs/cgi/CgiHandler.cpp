#include "CgiHandler.hpp"
#include <sstream>
/*
** -------------------------------------------------------------------------------
** ----------------------- COPLIEN FORM ------------------------------------------
** -------------------------------------------------------------------------------
*/

CgiHandler::CgiHandler(ClientSocket* client, Config & config, Request & request, Response* response):
	_vectorEnv(0), _varEnv(0), _instructionsCGI(0), _client(client),_config(config), _request(request), _response(response)
{
	_headers = request.getHeaders();
	_parsedUrl = parseTheUri(request.getUrl());
	parsePathforCgi();//pour scriptname et additionnal path
	client->setCgiState(CGI_IN_PROGRESS);
}

CgiHandler::~CgiHandler()
{
	if (_varEnv)
	{
		int i = -1;
		while (_varEnv[++i])
			free (_varEnv[i]);
		delete [] _varEnv;
	}
	if (_instructionsCGI)
	{
		int j = -1;
		while (_instructionsCGI[++j])
			free (_instructionsCGI[j]);
		delete [] _instructionsCGI;
	}
	// std::cout << "--------------fermeture CGI handler-----------------\n";
}

CgiHandler::CgiHandler(CgiHandler const & other):
	_vectorEnv(other._vectorEnv), _varEnv(other._varEnv), _instructionsCGI(other._instructionsCGI),
	_client(other._client),_config(other._config), _request(other._request),_response(other._response), _parsedUrl(other._parsedUrl), _headers(other._headers),
	_pathForExec(other._pathForExec)
	{}

CgiHandler & CgiHandler::operator= (const CgiHandler & other)
{
	if (this != &other)
	{
		if (_varEnv)
		{
			int i = -1;
			while (_varEnv[++i])
				free (_varEnv[i]);
			delete [] _varEnv;
		}
		if (_instructionsCGI)
		{
			int j = -1;
			while (_instructionsCGI[++j])
				free (_instructionsCGI[j]);
			delete [] _instructionsCGI;
		}
		_vectorEnv = other._vectorEnv;
		_varEnv = other._varEnv;
		_instructionsCGI = other._instructionsCGI;
		_request = other._request;
		_client = other._client;
		_config = other._config;
		_response = other._response;
		_parsedUrl = other._parsedUrl;
		_headers = other._headers;
		_pathForExec = other._pathForExec;
	}
	return (*this);

}

/*
** -------------------------------------------------------------------------------
** ------------------- PUBLIC FUNCTIONS ------------------------------------------
** -------------------------------------------------------------------------------
*/

void CgiHandler::executeCgi(void){
	creationVectorEnviron();
	if(_response->getStatus()/100 == 2)
	{
		setVarEnv();
		setInstructionCgi();
		executingCgi();
	}
	else
		_client->setCgiState(NO_CGI);
}

/*
** -------------------------------------------------------------------------------
** ------------------- PRIVATE FUNCTIONS -----------------------------------------
** -------------------------------------------------------------------------------
*/
void CgiHandler::setInstructionCgi(void)
{
	if (!(_instructionsCGI = new char*[3]))
		throw std::runtime_error("error allocation setting CGI instructions");
	char* buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		throw std::runtime_error("error during getcwd");
	std::string toExec = static_cast<std::string>(buf) + "/" + WORKPATH;
	toExec = toExec + _config.getCGI().value.second;
	free (buf);
	_instructionsCGI[0] = strdup(toExec.c_str());
	_instructionsCGI[1] = strdup(_pathForExec.c_str());
	_instructionsCGI[2] = NULL;
}


void CgiHandler::creationVectorEnviron(void){
	auth("Authorization");
	contentLength("Content-Length");
	contentType("Content-Type");
	pathInfo(_parsedUrl["additionnal_path"]);
	queryString(_parsedUrl["query"]);
	remoteAddr(_client->getClientAddress());
	remoteHost();
	remoteUser(_parsedUrl["user_name"]);
	scriptName(_parsedUrl["cgi_path"]);//checke existence de lexecutable
	serverPort(_parsedUrl["port"]);
	serverProtocol();
	otherMetaVariables();
	redirectStatus();
	checkIfPhpCgi();
}

void CgiHandler::checkIfPhpCgi(void)
{
	//resolve php-cgi bug
	std::string verif;
	size_t found = (_config.getCGI().value.second).find_last_of("/");
	if (found != std::string::npos)
		verif = (_config.getCGI().value.second).substr(found + 1);
	std::cout << verif << "=verif\n";
	// if(verif == "php-cgi")
	// 	return;
	serverName(_parsedUrl["host"]);
	serverSoftware();
	gatewayInterface();
	requestMethod(_request.getMethods());

}

void CgiHandler::setVarEnv(void){
	size_t len = _vectorEnv.size();

	if ((_varEnv = new char*[len + 1]) == NULL)
		throw std::runtime_error("error allocation setting CGI environnement variables");
	for (size_t i = 0; i < len; i++)
		_varEnv[i] = strdup(_vectorEnv[i].c_str());
	_varEnv[len] = NULL;
}

void CgiHandler::executingCgi(void)
{
	int fdPipeIn[2];
	int fdPipeOut[2];
	pid_t pid;

	// pere     |     fils
	//	  In  1-|->0
	//   Out 0<-|-1

	if (pipe(fdPipeOut) < 0)
		throw std::runtime_error("error piping CGI");
	// if (_request.getMethods() == "POST")
	// {
	if (pipe(fdPipeIn) < 0)
	{
		close (fdPipeOut[0]);
		close (fdPipeOut[1]);
		throw std::runtime_error("error piping CGI");
	}
	// }
	pid = fork();
	if (pid < 0) //error
	{
		close(fdPipeOut[0]);
		close(fdPipeOut[1]);
		// if (_request.getMethods() == "POST")
		// {
		close (fdPipeIn[0]);
		close (fdPipeOut[1]);
		// }
		throw std::runtime_error("error forking CGI");
	}

	else if (!pid)// processus fils
	{
		close(fdPipeOut[0]);
		if (dup2(fdPipeOut[1], STDOUT_FILENO) < 0)
			std::cerr << "error with dup2 in CGI son\n";
		close(fdPipeOut[1]);

		// if (_request.getMethods() == "POST")
		// {
		close(fdPipeIn[1]);
		if(dup2(fdPipeIn[0], STDIN_FILENO) < 0)
			std::cerr << "error with dup2 in CGI son\n";
		close(fdPipeIn[0]);
		// }

		chdir((_pathForExec.substr(0, _pathForExec.find_last_of("/")).c_str())); //on se met dans le bon repertoire pour execve
		// char* buf = NULL; ////a retirer
		// buf = getcwd(buf, 0);////a retirer
		// std::cerr << " nous sommes dans : " << buf << "\n";///a retirer
		// std::cerr << " arg[0] : " << _instructionsCGI[0] << "\n";///a retirer
		// std::cerr << " arg[1] : " << _instructionsCGI[1] << "\n";///a retirer

 		if (execve(_instructionsCGI[0], _instructionsCGI, _varEnv)< 0)
			std::cerr << "error with CGI execution\n";
		exit(1);
	}

	else if (pid > 0) //pere
	{
		_client->setCgiFd(fdPipeOut[0], fdPipeOut[1], fdPipeIn[0], fdPipeIn[1]);
		// set fd as non blocking
		fcntl(fdPipeOut[0], F_SETFL, O_NONBLOCK);
		// close (fdPipeOut[1]);

		// struct pollfd out;
		// out.fd = fdPipeOut[0];
		// out.events = POLLIN | POLLOUT;
		// out.revents = 0;
		CgiSocketFromCgi* socketFromCgi = new CgiSocketFromCgi(fdPipeOut, _client, _response);
		// socketFromCgi->setPollFD(out);
		_client->getListFD()->addSocket(socketFromCgi);

		// if (_request.getMethods() == "POST")
		// {
			// // set fd as non blocking
										// fcntl(fdPipeIn[1], F_SETFL, O_NONBLOCK);
										// // close (fdPipeIn[0]);
										// // out.fd = fdPipeIn[1];
										// CgiSocketToCgi* socketToCgi = new CgiSocketToCgi(fdPipeIn[1], _request, _client);
										// // socketToCgi->setPollFD(out);
										// _client.getListFD()->addSocket(socketToCgi);
										_response->_cgiResponse = true;
		// while (!(socketToCgi->getPollFD().revents & POLLOUT))
		// {
		// 	usleep(100000); //on attend 100 msec avant nouvelle tentative
		// 	_client.getListFD()->myPoll();
		// 	if (socketToCgi->getPollFD().revents & POLLERR)
		// 		throw std::runtime_error("error while polling CGI");
		// }
		//la partie qui write le body pour le cgi
		// if (write(fdPipeIn[1], _request.getBody().c_str(), _request.getBody().length()) < 0)
		// 	throw std::runtime_error("error while writing to CGI");
		// _client.getListFD()->rmSocket(fdPipeIn[1]);
		// close (fdPipeIn[1]);

		// }

		// while (!(socketFromCgi->getPollFD().revents & POLLIN))
		// {
		// 	usleep(100000); //on attend 100 msec avant nouvelle tentative
		// 	_client.getListFD()->myPoll();
		// 	if (socketFromCgi->getPollFD().revents & POLLERR)
		// 		throw std::runtime_error("error while polling CGI");
		// }

		// std::string cgiResponse;
		// std::string cgiHeaders;
		// char buf[2049] = {0};
		// ssize_t readResult;
		// int state = 0;///onmonte jusqu'a 4 pour trouver la fin du header attention a mettre dans une structure qui reviendra
		// bool sended = false;

		//ATTENTION A PRIORI A TERME PAS DE WHILE MAIS UN IF
		// if((readResult = read(fdPipeOut[0], buf, 2048)) > 0)//et snded = false a rtravailler ++

		// while ((readResult = read(fdPipeOut[0], buf, 2048)) > 0)//et snded = false a rtravailler ++
		// {
		// 	int i = 0;
		// 	char c;
		// 	while (buf[i])
		// 	{
		// 		c = buf[i];
		// 		if (state < 4)
		// 		{
		// 			state = checkHeaders(c, state);//on cree le header
		// 			cgiHeaders += c;
		// 		}
		// 		else
		// 			cgiResponse += c;
		// 		i++;
		// 	}
			// if (state == 4)///on vient juste de passer le header et la fin du buffer
			// {
			// 	cgiHeaders = cgiResponseHeaderPreparation(cgiHeaders);
			// 	state +=1;
			// 	readResult = cgiResponse.length();
			// 	cgiResponse = cgiResponseChunkedPreparation(cgiResponse, readResult);
			// 	_response->setCgiResponse(cgiHeaders + cgiResponse);
			// 	_response->_cgiResponse = true;
			// 	//envoi du header et du 1er buffer
			// 	sended = true;
			// }
		// 	else if (state > 4) //le header a deja ete envoye
		// 	{
		// 		cgiResponse = cgiResponseChunkedPreparation(cgiResponse, readResult);
		// 		_response->setCgiResponse(cgiResponse);
		// 		_response->_cgiResponse = true;
		// 		sended = true;
		// 	}
		// 	memset(buf, 0, 2049);
		// }
		// if (sended == false) //ie le read = 0 ou error
		// {
		// 	cgiResponse = cgiResponseChunkedPreparation("", 0);
		// 	_response->setCgiResponse(cgiResponse);
		// 	_response->_cgiResponse = true;
		// 	_client.getListFD()->rmSocket(socketFromCgi->getPollFD().fd);
		// 	close (fdPipeOut[1]);
		// 	close (fdPipeOut[0]);
		// }
		// //sinon on ne clot rien
		// std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\nla reponse du cgi est : \n" << cgiResponse
		// << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
	}
}

void CgiHandler::auth(const std::string & str)
{
	std::map<std::string, std::string>::iterator it;
	std::string value = "";

	if ((it = _headers.find(str)) != _headers.end())
	{
		value = it->second;
		_headers.erase(it);
	}
	_vectorEnv.push_back("AUTH_TYPE=" + value);
}

void CgiHandler::contentLength(const std::string & str)
{
	std::map<std::string, std::string>::iterator it;
	std::string value = "";

	if ((it = _headers.find(str)) != _headers.end())
	{
		value = it->second;
		_headers.erase(it);
	}
	_vectorEnv.push_back("CONTENT_LENGTH=" + value);
}


void CgiHandler::contentType(const std::string & str)
{
	std::map<std::string, std::string>::iterator it;
	std::string value = "";

	if ((it = _headers.find(str)) != _headers.end())
	{
		value = it->second;
		_headers.erase(it);
	}
	_vectorEnv.push_back("CONTENT_TYPE=" + value);
}


void CgiHandler::gatewayInterface(void)
{
	_vectorEnv.push_back("GATEWAY_INTERFACE=CGI/1.1");
}

void CgiHandler::pathInfo(const std::string & str)
{
	//mise en place chemin absolu
	char* buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		throw std::runtime_error("error during getcwd");
	std::string s2 = static_cast<std::string>(buf) + "/" + WORKPATH + str + _parsedUrl["cgi_path"];
	// std::cout << "path info : " << s2 << std::endl;
	_vectorEnv.push_back("PATH_INFO=" + s2);
	_pathForExec = s2;
	free (buf);
}

void CgiHandler::queryString(const std::string & str) {
	if (str != "")
		_vectorEnv.push_back("QUERY_STRING=" + str);
	else
		_vectorEnv.push_back("QUERY_STRING=" + _request.getParsedUri()["query"]);
}

void CgiHandler::remoteAddr(const std::string & str)
{
	_vectorEnv.push_back("REMOTE_ADDR=" + str);
}

void CgiHandler::remoteHost(void)
{
	std::string s = _client->getClientAddress();
	_vectorEnv.push_back("REMOTE_HOST=" + s);
}

void CgiHandler::remoteUser(const std::string & str)
{
	_vectorEnv.push_back("REMOTE_USER=" + str);
}

void CgiHandler::requestMethod(const std::string & str)
{
	if (str == "GET" || str == "POST")
		_vectorEnv.push_back("REQUEST_METHOD=" + str);
	else
		throw std::runtime_error("method not supported by webserv");
}

bool CgiHandler::scriptName(std::string & str)
{
	_vectorEnv.push_back("SCRIPT_NAME=" + str);
	checkExecutableExistence(str);
	return true;
}


void CgiHandler::serverName(const std::string & str)
{
	if (str != "")
		_vectorEnv.push_back("SERVER_NAME=" + str);
	else
		_vectorEnv.push_back("SERVER_NAME=" + _client->getServerName());
}

void CgiHandler::serverPort(const std::string & str)
{
	if (str == "")
	{
		unsigned short port = _config.getPort(_client->getServerName());
		std::ostringstream s;

		s << port;
		std::string str2 = s.str();
		_vectorEnv.push_back("SERVER_PORT=" + str2);
	}
	else
		_vectorEnv.push_back("SERVER_PORT=" + str);
}

void CgiHandler::serverProtocol(void)
{
	_vectorEnv.push_back("SERVER_PROTOCOL=HTTP/1.1");
}

void CgiHandler::serverSoftware(void)
{
	_vectorEnv.push_back("SERVER_SOFTWARE=DREAMTEAM/42.42");
}

void CgiHandler::otherMetaVariables(void)
{
	std::map<std::string, std::string>::iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it)
	{
		std::string str = "HTTP_" + upperCaseAndMinus(it->first);
		str = str + "=" + it->second;
		_vectorEnv.push_back(str);
	}
	_headers.clear();
}

//apparemment necessaire pour php-cgi
void CgiHandler::redirectStatus(void)
{
	_vectorEnv.push_back("REDIRECT_STATUS=200");
}

std::string CgiHandler::upperCaseAndMinus(const std::string & str)
{
	std::string newstr(str);
	for (unsigned long i = 0; i < newstr.length(); i++)
	{
		if (newstr[i] > 96 && newstr[i] < 123)
			newstr[i] -= 32;
		if (newstr[i] == 45)
			newstr[i] = 95;
	}
	return (newstr);
}

bool CgiHandler::checkExecutableExistence(std::string const & str)
{
	std::string str1 = WORKPATH + str;
	struct stat st;
	if (stat(str1.c_str(), &st) == -1)
	{
		_response->setStatus(404);
		return (false);
	}
	return true;
}

void CgiHandler::parsePathforCgi(void)
{
//on recherche si script.php
	size_t found = _parsedUrl["path"].find(_config.getCGI().value.first);
	if (found != std::string::npos)
	{
		//on delimite le path du cgi et le additionnal path
		std::string cgiPath = _parsedUrl["path"].substr(0, found + 4);
		UrlDecoder(cgiPath);
		found = _parsedUrl["path"].find_first_of("/", found + 4);
		if (found != std::string::npos)
		{
			std::string additionnalPath = _parsedUrl["path"].substr(found);
			UrlDecoder(additionnalPath);
			_parsedUrl.insert(std::make_pair("additionnal_path", additionnalPath));
		}
		else
			_parsedUrl.insert(std::make_pair("additionnal_path", ""));
		//on met le "real path" en fonction de "location  de - \.php"
		// std::string root =  "";//donnees a recuperer de config
		// cgiPath = root + "/" + cgiPath;
		// additionnalPath = root + "/" + additionnalPath;
		_parsedUrl.insert(std::make_pair("cgi_path", cgiPath));
	}
}

/*
** ------------------ UrlDecoder --------------------------------------
**
** fonction qui transforme un string URL encoded en string URL decoded
** les signes + sont remplaces par des espaces
** les caracteres speciaux codes en % hex hex sont remplaces par leur valeur
*/
void UrlDecoder(std::string & str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '+')
			str[i] = ' ';
		if (str[i] == '%' && (i < (str.length() - 2)))
		{
			if (isxdigit(str[i + 1]) && (isxdigit(str[i + 2])))
			{
				str[i] = static_cast<char>(strtol((str.substr(i + 1, 2)).c_str(), 0, 16));
				str = str.erase(i + 1, 2);
			}
		}
	}
}


// ---------------------------------------------------------------------------------
//------------------------- map parseTheUri(string)---------------------------------
//----------------------------------------------------------------------------------

//scheme-- : ----------------------------------------path---------------------------
//				\									/	 \			/  \			/
//					//------------host------------			? query		#fragment
//					\			/		\		/
//					  user_info@		: port

//parsing simple de l'URL
//renvoie une map avec chacun des elements

std::map<std::string, std::string> parseTheUri(std::string url)
{
	size_t found;
	std::map<std::string, std::string> parsedUrl;
	std::string userinfo;

//on cherche si scheme et on l'extrait, http sinon
	found = url.find("//");
	if (found != url.npos)
	{
		found = url.find_first_of(":");
		if (found != url.npos)
		{
			parsedUrl.insert(std::make_pair("scheme", url.substr(0, found)));
			url = url.substr(found + 1);
		}
	}
	else
		parsedUrl.insert(std::make_pair("scheme", "http"));

//on cherche le fragment
	found = url.find_first_of("#");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("fragment", url.substr(found + 1)));
		url = url.substr(0, found);
	}

//on cherche le query
	found = url.find_first_of("?");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("query", url.substr(found + 1)));
		url = url.substr(0, found);
	}

//on cherche si authority
	if (url.substr(0, 2) == "//" || url.substr(0, 1) != "/") //il existe authority, a parser,
	{
		if (url.substr(0, 2) == "//")
			url = url.substr(2);
		found = url.find_first_of("/");//on separe le chemin
		if (found == url.npos)
		{
			std::cout << "probleme : pas de path trouve" << std::endl;
			parsedUrl.clear();
			return (parsedUrl);
		}
		parsedUrl.insert(std::make_pair("path", url.substr(found)));
		url = url.substr(0, found); //ne reste plus que le segment authorisation
		found = url.find_first_of("@");//on cherche si userinfo
		if (found != url.npos)
		{
			userinfo = url.substr(0, found);
			url =url.substr(found + 1);
			// on parse userinfo
			found = userinfo.find_first_of(":");
			if (found != url.npos)
			{
				parsedUrl.insert(std::make_pair("password", userinfo.substr(found + 1)));
				userinfo = userinfo.substr(0, found);
			}
			parsedUrl.insert(std::make_pair("user_name", userinfo.substr(0,found)));
		}
		// reste plus que host et port (attention , ne gere pas ipv6)
		found = url.find_first_of(":");//on cherche si port
		if (found != url.npos)
		{
			parsedUrl.insert(std::make_pair("port", url.substr(found + 1)));
			parsedUrl.insert(std::make_pair("host", url.substr(0, found)));
		}
		else
		{
			parsedUrl.insert(std::make_pair("port", ""));
			parsedUrl.insert(std::make_pair("host", url));
		}
	}
	else
	{
		parsedUrl.insert(std::make_pair("path", url));
	}
	return (parsedUrl);
}

void CgiHandler::visualizeEnviron(void) const {
	std::cout << "<<<<<<<<<<<<< MES VARIABLES ENVIRONNEMENT CGI >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	std::vector<std::string>::const_iterator it;
	for (it = _vectorEnv.begin(); it != _vectorEnv.end(); it++)
		std::cout << *it <<std::endl;
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n";
}

std::string & CgiHandler::checkCgiResponse(std::string & response)
{
	if (response == "")
		return (response);
	std::string body;
	size_t pos = response.find("\r\n\r\n");
	if (pos == response.npos)
	{
		pos = response.find("\n\n");
		if (pos != response.npos)
			body = response.substr(pos + 2);
	}
	else
		body = response.substr(pos + 4);
	std::string header = response.substr(0, pos);
	std::cout << "header = " << header << "\n";
	if (header.substr(0, 4) != "HTTP")
		header = "HTTP/1.1 \r\n" + header;
	size_t bodySize = body.length();
	if (header.find("Content-Length:") == header.npos && bodySize)
	{
		std::ostringstream s;
		s << bodySize;
		std::string str2 = s.str();
		header = header + "\r\nContent-Length: " + s.str();
	}
	return (response = header + "\r\n\r\n" + body);
}

// int CgiHandler::checkHeaders(char c, int state)
// {
// 	if ((c == '\r') && ((state % 2) == 0))
// 		state ++;
// 	else if ((c == '\n') && ((state % 2) == 1))
// 		state ++;
// 	else
// 		state = 0;
// 	return (state);
// }

// std::string CgiHandler::cgiResponseHeaderPreparation(std::string & cgiHeaders)
// {
// 	//on checke si HTTP
// 	size_t found;
// 	size_t found2;
// 	std::string firstLine;
// 	if (!(cgiHeaders.substr(0, 4) == "HTTP"))
// 		//on checke si status et on cree la premiere ligne
// 	{
// 		std::string status;
// 		if ((found = cgiHeaders.find("Status:")) != cgiHeaders.npos) //il existe une ligne status
// 		{
// 			found2 = cgiHeaders.find("\r\n", found);
// 			status = cgiHeaders.substr(found + 7, found2 - found - 7);
// 			cgiHeaders = cgiHeaders.erase(found, 9 + status.length());
// 		}
// 		firstLine = "HTTP/1.1" + status + "\r\n";
// 	}
// 	else
// 	{
// 		firstLine = cgiHeaders.substr(0, (cgiHeaders.find("\r\n") + 2));
// 		cgiHeaders = cgiHeaders.erase(0, firstLine.length());
// 	}
// 	// on check si Content-Length ou Transfer-Encoding
// 	//si non existant on cree un header Transfer-Encoding: chunked
// 	std::string encoding;
// 	if ((cgiHeaders.find("Content-Length") == cgiHeaders.npos) && (cgiHeaders.find("Transfer-Encoding") == cgiHeaders.npos))
// 		encoding = "Transfer-Encoding: chunked\r\n";
// 	return (firstLine + encoding + cgiHeaders);
// }

// std::string CgiHandler::cgiResponseChunkedPreparation(std::string cgiResponse, int readResult)
// {
// 	 return(itoaBase16(readResult) + "\r\n" + cgiResponse + "\r\n");
// }

// std::string itoaBase16(size_t num)
// {
// 	std::string ret;
// 	if (num == 0)
// 		return ("0");
// 	while (num != 0)
// 	{
// 		size_t temp = num % 16;
// 		char unit = (temp > 9)? (temp - 10) + 'A' : temp + '0';
// 		ret.insert(ret.begin(), unit);
// 		num = num / 16;
// 	}
// 	return (ret);
// }


