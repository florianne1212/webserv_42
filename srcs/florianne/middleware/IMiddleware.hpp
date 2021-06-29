#ifndef IMIDDLEWARE_HPP
# define IMIDDLEWARE_HPP

#include <iostream>
#include "../request.hpp"
#include "../response.hpp"
#include "MiddlewareChain.hpp"
#include "../Methods/getMethod.hpp"
#include "../Methods/postMethod.hpp"
#include "../Methods/deleteMethod.hpp"

class Client;
// class Request;
// class Response;
class MiddlewareChain;

class IMiddleware {

	public:
		virtual ~IMiddleware();

		virtual void handle(Client &client, Request &request, Response &response, MiddlewareChain &next) = 0;

};

#endif