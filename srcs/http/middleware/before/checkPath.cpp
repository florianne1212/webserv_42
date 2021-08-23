#include "checkPath.hpp"


CheckPath::~CheckPath() {
}

void CheckPath::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {

	(void)client;
	(void)response;
	(void)request;
	(void)config;
	
	std::cout << "\nI AM IN CHECK PATH !!! URL =  \n" << request.getUrl()  << "\n";

	std::string url = request.getUrl();
	std::string delimiter = "/";
	std::list<std::string> mylist;

	size_t pos = 0;
	std::string token;
	while ((pos = url.find(delimiter)) != std::string::npos) {
		token = url.substr(0, pos);
		if(token == "..")
			mylist.pop_back();
		else if (token == "."|| token == "")
			;
		else 
		{
			mylist.push_back("/" + token);
			std::cout << "token";
		}
		// std::cout << "\n my token = ." << token << "." << std::endl;
		url.erase(0, pos + delimiter.length());
	}
	if(url == "..")
		mylist.pop_back();
	else if (url == ".")
		;
	else 
		mylist.push_back("/" + url);
	// std::cout << url << std::endl;

	// std::cout << "mylist contains:";
	// for (std::list<std::string>::iterator it=mylist.begin(); it != mylist.end(); ++it)
    // 	std::cout << ' ' << *it;

	// std::cout << "\n" << std::endl;

	std::string new_url;
	for (std::list<std::string>::iterator it=mylist.begin(); it != mylist.end(); ++it)
		new_url += *it;
	std::cout << "\n new url ==" << new_url << std::endl;

	next();

}