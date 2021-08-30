#ifndef CHECKREDIR_HPP
# define CHECKREDIR_HPP

# include "IMiddleware.hpp"

class CheckRedir : public IMiddleware , public Singleton<CheckRedir> {

	public:
		virtual ~CheckRedir();


		virtual void handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next);

};

#endif