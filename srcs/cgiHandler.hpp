#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <map>
#include <vector>
#include <cstring>
#include <string>
#include <cctype>
#include <unistd.h>
#include <iostream>
#include <exception>
#include "./florianne/request.hpp"
#include "./socket/ClientSocket.hpp"

class cgiHandler
{
protected:
	//les donnees de tanguy du formulaire
	std::vector<std::string> _monVectorEnv;
	char** _varEnv;
	char** _instructionsCGI;
	Request* _request;
	ClientSocket* _client;
	std::map<std::string, std::string> _parsedUrl;
	std::map<std::string, std::string> _headers;

private:
	cgiHandler();

public:
	cgiHandler(Request* requestComingFromFlorianne, ClientSocket* client); //et donnees de Tanguy a mettre
	~cgiHandler();
	cgiHandler(cgiHandler const & other);
	cgiHandler & operator= (const cgiHandler & other);


	void creationVectorEnviron(void);
	void setVarEnv(void);
	// void setInstructionCGI(void) a faire soit un seul argument, soit on a une extension en plus pour l'executable
	void executeCGI(void);

private:
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
	void scriptName(const std::string & str);
	void serverName(const std::string & str);
	void serverPort(const std::string & str);
	void serverProtocol(void);
	void serverSoftware(void);
	void otherMetaVariables(void);
	std::string upperCaseAndMinus(const std::string & str);
	std::map<std::string, std::string> cgiHandler::parseTheUri(std::string);
};

void UrlDecoder(std::string & str);
std::map<std::string, std::string> parseTheUri(std::string url);

#endif