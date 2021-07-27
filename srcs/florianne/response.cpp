#include "response.hpp"
#include <algorithm>

Response::Response(/* args */):
_status(200)
{
}

Response::Response (Response const & copy)
{
	*this = copy;
}

Response::~Response()
{
}

Response& Response::operator=(Response const & ope)
{
	if (this != &ope)
    {
        this->_status = ope._status;
		this->_body = ope._body;
        this->_headers = ope._headers;
    }
    return (*this);
}

std::string Response::Body404()
{
	std::string response_body =
		"<!DOCTYPE html>\n"
		"<html>"
		" <head>\n"
		"<title>\n"
		"404</title>\n"
		"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
		"<style>\n"
		"body {background-color:#61adb3;background-repeat:no-repeat;background-position:top left;background-attachment:fixed;}\n"
		"h1{font-family:Arial, serif;color:#000000;text-align: center;font-size:80px}\n"
		"p {font-family:Arial, serif;font-size:20px;font-style:normal;font-weight:normal;color:#000000;text-align: center;}\n"
		"</style>\n"
		"</head>\n"
		"<body>\n"
		"<h1><svg width=\"24px\" height=\"24px\" xmlns=\"http://www.w3.org/2000/svg\" fill-rule=\"evenodd\" clip-rule=\"evenodd\"><path d=\"M17 24h-10v-1.342c1.808-.985 2.005-2.205 2-3.658h-8c-.265 0-.52-.105-.707-.293-.188-.187-.293-.442-.293-.707v-17c0-.265.105-.52.293-.707.187-.188.442-.293.707-.293h22c.265 0 .52.105.707.293.188.187.293.442.293.707v17c0 .265-.105.52-.293.707-.187.188-.442.293-.707.293h-8c.004 1.374.155 2.66 2 3.658v1.342zm-3.984-5h-2.044c-.015.802.004 2.003-.719 3h3.493c-.757-1.02-.716-2.25-.73-3zm8.984-5h-20v3h20v-3zm-10 .537c.552 0 1 .448 1 1s-.448 1-1 1-1-.448-1-1 .448-1 1-1zm-10-12.537v10h20v-10h-20z\"/></svg> </h1>\n"
		"<h1>404</h1>\n"
		"<p>Page not found</p>\n"
		"</body>\n"
		"</html>";
	
	return(response_body);
}

std::string Response::create_response()
{
	std::string s = std::to_string(_status);
	_response.append("HTTP/1.1 ");
	_response.append(s);
	_response.append(" ");
	_response.append(_find_status.find_message(_status));
	_response.append("\r\n");
	for (std::map<std::string, std::string>::iterator it=_headers.begin(); it!=_headers.end(); ++it)
    {
		_response.append(it->first);
		_response.append(": ");
		_response.append(it->second);
		_response.append("\r\n");
	}
	_response.append("\r\n");
	_response.append(_body);
	// 	std::cout << it->first << " => " << it->second << '\n';
	// _response.append("\r\n");

	
	// std::cout << "\n" << _response.substr(0, std::min(_response.length() - 1, (size_t)300)) << "\n";

	return(_response);
}

std::string Response::getBody()
{
    return(this->_body);
}

int Response::getStatus()
{
    return(this->_status);
}

std::string Response::getResponse()
{
	return(this->_response);
}

void Response::setBody(std::string str)
{
    this->_body=str;
}

void Response::setStatus(int status)
{
    this->_status=status;
}

void Response::cleanResponse()
{
	_response.clear();
}

void Response::setHeaders(std::string header_name, std::string header_value)
{
	_headers.insert(std::pair<std::string, std::string>(header_name, header_value));

	// for (std::map<std::string, std::string>::iterator it=_headers.begin(); it!=_headers.end(); ++it)
    // 	std::cout << it->first << " => " << it->second << '\n';
	std::cout << '\n';
}