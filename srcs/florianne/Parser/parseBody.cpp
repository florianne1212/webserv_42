#include "parseBody.hpp"


ParseBody::ParseBody(Config &config):
_count(0),
_config(config),
_error(false)
{
}

ParseBody::ParseBody(std::string Body,Config &config):
_body(Body),
_count(0),
_config(config),
_error(false)
{
}

ParseBody::ParseBody(ParseBody const & copy)
{
	*this = copy;
}

ParseBody::~ParseBody()
{
}

ParseBody& ParseBody::operator=(ParseBody const & ope)
{
	if (this != &ope)
    {
        this->_body = ope._body;
        this->_state = ope._state;
		this->_count = ope._count;
    }
    return (*this);
    
}

void ParseBody::parse_chunked(char c)
{
	_parseChunkedBody.parse(c);
	if(_parseChunkedBody.get_state() == ParseChunkedBody::S_END)
	{
		_body = _parseChunkedBody.get_Body();
		std::cout << "_FINAL BODY " << _body;
		_state = S_END;
	}
}

void ParseBody::parse_identity(char c, std::map<std::string, std::string> _headers, std::string url)
{
	if(_headers.find("Content-Length") != _headers.end())
	{
		//Tanguy il faut résoudre mes problémes
		std::cout << "\nCONTENT LENGTH\n content length = " << (size_t)atoi(_headers.find("Content-Length")->second.c_str()) <<"\n";
		std::cout << "\n content length = " << _config.getBodyMaxSize(_headers.find("Host")->second.c_str(), url) <<"\n";
		if((size_t)atoi(_headers.find("Content-Length")->second.c_str()) > _config.getBodyMaxSize(_config.getServerName(_headers.find("Host")->second.c_str()).value, url))
		{
			std::cout << "\nTOO TOO LONG\n " << _body;
			_error = true;
			
		}
	}
	if (_count < (atoi(_headers.find("Content-Length")->second.c_str()) - 1))
	{
		_body.push_back(c);
		_count++;
	}
	else
	{
		_body.push_back(c);
		std::cout << "\nBody = " << _body; 
		_state = S_END;
	}
}

void ParseBody::parse(char c, std::map<std::string, std::string> _headers, std::string url)
{
	if(_headers.find("Transfer-Encoding") != _headers.end() && _headers.find("Transfer-Encoding")->second == "chunked")
		parse_chunked(c);
	else if(_headers.find("Transfer-Encoding") != _headers.end() && _headers.find("Transfer-Encoding")->second == "identity")
		parse_identity(c, _headers, url);
	else if(_headers.find("Content-Length") != _headers.end()) 
		parse_identity(c, _headers, url);
	else
		_state = S_END;
}
