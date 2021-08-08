#include "httpStatus.hpp"
#include <map>


HttpStatus::HttpStatus():
_code(0),
_message()
{
	_status_error[100]="Continue";
	_status_error[101]="Switching Protocol";
	_status_error[103]="Processing";
	_status_error[200]="OK";
	_status_error[201]="Created";
	_status_error[202]="Accepted";
	_status_error[203]="Non-Authoritative Information";
	_status_error[204]="No Content";
	_status_error[205]="Reset Content";
	_status_error[206]="Partial Content";
	_status_error[207]="Multi-Status";
	_status_error[208]="Already Reported";
	_status_error[226]="IM Used";
	_status_error[300]="Multiple Choice";
	_status_error[301]="Moved Permanently";
	_status_error[302]="Found";
	_status_error[303]="See Other";
	_status_error[304]="Not Modified";
	_status_error[305]="Use Proxy";
	_status_error[306]="unused";
	_status_error[307]="Temporary Redirect";
	_status_error[400]="Bad Request";
	_status_error[401]="Unauthorized";
	_status_error[402]="Payment Required ";
	_status_error[403]="Forbidden";
	_status_error[404]="Not Found";
	_status_error[405]="Method Not Allowed";
	_status_error[406]="Not Acceptable";
	_status_error[407]="Proxy Authentication Required";
	_status_error[408]="Request Timeout";
	_status_error[409]="Conflict";
	_status_error[410]="Gone";
	_status_error[411]="Length Required";
	_status_error[412]="Precondition Failed";
	_status_error[413]="Payload Too Large";
	_status_error[414]="URI Too Long";
	_status_error[415]="Unsupported Media Type";
	_status_error[416]="Range Not Satisfiable";
	_status_error[417]="Expectation Failed";
	_status_error[418]="I'm a teapot";
	_status_error[421]="Misdirected Request";
	_status_error[422]="Unprocessable Entity";
	_status_error[423]="Locked";
	_status_error[424]="Failed Dependency";
	_status_error[425]="Too Early";
	_status_error[426]="Upgrade Required";
	_status_error[428]="Precondition Required";
	_status_error[429]="Too Many Requests";
	_status_error[431]="Request Header Fields Too Large";
	_status_error[451]="Unavailable For Legal Reasons";
	_status_error[500]="Internal Server Error";
	_status_error[501]="Not Implemented";
	_status_error[502]="Bad Gateway";
	_status_error[503]="Service Unavailable";
	_status_error[504]="Gateway Timeout";
	_status_error[505]="HTTP Version Not Supported";
	_status_error[506]="Variant Also Negotiates";
	_status_error[507]="Insufficient Storage";
	_status_error[508]="Loop Detected";
	_status_error[510]="Not Extended";
	_status_error[511]="Network Authentication Required";
}

HttpStatus::HttpStatus(int code, std::string message):
_code(code),
_message(message)
{

}

HttpStatus::HttpStatus(HttpStatus const & copy)
{
	*this = copy;
}

HttpStatus::~HttpStatus()
{
}

HttpStatus& HttpStatus::operator=(HttpStatus const & ope)
{
    if(this != &ope)
	{
		_message = ope._message;
		_code = ope._code;
	}
	return(*this);
}

std::string HttpStatus::find_message(int code)
{
	std::string message = _status_error.find(code)->second;
	return(message);
}

int HttpStatus::get_code()
{
	return(_code);
}

std::string HttpStatus::get_message()
{
	return(_message);
}
