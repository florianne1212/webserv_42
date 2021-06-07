std::string Config::getExpression(std::string::iterator &it, std::string::iterator end)
{
	std::string::iterator begin(it);

	while (it != '"' && it != end)
		it++;
	if (it == end)
		throw wath("Expression not found");
	return std::string(begin, it);
}

size_t Config::getValue(std::string str)
{
	std::string::iterator begin(it);

	while (it != '"' && it != end)
		it++;
	if (it == end)
		throw wath("Expression not found");
	std::string ret(begin, it)
	return atoi(ret.c_str());
}

Config::setup(std::string setupFile)
: _setupFile(setupFile), _serverList(), _pathErrorFile("serverFile/error.html"), _bodyMaxSize(1)
{
	parser(setupFile);
}

const char *Config::ErrorParsing::what(std::string str) const throw()
{
	return "Error in Parsing: " + str;
}

void	Config::parser(std::string setupFile)
{
	std::ifstream ifs(_setupFile);
	std::string line;

	//get file and del white space
	while (ifs)
	{
		std::getline(ifs, line);
		_buffer += line;
	}
	_buffer.erase(std::remove_if(_buffer.begin(), ::isspace, _buffer.end()));

	//parser json
	std::string::iterator begin;
	std::string toCompare;

	for (std::string::iterator it = _buffer.begin(); it < _buffer.end(); it++)
	{
		if (*it == '"')
		{
			toCompare = getExpression(it , _buffer.end());
			if (strcmp(toCompare, "error_path") == 0)
			{
				if (*(it + 1) == ':' && *(it + 2) == '"')
				{
					it += 2;
					_pathErrorFile = getExpression(it , _buffer.end());
				}
				else
					throw wath("Bad Format");
			}
			else if (strcmp(toCompare, "client_body_max_size"))
			{
				if (*(it + 1) == ':' && *(it + 2) == '"')
				{
					it += 2;
					_bodyMaxSize = atoi(getValue(it , _buffer.end()));
				}
				else
					throw wath("Bad Format");
			}
			else if (strcmp(toCompare, "server"))
			{
				if (*(it + 1) == ':')
					if (*(it + 2) == '{' && *(it + 3) == '"')
					{
						//parseServer()
					}
					else if (*(it + 2) == '[' && *(it + 3) ==  '{' && *(it + 4) == '"')
					{
						//parserServer()
					}
					else
						throw what ("Bad Format");
				else
					throw what("Bad Format");

			}
			else
				throw wath("No match config parametre");
		}
	}
}
