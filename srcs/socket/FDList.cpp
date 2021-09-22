#include "FDList.hpp"

#include <poll.h>
#include "ASocket.hpp"

FDList::FDList()
    : _SocketLists(), _fileList()
{
}

FDList::~FDList()
{
	for (std::list<ASocket *>::iterator it = _SocketLists.begin(); it != _SocketLists.end(); it++)
    {
		close((*it)->getFd());
		delete *it;
    }
}

void FDList::addSocket(ASocket *toAdd)
{
    _SocketLists.push_back(toAdd);
}

void FDList::addFile(struct pollfd* toAdd)
{
    _fileList.push_back(toAdd);
}

void FDList::rmSocket(const int& toRemove)
{
	std::list<ASocket *>::iterator it = _SocketLists.begin();
    while (it != _SocketLists.end() && (*it)->getFd() != toRemove)
        it++;
	if (it != _SocketLists.end())
	{
		delete *it;
		_SocketLists.erase(it);
	}
}

void FDList::rmFile(const int& toRemove)
{
    std::list<struct pollfd*>::iterator it = _fileList.begin();
    while (it != _fileList.end() && (*it)->fd != toRemove)
        it++;
    _fileList.erase(it);
}

std::list<ASocket *> FDList::getSocketList(void) const
{
    return (_SocketLists);
}

std::list<struct pollfd*> FDList::getFileList(void) const
{
    return (_fileList);
}

void FDList::myPoll()
{
    struct pollfd* tab = new struct pollfd[_SocketLists.size() + _fileList.size()];

    size_t i = 0;
    for (std::list<ASocket *>::const_iterator it = _SocketLists.begin(); it != _SocketLists.end(); it++)
    {
        tab[i] = (*it)->getPollFD();
        i++;
    }
    for (std::list<struct pollfd*>::iterator it = _fileList.begin(); it != _fileList.end(); it++)
    {
        tab[i] = *(*it);
        i++;
    }

    poll(tab, _SocketLists.size() + _fileList.size(), 1000);
    i = 0;
    for (std::list<ASocket *>::const_iterator it = _SocketLists.begin(); it != _SocketLists.end(); it++)
    {
        if (tab[i].fd == (*it)->getFd())
        (*it)->setPollFD(tab[i]);
		i++;
    }
    for (std::list<struct pollfd*>::iterator it = _fileList.begin(); it != _fileList.end(); it++)
    {
        if (tab[i].fd == (*it)->fd)
        {
            *(*it) = tab[i];
        }
        i++;
    }
    delete[] tab;
}
