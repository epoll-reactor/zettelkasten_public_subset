// Jun 19, 2021
#ifndef SANDBOX_EXAM_LIST_HPP
#define SANDBOX_EXAM_LIST_HPP

#include <bits/stdc++.h>

template <typename T>
class List
{
public:
    using value_type = T;

    List() = default;

    void non_sorted_push(const T& value);
    void sorted_push(const T& value);

//    void push_back(const T& value);
//    void pop();
    T pop_one();
    void pop_different(std::size_t count);
    void change();

    size_t size() const noexcept;
    bool empty() const noexcept;

    void print() const;

//    bool operator==(const List<T>& other) const;

private:
    struct Node
    {
        T value;
        Node* next = nullptr;
    };

    std::size_t m_size = 0;
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
};

template <typename T>
size_t List<T>::size() const noexcept
{
    return m_size;
}

template <typename T>
bool List<T>::empty() const noexcept
{
    return m_head == m_tail;
}

//template <typename T>
//bool List<T>::operator==(const List<T> &other) const
//{
//    if (this == &other)
//        return true;
//
//    if (m_size != other.m_size)
//        return false;
//
//    Node* this_head = m_head;
//    Node* other_head = other.m_head;
//    Node* this_tail = m_tail;
//    Node* other_tail = other.m_tail;
//
//    while (this_head != this_tail)
//    {
//        if (this_head->value != other_head->value)
//        {
//            return false;
//        }
//
//        this_head = this_head->next;
//        other_head = other_head->next;
//    }
//
//    return true;
//}

template <typename T>
void List<T>::non_sorted_push(const T &value)
{
    Node* newNode = new Node{value};

    if (!m_head)
    {
        ++m_size;
        newNode->next = m_head;
        m_head = newNode;
        return;
    }

    Node* current = m_head;
    while (current->next)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    ++m_size;
}

template <typename T>
void List<T>::sorted_push(const T& value)
{
    Node* newNode = new Node{value};

//    if (!m_head || m_head->value <= newNode->value)
    if (!m_head || m_head->value > newNode->value)
    {
        ++m_size;
        newNode->next = m_head;
        m_head = newNode;
        return;
    }

    Node* current = m_head;
//    while (current->next && current->next->value > newNode->value)
    while (current->next && current->next->value <= newNode->value)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    ++m_size;
}

//template <typename T>
//void List<T>::pop()
//{
//    if (m_size == 0)
//    {
//        throw std::runtime_error("Attempt to pop empty list");
//    }
//
//    Node* walker = m_head;
//
//    while (walker)
//    {
//        Node* walker2 = walker->next;
//
//        if (!walker2)
//        {
//            break;
//        }
//
//        Node* walker3 = walker2->next;
//
//        if (!walker3)
//        {
//            break;
//        }
//
//        if ((((walker->value + walker3->value) / 2) == walker2->value))
//        {
//            walker->next = walker3;
//            delete walker2;
//            --m_size;
//        }
//        else {
//            pop_one();
//            pop_one();
//            pop_one();
//        }
//
//        walker = walker->next;
//    }
//}


/// Параметр K - оставляем K элементов или, если меньше то столько, сколько есть
/// Удалить самую длинную последовательность различающихся чисел
/// Затем пропускаем K элементов
/// Повторяем до конца списка ^

//template <typename T>
//void List<T>::pop_different(std::size_t count)
//{
//    Node* this_head = m_head;
//    Node* this_tail = m_tail;
//
//    auto has_next = [](Node* node) noexcept {
//        return static_cast<bool>(node->next);
//    };
//
//    while (this_head != this_tail)
//    {
//        std::size_t temporary_count = count;
//
//        if (temporary_count <= m_size)
//        {
//            for (std::size_t runner = 0; has_next(this_head) && runner < temporary_count; runner++)
//            {
//                this_head = this_head->next;
//            }
//        }
//
//        if (has_next(this_head))
//        {
//            do
//            {
//                Node* killer = this_head->next;
//                this_head->next = this_head->next->next;
//                delete killer;
//
//                --m_size;
//
//            } while (this_head->value == this_head->next->value);
//        }
//
//        this_head = this_head->next;
//    }
//}


template <typename T>
void List<T>::pop_different(std::size_t count)
{
    Node* this_head = m_head;
    Node* this_tail = m_tail;

    auto has_next = [](Node* node) noexcept {
        return static_cast<bool>(node->next);
    };

    while (this_head != this_tail)
    {
        if (has_next(this_head) && has_next(this_head->next))
        {
            size_t count_copy = count;

            if (count_copy <= m_size)
            {
                for (size_t runner = 0; has_next(this_head) && runner < count_copy - 1; runner++)
                {
                    this_head = this_head->next;
                }
            }

            do
            {
                Node* killer = this_head->next;
                this_head->next = this_head->next->next;
                delete killer;
                --m_size;
            } while (has_next(this_head) && has_next(this_head->next) && this_head->next->value != this_head->next->next->value);
        }

        this_head = this_head->next;
    }
}

template <typename T>
void List<T>::change()
{
    Node* this_head = m_head;
    Node* this_tail = m_tail;

    auto three_elements_available = [](Node* node) noexcept {
        return node && node->next && node->next->next;
    };

    while (this_head != this_tail)
    {
        if (three_elements_available(this_head))
        {
            Node* first_node = this_head;
            Node* second_node = this_head->next;
            Node* third_node = this_head->next->next;

            std::cout << "\n(" << first_node->value << ", " << second_node->value << ", " << third_node->value << ")";

            if ((first_node->value + second_node->value + third_node->value) % 2 == 0)
            {
                first_node->next = third_node;
                int second_value = second_node->value;
                delete second_node;

                while (first_node != this_tail)
                {
                    first_node = first_node->next;
                }

                first_node = new Node{second_value};
            }

            this_head = this_head->next;
            this_head = this_head->next;
            this_head = this_head->next;
        }
        else {
            this_head = this_head->next;
        }
    }
}

template <typename T>
void List<T>::print() const
{
    Node* walker = m_head;

    while (walker != m_tail)
    {
        std::cout << walker->value << ' ';
        walker = walker->next;
    }
}

//class Line : public std::string
//{
//    friend std::istream& operator>>(std::istream& in, Line& line)
//    {
//        return std::getline(in, line);
//    }
//};
//
//template <typename Iterable>
//void read_lines(std::istream& in, Iterable&& iterator)
//{
//    using iterator_t = std::istream_iterator<Line>;
//    std::copy(iterator_t(in), iterator_t(), iterator);
//}

//template<std::size_t... Ids>
//void run_list_test()
//{
//    static constexpr std::array<std::size_t, sizeof...(Ids)> args = { { Ids... } };
//    List<int> container;
//
//    for (int i : args)
//    {
//        container.sorted_push(i);
//    }
//
//    std::cout << "Source:  ";
//    container.print();
//
//    container.change();
//
//    std::cout << "\nResult:  ";
//    container.print();
//    std::cout << '\n';
//}

void run_list_tests()
{
  List<int> container;

  for (int i : {1, 9, 3, 2, 1, 4})
  {
    container.sorted_push(i);
  }

  container.print();

//    run_list_test<9,7,6,5,4,3,1,1,1,1>();
//    run_list_test<9,8,7,6,6,5,5,4,4,4,3,3,2,1,1,1,1>();
}

#endif //SANDBOX_EXAM_LIST_HPP
