#ifndef WEBSERV_ROUTES_HPP
# define WEBSERV_ROUTES_HPP

class Routes
{
	protected :
		usable<char >						_httpRequest;
		usable<pair<int , std::string> >	_httpRedirection;
		usable<std::string>					_rooted;
		usable<bool>						_autoIndex;
		usable<std::string>					_directoryPage;
		usable<size_t>						_bodyMaxSize;

	public :
		Routes();
		Routes(const Routes & toCopie);
		void setHttpRequest(char httpRequest);
		void setHttpRedirection(pair<int, std::string> httpRedirection);
		void setRooted(std::string rooted);
		void setAutoIndex(bool autoIndex);
		void setDirectoryPage(std::string directoryPage);
		void setBodyMaxSize(size_t bodyMaxSize);

		vector<std::string> getHttpRequest(void) const;
}

#endif
