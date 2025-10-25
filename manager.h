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

	bool is_empty();

private:
	data_reader reader;
	segment_tree tree;
};

#endif