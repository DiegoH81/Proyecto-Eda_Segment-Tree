#include <iostream>
#include "manager.h"

int main()
{

	std::string stop_words_path = "stopwords.txt";
	std::string files_path = "C:\\Users\\diego\\OneDrive\\Escritorio\\EDA\\archivos_txt\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";

	manager managersin(5, stop_words_path);
	managersin.load_files(files_path);


	int input{ -1 };
	std::cout << "DATOS CARGADOS\n";
	while (!managersin.is_empty())
		managersin.insert();
	
	managersin.print_tree();
	std::cout << "\n\n\n";
	managersin.print_tree_root();

}