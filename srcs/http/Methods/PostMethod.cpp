#include "PostMethod.hpp"

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

	if(config.getUploadDir(client.getServerName()).state == true)
        url = WORKPATH + config.getUploadDir(client.getServerName()).value + request.getUrl();
    else
        url = WORKPATH + request.getUrl();
    File filePost(url);

	if (filePost.isPresent()) {
        if(filePost.isFile())
            response.setAppend(url, request.getBody());
        else
        {
			filePost.fileCreate(url);
            if(filePost.isFile())
                response.setAppend(url + "/" , request.getBody());
        }
    }
    else {
        if(filePost.fileCreate(url) == false)
            response.setStatus(422);
        if(filePost.isFile())
			response.setAppend(url, request.getBody());
        response.setStatus(201);
    }
}