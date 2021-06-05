Server::server()
: _name(), _port(80)
{}

Server::Server(const &server toCopie)
: _name(toCopie._name), _port(toCopie._port), _bodyMaxSize(toCopie._bodyMaxSize)
{}

void Server::setPort(int port)
{
	_port = port;
}

void setBodyMaxSize(size_t bodyMaxSize)
{
	_bodyMaxSize = bodyMaxSize;
}

bool addRoutes(std::string root, Routes routes)
{
	if (_routes.find(root) == _routes.end())
		_routes[root] = routes;
	else
		return false;
	return true;
}
