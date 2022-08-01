#include <utility>

enum x {_1=100,_2=200};

std::pair<x, x> operator,(x lhs, x rhs) {
	return std::make_pair(lhs, rhs);
}
std::pair<x, x> operator,(std::pair<x, x> pair, x rhs) {
	return std::make_pair(pair.first, rhs);
}

struct comma_operator_test {
	int operator[](std::pair<x, x> pair) {
		return pair.first + pair.second;
	}
};

int main() {
	comma_operator_test test;
	return test[_1,_2,_1,_2,_1,_2,_1];
}
