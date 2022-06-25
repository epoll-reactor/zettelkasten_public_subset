// Модифицируйте свою реализацию класса UniquePtr из предыдущей задачи, добавив к нему Deleter.

// Deleter - это функция или объект класса с перегруженным оператором ().
// Умный указатель вместо выполнения delete для уничтожения объекта должен вызывать эту
// функцию, передавая ей голый указатель в качестве аргумента. По умолчанию UniquePtr должен
// использовать тривиальный Deleter, который, как и раньше, вызывает delete (например, std::default_delete<T>).
// Но пользователь при желании может передать свой Deleter, который, например, может писать какие-либо
// логи, или подсчитывать число вызовов, или удалять особым образом память из вручную выделенного пула.

// Подробную спецификацию стандартного std::unique_ptr можно посмотреть на http://en.cppreference.com/w/cpp/memory/unique_ptr.

// В дополнение к функциям из предыдущей задачи, напишите следующие функции:

// 1. Конструктор, принимающий T * и Deleter.
// 2. Константную и неконстантную функцию get_deleter(), возвращающую соответственно константную
//    и неконстантную ссылку на сохранённый Deleter.
// В случае, если класс Deleter'а пустой (не имеет никаких полей и не унаследован от непустых классов),
// то sizeof вашего UniquePtr не должен превосходить sizeof обычного указателя, и никакой дополнительной
// динамической памяти выделяться не должно. Проще всего этого достичь, упаковав хранящийся указатель и
// Deleter в std::tuple, так как большинство реализаций std::tuple используют Empty Base Class Optimization.

#include <iostream>
#include <tuple>

template<typename T>
struct mydefaultdel {
  void operator() (T* ptr) noexcept {
    delete ptr;
  }
};

template<typename T>
struct mydefaultdel<T[]> {
   void operator() (T* ptr) noexcept {
      delete[] ptr;
   }
};

template<class T, class Deleter = mydefaultdel<T>>
class UniquePtr {
public:
  using ptr_t = std::decay_t<std::remove_extent_t<T>>;

  static constexpr size_t pointer_i = 0;
  static constexpr size_t deleter_i = 1;

  UniquePtr() noexcept {
    pointer = nullptr;
  }
  UniquePtr(ptr_t* cptr) {
    pointer = cptr;
  }
  UniquePtr(ptr_t* cptr, Deleter lambdadel) {
    pointer = cptr;
    deleter = lambdadel;
  }

  UniquePtr(UniquePtr&& rvref) {
    pointer = rvref.pointer;
    deleter = rvref.deleter;
    rvref.pointer = nullptr;
  }
  UniquePtr& operator = (UniquePtr&& rvref) {
    if (pointer != nullptr) {
      deleter(pointer);
    }
    deleter = std::move(rvref.deleter);
    pointer = rvref.pointer;
    rvref.pointer = nullptr;
    return *this;
  }

  UniquePtr& operator = (UniquePtr& rvref) = delete;
  UniquePtr(const UniquePtr& lvref) = delete;

  ptr_t* get() const noexcept {
    return pointer;
  }
  operator bool() const noexcept {
    return pointer != nullptr;
  }
  void reset() noexcept {
    if (*this) {
      deleter(pointer);
      pointer = nullptr;
    }
  }
  void reset(ptr_t* pointer_) {
    auto&& this_pointer = pointer;
    auto&& this_deleter = deleter;
    if (*this) {
      this_deleter(this_pointer);
      this_pointer = pointer_;
    } else {
      this_pointer = pointer_;
    }
  }
  ptr_t& operator *() const {
    return *pointer;
  }
  ptr_t* operator ->() const {
    return pointer;
  }
  bool operator == (const UniquePtr& ref) noexcept {
    return pointer == ref.pointer;
  }
  bool operator != (const UniquePtr& ref) noexcept {
    return *this == ref;
  }
  void swap(UniquePtr& rref) noexcept {
    std::swap(pointer, rref.pointer);
  }
  ptr_t operator[](std::uint64_t index) const {
    return pointer[index];
  }
  ptr_t* release() noexcept {
    auto&& this_pointer = pointer;
    ptr_t* temp = this_pointer;
    this_pointer = nullptr;
    return temp;
  }
  Deleter& get_deleter() noexcept {
    return deleter;
  }
  const Deleter& get_deleter() const noexcept {
    return deleter;
  }

  ~UniquePtr() {
    if (pointer) {
      deleter(pointer);
    }
  }

private:
  ptr_t* pointer;
  [[no_unique_address]] Deleter deleter;
};
