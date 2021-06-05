#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

class Server
{
	private:
		map<std::string, Routes>		_routes;
		int								_port;
		usable<size_t>					_bodyMaxSize;


	public:
		Server();
		Server(const &server toCopie);
		void setPort(int port);
		void setBodyMaxSize(size_t bodyMaxSize);
		void addRoutes(std::string root, Routes routes);
}

#endif
