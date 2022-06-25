// По данной последовательности постройте дерево, запоминая для каждого
// элемента его значение и количество его повторений в последовательности.
#include <iostream>
#include <map>

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
            m_value_counts[key]++;
            insert(key, m_root);
        }
        else {
            m_root = new TreeNode;
            m_root->value = key;
            m_root->left = nullptr;
            m_root->right = nullptr;

            m_value_counts[key]++;
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


    void print_with_count()
    {
        for (auto&& element : m_value_counts)
        {
            std::cout << element.first << " " << element.second << std::endl;
        }
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
    std::map<int, int> m_value_counts;
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

    tree->print_with_count();

    delete tree;
    return 0;
}
