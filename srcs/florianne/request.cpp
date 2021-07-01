#include "request.hpp"


// std::vector<std::string> Request::createMethods()
// {
// 	std::vector<std::string> authorized_methods;
	
// 	authorized_methods.push_back("GET");
// 	authorized_methods.push_back("POST");
// 	authorized_methods.push_back("DELETE");

// 	return(authorized_methods)
// }


Request::Request()
{

}

Request::Request(Request const & copy)
{
	*this = copy;
}

Request::~Request()
{
}

Request& Request::operator=(Request const & ope)
{
	if (this != &ope)
	{
		this->_methods = ope._methods;
		this->_url = ope._url;
		this->_version = ope._version;
		this->_headers = ope._headers;
	}
	return (*this);
}

std::string	Request::getMethods()
{
	return(this->_methods);
}


std::string	Request::getUrl()
{
	return(this->_url);
}

std::string	Request::getBody()
{
	return(this->_body);
}


std::string	Request::getVersion()
{
	return(this->_version);
}

std::map<std::string, std::string> Request::getHeaders()
{
	return(this->_headers);
}

void	Request::setMethods(std::string str)
{
	this->_methods = str;
}

void	Request::setUrl(std::string str)
{
	this->_url = str;
}

void	Request::setVersion(std::string str)
{
	this->_version = str;
}

void	Request::setBody(std::string str)
{
	this->_body = str;
}

void Request::setHeaders(std::map<std::string, std::string> header)
{
	this->_headers = header;
}