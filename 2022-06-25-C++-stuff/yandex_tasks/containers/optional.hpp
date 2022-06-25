// Напишите свою реализацию класса std::optional (https://en.cppreference.com/w/cpp/utility/optional),
// который может либо хранить элемент, либо хранить метку об отсутствии элемента.

// Вот заготовка для вашего класса: https://clck.ru/FLoZE. Вам нужно вписать сюда реализацию функций класса
// и сдать класс с функциями в систему.

// Обратите внимание, что Optional, в отличие от unique_ptr, должен хранить данные на стеке, а не в
// динамической памяти. Эти данные нужно проинициализировать только при необходимости (по умолчанию
// Optional создаётся пустым). Для этого нужно использовать конструкцию placement new - явное указание
// компилятору создать объект в указанном месте памяти. Например, сконструировать новый объект типа T
// в блоке памяти data можно так: new (data) T(). За временем жизни такого объекта придётся следить
// самостоятельно. Сама сырая память data в данном случае будет удалена автоматически, но деструктор
// придётся позвать вручную: reinterpret_cast<T*>(data)->T(). Запомните: конструирование объекта с
// помощью placement new в уже имеющейся памяти, как в этой задаче, - это единственный случай, когда
// которого нужно вызывать деструктор самостоятельно.

// Пытайтесь не создавать лишние объекты типа T зря. Например, при присваивании нового объекта уже
// заполненному Optional просто вызовите соответствующий оператор присваивания для типа T.

// Вот код нашей тестовой программы, который вам поможет в отладке: https://clck.ru/FLoZo. Вы можете
// сравнить её вывод для вашего класса и для стандартного std::optional.

// Заметим, что настоящий std::optional реализован сложнее. Просто конструировать объект в буфере байт
// на стеке он не может: тогда такой std::optional нельзя было бы использовать в constexpr-выражениях.
// Вместо этого в стандартном std::optional применяется union. Однако в этой задаче вполне достаточно
// использовать обычный буфер.

// Не пытайтесь использовать в своём решении стандартный std::optional: это не скомпилируется.

#include <utility>

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

    T* get_data() {
        return reinterpret_cast<T*>(data);
    }

    const T* get_data() const {
        return reinterpret_cast<const T*>(data);
    }

    void initialize() {
        if (!defined) {
            new (data) T;
            defined = true;
        }
    }

    void copy_data(const T& elem) {
      if (!defined) {
          new (data) T(elem);
          defined = true;
      } else {
        *get_data() = elem;
      }
    }

    void move_data(T && elem) {
        if (!defined) {
            new (data) T(std::forward<T>(elem));
            defined = true;
        }
    }

    void copy(const Optional& other) {
        if (other.defined)
            copy_data(*other);
        else
            reset();
    }

    void move(Optional && other) {
        if (other.defined)
            move_data(*other);
        else
            reset();
    }

    void ensure_defined() const {
        if (!defined)
            throw ::BadOptionalAccess();
    }

public:
    Optional() = default;

    Optional(const T& elem) {
        copy_data(elem);
    }

    Optional(T && elem) {
        move_data(std::forward<T>(elem));
    }

    Optional(const Optional& other) {
        copy(other);
    }

    Optional& operator=(const Optional& other) {
        if (this != &other) {
            copy(other);
        }
        return *this;
    }

    Optional& operator=(const T& elem) {
        copy_data(elem);
        return *this;
    }

    Optional& operator=(T&& elem) {
        if (!defined) {
            move_data(std::forward<T>(elem));
        } else {
            *get_data() = std::move(elem);
        }
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        ensure_defined();
        return *get_data();
    }

    const T& operator*() const {
        ensure_defined();
        return *get_data();
    }

    T* operator->() {
        ensure_defined();
        return get_data();
    }
    const T* operator->() const {
        ensure_defined();
        return get_data();
    }

    T& value() {
        ensure_defined();
        return *get_data();
    }

    const T& value() const {
        ensure_defined();
        return *get_data();
    }

    void reset() {
        if (defined) {
            get_data()->~T();
            defined = false;
        }
    }

    ~Optional() {
        reset();
    }
};
