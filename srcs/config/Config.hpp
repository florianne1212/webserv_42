#ifndef WEBSERV_SETUP_HPP
# define WEBSERV_SETUP_HPP

class config:
{
	private :
		FD					_setupFile;

		std::map<std::string, server>	_serverList;
		std::string			_pathErrorFile;
		size_t				_bodyMaxSize;

	public :
		setup(std::string setupFile)
		: _setupFile(setupFile), _serverList(), _pathErrorFile("serverFile/error.html"), _bodyMaxSize(1)
		{}

		void	parser(void)

}
#endif
