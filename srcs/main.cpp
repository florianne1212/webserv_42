#include "webserv.hpp"

void selector(fdStorage fdList)
{
	timeval to;

	to.tv_sec = 0;
	to.tv_usec = 0;

	if (select(FD_SETSIZE, &(fdList->second), &(fdList->second), NULL, &to) > 0)
	{
		for (std::vector<client>::iterator it = fdList->first.begin(); it < fdList->first.end(); it++)
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
	fd_set rfds;
 	fdStorage fdList;

	FD_ZERO(&rfds);
	fdList.second = rfds;
	while (1)
	{
		//ecoute
		fdList->first.push_back(client(fd));
		FD_SET(fd, &(fdList->second));
		//
		selector(fdList);
	}
	return 0;
}

int main()
{
	//setup
	//ouvrir socket
	return eventLoop();
}
