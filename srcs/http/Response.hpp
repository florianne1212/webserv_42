#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <iostream>
# include "HttpStatus.hpp"
# include <stdlib.h>
# include "..//utils/usable.hpp"
# include "usable.hpp"

class Response
{
public:
	HttpStatus _find_status;
	int _status;
	std::map <std::string, std::string> _headers;
	usable< std::string > _body;
	usable< std::string > _body_path;
	std::string _response;
	bool _cgiResponse;
	usable<std::pair<std::string, std::string> > _append;
	bool _dir;

public:
	Response(/* args */);
	Response (Response const & copy);
	~Response();
	Response& operator=(Response const & ope);

	std::string create_response();
	usable< std::string > getBody();
	usable< std::string > getBodyPath();
	std::string Body404();
	int getStatus();
	bool getDir();
	std::string getResponse();
	usable<std::pair<std::string, std::string> > getAppend();
	bool getCgi();
	void setCgi(bool cgiResponse);

	void cleanResponse();
	void setBody(std::string str);
	void setBodyPath(std::string str);
	void setStatus(int status);
	void setHeaders(std::string header_name, std::string header_value);
	std::string int_to_str(int n);
	void setCgiResponse(std::string const & cgiResponse);
	void setAppend(std::string filename, std::string to_append);
	void setDir(bool dir);
	void responseClassInitialization(void);
	std::string getCgiResponse();


};


#endif
