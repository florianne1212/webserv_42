#ifndef CGISOCKETToCgiTOCGI_HPP
# define CGISOCKETToCgiTOCGI_HPP

class CgiSocketToCgi;

# include "ASocket.hpp"
# include "Buffer.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "FDList.hpp"
# include <sys/time.h>
# include "ClientSocket.hpp"


class CgiSocketToCgi : public ASocket
{
	private :
		CgiSocketToCgi();

	protected:
		struct timespec _lastInterTime;
		Request _request;
		ClientSocket * _client;

	public :
		CgiSocketToCgi(int fd, Request & request, ClientSocket * client);
		virtual ~CgiSocketToCgi();

		virtual int	getFd(void) const;
		virtual void read(Config *datas, FDList *listFD);
		virtual void write(Config *datas, FDList *listFD);
		virtual bool getTimeout();
		virtual void setTime();
};

#endif