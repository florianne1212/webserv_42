#include "IsConnected.hpp"

IsConnected::~IsConnected() {
}

void IsConnected::handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next) {
	(void)client;
	(void)response;
	(void)request;
	(void)config;
	std::map<std::string, std::string> _headers = request.getHeaders();

	if(response.getStatus()/100 == 2)
	{
		//std::string getIp(std::string serverName) const;
		if(_headers.find("Host") != _headers.end())
		{
			usable<std::string> hostServer = config.getServerName(_headers.find("Host")->second);
			if (hostServer.state == false)
			{
				if(config.getIp(_headers.find("Host")->second) == std::string())
					response.setStatus(400);
			}
			else if(hostServer.state == true && hostServer.value != client.getServerName())
			{
					response.setStatus(400);
			}
		}
		else
		{
			response.setStatus(400);
		}
	}

	next();
}