#include "checkRedir.hpp"
// #include "G"
// #include "File.hpp"

CheckRedir::~CheckRedir() {
}

void CheckRedir::handle(Client &client, Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;




	std::cout << "CHECK REDIR" << std::endl;

	next();
}