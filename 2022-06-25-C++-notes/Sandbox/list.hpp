#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>


class List
{
public:
    List() = default;

    List(const List&);

    ~List();

    void push(int value);
    void pop();
    int pop_one();
//    void reverse();
    void clear();
//    int front() const;
//    int back() const;
    int size() const;
    void print() const;
    bool empty() const;

//    List& operator=(const List&);
//    bool operator==(const List&) const;

private:
    struct Node
    {
        int value;
        Node* next = nullptr;
    };

    std::size_t m_counter = 0;
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
};

#include <random>

inline void run()
{
    std::cout << "Podaj liczby do listy, aby skonczyc, podaj -1" << std::endl;
    List l;

    while (true)
    {
        int user_value;
        std::cin >> user_value;
        if (user_value == -1)
            break;
        l.push(user_value);
    }

    while (!l.empty())
    {
        l.pop();
    }

    l.print();
}

#endif // LIST_HPP
