#ifndef SORT_H
#define SORT_H


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
#endif