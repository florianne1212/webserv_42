#include "checkRedir.hpp"
// #include "G"
// #include "File.hpp"

CheckRedir::~CheckRedir() {
}

void CheckRedir::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;
	(void)config;




	std::cout << "CHECK REDIR" << std::endl;

	next();
}