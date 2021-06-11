#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

class Server;

# include "Includes.hpp"

class Server
{
	private:
		std::map<std::string, Routes>	_routes;
		usable<std::string>				_ip;
		usable<unsigned short>			_port;
		usable<std::string>				_uploadDir;
		usable<size_t>					_bodyMaxSize;

	public:
		Server();
		Server(const Server &toCopie);
		bool setIp(std::string ip);
		bool setPort();
		bool setBodyMaxSize(size_t bodyMaxSize);
		bool addRoutes(std::string root, Routes routes);
};

#endif
