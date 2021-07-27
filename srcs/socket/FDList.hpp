#ifndef FDLIST_HPP
# define FDLIST_HPP

class FDList;

# include <list>
# include <poll.h>
# include "ASocket.hpp"

class FDList
{
    private :
        std::list<ASocket *>        _SocketLists;
        std::list<struct pollfd>  _fileList;

    public :
        FDList();
        
        void addSocket(ASocket* toAdd);
        void addFile(const struct pollfd& toAdd);
        void rmSocket(const int& toRemove);
        void rmFile(const int& toRemove);

        std::list<ASocket *> getSocketList(void) const;
        std::list<struct pollfd> getFileList(void) const;
        bool myPoll(void);
};

#endif