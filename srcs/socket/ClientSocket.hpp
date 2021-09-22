#ifndef ClientSOCKET_HPP
# define ClientSOCKET_HPP

class ClientSocket;

# include <sys/time.h>
# include "ASocket.hpp"
# include "Buffer.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "FDList.hpp"

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
		struct pollfd _fd_out;
		struct pollfd _fd_read;
		bool _read;
		std::string _body;
		FDList* _listFD;
		struct timespec _lastInterTime;
		int _status;
		Response _response;
		int _cgiFd[4];
		bool _closeToCgiSocket;


	public :
		ClientSocket(int fd, std::string serverName, std::string clientAddress, std::string clientPort, FDList* listFD);
		virtual ~ClientSocket();
		ClientSocket(const ClientSocket & other);
		ClientSocket & operator=(const ClientSocket & other);

		virtual int	getFd(void) const;
		virtual void read(Config *datas, FDList *listFD);
		virtual void write(Config *datas, FDList *listFD);
		std::string getClientAddress(void) const;
		std::string getClientPort(void) const;
		void my_append(Response *response, FDList *listFD);
		void my_read(Response *response, FDList *listFD);
		FDList* getListFD(void);
		virtual bool getTimeout(void);
		virtual void setTime(void);
		Response & getResponse();
		void reinitResponse();

		void setCgiFd(int pipeOut0, int pipeout1, int pipein0, int pipein1 );
		void setCgiFd(int index, int value);
		int getCgiFdValue(int index);
		void destroyCgiSockets(void);
		bool getCloseToCgi(void);
		void setCloseToCgi(bool value);
};

#endif