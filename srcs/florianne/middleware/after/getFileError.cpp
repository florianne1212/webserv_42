#include "getFileError.hpp"
// #include "G"
// #include "File.hpp"

GetFileError::~GetFileError() {
}

void GetFileError::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {
	
	// File fileGet(request.getUrl());
	(void)client;
	(void)response;
	(void)request;
	(void)config;

	std::cout << "TUTUTUasdasd";

	//response.setStatus(404);
	if(response.getStatus()/100 != 2)
	{
		usable<std::string> errorFile;
		
		errorFile = config.getPathErrorFile(response.getStatus());
		if(errorFile.state == true)
		{
			File fileError(WORKPATH + errorFile.value);
			if (fileError.isFile())
			{
				std::string file_content(fileError.find_content());
				response.setBody(file_content);

				std::string content_type = fileError.find_content_type();
				if(!content_type.empty())
					response.setHeaders("Content-Type", fileError.find_content_type());

				std::stringstream my_stream;
				my_stream << fileError.fileLength();
				response.setHeaders("Content-Length", my_stream.str());
				
				response.setHeaders("Last-Modified", fileError.fileLastModified());

			}
		}
	}


	

	next();
}