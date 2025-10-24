#pragma once

#include <cassert>
#include <cstddef>

class Tree
{
    enum Color
    {
        red,
        black
    };

    struct Node
    {
        int value;

        Node* left;
        bool is_lthread;

        Node* right;
        bool is_rthread;

        Color color;

        Node(int val = 0)
            : value(val), left(nullptr), is_lthread(true),
            right(nullptr), is_rthread(true) {}
    };

    Node* root = nullptr;
    Node* head = nullptr;

    Node* GoToLeftmost(Node* node)
    {
        assert(node);

        while (node->is_lthread != true) { node = node->left; }

        return node;
    }

    Node* Successor(Node* node)
    {
        assert(node);

        if (node->is_rthread == true) { return node->right; }
        else 
        {
            node = node->right;
            node = GoToLeftmost(node);
            return node;
        }
    }

    Node* LowerBound(int min)
    {
        Node* node = root;
        for (;;)
        {
            if (node->value == min) { return node; }
            else if (node->value > min)
            {
                if (node->is_lthread) { return node; }
                node = node->left;
            }
            else if (node->value < min)
            {
                if (node->is_rthread) { return Successor(node); }
                node = node->right;
            }
        }
    }

    void ClearTree()
    {
        if(root == nullptr)
        {
            head->left = head;
            head->is_lthread = true;
            return;
        }

        Node* iter_node = root->right;
        iter_node = GoToLeftmost(iter_node);

        while (iter_node != head)
        {
            Node* next_node = Successor(iter_node);
            delete iter_node;
            iter_node = next_node;
        }

        delete root;
        root = nullptr;
    }
    
    Node* RotateRight(Node* subroot)
    {
        Node* y = subroot;
        if (y->is_lthread == true) { return subroot; }
        Node* x = y->left;
        
        if (x->is_rthread == true)
        {
            x->right = y;
            x->is_rthread = false;
            
            y->left = x;
            y->is_lthread = true;

            return x;
        }
        
        else
        {
            y->left = x->right;
            y->is_lthread = false;
            
            x->right = y;
            x->is_rthread =  false;
            
            return x;
        }
    }
    
    Node* RotateLeft(Node* subroot)
    {
        Node* y = subroot;
        if (y->is_rthread == true) { return subroot; }
        Node* x = y->right;
        
        if (x->is_lthread == true)
        {
            x->left = y;
            x->is_lthread = false;
            
            y->right = x;
            y->is_rthread = true;

            return x;
        }
        
        else
        {
            y->right = x->left;
            y->is_rthread = false;
            
            x->left = y;
            x->is_lthread = false;
            
            return x;
        }
    }
    
    public:
    
    Tree()
    {
        head = new Node();
        head->left = head;
        head->right = head;
    }
    
    ~Tree()
    {
        ClearTree();
        delete head;
    }
    
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    void Insert(int value)
    {
        Node* node = root;
        
        if (root == nullptr)
        {
            root = new Node(value);
            root->left = head;
            root->right = head;
            head->left = root;
            head->is_lthread = false;
            return;
        }

        bool is_node_left = true;
        for (;;)
        {
            if (value < node->value)
            {
                if (node->is_lthread) { break; }
                node = node->left;
            }
            else if (value > node->value)
            {
                if (node->is_rthread) { is_node_left = false; break; }
                node = node->right;
            }
        }

        if (is_node_left)
        {
            Node* pred = node->left;
            Node* new_node = new Node(value);
            new_node->left = pred;
            new_node->right = node;
            node->is_lthread = false;
            node->left = new_node;
        }

        else
        {
            Node* succ = node->right;
            Node* new_node = new Node(value);
            new_node->right = succ;
            new_node->left = node;
            node->is_rthread = false;
            node->right = new_node;
        }
    }

    std::size_t RangeQuery(int min, int max)
    {
        if (root == nullptr) { return 0; }

        std::size_t count = 0;
        for (Node* iter_node = LowerBound(min);
             iter_node != head && iter_node->value <= max;
             count++)
        {
            iter_node = Successor(iter_node);
        }

        return count;
    }

};