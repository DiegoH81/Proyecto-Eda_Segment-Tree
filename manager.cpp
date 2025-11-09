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

void manager::insert()
{
	if (!reader.is_empty())
	{
		auto trending_topics = reader.get_current_trending_topic(k_topics);

		vector<pair<std::string, size_t>> my_vector;
		my_vector.reserve(trending_topics.size());

		for (auto& tpc : trending_topics)
			my_vector.push_back(pair(tpc.first, tpc.second));

		tree.insert(my_vector);
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

void manager::menu(std::string time_manual, std::string time_batch, std::string time_complete)
{
	while (true)
	{
		std::cout << "\n=== MENU PRINCIPAL ===\n";
		std::cout << "1. Insercion manual (una por una)\n";
		std::cout << "2. Insercion por lotes (10% de archivos)\n";
		std::cout << "3. Ejecucion completa\n";
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

	auto start_insert = std::chrono::high_resolution_clock::now();
	insert();
	auto end_insert = std::chrono::high_resolution_clock::now();

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
	
	auto duration_insert = std::chrono::duration_cast<std::chrono::milliseconds>(end_insert - start_insert).count();
	std::cout << "Tiempo de insert(): " << duration_insert << " ms\n";
	auto duration_query = std::chrono::duration_cast<std::chrono::milliseconds>(end_query - start_query).count();
	std::cout << "Tiempo de query(): " << duration_query << " ms\n";

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

	double total_insert_time = 0;
	double total_query_time = 0;

	std::cout << "Procesando: " << limit << " topicos\n";

	// Process batch
	for (size_t i = 0; i < limit && !reader.is_empty(); ++i)
	{
		auto start_insert = std::chrono::high_resolution_clock::now();
		insert();
		auto end_insert = std::chrono::high_resolution_clock::now();
		total_insert_time += std::chrono::duration_cast<std::chrono::milliseconds>(end_insert - start_insert).count();



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
			total_query_time += std::chrono::duration_cast<std::chrono::milliseconds>(end_query - start_query).count();
			export_query(q);
			open_python(time);
		}
	}

	size_t query_count = limit / step;

	double avg_insert = double(total_insert_time) / limit;
	double avg_query = double(total_query_time) / query_count;

	std::cout << "Tiempo promedio de insert(): " << avg_insert << " ms\n";
	std::cout << "Tiempo promedio de query(): " << avg_query << " ms\n";
}

void manager::complete_mode(std::string time)
{
	if (reader.is_empty())
	{
		std::cout << "No hay suficientes topicos para ingresar.\n";
		return;
	}

	double total_insert_time = 0;
	

	size_t max_size = size();

	while (!reader.is_empty())
	{
		auto start_insert = std::chrono::high_resolution_clock::now();
		insert();
		auto end_insert = std::chrono::high_resolution_clock::now();
		total_insert_time += std::chrono::duration_cast<std::chrono::milliseconds>(end_insert - start_insert).count();
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

	auto total_query_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_query - start_query).count();

	double avg_insert_time = double(total_insert_time) / max_size;
	std::cout << "Tiempo total de insert(): " << std::fixed << total_insert_time << " ms\n";
	std::cout << "Tiempo promedio por insert(): " << std::fixed << avg_insert_time << " ms\n";
	std::cout << "Tiempo de query(): " << std::fixed << total_query_time << " us\n";
	export_query(q);
	open_python(time);
}