// Дерево называется АВЛ-сбалансированным, если для любой его вершины
// высота левого и правого поддерева для этой вершины различаются не более чем на 1.
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

    bool check_if_avl()
    {
        return check_if_avl(m_root);
    }


private:
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

    bool check_if_avl(TreeNode* leaf)
    {
        int left;
        int right;

        if (!leaf)
        {
            return 1;
        }

        left = max_depth(leaf->left);
        right = max_depth(leaf->right);

        if (abs(left - right) <= 1 && check_if_avl(leaf->left) && check_if_avl(leaf->right))
        {
            return true;
        }

        return false;
    }

    void print_in_order(TreeNode* leaf)
    {
        if (!leaf)
        {
            return;
        }

        print_in_order(leaf->left);

        if ((!leaf->left && leaf->right) || (leaf->left && !leaf->right))
        {
            std::cout << leaf->value << std::endl;
        }

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

    std::cout << (tree->check_if_avl() ? "YES" : "NO") << std::endl;

    delete tree;
    return 0;
}
