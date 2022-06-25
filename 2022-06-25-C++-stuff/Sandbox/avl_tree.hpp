// Aug 6, 2021
#ifndef SANDBOX_AVL_TREE_HPP
#define SANDBOX_AVL_TREE_HPP

#include <iostream>
#include <sstream>
#include <iterator>

class binary_tree
{
    struct node
    {
        int value;
        node* left;
        node* right;
    };

public:
    binary_tree() noexcept : m_root(nullptr) {}
    ~binary_tree() { destroy_tree(m_root); }

    void insert(int key)
    {
        if (m_root) {
            insert(key, m_root);
        } else {
            m_root = new node;
            m_root->value = key;
            m_root->left = nullptr;
            m_root->right = nullptr;
        }
    }

    bool is_avl()
    {
        return is_avl(m_root);
    }

private:
    int max_depth(node* leaf)
    {
        if (!leaf) {
            return 0;
        } else {
            int l_depth = max_depth(leaf->left);
            int r_depth = max_depth(leaf->right);

            if (l_depth > r_depth) {
                return l_depth + 1;
            } else {
                return r_depth + 1;
            }
        }
    }

    bool is_avl(node* leaf)
    {
        int left;
        int right;

        if (!leaf) { return true; }

        left = max_depth(leaf->left);
        right = max_depth(leaf->right);

        if (abs(left - right) <= 1 && is_avl(leaf->left) && is_avl(leaf->right)) {
            return true;
        }
        return false;
    }

    void destroy_tree(node* leaf)
    {
        if (leaf) {
            destroy_tree(leaf->left);
            destroy_tree(leaf->right);
            delete leaf;
        }
    }

    void insert(int key, node* leaf)
    {
        if (key == leaf->value) { return; }

        if (key < leaf->value) {
            if (leaf->left) {
                insert(key, leaf->left);
            } else {
                leaf->left = new node;
                leaf->left->value = key;
                leaf->left->left = nullptr;
                leaf->left->right = nullptr;
            }
        } else if (key >= leaf->value) {
            if (leaf->right) {
                insert(key, leaf->right);
            } else {
                leaf->right = new node;
                leaf->right->value = key;
                leaf->right->left = nullptr;
                leaf->right->right = nullptr;
            }
        }
    }

    node* m_root;
};

void avl_tree_test()
{
    binary_tree tree;

    std::istringstream expression("100 50 150 25 75 125 175 65 85");
    [&tree]<typename Iterator>(Iterator&& begin, Iterator&& end) {
        for (; begin != end; ++begin) {
            tree.insert(std::stoi(*begin));
        }
    }(std::istream_iterator<std::string>(expression), {});

    std::cout << "is avl? " << (tree.is_avl() ? "YES" : "NO") << std::endl;
}

#endif //SANDBOX_AVL_TREE_HPP
