#include "FDList.hpp"

#include <poll.h>
#include "ASocket.hpp"

FDList::FDList()
    : _SocketLists(), _fileList()
{
}

void FDList::addSocket(ASocket *toAdd)
{
    _SocketLists.push_back(toAdd);
}

void FDList::addFile(const struct pollfd& toAdd)
{
    _fileList.push_back(toAdd);
}

void FDList::rmSocket(const int& toRemove)
{
    std::list<ASocket *>::iterator it = _SocketLists.begin();
    while (it != _SocketLists.end() && (*it)->getFd() != toRemove)
        it++;
    _SocketLists.erase(it);
}

void FDList::rmFile(const int& toRemove)
{
    std::list<struct pollfd>::iterator it = _fileList.begin();
    while (it != _fileList.end() && it->fd != toRemove)
        it++;
    _fileList.erase(it);
}

std::list<ASocket *> FDList::getSocketList(void) const
{
    return (_SocketLists);
}

std::list<struct pollfd> FDList::getFileList(void) const
{
    return (_fileList);
}

pollfd *FDList::getPollList() const
{
    struct pollfd* ret = new struct pollfd[_SocketLists.size() + _fileList.size()];

    size_t i = 0;
    for (std::list<ASocket *>::const_iterator it = _SocketLists.begin(); it != _SocketLists.end(); it++)
    {
        ret[i] = (*it)->getPollFD();
        i++;
    }
    
    for (std::list<struct pollfd>::const_iterator it = _fileList.begin(); it != _fileList.end(); it++)
    {
        ret[i] = *it;
        i++;
    }

    return ret;
}

size_t FDList::getSizePollList(void) const
{
    return _SocketLists.size() + _fileList.size();
}