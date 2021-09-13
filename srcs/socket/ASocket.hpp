#ifndef ASOCKET_HPP
# define ASOCKET_HPP

class ASocket;

# include <poll.h>
# include <string>
# include "Config.hpp"
# include "FDList.hpp"

enum CgiState
{
	NO_CGI,
	CGI_IN_PROGRESS, //client
	CGI_DONE, //client
	FROM_CGI_IN_PROGRESS, //cgisocketfromcgi
	TO_CGI_IN_PROGRESS, //cgisockettocgi
	FROM_CGI_DONE,
	TO_CGI_DONE
};

class ASocket
{
	protected:
		ASocket();

		int _fd;
		struct pollfd _pollFD;
		std::string _serverName;
		int _cgiState;

	public:
		ASocket(int fd, std::string ServerName);
		virtual ~ASocket();
		ASocket(const ASocket & other);
		ASocket & operator=(const ASocket & other);

		virtual int	getFd(void) const = 0;
		virtual void read(Config *datas, FDList *listFD) = 0;
		virtual void write(Config *datas, FDList *listFD) = 0;
		bool getReadStatus(void) const;
		bool getWriteStatus(void) const;
		bool endFromCgiStatus(void) const;
		struct pollfd getPollFD(void) const;
		std::string getServerName() const;
		void setPollFD(struct pollfd toSet);
		virtual bool getTimeout() = 0;
		virtual void setTime() = 0;
		int getcgiState();
		void setCgiState(int cgiState);
		virtual void prepareCgiEnd();
};

# include "ClientSocket.hpp"
# include "ListeningSocket.hpp"

#endif