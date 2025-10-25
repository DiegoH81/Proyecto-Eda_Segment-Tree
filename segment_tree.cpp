#include "segment_tree.h"

// Constructores
segment_tree::segment_tree(size_t in_k_topics)
	: root(nullptr), k_topics(in_k_topics), size(0), time(1), word_to_id(), id_to_word()
{}


// Funciones
void segment_tree::insert(std::string topic)
{
	process_topic(topic);
	
	size_t id = word_to_id[topic];

	std::vector<node*> path;
	node** pos = find_pos(path);

	*pos = new node(time, time, { {id, 1} }, 0, k_topics);
	adjust_tree(path);	

	time++;
	size++;
}

node** segment_tree::find_pos(std::vector<node*>& path)
{
	node** ptr = &root;

	while (*ptr)
	{
		size_t max_limits = std::pow(2, (*ptr)->height);
		size_t cur_size = (*ptr)->end - (*ptr)->start + 1;
		
		if (cur_size >= max_limits)
			grow_node(ptr);

		(*ptr)->end = time; // Adjust limits
		path.push_back(*ptr);

		size_t mid = ((*ptr)->start + (*ptr)->end) / 2;

		if (time < mid)
			ptr = &((*ptr)->left);
		else
			ptr = &((*ptr)->right);
	}

	return ptr;
}

void segment_tree::adjust_tree(std::vector <node*>& path)
{
	while (!path.empty())
	{
		node* ptr = path.back();
		path.pop_back();

		ptr->update_height();
		ptr->merge();	
	}
}

void segment_tree::process_topic(std::string& in_topic)
{
	if (word_to_id.find(in_topic) == word_to_id.end()) // New word
	{
		word_to_id[in_topic] = id_to_word.size();
		id_to_word.push_back(in_topic);
	}
}

void segment_tree::grow_node(node** in_ptr)
{
	node* old_ptr = *in_ptr;
	*in_ptr = new node(old_ptr->start, time, {}, old_ptr->height + 1, k_topics, old_ptr);
}

void segment_tree::print()
{
	print_recursive(root, 0);
}

void segment_tree::print_recursive(node* in_ptr, int space)
{
	if (!in_ptr)
		return;

	print_recursive(in_ptr->left, space + 5);

	for (int i = 0; i < space; i++)
		std::cout << " ";
	std::cout << "NODO: [" << in_ptr->start << " - " << in_ptr->end << "] [ ";
	for (int i = 0; i < in_ptr->top_topics.size(); i++)
	{
		size_t id = in_ptr->top_topics[i].first;

		size_t count = in_ptr->top_topics[i].second;
		std::string& word = id_to_word[id];
		
		std::cout << word << "(" << count << ")";
		if (i != in_ptr->top_topics.size() - 1)
			std::cout << ", ";
	}

	std::cout << " ] H: " << in_ptr->height << "\n";
	print_recursive(in_ptr->right, space + 5);;
}