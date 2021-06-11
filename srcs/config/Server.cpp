#include "Server.hpp"

Server:: Server()
: _routes(), _ip(), _port(80), _uploadDir(), _bodyMaxSize()
{}

Server::Server(const Server &toCopie)
: _routes(toCopie._routes), _ip(toCopie._ip), _port(toCopie._port), _uploadDir(toCopie._uploadDir),  _bodyMaxSize(toCopie._bodyMaxSize)
{}

bool Server::setIp(std::string ip)
{
	if (_ip.state == true)
		return false;
	if (ip.find_first_not_of("0123456789.:") != std::string::npos)
		return false;
	if (ip.find_first_of(".:") != std::string::npos)
	{
		std::string tmp(ip);
		int val;
		size_t j;
		size_t k;
		for (int i = 0; i < 3; i++)
		{
			if ((j = tmp.find(".")) == std::string::npos)
				return false;
			val = atoi((tmp.substr(k, (j -  1) - k)).c_str());
			if (val > 254 && val < 0)
				return false;
			tmp.erase(k, (j - 1) - k);
			k = j + 1;
		}
		j = tmp.size();
		val = atoi((tmp.substr(k, (j -  1) - k)).c_str());
		if (val > 254 && val < 0)
			return false;
	}
	_ip = usable<std::string>(ip);
	return true;
}

bool Server::setPort()
{
	std::string tmp;
	size_t i;

	if (_port.state == true)
		return false;
	if (_ip.state)
	{
		if ((i = _ip.value.find(':')) == std::string::npos)
		{

			if (_ip.value.find_first_not_of("0123456789") == std::string::npos)
				return false;
			i = 0;
			_ip = usable<std::string>();
		}
		else
			i++;
		tmp = _ip.value.substr(i, _ip.value.size() - i);
		_port = usable<int>(atoi(tmp.c_str()));
	}
	return true;
}

bool Server::setBodyMaxSize(size_t bodyMaxSize)
{
	if (_bodyMaxSize.state == true)
		return false;
	_bodyMaxSize = usable<size_t>(bodyMaxSize);
	return true;
}

bool Server::addRoutes(std::string root, Routes routes)
{
	if (_routes.find(root) == _routes.end())
		_routes[root] = routes;
	else
		return false;
	return true;
}
