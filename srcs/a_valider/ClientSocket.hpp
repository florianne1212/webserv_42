#ifndef ClientSOCKET_HPP
#define ClientSOCKET_HPP


#include "ASocket.hpp"

class ClientSocket : public ASocket
{
	private :
		ClientSocket();

	protected :
		char* _bareRequest; //ou un vector<char> ?
		char* _bareAnswer; //ou un vector<char> ?

	public :
		ClientSocket(int fd);
		virtual ~ClientSocket();
		ClientSocket(const ClientSocket & other);
		ClientSocket & operator=(const ClientSocket & other);

		virtual int	getFd(void) const;

};

#endif