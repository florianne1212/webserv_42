#include "CheckBodySize.hpp"


CheckBodySize::~CheckBodySize() {
}

void CheckBodySize::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {

	(void)client;
	(void)response;
	(void)request;
	(void)config;
	
	if(response.getStatus()/100 == 2 && request.getError() == true)
	{
		//std::cout << "\nTOO LONG\n";
		response.setStatus(413);
	}

	next();
}