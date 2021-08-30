#include "ParseChunkedBody.hpp"


ParseChunkedBody::ParseChunkedBody():
_state(S_LENGTH),
_count(1),
_status(200)
{
}

ParseChunkedBody::ParseChunkedBody(std::string Body):
_body(Body),
_state(S_LENGTH),
_count(1),
_status(200)
{
}

ParseChunkedBody::ParseChunkedBody(ParseChunkedBody const & copy)
{
	*this = copy;
}

ParseChunkedBody::~ParseChunkedBody()
{
}

ParseChunkedBody& ParseChunkedBody::operator=(ParseChunkedBody const & ope)
{
	if (this != &ope)
	{
		this->_body = ope._body;
		this->_state = ope._state;
		this->_count = ope._count;
		this->_nb = ope._nb;
	}
	return (*this);
	
}


int ParseChunkedBody::convertHex(std::string hex_number)
{

	const char *hexstring = hex_number.c_str();
	int _nbConvert = (int)strtol(hexstring, NULL, 16);
	return (_nbConvert);
}

void ParseChunkedBody::parse(char c)
{
	switch(_state)
	{
		case (S_LENGTH):
		{
			if(c != '\r')
				_nb.push_back(c);
			else 
			{
				_state = S_END_R;
			}
			break;
		}
		case(S_PARSE_BODY):
		{ 
			if(_count < (convertHex(_nb)))
			{
				_body.push_back(c);
				
				_count++;
				
			}
			if(_count == (convertHex(_nb)))
			{
				_count++;
				break;
			}
			if(_count == (convertHex(_nb)) + 1)
			{
				_nb.clear();
				_count = 1;
				if(c != '\r')
				{
					throw(400);
					//throw std::string("there is supposed to be a '\\r", c);
					//_status = 400;
				}
				else
					_state = S_END_R;
			}
			break;
		}
		case(S_END_R):
		{
			if(c == '\n')
				_state = S_END_N;
			else
			{
				throw(400);
				//throw std::string("there is supposed to be a '\\n");
				//_status = 400;
			}
			break;
		}
		case(S_END_N):
		{
			if(c == '\r')
				_state = S_END_R;
			else if(!_nb.empty() && convertHex(_nb) == 0)
			{
				_state = S_END_R2;
			}
			else if(_nb.empty())
			{
				_nb.push_back(c);
				if (convertHex(_nb) == 0)
					_state = S_END;
				else
					_state = S_LENGTH;
			}
			else 
			{
				_body.push_back(c);
				_state = S_PARSE_BODY;
			}
			
			
			break;
		}
		case(S_END_R2):
		{
			if(c == '\n')
				_state = S_END;
			else
			{
				throw(400);
				//throw std::string("there is supposed to be a '\\n");
				//_status = 400;
			}
			break;
		}
		case(S_END):
		{
			break;
		}
	}
}	
