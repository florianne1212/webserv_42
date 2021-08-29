
#include "ParseRequest.hpp"

ParseRequest::ParseRequest(Config &config):
_state(S_NOT_STARTED),
_parseBody(config),
_config(config),
_error(false),
_status(200)
{

	
}



ParseRequest::~ParseRequest()
{
}



void ParseRequest::parse(char c)
{
	try
	{
		
		std::string message;
		switch(_state)
		{
			case S_NOT_STARTED:
			{
				if(c == '\r' || c == '\n')
					break;
				else
				{
					if(c != ' ')
						_method+=c;
					_state = S_METHOD;
				}
				break;
			}
			case(S_METHOD):
			{
				if(c == ' ')
				{
					if(_state == S_NOT_STARTED)
					{
						throw(400);
						//throw std::string( "there is no method");
						//_status = 400;
					}
					_state = S_SPACES_BEFORE_PATH;
				}
				else
				{
					if(islower(c))
					{
						throw(400);
						//throw std::string( "method is supposed to be uppercase");
						//_status = 400;
					}
					if(_method.length() > 15)
					{
						throw(400);
						//throw std::string( "method can't be that long");
						//_status = 400;
					}
					_method += c;
				}
				break;
			}
			case(S_SPACES_BEFORE_PATH):
			{
				if(c != ' ')
				{
					if(c == '/')
					{
						_path += c;
						_state = S_PATH;
					}
					else
					{
						throw(400);
						//throw std::string( "must have path");
						//_status = 400;
					}
				}
				break;
			}
			case(S_PATH):
			{
				if(c != ' ')
					_path += c;
				else
					_state = S_HTTP_H;
				
				break;
			}
			case(S_HTTP_H):
			{
				if(c != 'H')
				{
					throw(400);
					//throw std::string( "wrong charachter must be : H");
					//_status = 400;
				}
				_state = S_HTTP_HT;
				break;
			}
			case(S_HTTP_HT):
			{
				if(c != 'T')
				{
					throw(400);
					//throw std::string( "wrong charachter must be : T");
					//_status = 400;
				}
				_state = S_HTTP_HTT;
				break;
			}
			case(S_HTTP_HTT):
			{
				if(c != 'T')
				{
					throw(400);
					//throw std::string( "wrong charachter must be : T");
					//_status = 400;
				}
				_state = S_HTTP_HTTP;
				break;
			}
			case(S_HTTP_HTTP):
			{
				if(c != 'P')
				{
					throw(400);
					//throw std::string( "wrong charachter must be : P");
					//_status = 400;
				}
				_state = S_HTTP_SLASH;
				break;
			}
			case(S_HTTP_SLASH):
			{
				if(c != '/')
				{
					throw(400);
					//throw std::string( "wrong charachter must be : /");
					//_status = 400;
				}
				_state = S_HTTP_MAJOR;
				break;
			}
			case(S_HTTP_MAJOR):
			{
				if(!isdigit(c))
				{
					throw(400);
					//throw std::string( "http version major must be a number");
					//_status = 400;
				}
				if(c != '1')
				{
					throw(400);
					//throw std::string( "wrong version only HTTP/1.1 is supported");
					//_status = 400;
				}
				_major = c - '0';
				_state = S_HTTP_DOT;
				break;
			}
			case(S_HTTP_DOT):
			{
				if(c != '.')
				{
					throw(400);
					//throw std::string( "wrong character must be : '.'");
					//_status = 400;
				}
				_state = S_HTTP_MINOR;
				break;
			}
			case(S_HTTP_MINOR):
			{
				if(!isdigit(c))
				{
					throw(400);
					//throw std::string( "http version minor must be a number", c);
					//_status = 400;
				}
				else if(c != '1')
				{
					throw(400);
					//throw std::string( "wrong version only HTTP/1.1 is supported");
					//_status = 400;
				}
				else
				{
					_minor = c - '0';
					_state = S_AFTER_HTTP;
				}
				
				break;
			}

			case(S_AFTER_HTTP):
			{
				if(c == '\r')
					_state = S_HTTP_END_R;
				else if(c == '\n')
					_state = S_HTTP_END_N;
				else
				{
					//throw std::string( "there is suposed to be n or r");
					//_status = 400;
					throw(400);
				}
				break;
			}

			case(S_HTTP_END_R):
			{
				if(c != '\n')
				{
					//throw std::string( "expected return line (\\n)");
					throw(400);
					//_status = 400;
				}
				else
					_state = S_HTTP_END_N;
				
				break;
			}

			case(S_HTTP_END_N):
			{
				if(c == '\n')
					_state = S_HTTP_END_N;
				else if(c == '\r')
					_state = S_END;
				else
				{
					_state = S_HEADER_FIELDS;
					_parseHeaderFields.parse(c);
				}

				break;
			}

			case(S_HEADER_FIELDS):
			{
				_parseHeaderFields.parse(c);
				if(_parseHeaderFields.get_state() == ParseHeaderFields::S_END)
				{
					if(_parseHeaderFields.getStatus() != 200)
						_status = _parseHeaderFields.getStatus();
					_state = S_BODY;
				}
				break;
			}

			case(S_BODY):
			{
				_parseBody.parse(c, _parseHeaderFields.get_headers_map(), _path);
				if(_parseBody.get_state() == ParseBody::S_END)
				{
					if(_parseBody.getStatus() != 200)
						_status = _parseBody.getStatus();
					_body = _parseBody.get_Body();
					_error = _parseBody.get_error();
					_state = S_END;
				}
				break;
			}
			
			case(S_END):
				break;

		}
		/* code */
	}
	catch(int error)
	{
		//std::cout << "Access denied - You must be at least 18 years old.\n";
		//std::cout << "\nerror :" << error;
  		_status = error;
	}
}