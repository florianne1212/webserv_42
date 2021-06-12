#include "Config.hpp"

std::string Config::getExpression(std::string::iterator &it, std::string::iterator end, std::string err)
{
	it++;
	std::string::iterator begin(it);

	while (*it != '"' && it != end)
		it++;
	if (it == end)
		throw std::string("Expression not found in " + err);
	return std::string(begin, it);
}

size_t Config::getValue(std::string::iterator &it, std::string::iterator end, std::string err)
{
	it++;
	std::string::iterator begin(it);
	int num;

	while (*it != '"' && it != end)
		it++;
	if (it == end)
		throw std::string("Expression not found in " + err);
	std::string ret(begin, it);
	if (ret.find_first_not_of("0123456789") != std::string::npos)
		throw std::string("Expression not found in " + err);
	if ((num = atoi(ret.c_str())) < 0)
		throw std::string("Error in difining parametre " + err);
	return num;
}
void	Config::putConfig()
{
	if (_bodyMaxSize.state == true)
		std::cout <<  "bodyMaxSize:" << _bodyMaxSize.value << std::endl;
	for (std::map<int, std::string>::iterator it = _pathErrorFile.begin(); it != _pathErrorFile.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
	for (std::map<std::string, Server>::iterator it = _serverList.begin(); it != _serverList.end(); it++)
	{
		std::cout << it->first << ":" << std::endl;
		it->second.putServer();
	}
}

Config::Config(std::string setupFile)
: _serverList(), _pathErrorFile(), _bodyMaxSize()
{
	try
	{
		parser(setupFile);
	}
	catch (std::string err)
	{
		std::cerr << err << std::endl;
	}
	putConfig();
}

bool Config::addServer(std::string name, Server &server)
{
	if (_serverList.find(name) != _serverList.end())
		return false;
	_serverList[name] = server;
	return true;
}

bool Config::setPathErrorFile(std::string pathErrorFile)
{
	std::string num;
	size_t i;
	int errorNum;
	if ((i = pathErrorFile.find("/")) == std::string::npos)
		return false;
	num = pathErrorFile.substr(0, i);
	pathErrorFile.erase(0, i);
	if (num.find_first_not_of("0123456789") != std::string::npos)
		return false;
	errorNum = atoi(num.c_str());
	if (errorNum > 599 || errorNum < 300)
		return false;
	if (_pathErrorFile.find(errorNum) != _pathErrorFile.end())
		return false;
	_pathErrorFile[errorNum] = pathErrorFile;
	return true;

}

bool Config::setBodyMaxSize(size_t bodyMaxSize)
{
	if (_bodyMaxSize.state == true)
		return false;
	_bodyMaxSize = usable<size_t>(bodyMaxSize);
	return true;
}

void	Config::parser(std::string setupFile)
{
	std::ifstream ifs(setupFile.c_str());
	std::string buffer;

	//get file and del white space
	std::getline(ifs, buffer, '\0');
	buffer.erase(std::remove_if(buffer.begin(), buffer.end(), ::isspace), buffer.end());

	//parser json
	std::string toCompare;
	std::string::iterator it = buffer.begin();
	if (*it != '{')
		throw std::string("Bad Format: Config scope");
	it++;
	while (it != buffer.end())
	{
		if (*it == '"')
		{
			toCompare = getExpression(it , buffer.end(), "\"unknow\": Config scope");
			it++;
			if (strcmp(toCompare.c_str(), "error_path") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (setPathErrorFile(getExpression(it , buffer.end(), "\"error_path\": Config scope")) == false)
						throw std::string("Error in difining parametre \"error_path\": Config scope");
				}
				else
					throw std::string("Error in difining parametre \"error_path\": Config scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Config scope")) == false)
						throw std::string("Error in difining parametre \"client_body_max_size\": Config scope");
				}
				else
				{
					throw std::string("Error in difining parametre \"client_body_max_size\": Config scope");
				}
			}
			else if (strcmp(toCompare.c_str(), "server") == 0)
			{
				if (*(it) == ':')
					if (*(it + 1) == '{' && *(it + 2) == '"')
					{
						it += 2;
						parserServer(it, buffer);
						std::cout << *(it  - 1) << *it << *(it + 1) << std::endl;
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
								throw std::string("Error in difining parametre \"server\": Config scope");
						}
						if (*it != ']')
							throw std::string("Error \"server\" in array: Config scope");
						it++;
					}
					else
						throw std::string("Error in difining parametre \"server\": Config scope");
				else
					throw std::string("Error in difining parametre \"server\": Config scope");
			}
			else
				throw std::string("No match parametre: Config scope");
			it++;
		}
		else if (*it == ',')
			it++;
		else
			throw std::string("Bad Format: Config scope");
	}
}

void Config::parserServer(std::string::iterator &it, std::string &buffer)
{
	Server tmp;
	std::string toCompare, name;

	while (*it != '}' && it != buffer.end())
	{
		if (*it == '"')
		{
			toCompare = getExpression(it , buffer.end(), "\"unknow\": Server scope");
			it++;
			if (strcmp(toCompare.c_str(), "server_name") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (name != "")
							throw std::string("Error in difining parametre \"server_name\": Server scope");
							name = getExpression(it , buffer.end(), "\"server_name\": Server scope");
				}
				else
					throw std::string("Error in difining parametre \"server_name\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "listen") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setIp(getExpression(it , buffer.end(), "\"listen\": Server scope")) == false)
					throw std::string("Error in difining parametre \"listen\": Server scope");
					if (tmp.setPort() == false)
						throw std::string("Error in difining parametre \"listen\": Server scope");
				}
				else
					throw std::string("Error in difining parametre \"listen\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Server scope")) == false)
						throw std::string("Error in difining parametre \"client_body_max_size\": Server scope");
				}
				else
					throw std::string("Error in difining parametre \"client_body_max_size\": Server scope");
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
								throw std::string("Error in difining parametre \"routes\": Server scope");
						}
						if (*it != ']')
							throw std::string("Error \"routes\" in array: Server scope");
					}
					else
						throw std::string("Error in difining parametre \"routes\": Server scope");
				else
					throw std::string("Error in difining parametre \"routes\": Server scope");
			}
			else
				throw std::string("No match parametre: Server scope");
			it++;
		}
		else if (*it == ',')
			it++;
		else
			throw std::string("File bad Format: Server scope");
	}
	if (it == buffer.end())
		throw std::string("Missin \"}\": Server scope");
	it++;
	if (addServer(name, tmp) == false)
		throw std::string("Error redifining \"" + name + "\": Server scope");
}

void Config::parserRoutes(std::string::iterator &it, std::string &buffer, Server &serv)
{
	Routes tmp;
	std::string toCompare, name;

	while (*it != '}' && it != buffer.end())
	{
		if (*it == '"')
		{
			toCompare = getExpression(it , buffer.end(), "\"unknow\": Routes scope");
			it++;
			if (strcmp(toCompare.c_str(), "location") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (name != "")
							throw std::string("Error in difining parametre \"location\": Routes scope");
							name = getExpression(it , buffer.end(), "\"location\": Routes scope");
				}
				else
					throw std::string("Error in difining parametre \"location\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "directory_page") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setDirectoryPage(getExpression(it, buffer.end(), "\"directory_page\": Routes scope")) == false)
						throw std::string("Error in difining parametre \"directory_page\": Routes scope");
				}
				else
					throw std::string("Error in difining parametre \"directory_page\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "root") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setRoot(getExpression(it, buffer.end(), "\"root\": Routes scope")) == false)
						throw std::string("Error in difining parametre \"root\": Routes scope");
				}
				else
					throw std::string("Error in difining parametre \"root\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "autoIndex") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setAutoIndex(getExpression(it, buffer.end(), "\"autoIndex\": Routes scope")) == false)
						throw std::string("Error in difining parametre \"autoIndex\": Routes scope");
				}
				else
					throw std::string("Error in difining parametre \"autoIndex\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "httpRequest") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setHttpRequest(getExpression(it, buffer.end(), "\"httpRequest\": Routes scope")) == false)
						throw std::string("Error in difining parametre \"httpRequest\": Routes scope");
				}
				else
					throw std::string("Error in difining parametre \"httpRequest\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "httpRedirection") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setHttpRedirection(getExpression(it, buffer.end(), "\"httpRedirection\": Routes scope")) == false)
						throw std::string("Error in difining parametre \"httpRedirection\": Routes scope");
				}
				else
					throw std::string("Error in difining parametre \"httpRedirection\": Routes scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size") == 0)
			{
				if (*(it) == ':' && *(it + 1) == '"')
				{
					it++;
					if (tmp.setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Routes scope")) == false)
						throw std::string("Error in difining parametre \"client_body_max_size\": Routes scope");
				}
				else
					throw std::string("Error in difining parametre \"client_body_max_size\": Routes scope");
			}
			else
				throw std::string("No match parametre: Routes scope");
			it++;
		}
		else if (*it == ',')
			it++;
		else
			throw std::string("File bad Format: Routes scope");
	}
	if (it == buffer.end())
		throw std::string("Missin \"}\": Routes scope");
	it++;
	if (serv.addRoutes(name, tmp) == false)
		throw std::string("Error redifining \"" + name + "\": Routes scope");
}
