#ifndef WEBSERV_SETUP_HPP
# define WEBSERV_SETUP_HPP

class Config
{
	private :
		std::map<std::string, server>	_serverList;
		vector<std::string>				_pathErrorFile;
		size_t							_bodyMaxSize;

		void	parser(std::strin setupFile);
		std::string getExpression(std::string str);
		size_t getValue(std::string str);

	public :
		setup(std::string setupFile);

		class ErrorParsing : public std::exception
		{
			virtual const char * what() const throw;
		};

};
#endif
