#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>

template <typename T, typename Compare>
void quick_sort(T* begin, T* end, Compare comp)
{
	if (begin >= end - 1)
		return;

	T* left = begin;
	T* right = end - 1;
	T pivot = *(begin + (end - begin) / 2);

	while (left <= right)
	{
		while (comp(*left, pivot)) ++left;
		while (comp(pivot, *right)) --right;

		if (left <= right)
		{
			std::swap(*left, *right);
			++left;
			--right;
		}
	}

	if (begin < right)
		quick_sort(begin, right + 1, comp);
	if (left < end)
		quick_sort(left, end, comp);
}

class string_hash
{
public:
	size_t operator()(const std::string& str) const
	{
		size_t h = 0;
		for (char c : str)
			h = h * 31 + c;
		return h;
	}
};

class int_hash
{
public:
	size_t operator()(size_t x) const
	{
		x = ((x >> 16) ^ x) * 0x45d9f3bu;
		x = ((x >> 16) ^ x) * 0x45d9f3bu;
		x = (x >> 16) ^ x;
		return x;
	}
};

inline size_t my_pow(size_t x, size_t n)
{
	if (n == 0)
		return 1;

	size_t to_return = 1;
	
	while (n--)
		to_return *= x;

	return to_return;
}

#endif