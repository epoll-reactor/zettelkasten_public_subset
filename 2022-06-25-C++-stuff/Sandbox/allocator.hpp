// July 14, 2021
#include <cstdint>
#include <limits>

template<typename T>
class Allocator
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    template<typename U>
    struct rebind {
        typedef Allocator<U> other;
    };

public:
    Allocator() {}
    ~Allocator() {}
    Allocator(Allocator const&) { }
    template<typename U>
    explicit Allocator(Allocator<U> const&) {}

    pointer address(reference r) const noexcept { return &r; }
    const_pointer address(const_reference r) const noexcept { return &r; }

    pointer allocate(size_type cnt, const void* = 0) noexcept {
        return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T)));
    }
     void deallocate(pointer p, size_type) {
        ::operator delete(p);
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    void construct(pointer p, const T& t) { new(p) T(t); }
    void destroy(pointer p) const noexcept { p->~T(); }

    constexpr bool operator==(Allocator const&) const noexcept { return true; }
    constexpr bool operator!=(Allocator const& a) const noexcept { return !operator==(a); }
};

int test()
{
    Allocator<int> alloc;

    int* ptr = alloc.allocate(1);
    alloc.deallocate(ptr, sizeof(int));
    alloc.max_size();
    int i = 1;
    alloc.destroy(ptr);
}
