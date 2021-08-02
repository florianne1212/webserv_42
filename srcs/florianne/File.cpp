#include "File.hpp"
#include "buffer.hpp"

File::File()
{
}

File::File(std::string path):
_path(path)
{
}

// File::File(File const & copy)
// {

// }

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
	
	if (found!=std::string::npos)
		ext=_path.substr(found+1);

	mime = my_mime.find_mime(ext);
	return(mime);
}

std::string File::find_content()
{
	// int fd;
	// std::string content;

	std::ifstream ifs(_path.c_str());
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
						(std::istreambuf_iterator<char>()) );

	return(content);
	
}

size_t File::fileLength()
{
	return(_stats.st_size);
}

std::string File::convert_int_to_string(int nb)
{
	std::string str;

	std::stringstream my_stream; 
	my_stream << nb;  
	my_stream >> str;

	return(str); 
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
 
    // declaring character array
    char *my_array;

	if(!(my_array = (char *)malloc(sizeof(char) * (n+1))))
    // copying the contents of the
    // string to char array
    strcpy(my_array, s.c_str());
	return(my_array);
}

bool File::fileAppend(std::string filename, std::string to_append)
{
	//std::cout << "\nto append = " << to_append << "\nfilename = " << filename << "\n";
	Buffer out(to_append, 0);
	int fd_out = ::open(filename.c_str(), O_APPEND);
	if (out.flush(fd_out) == true)
		close(fd_out);
	/*
	std::ofstream outfile;
    outfile.open(filename.c_str() , std::ofstream::out | std::ofstream::app);
    outfile << to_append;
	outfile.close();
	*/
	return(true);
}

struct stat File::getStat()
{
	return(_stats);
}