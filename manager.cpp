#include "manager.h"

// Constructor
manager::manager(size_t k_topics, std::string stop_words_path):
	tree(k_topics), reader(stop_words_path)
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
		std::string trending_topic = reader.get_current_trending_topic();
		tree.insert(trending_topic);
	}
}

std::vector<std::pair<std::string, size_t>> manager::query(size_t start, size_t end)
{
	return tree.query(start, end);
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