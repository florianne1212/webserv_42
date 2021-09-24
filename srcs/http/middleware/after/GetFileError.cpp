#include "GetFileError.hpp"

GetFileError::~GetFileError() {
}

void GetFileError::handle(ClientSocket &client,Config &config, Request &request, Response &response, MiddlewareChain &next) {
	(void)client;
	(void)config;
	(void)request;
	HttpStatus _find_status;

	if(response.getStatus()/100 != 2)
	{
		usable<std::string> errorFile;
		
		errorFile = config.getPathErrorFile(response.getStatus());
		if(errorFile.state == true)
		{
			File fileError(WORKPATH + errorFile.value);
			if (fileError.isFile())
			{
				response.setDir(false);
				response.setBodyPath(fileError.getPath());

				std::string content_type = fileError.find_content_type();
				if(!content_type.empty())
					response.setHeaders("Content-Type", fileError.find_content_type());

				response.setHeaders("Content-Length", fileError.fileLengthStr());
				
				response.setHeaders("Last-Modified", fileError.fileLastModified());

			}
		}
		else
		{
		 std::string body=
				"<!DOCTYPE html>\n"
				"<html>\n"
				"<head>\n"
				"	<title>"+ response.int_to_str(response.getStatus())  +"</title>\n"
				"	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
				"	<style>\n"
				"		body {background-color:#1c7d76;left: 0;margin-top: -160px;position: absolute;text-align: center;top: 50%;width: 100%}\n"
				"		h1{font-family:Arial, serif;color:#000000;font-size:80px}\n"
				"		p {font-family:Arial, serif;font-size:20px;font-style:normal;font-weight:normal;color:#000000;}\n"
				"	</style>\n"
				"	</head>\n"
				"</head>\n"
				"<body>\n"
				"	<h1>" + response.int_to_str(response.getStatus())  + "</h1>\n"
				"	<p>" + _find_status.find_message(response.getStatus()) + "</p>\n"
				"</body>\n"
				"</html>\n";
			
			File convert;
			response.setDir(true);
			response.setBody(body);

			response.setHeaders("Content-Type", "text/html");

			response.setHeaders("Content-Length", convert.int_to_str(body.size()));
		}
	}



	next();
}