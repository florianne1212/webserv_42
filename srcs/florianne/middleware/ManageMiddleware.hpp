#ifndef MANAGEMIDDLEWARE_HPP
# define MANAGEMIDDLEWARE_HPP

#include "IMiddleware.hpp"
#include "MiddlewareChain.hpp"
#include "method/MethodMiddleware.hpp"
#include "before/checkRedir.hpp"
#include "before/isConnected.hpp"
#include "before/checkRequest.hpp"
#include "before/checkRoot.hpp"
#include "before/checkBodySize.hpp"
#include "../request.hpp"
#include "../response.hpp"



class ManageMiddleware {

	public:
		enum State
		{
			S_BEFORE,
			S_METHOD,
			// S_AFTER,
			S_END,
		};
	private:
		State _state;


	public:
		ManageMiddleware();
		~ManageMiddleware(void);

		void middlewareStart(ClientSocket &client, Config &config, Request &request, Response &response);


};

#endif