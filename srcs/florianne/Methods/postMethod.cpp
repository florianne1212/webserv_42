#include "postMethod.hpp"

PostMethod::PostMethod(/* args */)
{
}

PostMethod::PostMethod(PostMethod const & copy)
{
	(void)copy;
}

PostMethod::~PostMethod()
{
}

PostMethod& PostMethod::operator=(PostMethod const & ope)
{
	(void)ope;
	return(*this);
}

void PostMethod::handlePost(ClientSocket &client, Config &config,Request &request, Response &response)
{
	
    (void)client;
	(void)config;
	(void)response;

	std::string url;

	std::cout << "\nPOST POST\n";

	if(config.getUploadDir(client.getServerName()).state == true)
    {
        std::cout << "\nI AM HERE IN POST \n";
        url = WORKPATH + config.getUploadDir(client.getServerName()).value + request.getUrl();
    }
    else
        url = WORKPATH + request.getUrl();
    File filePost(url);

	if (filePost.isPresent()) {
        if(filePost.isFile())
        {
            std::cout << "APPEND present\n";
            response.setAppend(url, request.getBody());
        }
        else
        {
            std::cout << "CREATION IS Present\n";
			filePost.fileCreate(url);
            if(filePost.isFile())
                response.setAppend(url + "/" , request.getBody());
        }
        //o create o append
    }
    else {
        std::cout << "CREATION\n";
        filePost.fileCreate(url);
        if(filePost.isFile())
			response.setAppend(url, request.getBody());
    }
}