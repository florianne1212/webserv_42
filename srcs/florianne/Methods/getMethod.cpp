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

void GetMethod::handleGet(ClientSocket &client,Config &config, Request &request, Response &response)
{
	File fileGet;
	
	//Tanguy getDirectory always false
	if(config.getDirectoryPage(client.getServerName(), request.getUrl()).state == true) 
	{
		std::cout << "\nI AM HERE IN POST \n";
		File myupload(WORKPATH + config.getDirectoryPage(client.getServerName(), request.getUrl()).value);
		fileGet = myupload;
	}
	else
	{
		File filenormal(WORKPATH + request.getUrl());
		fileGet = filenormal;
	}


	if (fileGet.isPresent()) {
		if (fileGet.isFile()) {
			setHeader(response, fileGet);
		} 
		else if (fileGet.isDirectory()) {
			if(config.getAutoIndex(client.getServerName(), request.getUrl()) && *request.getUrl().rbegin() == '/')
				setHeader_Dir(response, setDirectory(fileGet, request.getUrl(), config.getIp(client.getServerName())));
			else
				response.setStatus(404);
			
		}	
	}
	else
		response.setStatus(404);
}

std::string GetMethod::setDirectory(File &fileGet, std::string url, std::string ip)
{
	(void)ip;
	std::string response_body =
		"<html>\n"
		" <head>\n"
		"  <title>file of" + url + "</title>\n"
		" </head>\n"
		" <body>\n"
		"<h1>Index of </h1>\n";
	std::list<std::string> files_list = fileGet.listDirFiles();

	std::string file_list;
	for (std::list<std::string>::iterator it=files_list.begin(); it != files_list.end(); ++it)
	{
		std::string newurl =  WORKPATH +  url + "/" + *it ;
		File fileTest(newurl);
		if (fileTest.isPresent()) {
			if (fileTest.isFile())
				file_list = "  <a href=\"./" + *it + "\">"  + *it + "</a> <br/>\n";
			else if (fileTest.isDirectory())
				file_list = "  <a href=\"./"  + *it + "/\">"  + *it + "/ </a> <br/>\n";
		} 
		response_body += file_list;
	}
	response_body += "</body></html>";
	std::cout << response_body << std::endl;
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

void GetMethod::setHeader_Dir(Response &response, std::string html_generated)
{
	std::string file_content(html_generated);
	response.setBody(file_content);

	response.setHeaders("Content-Type", "text/html");

	std::stringstream my_stream;
	my_stream << html_generated.size();
	response.setHeaders("Content-Length", my_stream.str());
	
	//response.setHeaders("Last-Modified", fileGet.fileLastModified());
}