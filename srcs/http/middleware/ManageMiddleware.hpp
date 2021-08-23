#ifndef MANAGEMIDDLEWARE_HPP
# define MANAGEMIDDLEWARE_HPP

class ManageMiddleware;

# include "ClientSocket.hpp"
# include "Config.hpp"
# include "request.hpp"
# include "response.hpp"

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
# include "checkRedir.hpp"
# include "isConnected.hpp"
# include "checkRequest.hpp"
# include "checkPath.hpp"
# include "checkRoot.hpp"
# include "checkBodySize.hpp"
# include "getFileError.hpp"

#endif