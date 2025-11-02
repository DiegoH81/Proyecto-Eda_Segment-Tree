#include <iostream>
#include <chrono>
#include "manager.h"

#include "porter_algorithm.h"


int main()
{
	std::string stop_words_path = "stopwords.txt";
	std::string files_path = "C:\\Users\\diego\\OneDrive\\Escritorio\\EDA\\archivos_txt\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";

	manager managersin(5, stop_words_path);
	managersin.load_files(files_path);


	int input{ -1 };
	std::cout << "DATOS CARGADOS\n";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (!managersin.is_empty())
		managersin.insert();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	//managersin.print_tree();
	std::cout << "\n\n\n";
	managersin.print_tree_root();

	size_t start_mark = 1;
	size_t end_mark = 120;
	size_t topic_size = 10;
	auto query = managersin.query(start_mark, end_mark, topic_size);

	std::cout << "\n\nQuery: [ " << start_mark << " - " << end_mark << " ]\n";
	std::cout << "\n\nEncontro: " << query.size() << "\n";

	for (auto& topic : query)
		std::cout << topic.first << " - " << topic.second << "\n";
		
	std::cout << "\nTiempo que demoro en insertar: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	/*
	porter_algorithm port;
	port.debug("us");
	*/

}