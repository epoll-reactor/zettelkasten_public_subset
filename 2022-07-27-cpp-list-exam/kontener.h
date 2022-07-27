#include <iostream>

// Skoro lista jesz szablonowa, .cpp plik nie za bardzo nam
// potrzebny (jezeli nie uzywamy extern template i innych
// rzeczy, wiec nie bede z tym bawil teraz).
template <typename T>
class list {
private:
    struct node_ {
        node_(node_ *next_, T &&value_)
            : next(next_), value(std::forward<T>(value_)) {}

        node_ *next;
        T      value;
    } *head_{nullptr};
    size_t size_ = 0;

    void ensure_not_empty() {
        if (!head_) {
            throw std::runtime_error("front() on empty list called");
        }
    }

public:
    list() = default;

    list(list &&rhs) : head_(rhs.head_), size_(rhs.size_) {
        rhs.head_ = nullptr;
        rhs.size_ = 0;
    }

    list(const list &rhs) : head_(nullptr), size_(0) {

        node_* runner = rhs.head_;
        while (runner) {
            push(runner->value);
            runner = runner->next;
        }
    }

    ~list() { clear(); }

    size_t size() const {
        return size_;
    }

    bool is_empty() const {
        return size_ == 0;
    }

    template <typename UniversalRef>
    void push(UniversalRef &&value) {
        node_ *node = new node_(nullptr, std::forward<T>(value));

        if (!head_) {
            head_ = node;
            head_->next = nullptr;
            ++size_;
            return;
        }

        node_ *runner = head_;
        while (runner->next) {
            runner = runner->next;
        }

        ++size_;
        runner->next = node;
    }

    template <typename UniversalRef>
    void sorted_push(UniversalRef &&value) {
        node_ *node = new node_(nullptr, std::forward<T>(value));

        if (!head_) {
            head_ = node;
            head_->next = nullptr;
            ++size_;
            return;
        }

        if (head_->value >= value) {
            node_ *tmp_head = head_;
            head_ = node;
            node = tmp_head;
        }

        node_ *runner = head_;
        while (runner->next && runner->value <= value) {
            std::cout << "[DEBUG]: Running through node " << runner << std::endl;
            runner = runner->next;
        }

        ++size_;
        runner->next = node;
    }

    const T &front() const {
        ensure_not_empty();
        return head_->value;
    }

    T &front() {
        ensure_not_empty();
        return head_->value;
    }

    void pop_front() {
        ensure_not_empty();
        node_ *killer = head_;
        head_ = head_->next;
        --size_;
        delete killer;
    }

    template <typename Executor>
    void for_each(Executor &&executor) const {
        node_ *runner = head_;
        while (runner) {
            executor(runner->value);
            runner = runner->next;
        }
    }

    void clear() {
        node_ *runner = head_;
        while (runner) {
            node_ *killer = runner;
            runner = runner->next;
            //std::cout << "[DEBUG]: delete at " << killer << ": " << killer->value << std::endl;
            delete killer;
        }
        size_ = 0;
    }

    template <typename __T>
    friend std::ostream &operator<<(std::ostream &stream, const list<__T> &list);
};
