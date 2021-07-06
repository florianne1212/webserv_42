#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

class Config;
class FDList;

#include "ASocket.hpp"

class ListeningSocket : public ASocket
{
	private :
		ListeningSocket();

	protected :
		// sockaddr_in _addr;

	public :
		// ListeningSocket(int fd, sockaddr_in addr);
		ListeningSocket(int fd);
		virtual ~ListeningSocket();
		ListeningSocket(const ListeningSocket & other);
		ListeningSocket & operator=(const ListeningSocket & other);

		virtual int	getFd(void) const;
		virtual void read(Config *datas, FDList *listFD);		//methode connect()
		virtual void write(Config *datas, FDList *istFD);	//useless methode
};

#endif