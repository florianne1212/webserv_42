#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include "Config.hpp"
#include "FDList.hpp"

int	g_signalHandler;

void selector(Config *datas, FDList *listFD)
{
	listFD->myPoll();
	std::list<ASocket *> Socketlist = listFD->getSocketList();
	for (std::list<ASocket *>::iterator it = Socketlist.begin(); it != Socketlist.end(); it++)
	{
		try
		{
			if ((*it)->getReadStatus())
			{
				(*it)->setTime();
				(*it)->read(datas, listFD);
			}
			else if ((*it)->getWriteStatus())
			{
				(*it)->setTime();
				(*it)->write(datas, listFD);
			}
			else if ((*it)->endFromCgiStatus())
			{
				(*it)->setTime();
				(*it)->prepareCgiEnd();
			}
			else if ((*it)->getTimeout())
			{
				close((*it)->getFd());
				listFD->rmSocket((*it)->getFd());
			}
		}
		catch (std::string err)
		{
			std::cerr << err << std::endl;
		}
	}
}

int eventLoop(Config *datas, FDList *listFD)
{
	while (g_signalHandler)
	{
		selector(datas, listFD);
	}
	return 0;
}

void openSocket(Config *datas, FDList *listFD)
{
	int sock;
	sockaddr_in sin;
	std::map<std::string, Server>::const_iterator it;
	for (it = datas->getServerBegin(); it != datas->getServerEnd(); it++)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == -1)
			throw std::string("Error socket function");
		int iSetOption = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &iSetOption, sizeof(iSetOption)) == -1)
			throw std::string("Error to setting socket options");
		if (sock)
		{
			sin.sin_addr.s_addr = inet_addr(datas->getIp(it->first).c_str());
			sin.sin_family = AF_INET;
			sin.sin_port = htons(datas->getPort(it->first));
			if (bind(sock, (sockaddr *)&sin, sizeof(sin)) == -1)
			{
				perror("l'erreur est ");
				throw std::string("Error to binding socket");
			}
			if (listen(sock, 100) == -1)
				throw std::string("Error to listening socket");
		}
		else
			throw std::string("error to create socket");

		int true_value = 1;
		if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &true_value, sizeof(int)) == -1)
			throw std::string("could not set reusable");

		listFD->addSocket(new ListeningSocket(sock, it->first, sin)); //pour avoir ip et port
	}
}

void	ignore(int i)
{
	(void)i;
}

void	control_c(int i)
{
	g_signalHandler = 0;
	(void)i;
}

int main(int argc, char *argv[])
{
	Config *datas = NULL;
	FDList *listFD = new FDList();
	g_signalHandler = 1;

	signal(SIGPIPE, ignore);
	signal(SIGINT, control_c);
	try
	{
		if (argc > 2)
			return (1);
		else if (argc == 2)
			datas = new Config((std::string)argv[1]);
		else
			datas = new Config("configuration_file/test3.conf");

		openSocket(datas, listFD);
	}
	catch (std::string err)
	{
		if (datas != NULL)
     		delete(datas);
		std::cerr << err << std::endl;
		delete listFD;
		return 0;
	}
	std::cout << "START" << std::endl;
	eventLoop(datas, listFD);
	delete datas;
	delete listFD;
	return (0);
}
