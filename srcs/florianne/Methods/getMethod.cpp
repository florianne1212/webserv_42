#include "getMethod.hpp"

GetMethod::GetMethod(/* args */)
{
}

GetMethod::GetMethod(GetMethod const & copy)
{
	(void)copy;
}

GetMethod::~GetMethod()
{
}

GetMethod& GetMethod::operator=(GetMethod const & ope)
{
	(void)ope;
	return(*this);
}

void GetMethod::handleGet(Client &client, Request &request, Response &response)
{
	
    (void)client;
	// (void)response;
	(void)request;
	std::cout << "GET\n";

	File fileGet(request.getUrl());

	if (fileGet.isPresent()) {
		if (fileGet.isFile()) {
			std::cout << "it's a file\n";
			setHeader(response, fileGet);


			// response.setContent
		} 
		else if (fileGet.isDirectory()) {
			setDirectory(fileGet, request.getUrl());
		}	
	}
	else
	{
		std::cout << "404 not found\n";
		response.setStatus(404);
	}
}

std::string GetMethod::setDirectory(File &fileGet, std::string url)
{
	std::string response_body =
		"<html>\n"
		" <head>\n"
		"  <title>file of" + url + "</title>\n"
		" </head>\n"
		" <body>\n";
	std::list<std::string> files_list = fileGet.listDirFiles();

	std::string file_list;
	for (std::list<std::string>::iterator it=files_list.begin(); it != files_list.end(); ++it)
	{
		std::string newurl = "" + url + "/" + *it + "";
		File fileTest(newurl);
		if (fileTest.isPresent()) {
			if (fileTest.isFile())
				file_list = "  <a href=\"/" + url + "/" + *it + "\">"  + *it + "</a> <br/>\n";
			else if (fileTest.isDirectory())
				file_list = "  <a href=\"/" + url + "/" + *it + "\">"  + *it + "/ </a> <br/>\n";
		} 
		response_body += file_list;
	}
	response_body += "</body></html>";
	std::cout << response_body;
			// response.setBody(listFiles(file));
	return(response_body);
}

void GetMethod::setHeader(Response &response, File &fileGet)
{
	std::string file_content(fileGet.find_content());
	response.setBody(file_content);

	std::string content_type = fileGet.find_content_type();
	if(!content_type.empty())
		response.setHeaders("Content-Type", fileGet.find_content_type());

	std::stringstream my_stream;
	my_stream << fileGet.fileLength();
	response.setHeaders("Content-Length", my_stream.str());
	
	response.setHeaders("Last-Modified", fileGet.fileLastModified());
}