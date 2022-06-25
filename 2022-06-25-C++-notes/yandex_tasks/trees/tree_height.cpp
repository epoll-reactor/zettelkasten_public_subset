// Реализуйте бинарное дерево поиска для целых чисел. Программа получает
// на вход последовательность целых чисел и строит из них дерево. Элементы
// в деревья добавляются в соответствии с результатом поиска их места. Если
// элемент уже существует в дереве, добавлять его не надо. Балансировка
// дерева не производится.
#include <iostream>

struct TreeNode
{
    int value;
    TreeNode* left;
    TreeNode* right;
};


class BinaryTree
{
public:
    BinaryTree()
        : m_root(nullptr)
    { }

    ~BinaryTree()
    {
        destroy_tree();
    }

    void insert(int key)
    {
        if (m_root)
        {
            insert(key, m_root);
        }
        else {
            m_root = new TreeNode;
            m_root->value = key;
            m_root->left = nullptr;
            m_root->right = nullptr;
        }
    }

    void destroy_tree()
    {
        destroy_tree(m_root);
    }

    int max_depth()
    {
        return max_depth(m_root);
    }

    int max_depth(TreeNode* leaf)
    {
        if (!leaf)
        {
            return 0;
        }
        else {
            int l_depth = max_depth(leaf->left);
            int r_depth = max_depth(leaf->right);

            if (l_depth > r_depth)
            {
                return l_depth + 1;
            }
            else {
                return r_depth + 1;
            }
        }
    }

private:
    void destroy_tree(TreeNode* leaf)
    {
        if (leaf)
        {
            destroy_tree(leaf->left);
            destroy_tree(leaf->right);
            delete leaf;
        }
    }

    void insert(int key, TreeNode* leaf)
    {
        if (key == leaf->value)
        {
            return;
        }
        if (key < leaf->value)
        {
            if (leaf->left)
            {
                insert(key, leaf->left);
            }
            else {
                leaf->left = new TreeNode;
                leaf->left->value = key;
                leaf->left->left = nullptr;
                leaf->left->right = nullptr;
            }
        }
        else if (key >= leaf->value)
        {
            if (leaf->right)
            {
                insert(key, leaf->right);
            }
            else {
                leaf->right = new TreeNode;
                leaf->right->value = key;
                leaf->right->left = nullptr;
                leaf->right->right = nullptr;
            }
        }
    }

    TreeNode* m_root;
};

int main()
{
    BinaryTree* tree = new BinaryTree;

    while (true)
    {
        int digit;
        std::cin >> digit;

        if (digit == 0)
        {
            break;
        }

        tree->insert(digit);
    }

    std::cout << tree->max_depth() << std::endl;

    delete tree;
    return 0;
}
