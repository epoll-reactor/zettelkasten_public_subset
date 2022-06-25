#include "src/Lightning/Array/array.hpp"

using lightning::Array;
using std::initializer_list;
using std::invalid_argument;

template <class T, uint Size>
	Array < T, Size > ::Array(): length(Size) {
		for (uint i = 0; i < length; i++) {
			array[i] = 0;
		}
	}

template <class T, uint Size>
	Array < T, Size > ::Array(const initializer_list < T > static_array) {
		if (static_array.size() <= Size) {
			std::copy(static_array.begin(),
                      static_array.end(), array);
			this -> length = static_array.size();
		} else {
			throw invalid_argument("Array size exceeded");
		}
	}

template <class T, uint Size>
	void Array < T, Size > ::fill(const T what) {
		uint16_t index = 0;
		while (index != length) {
			array[index++] = what;
		}
	}

template <class T, uint Size>
	T & Array < T, Size > ::operator[](uint index) {
		if (index < length) {
			return array[index];
		} else {
			throw invalid_argument("Out of range");
		}
	}

template <class T, uint Size>
	void Array < T, Size > ::operator = (Array & rhs) {
		this -> array[length];
		for (int i = 0; i < length; i++) {
			this -> array[i] = rhs.array[i];
		}
	}

template <class T, uint Size>
	bool Array < T, Size > ::operator == (Array & rhs) {
		for (int i = 0; i < length; i++) {
			if (this -> array[i] != rhs.array[i]) {
				return false;
			}
		}
		return true;
	}

template <class T, uint Size>
	bool Array < T, Size > ::operator != (Array & rhs) {
		return !(( * this) == rhs);
	}

template <class T, uint Size>
	bool Array < T, Size > ::operator < (Array & rhs) {
		uint lhs_len = ( * this).size();
		uint rhs_len = rhs.size();
		if (lhs_len > rhs_len) {
			for (uint i = 0; i < lhs_len; i++) {
				if (this -> array[i] < rhs.array[i]) {
					return true;
				}
			}
		} else {
			for (uint i = 0; i < rhs_len; i++) {
				if (this -> array[i] < rhs.array[i]) {
					return true;
				}
			}
		}
		return false;
	}

template <class T, uint Size>
	bool Array < T, Size > ::operator > (Array & rhs) {
		uint lhs_len = ( * this).size();
		uint rhs_len = rhs.size();
		if (lhs_len > rhs_len) {
			for (uint i = 0; i < lhs_len; i++) {
				if (this -> array[i] > rhs.array[i]) {
					return true;
				}
			}
		} else {
			for (uint i = 0; i < rhs_len; i++) {
				if (this -> array[i] > rhs.array[i]) {
					return true;
				}
			}
		}
		return false;
	}

template <class T, uint Size>
	constexpr void Array < T, Size > ::swap(Array & other) {
		std::swap(this -> array, other.array);
	}

template <class T, uint Size>
	constexpr uint Array < T, Size > ::size() {
		return this -> length;
	}

template <class T, uint Size>
	void Array < T, Size > ::reverse() {
		for (uint i = 0; i < this -> length / 2; i++) {
			array[i] ^= array[this -> length - i - 1];
			array[this -> length - i - 1] ^= array[i];
			array[i] ^= array[this -> length - i - 1];
		}
	}

template <class T, uint Size>
	T Array < T, Size > ::first() {
		return this -> array[0];
	}

template <class T, uint Size>
	T Array < T, Size > ::last() {
		return this -> array[length - 1];
	}
