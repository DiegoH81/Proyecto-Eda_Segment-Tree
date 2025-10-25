#include "data_reader.h"

// Constructor
data_reader::data_reader(std::string in_f_path, std::string s_w_path)
{
	load_stop_words(s_w_path);
}

// Funciones
void data_reader::load_stop_words(std::string file_path)
{
	std::ifstream file(file_path);

	if (!file.is_open())
		throw std::runtime_error("No se pudo abrir el archivo de stopwords: " + file_path);


	std::string buffer;
	while (std::getline(file, buffer))
		stop_words.insert(buffer);

	file.close();
}

void data_reader::load_files(std::string folder_path)
{
	for (auto& entry : std::filesystem::directory_iterator(folder_path))
		files.push(folder_path);
}

std::string data_reader::get_current_trending_topic()
{
	std::unordered_map <std::string, size_t> helper;
	std::string trending_topic;

	if (!is_empty())
	{
		std::string file_path = files.front();
		files.pop();

		std::ifstream file(file_path);

		if (!file.is_open())
			throw std::runtime_error("No se pudo abrir el archivo: " + file_path);


		std::string buffer;

		while (std::getline(file, buffer))
		{
			to_lower_str(buffer);
			remove_puctuation(buffer);

			std::stringstream ss(buffer);

			std::string word;
			while (ss >> word)
			{

				if (stop_words.find(word) == stop_words.end()) // Not a stopword
					helper[word]++;

			}
		}

		file.close();
	}

	size_t count = 0;

	for (auto& it : helper)
		if (it.second > count)
		{
			trending_topic = it.first;
			count = it.second;
		}

	return trending_topic;
}

void data_reader::to_lower_str(std::string& in_string)
{
	for (char& c : in_string)
		c = std::tolower(c);
}

void data_reader::remove_puctuation(std::string& in_string)
{
	for (char& c : in_string)
		if (std::ispunct(c))
			c = ' ';
}


bool data_reader::is_empty() { return files.empty(); }

