#include "data_reader.h"

// Constructor
data_reader::data_reader(std::string s_w_path): porter()
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
	{
		//std::cout << entry.path().string() << "\n";
		files.push(entry.path().string());
	}
	//std::cout << "s: " << files.size() << "\n";
}

std::vector<std::pair<std::string, size_t>> data_reader::get_current_trending_topic(size_t& limit)
{
	std::unordered_map <std::string, size_t> helper;
	std::vector<std::pair<std::string, size_t>> trending_topics;


	if (!is_empty())
	{
		std::string file_path = files.front();
		files.pop();

		//std::cout << "FILE PATH: " << file_path << "\n";

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
				{
					porter.porter_stem(word);
					helper[word]++;
				}
			}
		}

		file.close();
	}

	
	// Get maximum freq
	size_t max_count = 0;
	for (auto& it : helper)
		if (it.second > max_count)
			max_count = it.second;

	// Those words that have the same freq
	for (auto& it : helper)
	{
		if (it.second == max_count)
		{
			trending_topics.push_back({ it.first, it.second });
			if (trending_topics.size() >= limit)
				break;
		}
	}

	return trending_topics;
}

void data_reader::to_lower_str(std::string& in_string)
{
	for (char& c : in_string)
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

void data_reader::remove_puctuation(std::string& in_string)
{
	for (char& c : in_string)
		if (std::ispunct(static_cast<unsigned char>(c)))
			c = ' ';
}


bool data_reader::is_empty() { return files.empty(); }

size_t data_reader::size() { return files.size(); }

