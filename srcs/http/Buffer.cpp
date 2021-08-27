#include "Buffer.hpp"

Buffer::Buffer(/* args */)
{
}

Buffer::Buffer(std::string content, size_t sent):
_content(content),
_sent(sent)
{

}



Buffer::~Buffer()
{
}



bool Buffer::flush(int fd) {
	ssize_t wrote = ::write(fd, _content.c_str() + _sent, _content.length() - _sent);
	if (wrote != -1) {
		_sent += wrote;
	}

	return (_sent == _content.length());
}