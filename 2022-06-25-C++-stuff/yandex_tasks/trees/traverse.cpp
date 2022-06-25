// Выведите все элементы полученного дерева в порядке возрастания.
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

    void print_in_ascending_order()
    {
        print_in_order(m_root);
    }


private:
    void print_in_order(TreeNode* leaf)
    {
        if (!leaf)
        {
            return;
        }

        print_in_order(leaf->left);

        std::cout << leaf->value << std::endl;

        print_in_order(leaf->right);
    }

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

    tree->print_in_ascending_order();

    delete tree;
    return 0;
}
