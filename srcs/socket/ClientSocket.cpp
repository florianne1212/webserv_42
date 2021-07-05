#include "ClientSocket.hpp"
#include "../Parser/parseRequest.hpp"
#include "../request.hpp"


ClientSocket::ClientSocket(int fd) : ASocket(fd), _bareRequest(), _bareAnswer(){}

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
	}
	return (*this);
}

int	ClientSocket::getFd(void) const{
	return (_fd);
}

void ClientSocket::read(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
	char line_buf[1001];
	int i;
	char c;
	ParseRequest _parserequest;
	ParseHeaderFields _parseheader;
	Request _request;

	i = 0;
	::read(_fd, line_buf, 1000);
	line_buf[1000] = '\0';
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

	std::cout << "\nmethod = -" << _request.getMethods() << "-\npath = ";
	std::cout << _request.getUrl() << "-\nmajor = ";
	std::cout << _parserequest.get_major() << "-\nminor = ";
	std::cout << _parserequest.get_minor() << "\n";
	std::cout << _parseheader.get_field();

	for (std::map<std::string, std::string>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    	std::cout << it->first << " => " << it->second << '\n';
}

void ClientSocket::write(Config *datas, FDList *listFD)
{
	(void)datas;
	(void)listFD;
	//a fair
}