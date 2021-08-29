#ifndef GETFILEERROR_HPP
# define GETFILEERROR_HPP

# include "IMiddleware.hpp"
# include "File.hpp"

class GetFileError : public IMiddleware , public Singleton<GetFileError> {

	public:
		virtual ~GetFileError();


		virtual void handle(ClientSocket &client, Config &config,Request &request, Response &response, MiddlewareChain &next);

};

#endif