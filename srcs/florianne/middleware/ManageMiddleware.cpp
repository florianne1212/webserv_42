
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

	middlewares.push_back(new IsConnected());
	middlewares.push_back(new CheckRequest());
	middlewares.push_back(new CheckBodySize());
	middlewares.push_back(new CheckRedir());
	middlewares.push_back(new CheckRoot());
			
	middlewares.push_back(new MethodMiddleware());

	middlewares.push_back(new GetFileError());
			
			
	_state = S_END;

	MiddlewareChain chain(middlewares, config, client, request, response);
	chain();
}

	