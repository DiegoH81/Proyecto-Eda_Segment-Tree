#include "manager.h"

// Constructor
manager::manager(size_t k_topics, std::string stop_words_path):
	tree(k_topics), reader(stop_words_path), k_topics(k_topics)
{ }

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
		tree.insert(trending_topics);
	}
}

std::vector<std::pair<std::string, size_t>> manager::query(size_t start, size_t end, size_t query_size)
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

void manager::export_query(std::vector<std::pair<std::string, size_t>>& query)
{
	std::ofstream file("query.txt", std::ios::out);

	if (!file.is_open())
		throw std::runtime_error("No se pudo abrir el archivo: query.txt");

	for (auto& topic : query)
		file << topic.first << " " << topic.second << "\n";

	file.close();
}

void manager::open_python(std::string path)
{
	std::string command = "start /B python \"" + path + "\"";

	system(command.c_str());
}
size_t manager::size()
{
	return reader.size();
}