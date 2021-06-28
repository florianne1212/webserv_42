// #include "webserv.hpp"
#include "./config/Config.hpp"

void selector(list<client> clientList)
{
	if (poll(clientList.size(), clientList, 1000) > 0)
	{
		for (std::list<client>::iterator clientList.begin(); it < clientList.end(); it++)
		{
			if (*it->readable())
				*it->read();
			else if (*it->writable())
				*it->write();
		}
	}
}

int eventLoop()
{
 	list<client>	clientList;

	while (1)
	{
		//ecoute
		//fill clientList
		selector(clientList);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	Config* datas;

	if (argc > 2)
		return (1);
	else if (argc == 2)
		datas = new Config((std::string)argv[1]);
	else 
		datas = new Config("chemin par defaut");
	
	//setup
	//ouvrir socket
	return eventLoop();
	delete (datas);
}
