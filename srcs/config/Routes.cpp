#include "Routes.hpp"

void Routes::putRoutes()
{
	if (_bodyMaxSize.state == true)
		std::cout <<  "  - bodyMaxSize :" << _bodyMaxSize.value << std::endl;
	if (_directoryPage.state == true)
		std::cout <<  "  - directoryPage :" << _directoryPage.value << std::endl;
	if (_root.state == true)
		std::cout <<  "  - root :" << _root.value << std::endl;
	if (_autoIndex.state == true)
		std::cout <<  "  - autoIndex :" << _autoIndex.value << std::endl;
	if (_httpRedirection.state == true)
		std::cout << "  - httpRedirection :" << _httpRedirection.value.first << "=>" << _httpRedirection.value.second << std::endl;
	if (_httpRequest.state == true)
	{
		std::cout << "  - httpRequest :";
		for (std::vector<std::string>::iterator it = _httpRequest.value.begin(); it != _httpRequest.value.end(); it++)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
}

Routes::Routes()
: _httpRequest(), _httpRedirection(), _root(), _autoIndex(), _directoryPage(), _bodyMaxSize()
{}

Routes::Routes(const Routes & toCopie)
: _httpRequest(toCopie._httpRequest), _httpRedirection(toCopie._httpRedirection), _root(toCopie._root),
_autoIndex(toCopie._autoIndex), _directoryPage(toCopie._directoryPage), _bodyMaxSize(toCopie._bodyMaxSize)
{}

bool Routes::setHttpRequest(std::string httpRequest)
{
	if (_httpRequest.state == true)
		return false;

	std::vector<std::string> ret;

	while (httpRequest.size())
	{
		if (strncmp(httpRequest.c_str(), "GET", 3) == 0)
		{
			ret.push_back(httpRequest.substr(0, 3));
			httpRequest.erase(0, 3);
		}
		else if (strncmp(httpRequest.c_str(), "POST", 4) == 0)
		{
			ret.push_back(httpRequest.substr(0, 4));
			httpRequest.erase(0, 4);
		}
		else if (strncmp(httpRequest.c_str(), "DELETE", 6) == 0)
		{
			ret.push_back(httpRequest.substr(0, 6));
			httpRequest.erase(0, 6);
		}
		else
			return false;
	}
	_httpRequest = usable<std::vector<std::string> >(ret);
	return true;
}

bool Routes::setHttpRedirection(std::string httpRedirection)
{
	if (_httpRedirection.state == true)
		return false;

	int ret = atoi(httpRedirection.substr(0, 3).c_str());
	if (ret < 100 || ret > 599)
		return false;
 	httpRedirection.erase(0, 3);
	_httpRedirection = usable<std::pair<int, std::string> >(std::pair<int, std::string >(ret, httpRedirection));
	return true;
}

bool Routes::setRoot(std::string root)
{
	if (_root.state == true)
		return false;
	_root = usable<std::string>(root);
	return true;
}

bool Routes::setAutoIndex(std::string autoIndex)
{
	bool ret;

	if (_autoIndex.state == true)
		return false;
	if (autoIndex != "on" && autoIndex != "off")
		return false;
	if (autoIndex == "on")
		ret = 1;
	if (autoIndex == "off")
		ret = 0;
	_autoIndex = usable<bool>(ret);
	return true;
}

bool Routes::setDirectoryPage(std::string directoryPage)
{
	if (_directoryPage.state == true)
		return false;
	_directoryPage = usable<std::string>(directoryPage);
	return true;
}

bool Routes::setBodyMaxSize(size_t bodyMaxSize)
{
	if (_bodyMaxSize.state == true)
		return false;
	_bodyMaxSize = usable<size_t>(bodyMaxSize);
	return true;
}
