#include "deleteMethod.hpp"

DeleteMethod::DeleteMethod()
{
}

DeleteMethod::DeleteMethod(DeleteMethod const & copy)
{
	(void)copy;
}

DeleteMethod::~DeleteMethod()
{
}

DeleteMethod& DeleteMethod::operator=(DeleteMethod const & ope)
{
	(void)ope;
	return(*this);
}


void DeleteMethod::handleDelete(Client &client, Request &request, Response &response)
{
    (void)client;
	std::cout << "DELETE\n";

	File fileDelete(request.getUrl());
	

	if (fileDelete.isPresent()) {
		if(fileDelete.fileDelete())
			response.setStatus(200);
		else
			response.setStatus(204);
	}
	else
	{
		response.setStatus(204);
	}
}