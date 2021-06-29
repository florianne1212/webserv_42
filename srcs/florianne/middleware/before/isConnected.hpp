#ifndef ISCONNECTED_HPP
# define ISCONNECTED_HPP

#include "../IMiddleware.hpp"

class IsConnected : public IMiddleware {

	public:
		virtual ~IsConnected();

		virtual void handle(Client &client, Request &request, Response &response, MiddlewareChain &next);

};

#endif