#include "getFileError.hpp"
// #include "G"
// #include "File.hpp"

GetFileError::~GetFileError() {
}

void GetFileError::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {
	(void)client;
	(void)config;
	(void)request;

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

	
				response.setHeaders("Content-Length", fileError.fileLengthStr());
				
				response.setHeaders("Last-Modified", fileError.fileLastModified());

			}
		}
	}



	next();
}