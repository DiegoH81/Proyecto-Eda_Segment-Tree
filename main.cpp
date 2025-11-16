#include <iostream>
#include <chrono>
#include <thread>

#include "manager.h"
#include "porter_algorithm.h"
#include "funciones.h"
#include "vector.h"
#include "unordered_map.h"

int main()
{
	std::string files_path = "D:\\Archivos_EDA\\AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull\\";
	
	std::string time_manual = "-1",
				time_batch = "-1",
				time_complete = "-1",
				time_query = "-1";

	manager managersin;
	
	managersin.load_files(files_path);
	managersin.configure();
	managersin.menu(time_manual, time_batch, time_complete, time_query);
	

	

	/*
	size_t k_topics = 15;
	segment_tree test(k_topics, 8192);


	data_reader prueba_reader("stopwords.txt");
	prueba_reader.load_files(files_path);
	int init_size = prueba_reader.size();
	double time = 0;
	double prepo_time = 0;
	double build_vec_time = 0; // convertir read_vec -> my_vec (ms)
	vector<double> my_vec_times;
	my_vec_times.push_back(0);
	my_vec_times.push_back(0);

	std::cout << "EMPEZO\n";
	auto global_start = std::chrono::high_resolution_clock::now();
	int current_file_index = 0;

	vector<std::string> temp_insert;
	temp_insert.push_back("banana");

	while (!prueba_reader.is_empty())
	{
		//std::cout << "Procesando archivo: " << current_file_index++ << "\n";
		auto start_prepo = std::chrono::high_resolution_clock::now();
		std::vector<std::pair<std::string, size_t>> read_vec = prueba_reader.get_current_trending_topic(k_topics);
		auto end_prepo = std::chrono::high_resolution_clock::now();

		auto dur_prepo = std::chrono::duration_cast<std::chrono::milliseconds>(end_prepo - start_prepo).count();
		prepo_time += dur_prepo;


		// ---- BUILD VEC (read_vec -> my_vec) ----
		auto start_build = std::chrono::high_resolution_clock::now();
		vector<pair<std::string, size_t>> my_vec;
		my_vec.reserve(read_vec.size());

		for (auto& item : read_vec)
			my_vec.push_back({ item.first, item.second });

		auto end_build = std::chrono::high_resolution_clock::now();
		auto dur_build = std::chrono::duration_cast<std::chrono::milliseconds>(end_build - start_build).count();
		build_vec_time += dur_build;

		auto start_insert = std::chrono::high_resolution_clock::now();

		// insert
		test.insert(my_vec, my_vec_times);

		auto end_insert = std::chrono::high_resolution_clock::now();

		auto duration_insert = std::chrono::duration_cast<std::chrono::nanoseconds>(end_insert - start_insert).count();

		time += duration_insert;
	}

	auto global_end = std::chrono::high_resolution_clock::now();
	double global_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(global_end - global_start).count();


	std::cout << "TOPICOS INSERTADOS : " << init_size << "\n";
	std::cout << "TIEMPO TOTAl (s): " << std::fixed << time / 1000000000 << "\n";
	std::cout << "Promedio (ns): " << std::fixed << time / init_size << "\n";

	std::cout << "TIEMPO PREPROCESADO (ms): " << std::fixed << prepo_time << "\n";
	std::cout << "Promedio PREPROCESADO (ms): " << std::fixed << prepo_time / init_size << "\n";

	std::cout << "Tiempo grow promedio (ns): " << std::fixed << my_vec_times[0] / init_size << "\n";
	std::cout << "Tiempo find_pos promedio (ns): " << std::fixed << my_vec_times[1] / init_size << "\n";

	std::cout << "TIEMPO GLOBAL (s): " << global_seconds << "\n";


	auto query_start = std::chrono::high_resolution_clock::now();
	auto query = test.query(10, 100000, 40);
	auto query_end = std::chrono::high_resolution_clock::now();
	double query_time = std::chrono::duration_cast<std::chrono::nanoseconds>(query_end - query_start).count();
	std::cout << "QUERY TIME (ns): " << std::fixed << query_time << "\n";
	std::cout << "ROOT\n";
	test.print_root();
	
	*/

    return 0;
}