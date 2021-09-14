#include "ClientSocket.hpp"
#include "ParseRequest.hpp"
#include "ManageMiddleware.hpp"

ClientSocket::ClientSocket(int fd, std::string serverName, std::string clientAddress, std::string clientPort, FDList* listFD) : ASocket(fd, serverName),
_clientAddress(clientAddress), _clientPort(clientPort), _request(), _buffer(), _responseSent(true), _test(true), _append(true),  _fd_read(), _read(true),
_listFD(listFD), _response()
{
	clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
	_cgiFd[0] = 0;
	_cgiFd[1] = 0;
	_cgiFd[2] = 0;
	_cgiFd[3] = 0;
	// std::cout << "00000000000000000000000000ouverture d'un socket client , son fd est : " << _fd << "    00000000000000000000000\n";
}

ClientSocket::~ClientSocket(){
	// std::cout << "(((((((((((((((((((((((fermeture du client socket de fd " << _fd << ")))))))))))))))))))))))))))\n\n";
	for (int i = 0; i < 4; i++)
		if (_cgiFd[i])
		{
			close (_cgiFd[i]);
			_cgiFd[i] = 0;
		}
}

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
		_request = other._request;
		_buffer = other._buffer;
		_responseSent = other._responseSent;
		_test = other._test;
		_append = other._append;
		_fd_out = other._fd_out;
		_fd_read = other._fd_read;
		_read = other._read;
		_body = other._body;
		_lastInterTime = other._lastInterTime;
		_status = other._status;
		_response = other._response;
		_cgiFd[0] = other._cgiFd[0];
		_cgiFd[1] = other._cgiFd[1];
		_cgiFd[2] = other._cgiFd[2];
		_cgiFd[3] = other._cgiFd[3];
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
	_status = _parserequest.getStatus();

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
		char BodyBuffer[131001];
		size_t rod = ::read(_fd_read.fd, BodyBuffer, 131000);
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
	// std::cout << "cache? : \n" << _response.getResponse() << " Youpi\n\n";
	if (_cgiState == NO_CGI)
		reinitResponse();
	// std::cout << "-------------cache? : \n" << _response.getResponse() << "\n\n";

	// std::cout << "l'adresse de ma reponse est  : " << &_response << "\n";
	ManageMiddleware manage;
	_response.setStatus(_status);
	if (_responseSent)
	{
		// std::cout << "cache2 : \n" << _response.getResponse() << " Youpi\n\n";
		//std::cout << "\n STATUS = "<< response.getBodyPath().state <<"\n";
		//std::cout << "\n STATUS = "<< response.getBody().state <<"\n";
		manage.middlewareStart(*this, *datas, _request, _response);
		//when there is a post open the file and append the content
		if(_response.getAppend().state == true && _append)
			my_append(&_response, listFD);
		else
			_append = false;
		//when there is a get open the file and write the content in the _body
		if(_response.getBodyPath().state == true && _response.getDir() == false && _read == true && _append == false)
			my_read(&_response, listFD);
		else
			_read = false;
		if (_append == false && _read == false)
		{
			if (_response.getBodyPath().state == true && _response.getDir() == false )
				_response.setBody(_body);
			if(_response.getCgi() == false)
		   		_response.create_response();
			// std::cout << "l'adresse de la reponse est " << &_response << "\n\n";
			// std::cout << "000000000000000000000000\n\nla reponse  envoyee au write du socket "<< _fd <<" est " << _response.getResponse() << "\n\n0000000000000000\n";
			// std::cout << "la reponse  envoyee au write du socket "<< _fd <<" est " << _response.getResponse() << "\n0000000000000000\n";
			_buffer = Buffer(_response.getResponse(), 0);
			// std::cout << "000000000000000000000000\n\nla reponse  envoyee au write du socket "<< _fd <<" est " << _response.getResponse() << "\n\n0000000000000000\n";
			_responseSent = false;
			_append = true;
			_test = true;
			_read = true;
		}
		//the response is fully created and is ready to be sent
	}
	else
	{
		//the response is send
		_responseSent = _buffer.flush(_fd);
		// if (_responseSent == true)
		if (_responseSent == true && _cgiState != CGI_IN_PROGRESS)
		//when the response is fully sent everything get closed
		{
			close(_fd);
			// std::cout <<"on est dans client avec le fd "<< _fd << " et on le ferme\n";
					// std::cout << "--------------------MAMA MIA----------------------\n";

			listFD->rmSocket(_fd);
		}
		// else
		// {
		// 	sleep(1);
		// 	exit(1);
		// }
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
	return (time_taken > 30);
}

void ClientSocket::setTime()
{
		clock_gettime(CLOCK_MONOTONIC, &_lastInterTime);
}

// void ClientSocket::setResponse(Response response)
// {
// 	_response = response;
// }

Response & ClientSocket::getResponse()
{
	return (_response);
}

void ClientSocket::reinitResponse()
{
	_response.responseClassInitialization();
}

void ClientSocket::setCgiFd(int pipeOut0, int pipeOut1, int pipeIn0, int pipeIn1 )
{
	_cgiFd[0] = pipeOut0;
	_cgiFd[1] = pipeOut1;
	_cgiFd[2] = pipeIn0;
	_cgiFd[3] = pipeIn1;
}

void ClientSocket::setCgiFd(int index, int value)
{
	_cgiFd[index] = value;
}

int ClientSocket::getCgiFdValue(int index)
{
	return (_cgiFd[index]);
}

void ClientSocket::destroyCgiSockets()
{
	std::cout << "on passe dans cgisockets destruktor !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	if (_cgiFd[2])
	{
		close (_cgiFd[2]);
		_cgiFd[2] = 0;
	}
	if (_cgiFd[3])
	{
		close (_cgiFd[3]);
		_listFD->rmSocket(_cgiFd[3]);
		_cgiFd[3] = 0;
	}
	if (_cgiFd[1])
	{
		close (_cgiFd[1]);
		_cgiFd[1] = 0;
	}
	if (_cgiFd[0])
	{
		close (_cgiFd[0]);
		_listFD->rmSocket(_cgiFd[0]);
		_cgiFd[0] = 0;
	}
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

