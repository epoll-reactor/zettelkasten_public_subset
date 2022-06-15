#include <cstdio>
#include <cstdlib>

template <typename MemoryPool>
void *::operator new(std::size_t size, MemoryPool &pool) {
	return pool.allocate(size);
}

int main() {
	struct MemoryPool {
		void *allocate(std::size_t size) {
			return std::malloc(size);
		}
	} pool;

	struct A { A(int a, int b) : a_(a), b_(b) {} int a_, b_; };
	A *a = new (pool) A(1, 2);
	std::printf("a->a_ = %d\n", a->a_);
	std::printf("a->b_ = %d\n", a->b_);
}
