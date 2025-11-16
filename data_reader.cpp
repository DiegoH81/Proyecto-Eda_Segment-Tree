#include "data_reader.h"

// Constructor
data_reader::data_reader(std::string s_w_path): porter(), n_files(0)
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
	std::vector<std::string> temp;
	for (auto& entry : std::filesystem::directory_iterator(folder_path))
	{
		//std::cout << entry.path().string() << "\n";
		files.push(entry.path().string());
		temp.push_back(entry.path().string());
	}
	
	
	/*
	int counter = 0;
	int limit = 1000000;
	while (counter < limit)
	{
		for (int i = 0; i < temp.size() && counter < limit; i++)
		{
			files.push(temp[i]);
			counter++;
		}
	}
	*/
	

	n_files = files.size();
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
			std::vector<std::string> tokens;
			process_word(buffer);
			tokenize(buffer, tokens);

			std::string word;
			for (auto& word : tokens)
			{
				if (stop_words.find(word) == stop_words.end()) // Not a stopword
				{
					std::string prev = word;
					porter.porter_stem(word);
					helper[word]++;
				}
			}
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
				trending_topics.push_back({ it.first, 1 });
				if (trending_topics.size() >= limit)
					break;
			}
		}

		/*
		std::cout << file_path << ":\n";
		for (auto& topic : trending_topics)
			std::cout << topic.first << "\t";
		std::cout << "\n";
		*/

		file.close();
	}

	
	return trending_topics;
}




bool data_reader::is_empty() { return files.empty(); }

size_t data_reader::size() { return n_files; }

void data_reader::process_word(std::string& s)
{
	for (char& c : s)
	{
		unsigned char x = c;

		if (x >= 'A' && x <= 'Z')
		{
			c = x + 32;
			continue;
		}

		if ((x < 'a' || x > 'z') && (x < '0' || x > '9'))
			c = ' ';
	}
}

void data_reader::tokenize(std::string& in_string, std::vector<std::string>& tokens)
{
	tokens.clear();
	tokens.reserve(64);

	std::string cur;
	cur.reserve(32);

	for (char c : in_string)
	{
		if (c == ' ')
		{
			if (!cur.empty())
			{
				tokens.push_back(cur);
				cur.clear();
			}
		}
		else
			cur.push_back(c);
	}

	if (!cur.empty())
		tokens.push_back(cur);
}