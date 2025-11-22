#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "vector.h"
#include "pair.h"


template <typename KEY, typename VALUE>
struct node_u_map
{
    pair<KEY, VALUE> data;
    node_u_map* next;

    // Constructor
    node_u_map(KEY& in_key, VALUE& in_value) :
        data(in_key, in_value), next(nullptr)
    {}
};


template <typename KEY, typename VALUE>
class unordered_map_iterator
{
public:
    using node_t = node_u_map<KEY, VALUE> ;

    // Constructor
    unordered_map_iterator(node_t** in_buckets, size_t in_bucket_count, size_t in_bucket_index, node_t* in_current)
        : buckets(in_buckets), bucket_count(in_bucket_count), bucket_index(in_bucket_index), current(in_current)
    {}

    unordered_map_iterator& operator++()
    {
        if (current)
            current = current->next;

        while (!current && bucket_index + 1 < bucket_count)
        {
            bucket_index++;
            current = buckets[bucket_index];
        }
        return *this;
    }

    bool operator==(unordered_map_iterator& other)
    {
        return current == other.current;
    }

    bool operator!=(unordered_map_iterator& other)
    {
        return current != other.current;
    }

    pair<KEY, VALUE>& operator*()
    {
        return current->data;
    }

    pair<KEY, VALUE>* operator->()
    {
        return &(current->data);
    }

private:
    node_t** buckets;
    size_t bucket_count;
    size_t bucket_index;
    node_t* current;
};


template <typename KEY, typename VALUE, typename FUNC>
class unordered_map
{
public:
    using iterator = unordered_map_iterator<KEY, VALUE>;
    
    // Constructor
    unordered_map(size_t bucket_count = 16) :
        bucket_count(bucket_count), max_load_factor(0.75f), hash_func()
    {
        buckets = new node_u_map<KEY, VALUE>* [bucket_count]();
        _size = 0;
    }

    unordered_map(unordered_map& other)
        : bucket_count(other.bucket_count), _size(other._size),
          max_load_factor(other.max_load_factor), hash_func(other.hash_func)
    {
        buckets = new node_u_map<KEY, VALUE>* [bucket_count]();

        // Copy data
        for (size_t i = 0; i < bucket_count; ++i)
        {
            node_u_map<KEY, VALUE>* cur_bucket = other.buckets[i];
            node_u_map<KEY, VALUE>** ptr_to_next = &buckets[i];

            while (cur_bucket)
            {
                *ptr_to_next = new node_u_map<KEY, VALUE>(cur_bucket->data.first, cur_bucket->data.second);

                cur_bucket = cur_bucket->next;
                ptr_to_next = &((*ptr_to_next)->next);
            }
        }
    }

    unordered_map& operator=(const unordered_map& other)
    {
        if (this != &other)
        {
            clear();

            bucket_count = other.bucket_count;
            _size = other._size;
            max_load_factor = other.max_load_factor;
            hash_func = other.hash_func;

            buckets = new node_u_map<KEY, VALUE>* [bucket_count]();

            // Copy data
            for (size_t i = 0; i < bucket_count; ++i)
            {
                node_u_map<KEY, VALUE>* cur_bucket = other.buckets[i];
                node_u_map<KEY, VALUE>** ptr_to_next = &buckets[i];

                while (cur_bucket)
                {
                    *ptr_to_next = new node_u_map<KEY, VALUE>(cur_bucket->data.first, cur_bucket->data.second);

                    cur_bucket = cur_bucket->next;
                    ptr_to_next = &((*ptr_to_next)->next);
                }
            }
        }

        return *this;
    }

    ~unordered_map()
    {
        clear();
        delete[] buckets;
    }

    // Methods
    void insert(KEY& key, VALUE value)
    {
        if ((_size + 1) > bucket_count * max_load_factor)
            rehash(bucket_count * 2);

        size_t index = hash(key) % bucket_count;
        node_u_map<KEY, VALUE>* cur_node = buckets[index];

        // Find key
        while (cur_node)
        {
            if (cur_node->data.first == key)
            {
                cur_node->data.second += value;
                return;
            }

            cur_node = cur_node->next;
        }

        // Insert at the the start
        cur_node = new node_u_map<KEY, VALUE>(key, value);
        cur_node->next = buckets[index];
        buckets[index] = cur_node;

        _size++;
    }

    VALUE* find(KEY& key)
    {
        size_t index = hash(key) % bucket_count;
        node_u_map<KEY, VALUE>* cur_node = buckets[index];

        while (cur_node)
        {
            if (cur_node->data.first == key) // If found
                return &cur_node->data.second;

            cur_node = cur_node->next;
        }

        return nullptr;
    }

    void clear()
    {
        for (size_t i = 0; i < bucket_count; ++i)
        {
            node_u_map<KEY, VALUE>* cur_node = buckets[i];

            while (cur_node)
            {
                node_u_map<KEY, VALUE>* temp = cur_node;

                cur_node = cur_node->next;
                delete temp;
            }

            buckets[i] = nullptr;
        }
        _size = 0;
    }

    size_t size() { return _size; }

    iterator begin()
    {
        for (size_t i = 0; i < bucket_count; ++i)
        {
            if (buckets[i])
                return iterator(buckets, bucket_count, i, buckets[i]);
        }
        return end();
    }

    iterator end()
    {
        return iterator(buckets, bucket_count, bucket_count, nullptr);
    }

private:
    node_u_map<KEY, VALUE>** buckets;
    size_t bucket_count, _size;
    FUNC hash_func;
    float max_load_factor;

    // Methods
    size_t hash(KEY& key)
    {
        return hash_func(key);
    }

    void rehash(size_t new_bucket_count)
    {
        node_u_map<KEY, VALUE>** new_buckets = new node_u_map<KEY, VALUE>* [new_bucket_count]();

        for (size_t i = 0; i < bucket_count; ++i)
        {
            node_u_map<KEY, VALUE>* cur_node = buckets[i];

            // Re-assign nodes
            while (cur_node)
            {
                node_u_map<KEY, VALUE>* next = cur_node->next;

                size_t new_index = hash(cur_node->data.first) % new_bucket_count;
                cur_node->next = new_buckets[new_index];
                new_buckets[new_index] = cur_node;
                cur_node = next;
            }
        }

        delete[] buckets;
        buckets = new_buckets;
        bucket_count = new_bucket_count;
    }
};

#endif