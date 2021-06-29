#ifndef CHECKREQUEST_HPP
# define CHECKREQUEST_HPP

#include "../IMiddleware.hpp"

class CheckRequest : public IMiddleware {

	public:
		virtual ~CheckRequest();

		virtual void handle(Client &client, Request &request, Response &response, MiddlewareChain &next);

};

#endif