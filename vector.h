#ifndef VECTOR_H
#define VECTOR_H
#include "sort.h"

template <class T>
class vector_iterator
{
public:
	// Constructores
	vector_iterator(T* in_ptr) :
		ptr(in_ptr) {}

	vector_iterator& operator++()
	{
		++ptr;
		return *this;
	}

	vector_iterator operator++(int)
	{
		vector_iterator<T> temp = *this;
		++ptr;
		return temp;
	}

	bool operator==(vector_iterator& other)
	{
		return ptr == other.ptr;
	}

	bool operator!=(vector_iterator& other)
	{
		return ptr != other.ptr;
	}

	T& operator*()
	{
		return *ptr;
	}

	T* operator->()
	{
		return ptr;
	}
private:
	T* ptr;
};


template <class T>
class vector
{
public:
	using iterator = vector_iterator<T>;

	//Constructores
	vector() :data(nullptr), _size(0), capacity(0)
	{}
	vector(vector& other):
		data(nullptr), _size(other._size), capacity(other.capacity)
	{
		if (_size)
		{
			data = new T[capacity];

			for (size_t i = 0; i < _size; i++)
				data[i] = other.data[i];
		}
	}
	~vector()
	{
		clear();
	}
	
	vector& operator=(const vector& other)
	{
		if (this != &other)
		{
			if (data)
				delete[] data;

			_size = other._size;
			capacity = other.capacity;
			data = nullptr;

			if (_size)
			{
				data = new T[capacity];

				for (size_t i = 0; i < _size; i++)
					data[i] = other.data[i];
			}
		}
		return *this;
	}
	T& operator[](size_t idx)
	{
		return *(data + idx);
	}

	//Metodos
	void push_back(T value)
	{
		if (_size == capacity)
		{
			size_t new_capacity = 0;
			if (capacity == 0)
				new_capacity = 1;
			else
				new_capacity = capacity * 2;

			reserve(new_capacity);
		}

		data[_size] = value;
		_size++;
	}
	void pop_back()
	{
		if (_size)
			_size--;
	}
	void clear()
	{
		if (data)
		{
			delete[] data;
			data = nullptr;
			_size = capacity = 0;
		}
	}
	void reserve(size_t new_capacity)
	{
		if (new_capacity <= capacity)
			return;

		T* new_data = new T[new_capacity];

		for (size_t i = 0; i < _size; i++)
			new_data[i] = data[i];


		delete[] data;

		data = new_data;
		capacity = new_capacity;
	}
	void resize(size_t new_size)
	{
		if (new_size < _size)
		{
			_size = new_size;
			return;
		}
		if (new_size > capacity)
			reserve(new_size);

		for (size_t i = _size; i < new_size; ++i)
			data[i] = T();

		_size = new_size;
	}

	bool empty()
	{
		if (_size == 0)
			return true;
		return false;
	}
	template <class OP>
	void sort(OP op)
	{
		if (_size <= 1)
			return;

		quick_sort(data, data + _size, op);
	}

	T& back()
	{
		return data[_size - 1];
	}

	iterator begin()
	{
		return vector_iterator<T>(data);
	}
	iterator end()
	{
		return vector_iterator<T>(data + _size);
	}
	size_t size()
	{
		return _size;
	}
private:
	T* data;
	size_t _size;
	size_t capacity;
};

#endif