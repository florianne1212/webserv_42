// #include "webserv.hpp"
#include "Includes.hpp"

void selector(Config datas, FDList listFD)
{
	(void)datas;
	if (poll(listFD.getPollList(), listFD.getSizePollList(), 1000) > 0)
	{
		for (std::list<ASocket *>::iterator it = listFD.getSocketList().begin(); it != listFD.getSocketList().end(); it++)
		{
			if ((*it)->getReadStatus())
				(*it)->read(datas, listFD);
			else if ((*it)->getWriteStatus())
				(*it)->write(datas, listFD);
		}
	}
}

int eventLoop(Config datas, FDList listFD)
{
	while (1)
	{
		selector(datas, listFD);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	Config	datas;
	FDList	listFD;

	if (argc > 2)
		return (1);
	else if (argc == 2)
		datas = Config((std::string)argv[1]);
	else 
		datas = Config("chemin par defaut");
	
	//setup
	//ouvrir socket
	std::cout << "START" << std::endl;
	return eventLoop(datas, listFD);
}
