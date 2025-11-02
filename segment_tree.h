#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include "node.h"
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>

class segment_tree
{
public:
    // Constructor
    segment_tree(size_t k_topics);

    // Metodos
    std::vector<std::pair<std::string, size_t>> query(size_t start, size_t end, size_t in_k);
    void print();
    void print_root();
    void insert(std::vector<std::pair<std::string, size_t>>& topics);

    size_t get_time();
private:
    node* root;
    size_t k_topics, size, time;
    std::map <std::string, size_t> word_to_id;
    std::vector <std::string> id_to_word;

    // Metodos
    void process_topic(std::string& in_topic);
    void grow_node(node** in_ptr);
    void adjust_tree(std::vector <node*>& path);
    void print_recursive(node* in_ptr, int space);
    void recursive_query(node* in_ptr, size_t start, size_t end, std::map <size_t, size_t>& answer);
    

    node** find_pos(std::vector <node*>& path);

};

#endif