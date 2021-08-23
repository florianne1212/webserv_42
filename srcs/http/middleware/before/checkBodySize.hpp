#ifndef CHECKBODYSIZE_HPP
# define CHECKBODYSIZE_HPP

# include "IMiddleware.hpp"

class CheckBodySize : public IMiddleware , public Singleton<CheckBodySize> {

	public:
		virtual ~CheckBodySize();


		virtual void handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next);

};

#endif