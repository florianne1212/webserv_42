#ifndef CHECKREDIR_HPP
# define CHECKREDIR_HPP

#include "../IMiddleware.hpp"

class CheckRedir : public IMiddleware {

	public:
		virtual ~CheckRedir();

		virtual void handle(Client &client, Request &request, Response &response, MiddlewareChain &next);

};

#endif