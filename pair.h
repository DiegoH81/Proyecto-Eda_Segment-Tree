#ifndef PAIR_H
#define PAIR_H


template <typename A, typename B>
class pair
{
public:
    A first;
    B second;

    pair() = default;
    pair(const A& in_a, const B& in_b) : first(in_a), second(in_b) {}
};

#endif