#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <vector>
#include <string>


class Request
{
	private:
		std::string _methods;
		std::string _url;
		std::string _version;
		std::string _body;
		std::map<std::string, std::string> _headers;
		

	public:
		Request();
		Request(Request const & copy);
		~Request();
		Request& operator=(Request const & ope);

		std::vector<std::string> createMethods();
		std::string getMethods();
		std::string getUrl();
		std::string getBody();
		std::string getVersion();
		std::map<std::string, std::string> getHeaders();

		void setMethods(std::string str);
		void setUrl(std::string str);
		void setBody(std::string str);
		void setVersion(std::string str);
		void setHeaders(std::map<std::string, std::string> header);
};

//methods
//url
//headers
//page

#endif