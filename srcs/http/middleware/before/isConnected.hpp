#ifndef ISCONNECTED_HPP
# define ISCONNECTED_HPP

# include "IMiddleware.hpp"

class IsConnected : public IMiddleware {

	public:
		virtual ~IsConnected();

		virtual void handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next);

};

#endif