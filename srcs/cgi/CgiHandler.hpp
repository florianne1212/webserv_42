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
#include <fcntl.h>
# include <exception>
# include "Request.hpp"
# include "ClientSocket.hpp"
# include "Config.hpp"
# include "CgiSocket.hpp"
# include "Response.hpp"
# include "CgiSocketFromCgi.hpp"
# include "CgiSocketToCgi.hpp"

class CgiHandler
{
protected:
	std::vector<std::string> _vectorEnv;
	char** _varEnv;
	char** _instructionsCGI;
	ClientSocket* _client;
	Config _config;
	Request _request;
	Response* _response;
	std::map<std::string, std::string> _parsedUrl;
	std::map<std::string, std::string> _headers;
	std::string _pathForExec;

private:
	CgiHandler();

public:
	CgiHandler(ClientSocket * client, Config & config, Request & request, Response * response);
	~CgiHandler();
	CgiHandler(CgiHandler const & other);
	CgiHandler & operator= (const CgiHandler & other);

	void executeCgi(void);


private:

	void creationVectorEnviron(void);
	void setVarEnv(void);
	void setInstructionCgi(void);
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
	bool scriptName(std::string & str);
	void serverName(const std::string & str);
	void serverPort(const std::string & str);
	void serverProtocol(void);
	void serverSoftware(void);
	void otherMetaVariables(void);
	void visualizeEnviron(void) const;
	bool checkExecutableExistence(std::string const & str);
	std::string upperCaseAndMinus(const std::string & str);
	void redirectStatus(void);
	void parsePathforCgi(void);
	std::string & checkCgiResponse(std::string & response);
	void checkIfPhpCgi(void);
	int checkHeaders(char c, int state);
	std::string cgiResponseHeaderPreparation(std::string & cgiHeaders);
	std::string cgiResponseChunkedPreparation(std::string cgiResponse, int readResult);

};

void UrlDecoder(std::string & str);
std::map<std::string, std::string> parseTheUri(std::string url);

#endif