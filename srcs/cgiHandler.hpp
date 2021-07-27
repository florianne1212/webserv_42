#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

class CgiHandler;

# include <map>
# include <vector>
# include <cstring>
# include <string>
# include <cctype>
# include <unistd.h>
# include <iostream>
# include <exception>
# include "request.hpp"
# include "ClientSocket.hpp"
# include "Config.hpp"
# include "response.hpp"

class CgiHandler
{
protected:
	std::vector<std::string> _vectorEnv;
	char** _varEnv;
	char** _instructionsCGI;
	ClientSocket _client;
	Config _config;
	Request _request;
	Response _response;
	std::map<std::string, std::string> _parsedUrl;
	std::map<std::string, std::string> _headers;

private:
	CgiHandler();

public:
	CgiHandler(ClientSocket& client, Config & config, Request& request, Response & response);
	~CgiHandler();
	CgiHandler(CgiHandler const & other);
	CgiHandler & operator= (const CgiHandler & other);

	void executeCgi(void);


private:
////////////////////////
void recupererLeRoot(void);
////////////////////////

	void creationVectorEnviron(void);
	void setVarEnv(void);
	// void setInstructionCGI(void) a faire soit un seul argument, soit on a une extension en plus pour l'executable
	void executingCgi(void);

	void auth(const std::string & str);
	void contentLength(const std::string & str);
	void contentType(const std::string & str);
	void gatewayInterface(void);
	void pathInfo(const std::string & str);
	// void pathTranslated(const std::string & str); ///c'est un SHOULD
	void queryString(const std::string & str);
	void remoteAddr(const std::string & str);
	void remoteHost(void);
	void remoteUser(const std::string & str);
	void requestMethod(const std::string & str);
	void scriptNameAndScriptFilename(const std::string & str);
	void serverName(const std::string & str);
	void serverPort(const std::string & str);
	void serverProtocol(void);
	void serverSoftware(void);
	void otherMetaVariables(void);
	std::string upperCaseAndMinus(const std::string & str);
	// std::map<std::string, std::string> parseTheUri(std::string);
};

void UrlDecoder(std::string & str);
std::map<std::string, std::string> parseTheUri(std::string url);
		
#endif