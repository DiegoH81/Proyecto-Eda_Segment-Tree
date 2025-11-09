#ifndef NODE_H
#define NODE_H

#include <unordered_map>
#include "vector.h"
#include "topic.h"

using topic_vector = vector<topic>;
bool topic_cmp(topic &a, topic& b);

class node
{
public:
    node* left, * right;
    topic_vector top_topics;
    size_t height, k_topics, start, end;

    // Constructores
    node();
    node(size_t in_start, size_t in_end, topic_vector in_t_topics, size_t in_height, size_t in_k_topics, node* in_left = nullptr, node* in_right = nullptr);

    // Metodos
    bool isLeaf();
    void merge();
    void update_height();

};

#endif