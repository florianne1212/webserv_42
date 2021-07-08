#include "MethodMiddleware.hpp"
// #include "G"
// #include "File.hpp"

MethodMiddleware::~MethodMiddleware() {
}

void MethodMiddleware::handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	// (void)request;

	if(request.getMethods() == "GET")
	{
		GetMethod myGet;
		myGet.handleGet(client, config,request, response);
	}
	else if(request.getMethods() == "POST")
	{
		PostMethod myPost;
		myPost.handlePost(client,config, request, response);
	}
	else if(request.getMethods() == "DELETE")
	{
		DeleteMethod myDelete;
		myDelete.handleDelete(client,config, request, response);
	}
	else
	{
		std::cout << "this method is not handled : " << request.getMethods() << std::endl;
	}



	std::cout << request.getUrl() << std::endl;

	next();
}