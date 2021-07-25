#include "ClientSocket.hpp"
#include "../Parser/parseRequest.hpp"
#include "../middleware/ManageMiddleware.hpp"
#include "../buffer.hpp"



ClientSocket::ClientSocket(int fd, std::string serverName, std::string clientAddress, std::string clientPort) : ASocket(fd, serverName), _bareRequest(), _bareAnswer(),
_clientAddress(clientAddress), _clientPort(clientPort)
{}

ClientSocket::~ClientSocket(){}
//en fonction, probable delete[] a faire ou pas si vector

ClientSocket::ClientSocket(const ClientSocket & other) : ASocket(){
	*this = other;
}

ClientSocket & ClientSocket::operator=(const ClientSocket & other){
	if (this != &other)
	{
		ASocket::operator=(other);
		_bareAnswer = other._bareAnswer;
		_bareRequest = other._bareRequest;
		_clientAddress = other._clientAddress;
		_clientPort = other._clientPort;
	}
	return (*this);
}

int	ClientSocket::getFd(void) const{
	return (_fd);
}

Request _request;

void ClientSocket::read(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
	char line_buf[5001] ={'\n'};
	int i;
	char c;
	ParseRequest _parserequest;
	ParseHeaderFields _parseheader;


	i = 0;
	::read(_fd, line_buf, 5000);
	line_buf[5000] = '\0';
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

	_parseheader = _parserequest.get_parserfields();
	std::map<std::string, std::string>  mymap = _parseheader.get_headers_map();
	_request.setHeaders(mymap);

	// std::cout << "\nmethod = -" << _request.getMethods() << "-\npath = ";
	// std::cout << _request.getUrl() << "-\nmajor = ";
	// std::cout << _parserequest.get_major() << "-\nminor = ";
	// std::cout << _parserequest.get_minor() << "\n";
	// std::cout << _parseheader.get_field();

	// for (std::map<std::string, std::string>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    // 	std::cout << it->first << " => " << it->second << '\n';

	_pollFD.events = POLLOUT;
}

void ClientSocket::write(Config *datas, FDList *listFD)
{
	bool _responseSent = true;
	Response response;
	ManageMiddleware manage;

	if (_responseSent)
	{
		manage.middlewareStart(*this, *datas, _request, response);


		response.create_response();
		// size_t len = response.getResponse().length();
		// ssize_t sent = ::write(_fd, response.getResponse().c_str(), len);

		_responseSent = false;
	}
	Buffer _buffer(response.getResponse(), 0);
	//if (_sendableReponse && !_responseSent) {
	if (!_responseSent) {
		_responseSent = _buffer.flush(_fd);	
	}


	listFD->rmSocket(_fd);
	close(_fd);
}

std::string ClientSocket::getClientAddress(void) const {
	return (_clientAddress);
}

std::string ClientSocket::getClientPort(void) const {
	return (_clientPort);
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
