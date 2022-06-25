#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>
#include <initializer_list>

const unsigned int DEFAULT_RESERVED = 4;

template <typename T>
class Numvec
{
public:
    typedef unsigned int size_type;

    class BadLength
    {
    };

    Numvec() noexcept;
    explicit Numvec(size_type);
    Numvec(T *array, size_type);
    Numvec(std::initializer_list<T>);
    Numvec<T> &operator=(std::initializer_list<T>);

    Numvec(const Numvec<T> &);
    Numvec<T> &operator=(const Numvec<T> &);

    Numvec(Numvec<T> &&) noexcept;
    Numvec<T> &operator=(Numvec<T> &&);

    ~Numvec();

    size_type size() const;
    size_type capacity() const;
    bool is_empty() const;

    T &at(size_type) const;
    T &operator[](size_type) const;
    T *data() const;
    T &front() const;
    T &back() const;

    void push_back(const T &);
    void plus_back(const Numvec<T> &vec);
    void pop_back();
    void clear() noexcept;

    bool operator==(const Numvec<T> &other) const;

private:
    T *arr;
    size_type len = 0;
    size_type reserved = DEFAULT_RESERVED;

    int _check_length(int length);
    void reallocate();
};

template <typename T>
void Numvec<T>::reallocate()
{
    T *ptr = new T[reserved];
    for (size_type i = 0; i < len; i++)
    {
        ptr[i] = arr[i];
    }

    delete[] arr;
    arr = ptr;
}

template <typename T>
Numvec<T>::Numvec() noexcept
{
    arr = new T[reserved];
}

template <typename T>
Numvec<T>::Numvec(typename Numvec<T>::size_type n)
{
    size_type i;
    reserved = n << 2;
    arr = new T[reserved];
    for (i = 0; i < n; ++i)
        arr[i] = T();
    len = n;
}

template <typename T>
Numvec<T>::Numvec(T *array, size_type length)
    : len{length}, reserved{len * 4}, arr{new T[reserved]}
{
    for (size_type i = 0; i < length; i++)
    {
        arr[i] = array[i];
    }
}

template <typename T>
Numvec<T>::Numvec(std::initializer_list<T> lst)
    : len{static_cast<size_type>(lst.size())}, reserved{len * 4}, arr{new T[reserved]}
{
    uninitialized_copy(lst.begin(), lst.end(), arr);
}

template <typename T>
Numvec<T> &Numvec<T>::operator=(std::initializer_list<T> lst)
{
    len = static_cast<size_type>(lst.size());
    reserved = len * 4;
    arr = new T[reserved];

    return *this;
}

template <typename T>
Numvec<T>::Numvec(const Numvec<T> &other)
{
    size_type i;
    reserved = other.reserved;
    arr = new T[reserved];
    for (i = 0; i < other.len; ++i)
    {
        arr[i] = other.arr[i];
    }
    len = other.len;
}

template <typename T>
Numvec<T> &Numvec<T>::operator=(const Numvec<T> &other)
{
    size_type i;
    if (reserved < other.len)
    {
        reserved = other.len << 2;
        reallocate();
    }
    for (i = 0; i < other.len; ++i)
        arr[i] = other.arr[i];
    len = other.len;

    return *this;
}

template <typename T>
Numvec<T>::Numvec(Numvec<T> &&obj) noexcept
    : len{obj.size()}, arr{obj.data()} {}

template <typename T>
Numvec<T> &Numvec<T>::operator=(Numvec<T> &&other)
{
    size_type i;
    if (reserved < other.len)
    {
        reserved = other.len << 2;
        reallocate();
    }
    for (i = 0; i < other.len; ++i)
        arr[i] = std::move(other.arr[i]);
    len = other.len;

    return *this;
}

template <typename T>
Numvec<T>::~Numvec() { delete[] arr; }

template <typename T>
typename Numvec<T>::size_type Numvec<T>::size() const { return len; }

template <typename T>
typename Numvec<T>::size_type Numvec<T>::capacity() const { return reserved; }

template <typename T>
bool Numvec<T>::is_empty() const { return (!len); }

template <typename T>
T &Numvec<T>::at(size_type n) const
{
    if (n >= len)
        throw BadLength{};

    return arr[n];
}

template <typename T>
T &Numvec<T>::operator[](size_type n) const
{
    return arr[n];
}

template <typename T>
T *Numvec<T>::data() const { return arr; }

template <typename T>
T &Numvec<T>::front() const { return arr[0]; }

template <typename T>
T &Numvec<T>::back() const { return arr[len - 1]; }

template <typename T>
void Numvec<T>::push_back(const T &elem)
{
    if (len == reserved)
    {
        reserved <<= 2;
        reallocate();
    }

    arr[len++] = elem;
}

template <typename T>
void Numvec<T>::plus_back(const Numvec<T> &vec)
{
    size_type after_len = len + vec.size();
    if (after_len > reserved)
    {
        reserved = after_len << 2;
        reallocate();
    }

    for (size_type i = len, j = 0; i < after_len; i++, j++)
    {
        arr[i] = vec[j];
    }

    len = after_len;
}

template <typename T>
void Numvec<T>::pop_back() { len--; }

template <typename T>
void Numvec<T>::clear() noexcept { len = 0; }

template <typename T>
bool Numvec<T>::operator==(const Numvec<T> &other) const
{
    if (len != other.size())
        return false;

    T *p = other.data();
    for (size_type i = 0; i < len; i++)
    {
        if (p[i] != arr[i])
            return false;
    }

    return true;
}


#endif // VECTOR_HPP













