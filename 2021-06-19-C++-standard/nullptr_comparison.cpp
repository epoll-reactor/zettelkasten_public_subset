#include <cstddef>
#include <cstring>
#include <cassert>

int main() {
	std::nullptr_t my_nullptr;
	const char *str = "XXXXXXXXXXXXXXXXXXXXXXXXXXXX";
	memcpy(&my_nullptr, str, sizeof(std::nullptr_t));
	const int *x{nullptr};
	assert(x == my_nullptr);
}
