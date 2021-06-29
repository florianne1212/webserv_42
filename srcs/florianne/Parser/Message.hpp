#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

class Message
{
private:
	std::string _message;
public:
    Message();
	Message(std::string message);
    Message(Message const & copy);
    ~Message();
    Message & operator=(Message const & ope);

public:
	std::string get_message()
	{
		return(_message);
	}

};

// GET 
// POST
// DELETE

#endif