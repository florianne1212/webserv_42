#ifndef CGISOCKETFROMCGI_HPP
# define CGISOCKETFROMCGI_HPP

class CgiSocketFromCgi;

# include "ASocket.hpp"
# include "Buffer.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "FDList.hpp"
# include <sys/time.h>


class CgiSocketFromCgi : public ASocket
{
	private :
		CgiSocketFromCgi();

	protected:
		struct timespec _lastInterTime;
		int _state;
		std::string _cgiHeaders;
		Response* _response;
		ClientSocket* _client;
		int _otherFdToClose;
		bool _contentLengthPresent;

	protected:
		int checkHeaders(char c, int state);
		std::string cgiResponseHeaderPreparation(std::string & cgiHeaders);
		std::string cgiResponseChunkedPreparation(std::string cgiResponse, int readResult);
		std::string itoaBase16(size_t num);

	public :
		CgiSocketFromCgi(int fd[2], ClientSocket * client, Response * response);
		virtual ~CgiSocketFromCgi();

		virtual int	getFd(void) const;
		virtual void read(Config *datas, FDList *listFD);
		virtual void write(Config *datas, FDList *listFD);
		virtual bool getTimeout();
		virtual void setTime();
		virtual void prepareCgiEnd();

};

#endif