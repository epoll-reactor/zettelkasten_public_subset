#include <utility>

enum x {_1=100,_2=200};

struct pair_wrapper {
	std::pair<enum x, enum x> pair_;
	pair_wrapper(std::pair<enum x, enum x> pair)
		: pair_(pair) {}
	operator std::pair<enum x, enum x>() const {
		return pair_;
	}
};

pair_wrapper operator,(enum x lhs, enum x rhs) {
	return std::make_pair(lhs, rhs);
}
pair_wrapper operator,(pair_wrapper wrapper, enum x rhs) {
	return std::make_pair(
		wrapper.pair_.first, rhs);
}


struct comma_operator_test {
	int operator[](std::pair<enum x, enum x> pair) {
		return pair.first + pair.second;
	}
};

int main() {
	comma_operator_test test;
	return test[_1,_2,_1,_2,_1,_2,_1];
}
