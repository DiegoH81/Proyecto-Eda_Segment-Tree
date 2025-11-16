#include "manager.h"

// Constructor
manager::manager(size_t k_topics, std::string stop_words_path, std::string path_script, std::string path_txt):
	tree(k_topics), k_topics(k_topics), reader(stop_words_path), path_script(path_script), path_txt(path_txt)
{ }

manager::manager(std::string stop_words_path, std::string path_script, std::string path_txt) :
	tree(), k_topics(0), reader(stop_words_path), path_script(path_script), path_txt(path_txt)
{}
// Funciones

void manager::load_files(std::string folder_path)
{
	reader.load_files(folder_path);
}

void manager::insert(double &process_time, double &insert_time)
{
	if (!reader.is_empty())
	{
		// Pre-process time (porter, tokenize, etc)
		auto start_pre = std::chrono::high_resolution_clock::now();
		auto trending_topics = reader.get_current_trending_topic(k_topics);
		auto end_pre = std::chrono::high_resolution_clock::now();

		process_time += std::chrono::duration_cast<std::chrono::milliseconds>(end_pre - start_pre).count();


		vector<pair<std::string, size_t>> my_vector;
		my_vector.reserve(trending_topics.size());

		for (auto& tpc : trending_topics)
			my_vector.push_back(pair(tpc.first, tpc.second));

		// Insert time
		auto start_insert = std::chrono::high_resolution_clock::now();
		tree.insert(my_vector);
		auto end_insert = std::chrono::high_resolution_clock::now();

		insert_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_insert - start_insert).count();
	}
}

vector<pair<std::string, size_t>> manager::query(size_t start, size_t end, size_t query_size)
{
	return tree.query(start, end, query_size);
}

size_t manager::get_time()
{
	return tree.get_time();
}

void manager::print_tree()
{
	tree.print();
}

void manager::print_tree_root()
{
	tree.print_root();
}
bool manager::is_empty()
{
	return reader.is_empty();
}

void manager::export_query(vector<pair<std::string, size_t>>& query)
{
	std::ofstream file(path_txt, std::ios::out);

	if (!file.is_open())
		throw std::runtime_error("No se pudo abrir el archivo: " + path_txt);

	for (auto& topic : query)
		file << topic.first << " " << topic.second << "\n";

	file.close();
}

void manager::open_python(std::string seconds)
{
	std::string command = "python \"" + path_script + "\" \"" + path_txt + "\" " + seconds;

	system(command.c_str());
}

size_t manager::size()
{
	return reader.size();
}


// Menu
void manager::configure()
{
	std::cout << "\n=== CONFIGURACION INICIAL ===\n";
	std::cout << "Numero de topicos (K): ";
	std::cin >> k_topics;

	tree = segment_tree(k_topics);
}

void manager::menu(std::string time_manual, std::string time_batch, std::string time_complete, std::string time_q)
{
	while (true)
	{
		std::cout << "\n=== MENU PRINCIPAL ===\n";
		std::cout << "1. Insercion manual (una por una)\n";
		std::cout << "2. Insercion por lotes (10% de archivos)\n";
		std::cout << "3. Ejecucion completa\n";
		std::cout << "4. Ejecutar query\n";
		std::cout << "0. Salir\n";
		std::cout << "Opcion: ";

		int op; std::cin >> op;
		
		switch (op)
		{
		case 1:
			manual_mode(time_manual);
			break;
		case 2:
			batch_mode(time_batch);
			break;
		case 3:
			complete_mode(time_complete);
			break;
		case 4:
			query_mode(time_q);
			break;
		case 0:
			return;
		default:
			std::cout << "Opcion inválida.\n";
			break;
		}

	}
}

void manager::manual_mode(std::string time)
{
	if (reader.is_empty())
	{
		std::cout << "No hay suficientes topicos para ingresar.\n";
		return;
	}

	int subop;
	std::cout << "1. Mostrar ultimos K topicos\n";
	std::cout << "2. Mostrar rango personalizado\n";
	std::cout << "Opción: \n";
	std::cin >> subop;
	
	double process_time = 0, insert_time = 0;
	insert(process_time, insert_time);


	size_t K;
	std::cout << "Ingrese K (numero de topicos): ";
	std::cin >> K;

	size_t end_mark = 0, start_mark = 0;

	if (subop == 1)
	{
		size_t N;
		std::cout << "Ultimas N inserciones: ";
		std::cin >> N;
		end_mark = get_time();

		if (int(end_mark) - int(N) < 0)
			start_mark = 0;
		else
			start_mark = end_mark - N;
	}
	else if (subop == 2)
	{
		std::cout << "Inicio del rango: ";
		std::cin >> start_mark;
		std::cout << "Fin del rango: ";
		std::cin >> end_mark;

	}
	auto start_query = std::chrono::high_resolution_clock::now();
	auto q = query(start_mark, end_mark, K);
	auto end_query = std::chrono::high_resolution_clock::now();
	
	auto duration_query = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();
	
	std::cout << "Tiempo de insert(): " << insert_time << " ms\n";
	std::cout << "Tiempo de pre-procesado(): " << process_time << " ns\n";
	std::cout << "Tiempo de query(): " << duration_query << " ns\n";

	export_query(q);
	open_python(time);
}

void manager::batch_mode(std::string time)
{
	if (reader.is_empty())
	{
		std::cout << "No hay suficientes topicos para ingresar.\n";
		return;
	}

	int subop;
	std::cout << "1. Mostrar ultimos K topicos\n";
	std::cout << "2. Mostrar rango personalizado\n";
	std::cout << "Opcion: \n";
	std::cin >> subop;

	size_t K;
	std::cout << "Ingrese K (numero de topicos): ";
	std::cin >> K;

	size_t end_mark = 0, start_mark = 0;

	size_t N;
	if (subop == 1)
	{
		std::cout << "Ultimas N inserciones: ";
		std::cin >> N;
	}
	else if (subop == 2)
	{
		std::cout << "Inicio del rango: ";
		std::cin >> start_mark;
		std::cout << "Fin del rango: ";
		std::cin >> end_mark;

	}

	size_t limit = size() / 10;
	size_t step = limit / 10;

	double query_time = 0;
	double process_time = 0;
	double insert_time = 0;
	std::cout << "Procesando: " << limit << " topicos\n";

	// Process batch
	for (size_t i = 0; i < limit && !reader.is_empty(); ++i)
	{
		insert(process_time, insert_time);

		if (i % step == 0)
		{
			if (subop == 1)
			{
				end_mark = get_time();
				if (int(end_mark) - int(N) < 0)
					start_mark = 0;
				else
					start_mark = end_mark - N;
			}
			auto start_query = std::chrono::high_resolution_clock::now();
			auto q = query(start_mark, end_mark, K);
			auto end_query = std::chrono::high_resolution_clock::now();
			query_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();
			export_query(q);
			open_python(time);
		}
	}

	size_t query_count = limit / step;

	
	std::cout << "\n-------------------------------------------------\n";
	std::cout << "Tiempo total de insert(): " << insert_time / 1000000000 << " s\n";
	std::cout << "Tiempo promedio de insert(): " << insert_time / limit<< " ns\n";
	std::cout << "-------------------------------------------------\n";
	std::cout << "Tiempo total de pre-procesado(): " << process_time / 1000 << " s\n";
	std::cout << "Tiempo promedio de pre-procesado(): " << process_time / limit << " ms\n";
	std::cout << "-------------------------------------------------\n";
	std::cout << "Tiempo total de query(): " << query_time / 1000000 << " ms\n";
	std::cout << "Tiempo promedio de query(): " << query_time / query_count << " ns\n";
	std::cout << "-------------------------------------------------\n\n";
}

void manager::complete_mode(std::string time)
{
	if (reader.is_empty())
	{
		std::cout << "No hay suficientes topicos para ingresar.\n";
		return;
	}

	double total_insert_time = 0;
	
	int temp_size = size();
	/*
	std::cout << "size: " << temp_size << "\n";

	std::cout << "PERC SIZE: " << perc << "\n";
	*/
	int perc = temp_size / 1000;


	size_t limit = size();

	int counter = 0;
	int perc_counter = 0;

	double query_time = 0;
	double process_time = 0;
	double insert_time = 0;

	while (!reader.is_empty())
	{
		/*
		if (counter % perc == 0)
		{
			//std::cout << perc_counter << "\n";
			perc_counter++;
		}
		*/

		insert(process_time, insert_time);

		counter++;
	}

	int subop;
	std::cout << "1. Mostrar ultimos K topicos\n";
	std::cout << "2. Mostrar rango personalizado\n";
	std::cout << "Opcion: \n";
	std::cin >> subop;

	size_t K;
	std::cout << "Ingrese K (numero de topicos): ";
	std::cin >> K;

	size_t end_mark = 0, start_mark = 0;


	if (subop == 1)
	{
		size_t N;
		std::cout << "Ultimas N inserciones: ";
		std::cin >> N;
		end_mark = get_time();

		if (int(end_mark) - int(N) < 0)
			start_mark = 0;
		else
			start_mark = end_mark - N;
	}
	else if (subop == 2)
	{
		std::cout << "Inicio del rango: ";
		std::cin >> start_mark;
		std::cout << "Fin del rango: ";
		std::cin >> end_mark;

	}


	auto start_query = std::chrono::high_resolution_clock::now();
	auto q = query(start_mark, end_mark, K);
	auto end_query = std::chrono::high_resolution_clock::now();

	query_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();


	std::cout << "\n-------------------------------------------------\n";
	std::cout << "Tiempo total de insert(): " << insert_time / 1000000000 << " s\n";
	std::cout << "Tiempo promedio de insert(): " << insert_time / limit << " ns\n";
	std::cout << "-------------------------------------------------\n";
	std::cout << "Tiempo total de pre-procesado(): " << process_time / 1000 << " s\n";
	std::cout << "Tiempo promedio de pre-procesado(): " << process_time / limit << " ms\n";
	std::cout << "-------------------------------------------------\n";
	std::cout << "Tiempo total de query(): " << query_time << " ns\n";
	std::cout << "-------------------------------------------------\n\n";
	export_query(q);
	open_python(time);
}

void manager::query_mode(std::string time)
{
	if (tree.is_empty())
	{
		std::cout << "El arbol esta vacio, no se pueden hacer queries.\n";
		return;
	}
	size_t K;
	size_t start_mark, end_mark;

	std::cout << "Inicio del rango: ";
	std::cin >> start_mark;
	std::cout << "Fin del rango: ";
	std::cin >> end_mark;
	std::cout << "Cantidad K de topicos: ";
	std::cin >> K;

	auto start_query = std::chrono::high_resolution_clock::now();
	auto q = query(start_mark, end_mark, K);
	auto end_query = std::chrono::high_resolution_clock::now();

	auto query_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();

	std::cout << "Tiempo de query(): " << query_time << " ns\n";

	export_query(q);
	open_python(time);
}
