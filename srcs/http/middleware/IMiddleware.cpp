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


}