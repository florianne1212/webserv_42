#ifndef HttpStatus_HPP
#define HttpStatus_HPP

#include <string>
#include <map>

class HttpStatus
{
    private:
        int _code;
		std::string _message;
		std::map<int, std::string> _status_error;

    public:
        HttpStatus();
		HttpStatus(int code, std::string message);
        HttpStatus(HttpStatus const & copy);
        ~HttpStatus();
        HttpStatus & operator=(HttpStatus const & ope);

    public:

	std::string find_message(int code);
	int get_code();
	std::string get_message();

    

};

// GET 
// POST
// DELETE

#endif