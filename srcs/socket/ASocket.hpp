#ifndef ASOCKET_HPP
# define ASOCKET_HPP

class ASocket;

# include <poll.h>
# include <string>
# include "Config.hpp"
# include "FDList.hpp"

class ASocket
{
	protected:
		ASocket();

		int _fd;
		struct pollfd _pollFD;
		std::string _serverName;

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
		struct pollfd getPollFD(void) const;
		std::string getServerName() const;
		void setPollFD(struct pollfd toSet);
		virtual bool getTimeout() = 0;
		virtual void setTime() = 0;
};

# include "ClientSocket.hpp"
# include "ListeningSocket.hpp"

#endif