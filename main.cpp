#include <iostream>
#include <chrono>
#include <thread>

#include "manager.h"


#include "porter_algorithm.h"


int main()
{
	std::string stop_words_path = "stopwords.txt";
	manager managersin(8, stop_words_path);

	std::string files_path = "C:\\Users\\diego\\OneDrive\\Escritorio\\EDA\\archivos_txt\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";
	managersin.open_python("C:\\Users\\diego\\source\\repos\\Proyecto-Eda_Segment-Tree\\Proyecto-Eda_Segment-Tree\\graficador.py");
	
	managersin.load_files(files_path);

	std::cout << "DATOS CARGADOS\n";

	double duracion = 0;
	int init_size = managersin.size();
	while (!managersin.is_empty())
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		managersin.insert();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		duracion+= std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();


		size_t last_k = 8;

		size_t end_mark = managersin.get_time();
		size_t start_mark = 0;

		if (int(end_mark) - int(last_k) < 0)
			start_mark = 0;
		else
			start_mark = end_mark - last_k;

		size_t topic_size = 10;

		auto query = managersin.query(start_mark, end_mark, topic_size);
		
		std::cout << "\n\nQuery: [ " << start_mark << " - " << end_mark << " ]\n";
		std::cout << "\n\nEncontro: " << query.size() << "\n";

		for (auto& topic : query)
			std::cout << topic.first << " - " << topic.second << "\n";
		
		std::cout << "Exportando query\n";

		managersin.export_query(query);

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	std::cout << "\nTiempo promedio en insertar: " << duracion / double(init_size) << " ms\n";
	
	//managersin.print_tree();
	//std::cout << "\n\n\n";
	//managersin.print_tree_root();



	
	/*
	porter_algorithm port;
	port.debug("us");
	*/

}