#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

class Server;

# include <map>
# include <vector>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <sys/stat.h>
# include "usable.hpp"
# include "Routes.hpp"

class Server
{
	private :
		std::map<std::string, Routes>	_routes;
		usable<std::string>				_ip;
		usable<unsigned short>			_port;
		usable<std::string>				_uploadDir;
		usable<size_t>					_bodyMaxSize;

		void	putServer();
		void 	checker();

	public :
		Server();
		Server(const Server &toCopie);
		bool setIp(std::vector<std::string> ip);
		bool setPort();
		bool setUploadDir(std::vector<std::string> uploadDir);
		bool setBodyMaxSize(size_t bodyMaxSize);
		bool addRoutes(std::string root, Routes routes);

		friend class Config;
};

#endif
