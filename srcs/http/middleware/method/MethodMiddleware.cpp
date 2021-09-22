#include "MethodMiddleware.hpp"

MethodMiddleware::~MethodMiddleware() {
}

void MethodMiddleware::handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next) {

	(void)client;
	(void)response;

	if(response.getStatus()/100 == 2)
	{
		if (client.getcgiState() != NO_CGI)
		{
			response.setCgi(true);
		}
		else if ((config.getCGI().state) && (request.getUrl().find(config.getCGI().value.first) != std::string::npos)
			&& (request.getMethods() == "GET" || request.getMethods() == "POST"))
		{
			CgiHandler myCgi(&client, config, request, &response);
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