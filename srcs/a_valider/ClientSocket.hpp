#ifndef ClientSOCKET_HPP
#define ClientSOCKET_HPP


#include "ASocket.hpp"

class ClientSocket : public ASocket
{
	private :
		ClientSocket();

	protected :
		std::string _bareRequest; //ou un vector<char> ?
		std::string _bareAnswer; //ou un vector<char> ?

	public :
		ClientSocket(int fd);
		virtual ~ClientSocket();
		ClientSocket(const ClientSocket & other);
		ClientSocket & operator=(const ClientSocket & other);

		virtual int	getFd(void) const;

};

#endif