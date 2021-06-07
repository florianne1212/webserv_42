#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

class Server
{
	private:
		std::map<std::string, Routes>	_routes;
		usable<std::string>				_ip;
		usable<int>						_port;
		usable<std::string>				_uploadDir;
		usable<size_t>					_bodyMaxSize;

	public:
		Server();
		Server(const &server toCopie);
		void setIp(std::string ip);
		void setPort(int port);
		void setBodyMaxSize(size_t bodyMaxSize);
		void addRoutes(std::string root, Routes routes);
}

#endif
