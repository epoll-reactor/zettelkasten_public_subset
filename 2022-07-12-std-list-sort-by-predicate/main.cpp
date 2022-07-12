#include <iostream>
#include <list>

int main() {
	std::list<int> values{3, 1, 3, 2, 5, 5, 8, 6};
	values.sort([](int lhs, int rhs) {
		return lhs < rhs;
	});
	for (int i : values)
		std::cout << i << " ";
}