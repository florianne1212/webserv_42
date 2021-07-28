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
	
    Buffer(/* args */);
	Buffer(std::string content, size_t sent);
    // buffer(buffer const & copy);
    ~Buffer();
    // buffer& operator=(buffer const & ope);
	bool flush(int fd);
};

//

#endif