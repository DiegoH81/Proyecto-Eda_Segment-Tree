#ifndef DATA_READER_H
#define DATA_READER_H

#include <string>
#include <queue>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <stdexcept>

#include "porter_algorithm.h"

class data_reader
{
public:
	// Constructor
	data_reader(std::string s_w_path);

	// Methods
	void load_files(std::string folder_path);
	std::vector<std::pair<std::string, size_t>> get_current_trending_topic(size_t& limit);
	bool is_empty();
	size_t size();
private:
	std::unordered_set<std::string> stop_words;
	std::queue<std::string> files;
	porter_algorithm porter;
	size_t n_files;

	// Methods
	void load_stop_words(std::string file_path);
	void process_word(std::string& in_string);
	void tokenize(std::string& in_string, std::vector<std::string>& tokens);
};

#endif