#include "Config.hpp"

std::string g_workpath;

static std::vector<std::string> cppSplit(std::string str)
{
	std::vector<std::string> ret;
	std::string token;
	size_t i = 0;

	while (str.size())
	{
		if (::isspace(str[i]))
		{
			while (::isspace(str[i]) && i < str.size())
				i++;
			str.erase(0, i);
			i = 0;
		}
		else
		{
			while (!::isspace(str[i]) && i < str.size())
				i++;
	   		token = str.substr(0, i);
	   		ret.push_back(token);
			str.erase(0, i);
			i = 0;
		}
	}
	return ret;
}

std::vector<std::string> Config::getExpression(std::string::iterator &it, std::string::iterator end, std::string err)
{
	it++;
	std::string::iterator begin(it);

	while (*it != '"' && it != end)
		it++;
	if (it == end)
		throw std::string("Expression not found in " + err);
	return cppSplit(std::string(begin, it));
}

size_t Config::getValue(std::string::iterator &it, std::string::iterator end, std::string err)
{
	it++;
	std::string::iterator begin(it);

	while (*it != '"' && it != end)
		it++;
	if (it == end)
		throw std::string("Expression not found in " + err);
	std::vector<std::string> ret = cppSplit(std::string(begin, it));
	if (ret.size() != 1)
		throw std::string("Expression not found in " + err);
	if (ret[0].find_first_not_of("0123456789") != std::string::npos)
		throw std::string("Expression not found in " + err);
	return atoi(ret[0].c_str());
}
void	Config::putConfig()
{
	if (_workpath.state == true)
		std::cout << "workpath: " << _workpath.value << std::endl;
	if (_bodyMaxSize.state == true)
		std::cout <<  "bodyMaxSize: " << _bodyMaxSize.value << std::endl;
	if (_cgi.state == true)
		std::cout <<  "cgi: " << _cgi.value.first << " " << _cgi.value.second << std::endl;
	for (std::map<int, std::string>::iterator it = _pathErrorFile.begin(); it != _pathErrorFile.end(); it++)
		std::cout << "errorPath: " << it->first << " " << it->second << std::endl;
	for (std::map<std::string, Server>::iterator it = _serverList.begin(); it != _serverList.end(); it++)
	{
		std::cout << it->first << ":" << std::endl;
		it->second.putServer();
	}
}

void	Config::checker()
{
	struct stat useless;
	if (_workpath.state == true)
	{
		if (stat(_workpath.value.c_str(), &useless) != 0)
			throw std::string("workpath is invalid");
		else
			WORKPATH = _workpath.value;
	}
	else
		throw std::string("workpath is undefined");
	for (std::map<int, std::string>::iterator it = _pathErrorFile.begin(); it != _pathErrorFile.end(); it++)
		if (stat((WORKPATH + it->second).c_str() , &useless) != 0)
			throw std::string(WORKPATH + it->second + " is a unknow error file");
	for (std::map<std::string, Server>::iterator it = _serverList.begin(); it != _serverList.end(); it++)
		it->second.checker();
	if (_cgi.state == true)
		if (stat((WORKPATH + _cgi.value.second).c_str() , &useless) != 0)
			throw std::string(_cgi.value.second + " is a unknow cgi file");
}

void	Config::removeIsSpace(std::string &buffer)
{
	std::string::iterator it = buffer.begin();
	while (it != buffer.end())
	{
		if (*it == '"')
		{
			it++;
		 	while (*it != '"' && it != buffer.end())
				it++;
			it++;
		}
		else if (::isspace(*it))
			it = buffer.erase(it);
		else
			it++;
	}
}

Config::Config()
: _serverList(), _pathErrorFile(), _bodyMaxSize(), _cgi()
{}

Config::Config(std::string configFile)
: _workpath(), _serverList(), _pathErrorFile(), _bodyMaxSize(), _cgi()
{
	parser(configFile);
	checker();
	//putConfig();
}

bool Config::setWorkpath(std::vector<std::string> workpath)
{
	if (workpath.size() != 1)
		return false;
	_workpath = usable<std::string>(workpath[0]);
	return true;
}

bool Config::addServer(std::string name, Server &server)
{
	if (_serverList.find(name) != _serverList.end())
		return false;
	_serverList[name] = server;
	return true;
}

bool Config::setPathErrorFile(std::vector<std::string> pathErrorFile)
{
	std::string num;
	int errorNum;

	if (pathErrorFile.size() != 2)
		return false;
	if (pathErrorFile[0].find_first_not_of("0123456789") != std::string::npos)
		return false;
	errorNum = atoi(pathErrorFile[0].c_str());
	if (errorNum > 599 || errorNum < 300)
		return false;
	if (_pathErrorFile.find(errorNum) != _pathErrorFile.end())
		return false;
	_pathErrorFile[errorNum] = pathErrorFile[1];
	return true;

}

bool Config::setBodyMaxSize(size_t bodyMaxSize)
{
	if (_bodyMaxSize.state == true)
		return false;
	_bodyMaxSize = usable<size_t>(bodyMaxSize);
	return true;
}

bool Config::setCGI(std::vector<std::string> cgi)
{
	if (_cgi.state == true || cgi.size() != 2)
		return false;
	if (cgi[0][0] != '.')
		return false;
	_cgi = usable<std::pair<std::string, std::string> >(std::pair<std::string , std::string>(cgi[0], cgi[1]));
	return true;
}

void	Config::parser(std::string setupFile)
{
	std::ifstream ifs(setupFile.c_str());
	std::string buffer;

	int size = setupFile.size();
	if (setupFile[size - 1] != 'f' || setupFile[size - 2] != 'n' || setupFile[size - 3] != 'o' || setupFile[size - 4] != 'c')
		throw std::string("Invalid Config File");
	if (ifs.fail())
		throw std::string("Invalid Config File");
	//get file and del white space
	std::getline(ifs, buffer, '\0');
	ifs.close();
	removeIsSpace(buffer);

	//parser json
	std::vector<std::string> ret;
	std::string toCompare;
	std::string::iterator it = buffer.begin();
	if (*it != '{')
		throw std::string("Bad Format: Config scope");
	it++;
	while (it != buffer.end())
	{
		if (*it == '"')
		{ 
			ret = getExpression(it , buffer.end(), "\"unknow\": Config scope");
			if (ret.size() != 1)
				throw std::string("No match parametre: Config scope");
			toCompare = ret[0];
			it++;
			if (strcmp(toCompare.c_str(), "workpath") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (setWorkpath(getExpression(it, buffer.end(), "\"workpath\": Config scope")) == false)
						throw std::string("Error in defining parametre \"workpath\": Config Scope");
				}
				else
					throw std::string("Error in defining parametre \"workpath\": Config Scope");
			}
			else if (strcmp(toCompare.c_str(), "error_path") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (setPathErrorFile(getExpression(it , buffer.end(), "\"error_path\": Config scope")) == false)
						throw std::string("Error in defining parametre \"error_path\": Config scope");
				}
				else
					throw std::string("Error in defining parametre \"error_path\": Config scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Config scope")) == false)
						throw std::string("Error in defining parametre \"client_body_max_size\": Config scope");
				}
				else
				{
					throw std::string("Error in defining parametre \"client_body_max_size\": Config scope");
				}
			}
			else if (strcmp(toCompare.c_str(), "cgi") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (setCGI(getExpression(it , buffer.end(), "\"cgi\": Config scope")) == false)
						throw std::string("Error in defining parametre \"cgi\": Config scope");
				}
				else
				{
					throw std::string("Error in defining parametre \"client_body_max_size\": Config scope");
				}
			}
			else if (strcmp(toCompare.c_str(), "server") == 0)
			{
				if (*(it) == ':')
					if (*(it + 1) == '{' && *(it + 2) == '"')
					{
						it += 2;
						parserServer(it, buffer);
						it--;
					}
					else if (*(it + 1) == '[' && *(it + 2) ==  '{' && *(it + 3) == '"')
					{
						it += 3;
						parserServer(it, buffer);
						while (*(it) == ',')
						{
							it++;
							if (*it == '{')
							{
								it++;
								parserServer(it, buffer);
							}
							else
								throw std::string("Error in defining parametre \"server\": Config scope");
						}
						if (*it != ']')
							throw std::string("Error \"server\" in array: Config scope");
					}
					else
						throw std::string("Error in defining parametre \"server\": Config scope");
				else
					throw std::string("Error in defining parametre \"server\": Config scope");
			}
			else
				throw std::string("No match parametre: Config scope");
			it++;
		}
		if (*it == ',' && *(it + 1) == '"')
			it++;
		else if (*(it) == '}' && (it + 1) == buffer.end())
			break;
		else
			throw std::string("Bad Format: Config scope");
	}
}

void Config::parserServer(std::string::iterator &it, std::string &buffer)
{
	Server tmp;
	std::vector<std::string> ret;
	std::string toCompare, name;

	while (*it != '}' && it != buffer.end())
	{
		if (*it == '"')
		{
			ret = getExpression(it , buffer.end(), "\"unknow\": Server scope");
			if (ret.size() != 1)
				throw std::string("No match parametre: Server scope");
			toCompare = ret[0];
			it++;
			if (strcmp(toCompare.c_str(), "server_name") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (name != "")
						throw std::string("Error in defining parametre \"server_name\": Server scope");
					ret = getExpression(it , buffer.end(), "\"server_name\": Server scope");
					if (ret.size() != 1)
						throw std::string("Error in defining parametre \"server_name\": Server scope");
					name = ret[0];
				}
				else
					throw std::string("Error in defining parametre \"server_name\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "listen") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setIp(getExpression(it , buffer.end(), "\"listen\": Server scope")) == false)
						throw std::string("Error in defining parametre \"listen\": Server scope");
					if (tmp.setPort() == false)
						throw std::string("Error in defining parametre \"listen\": Server scope");
				}
				else
					throw std::string("Error in defining parametre \"listen\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "upload_pass") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setUploadDir(getExpression(it , buffer.end(), "\"upload_pass\": Server scope")) == false)
						throw std::string("Error in defining parametre \"upload_pass\": Server scope");
				}
				else
					throw std::string("Error in defining parametre \"upload_pass\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Server scope")) == false)
						throw std::string("Error in defining parametre \"client_body_max_size\": Server scope");
				}
				else
					throw std::string("Error in defining parametre \"client_body_max_size\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "routes") == 0)
			{
				if (*(it) == ':')
					if (*(it + 1) == '{' && *(it + 2) == '"')
					{
						it += 2;
						parserRoutes(it, buffer, tmp);
						it--;
					}
					else if (*(it + 1) == '[' && *(it + 2) ==  '{' && *(it + 3) == '"')
					{
						it += 3;
						parserRoutes(it, buffer, tmp);
						while (*(it) == ',')
						{
							it++;
							if (*it == '{')
							{
								it++;
								parserRoutes(it, buffer, tmp);
							}
							else
								throw std::string("Error in defining parametre \"routes\": Server scope");
						}
						if (*it != ']')
							throw std::string("Error \"routes\" in array: Server scope");
					}
					else
						throw std::string("Error in defining parametre \"routes\": Server scope");
				else
					throw std::string("Error in defining parametre \"routes\": Server scope");
			}
			else
				throw std::string("No match parametre: Server scope");
			it++;
		}

		if (*it == ',' && (*(it + 1) == '"' || *(it + 1) == '{'))
			it++;
		else if (*(it) == '}')
			break;
		else
			throw std::string("File bad Format: Server scope");
	}
	if (it == buffer.end())
		throw std::string("Missin \"}\": Server scope");
	it++;
	if (addServer(name, tmp) == false)
		throw std::string("Error redefining \"" + name + "\": Server scope");
}

void Config::parserRoutes(std::string::iterator &it, std::string &buffer, Server &serv)
{
	Routes tmp;
	std::vector<std::string> ret;
	std::string toCompare, name;

	while (*it != '}' && it != buffer.end())
	{
		if (*it == '"')
		{
			ret = getExpression(it , buffer.end(), "\"unknow\": Routes scope");
			if (ret.size() != 1)
				throw std::string("No match parametre: Routes scope");
			toCompare = ret[0];
			it++;
			if (strcmp(toCompare.c_str(), "location") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (name != "")
						throw std::string("Error in defining parametre \"location\": Routes scope");
					ret = getExpression(it , buffer.end(), "\"location\": Routes scope");
					if (ret.size() != 1)
						throw std::string("Error in defining parametre \"location\": Routes scope");
					name = ret[0];
				}
				else
					throw std::string("Error in defining parametre \"location\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "directory_page") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setDirectoryPage(getExpression(it, buffer.end(), "\"directory_page\": Routes scope")) == false)
						throw std::string("Error in defining parametre \"directory_page\": Routes scope");
				}
				else
					throw std::string("Error in defining parametre \"directory_page\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "root") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setRoot(getExpression(it, buffer.end(), "\"root\": Routes scope")) == false)
						throw std::string("Error in defining parametre \"root\": Routes scope");
				}
				else
					throw std::string("Error in defining parametre \"root\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "autoIndex") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setAutoIndex(getExpression(it, buffer.end(), "\"autoIndex\": Routes scope")) == false)
						throw std::string("Error in defining parametre \"autoIndex\": Routes scope");
				}
				else
					throw std::string("Error in defining parametre \"autoIndex\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "httpRequest") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setHttpRequest(getExpression(it, buffer.end(), "\"httpRequest\": Routes scope")) == false)
						throw std::string("Error in defining parametre \"httpRequest\": Routes scope");
				}
				else
					throw std::string("Error in defining parametre \"httpRequest\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "httpRedirection") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setHttpRedirection(getExpression(it, buffer.end(), "\"httpRedirection\": Routes scope")) == false)
						throw std::string("Error in defining parametre \"httpRedirection\": Routes scope");
				}
				else
					throw std::string("Error in defining parametre \"httpRedirection\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Routes scope")) == false)
						throw std::string("Error in defining parametre \"client_body_max_size\": Routes scope");
				}
				else
					throw std::string("Error in defining parametre \"client_body_max_size\": Routes scope");
			}
			else
				throw std::string("No match parametre: Routes scope");
			it++;
		}
		if (*it == ',' && (*(it + 1) == '"' || *(it + 1) == '{'))
			it++;
		else if (*(it) == '}')
			break;
		else
			throw std::string("File bad Format: Routes scope");
	}
	if (it == buffer.end())
		throw std::string("Missin \"}\": Routes scope");
	it++;
	if (serv.addRoutes(name, tmp) == false)
		throw std::string("Error redefining \"" + name + "\": Routes scope");
}

usable<std::string> Config::getPathErrorFile(int errorVal) const
{
	std::map<int, std::string>::const_iterator it;
	it = _pathErrorFile.find(errorVal);
	if (it != _pathErrorFile.end())
		return usable<std::string>(it->second);
	return usable<std::string>();
}

usable<std::pair<std::string, std::string> > Config::getCGI()
{
	return (_cgi);
}

size_t Config::getBodyMaxSize(std::string serverName, std::string path) const
{
	size_t ret;

	if (_bodyMaxSize.state == false)
		ret = 1000000;
	else
		ret = _bodyMaxSize.value;

	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		if (server->second._bodyMaxSize.state == true)
			ret = server->second._bodyMaxSize.value;

		size_t i = 1;
		std::string toCompare;
		std::map<std::string, Routes>::const_iterator routes;
		while (i != std::string::npos)
		{
			toCompare = path.substr(0,i);
			if ((routes = server->second._routes.find(toCompare)) != server->second._routes.end())
			{
				if (routes->second._bodyMaxSize.state == true)
					ret = routes->second._bodyMaxSize.value;
			}
			i++;
			i = path.find('/', i);
		}
		if ((routes = server->second._routes.find(path)) != server->second._routes.end())
		{
			if (routes->second._bodyMaxSize.state == true)
				ret = routes->second._bodyMaxSize.value;
		}
	}

	return ret;
}

std::map<std::string, Server>::const_iterator Config::getServerBegin(void) const
{
	return _serverList.begin();
}

std::map<std::string, Server>::const_iterator Config::getServerEnd(void) const
{
	return _serverList.end();
}

usable<std::string> Config::getServerName(std::string ip) const
{
	std::string adr_ip;
	size_t port_ip;

	if (strncmp(ip.c_str(), "localhost", 9) == 0)
	{
		if (ip.substr(9).find_first_not_of("0123456789:") != std::string::npos)
			return usable<std::string>();
		else
			adr_ip = "127.0.0.1";
	}
	else if (ip.find_first_not_of("0123456789.:") != std::string::npos)
		return usable<std::string>();
	else
	{
		adr_ip = ip.substr(0, ip.find(":"));
	}
	if (ip.find(":") != std::string::npos)
		port_ip = atoi(ip.substr(ip.find(":") + 1).c_str());
	else
		port_ip = 80;

	for (std::map<std::string, Server>::const_iterator it = _serverList.begin(); it != _serverList.end(); it++)
	{
		if (adr_ip == getIp(it->first) && port_ip == getPort(it->first))
		{
			return usable<std::string>(it->first);
		}
	}
	return usable<std::string>();
}

std::string Config::getIp(std::string serverName) const
{
	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		if (server->second._ip.state == true)
			return server->second._ip.value;
		else
			return "127.0.0.1";
	}
	return std::string();
}

unsigned short Config::getPort(std::string serverName) const
{
	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		if (server->second._port.state == true)
			return server->second._port.value;
		else
			return 0;
	}
	return 0;
}

usable<std::string> Config::getUploadDir(std::string serverName) const
{
	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
			return server->second._uploadDir;
	return usable<std::string>();
}

std::vector<std::string> Config::getHttpRequest(std::string serverName, std::string path) const
{
	std::vector<std::string> ret;

	ret.push_back("GET");
	ret.push_back("POST");
	ret.push_back("DELETE");
	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		size_t i = 1;
		std::string toCompare;
		std::map<std::string, Routes>::const_iterator routes;
		while (i != std::string::npos)
		{
			toCompare = path.substr(0,i);
			if ((routes = server->second._routes.find(toCompare)) != server->second._routes.end())
			{
				if (routes->second._httpRequest.state == true)
					ret = routes->second._httpRequest.value;
			}
			i++;
			i = path.find('/', i);
		}
		if ((routes = server->second._routes.find(path)) != server->second._routes.end())
		{
			if (routes->second._httpRequest.state == true)
				ret = routes->second._httpRequest.value;
		}
	}
	return ret;
}

usable<std::pair<size_t , std::string> > Config::getHttpRedirection(std::string serverName, std::string path) const
{
	usable<std::pair<size_t , std::string> > ret;

	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		size_t i = 1;
		std::string toCompare;
		std::map<std::string, Routes>::const_iterator routes;
		while (i != std::string::npos)
		{
			toCompare = path.substr(0,i);
			if ((routes = server->second._routes.find(toCompare)) != server->second._routes.end())
			{
				if (routes->second._httpRedirection.state == true)
					ret = routes->second._httpRedirection;
			}
			i++;
			i = path.find('/', i);
		}
		if ((routes = server->second._routes.find(path)) != server->second._routes.end())
		{
			if (routes->second._httpRedirection.state == true)
				ret = routes->second._httpRedirection;
		}
	}
	return ret;
}

usable<std::pair<std::string, std::string> > Config::getRoot(std::string serverName, std::string path) const
{
	usable<std::pair<std::string, std::string> > ret;

	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		size_t i = 1;
		std::string toCompare;
		std::map<std::string, Routes>::const_iterator routes;
		while (i != std::string::npos)
		{
			toCompare = path.substr(0,i);
			if ((routes = server->second._routes.find(toCompare)) != server->second._routes.end())
			{
				if (routes->second._root.state == true)
				{
					ret = usable<std::pair<std::string, std::string> >(std::pair<std::string, std::string>(toCompare, routes->second._root.value));
				}
			}
			i++;
			i = path.find('/', i);
		}
		if ((routes = server->second._routes.find(path)) != server->second._routes.end())
		{
			if (routes->second._root.state == true)
				ret = usable<std::pair<std::string, std::string> >(std::pair<std::string, std::string>(path, routes->second._root.value));
		}
	}
	return ret;
}

bool Config::getAutoIndex(std::string serverName, std::string path) const
{
	bool ret;

	ret = 0;
	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		size_t i = 1;
		std::string toCompare;
		std::map<std::string, Routes>::const_iterator routes;
		while (i != std::string::npos)
		{
			toCompare = path.substr(0,i);
			if ((routes = server->second._routes.find(toCompare)) != server->second._routes.end())
			{
				if (routes->second._autoIndex.state == true)
					ret = routes->second._autoIndex.value;
			}
			i++;
			i = path.find('/', i);
		}
		if ((routes = server->second._routes.find(path)) != server->second._routes.end())
		{
			if (routes->second._autoIndex.state == true)
				ret = routes->second._autoIndex.value;
		}
	}
	return ret;
}

usable<std::string> Config::getDirectoryPage(std::string serverName, std::string path) const
{
	usable<std::string> ret;

	std::map<std::string, Server>::const_iterator server;
	if ((server = _serverList.find(serverName)) != _serverList.end())
	{
		std::map<std::string, Routes>::const_iterator routes;
		if ((routes = server->second._routes.find(path)) != server->second._routes.end())
		{
			if (routes->second._directoryPage.state == true)
				ret = routes->second._directoryPage;
		}
	}
	return ret;
}
