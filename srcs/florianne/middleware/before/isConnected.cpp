#include "isConnected.hpp"
// #include "G"
// #include "File.hpp"

IsConnected::~IsConnected() {
}

void IsConnected::handle(Client &client, Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;




	std::cout << "IS CONNECTED" << std::endl;

	next();
}