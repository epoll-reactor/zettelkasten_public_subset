// Напишите свою упрощённую реализацию класса std::shared_ptr<T>.

// Подробную спецификацию стандартного std::shared_ptr можно посмотреть на
// http://en.cppreference.com/w/cpp/memory/shared_ptr. В этой задаче вам не
// нужно делать специализацию для массива и не нужно предусматривать свой Deleter.

// Назовите ваш класс SharedPtr. У класса должен быть один шаблонный
// параметр T – тип элемента, указатель на который будет храниться внутри.

// Напишите следующие функции в классе:

// 1.  Конструктор по умолчанию, создающий пустой умный указатель.
// 2.  Конструктор, принимающий T * и захватывающий владение этой динамической памятью.
// 3.  Конструктор копирования, принимающий на вход другой аналогичный SharedPtr и увеличивающий
//     счётчик ссылок на данный объект.
// 4.  Конструктор перемещения, получающий на вход rvalue-ссылку на другой SharedPtr и отбирающий у него владение ресурсом.
// 5.  Оператор присваивания, получающий на вход указатель. В результате счётчик ссылок на текущий объект должен
//     уменьшиться, а SharedPtr должен захватить владение переданным указателем.
// 6.  Оператор присваивания, получающий на вход другой SharedPtr. В результате счётчик ссылок на текущий объект
//     должен уменьшиться, а SharedPtr должен присоединиться к владению переданным указателем.
// 7.  Move-оператор присваивания, получающий на вход rvalue-ссылку на другой SharedPtr. Этот оператор должен немедленно
//     уничтожать ссылку на старый объект (аналогично стандартному std::shared_ptr)
// 8.  Деструктор.
// 9.  Константный и неконстантный оператор *.
// 10. Оператор ->.
// 11. Функцию void reset(T * ptr), после выполнения которой умный указатель должен уменьшить счётчик ссылок и захватить ptr.
// 12. Функцию void swap(SharedPtr& other), обменивающуюся содержимым с другим умным указателем.
// 13. Функцию T * get() const, возвращающую указатель.
// 14. explicit operator bool() const, позволяющий определить, не пуст ли умный указатель.
//
// Пометьте все подходящие функции тегом noexcept (чтобы подчеркнуть, что они не должны генерировать исключений).

// В вашей программе не должно быть утечек памяти. Объект, которым владеет SharedPtr, должен быть удален тогда и только
// тогда, когда будет уничтожен последний SharedPtr, владеющий им.

////////////////////////////////////////////////////////
////////////////////// SHARED PTR //////////////////////
////////////////////////////////////////////////////////

#include <utility>
#include <cassert>

template<typename T>
class SharedPtr {
public:
    SharedPtr() noexcept : ptr(nullptr), reference_count(nullptr) {}
    SharedPtr(T* ptr) : ptr(ptr), reference_count(nullptr) {
        acquire();
    }
    SharedPtr(const SharedPtr& other) : ptr(other.ptr),
      reference_count(other.reference_count) {
        acquire();
    }
    SharedPtr(SharedPtr&& other) : ptr(other.ptr),
      reference_count(other.reference_count) {
        other.ptr = nullptr;
        other.reference_count = nullptr;
    }

    SharedPtr& operator=(T* other) {
        if (ptr != other) {
            release();
            ptr = other;
            reference_count = nullptr;
            acquire();
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) {
        if (ptr != other.ptr) {
            Copy(std::forward<SharedPtr&&>(other));
            other.ptr = nullptr;
            other.reference_count = nullptr;
        }
        return *this;
    }
    SharedPtr& operator=(const SharedPtr& other) {
        if (ptr != other.ptr) {
            Copy(std::forward<const SharedPtr&>(other));
            acquire();
        }
        return *this;
    }

    ~SharedPtr() {
        if (ptr) {
            --(*reference_count);
            if (*reference_count == 0) {
                delete ptr;
                delete reference_count;
            }
        }
    }

    T* operator->() const noexcept {
        return ptr;
    }
    T& operator*() {
        return *ptr;
    }
    const T& operator*() const noexcept {
        return *ptr;
    }
    void reset(T* other) noexcept {
        *this = other;
    }

    void swap(SharedPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(reference_count, other.reference_count);
    }

    T* get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return reference_count != nullptr;
    }

    long use_count() const noexcept {
      return (reference_count) ? *reference_count : 0;
    }

private:
    T* ptr;
    long* reference_count;
    void acquire() noexcept {
        if (ptr != nullptr) {
          if (reference_count == nullptr) {
              reference_count = new long(0);
          }
          ++(*reference_count);
        }
    }
    void release() {
      if (ptr != nullptr) {
        --(*reference_count);
        if (*reference_count == 0) {
        delete ptr;
        delete reference_count;
        }
      }
    }
    template <typename _Ptr>
    void Copy(_Ptr&& other) {
        release();
        ptr = other.ptr;
        reference_count = other.reference_count;
    }
};

////////////////////////////////////////////////////////
//////////////////////    TESTS   //////////////////////
////////////////////////////////////////////////////////

#include <cstddef>
#include <iostream>
#include <memory>
#include <type_traits>

class Test {
private:
    static int counter;
    int id;
public:
    Test() : id(++counter) {
        std::cout << "Test() " << id << "\n";
    }
    Test(const Test& other) : id(++counter) {
        std::cout << "Test(const Test&) " << id << " " << other.id << "\n";
    }
    Test(Test&& other) : id(++counter) {
        std::cout << "Test(Test&&) " << id << " " << other.id << "\n";
    }
    ~Test() {
        std::cout << "~Test() " << id << "\n";
    }
    void foo() const {
        std::cout << "Test::foo() " << id << "\n";
    }
};

int Test::counter = 0;

int main() {
    using T = Test;
    //static_assert(sizeof(SharedPtr<T>) == sizeof(T*), "");
    static_assert(std::is_copy_constructible<SharedPtr<T>>::value, "");
    static_assert(std::is_move_constructible<SharedPtr<T>>::value, "");
    static_assert(std::is_constructible<SharedPtr<T>, std::nullptr_t>::value, "");
    static_assert(std::is_constructible<SharedPtr<T>, T*>::value, "");
    static_assert(!std::is_constructible<SharedPtr<T>, std::shared_ptr<T>>::value, "");
    static_assert(!std::is_assignable<SharedPtr<T>, std::shared_ptr<T>>::value, "");
    static_assert(std::is_assignable<SharedPtr<T>, std::nullptr_t>::value, "");
    static_assert(std::is_copy_assignable<SharedPtr<T>>::value, "");
    static_assert(std::is_move_assignable<SharedPtr<T>>::value, "");
    static_assert(std::is_destructible<SharedPtr<T>>::value, "");
    static_assert(!std::is_convertible<SharedPtr<T>, T*>::value, "");
    static_assert(!std::is_convertible<SharedPtr<T>, std::shared_ptr<T>>::value, "");
    static_assert(!std::is_convertible<std::shared_ptr<T>, SharedPtr<T>>::value, "");

    std::cout << "Step 1\n";
    SharedPtr<T> p(new T);

    std::cout << "Step 2\n";
    SharedPtr<T> p2(std::move(p));

    std::cout << "Step 3\n";
    SharedPtr<T> p3;
    std::cout << bool(p3) << std::endl;

    std::cout << "Step 4\n";
    p3 = std::move(p2);

    {
        SharedPtr<int> up1(new int(42));
        SharedPtr<int> up2(new int(17));
        up2 = std::move(up1);
    }

    std::cout << "Step 5\n";
    (*p3).foo();
    p3->foo();
    const auto& cref = p3;
    (*cref).foo();
    cref->foo();

    std::cout << "Step 6\n";
    T * ptr21 = p2.get();
    T * ptr31 = p3.get();
    std::cout << (ptr21 != ptr31) << "\n";
    p2.swap(p3);
    T * ptr22 = p2.get();
    T * ptr32 = p3.get();
    std::cout << (ptr22 == ptr31) << "\n";
    std::cout << (ptr21 == ptr32) << "\n";

    std::cout << "Step 7\n";
    std::cout << bool(p2) << "\n";
    T * np = nullptr;
    p2.reset(np);
    std::cout << bool(p2) << "\n";

    std::cout << "Step 8\n";
    {
        SharedPtr<T> p4(new T);
        p4.reset(new T);
    }

    std::cout << "Step 9\n";
    {
      SharedPtr<T> p1(new T);
      SharedPtr<T> p2(p1);
      SharedPtr<T> p3(nullptr);
      std::cout << bool(p3) << std::endl;
      p3 = p1;
      SharedPtr<T> p4(new T);
      p4 = p2;

      p1->foo();
      p2->foo();
      p3->foo();
      p4->foo();
      std::cout << (p1.get() == p2.get()) << std::endl;
      std::cout << (p2.get() == p3.get()) << std::endl;
      std::cout << (p3.get() == p4.get()) << std::endl;
    }

    std::cout << "Step 10\n";
    {
      SharedPtr<T> p1(new T);
      SharedPtr<T> p2(p1);
      p1.reset(new T);
      p1->foo();
      p2->foo();
      p1 = std::move(p2);
      p1->foo();
      std::cout << bool(p2) << std::endl;
    }

    std::cout << "Step 11\n";
    {
      SharedPtr<T> p1(new T);
      SharedPtr<T> p2(new T);
      SharedPtr<T> p3(p2);
      p2 = std::move(p1);
      p2->foo();
      p3->foo();
    }

    std::cout << "Step 12\n";
    {
      SharedPtr<T> p1(new T);
      SharedPtr<T> p2(new T);
      p2 = std::move(p1);
      p2->foo();
    }

    std::cout << "Step 13\n";
    {
      SharedPtr<T> p1(new T);
      p1 = p1;
      p1->foo();
      p1 = std::move(p1);
      p1->foo();
    }

    std::cout << "End\n";
}
