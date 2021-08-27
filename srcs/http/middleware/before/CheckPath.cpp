#include "CheckPath.hpp"


CheckPath::~CheckPath() {
}

void CheckPath::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {

	(void)client;
	(void)response;
	(void)request;
	(void)config;

	std::string url = request.getUrl();
	std::string delimiter = "/";
	std::list<std::string> mylist;

	size_t pos = 0;
	std::string token;
	while ((pos = url.find(delimiter)) != std::string::npos) {
		token = url.substr(0, pos);
		if(token == "..")
		{
			if(!mylist.empty())
				mylist.pop_back();
		}
		else if (token == "."|| token == "")
			;
		else 
		{
			mylist.push_back("/" + token);
		}
		url.erase(0, pos + delimiter.length());
	}
	if(token == "..")
	{
		if(!mylist.empty())
			mylist.pop_back();
	}
	else if (url == ".")
		;
	else 
		mylist.push_back("/" + url);
	

	std::string new_url;
	if(mylist.empty())
		new_url = "/";
	for (std::list<std::string>::iterator it=mylist.begin(); it != mylist.end(); ++it)
		new_url += *it;

	request.setUrl(new_url);

	next();

}