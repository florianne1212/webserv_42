#ifndef ASOCKET_HPP
#define ASOCKET_HPP

#include <vector>
#include <string>

class ASocket
{
	protected:
		ASocket();

		int _fd;
		bool _readStatus; //true = resultatdu poll ou du select, donnees en attente
		bool _writeStatus; //true on peut envoyer des donnees (resultat du select ou du poll)

	public:
		ASocket(int fd);
		virtual ~ASocket();
		ASocket(const ASocket & other);
		ASocket & operator=(const ASocket & other);

		virtual int	getFd(void) const= 0;
		bool getReadStatus(void) const;
		bool getWriteStatus(void) const;
		void setReadStatus(bool status);
		void setWriteStatus(bool status);
};


#endif