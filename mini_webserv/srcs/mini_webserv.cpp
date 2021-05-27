#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>

#define PORT 8080
#define SOCKET_ERROR -1
#define MAX_CONNECT 1
#define BUFFER_SIZE 512
#define TIMEOUT 8

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

void log_recv(SOCKET csock)
{
	struct timeval begin , now;
	std::string log;
	std::string::iterator it;
	char buffer[BUFFER_SIZE];
	int	size_recv;
	double timediff;

	gettimeofday(&begin , NULL);

	std::ofstream file("log");

	while(1)
	{
		gettimeofday(&now , NULL);

		timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);

		if(timediff > TIMEOUT)
		{
			break;
		}	
		memset(buffer ,0 , BUFFER_SIZE);
		if((size_recv = recv(csock , buffer , BUFFER_SIZE , 0) ) < 0)
		{
			usleep(100000);
		}
		else
		{
			log += buffer;
			gettimeofday(&begin , NULL);
			it = log.end() - 1;
			if (log.size() > 3 && *it == '\n' && *(it - 1) == '\r' && *(it - 2) == '\n')
				break;
		}
	}	
	file << log << "==========----------==========\n\n";
	file.close();
}

int send_page(SOCKET csock)
{
	std::string page = "HTTP/1.1 200 OK\r\n"
		"Server: Server Test\r\n"
		"Content-Type: text/html\r\n"
		"Connection: Keep-alive\r\n\r\n";

	std::ifstream ifs("index.html");
	std::string line;

	while (ifs)
	{
		std::getline(ifs, line);
		page += "\n";
		page += line;
	}
	int sendSocket = send(csock, page.c_str(), page.size(), 0);
	if(sendSocket == SOCKET_ERROR)
	{
		perror("send");
		return -1;
	}
	else
	{
		std::cout << "Contenue envoyé vers le client.\n";
		usleep(100000);
	}
	return 0;
}

int main(void)
{
	SOCKET sock;
	SOCKADDR_IN sin;
	socklen_t sizeSin = sizeof(sin);

	SOCKET csock;
	SOCKADDR_IN csin;
	socklen_t sizeCsin = sizeof(csin);

	fd_set rfds;
	timeval to;

	to.tv_sec = 5;
	to.tv_usec = 0;

	int bindSock = 0, listenSock = 0;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock)
	{
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_family = AF_INET;
		sin.sin_port = htons(PORT);
		bindSock = bind(sock, (SOCKADDR *)&sin, sizeSin);
		if (bindSock == SOCKET_ERROR)
		{
			perror("bind");
			return -1;
		}
		else
		{
			listenSock = listen(sock, MAX_CONNECT);
			if (bindSock == SOCKET_ERROR)
			{
				perror("listen");
				return -1;
			}
			else
			{
				while (1)
				{
					std::cout << "Ecoute sur le port " << htons(sin.sin_port) << "..\n";
					csock = accept(sock, (SOCKADDR *)&csin, &sizeCsin);
					fcntl(csock, F_SETFL, O_NONBLOCK);
					FD_ZERO(&rfds);
					FD_SET(csock, &rfds);
					if(csock == SOCKET_ERROR)
					{
						perror("accept");
						return -1;
					}
					else
					{
						std::cout << "Une connexion vient d'être ouvert entre le serveur et le client (" << inet_ntoa(csin.sin_addr) << ":" << htons(csin.sin_port) << ")\n";
						if (select(FD_SETSIZE, &rfds, &rfds, NULL, &to) > 0)
						{
							log_recv(csock);
							if (send_page(csock) == -1)
								return -1;
						}
						close(csock);
					}
				}
			}
		}
	}
	else
	{
		perror("sock");
		return -1;
	}
	return 0;
}
