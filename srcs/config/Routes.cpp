#include "Routes.hpp"

void Routes::putRoutes()
{
	if (_bodyMaxSize.state == true)
		std::cout <<  "  - bodyMaxSize: " << _bodyMaxSize.value << std::endl;
	if (_directoryPage.state == true)
		std::cout <<  "  - directoryPage: " << _directoryPage.value << std::endl;
	if (_root.state == true)
		std::cout <<  "  - root: " << _root.value << std::endl;
	if (_autoIndex.state == true)
		std::cout <<  "  - autoIndex: " << _autoIndex.value << std::endl;
	if (_httpRedirection.state == true)
		std::cout << "  - httpRedirection: " << _httpRedirection.value.first << "=>" << _httpRedirection.value.second << std::endl;
	if (_httpRequest.state == true)
	{
		std::cout << "  - httpRequest: ";
		for (std::vector<std::string>::iterator it = _httpRequest.value.begin(); it != _httpRequest.value.end(); it++)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
}

void Routes::checker()
{
	struct stat useless;
	if (_root.state)
		if (stat((WORKPATH + _root.value).c_str(), &useless) != 0)
			throw std::string(_root.value + " is a unknow file or directory");
	if (_directoryPage.state)
		if (stat((WORKPATH + _directoryPage.value).c_str(), &useless) != 0)
			throw std::string(_directoryPage.value + " is a unknow file");
}

Routes::Routes()
: _httpRequest(), _httpRedirection(), _root(), _autoIndex(), _directoryPage(), _bodyMaxSize()
{}

Routes::Routes(const Routes & toCopie)
: _httpRequest(toCopie._httpRequest), _httpRedirection(toCopie._httpRedirection), _root(toCopie._root),
_autoIndex(toCopie._autoIndex), _directoryPage(toCopie._directoryPage), _bodyMaxSize(toCopie._bodyMaxSize)
{}

bool Routes::setHttpRequest(std::vector<std::string> httpRequest)
{
	if (_httpRequest.state == true)
		return false;

	std::vector<std::string> ret;
	size_t i = 0;

	while (i < httpRequest.size())
	{
		if (httpRequest[i] == "GET" && httpRequest[i] == "POST" && httpRequest[i] ==  "DELETE")
			return false;
		i++;
	}
	_httpRequest = usable<std::vector<std::string> >(httpRequest);
	return true;
}

bool Routes::setHttpRedirection(std::vector<std::string> httpRedirection)
{
	if (_httpRedirection.state == true || (httpRedirection.size() < 1 && httpRedirection.size() > 2))
		return false;
	int ret = atoi(httpRedirection[0].c_str());
	if (ret < 100 || ret > 599)
		return false;
	if (ret >= 300 && ret < 400 && httpRedirection.size() != 2)
		return false;
	if (httpRedirection.size() == 1)
		_httpRedirection = usable<std::pair<int, std::string> >(std::pair<int, std::string >(ret, ""));
	else
		_httpRedirection = usable<std::pair<int, std::string> >(std::pair<int, std::string >(ret, httpRedirection[1]));
	return true;
}

bool Routes::setRoot(std::vector<std::string> root)
{
	if (_root.state == true || root.size() != 1)
		return false;
	_root = usable<std::string>(root[0]);
	return true;
}

bool Routes::setAutoIndex(std::vector<std::string> autoIndex)
{
	bool ret;

	if (_autoIndex.state == true || autoIndex.size() != 1)
		return false;
	if (autoIndex[0] != "on" && autoIndex[0] != "off")
		return false;
	if (autoIndex[0] == "on")
		ret = 1;
	if (autoIndex[0] == "off")
		ret = 0;
	_autoIndex = usable<bool>(ret);
	return true;
}

bool Routes::setDirectoryPage(std::vector<std::string> directoryPage)
{
	if (_directoryPage.state == true || directoryPage.size() != 1)
		return false;
	_directoryPage = usable<std::string>(directoryPage[0]);
	return true;
}

bool Routes::setBodyMaxSize(size_t bodyMaxSize)
{
	if (_bodyMaxSize.state == true)
		return false;
	_bodyMaxSize = usable<size_t>(bodyMaxSize);
	return true;
}
