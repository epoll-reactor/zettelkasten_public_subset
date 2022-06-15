#include <new>
#include <cstdio>
#include <cstdint>
#include <cassert>

int main() {
	auto is_aligned = [](void *memory, size_t bytes) {
		return reinterpret_cast<uintptr_t>(memory) % bytes == 0;
	};

	std::align_val_t align = static_cast<std::align_val_t>(256);
	if (void *memory = ::operator new(8, align)) {
		assert(is_aligned(memory, 256));
		::operator delete(memory, align);
	}
}
