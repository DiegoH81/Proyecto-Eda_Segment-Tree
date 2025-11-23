#include "manager.h"

// Constructor
manager::manager(size_t k_topics, std::string stop_words_path, std::string path_script, std::string path_txt):
	tree(k_topics), k_topics(k_topics), reader(stop_words_path), path_script(path_script), path_txt(path_txt), size(0)
{ }

manager::manager(std::string stop_words_path, std::string path_script, std::string path_txt) :
	tree(), k_topics(0), reader(stop_words_path), path_script(path_script), path_txt(path_txt), size(0)
{}

// Methods
void manager::load_files(std::string folder_path)
{
	reader.load_files(folder_path);
	size = reader.size();
}

void manager::insert(double &process_time, double &insert_time)
{
	if (!reader.is_empty())
	{
		// Pre-process time (porter, tokenize, etc)
		auto start_pre = std::chrono::high_resolution_clock::now();
		auto trending_topics = reader.get_current_trending_topic(k_topics);
		auto end_pre = std::chrono::high_resolution_clock::now();

		process_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_pre - start_pre).count();


		vector<pair<std::string, size_t>> my_vector;
		my_vector.reserve(trending_topics.size());

		for (auto& tpc : trending_topics)
			my_vector.push_back(pair(tpc.first, tpc.second));

		// Insert time
		auto start_insert = std::chrono::high_resolution_clock::now();
		tree.insert(my_vector);
		auto end_insert = std::chrono::high_resolution_clock::now();

		insert_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_insert - start_insert).count();
	}
}

vector<pair<std::string, size_t>> manager::query(size_t start, size_t end, size_t query_size)
{
	return tree.query(start, end, query_size);
}

size_t manager::get_time() { return tree.get_time(); }

void manager::print_tree() { tree.print(); }

void manager::print_tree_root() { tree.print_root(); }

bool manager::is_empty() { return reader.is_empty(); }

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

// Menu
void manager::configure(const size_t& in_update, const float& factor)
{
	std::cout << "\n=== CONFIGURACION INICIAL ===\n";
	std::cout << "Numero de topicos (K): ";
	std::cin >> k_topics;

	tree = segment_tree(k_topics, factor, in_update);
}

void manager::menu(std::string time_manual, std::string time_batch, std::string time_complete, std::string time_manual_query, std::string time_q)
{
	while (true)
	{
		std::cout << "\n=== MENU PRINCIPAL ===\n";
		std::cout << "1. Insertar y consultar 1 dato\n";
		std::cout << "2. Insercion 10%  y cada 1% una query\n";
		std::cout << "3. Ejecucion completa y query instantanea\n";
		std::cout << "4. Ejecucion completa y query al final\n";
		std::cout << "5. Ejecutar query\n";
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
			manual_mode_query(time_manual_query);
			break;
		case 4:
			complete_mode(time_complete);
			break;
		case 5:
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

void manager::show_cover()
{
	std::cout << "====================================================\n";
	std::cout << "                    SEGMENT_TREE                    \n";
	std::cout << "====================================================\n";
	std::cout << "Elaborado por: Diego Hidalgo\n";
	std::cout << "Curso: Estructuras de datos avanzada\n";
	std::cout << "Profesor: Erick Gomez Nieto\n\n";
}

void manager::manual_mode(std::string time)
{
	if (reader.is_empty())
	{
		std::cout << "No hay suficientes topicos para ingresar.\n";
		return;
	}

	int subop;
	std::cout << "1. Mostrar ultimas N inserciones\n";
	std::cout << "2. Mostrar rango personalizado\n";
	std::cout << "Opción: \n";
	std::cin >> subop;
	
	double process_time = 0, insert_time = 0;
	insert(process_time, insert_time);

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

	size_t K;
	std::cout << "Ingrese K (numero de topicos): ";
	std::cin >> K;

	auto start_query = std::chrono::high_resolution_clock::now();
	auto q = query(start_mark, end_mark, K);
	auto end_query = std::chrono::high_resolution_clock::now();
	
	double duration_query = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();
	
	print_insert_process(insert_time, process_time);
	print_query(1, duration_query);

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
	std::cout << "1. Mostrar ultimas N inserciones\n";
	std::cout << "2. Mostrar rango personalizado\n";
	std::cout << "Opcion: ";
	std::cin >> subop;


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

	size_t K;
	std::cout << "Ingrese K (numero de topicos): ";
	std::cin >> K;

	size_t limit = size / 10;
	size_t step = limit / 10;

	double query_time = 0;
	double process_time = 0;
	double insert_time = 0;

	// Total sum
	double total_process = 0;
	double total_insert = 0;

	std::cout << "\nProcesando: " << limit << " topicos\n";


	size_t counter = 0;

	// Process batch
	for (size_t i = 0; i < limit && !reader.is_empty(); i++)
	{
		insert(process_time, insert_time);

		total_process += process_time;
		total_insert += insert_time;


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
			double query_diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();
			query_time += query_diff;

			export_query(q);
			open_python(time);

			counter++;
			print_query(counter, query_diff);
		}

		print_insert_process(insert_time, process_time);
	}

	size_t query_count = limit / step;

	// Print total
	print_totals(total_insert, total_process, query_time, limit, limit, query_count);
}

void manager::complete_mode(std::string time)
{
	if (reader.is_empty())
	{
		std::cout << "No hay suficientes topicos para ingresar.\n";
		return;
	}

	double total_insert_time = 0;
	size_t limit = reader.size();

	int counter = 0;

	double query_time = 0;
	double process_time = 0;
	double insert_time = 0;
	
	// Total sum
	double total_process = 0;
	double total_insert = 0;

	while (!reader.is_empty())
	{
		insert(process_time, insert_time);

		total_process += process_time;
		total_insert += insert_time;

		counter++;

		print_insert_process(insert_time, process_time);
	}

	int subop;
	std::cout << "1. Mostrar ultimos N inserciones\n";
	std::cout << "2. Mostrar rango personalizado\n";
	std::cout << "Opcion: ";
	std::cin >> subop;


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

	size_t K;
	std::cout << "Ingrese K (numero de topicos): ";
	std::cin >> K;

	auto start_query = std::chrono::high_resolution_clock::now();
	auto q = query(start_mark, end_mark, K);
	auto end_query = std::chrono::high_resolution_clock::now();

	query_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();

	// Print total
	print_totals(total_insert, total_process, 0, limit, limit, 0);
	print_query(1 , query_time);

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
	int subop;
	std::cout << "1. Mostrar ultimos N inserciones\n";
	std::cout << "2. Mostrar rango personalizado\n";
	std::cout << "Opcion: ";
	std::cin >> subop;


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

	size_t K;
	std::cout << "Ingrese K (numero de topicos): ";
	std::cin >> K;

	auto start_query = std::chrono::high_resolution_clock::now();
	auto q = query(start_mark, end_mark, K);
	auto end_query = std::chrono::high_resolution_clock::now();

	double query_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();

	print_query(1, query_time);

	export_query(q);
	open_python(time);
}

void manager::manual_mode_query(std::string time)
{
	if (reader.is_empty())
	{
		std::cout << "No hay suficientes topicos para ingresar.\n";
		return;
	}

	size_t limit = reader.size();
	size_t step = limit / 10;

	double query_time = 0;
	double process_time = 0;
	double insert_time = 0;

	// Total sum
	double total_process = 0;
	double total_insert = 0;

	std::cout << "Procesando: " << limit << " topicos\n";


	size_t counter = 0;
	size_t end_mark = 0, start_mark = 0;
	size_t K = 0;

	int subop;
	size_t N;

	for (size_t i = 0; i < size && !reader.is_empty(); i++)
	{
		insert(process_time, insert_time);

		total_process += process_time;
		total_insert += insert_time;


		if (i % step == 0)
		{
			std::cout << "1. Mostrar ultimas N inserciones para el 10% siguiente\n";
			std::cout << "2. Mostrar rango personalizado para el 10% siguiente\n";
			std::cout << "Opcion: ";
			std::cin >> subop;

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

			std::cout << "Ingrese K (numero de topicos) para el 10% siguiente: ";
			std::cin >> K;
		}

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
		double query_diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end_query - start_query).count();
		query_time += query_diff;

		export_query(q);
		open_python(time);

		counter++;

		// Print total
		std::cout << "\n\n";
		print_insert_process(insert_time, process_time);
		print_query(counter, query_diff);
	}

	size_t query_count = limit / step;

	print_totals(total_insert, total_process, query_time, limit, limit, query_count);
}

// Methods
void manager::print_insert_process(double& insert_time, double& process_time)
{
	std::cout << "Insercion " << tree.get_size() << " -> INSERCION: " << insert_time << " ns - PRE-PROCESADO: " << process_time << " ns\n";
}

void manager::print_query(const size_t& counter, double& query_time)
{
	std::cout << "\n============== QUERY INFO ==============\n";
	std::cout << "Query " << counter << " : " << query_time << " ns \n";
	std::cout << "========================================\n\n";
}

void manager::print_totals(const double& insert_time, const double& process_time, const double& query,
						   const size_t& i_count, const size_t& p_count, const size_t& q_count)
{
	if (i_count)
	{
		std::cout << "\n================== ESTADISTICAS ==================\n";
		std::cout << "-------------------------------------------------\n";
		std::cout << "Tiempo total de insert(): " << std::fixed << insert_time << " ns\n";
		std::cout << "Tiempo promedio de insert(): " << std::fixed << insert_time / i_count << " ns\n";
		std::cout << "-------------------------------------------------\n";
	}
	if (p_count)
	{
		std::cout << "Tiempo total de pre-procesado(): " << std::fixed << process_time << " ns\n";
		std::cout << "Tiempo promedio de pre-procesado(): " << std::fixed << process_time / p_count << " ns\n";
		std::cout << "-------------------------------------------------\n";
	}
	if (q_count)
	{
		std::cout << "Tiempo total de query(): " << std::fixed << query << " ns\n";
		std::cout << "Tiempo promedio de query(): " << std::fixed << query / q_count << " ns\n";
		std::cout << "-------------------------------------------------\n\n";
	}
}