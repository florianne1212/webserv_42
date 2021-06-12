#include "Server.hpp"

void Server::putServer()
{
	if (_bodyMaxSize.state == true)
		std::cout <<  "- bodyMaxSize :" << _bodyMaxSize.value << std::endl;
	if (_uploadDir.state == true)
		std::cout <<  "- uploadDir :" << _uploadDir.value << std::endl;
	if (_ip.state == true)
		std::cout <<  "- ip :" << _ip.value << std::endl;
	if (_port.state == true)
		std::cout << "- port :" << _port.value << std::endl;
	for (std::map<std::string, Routes>::iterator it = _routes.begin(); it != _routes.end(); it++)
	{
		std::cout << "- " << it->first << ":" << std::endl;
		it->second.putRoutes();
	}
}

Server::Server()
: _routes(), _ip(), _port(), _uploadDir(), _bodyMaxSize()
{}

Server::Server(const Server &toCopie)
: _routes(toCopie._routes), _ip(toCopie._ip), _port(toCopie._port), _uploadDir(toCopie._uploadDir),  _bodyMaxSize(toCopie._bodyMaxSize)
{}

bool Server::setIp(std::string ip)
{
	if (_ip.state == true || _port.state == true)
		return false;
	if (ip.find_first_not_of("0123456789.:") != std::string::npos)
		return false;
	if (ip.find_first_of(".:") != std::string::npos)
	{
		std::string tmp(ip);
		int val;
		size_t j;
		size_t k = 0;

		for (int i = 0; i < 3; i++)
		{
			if ((j = tmp.find(".")) == std::string::npos)
				return false;
			val = atoi(tmp.substr(k, j - k).c_str());
			if (val > 254 || val < 0)
				return false;
			tmp.erase(k, (j - 1) - k);
			k = j + 1;
		}
		j = tmp.size();
		val = atoi(tmp.substr(k, j - k).c_str());
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
			if (_ip.value.find_first_not_of("0123456789") != std::string::npos)
				return false;
			_port = usable<int>(atoi(_ip.value.c_str()));
			_ip = usable<std::string>();
			return true;
		}
		else
			i++;
		tmp = _ip.value.substr(i, _ip.value.size() - i);
			i--;
		_ip.value.erase(i, _ip.value.size() - i);
		_port = usable<int>(atoi(tmp.c_str()));
		return true;
	}
	return false;
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
