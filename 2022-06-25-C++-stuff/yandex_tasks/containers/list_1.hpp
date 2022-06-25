// Реализуйте свой класс List, аналогичный std::list<int> (или std::deque<int>).
// Ваш класс должен определять функции push_back, push_front, pop_back, pop_front,
// size и begin/end, а также конструктор без параметров, оператор присваивания и
// деструктор. Функции begin и end должны возвращать итераторы, для которых определены
// операторы ++, -- и унарный * (разыменование), а также операторы сравнения == и !=.

// Использовать контейнеры стандартной библиотеки запрещается.
#include <cstddef>

class List {
private:
    struct Node {
        int value;
        Node* prev, * next;
        Node(int value, Node* prev, Node* next) :
            value(value), prev(prev), next(next) {}
    } *head, * tail;
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
    void push_back(int elem) {
        if (length == 0) {
            head = new Node(elem, nullptr, nullptr);
            tail = head;
        } else {
            tail->next = new Node(elem, tail, nullptr);
            tail = tail->next;
        }
        ++length;
    }
    void push_front(int elem) {
        if (length == 0) {
            tail = new Node(elem, nullptr, nullptr);
            head = tail;
        } else {
            head->prev = new Node(elem, nullptr, head);
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
        int operator*() const {
            return node->value;
        }
        bool operator==(const ConstIterator& other) const {
            return (node == other.node && list == other.list);
        }
        bool operator!=(const ConstIterator& other) const {
            return (node != other.node || list != other.list);
        }
    };
    ConstIterator begin() const {
        return ConstIterator(this, head);
    }
    ConstIterator end() const {
        return ConstIterator(this, tail->next);
    }
};
