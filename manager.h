#ifndef MANAGER_H
#define MANAGER_H

#include <fstream>
#include <cstdlib>
#include <chrono>

#include "pair.h"
#include "vector.h"
#include "segment_tree.h"
#include "data_reader.h"

class manager
{
public:
	// Constructor
	manager(size_t k_topics, std::string stop_words_path, std::string path_script, std::string path_txt);
	manager(std::string stop_words_path = "stopwords.txt", std::string path_script = "graficador.py", std::string path_txt = "query.txt");

	// Metodos
	void load_files(std::string folder_path);
	void insert();
	void print_tree();
	void print_tree_root();
	void export_query(vector<pair<std::string, size_t>>& query);
	void open_python(std::string seconds);

	// Menu
	void menu(std::string time_manual, std::string time_batch, std::string time_complete);
	void configure();

	vector<pair<std::string, size_t>> query(size_t start, size_t end, size_t in_k);
	size_t get_time();
	bool is_empty();
	size_t size();

private:
	data_reader reader;
	segment_tree tree;
	size_t k_topics;
	std::string path_script, path_txt;
	
	// Menu
	void manual_mode(std::string time);
	void batch_mode(std::string time);
	void complete_mode(std::string time);
};

#endif