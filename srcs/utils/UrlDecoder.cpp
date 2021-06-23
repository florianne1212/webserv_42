#include <iostream>
#include <string>
#include <cctype>

/*
** ------------------ UrlDecoder --------------------------------------
**
** fonction qui transforme un string URL encoded en string URL decoded
** les signes + sont remplaces par des espaces
** les caracteres speciaux codes en % hex hex sont remplaces par leur valeur
*/
void UrlDecoder(std::string & str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '+')
			str[i] = ' ';
		if (str[i] == '%' && (i < (str.length() - 2)))
		{
			if (isxdigit(str[i + 1]) && (isxdigit(str[i + 2])))
			{
				str[i] = static_cast<char>(strtol((str.substr(i + 1, 2)).c_str(), 0, 16));
				str = str.erase(i + 1, 2);
			}
		}
	}
}


// MAIN DE TEST
// int main(int argc, char* argv[])
// {
// 	if (argc !=2)
// 		return (0);
// 	std::string str = argv[1];
// 	std::cout << str << std::endl;

// 	UrlDecoder(str);
// 	std::cout << str << std::endl;

// 	return (0);
// }