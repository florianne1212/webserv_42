#ifndef ClientSOCKET_HPP
# define ClientSOCKET_HPP

class ClientSocket;

# include "ASocket.hpp"
# include "buffer.hpp"
# include "request.hpp"
# include "response.hpp"

class ClientSocket : public ASocket
{
	private :
		ClientSocket();

	protected :
		std::string _clientAddress;
		std::string _clientPort;
		Request		_request;
		Buffer		_buffer;
		bool		_responseSent;
		bool _test;
		bool _append;
		int _fd_out;

	public :
		ClientSocket(int fd, std::string serverName, std::string clientAddress, std::string clientPort);
		// ClientSocket(int fd, std::string clientAddress, std::string clientPort);
		virtual ~ClientSocket();
		ClientSocket(const ClientSocket & other);
		ClientSocket & operator=(const ClientSocket & other);

		virtual int	getFd(void) const;
		virtual void read(Config *datas, FDList *listFD);
		virtual void write(Config *datas, FDList *listFD);
		std::string getClientAddress(void) const;
		std::string getClientPort(void) const;
		void my_append(Response *response);
};

#endif