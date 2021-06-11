#include "Routes.hpp"

Routes::Routes()
: _httpRequest(), _httpRedirection(), _rooted(), _autoIndex(), _directoryPage(), _bodyMaxSize()
{}

Routes::Routes(const Routes & toCopie)
: _httpRequest(toCopie._httpRequest), _httpRedirection(toCopie._httpRedirection), _rooted(toCopie._rooted),
_autoIndex(toCopie._autoIndex), _directoryPage(toCopie._directoryPage), _bodyMaxSize(toCopie._bodyMaxSize)
{}

bool Routes::setHttpRequest(char httpRequest)
{
	if (_httpRequest.state == true)
		return false;
	_httpRequest = usable<char>(httpRequest);
	return true;
}

bool Routes::setHttpRedirection(std::pair<int, std::string> httpRedirection)
{
	if (_httpRequest.state == true)
		return false;
	_httpRedirection = usable<std::pair<int, std::string> >(httpRedirection);
	return true;
}

bool Routes::setRooted(std::string rooted)
{
	if (_httpRequest.state == true)
		return false;
	_rooted = usable<std::string>(rooted);
	return true;
}

bool Routes::setAutoIndex(bool autoIndex)
{
	if (_httpRequest.state == true)
		return false;
	_autoIndex = usable<bool>(autoIndex);
	return true;
}

bool Routes::setDirectoryPage(std::string directoryPage)
{
	if (_httpRequest.state == true)
		return false;
	_directoryPage = usable<std::string>(directoryPage);
	return true;
}

bool Routes::setBodyMaxSize(size_t bodyMaxSize)
{
	if (_httpRequest.state == true)
		return false;
	_bodyMaxSize = usable<size_t>(bodyMaxSize);
	return true;
}

std::vector<std::string> Routes::getHttpRequest(void) const
{
	std::vector<std::string> ret;
	char tmp;

	if (_httpRequest.state == false)
		tmp = 7;
	else
		tmp = _httpRequest.value;

	if (tmp >= 4)
	{
		ret.push_back("DELETE");
		tmp -= 4;
	}
	if (tmp >= 2)
	{
		ret.push_back("POST");
		tmp -= 2;
	}
	if (tmp >= 1)
	{
		ret.push_back("GET");
		tmp -= 1;
	}
	return ret;
}
