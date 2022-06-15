#include <cstdio>
#include <new>

void handler() {
	std::printf("::operator new failed\n");
	std::set_new_handler(nullptr);
}

int main() {
	std::set_new_handler(handler);

	try {
		while (true) {
			new int[100000000ul];
		}
	} catch (const std::bad_alloc& e) {
		printf("%s\n", e.what());
	}
}
