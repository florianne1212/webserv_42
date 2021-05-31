#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <exception>
#include <ctime>
#include <cstring>

class Client
{
	private:
		Client();
		int _fd;
		std::string _bareRequest;

	public:
		Client(int fd);
		~Client();
		Client(const Client & other);
		Client operator=(const Client & other);

		std::string getBareRequest() const;
		// void setBareRequest(int fd);
		void readInfo(int fd);
		
		

};


#endif