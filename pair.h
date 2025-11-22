#ifndef PAIR_H
#define PAIR_H


template <typename A, typename B>
class pair
{
public:
    A first;
    B second;

    pair() :
        first(), second() {}

    pair(A& in_a, B& in_b) :
        first(in_a), second(in_b) {}
};

#endif