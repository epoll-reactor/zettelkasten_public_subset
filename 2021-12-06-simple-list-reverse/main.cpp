#include <iostream>

class big_int {
    struct node {
        char c;
        node* next{nullptr};
        node* prev{nullptr};
    } *root_{nullptr}, *head_{nullptr};
    char sign;

public:
    big_int(std::string s) {
        sign = s.front();
        for (auto it = s.rbegin(); it != s.rend() - 1; ++it) {
            node* value = new node{*it};
            if (!root_) {
                root_ = value;
                head_ = root_;
            } else {
                head_->next = value;
                node* tmp_prev = head_;
                head_ = head_->next;
                head_->prev = tmp_prev;
            }
        }
    }

    std::string to_string_original() {
        std::string output;
        output += sign;
        node* walker = root_;
        while (walker) {
            output += walker->c;
            walker = walker->next;
        }
        return output;
    }

    std::string to_string() {
        std::string output;
        output += sign;
        node* walker = head_;
        while (walker) {
            output += walker->c;
            walker = walker->prev;
        }
        return output;
    }
};

int main()
{
    big_int i("+123456");
    std::cout << i.to_string_original() << std::endl;
    std::cout << i.to_string() << std::endl;
}
