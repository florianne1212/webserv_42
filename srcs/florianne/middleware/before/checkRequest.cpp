#include "checkRequest.hpp"
// #include "G"
// #include "File.hpp"

CheckRequest::~CheckRequest() {
}

void CheckRequest::handle(Client &client, Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;

	if(response.getStatus() == 200)
	{
		std::cout << "CHECK REQUEST" << std::endl;
	}


	

	next();
}