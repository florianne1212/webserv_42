#include "isConnected.hpp"
// #include "G"
// #include "File.hpp"

IsConnected::~IsConnected() {
}

void IsConnected::handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;
	(void)config;
	std::map<std::string, std::string> _headers = request.getHeaders();

	if(response.getStatus()/100 == 2)
	{
		if(_headers.find("Host") != _headers.end())
		{
			usable<std::string> hostServer = config.getServerName(_headers.find("Host")->second);
			if(hostServer.state == true && hostServer.value != client.getServerName())
				response.setStatus(400);
		}
		else
		{
			response.setStatus(400);
		}
	}

	//choper host dans header
	//comparer avec server name du client
	//si diff erreur 403

	next();
}