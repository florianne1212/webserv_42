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

	public :
		ListeningSocket(int fd);
		virtual ~ListeningSocket();
		ListeningSocket(const ListeningSocket & other);
		ListeningSocket & operator=(const ListeningSocket & other);

		virtual int	getFd(void) const;
		virtual void read(Config datas, FDList listFD);		//methode connect()
};

#endif