#ifndef POSTMETHOD_HPP
# define POSTMETHOD_HPP
#include "../request.hpp"
#include "../response.hpp"
#include "../Client.hpp"
#include "../File.hpp"
#include <iostream>

class PostMethod
{
	public:
		PostMethod();
		PostMethod(PostMethod const & copy);
		~PostMethod();
		PostMethod& operator=(PostMethod const & ope);

		void handlePost(Client &client, Request &request, Response &response);
};

#endif