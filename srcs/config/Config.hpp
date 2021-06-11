#ifndef WEBSERV_SETUP_HPP
# define WEBSERV_SETUP_HPP

# include "Includes.hpp"

class Config
{
	private :
		std::map<std::string, Server>	_serverList;
		std::map<int, std::string>		_pathErrorFile;
		usable<size_t>					_bodyMaxSize;

		std::string getExpression(std::string::iterator &it, std::string::iterator end, std::string err);
		size_t getValue(std::string::iterator &it, std::string::iterator end, std::string err);
		void	parser(std::string setupFile);
		void	parserServer(std::string::iterator &it, std::string &buffer);

	public :
		Config(std::string setupFile);
		bool addServer(std::string name, Server &server);
		bool setPathErrorFile(std::string pathErrorFile);
		bool setBodyMaxSize(size_t bodyMaxSize);

};
#endif
