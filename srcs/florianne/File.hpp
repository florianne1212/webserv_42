#ifndef FILE_HPP
# define FILE_HPP

#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <list>
#include <iterator>
#include <iostream>
#include <fstream> 
#include "mime.hpp"
#include <sstream> 

class File{

	private:
		std::string _path;
		struct stat _stats;


	public:
	    File(/* args */);
		File(std::string path);
		// File(File const & copy);
		~File();
		// File& operator=(File const & ope);

		bool isPresent();
		bool isFile();
		bool isDirectory();
		bool fileDelete();
		bool fileCreate(std::string filename);
		size_t fileLength();
		std::string fileLastModified();
		std::string find_content_type();
		std::list<std::string> listDirFiles();
		std::string find_content();
		std::string convert_int_to_string(int);
		struct stat getStat();
		bool fileAppend(std::string filename, std::string to_append);


};

#endif