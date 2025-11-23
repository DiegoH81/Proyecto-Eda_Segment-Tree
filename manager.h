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

	// Methods
	void load_files(std::string folder_path);
	void insert(double& process_time, double& insert_time);
	void print_tree();
	void print_tree_root();
	void export_query(vector<pair<std::string, size_t>>& query);
	void open_python(std::string seconds);
	vector<pair<std::string, size_t>> query(size_t start, size_t end, size_t in_k);
	size_t get_time();
	bool is_empty();

	// Menu
	void menu(std::string time_manual, std::string time_batch, std::string time_complete, std::string time_manual_query, std::string time_q);
	void configure(const size_t& in_update = 8192, const float& factor = 4);
	void show_cover();
private:
	data_reader reader;
	segment_tree tree;
	size_t k_topics, size;
	std::string path_script, path_txt;
	
	// Menu
	void manual_mode(std::string time);
	void batch_mode(std::string time);
	void complete_mode(std::string time);
	void query_mode(std::string time);
	void manual_mode_query(std::string time);

	// Methods
	void print_insert_process(double& insert_time, double& process_time);
	void print_query(const size_t& counter, double& query_time);
	void print_totals(const double& insert_time, const double& process_time, const double& query, const size_t& i_count, const size_t& p_count, const size_t& q_count);

};

#endif