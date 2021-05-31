selector()
{
	timeval to;

	to.tv_sec = 0;
	to.tv_usec = 0;

	if (select(FD_SETSIZE, &(fd_list->second), &(fd_list->second), NULL, to) > 0)
	{
		for (vector<client>::iterator it = fd_list->first.begin(); it < fd_list->first.end(); it++)
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
	ft::pair<vector<client>, fd_set> fd_list; //?

	FD_ZERO(&rfds);
	fd_list.second = rfds;
	while (1)
	{
		//ecoute
		selector();
	}
	return 0;
}

int main()
{
	return event_loop();
}
