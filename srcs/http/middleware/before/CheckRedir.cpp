#include "CheckRedir.hpp"

CheckRedir::~CheckRedir() {
}

void CheckRedir::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {
	
	(void)client;
	(void)response;
	(void)request;
	(void)config;
	
	if(response.getStatus()/100 == 2)
	{
		usable<std::pair<size_t , std::string> > redir;
		redir = config.getHttpRedirection(client.getServerName(), request.getUrl());

		if(redir.state == true)
		{
			response.setStatus(redir.value.first);
			response.setHeaders("Location", redir.value.second);
		}
	}


	next();
}