#ifndef NODE_H
#define NODE_H

#include <vector>
#include <unordered_map>
#include <algorithm>


using topic_vector = std::vector<std::pair<size_t, size_t>>;
bool topic_cmp(std::pair<size_t, size_t>& a, std::pair<size_t, size_t>& b);

class node
{
public:
    int start;
    int end;
    node* left, * right;
    topic_vector top_topics;
    size_t height, k_topics;


    // Constructores
    node();
    node(int in_start, int in_end, topic_vector in_t_topics, size_t in_height, size_t in_k_topics, node* in_left = nullptr, node* in_right = nullptr);

    // Metodos
    bool isLeaf();
    void merge();
    void update_height();

};

#endif