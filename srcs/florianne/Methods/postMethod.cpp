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
	std::cout << "POST\n";

	File filePost(WORKPATH + request.getUrl());

	if (filePost.isPresent()) {
		std::cout << "ispresent\n";
		if(filePost.isFile())
		{
			filePost.fileAppend(request.getUrl(), request.getBody());
			response.setStatus(200);
		}
		//o create o append	
	}
	else {
		std::cout << "create\n";
		filePost.fileCreate(WORKPATH + request.getUrl());
	}
		
}