#include "CheckRequest.hpp"

CheckRequest::~CheckRequest() {
}

void CheckRequest::handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next) {
	

	(void)client;
	(void)response;
	(void)request;
	(void)config;

	if(response.getStatus()/100 == 2)
	{
		int i = 0;
		std::vector<std::string> allowed_method;
		allowed_method = config.getHttpRequest(client.getServerName(), request.getUrl());

		
		for (std::vector<std::string>::iterator it = allowed_method.begin() ; it != allowed_method.end(); ++it)
			if(request.getMethods() == *it)
				i = 1;
		if (i == 0)
			response.setStatus(405);	
	}
	next();
}