#ifndef IMIDDLEWARE_HPP
# define IMIDDLEWARE_HPP

class IMiddleware;

# include <iostream>
# include "ClientSocket.hpp"
# include "Config.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "MiddlewareChain.hpp"
# include "Singleton.hpp"

class IMiddleware {

	public:
		virtual ~IMiddleware();

		virtual void handle(ClientSocket &client, Config &config, Request &request, Response &response, MiddlewareChain &next) = 0;

};

# include "DeleteMethod.hpp"
# include "PostMethod.hpp"
# include "GetMethod.hpp"
# include "CgiHandler.hpp"

#endif