#include "MethodMiddleware.hpp"
// #include "G"
// #include "File.hpp"

MethodMiddleware::~MethodMiddleware() {
}

void MethodMiddleware::handle(Client &client, Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	// (void)request;

	if(request.getMethods() == "GET")
	{
		GetMethod myGet;
		myGet.handleGet(client, request, response);
	}
	else if(request.getMethods() == "POST")
	{
		PostMethod myPost;
		myPost.handlePost(client, request, response);
	}
	else if(request.getMethods() == "DELETE")
	{
		DeleteMethod myDelete;
		myDelete.handleDelete(client, request, response);
	}
	else
	{
		std::cout << "this method is not handled" << std::endl;
	}



	std::cout << request.getUrl() << std::endl;

	next();
}