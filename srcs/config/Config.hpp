#ifndef WEBSERV_SETUP_HPP
# define WEBSERV_SETUP_HPP

class Config;


// # define WORKPATH "./workDir"

// # define WORKPATH "./testingDirectory"

# include <map>
# include <vector>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <fstream>
# include <sys/stat.h>
# include "usable.hpp"
# include "Server.hpp"
# include "includes.hpp"

class Config
{
	private :
		std::map<std::string, Server>	_serverList; //string est le nom du serveur
		std::map<int, std::string>		_pathErrorFile;
		usable<size_t>					_bodyMaxSize;

		std::vector<std::string> getExpression(std::string::iterator &it, std::string::iterator end, std::string err);
		size_t getValue(std::string::iterator &it, std::string::iterator end, std::string err);
		void	removeIsSpace(std::string &buffer);
		void	checker();
		void	parser(std::string setupFile);
		void	parserServer(std::string::iterator &it, std::string &buffer);
		void	parserRoutes(std::string::iterator &it, std::string &buffer, Server &serv);

	public :
		Config();
		Config(std::string setupFile);
		bool addServer(std::string name, Server &server);
		bool setPathErrorFile(std::vector<std::string> pathErrorFile);
		bool setBodyMaxSize(size_t bodyMaxSize);

		void	putConfig();

		//geter Config
		usable<std::string> getPathErrorFile(int errorVal) const;
		size_t getBodyMaxSize(std::string serverName, std::string path) const;
		std::map<std::string, Server>::const_iterator getServerBegin(void) const;
		std::map<std::string, Server>::const_iterator getServerEnd(void) const;
		//geter Server
		usable<std::string> getServerName(std::string ip) const;
		std::string getIp(std::string serverName) const;
		unsigned short getPort(std::string serverName) const;
		usable<std::string> getUploadDir(std::string serverName) const; //a revoir
		//geter Routes
		std::vector<std::string> getHttpRequest(std::string serverName, std::string path) const;
		usable<std::pair<size_t , std::string> > getHttpRedirection(std::string serverName, std::string path) const;
		usable<std::pair<std::string, std::string> > getRoot(std::string serverName, std::string path) const;
		bool getAutoIndex(std::string serverName, std::string path) const;
		usable<std::string> getDirectoryPage(std::string serverName, std::string path) const;

};
#endif
