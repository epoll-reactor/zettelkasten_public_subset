#include <iostream>
#include <algorithm>

using namespace std;

// Конструктор по шаблону может использоваться для копирования
// объекта, если он обеспечивает лучшее совпадение, чем все остальные
// конструкторы.
template <typename T, size_t size>
class fixed_vector
{
public:
    using       iterator =       T *;
    using const_iterator = const T *;

    // Необходим, так как при определении других конструкторов,
    // удаляется.
    fixed_vector() = default;
    // fixed_vector<char, 4> v;
    // fixed_vector<int , 4> w(v);
    // Привело бы к ошибке с таким конструктором, так как
    // он ожидает полное соответствие типов, хоть и char
    // может быть неявно приведён к int.
    fixed_vector(const fixed_vector &) = default;
    fixed_vector &operator=(const fixed_vector &) = default;

    // Не конструктор копирования. Другие шаблонные параметры.
    template <typename O, size_t osize>
    fixed_vector(const fixed_vector<O, osize> &other)
    {
        copy(
            other.begin(),
            other.begin() + min(size, osize),
            begin()
        );
    }

    // Не оператор копирующего присваивания. Другие шаблонные параметры.
    template <typename O, size_t osize>
    fixed_vector<T, size> &operator=(const fixed_vector<O, osize> &other)
    {
        // В книге просто скопирован код конструктора выше.
        new (this) fixed_vector(other);
        return *this;
    }

          iterator begin()       { return v_; }
          iterator end  ()       { return v_ + size; }
    const_iterator begin() const { return v_; }
    const_iterator end  () const { return v_ + size; }

private:
    T v_[size];
};

int main()
{
    fixed_vector<char, 4> v;
    *(v.begin() + 0) = 1;
    *(v.begin() + 1) = 2;
    *(v.begin() + 2) = 3;
    *(v.begin() + 3) = 4;
    fixed_vector<int , 4> w;
    w = v;

    cout << *(w.begin() + 0) << endl;
    cout << *(w.begin() + 1) << endl;
    cout << *(w.begin() + 2) << endl;
    cout << *(w.begin() + 3) << endl;
}
