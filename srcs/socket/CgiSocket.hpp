#ifndef CGISOCKET_HPP
# define CGISOCKET_HPP

class CgiSocket;

# include "ASocket.hpp"
# include "buffer.hpp"
# include "request.hpp"
# include "response.hpp"
# include "FDList.hpp"

class CgiSocket : public ASocket
{
	private :
		CgiSocket();

	public :
		CgiSocket(int fd);
		virtual ~CgiSocket();

		virtual int	getFd(void) const;
		virtual void read(Config *datas, FDList *listFD);
		virtual void write(Config *datas, FDList *listFD);
};

#endif