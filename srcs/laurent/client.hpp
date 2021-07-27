#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <string>
# include <map>
# include <queue>
# include <stack>
# include <algorithm>
# include <exception>
# include <ctime>
# include <cstring>

struct SocketA //les sockets autres que les socket listen
{
	int fd;
	// bool read; //true si en attente
	// bool write; // true si en attente d'envoyer donnees (ces 2 bool peuvent etre en un seul)
	bool status; //true = read, false = write
	std::string input; //ce que l'on recoit du client
	std::string output; //ce que l'on renvoie au client/
	/* autres champs a determiner
	*/

};

class Client
{
	private:
		Client();
		void readInfo();

		int _fd;
		std::string _bareRequest;

	public:
		Client(int fd);
		virtual ~Client();
		Client(const Client & other);
		Client & operator=(const Client & other);

		std::string getBareRequest() const;
		// void setBareRequest(int fd);
		// void readInfo(int fd);
};


#endif