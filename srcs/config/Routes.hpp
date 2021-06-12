#ifndef WEBSERV_ROUTES_HPP
# define WEBSERV_ROUTES_HPP

class Routes;
# include "Includes.hpp"

class Routes
{
	protected :
		usable<std::vector<std::string> >			_httpRequest;
		usable<std::pair<size_t , std::string> >	_httpRedirection;
		usable<std::string>							_root;
		usable<bool>								_autoIndex;
		usable<std::string>							_directoryPage;
		usable<size_t>								_bodyMaxSize;

	public :
		Routes();
		Routes(const Routes & toCopie);
		bool setHttpRequest(std::string httpRequest);
		bool setHttpRedirection(std::string httpRedirection);
		bool setRoot(std::string root);
		bool setAutoIndex(std::string autoIndex);
		bool setDirectoryPage(std::string directoryPage);
		bool setBodyMaxSize(size_t bodyMaxSize);

		void putRoutes();
};

#endif
