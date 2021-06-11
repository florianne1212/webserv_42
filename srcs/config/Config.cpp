#include "Config.hpp"

std::string Config::getExpression(std::string::iterator &it, std::string::iterator end, std::string err)
{
	std::string::iterator begin(it);

	while (*it != '"' && it != end)
		it++;
	if (it == end)
		throw std::string("Expression not found in " + err);
	return std::string(begin, it);
}

size_t Config::getValue(std::string::iterator &it, std::string::iterator end, std::string err)
{
	std::string::iterator begin(it);
	int num;

	while (*it != '"' && it != end)
		it++;
	if (it == end)
		throw std::string("Expression not found in " + err);
	std::string ret(begin, it);
	if ((num = atoi(ret.c_str())) < 0)
		throw std::string("Error in difining parametre " + err);
	return num;
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
	num = pathErrorFile.substr(0, i - 1);
	pathErrorFile.erase(0, i - 1);
	if (num.find_first_not_of("0123456789") == std::string::npos)
		return false;
	errorNum = atoi(num.c_str());
	if (errorNum <= 599 && errorNum >= 300)
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

	for (std::string::iterator it = buffer.begin(); it < buffer.end();)
	{
		if (*it == '"')
		{
			toCompare = getExpression(it , buffer.end(), "\"unknow\": Config scope");
			if (strcmp(toCompare.c_str(), "error_path") == 0)
			{
				if (*(it + 1) == ':' && *(it + 2) == '"')
				{
					it += 2;
					if (setPathErrorFile(getExpression(it , buffer.end(), "\"error_path\": Config scope")) == false)
						throw std::string("Error in difining parametre \"error_path\": Config scope");
				}
				else
					throw std::string("Error in difining parametre \"error_path\": Config scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size"))
			{
				if (*(it + 1) == ':' && *(it + 2) == '"')
				{
					it += 2;
					if (setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Config scope")) == false)
						throw std::string("Error in difining parametre \"client_body_max_size\": Config scope");
				}
				else
					throw std::string("Error in difining parametre \"client_body_max_size\": Config scope");
			}
			else if (strcmp(toCompare.c_str(), "server"))
			{
				if (*(it + 1) == ':')
					if (*(it + 2) == '{' && *(it + 3) == '"')
					{
						it += 3;
						parserServer(it, buffer);
					}
					else if (*(it + 2) == '[' && *(it + 3) ==  '{' && *(it + 4) == '"')
					{
						it += 4;
						parserServer(it, buffer);
						while (*(it) == ',')
						{
							it++;
							if (*it == '{')
								parserServer(it, buffer);
							else
								throw std::string("Error in difining parametre \"server\": Config scope");
						}
					}
					else
						throw std::string("Error in difining parametre \"server\": Config scope");
				else
					throw std::string("Error in difining parametre \"server\": Config scope");

			}
			else
				throw std::string("No match parametre: Config scope");
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
	std::string toCompare;

	while (*it != '}' && it != buffer.end())
	{
		if (*it == '"')
		{
			toCompare = getExpression(it , buffer.end(), "\"unknow\": Server scope");
			if (strcmp(toCompare.c_str(), "server_name") == 0)
			{
				if (*(it + 1) == ':' && *(it + 2) == '"')
				{
					it += 2;
					if (addServer(getExpression(it , buffer.end(), "\"server_name\": Server scope"), tmp) == false)
						throw std::string("Error in difining parametre \"server_name\": Server scope");
				}
				else
					throw std::string("Error in difining parametre \"server_name\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "listen"))
			{
				if (*(it + 1) == ':' && *(it + 2) == '"')
				{
					it += 2;
					if (tmp.setIp(getExpression(it , buffer.end(), "\"listen\": Server scope")) == false)
					throw std::string("Error in difining parametre \"listen\": Server scope");
					if (tmp.setPort() == false)
						throw std::string("Error in difining parametre \"listen\": Server scope");
				}
				else
					throw std::string("Error in difining parametre \"listen\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "client_body_max_size"))
			{
				if (*(it + 1) == ':' && *(it + 2) == '"')
				{
					it += 2;
					if (tmp.setBodyMaxSize(getValue(it , buffer.end(), "\"client_body_max_size\": Server scope")) == false)
						throw std::string("Error in difining parametre \"client_body_max_size\": Server scope");
				}
				else
					throw std::string("Error in difining parametre \"client_body_max_size\": Server scope");
			}
			else if (strcmp(toCompare.c_str(), "routes"))
			{/*
				if (*(it + 1) == ':')
					if (*(it + 2) == '{' && *(it + 3) == '"')
					{
						it += 3;
						parseRoutes(it);
					}
					else if (*(it + 2) == '[' && *(it + 3) ==  '{' && *(it + 4) == '"')
					{
						it += 4;
						parserRoutes(it);
						while (*(it) == ',')
						{
							it++;
							if (*it == '{')
								parserRoutes(it);
							else
								throw std::string("Error in difining parametre \"server\": Config scope");
						}
					}
					else
						throw std::string("Bad Format: Server scope");
				else
					throw std::string("Bad Format: Server scope");
			*/}
			else
				throw std::string("No match config parametre: Server scope");
		}
		else if (*it == ',')
			it++;
		else
			throw std::string("File bad Format: Server scope");
	}
	it++;
}
