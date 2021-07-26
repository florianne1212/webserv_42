#include "Server.hpp"

void Server::putServer()
{
	if (_bodyMaxSize.state == true)
		std::cout <<  "- bodyMaxSize: " << _bodyMaxSize.value << std::endl;
	if (_uploadDir.state == true)
		std::cout <<  "- uploadDir: " << _uploadDir.value << std::endl;
	if (_ip.state == true)
		std::cout <<  "- ip: " << _ip.value << std::endl;
	if (_port.state == true)
		std::cout << "- port: " << _port.value << std::endl;
	for (std::map<std::string, Routes>::iterator it = _routes.begin(); it != _routes.end(); it++)
	{
		std::cout << "- " << it->first << ":" << std::endl;
		it->second.putRoutes();
	}
}

void Server::checker()
{
	struct stat useless;
	if (_ip.state == true && _port.state == false)
		throw std::string("error in listen configuration");
	if (_uploadDir.state)
		if (stat((WORKPATH + _uploadDir.value).c_str(), &useless) != 0)
			throw std::string(_uploadDir.value + " is a unknow upload directory");
	for (std::map<std::string, Routes>::iterator it = _routes.begin(); it != _routes.end(); it++)
		it->second.checker();
}

Server::Server()
: _routes(), _ip(), _port(), _uploadDir(), _bodyMaxSize()
{}

Server::Server(const Server &toCopie)
: _routes(toCopie._routes), _ip(toCopie._ip), _port(toCopie._port), _uploadDir(toCopie._uploadDir),  _bodyMaxSize(toCopie._bodyMaxSize)
{}

bool Server::setIp(std::vector<std::string> ip)
{
	if (_ip.state == true || _port.state == true || ip.size() != 1)
		return false;
	if (ip[0].find_first_not_of("0123456789.:") != std::string::npos)
		return false;
	if (ip[0].find_first_of(".:") != std::string::npos)
	{
		std::string tmp(ip[0]);
		int val;
		size_t j;

		for (int i = 0; i < 3; i++)
		{
			if ((j = tmp.find(".")) == std::string::npos)
				return false;
			val = atoi(tmp.substr(0, j).c_str());
			if (val > 254 || val < 0)
				return false;
			tmp.erase(0, j + 1);
		}
		j = tmp.size();
		val = atoi(tmp.substr(0, j).c_str());
		if (val > 254 && val < 0)
			return false;
	}
	_ip = usable<std::string>(ip[0]);
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
			{
				_port = usable<unsigned short>(atoi(_ip.value.c_str()));
				_ip = usable<std::string>();
			}
			else
				_port = usable<unsigned short>(80);
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
bool Server::setUploadDir(std::vector<std::string>  uploadDir)
{
	if (_uploadDir.state == true || uploadDir.size() != 1)
		return false;
	_uploadDir = usable<std::string>(uploadDir[0]);
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
