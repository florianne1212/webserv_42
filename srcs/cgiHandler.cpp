#include "cgiHandler.hpp"
#include <sstream>
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
	// std::cout << "server name = " << client.getServerName() << "\n";
	std::cout << "URL transmise = " << request.getUrl()<< "\n";
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
	if(_response.getStatus()/100 == 2)
	{
	setVarEnv();
	setInstructionCgi();
	executingCgi();
	}
}

/*
** -------------------------------------------------------------------------------
** ------------------- PRIVATE FUNCTIONS -----------------------------------------
** -------------------------------------------------------------------------------
*/
void CgiHandler::setInstructionCgi(void)
{
	if (!(_instructionsCGI = new char*[2]))
		throw std::runtime_error("error allocation setting CGI instructions");
	_instructionsCGI[0] = strdup(_pathForExec.substr(_pathForExec.find_last_of("/") + 1).c_str());
	std::cout << _instructionsCGI[0] << "<---------instructionCGI\n";
	_instructionsCGI[1] = NULL;
}


void CgiHandler::creationVectorEnviron(void){
	auth("Authorization"); //DONE
	contentLength("Content-Length"); //DONE
	contentType("Content-Type"); //DONE
	gatewayInterface(); //DONE
	pathInfo(_parsedUrl["additionnal_path"]);//DONE JE CROIS, A VERIFIER SI C EST CE QUE SUJET DEMANDE
	// pathTranslated("j'y comprend rien!");////////////SHOULD
	queryString(_parsedUrl["query"]); //DONE
	remoteAddr(_client.getClientAddress());//DONE
	remoteHost(); // DONE
	remoteUser(_parsedUrl["user_name"]);//DONE
	requestMethod(_request.getMethods()); // DONE
	if (!scriptName(_parsedUrl["cgi_path"]))//DONE, checke existence de lexecutable
		return;
	serverName(_parsedUrl["host"]);//DONE
	serverPort(_parsedUrl["port"]);//DONE
	serverProtocol();//DONE
	serverSoftware();//DONE
	otherMetaVariables();//DONE
	redirectStatus();
	visualizeEnviron();/////A RETIRER BIEN SUR
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
			std::cerr << "error with dup2 in CGI son\n";
		close(fdPipeOut[1]);

		if (_request.getMethods() == "POST")
		{
			close(fdPipeIn[1]);
			if(dup2(fdPipeIn[0], STDIN_FILENO) < 0)
				std::cerr << "error with dup2 in CGI son\n";
			close(fdPipeIn[0]);
		}
		// std::cerr << "le chemin pour l'executable est : " << _pathForExec.substr(0, _pathForExec.find_last_of("/") + 1) << "\n";
		chdir((_pathForExec.substr(0, _pathForExec.find_last_of("/")).c_str())); //on se met dans le bon repertoire pour execve
	// 	char* buf = NULL;
	// buf = getcwd(buf, 0);
	// 	std::cerr << " nous sommes dans : " << buf << "\n";
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
		char buf[2049] = {0};
		ssize_t readResult;
		while ((readResult = read(fdPipeOut[0], buf, 2048)) > 0)
		{
			cgiResponse += buf;
			memset(buf, 0, 2049);
		}
		if (readResult < 0)
			throw std::runtime_error("error while receiving cgi response");
		cgiResponse += buf;
		close (fdPipeOut[0]);
		//decoupage en header et body eventuel, calcul eventuel du body size
		size_t pos = cgiResponse.find("\r\n\r\n");
		std::string header = cgiResponse.substr(0, pos);
		if (pos != cgiResponse.npos)
		{
			std::string body = cgiResponse.substr(pos + 4);
			size_t bodySize = body.length();
			(void)bodySize;
		}
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
	// if (str == "") ///JE NE SAIS PAS S IL FAUT METTRE CHEMIN ABSOLU ICI
	// {
	// 	_vectorEnv.push_back("PATH_INFO=");
	// 	return ;
	// }
	//mise en place chemin absolu
	char* buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		throw std::runtime_error("error during getcwd");
	std::string s2 = static_cast<std::string>(buf) + "/" + str;
	// std::cout << "path info : " << s2 << std::endl;
	_vectorEnv.push_back("PATH_INFO=" + s2);
	_pathForExec = s2;
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
	std::string s = _client.getClientAddress();
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
	// scriptname
	// if (str == "")
		_vectorEnv.push_back("SCRIPT_NAME=" + str);
		_pathForExec += (WORKPATH + str);
	// else
	// {
	// 	std::string s1 = "/" + str;
	// 	_vectorEnv.push_back("SCRIPT_NAME=" + s1);
	// }
	//scriptFilename
	// std::string newAddress = str;
	// usable<std::pair<std::string, std::string> > locationResponse = _config.getRoot(_client.getServerName(), str);
	// if (locationResponse.state == true)
	// {
	// std::string old = locationResponse.value.first;
	// std::string newLoc = locationResponse.value.second;

	// newAddress = str.replace(0, old.size(), newLoc);
	// }
	// _vectorEnv.push_back("SCRIPT_FILENAME=" + newAddress);

	checkExecutableExistence(str);

	// if (!checkExecutableExistence(newAddress))
	// 	return false;
	return true;
}


void CgiHandler::serverName(const std::string & str)
{
	if (str != "")
		_vectorEnv.push_back("SERVER_NAME=" + str);
	else
		_vectorEnv.push_back("SERVER_NAME=" + _client.getServerName());
}

void CgiHandler::serverPort(const std::string & str)
{
	if (str == "")
	{
		unsigned short port = _config.getPort(_client.getServerName());
		std::cout << "LE PORT EST : " << port <<"\n";
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
	_vectorEnv.push_back("SERVER_SOFTWARE=le_Serveur_De_Florianne_Tanguy_Et_Laurent/1.0");
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
		_response.setStatus(404);
		std::cout << " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
		std::cout << "cet executable n existe pas : " << str << std::endl;
		return (false);
	}
	std::cout << " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
		std::cout << "cet executable existe : " << str << std::endl;

	return true;
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
	//on recherche si script.php
	found = parsedUrl["path"].find(".php");
	if (found != std::string::npos)
	{
		//on delimite le path du cgi et le additionnal path
		std::string cgiPath = parsedUrl["path"].substr(0, found + 4);
		UrlDecoder(cgiPath);
		found = parsedUrl["path"].find_first_of("/", found + 4);
		if (found != std::string::npos)
		{
			std::string additionnalPath = parsedUrl["path"].substr(found);
			UrlDecoder(additionnalPath);
			parsedUrl.insert(std::make_pair("additionnal_path", additionnalPath));
		}
		else
			parsedUrl.insert(std::make_pair("additionnal_path", ""));
		//on met le "real path" en fonction de "location  de - \.php"
		// std::string root =  "";//donnees a recuperer de config
		// cgiPath = root + "/" + cgiPath;
		// additionnalPath = root + "/" + additionnalPath;
		parsedUrl.insert(std::make_pair("cgi_path", cgiPath));
	}
	//else determination de la location en fonction de l'adresse et creation du real path
	//a enlever
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n PARSED URL\n";
	for (std::map<std::string, std::string>::iterator it = parsedUrl.begin(); it != parsedUrl.end(); ++it)
		std::cout << it->first << " : " << it->second <<"\n";
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";

	//a enlever
	return (parsedUrl);
}

void CgiHandler::visualizeEnviron(void) const {
	std::cout << "<<<<<<<<<<<<< MES VARIABLES ENVIRONNEMENT CGI >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	std::vector<std::string>::const_iterator it;
	for (it = _vectorEnv.begin(); it != _vectorEnv.end(); it++)
		std::cout << *it <<std::endl;
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n";
}
