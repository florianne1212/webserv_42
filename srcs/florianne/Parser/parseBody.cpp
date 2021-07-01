#include "parseBody.hpp"


ParseBody::ParseBody():
_count(0)
{
}

ParseBody::ParseBody(std::string Body):
_body(Body),
_count(0)
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

void ParseBody::parse_identity(char c, std::map<std::string, std::string> _headers)
{
	// std::cout << "nb :" << atoi(_headers.find("Content-Length")->second.c_str()) << " nb2 =" << _count;
	if (_count < atoi(_headers.find("Content-Length")->second.c_str()))
	{
		_body.push_back(c);
		_count++;
	}
	else
	{
		std::cout << "\nBody = " << _body; 
		_state = S_END;
	}
}

void ParseBody::parse(char c, std::map<std::string, std::string> _headers)
{
	if(_headers.find("Transfer-Encoding") != _headers.end() && _headers.find("Transfer-Encoding")->second == "chunked")
		parse_chunked(c);
	else if(_headers.find("Transfer-Encoding") != _headers.end() && _headers.find("Transfer-Encoding")->second == "identity")
		parse_identity(c, _headers);
	else if(_headers.find("Content-Length") != _headers.end()) 
		parse_identity(c, _headers);
	else
		_state = S_END;
	// std::cout << _headers.find("Content-Length")->first;
}	// else if(_headers.find("Content-Length") != _headers.end()) 
	// 	parse_identity(c);
