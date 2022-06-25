// Реализуйте свой класс Vector, аналогичный std::vector<int>. Эта задача простая:
// требуется, чтобы ваш класс работал только с числами типа int (то есть, не
// был бы шаблонным) и предоставлял функции push_back, pop_back, size, operator[],
// а также конструктор без параметров, конструктор копирования, оператор присваивания
// и деструктор. Ваш класс должен непосредственно работать с динамической памятью:
// пользоваться аналогичными классами стандартной библиотеки запрещено.
#include <stdexcept>


class BadLength {
};

class Vector {
public:
  typedef unsigned int size_type;

  explicit Vector() noexcept;

  Vector(const Vector&);
  Vector &operator=(const Vector&);

  ~Vector();

  size_type size() const;
  size_type capacity() const;

  int& operator[](size_type) const;

  void push_back(int);
  void pop_back();

private:
  int* arr;
  size_type len = 0;
  size_type reserved = 4;

  int _check_length(int length);
  void reallocate();
};


void Vector::reallocate() {
  int *ptr = new int[reserved];
  for (size_type i = 0; i < len; i++) {
    ptr[i] = arr[i];
  }

  delete[] arr;
  arr = ptr;
}


Vector::Vector() noexcept {
  arr = new int[reserved];
}

Vector::Vector(const Vector &other) {
  size_type i;
  reserved = other.reserved;
  arr = new int[reserved];
  for (i = 0; i < other.len; ++i) {
    arr[i] = other.arr[i];
  }
  len = other.len;
}


Vector &Vector::operator=(const Vector &other) {
  size_type i;
  if (reserved < other.len) {
    reserved = other.len << 2;
    reallocate();
  }
  for (i = 0; i < other.len; ++i)
      arr[i] = other.arr[i];
  len = other.len;

  return *this;
}

Vector::~Vector() { delete[] arr; }

typename Vector::size_type Vector::size() const { return len; }

typename Vector::size_type Vector::capacity() const { return reserved; }

int &Vector::operator[](size_type n) const {
  return arr[n];
}


void Vector::push_back(int elem) {
  if (len == reserved) {
    reserved <<= 2;
    reallocate();
  }

  arr[len++] = elem;
}


void Vector::pop_back() { len--; }
