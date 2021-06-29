#ifndef MIME_HPP
#define MIME_HPP

#include <string>
#include <map>

class Mime
{
    private:
        std::string _ext;
		std::string _mime;
		std::map<std::string, std::string> _storage_mime;

    public:
		const std::map<std::string, std::string> &init_mime();
        Mime();
		Mime(std::string ext, std::string mime);
        Mime(Mime const & copy);
        ~Mime();
        Mime & operator=(Mime const & ope);

    public:
		
		std::string find_mime(std::string ext);
		std::string get_ext();
		std::string get_mime();

    

};

#endif