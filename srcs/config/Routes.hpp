#ifndef WEBSERV_ROUTES_HPP
# define WEBSERV_ROUTES_HPP

class Routes;

# include <map>
# include <vector>
# include <string>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <sys/stat.h>
# include "usable.hpp"
# include "Config.hpp"

class Routes
{
	private :
		usable<std::vector<std::string> >			_httpRequest;
		usable<std::pair<size_t , std::string> >	_httpRedirection;
		usable<std::string>							_root;
		usable<bool>								_autoIndex;
		usable<std::string>							_directoryPage;
		usable<size_t>								_bodyMaxSize;

		void putRoutes();
		void checker();

	public :
		Routes();
		Routes(const Routes & toCopie);
		bool setHttpRequest(std::vector<std::string> httpRequest);
		bool setHttpRedirection(std::vector<std::string> httpRedirection);
		bool setRoot(std::vector<std::string> root);
		bool setAutoIndex(std::vector<std::string> autoIndex);
		bool setDirectoryPage(std::vector<std::string> directoryPage);
		bool setBodyMaxSize(size_t bodyMaxSize);

		friend class Server;
		friend class Config;
};

#endif
