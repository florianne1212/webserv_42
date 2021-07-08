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




	std::cout << "IS CONNECTED" << std::endl;

	next();
}