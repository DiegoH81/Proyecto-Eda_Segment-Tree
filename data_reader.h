#ifndef DATA_READER_H
#define DATA_READER_H

#include <string>
#include <sstream>
#include <queue>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <stdexcept>

class data_reader
{
public:
	// Constructor
	data_reader(std::string s_w_path);

	// Metodos
	void load_files(std::string folder_path);
	std::string get_current_trending_topic();
	bool is_empty();

private:
	std::unordered_set<std::string> stop_words;
	std::queue<std::string> files;

	// Metodos
	void load_stop_words(std::string file_path);
	void to_lower_str(std::string& in_string);
	void remove_puctuation(std::string& in_string);
};

#endif