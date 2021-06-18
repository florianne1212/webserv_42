#include "GestionCGI.hpp"

GestionCGI::GestionCGI(void){}

GestionCGI::~GestionCGI(){}

// GestionCGI::GestionCGI(const GestionCGI & other){

// }

// GestionCGI & GestionCGI::operator=(const GestionCGI & other){
// 	if (this != &other)
// 	{

// 	}
// 	return (*this);
// }

void GestionCGI::environCGICreation(void){
	///creation de la liste de variable
	// int n = _envCGIPrepare.size();
	// std::vector<const char*> tmp(_envCGIPrepare.size() + 1, NULL);

	// for (std::list<std::string>::iterator it = _envCGIPrepare.begin(); it != _envCGIPrepare.end(); ++it)
	// 	tmp.push_back((*it).c_str());

	// execve(tmp[0], &tmp[0], &tmp[0]);


// std::vector<std::string> vector = {"a", "std::vector", "of", "std::string"};
// // Result char**.
// char** result = new char*[vector.size()];
// for (int index = 0; index < vector.size(); index++) {
//     result[index] = const_cast<char*>(vector[index].c_str());
// }
// // Use the result.
// delete[] result;
// // Deallocate the memory from heap after usage.


std::vector<char *> cStrArray;
cStrArray.reserve(_envCGIPrepare.size() + 1);
for(int index = 0; index < _envCGIPrepare.size(); ++index)
{
  cStrArray.push_back(_envCGIPrepare[index].c_str());
}
cStrArray[_envCGIPrepare.size()] = NULL;

//NO RESIZING OF origVector!!!!
execve(cStrArray[0], &cStrArray[0], &cStrArray[0]);

// SomeCFunction(&cStrArray[0], cStrArray.size());

}


////////////////////////////////////////////////////
////////private fonctions///////////////////////////
////////////////////////////////////////////////////


std::string GestionCGI::convertToUpperCase(std::string const & toChange){
	std::string ret;
	for (std::string::const_iterator it = toChange.begin(); it != toChange.end(); it++)
		ret += std::toupper(*it);
	return ret;
}