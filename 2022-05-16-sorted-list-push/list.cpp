#include <iostream> // std::size_t, std::cout

template <typename T>
class List
{
public:
    ~List()
    {
        clear();
    }

    void non_sorted_push(const T& value);
    void sorted_push(const T& value);

    size_t size() const
    {
    	return size_;
    }
    
    bool empty() const
    {
        return head_ == nullptr;
    }

    void print()
    {
        Node* walker = head_;

        while (walker)
        {
            std::cout << walker->value << ' ';
            walker = walker->next;
        }
    }

    void clear()
    {
        Node *walker = head_;

        while (walker)
        {
      	    Node *killer = walker;
            walker = walker->next;
            delete killer;
        }
    }

private:
    struct Node
    {
        T value;
        Node* next = nullptr;
    };

    std::size_t size_{0};
    Node* head_{nullptr};
};

// ----------------------------///
// !!! Shit implementation !!! ///
// ----------------------------///

template <typename T>
void List<T>::non_sorted_push(const T &value)
{
    Node* new_node = new Node{value/*, next=nullptr*/};

    // Если пока нет ни одного элемента
    if (!head_)
    {
        ++size_;
        new_node->next = head_;
        head_ = new_node;
        return;
    }

    // Гуляем до конца списка
    Node* current = head_;
    while (current->next)
    {
        current = current->next;
    }

    // New_node - указатель на последний элемент (не нулевой)
    new_node->next = current->next;
    // И тоже самое, но наоборот, за current идёт new_node
    current->next = new_node;

    ++size_;
}

template <typename T>
void List<T>::sorted_push(const T& value)
{
    Node* new_node = new Node{value/*, next=nullptr*/};

    if (!head_ || head_->value <= new_node->value) // По убыванию
    // if (!head_ || head_->value > new_node->value) // По возрастанию
    {
        ++size_;
        new_node->next = head_;
        head_ = new_node;
        return;
    }

    Node* current = head_;
    while (current->next && current->next->value > new_node->value) // По убыванию
    // while (current->next && current->next->value <= new_node->value) // По возрастанию
    {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;

    ++size_;
}

int main()
{
    List<int> container;

    for (int i : {1, 9, 3, 2, 1, 4})
    {
        container.sorted_push(i);
    }

    container.print();
}
