#include "ClientSocket.hpp"
#include "parseRequest.hpp"
#include "ManageMiddleware.hpp"

ClientSocket::ClientSocket(int fd, std::string serverName, std::string clientAddress, std::string clientPort, FDList* listFD) : ASocket(fd, serverName),
_clientAddress(clientAddress), _clientPort(clientPort), _request(), _buffer(), _responseSent(true), _test(true), _append(true),  _fd_read(), _read(true),
_listFD(listFD)
{
	clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
}

ClientSocket::~ClientSocket(){}


ClientSocket::ClientSocket(const ClientSocket & other) : ASocket(){
	*this = other;
}

ClientSocket & ClientSocket::operator=(const ClientSocket & other){
	if (this != &other)
	{
		ASocket::operator=(other);
		_clientAddress = other._clientAddress;
		_clientPort = other._clientPort;
		_listFD = other._listFD;
	}
	return (*this);
}

int	ClientSocket::getFd(void) const{
	return (_fd);
}


FDList* ClientSocket::getListFD(void){
	return (_listFD);
}

void ClientSocket::read(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
	char line_buf[5001] ={'\n'};
	int i;
	char c;
	ParseRequest _parserequest(*datas);
	ParseHeaderFields _parseheader;


	i = 0;
	size_t ret = ::read(_fd, line_buf, 5000);
	line_buf[ret] = '\0';
	printf("%s", line_buf);

	while (line_buf[i])
	{
		c = line_buf[i];
		_parserequest.parse(c);
		i++;
	}

	_request.setMethods(_parserequest.get_method());
	_request.setUrl(_parserequest.get_path());
	_request.setBody(_parserequest.get_body());

	_request.setError(_parserequest.get_error());

	_parseheader = _parserequest.get_parserfields();
	std::map<std::string, std::string>  mymap = _parseheader.get_headers_map();
	_request.setHeaders(mymap);
	_request.setParsedUri(parseTheUri(_request.getUrl()));

	//std::cout << "\nMY BODY IS = \n" <<  _parserequest.get_body() << "\n";

	_pollFD.events = POLLOUT;
}

void ClientSocket::my_append(Response *response, FDList *listFD)
{
	Buffer out(response->getAppend().value.second, 0);
	if(_test == true )
	{
		_fd_out.fd = ::open(response->getAppend().value.first.c_str() , O_WRONLY|O_APPEND);
		_fd_out.events = POLLOUT;
		listFD->addFile(&_fd_out);
		_test = false;
	}
	if (_fd_out.revents == POLLOUT)
		_append = !out.flush(_fd_out.fd);
	if (_append == false)
	{
		close(_fd_out.fd);
		listFD->rmFile(_fd_out.fd);
		_test = true;
	}

}

void ClientSocket::my_read(Response *response, FDList *listFD)
{
	struct stat buf;
	if (stat(response->getBodyPath().value.c_str(), &buf) == -1)
	{
		_read = false;
		return ;
	}
	if(_test == true )
	{
		_fd_read.fd = ::open(response->getBodyPath().value.c_str() , O_RDONLY);
		_fd_read.events = POLLIN;
		listFD->addFile(&_fd_read);
		_test = false;
		return ;
	}
	if (_fd_read.revents == POLLIN)
	{
		char BodyBuffer[10001];
		size_t rod = ::read(_fd_read.fd, BodyBuffer, 10000);
		BodyBuffer[rod] = '\0';
		if (rod > 0)
		{
			std::string str(BodyBuffer, rod);
			_body.append(str);
		}
		else
		{
			close(_fd_read.fd);
			listFD->rmFile(_fd_read.fd);
			_test = true;
			_read = false;
		}
	}
	else
	{
		close(_fd_read.fd);
		listFD->rmFile(_fd_read.fd);
		_test = true;
		_read = false;
	}
}

void ClientSocket::write(Config *datas, FDList *listFD)
{
	Response response;
	ManageMiddleware manage;

	if (_responseSent)
	{
		//std::cout << "\n STATUS = "<< response.getBodyPath().state <<"\n";
		//std::cout << "\n STATUS = "<< response.getBody().state <<"\n";
		manage.middlewareStart(*this, *datas, _request, response);
		if(response.getAppend().state == true && _append)
			my_append(&response, listFD);
		else
			_append = false;
		if(response.getBodyPath().state == true && response.getDir() == false && _read == true && _append == false)
			my_read(&response, listFD);
		else
			_read = false;
		if (_append == false && _read == false)
		{
			if (response.getBodyPath().state == true && response.getDir() == false )
				response.setBody(_body);
			if(response.getCgi() == false)
		   		response.create_response();
			_buffer = Buffer(response.getResponse(), 0);
			_responseSent = false;
			_append = true;
			_test = true;
			_read = true;
		}
	}
	else
	{
		_responseSent = _buffer.flush(_fd);
		if (_responseSent == true)
		{
			close(_fd);
			listFD->rmSocket(_fd);
		}
	}

}

std::string ClientSocket::getClientAddress(void) const {
	return (_clientAddress);
}

std::string ClientSocket::getClientPort(void) const {
	return (_clientPort);
}

bool ClientSocket::getTimeout()
{
	struct timespec act;
	double time_taken;

	clock_gettime(CLOCK_MONOTONIC, &act);
	time_taken = (act.tv_sec - _lastInterTime.tv_sec) * 1e9;
    time_taken = (time_taken + (act.tv_nsec - _lastInterTime.tv_nsec)) * 1e-9;
	return (time_taken > 5);
}

void ClientSocket::setTime()
{
		clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
}

/*
class ClientSocket {
	Response _response;
	bool _sendableReponse;
	bool _responseSent;
	void write() {
		if (_sendableResponse && !_responseSent) {
			_responseSent = _response.write()
		}
	}
}
*/

/*
class Buffer {
	std::string _content;
	size_t _sent;
	bool flush(int fd) {
		ssize_t wrote = ::wrote(fd, _content.c_str() + _sent, _content.length() - _sent);
		if (wrote != -1) {
			_sent += wrote;
		}
		return (_sent == _content.length());
	}
}
*/

