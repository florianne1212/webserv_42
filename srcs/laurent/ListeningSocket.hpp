#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP


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
		//methode connect()
};

#endif