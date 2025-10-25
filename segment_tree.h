#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include "node.h"
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

class segment_tree
{
public:
    // Constructor
    segment_tree(size_t k_topics);

    // Metodos
    void insert(std::string topic);
    void query(size_t start, size_t end);
    void print();
    void print_root();
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

    node** find_pos(std::vector <node*>& path);


};

#endif