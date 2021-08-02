#ifndef WEBSERV_INCLUDES_HPP
# define WEBSERV_INCLUDES_HPP

#define WORKPATH "./workDir"
// # define WORKPATH "./www"

//C LIB
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//CPP LIB
# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <list>
# include <map>
# include <algorithm>
# include <exception>
# include <ctime>
# include <cstring>
# include <sstream>

//Project .hpp
# include "usable.hpp"
//# include "client.hpp"
# include "ASocket.hpp"
# include "FDList.hpp"
# include "Routes.hpp"
# include "Server.hpp"
# include "Config.hpp"

#endif
