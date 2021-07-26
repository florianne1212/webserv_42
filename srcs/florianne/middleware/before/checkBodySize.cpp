#include "checkBodySize.hpp"
// #include "G"
// #include "File.hpp"

CheckBodySize::~CheckBodySize() {
}

void CheckBodySize::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;
	(void)config;
	
	if(response.getStatus()/100 == 2)
	{
		if(request.getBody().size() > config.getBodyMaxSize(client.getServerName(), request.getUrl()))
			response.setStatus(413);



		// std::cout << "\nSIZE BODY =" << request.getBody().size();
		// std::cout << "\nMAX SIZE =" << config.getBodyMaxSize(client.getServerName(), request.getUrl());
	}

	next();
}