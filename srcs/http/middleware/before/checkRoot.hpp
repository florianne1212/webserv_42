#ifndef CHECKROOT_HPP
# define CHECKROOT_HPP

# include "IMiddleware.hpp"

class CheckRoot : public IMiddleware , public Singleton<CheckRoot> {

	public:
		virtual ~CheckRoot();

		virtual void handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next);

};

#endif