#ifndef CHECKPATH_HPP
# define CHECKPATH_HPP

# include "IMiddleware.hpp"

class CheckPath : public IMiddleware , public Singleton<CheckPath> {

	public:
		virtual ~CheckPath();


		virtual void handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next);

};

#endif