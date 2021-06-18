#ifndef GESTIONCGI_HPP
#define GESTIONCGI_HPP

#include <list>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <exception>
#include <ctime>
#include <cstring>
#include <unistd.h>

enum e_requestMethod {GET, POST};

class GestionCGI
{
	protected:
		std::string		_CGIRequest;
		std::string		_requestMetaVariables;
		std::vector<std::string> _envCGIPrepare;
		std::map<std::string, std::string> _dictionnary;
		std::vector<char*>		_environCGI; ////////ne pas oublier les metavariables provenant du client  auxquelles on rajoute HTTP_
		std::string		_requestMessageBody;
		e_requestMethod	_requestMethod;
		std::string		_CGIResponse;




	public:
		GestionCGI();
		virtual ~GestionCGI();
		GestionCGI(const GestionCGI & other);
		GestionCGI & operator=(const GestionCGI & other);

		void createCGIProcess(void);//faire le fork du serveur
		void receiveRequest(std::map<std::string, std::string>  & dictionnary);
		void convertRequestintoCGI(void);
		void environCGICreation(void);
		void executeCGIScript(void);
		void convertCGIResponse(void);

	private:
		std::string convertToUpperCase(std::string const & toChange);
		//gestionparsing metavariables
};


#endif