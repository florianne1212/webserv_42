#include "IMiddleware.hpp"

IMiddleware::~IMiddleware()
{
}

void IMiddleware::handle(Client &client, Request &request, Response &response, MiddlewareChain &next)
{
	(void)client;
	(void)response;
	(void)request;
	(void)next;

	/*
	ServerBlock *block = findBlock(request);

	if (block)
	{
		request.setServerBlock(block);
		next();
	} else {
		response.status(404);
		response.body("No server found");
		response.end();
	}
	*/
}