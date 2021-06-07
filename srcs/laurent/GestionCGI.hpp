#ifndef GESTIONCGI_HPP
#define GESTIONCGI_HPP


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

enum e_requestMethod {GET, POST};

class GestionCGI
{
	protected:
		std::string		_CGIRequest;
		std::string		_requestMetaVariables;
		char**		_environCGI; ////////ne pas oublier les metavariables provenant du client  auxquelles on rajoute HTTP_
		std::string		_requestMessageBody;
		e_requestMethod	_requestMethod;
		std::string		_CGIResponse;


		

	public:
		GestionCGI();
		virtual ~GestionCGI();
		GestionCGI(const GestionCGI & other);
		GestionCGI & operator=(const GestionCGI & other);

		void receiveRequest(void);
		void convertRequestintoCGI(void);
		void executeCGIScript(void);
		void convertCGIResponse(void);
		//convertuppercasemetavariable
		//gestionparsing metavariables
};


#endif