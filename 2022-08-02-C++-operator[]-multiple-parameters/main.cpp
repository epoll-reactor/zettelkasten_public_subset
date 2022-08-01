#include <utility>

enum x {_1=100,_2=200};

std::pair<enum x, enum x> operator,(enum x lhs, enum x rhs) {
	return std::make_pair(lhs, rhs);
}

struct comma_operator_test {
	int operator[](std::pair<enum x, enum x> pair) {
		return pair.first + pair.second;
	}
};

int main() {
	comma_operator_test test;
	return test[_1,_2];
}
