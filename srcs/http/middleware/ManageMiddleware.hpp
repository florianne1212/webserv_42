#ifndef MANAGEMIDDLEWARE_HPP
# define MANAGEMIDDLEWARE_HPP

class ManageMiddleware;

# include "ClientSocket.hpp"
# include "Config.hpp"
# include "Request.hpp"
# include "Response.hpp"

class ManageMiddleware {

	public:
		enum State
		{
			S_BEFORE,
			S_METHOD,
			S_END,
		};
	private:
		State _state;


	public:
		ManageMiddleware();
		~ManageMiddleware(void);

		void middlewareStart(ClientSocket &client, Config &config, Request &request, Response &response);


};

# include "IMiddleware.hpp"
# include "MiddlewareChain.hpp"
# include "MethodMiddleware.hpp"
# include "CheckRedir.hpp"
# include "IsConnected.hpp"
# include "CheckRequest.hpp"
# include "CheckPath.hpp"
# include "CheckRoot.hpp"
# include "CheckBodySize.hpp"
# include "GetFileError.hpp"

#endif