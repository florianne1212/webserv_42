#ifndef BUFFER_HPP
# define BUFFER_HPP

class Buffer;
# include <string>
# include <unistd.h>

class Buffer
{
private:
	std::string _content;
	size_t _sent;
public:
	
    Buffer();
	Buffer(std::string content, size_t sent);
    ~Buffer();
	bool flush(int fd);
};

#endif