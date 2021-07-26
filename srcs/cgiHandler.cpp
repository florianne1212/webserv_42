#include "cgiHandler.hpp"

/*
** -------------------------------------------------------------------------------
** ----------------------- COPLIEN FORM ------------------------------------------
** -------------------------------------------------------------------------------
*/

CgiHandler::CgiHandler(ClientSocket & client, Config & config, Request& request, Response & response):
	_vectorEnv(0), _varEnv(0), _instructionsCGI(0), _client(client),_config(config), _request(request), _response(response)
{
	_headers = request.getHeaders();
	_parsedUrl = parseTheUri(request.getUrl());
}

CgiHandler::~CgiHandler()
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

CgiHandler::CgiHandler(CgiHandler const & other):
	_vectorEnv(other._vectorEnv), _varEnv(other._varEnv), _instructionsCGI(other._instructionsCGI),
	_client(other._client),_config(other._config), _request(other._request),_response(other._response), _parsedUrl(other._parsedUrl), _headers(other._headers)
	{}

CgiHandler & CgiHandler::operator= (const CgiHandler & other)
{
	if (this != &other)
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
		_vectorEnv = other._vectorEnv;
		_varEnv = other._varEnv;
		_instructionsCGI = other._instructionsCGI;
		_request = other._request;
		_client = other._client;
		_config = other._config;
		_response = other._response;
		_parsedUrl = other._parsedUrl;
		_headers = other._headers;
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
	setVarEnv();
	executingCgi();
}

/*
** -------------------------------------------------------------------------------
** ------------------- PRIVATE FUNCTIONS -----------------------------------------
** -------------------------------------------------------------------------------
*/
// void CgiHandler::recupererLeRoot(void)
// {
// 	std::string lecheminduroot;
// 	if( _config.getRoot().state == true)
// 		lecheminduroot = _config.getRoot();

// }


void CgiHandler::creationVectorEnviron(void){
	auth("Authorization"); //DONE
	contentLength("Content-Length"); //DONE
	contentType("Content-Type"); //DONE
	gatewayInterface(); //DONE
	pathInfo("/scriptname+pathinfo");//////////////////
	// pathTranslated("j'y comprend rien!");////////////SHOULD
	queryString(_parsedUrl["query"]); //DONE
	remoteAddr(_client.getClientAddress());//DONE
	remoteHost(); // DONE
	remoteUser(_parsedUrl["user_name"]);//DONE
	requestMethod(_request.getMethods()); // DONE
	scriptName("vient du parsing de l url");////////////////
	serverName(_parsedUrl["host"]);//DONE
	serverPort(_parsedUrl["port"]);///////// petit detail a voir avec lucas
	serverProtocol();//DONE
	serverSoftware();//DONE
	otherMetaVariables();//DONE
}

void CgiHandler::setVarEnv(void){
	unsigned long len = _vectorEnv.size();

	if ((_varEnv = new char*[len + 1]) == NULL)
		throw std::runtime_error("error setting CGI environnement variables");
	for (unsigned long i = 0; i < len; i++)
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
	if (_request.getMethods() == "POST")
		if (pipe(fdPipeIn) < 0)
			throw std::runtime_error("error piping CGI");
	pid = fork();
	if (pid < 0)
	{
		close(fdPipeOut[0]);
		close(fdPipeOut[1]);
		if (_request.getMethods() == "POST")
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

		if (_request.getMethods() == "POST")
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
		if (_request.getMethods() == "POST")
		{
			close (fdPipeIn[0]);
			//la partie qui write le body pour le cgi
			if (write(fdPipeIn[1], _request.getBody().c_str(), _request.getBody().length()) < 0)
				throw std::runtime_error("error while writing to CGI");
			close (fdPipeIn[1]);
		}
		//la partie qui recupere l'info du cgi
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
		(void)bodySize;
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

void CgiHandler::pathInfo(const std::string & str) //str = script_name + PATH_info
{
	if (str == "")
	{
		_vectorEnv.push_back("PATH_INFO=");
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
	_vectorEnv.push_back("PATH_INFO=" + s2);
	if (buf)
		free (buf);
}

void CgiHandler::queryString(const std::string & str) //<querystring> from cgi uri
{
		_vectorEnv.push_back("QUERY_STRING=" + str);
}

void CgiHandler::remoteAddr(const std::string & str)
{
		_vectorEnv.push_back("REMOTE_ADDR=" + str);
}

void CgiHandler::remoteHost(void)
{
	std::string s = "";
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

void CgiHandler::scriptName(const std::string & str)
{
	if (str == "")
		_vectorEnv.push_back("SCRIPT_NAME=" + str);
	else
	{
		std::string s1 = "/" + str;
		_vectorEnv.push_back("SCRIPT_NAME=" + s1);
	}
}


void CgiHandler::serverName(const std::string & str)
{
		_vectorEnv.push_back("SERVER_NAME=" + str);
}

void CgiHandler::serverPort(const std::string & str)
{
	if (str == "")
	{
		std::string str2 = "default serveur port fourni par lucas";
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
	_vectorEnv.push_back("SERVER_SOFTWARE=leServeurDeFlorianneLucasEtLaurent/1.0");
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

//on cherche le scheme
	found = url.find_first_of(":");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("scheme", url.substr(0, found)));
		url = url.substr(found + 1);
	}
	// //checking scheme (doit etre http pour nous...)
	// if (parsedUrl["scheme"] != "http")
	// {
	// 	std::cout << "je ne supporte que le http !!" << std::endl;
	// 	parsedUrl.clear();
	// 	return (parsedUrl);
	// }
//on cherche le fragment
	found = url.find_first_of("#");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("fragment", url.substr(found + 1)));
		url = url.substr(0, found);
		//a priori pas besoin de checker car le cgi renvoie erreur si pas bon
	}
//on cherche le query
	found = url.find_first_of("?");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("query", url.substr(found + 1)));
		url = url.substr(0, found);
	}

//on cherche si authority
	if (url.substr(0, 2) == "//") //il existe authority, a parser,
	{
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
			parsedUrl.insert(std::make_pair("port", "80"));
			parsedUrl.insert(std::make_pair("host", url));
		}
	}
	else
	{
		parsedUrl.insert(std::make_pair("path", url));
	}
	//on recherche si script.php
	found = parsedUrl["path"].find(".php");
	if (found != std::string::npos)
	{
		//on delimite le path du cgi et le additionnal path
		std::string cgiPath = parsedUrl["path"].substr(0, found + 4);
		UrlDecoder(cgiPath);
		found = parsedUrl["path"].find_first_of("/", found + 4);
		std::string additionnalPath = parsedUrl["path"].substr(found);
		UrlDecoder(additionnalPath);
		//on met le "real path" en fonction de "location  de - \.php"
		std::string root =  "";//donnees a recuperer de config
		cgiPath = root + "/" + cgiPath;
		additionnalPath = root + "/" + additionnalPath;
		parsedUrl.insert(std::make_pair("additionnal_path", additionnalPath));
		parsedUrl.insert(std::make_pair("cgi_path", cgiPath));
	}
	//else determination de la location en fonction de l'adresse et creation du real path

	return (parsedUrl);

}
