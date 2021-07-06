#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <iostream>
#include "httpStatus.hpp"

class Response
{
public:
	HttpStatus _find_status;
	int _status;
	std::map <std::string, std::string> _headers;
	std::string _body;
	std::string _response;

public:
	Response(/* args */);
	Response (Response const & copy);
	~Response();
	Response& operator=(Response const & ope);

	std::string create_response();
	std::string getBody();
	int getStatus();

	void setBody(std::string str);
	void setStatus(int status);
	void setHeaders(std::string header_name, std::string header_value);
};


#endif
