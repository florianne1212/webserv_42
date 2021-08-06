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

	if(response.getStatus()/100 == 2)
	{
		if ((config.getCGI().state) && (request.getUrl().find(config.getCGI().value.first) != std::string::npos) && (request.getMethods() == "GET" || request.getMethods() == "POST"))
		{
			std::cout << "coucou mon lolo\n url = " << request.getUrl() << "\n";
			CgiHandler myCgi(client, config, request, &response); //a completer
			myCgi.executeCgi();
		}
		else if(request.getMethods() == "GET")
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
			throw std::string( "this method is not handled ");
	}

	next();
}