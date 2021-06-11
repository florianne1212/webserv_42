#ifndef WEBSERV_ROUTES_HPP
# define WEBSERV_ROUTES_HPP

class Routes;
# include "Includes.hpp"

class Routes
{
	protected :
		usable<char >						_httpRequest;
		usable<std::pair<size_t , std::string> >	_httpRedirection;
		usable<std::string>					_rooted;
		usable<bool>						_autoIndex;
		usable<std::string>					_directoryPage;
		usable<size_t>						_bodyMaxSize;

	public :
		Routes();
		Routes(const Routes & toCopie);
		bool setHttpRequest(char httpRequest);
		bool setHttpRedirection(std::pair<int, std::string> httpRedirection);
		bool setRooted(std::string rooted);
		bool setAutoIndex(bool autoIndex);
		bool setDirectoryPage(std::string directoryPage);
		bool setBodyMaxSize(size_t bodyMaxSize);

		std::vector<std::string> getHttpRequest(void) const;
};

#endif
