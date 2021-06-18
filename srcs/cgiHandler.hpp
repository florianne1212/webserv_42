#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <map>
#include <vector>
#include <cstring>
#include <string>
#include <unistd.h>
#include <iostream>
#include <exception>

class cgiHandler
{
protected:
	std::map<std::string,std::string> _monDico; //copie du dico de florianne
	//les donnees de tanguy du formulaire
	std::vector<std::string> _monVectorEnv;
	char** _varEnv;
	char** _instructionsCGI;


	cgiHandler();

public:
	cgiHandler(const std::map<std::string, std::string> & floMap);
	~cgiHandler();

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
};

#endif