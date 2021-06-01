#include "client.hpp"

Client::Client(int fd) : _fd(fd) {}

Client::~Client(){}

Client::Client(const Client & other) :
	_fd(other._fd), _bareRequest(other._bareRequest)
{}

Client& Client::operator=(const Client & other){
	if (this !=  &other)
	{
		_fd = other._fd;
		_bareRequest = other._bareRequest;
	}
	return (*this);
}

std::string Client::getBareRequest() const {
	return (_bareRequest);
}