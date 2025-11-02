#ifndef MANAGER_H
#define MANAGER_H

#include "segment_tree.h"
#include "data_reader.h"

class manager
{
public:
	// Constructor
	manager(size_t k_topics, std::string stop_words_path);

	// Metodos
	void load_files(std::string folder_path);
	void insert();
	void print_tree();
	void print_tree_root();

	std::vector<std::pair<std::string, size_t>> query(size_t start, size_t end, size_t in_k);
	size_t get_time();
	bool is_empty();

private:
	data_reader reader;
	segment_tree tree;
	size_t k_topics;
};

#endif