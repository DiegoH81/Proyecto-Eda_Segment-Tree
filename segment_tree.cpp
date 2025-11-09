#include "segment_tree.h"

// Constructores
segment_tree::segment_tree(size_t in_k_topics)
	: root(nullptr), k_topics(in_k_topics * 1.5), size(0), time(0), word_to_id(), id_to_word()
{}

segment_tree::segment_tree()
	: root(nullptr), k_topics(0), size(0), time(0), word_to_id(), id_to_word()
{
}

segment_tree::~segment_tree()
{
	if (root)
	{
		word_to_id.clear();
		id_to_word.clear();
		recursive_destructor(&root);
	}
}


// Funciones
void segment_tree::insert(std::vector<std::pair<std::string, size_t>>& topics)
{
	time++;
	size++;

	topic_vector topics_data;
	topics_data.reserve(topics.size());

	for (auto& topic : topics)
	{
		process_topic(topic.first);
		topics_data.push_back({ word_to_id[topic.first] , topic.second });
	}


	std::vector<node*> path;
	node** pos = find_pos(path);

	*pos = new node(time, time, topics_data, 0, k_topics);
	adjust_tree(path);
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

		size_t& start = (*ptr)->start;
		size_t end = start + std::pow(2, (*ptr)->height) - 1;

		size_t mid = (start + end) / 2;

		if (time <= mid)
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

std::vector<std::pair<std::string, size_t>> segment_tree::query(size_t start, size_t end, size_t in_k)
{
	if (start < 1)
		start = 1;
	if (end > time)
		end = time;
	if (start > end)
		std::swap(start, end);
	if (start > time)
		start = end = time;

	std::map <size_t, size_t> answer;
	recursive_query(root, start, end, answer);

	std::vector<std::pair<size_t, size_t>> new_order;

	for (auto& item : answer)
		new_order.push_back({ item.first, item.second });

	std::sort(new_order.begin(), new_order.end(), topic_cmp);

	if (new_order.size() > in_k)
		new_order.resize(in_k);

	// Join answers
	std::vector<std::pair<std::string, size_t>> answer_vec;
	answer_vec.reserve(in_k);

	for (auto& topic : new_order)
		answer_vec.push_back({ id_to_word[topic.first], topic.second });

	return answer_vec;
}

void segment_tree::recursive_query(node* in_ptr, size_t range_start, size_t range_end, std::map <size_t, size_t>& answer)
{
	if (!in_ptr)
		return;

	size_t& ptr_start = in_ptr->start;
	size_t& ptr_end = in_ptr->end;

	if (range_end < ptr_start || range_start > ptr_end) // Out of range
		return;

	if (ptr_start == range_start && ptr_end == range_end)
	{
		/*
		if (in_ptr->top_topics.size() < k_topics)
		{
			if (in_ptr->left)
				recursive_query(in_ptr->left, range_start, in_ptr->left->end, answer);
			if (in_ptr->right)
				recursive_query(in_ptr->right, in_ptr->right->start, range_end, answer);
			return;
		}
		*/
		for (auto& topic : in_ptr->top_topics)
			answer[topic.first] += topic.second;

		return;
	}

	size_t end = ptr_start + std::pow(2, in_ptr->height) - 1;

	size_t mid = (ptr_start + end) / 2;


	size_t& l_end = in_ptr->left->end;
	size_t& r_start = in_ptr->right->start;

	if ( range_start <= mid && mid <= range_end ) // Query is in left and right
	{
		recursive_query(in_ptr->left, range_start, l_end, answer);
		recursive_query(in_ptr->right, r_start, range_end, answer);
	}
	else if (range_end <= mid) // Query is in left
		recursive_query(in_ptr->left, range_start, range_end, answer);
	else					   // Query is in right
		recursive_query(in_ptr->right, range_start, range_end, answer);
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

void segment_tree::print_root()
{
	if (!root)
		return;

	std::cout << "NODO: [" << root->start << " - " << root->end << "] [ ";
	for (int i = 0; i < root->top_topics.size(); i++)
	{
		size_t id = root->top_topics[i].first;

		size_t count = root->top_topics[i].second;
		std::string& word = id_to_word[id];

		std::cout << word << "(" << count << ")";
		if (i != root->top_topics.size() - 1)
			std::cout << ", ";
	}

	std::cout << " ] H: " << root->height << "\n";
}

size_t segment_tree::get_time() { return time; }

void segment_tree::recursive_destructor(node** in_ptr)
{
	if (!*in_ptr)
		return;

	recursive_destructor(&((*in_ptr)->left));
	recursive_destructor(&((*in_ptr)->right));

	delete* in_ptr;
	*in_ptr = nullptr;
}