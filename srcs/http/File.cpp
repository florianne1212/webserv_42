#include "File.hpp"
#include "Buffer.hpp"
#include <algorithm> 

File::File()
{
}

File::File(std::string path):
_path(path)
{
}



File::~File()
{

}


bool File::isPresent()
{
	if(::stat(_path.c_str(), &_stats) == -1)
		return (false);
	else 
		return (true);
}

bool File::isFile()
{
	if(::stat(_path.c_str(), &_stats) == -1)
		return (false);
	else 
		return(S_ISREG(_stats.st_mode));
}

bool File::isDirectory()
{
	if(::stat(_path.c_str(), &_stats) == -1)
		return (false);
	else 
		return(S_ISDIR(_stats.st_mode));
}

bool File::fileDelete()
{
	if (remove(_path.c_str()) == 0)
		return (true);
	else
		return (false);
}

bool File::fileCreate(std::string filename)
{
	int fd;

	fd = open(filename.c_str(), O_CREAT, 0644);
	if(fd == -1)
		return (false);
	close(fd);

	return(true);

}

std::list<std::string> File::listDirFiles()
{
	DIR *d;
	struct dirent *dir;
	std::list<std::string> files_list;

	d = opendir(_path.c_str());
	if(d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			files_list.push_back(dir->d_name);
		}
		closedir(d);
	}
	
	return(files_list);
}

std::string File::find_content_type()
{

	std::string ext;
	std::size_t found = _path.find_last_of(".");
	std::string mime;
	Mime my_mime;
	if (found != std::string::npos && found != 0)
	{
		if(_path.size() > (found+1))
			ext=_path.substr(found+1);
		if(!ext.empty())
			mime = my_mime.find_mime(ext);
		return(mime);
	}
	return("");

	
}

std::string File::int_to_str(int n)
{
	if (n == 0)
		return "0";
	bool sign = true; //false represents "-"
	if (n < 0)
	{
		sign = false;
		n = -n;
	}
	std::string ret;
	if (!sign)
		ret.push_back('-');
	while (n != 0)
	{
		ret.push_back('0' + n % 10);
		n /= 10;
	}
	if (sign)
	    std::reverse(ret.begin(), ret.end());
	else
	    std::reverse(ret.begin() + 1, ret.end());
	return ret;
}

size_t File::fileLength()
{
	return(_stats.st_size);
}

std::string File::fileLengthStr()
{
	return(int_to_str(_stats.st_size));
}


std::string File::fileLastModified()
{
	struct tm *info;
	info = localtime(&_stats.st_ctime);
	char my_time [50];

	strftime(my_time, 50,"%a, %d %b %G %T GMT", info);

	std::string str;

	int i = 0;
	while (my_time[i])
	{
		str.push_back(my_time[i]);
		i++;
	}
	return(str);
}

char *File::str_to_char(std::string s)
{
    int n = s.length();
    char *my_array;

	if(!(my_array = (char *)malloc(sizeof(char) * (n+1))))
    strcpy(my_array, s.c_str());
	return(my_array);
}

bool File::fileAppend(std::string filename, std::string to_append)
{
	Buffer out(to_append, 0);
	int fd_out = ::open(filename.c_str(), O_APPEND);
	if (out.flush(fd_out) == true)
		close(fd_out);
	return(true);
}

struct stat File::getStat()
{
	return(_stats);
}

std::string File::getPath()
{
	return (_path);
}