
#include "ManageMiddleware.hpp"


ManageMiddleware::ManageMiddleware():
_state(S_BEFORE)
{
}


ManageMiddleware::~ManageMiddleware()
{
}

void ManageMiddleware::middlewareStart(ClientSocket &client, Config &config, Request &request, Response &response)
{
	std::list<IMiddleware *> middlewares;

	middlewares.push_back(&IsConnected::instance());
	middlewares.push_back(&CheckRedir::instance());
	middlewares.push_back(&CheckPath::instance());
	middlewares.push_back(&CheckRoot::instance());
	middlewares.push_back(&CheckRequest::instance());
	middlewares.push_back(&CheckBodySize::instance());
			
	middlewares.push_back(&MethodMiddleware::instance());

	middlewares.push_back(&GetFileError::instance());
			
			
	_state = S_END;

	MiddlewareChain chain(middlewares, config, client, request, response);
	chain();
}

	