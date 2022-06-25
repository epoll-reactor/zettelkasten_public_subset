// Напишите свою упрощённую реализацию класса std::unique_ptr<T>.

// Подробную спецификацию стандартного std::unique_ptr можно посмотреть на
// http://en.cppreference.com/w/cpp/memory/unique_ptr. В этой задаче вам не нужно делать
// специализацию для массива и не нужно предусматривать свой Deleter.

// Назовите ваш класс UniquePtr. У класса должен быть один шаблонный параметр T - тип
// элемента, указатель на который будет храниться внутри.

// Напишите следующие функции в классе:

// 1.  Конструктор по умолчанию, создающий пустой умный указатель.
// 2.  Конструктор, принимающий T * и захватывающий владение этой динамической памятью.
// 3.  Конструктор перемещения, получающий на вход rvalue-ссылку на другой UniquePtr и отбирающий у
//     него владение ресурсом.
// 4.  Оператор присваивания, получающий на вход nullptr (тип - std::nullptr_t, определенный в заголовочном файле cstddef).
//     В результате умный указатель должен стать пустым.
// 5.  Move-оператор присваивания, получающий на вход rvalue-ссылку на другой UniquePtr.
// 6.  Деструктор.
// 7.  Константный оператор *.
// 8.  Константный оператор -> (он должен вернуть просто сам указатель).
// 9.  Функцию T * release(), отменяющую владение объектом и возвращающую хранящийся внутри указатель.
// 10. Функцию void reset(T * ptr), после выполнения которой умный указатель должен захватить ptr.
// 11. Функцию void swap(UniquePtr& other), обменивающуюся содержимым с другим умным указателем.
// 12. Функцию T * get() const, возвращающую указатель.
// 13. explicit operator bool() const, позволяющий определить, не пуст ли умный указатель.
// В вашем классе должны быть запрещены конструктор копирования и обычный оператор присваивания. Пометьте
// все подходящие функции тегом noexcept (чтобы подчеркнуть, что они не должны генерировать исключений).

// sizeof вашего класса должен совпадать с sizeof от обычного указателя, а дополнительной динамической
// памяти внутри UniquePtr выделяться не должно. То есть UniquePtr не должен иметь накладных расходов на
// память по сравнению с обычным указателем.

#include <cstdint>
#include <utility>

template <typename T>
class UniquePtr {
public:
  UniquePtr() noexcept : ptr(nullptr) { }
  UniquePtr(T* ptr_) noexcept : ptr(ptr_) { ptr_ = nullptr; }
  UniquePtr(UniquePtr&& other) noexcept {
    ptr = other.release();
  }
  ~UniquePtr() {
    if (ptr) {
      delete ptr;
    }
    ptr = nullptr;
  }
  UniquePtr& operator=(std::nullptr_t) noexcept {
    if (ptr) {
      delete ptr;
    }
    ptr = nullptr;
    return *this;
  }
  UniquePtr& operator=(UniquePtr&& other) noexcept {
    reset(other.release());
    return *this;
  }

  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  const T& operator*() const noexcept {
    return *ptr;
  }
  T* operator->() const noexcept {
    return ptr;
  }
  T* get() const noexcept {
    return ptr;
  }
  T* release() noexcept {
    T* new_ptr = ptr;
    ptr = nullptr;
    return new_ptr;
  }
  void reset(T* new_ptr) noexcept {
    if (ptr) {
      delete ptr;
    }
    if (!new_ptr) {
      ptr = nullptr;
    }
    ptr = new_ptr;
  }
  void swap(UniquePtr& other) noexcept {
    std::swap(this->ptr, other.ptr);
  }
  explicit operator bool() const noexcept {
    return get() != nullptr;
  }

private:
  T* ptr;
};
