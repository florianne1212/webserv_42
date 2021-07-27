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

	File filePost(WORKPATH + request.getUrl());

	if (filePost.isPresent()) {
		if(filePost.isFile())
		{
			filePost.fileAppend(request.getUrl(), request.getBody());
			response.setStatus(200);
		}
		//o create o append	
	}
	else {
		filePost.fileCreate(WORKPATH + request.getUrl());
	}
		
}