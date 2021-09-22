#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

class ListeningSocket;

# include "ASocket.hpp"
# include "Config.hpp"
# include "FDList.hpp"
# include <arpa/inet.h>


class ListeningSocket : public ASocket
{
	private :
		ListeningSocket();

	protected :
		sockaddr_in _addr;

	public :
		ListeningSocket(int fd, std::string ServerName, sockaddr_in addr);
		virtual ~ListeningSocket();
		ListeningSocket(const ListeningSocket & other);
		ListeningSocket & operator=(const ListeningSocket & other);

		virtual int	getFd(void) const;
		virtual void read(Config *datas, FDList *listFD);		//methode connect()
		virtual void write(Config *datas, FDList *istFD);	//useless methode
		virtual bool getTimeout();
		virtual void setTime();
};

#endif