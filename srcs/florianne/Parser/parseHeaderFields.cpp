#include "parseHeaderFields.hpp"
#include <stdio.h>
#include <iostream>

ParseHeaderFields::ParseHeaderFields():
_state(S_FIELD)
{
}

ParseHeaderFields::ParseHeaderFields(ParseHeaderFields const & copy)
{
	*this = copy;
}

ParseHeaderFields::~ParseHeaderFields()
{
}

ParseHeaderFields& ParseHeaderFields::operator=(ParseHeaderFields const & ope)
{
    if (this != &ope)
    {
        this->_headers = ope._headers;
        this->_state = ope._state;
		this->_value = ope._value;
        this->_field = ope._field;
    }
    return (*this);
}

void ParseHeaderFields::parse(char c)
{
	switch(_state)
	{
		case S_FIELD:
		{
			if(c == ' ')
			{
				if(_field.empty())
					printf("%s\n","there is some space before field");
				else
					printf("%s\n","there is some space before fields");
				   
			}
			else if(c == ':')
				 _state=S_COLON;
			else
				_field += c;

			break;
		}
		case(S_COLON):
		{
			if(c == ' ')
				_state = S_SPACES_BEFORE_VALUE;
			
			break;
		}
		case(S_SPACES_BEFORE_VALUE):
		{

			if(c != ' ')
			{
				_state = S_VALUE;
				_value += c;
			}
			break;
		}
		case S_VALUE:
		{
			if (c == ' ')
				_state = S_SPACES_AFTER_VALUE;
			else if (c == '\r')
			{
				add_header();
				_state = S_END_R;
			}
			else if (c == '\n')
			{
				add_header();
				_state = S_END_N;
			}
			else
				_value += c;

			break;
		}

		case S_SPACES_AFTER_VALUE:
		{
			if (c == ' ')
				_state = S_SPACES_AFTER_VALUE;
			else if (c == '\n')
			{
				add_header();
				_state = S_END_N;
			}
			else if (c == '\r')
			{
				add_header();
				_state = S_END_N;
			}
			else
			{
				_value += ' ';
				_value += c;
				_state = S_VALUE;
			}

			break;
		}

		case S_END_R:
		{
			if (c == '\n')
				_state = S_END_N;
			else
				printf("%s\n","there is supposed to be a '\\n");

			break;
		}

		case S_END_N:
		{
			if (c == '\r')
			{
				_state = S_END_R2;
			}
			else if (c == '\n')
			{
				_state = S_END;
			}
			else
			{
				_field += c;
				_state = S_FIELD;
			}
			break;
		}

		case S_END_R2:
		{
			if (c == '\n')
				_state = S_END;
			else
				printf("%s\n","there is supposed to be a '\\n");

			break;
		}

		case S_END:
			return;
	}
}

void ParseHeaderFields::add_header()
{
	_headers.insert(std::pair<std::string, std::string>(_field, _value));
	_field.clear();
	_value.clear();
}
