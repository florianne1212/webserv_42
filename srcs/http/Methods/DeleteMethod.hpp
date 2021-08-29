#ifndef DELETEMETHOD_HPP
# define DELETEMETHOD_HPP

# include "../Request.hpp"
# include "../Response.hpp"
# include "ClientSocket.hpp"
# include "../File.hpp"
# include <iostream>

class DeleteMethod
{
	public:
		DeleteMethod();
		DeleteMethod (DeleteMethod const & copy);
		~DeleteMethod();
		DeleteMethod& operator=(DeleteMethod const & ope);

		void handleDelete(ClientSocket &client,Config &config, Request &request, Response &response);
};

#endif