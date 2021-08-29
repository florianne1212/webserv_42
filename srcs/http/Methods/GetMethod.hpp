#ifndef GETMETHOD_HPP
# define GETMETHOD_HPP

# include "../Request.hpp"
# include "../Response.hpp"
# include "ClientSocket.hpp"
# include "../File.hpp"
# include <iostream>
# include <sstream> 
# include <sys/stat.h>

class GetMethod
{
	public:
		GetMethod();
		GetMethod(GetMethod const & copy);
		~GetMethod();
		GetMethod& operator=(GetMethod const & ope);

		void handleGet(ClientSocket &client, Config &config,Request &request, Response &response);
		void setHeader(Response &response, File &fileGet);
		std::string setDirectory(File &fileGet, std::string url, std::string ip);
		void setHeader_Dir(Response &response, std::string html_generated);
};

#endif

