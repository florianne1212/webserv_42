#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <iostream>
# include "httpStatus.hpp"
# include <stdlib.h>
# include "../../utils/usable.hpp"

class Response
{
public:
	HttpStatus _find_status;
	int _status;
	std::map <std::string, std::string> _headers;
	std::string _body;
	// usable<std::string> _response;
	std::string _response;

public:
	Response(/* args */);
	Response (Response const & copy);
	~Response();
	Response& operator=(Response const & ope);

	std::string create_response();
	std::string getBody();
	std::string Body404();
	int getStatus();
	std::string getResponse();

	void cleanResponse();
	void setBody(std::string str);
	void setStatus(int status);
	void setHeaders(std::string header_name, std::string header_value);
	std::string int_to_str(int n);
	void setCgiResponse(std::string cgiResponse);
};


#endif
