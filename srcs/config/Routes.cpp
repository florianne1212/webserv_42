Routes::Routes()
: _httpRequest(), _httpRedirection(), _rooted(), _autoIndex(), _directoryPage(), _bodyMaxSize()
{}

Routes::Routes(const Routes & toCopie)
: _httpRequest(toCopie._httpRequest), _httpRedirection(toCopie._httpRedirection), _rooted(toCopie._rooted),
_autoIndex(toCopie._autoIndex), _directoryPage(toCopie._directoryPage), _bodyMaxSize(toCopie._bodyMaxSize)
{}

void Routes::setHttpRequest(char httpRequest)
{
	_httpRequest = usable<char>(httpRequest)
}

void Routes::setHttpRedirection(pair<int, std::string> httpRedirection)
{
	_httpRedirection = usable<pair<int, std::string> >(httpRedirection);
}

void Routes::setRooted(std::string rooted)
{
	_rooted = usable<std::string>(rooted);
}

void Routes::setAutoIndex(bool autoIndex)
{
	_autoIndex = usable<bool>(autoIndex);
}

void Routes::setDirectoryPage(std::string directoryPage)
{
	_directoryPage = usable<std::string>(directoryPage);
}

void Routes::setBodyMaxSize()
{
	_bodyMaxSize = usable<size_t>(bodyMaxSize);
}

vector<std::string> getHttpRequest(void) const
{
	vector<std::string> ret;
	size_t tmp;

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
