#include "cgiHandler.hpp"


cgiHandler::cgiHandler(const std::map<std::string, std::string> & floMap) : //donnees de tanguy
	_monDico(floMap), _varEnv(0), _instructionsCGI(0) {}

cgiHandler::~cgiHandler()
{
	if (_varEnv)
	{
		int i = -1;
		while (_varEnv[++i])
			delete (_varEnv[i]);
		delete [] _varEnv;
	}
	if (_instructionsCGI)
	{
		int j = -1;
		while (_instructionsCGI[++j])
			delete(_instructionsCGI[j]);
		delete [] _instructionsCGI;
	}
}
/*
** -------------------------------------------------------------------------------
** ------------------- PUBLIC FUNCTIONS ------------------------------------------
** -------------------------------------------------------------------------------
*/
void cgiHandler::creationVectorEnviron(void){
	auth("authorization");
	contentLength("Content-Length");
	contentType("Content-Type");
	gatewayInterface();
	pathIman nfo("/scriptname+pathinfo");//////////////////
	// pathTranslated("j'y comprend rien!");////////////SHOULD
	queryString("la sequence query string");
	remoteAddr("je ne sais pas ou on cherche cette putain d'adresse IP!");
	remoteHost(); /////////////SHOULD
	remoteUser("user id provided dans auth");
	requestMethod("method definie dans le header");
	scriptName("vient du parsing de l url");
	serverName("le servername du script uri");
	serverPort("rien-> default, sinon celui fourni dans URI");
	serverProtocol();
	serverSoftware();
	otherMetaVariables();
}

void cgiHandler::setVarEnv(void){
	unsigned long len = _monVectorEnv.size();

	if ((_varEnv = new char*[len + 1]) == NULL)
		throw std::runtime_error("error setting CGI environnement variables");
	for (unsigned long i = 0; i < len; i++)
		_varEnv[i] = strdup(_monVectorEnv[i].c_str());
	_varEnv[len] = NULL;
}

void cgiHandler::executeCGI(void)
{
	int fdPipeIn[2];
	int fdPipeOut[2];
	pid_t pid;

	// pere     |     fils
	//	  In  1-|->0
	//   Out 0<-|-1

	if (pipe(fdPipeOut) < 0)
		throw std::runtime_error("error piping CGI");
	// if (method == POST)
		if (pipe(fdPipeIn) < 0)
			throw std::runtime_error("error piping CGI");
	pid = fork();
	if (pid < 0)
	{
		close(fdPipeOut[0]);
		close(fdPipeOut[1]);
		// if (method==POST)
		{
			close (fdPipeIn[0]);
			close (fdPipeOut[1]);
		}
		throw std::runtime_error("error forking CGI");
	}

	else if (!pid)// processus fils
	{
		close(fdPipeOut[0]);
		if (dup2(fdPipeOut[1], STDOUT_FILENO) < 0)
			std::cerr << "error with dup2 in CGIson\n";
		close(fdPipeOut[1]);

		// if (method==POST)
		{
			close(fdPipeIn[1]);
			if(dup2(fdPipeIn[0], STDIN_FILENO) < 0)
				std::cerr << "error with dup2 in CGIson\n";
			close(fdPipeIn[0]);
		}
		// chdir("le string du chemin".c_str()) on se met dans le bon repertoire pour execve
		if (execve(_instructionsCGI[0], _instructionsCGI, _varEnv)< 0)
			std::cerr << "error with CGI execution\n";
		exit(1);
	}
	else if (pid > 0) //pere
	{
		close (fdPipeOut[1]);
		// if (method == POST)
		{
			close (fdPipeIn[0]);
			//la partie qui write le body pour le cgi
			// if (write(fdPipeIn[1], requestbody.c_str(), requestbody.length()) < 0)
			// 	throw std::runtime_error("error while writing to CGI");
			close (fdPipeIn[1]);
		}
		//la parie qui recupere l'info du cgi
		//voir si modif boucle pour les chunked unchunked
		std::string cgiResponse;
		char buf[1024];
		ssize_t x;
		while ((x = read(fdPipeOut[0], buf, 1024)) == 1024)
			cgiResponse += buf;
		if (x < 0)
			throw std::runtime_error("error while receiving cgi response");
		cgiResponse += buf;
		close (fdPipeOut[0]);
		//decoupage en header et body eventuel, calcul eventuel du body size
		size_t pos = cgiResponse.find("\r\n\r\n");
		std::string header = cgiResponse.substr(0, pos);
		std::string body = cgiResponse.substr(pos + 4);
		size_t bodySize = body.length();
	}
}


/*
** -------------------------------------------------------------------------------
** ------------------- PRIVATE FUNCTIONS -----------------------------------------
** -------------------------------------------------------------------------------
*/

void cgiHandler::auth(const std::string & str)
{
	std::map<std::string, std::string>::iterator it;
	std::string value = "";

	if ((it = _monDico.find(str)) != _monDico.end())
	{
		value = it->second;
		_monDico.erase(it);
	}
	_monVectorEnv.push_back("AUTH_TYPE=" + value);
}

void cgiHandler::contentLength(const std::string & str)
{
	std::map<std::string, std::string>::iterator it;
	std::string value = "";

	if ((it = _monDico.find(str)) != _monDico.end())
	{
		value = it->second;
		_monDico.erase(it);
	}
	_monVectorEnv.push_back("CONTENT_LENGTH=" + value);
}


void cgiHandler::contentType(const std::string & str)
{
	std::map<std::string, std::string>::iterator it;
	std::string value = "";

	if ((it = _monDico.find(str)) != _monDico.end())
	{
		value = it->second;
		_monDico.erase(it);
	}
	_monVectorEnv.push_back("CONTENT_TYPE=" + value);
}


void cgiHandler::gatewayInterface(void)
{
	_monVectorEnv.push_back("GATEWAY_INTERFACE=CGI/1.1");
}

void cgiHandler::pathInfo(const std::string & str) //str = script_name + PATH_info
{
	if (str == "")
	{
		_monVectorEnv.push_back("PATH_INFO=");
		return ;
	}
	//extraction de URI le scriptpath + extrapath
	char* buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		throw std::runtime_error("error during getcwd");
	std::string s2 = "/" + static_cast<std::string>(buf);
	s2 += str;
	// std::cout << "path info : " << s2 << std::endl;
	_monVectorEnv.push_back("PATH_INFO=" + s2);
	if (buf)
		free (buf);
}

void cgiHandler::queryString(const std::string & str) //<querystring> from cgi uri
{
		_monVectorEnv.push_back("QUERY_STRING=" + str);
}

void cgiHandler::remoteAddr(const std::string & str)
{
		_monVectorEnv.push_back("REMOTE_ADDR=" + str);
}

void cgiHandler::remoteHost(void)
{
	std::string s = "";
	_monVectorEnv.push_back("REMOTE_HOST=" + s);
}

void cgiHandler::remoteUser(const std::string & str)
{
	_monVectorEnv.push_back("REMOTE_USER=" + str);
}

void cgiHandler::requestMethod(const std::string & str)
{
	if (str == "GET" || str == "POST")
		_monVectorEnv.push_back("REQUEST_METHOD=" + str);
	else
		throw std::runtime_error("method not supported by webserv");
}

void cgiHandler::scriptName(const std::string & str)
{
	if (str == "")
		_monVectorEnv.push_back("SCRIPT_NAME=" + str);
	else
	{
		std::string s1 = "/" + str;
		_monVectorEnv.push_back("SCRIPT_NAME=" + s1);
	}
}


void cgiHandler::serverName(const std::string & str)
{
		_monVectorEnv.push_back("SERVER_NAME=" + str);
}

void cgiHandler::serverPort(const std::string & str)
{
	if (str == "")
	{
		std::string str2 = "default serveur port fourni par lucas";
		_monVectorEnv.push_back("SERVER_PORT=" + str2);
	}
	else
		_monVectorEnv.push_back("SERVER_PORT=" + str);
}

void cgiHandler::serverProtocol(void)
{
	_monVectorEnv.push_back("SERVER_PROTOCOL=HTTP/1.1");
}

void cgiHandler::serverSoftware(void)
{
	_monVectorEnv.push_back("SERVER_SOFTWARE=leServeurDeFlorianneLucasEtLaurent/1.0");
}

void cgiHandler::otherMetaVariables(void)
{
	std::map<std::string, std::string>::iterator it;
	for (it = _monDico.begin(); it != _monDico.end(); ++it)
	{
		std::string str = "HTTP_" + upperCaseAndMinus(it->first);
		str = str + "=" + it->second;
		_monVectorEnv.push_back(str);
	}
	_monDico.clear();
}

std::string cgiHandler::upperCaseAndMinus(const std::string & str)
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

