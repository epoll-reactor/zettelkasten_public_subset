#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "src/Lightning/namespace/lightning.hpp"
#include <iostream>

template<class T, unsigned int Size>
class lightning::Array {
public:
	typedef unsigned int uint;

	Array();

	Array(const std::initializer_list<T> _array);

	void fill(const T value);

	void reverse();

	T first();

	T last();

	constexpr uint size();

	constexpr void swap(Array& other);

	T& operator[](uint index);

	void operator=(Array&);

	bool operator==(Array&);

	bool operator!=(Array&);

	bool operator<(Array&);

	bool operator>(Array&);

private:
	uint length;

	T array[Size];
};

#include "src/Lightning/Array/array.tcc"

#endif // ARRAY_HPP

