
#include "ManageMiddleware.hpp"


ManageMiddleware::ManageMiddleware():
_state(S_BEFORE)
{
}


// ManageMiddleware::ManageMiddleware(ManageMiddleware const & copy)
// {
// 	*this = copy;
// }

ManageMiddleware::~ManageMiddleware()
{
}

void ManageMiddleware::middlewareStart(ClientSocket &client, Config &config, Request &request, Response &response)
{
	std::list<IMiddleware *> middlewares;
	// switch(_state)
	// {
	// 	case(S_BEFORE):
	// 	{
			middlewares.push_back(new CheckRedir());
			middlewares.push_back(new CheckRequest());
			middlewares.push_back(new IsConnected());
		// 	_state = S_METHOD;
		// 	break;
		// }
		// case(S_METHOD):
		// {
			middlewares.push_back(new MethodMiddleware());
			_state = S_END;
		// 	break;
		// }
		// case(S_AFTER):
		// {

		// 	break;
		// }
	// 	case(S_END):
	// 	{
	// 		break;
	// 	}
	// }

	
	MiddlewareChain chain(middlewares, config, client, request, response);
	chain();
}

	