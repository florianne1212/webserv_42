#include "webserv.hpp"

void selector(fd_storage fd_list)
{
	timeval to;

	to.tv_sec = 0;
	to.tv_usec = 0;

	if (select(FD_SETSIZE, &(fd_list->second), &(fd_list->second), NULL, &to) > 0)
	{
		for (std::vector<client>::iterator it = fd_list->first.begin(); it < fd_list->first.end(); it++)
		{
			if (*it->readable())
				*it->read();
			else if (*it->writable())
				*it->write();
		}
	}
}

int event_loop()
{
	fd_set rfds;
 	fd_storage fd_list;

	FD_ZERO(&rfds);
	fd_list.second = rfds;
	while (1)
	{
		//ecoute
		fd_list->first.push_back(client(fd));
		FD_SET(fd, &(fd_list->second));
		//
		selector(fd_list);
	}
	return 0;
}

int main()
{
	//setup
	//ouvrir socket
	return event_loop();
}
