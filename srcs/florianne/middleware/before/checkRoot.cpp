#include "checkRoot.hpp"
// #include "G"
// #include "File.hpp"
#include <map>

CheckRoot::~CheckRoot() {
}

void CheckRoot::handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;
	(void)config;

	if(response.getStatus() == 200)
	{
		// std::cout << "CHECK ROOT" << std::endl;
		std::map <std::string, std::string> parsedUri = request.getParsedUri();
		usable<std::pair<std::string, std::string> > root;
		
		root = config.getRoot(client.getServerName(), parsedUri.find("path")->second);
	
		if(root.state == true)
		{
			std::string newe = parsedUri.find("path")->second;
			std::string new_second = root.value.second;
			//new_second.insert(new_second.size(), "/");
			newe.replace(newe.find(root.value.first),root.value.first.length(),new_second);
			request.setUrl(newe);
  			// std::cout << "\nURL =" << parsedUri.find("path")->second;
			// std::cout << "\nSTRING 1 =" << root.value.first;
			// std::cout << "\nSTRING 2 =" << root.value.second;
			// std::cout << "\nNEW = " << newe << '\n';
		}
	}
	next();
}