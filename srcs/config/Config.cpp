setup(std::string setupFile)
: _setupFile(setupFile)
{}

void	parser(void)
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
}
