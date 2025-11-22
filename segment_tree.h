#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include "node.h"
#include "vector.h"
#include "topic.h"
#include "unordered_map.h"
#include "funciones.h"
#include "pair.h"

#include <string>
#include <iostream>

class segment_tree
{
public:
    // Constructor
    segment_tree(size_t in_k_topics = 0, size_t in_update = 8192);
    ~segment_tree();

    segment_tree& operator=(const segment_tree& other);

    // Methods
    vector<pair<std::string, size_t>> query(size_t start, size_t end, size_t in_k);
    void print();
    void print_root();
    void insert(vector<pair<std::string, size_t>>& topics);

    bool is_empty();

    size_t get_time();
    size_t get_size();
private:
    node* root;
    size_t k_topics, size, time, update_time;
    unordered_map <std::string, size_t, string_hash> word_to_id;
    vector <std::string> id_to_word;

    // Methods
    void process_topic(std::string& in_topic);
    void grow_node(node** in_tr);
    void adjust_tree(vector <node*>& path);
    void print_recursive(node* in_ptr, int space);
    void recursive_query(node* in_ptr, size_t start, size_t end, unordered_map<size_t, size_t, int_hash>& answer);
    void recursive_destructor(node **in_ptr);

    node* deep_copy_tree(node* in_ptr);

    node** find_pos(vector<node*>& path);

};
#endif