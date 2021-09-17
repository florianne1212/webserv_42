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
        std::list<struct pollfd*>  _fileList;

    public :
        FDList();
		~FDList();

        void addSocket(ASocket* toAdd);
        void addFile(struct pollfd* toAdd);
        void rmSocket(const int& toRemove);
        void rmFile(const int& toRemove);

        std::list<ASocket *> getSocketList(void) const;
        std::list<struct pollfd*> getFileList(void) const;
        void myPoll(void);
};

#endif
