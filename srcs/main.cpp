#include "webserv.hpp"

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

int main()
{
	//setup
	//ouvrir socket
	return eventLoop();
}
