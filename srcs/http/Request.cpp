#include "Request.hpp"


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
		this->_body = ope._body;
		this->_parsedUri = ope._parsedUri;
		this->_error = ope._error;
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

std::map<std::string, std::string> Request::getParsedUri()
{
	return(_parsedUri);
}

bool Request::getError()
{
	return (_error);
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

void Request::setParsedUri(std::map<std::string, std::string> uri)
{
	this->_parsedUri = uri;
}

void Request::setError(bool error)
{
	_error = error;
}