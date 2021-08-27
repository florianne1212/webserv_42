#ifndef POSTMETHOD_HPP
# define POSTMETHOD_HPP

# include "../Request.hpp"
# include "../Response.hpp"
# include "ClientSocket.hpp"
# include "../File.hpp"
# include <iostream>

class PostMethod
{
	public:
		PostMethod();
		PostMethod(PostMethod const & copy);
		~PostMethod();
		PostMethod& operator=(PostMethod const & ope);

		
		void handlePost(ClientSocket &client, Config &config,Request &request, Response &response);};
		void setHeader(Response &response, File &fileGet);

#endif