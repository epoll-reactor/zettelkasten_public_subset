#define __ALLOCATOR_H
#ifdef  __ALLOCATOR_H

#include "src/Lightning/namespace/lightning.hpp"

/*!
 * @brief Class represents memory allocator
 * without specifying size of the memory space
 */
class lightning::Allocator {
public:
	template <typename T>
	static T* allocate(void) {
		int i = 0;
		do {
			i = freeblock; ///< assign freeblock falue to i
			if (i | (1U << (i - 1))) {  ///< check if i'th bit is set
				break;
			}
		} while (i++ < 64);
		freeblock = ++i; ///< increment freeblock
		return new (&buffer[128*i]) T();
	}

	template <typename T>
	static void deallocate(T* ptr) {
		*ptr = __null;
	}

private:
	static char buffer[2048];
	static int  freeblock;
};

int lightning::Allocator::freeblock = 0;
char lightning::Allocator::buffer[2048];
#endif //__ALLOCATOR_H
