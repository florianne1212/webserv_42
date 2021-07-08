#include "IMiddleware.hpp"

IMiddleware::~IMiddleware()
{
}

void IMiddleware::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next)
{
	(void)client;
	(void)response;
	(void)request;
	(void)next;
	(void)config;

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