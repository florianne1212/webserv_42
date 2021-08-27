#ifndef MIDDLEWARECHAIN_HPP
# define MIDDLEWARECHAIN_HPP

class MiddlewareChain;

# include <list>
# include "IMiddleware.hpp"
# include "ClientSocket.hpp"
# include "Config.hpp"
# include "Request.hpp"
# include "Response.hpp"


class MiddlewareChain {

	private:
		std::list<IMiddleware*> m_middlewares;
		ClientSocket *m_client;
		Request *m_request;
		Response *m_response;
		Config *m_config;

	public:
		MiddlewareChain(const std::list<IMiddleware*> &middlewares,Config &config, ClientSocket &client, Request &request, Response &response);
		~MiddlewareChain(void);

	public:
		void operator ()();

};

#endif