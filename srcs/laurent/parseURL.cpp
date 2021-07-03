#include <map>
#include <iostream>
#include <string>
#include <cstring>



//scheme-- : -------------------------------------path-------------------------------
//				\									/	 \			/  \			/
//					//------------host------------			? query		#fragment
//					\			/		\		/
//					  user_info@		: port

//parsing simple de l'URL
//renvoie une map aec chacun des elements

std::map<std::string, std::string> parseTheUri(std::string url)
{
	size_t found;
	std::map<std::string, std::string> parsedUrl;
	std::string userinfo;

//on cherche le scheme
	found = url.find_first_of(":");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("scheme", url.substr(0, found)));
		url = url.substr(found + 1);
	}
	//checking scheme (doit etre http pour nous...)
	if (parsedUrl["scheme"] != "http")
	{
		std::cout << "je ne supporte que le http !!" << std::endl;
		parsedUrl.clear();
		return (parsedUrl);
	}
//on cherche le fragment
	found = url.find_first_of("#");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("fragment", url.substr(found + 1)));
		url = url.substr(0, found);
		//a priori pas besoin de checker car le cgi renvoie erreur si pas bon
	}
//on cherche le query
	found = url.find_first_of("?");
	if (found != url.npos)
	{
		parsedUrl.insert(std::make_pair("query", url.substr(found + 1)));
		url = url.substr(0, found);
	}

//on cherche si authority
	if (url.substr(0, 2) == "//") //il existe authority, a parser,
	{
		url = url.substr(2);
		found = url.find_first_of("/");//on separe le chemin
		if (found == url.npos)
		{
			std::cout << "probleme : pas de path trouve" << std::endl;
			parsedUrl.clear();
			return (parsedUrl);
		}
		parsedUrl.insert(std::make_pair("path", url.substr(found)));
		url = url.substr(0, found); //ne reste plus que le segment authorisation
		found = url.find_first_of("@");//on cherche si userinfo
		if (found != url.npos)
		{
			userinfo = url.substr(0, found);
			url =url.substr(found + 1);
			// on parse userinfo
			found = userinfo.find_first_of(":");
			if (found != url.npos)
			{
				parsedUrl.insert(std::make_pair("password", userinfo.substr(found + 1)));
				userinfo = userinfo.substr(0, found);
			}
			parsedUrl.insert(std::make_pair("user_name", userinfo.substr(0,found)));
		}
		// reste plus que host et port (attention , ne gere pas ipv6)
		found = url.find_first_of(":");//on cherche si port
		if (found != url.npos)
		{
			parsedUrl.insert(std::make_pair("port", url.substr(found + 1)));
			parsedUrl.insert(std::make_pair("host", url.substr(0, found)));
		}
		else
		{
			parsedUrl.insert(std::make_pair("port", "80"));
			parsedUrl.insert(std::make_pair("host", url));
		}
	}
	else
		parsedUrl.insert(std::make_pair("path", url));
	return (parsedUrl);

}

int main(int argc,char* argv[])
{
	if (argc != 2)
		return (0);
	std::map<std::string, std::string> parseduri = parseTheUri(argv[1]);
	std::map<std::string, std::string>::iterator it;
	for (it = parseduri.begin(); it != parseduri.end(); ++it)
		std::cout << it->first << "->" << it->second <<std::endl;
	return (0);
}