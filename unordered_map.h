#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "vector.h"
#include "pair.h"



template <typename KEY, typename VALUE>
struct node_u_map
{
    pair<KEY, VALUE> data;
    node_u_map* next;

    node_u_map(const KEY& k, const VALUE& v) :
        data(k, v), next(nullptr)
    {}
};


template <typename KEY, typename VALUE>
class unordered_map_iterator
{
public:
    using node_t = node_u_map<KEY, VALUE> ;

    unordered_map_iterator(node_t** buckets, size_t bucket_count, size_t bucket_index, node_t* current)
        : buckets(buckets), bucket_count(bucket_count), bucket_index(bucket_index), current(current)
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

    bool operator==(const unordered_map_iterator& other)
    {
        return current == other.current;
    }

    bool operator!=(const unordered_map_iterator& other)
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
    // Constructores
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

        for (size_t i = 0; i < bucket_count; ++i)
        {
            node_u_map<KEY, VALUE>* curr = other.buckets[i];
            node_u_map<KEY, VALUE>** ptr_to_next = &buckets[i];

            while (curr)
            {
                *ptr_to_next = new node_u_map<KEY, VALUE>(curr->data.first, curr->data.second);
                curr = curr->next;
                ptr_to_next = &((*ptr_to_next)->next);
            }
        }
    }

    unordered_map& operator=(const unordered_map& other)
    {
        if (this == &other)
            return *this;

        clear();

        bucket_count = other.bucket_count;
        _size = other._size;
        max_load_factor = other.max_load_factor;
        hash_func = other.hash_func;

        buckets = new node_u_map<KEY, VALUE>* [bucket_count]();

        for (size_t i = 0; i < bucket_count; ++i)
        {
            node_u_map<KEY, VALUE>* curr = other.buckets[i];
            node_u_map<KEY, VALUE>** ptr_to_next = &buckets[i];

            while (curr)
            {
                *ptr_to_next = new node_u_map<KEY, VALUE>(curr->data.first, curr->data.second);
                curr = curr->next;
                ptr_to_next = &((*ptr_to_next)->next);
            }
        }

        return *this;
    }

    ~unordered_map()
    {
        clear();
        delete[] buckets;
    }

    // Funciones
    void insert(const KEY& key, const VALUE& value)
    {
        if ((_size + 1) > bucket_count * max_load_factor)
            rehash(bucket_count * 2);

        size_t index = hash(key) % bucket_count;
        node_u_map<KEY, VALUE>* n = buckets[index];

        while (n)
        {
            if (n->data.first == key)
            {
                n->data.second += value;
                return;
            }
            n = n->next;
        }

        // Insert at the start
        n = new node_u_map<KEY, VALUE>(key, value);
        n->next = buckets[index];
        buckets[index] = n;
        _size++;
    }

    VALUE* find(const KEY& key)
    {
        size_t index = hash(key) % bucket_count;
        node_u_map<KEY, VALUE>* n = buckets[index];
        while (n)
        {
            if (n->data.first == key)
                return &n->data.second;
            n = n->next;
        }
        return nullptr;
    }

    void clear()
    {
        for (size_t i = 0; i < bucket_count; ++i)
        {
            node_u_map<KEY, VALUE>* n = buckets[i];
            while (n)
            {
                node_u_map<KEY, VALUE>* temp = n;
                n = n->next;
                delete temp;
            }
            buckets[i] = nullptr;
        }
        _size = 0;
    }

    size_t size()
    {
        return _size;
    
    }

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

    size_t hash(const KEY& key)
    {
        return hash_func(key);
    }

    void rehash(size_t new_bucket_count)
    {
        node_u_map<KEY, VALUE>** new_buckets = new node_u_map<KEY, VALUE>* [new_bucket_count]();

        for (size_t i = 0; i < bucket_count; ++i)
        {
            node_u_map<KEY, VALUE>* current = buckets[i];
            while (current)
            {
                node_u_map<KEY, VALUE>* next = current->next;
                size_t new_index = hash(current->data.first) % new_bucket_count;
                current->next = new_buckets[new_index];
                new_buckets[new_index] = current;
                current = next;
            }
        }

        delete[] buckets;
        buckets = new_buckets;
        bucket_count = new_bucket_count;
    }
};

#endif