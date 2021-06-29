#ifndef WEBSERV_INCLUDES_HPP
# define WEBSERV_INCLUDES_HPP

//C LIB
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//CPP LIB
# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <exception>
# include <ctime>
# include <cstring>

//Project .hpp
# include "../utils/usable.hpp"
# include "Routes.hpp"
# include "Server.hpp"
# include "Config.hpp"

#endif