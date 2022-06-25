// Jul 11, 2021
#ifndef SANDBOX_MEMORY_POOL_HPP
#define SANDBOX_MEMORY_POOL_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <utility>
#include <cstdio>
#include <new>

class MemoryPool
{
public:
    explicit MemoryPool(size_t bytes) noexcept : memory(nullptr), current_pointer(nullptr), memory_size(bytes)
    {
        allocate_memory(bytes);
    }

    ~MemoryPool()
    {
        release_memory();
    }

    template <typename T, typename... Args>
    T* allocate(Args&&... args) noexcept
    {
        if ((current_pointer + sizeof(T)) > (memory + memory_size)) { std::cout << "Out of memory" << std::endl; exit(-1); }
        current_pointer += sizeof(T);
        new (current_pointer) T(std::forward<Args>(args)...);

        return reinterpret_cast<T*>(current_pointer);
    }

private:
    void allocate_memory(size_t bytes) noexcept
    {
        if (memory) { delete[] memory; }
        memory = new char[bytes];
        std::cout << "allocate " << bytes << " bytes" << std::endl;
        current_pointer = memory;
        memory_size = bytes;
    }

    void release_memory() noexcept
    {
        if (memory)
        {
            delete[] memory;
            memory = nullptr;
            current_pointer = nullptr;
            memory_size = 0;
        }
    }

    char* memory;
    char* current_pointer;
    size_t memory_size;
};

#endif //SANDBOX_MEMORY_POOL_HPP
