// Сделайте класс List из предыдущей задачи шаблонным. Параметром шаблона должен
// быть тип хранимого в списке элемента. Ваш класс должен определять функции
// push_back, push_front, pop_back, pop_front, константные size и begin/end,
// а также конструктор без параметров, конструктор копирования, оператор
// присваивания и деструктор. Функции begin и end должны возвращать итераторы,
// для которых определены операторы ++, -- и унарный * (разыменование), а также
// операторы сравнения == и !=.

// Использовать контейнеры стандартной библиотеки запрещается.
#include <cstddef>
#include <utility>

template <typename T>
class List {
private:
    struct Node {
        T value;
        Node* prev, * next;
        Node(const T& value, Node* prev, Node* next)
          : value(std::forward<decltype(value)>(value)), prev(prev), next(next) {}
    };
    Node *head, *tail;
    size_t length;

public:
    List() : head(nullptr), tail(nullptr), length(0)
    { }
    List& operator=(const List& other) {
        while (length > 0) {
            pop_back();
        }
        for (auto el : other) {
            push_back(el);
        }
        return *this;
    }
    void push_back(const T& elem) {
        if (length == 0) {
            head = new Node(
              std::forward<decltype(elem)>(elem), nullptr, nullptr);
            tail = head;
        } else {
            tail->next = new Node(
              std::forward<decltype(elem)>(elem), tail, nullptr);
            tail = tail->next;
        }
        ++length;
    }
    void push_front(const T& elem) {
        if (length == 0) {
            tail = new Node(
              std::forward<decltype(elem)>(elem), nullptr, nullptr);
            head = tail;
        } else {
            head->prev = new Node(
              std::forward<decltype(elem)>(elem), nullptr, head);
            head = head->prev;
        }
        ++length;
    }
    void pop_back() {
        Node* temp = tail;
        tail = tail->prev;
        if (length == 1) {
            head = nullptr;
        } else {
            tail->next = nullptr;
        }
        --length;
        delete temp;
    }
    void pop_front() {
        Node* temp = head;
        head = head->next;
        if (length == 1) {
            tail = nullptr;
        } else {
            head->prev = nullptr;
        }
        --length;
        delete temp;
    }
    size_t size() const {
        return length;
    }
    ~List() {
        while (length > 0) {
            pop_back();
        }
    }

    template <typename _T>
    class ConstIterator {
    private:
        const List* list;
        Node* node;

    public:
        ConstIterator(const List* value, Node* i) : list(value), node(i) {}
        ConstIterator& operator++() {
            node = node->next;
            return *this;
        }
        ConstIterator& operator--() {
            node = (node != nullptr ? node->prev : list->tail);
            return *this;
        }
        const _T& operator*() const {
            return node->value;
        }
        _T& operator*() {
            return node->value;
        }
        bool operator==(const ConstIterator& other) const {
            return (node == other.node && list == other.list);
        }
        bool operator!=(const ConstIterator& other) const {
            return (node != other.node || list != other.list);
        }
    };
    ConstIterator<T> begin() const {
        return ConstIterator<T>(this, head);
    }
    ConstIterator<T> end() const {
        return ConstIterator<T>(this, tail->next);
    }
};
