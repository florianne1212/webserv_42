#ifndef PARSECHUNKEDBODY_HPP
# define PARSECHUNKEDBODY_HPP

# include <string>
# include <cstdlib>
# include <map>
# include <iostream>

class ParseChunkedBody
{

public:
	enum State
	{
		S_LENGTH,
		S_PARSE_BODY,
		S_END_R,
		S_END_N,
		S_END_R2,
		S_END,
	};
private:
	std::string _body;
	State _state;
	int _count;
	std::string _nb;
	int _status;
public:
    ParseChunkedBody();
	ParseChunkedBody(std::string ParseChunkedBody);
    ParseChunkedBody(ParseChunkedBody const & copy);
    ~ParseChunkedBody();
    ParseChunkedBody & operator=(ParseChunkedBody const & ope);
	int convertHex(std::string hex_number);

public:
	void parse(char c);
	std::string get_Body()
	{
		return(_body);
	}
	State get_state()
	{
		return(_state);
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