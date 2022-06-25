#include "list.hpp"



//List::List(const List& rhs)
//{
//    Node* walker = rhs.m_head;

//    while (walker != nullptr)
//    {
//        Node* creator = new Node{walker->value};

//        if (m_tail != nullptr)
//        {
//            m_tail->next = creator;
//        }
//        else {
//            m_head = creator;
//        }
//        m_tail = creator;
//        walker = walker->next;
//    }
//    m_counter = rhs.m_counter;
//}
// =======================================
//    Node* creator = new Node{value};

//    if (m_tail != nullptr)
//    {
//        m_tail->next = creator;
//    }
//    else {
//        m_head = creator;
//    }

//    m_tail = creator;
//    --m_counter;

List::~List()
{
    clear();
}

void List::push(int value)
{
    Node* newNode = new Node{value};
    if (m_head == NULL || (*m_head).value <= newNode->value)
    {
        newNode->next = m_head;
        m_head = newNode;
        return;
    }

    struct Node* current = m_head;
    while (current->next != NULL && current->next->value > newNode->value) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    --m_counter;
}

void List::pop()
{
    if (m_counter == 0)
    {
        throw std::logic_error("Empty list.");
    }

    Node* walker = m_head;

    while (walker != nullptr)
    {
        Node* walker2 = walker->next;

        if (walker2 == nullptr)
        {
            break;
        }

        Node* walker3 = walker2->next;

        if (walker3 == nullptr)
        {
            break;
        }

        if ((((walker->value + walker3->value) / 2) == walker2->value))
        {
            walker->next = walker3;
            delete walker2;
            --m_counter;
        }
        else {
            pop_one();
            pop_one();
            pop_one();
        }

        walker = walker->next;
    }

}


int List::pop_one()
{
    if (m_counter == 0)
    {
        throw std::logic_error("Empty list.");
    }

    Node* killer = m_head;
    int stored_value = killer->value;
    m_head = killer->next;

    if (m_tail == killer)
    {
        m_tail = nullptr;
    }

    delete killer;
    --m_counter;

    return stored_value;
}

//void List::reverse()
//{
//    Node* begin = nullptr;
//    Node* end = nullptr;
//    Node* walker = m_head;

//    while (walker != nullptr)
//    {
//        Node* mover = walker;
//        walker = walker->next;
//        m_head = walker;

//        if (m_tail == mover)
//        {
//            m_tail = nullptr;
//        }
//        mover->next = begin;
//        begin = mover;

//        if (end == nullptr)
//        {
//            end = mover;
//        }
//    }

//    m_head = begin;
//    m_tail = end;
//}

void List::clear()
{
    while (m_head != nullptr)
    {
        Node* killer = m_head;
        m_head = killer->next;
        delete killer;
    }
    m_tail = nullptr;
    m_counter = 0;
}

//int List::front() const
//{
//    if (m_counter == 0)
//        throw std::logic_error("Empty list.");

//    return m_head->value;
//}

//int List::back() const
//{
//    if (m_counter == 0)
//        throw std::logic_error("Empty list.");

//    return m_tail->value;
//}

int List::size() const
{
    return m_counter;
}

void List::print() const
{
    Node* walker = m_head;

    while (walker != nullptr)
    {
        std::cout << walker->value << " ";
        walker = walker->next;
    }

    if (m_counter == 0)
        std::cout << "Empty";

    std::cout << std::endl;
}

bool List::empty() const
{
    return m_counter == 0;
}

//bool List::operator==(const List& rhs) const
//{
//    if (m_counter != rhs.m_counter)
//        return false;

//    Node* this_walker = m_head;
//    Node* rhs_walker = rhs.m_head;

//    while (this_walker != nullptr)
//    {
//        if (this_walker->value != rhs_walker->value)
//            return false;

//        this_walker = this_walker->next;
//        rhs_walker = rhs_walker->next;
//    }

//    return true;
//}


