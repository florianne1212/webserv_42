#ifndef PARSEBODY_HPP
# define PARSEBODY_HPP

# include <string>
# include <map>
# include <iostream>
# include "ParseChunkedBody.hpp"
# include "Config.hpp"


class ParseBody
{

public:
	enum State
	{
		S_PARSE,
		S_END,
	};
private:
	std::string _body;
	State _state;
	int _count;
	ParseChunkedBody _parseChunkedBody;
	Config _config;
	bool _error;
	int _status;
public:
    ParseBody(Config &config);
	ParseBody(std::string ParseBody, Config &config);
    ParseBody(ParseBody const & copy);
    ~ParseBody();
    ParseBody & operator=(ParseBody const & ope);

public:
	void parse(char c, std::map<std::string, std::string> _headers, std::string url);
	void parse_chunked(char c);
	void parse_identity(char c, std::map<std::string, std::string> _headers, std::string url);
	std::string get_Body()
	{
		return(_body);
	}
	State get_state()
	{
		return(_state);
	}
	bool get_error()
	{
		return(_error);
	}

	int getStatus()
	{
		return(this->_status);
	}

	void setStatus(int status)
	{
		this->_status=status;
	}

};


#endif