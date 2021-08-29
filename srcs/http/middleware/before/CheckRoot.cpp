#include "CheckRoot.hpp"
#include <map>

CheckRoot::~CheckRoot() {
}

void CheckRoot::handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next) {
	
	(void)client;
	(void)response;
	(void)request;
	(void)config;

	if(response.getStatus()/100 == 2)
	{
		std::map <std::string, std::string> parsedUri = request.getParsedUri();
		usable<std::pair<std::string, std::string> > root;
		
		root = config.getRoot(client.getServerName(), parsedUri.find("path")->second);
	
		if(root.state == true)
		{
			std::string newe = parsedUri.find("path")->second;
			std::string new_second = root.value.second;
			newe.replace(newe.find(root.value.first),root.value.first.length(),new_second);
			request.setUrl(newe);
		}
	}
	next();
}