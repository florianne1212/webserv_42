#ifndef METHODMIDDLEWARE_HPP
# define METHODMIDDLEWARE_HPP

#include "../IMiddleware.hpp"

class MethodMiddleware : public IMiddleware {

	public:
		virtual ~MethodMiddleware();

		virtual void handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next);

};

#endif