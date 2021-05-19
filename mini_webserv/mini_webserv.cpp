
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define PORT 8080
#define SOCKET_ERROR -1
#define MAX_CONNECT 1

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

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

	char buffer[] = "HTTP/1.1 200 OK\n"
            "Server: Server Test\n"
            "Content-Type: text/html\n"
            "Connection: Keep-alive\n\n"
            "<h1>The game</h1>";

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
					printf("Ecoute sur le port %d..\n",htons(sin.sin_port));
					csock = accept(sock, (SOCKADDR *)&csin, &sizeCsin);
					//fcntl(csock, F_SETFL, O_NONBLOCK);
					FD_ZERO(&rfds);
					FD_SET(csock, &rfds);
					if(csock == SOCKET_ERROR)
					{
						perror("accept");
						return -1;
					}
					else
					{
						printf("Une connexion vient d'être ouvert entre le serveur et le client (%s:%d).\n",inet_ntoa(csin.sin_addr), htons(csin.sin_port));
						if (select(FD_SETSIZE, &rfds, &rfds, NULL, &to) > 0)
						{
							int sendSocket = send(csock, buffer, strlen(buffer), 0);
							if(sendSocket == SOCKET_ERROR)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("Contenue envoyé vers le client.\n");
								usleep(100000);
							}
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
