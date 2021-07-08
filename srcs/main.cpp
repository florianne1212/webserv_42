// #include "webserv.hpp"
#include "includes.hpp"

void selector(Config *datas, FDList *listFD)
{
	if (listFD->myPoll())
	{
		std::list<ASocket *> Socketlist = listFD->getSocketList();
		for (std::list<ASocket *>::iterator it = Socketlist.begin(); it != Socketlist.end(); it++)
		{
			if ((*it)->getReadStatus())
				(*it)->read(datas, listFD);
			else if ((*it)->getWriteStatus())
				(*it)->write(datas, listFD);
		}
	}
}

int eventLoop(Config *datas, FDList *listFD)
{
	while (1)
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
		if (sock)
		{
			sin.sin_addr.s_addr = inet_addr(datas->getIp(it->first).c_str());
			sin.sin_family = AF_INET;
			sin.sin_port = htons(datas->getPort(it->first));
			if (bind(sock, (sockaddr *)&sin, sizeof(sin)) == -1)
			{
				throw std::string("Errror to binding socket");
			}
			if (listen(sock, 100) == -1)
				throw std::string("Error to listening socket");
		}
		else
			throw std::string("error to create socket");
		listFD->addSocket(new ListeningSocket(sock, it->first));
		// listFD->addSocket(new ListeningSocket(sock, sin)); //pour avoir ip et port
	}
}

int main(int argc, char* argv[])
{
	Config	*datas;
	FDList	*listFD = new FDList();

	try
	{
		if (argc > 2)
			return (1);
		else if (argc == 2)
			datas = new Config((std::string)argv[1]);
		else
			datas = new Config("chemin par defaut");

		openSocket(datas, listFD);
	}
	catch (std::string err)
	{
		std::cerr << err << std::endl;
		return 0;
	}
	std::cout << "START" << std::endl;
	return eventLoop(datas, listFD);
}
