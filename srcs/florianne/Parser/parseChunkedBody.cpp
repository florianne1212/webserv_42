// #include "ParseChunkedBody.hpp"


// ParseChunkedBody::ParseChunkedBody():
// _state(S_LENGTH),
// _count(0)
// {
// }

// ParseChunkedBody::ParseChunkedBody(std::string Body):
// _state(S_LENGTH),
// _body(Body),
// _count(0)
// {
// }

// ParseChunkedBody::ParseChunkedBody(ParseChunkedBody const & copy)
// {
// 	*this = copy;
// }

// ParseChunkedBody::~ParseChunkedBody()
// {
// }

// ParseChunkedBody& ParseChunkedBody::operator=(ParseChunkedBody const & ope)
// {
// 	if (this != &ope)
// 	{
// 		this->_body = ope._body;
// 		this->_state = ope._state;
// 		this->_count = ope._count;
// 		this->_nb = ope._nb;
// 	}
// 	return (*this);
	
// }


// void ParseChunkedBody::parse(char c)
// {
// 	switch(_state)
// 	{
// 		case (S_LENGTH):
// 		{
// 			if(isdigit(c))
// 				_nb.push_back(c);
// 			else if(c == '\r')
// 				_state = S_END_R;
// 			else
// 				printf("%s\n","there is supposed to be a '\\r");
// 			break;
// 		}
// 		case(S_BEFORE_BODY):
// 		{
// 			break;
// 		}
// 		case(S_PARSE_BODY):
// 		{
// 			break;
// 		}
// 		case(S_END_R):
// 		{
// 			if(c == '\n')
// 				_state = S_END_N;
// 			else
// 				printf("%s\n","there is supposed to be a '\\n");
			
// 			break;
// 		}
// 		case(S_END_N):
// 		{
// 			if(c == '\r')
// 				_state = S_END;
// 			else
// 			{
// 				/* code */
// 			}
			
// 			else if(isdigit(c) && )
// 				_nb.push_back(c);
// 			else
// 				printf("%s\n","there is supposed to be a '\\r or a digit");
			
// 			break;
// 		}
// 		case(S_END):
// 		{
// 			break;
// 		}
// 	}
// }	
