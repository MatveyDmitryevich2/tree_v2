#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

class Tree
{
    enum class Color
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
            right(nullptr), is_rthread(true), color(Color::red) {}
    };

    Node* root = nullptr;
    Node* head = nullptr;

    Node* GoToLeftMost(Node* node)
    {
        assert(node);

        while (!node->is_lthread) { node = node->left; }

        return node;
    }

    Node* Successor(Node* node)
    {
        assert(node);

        if (node->is_rthread) { return node->right; }
        else 
        {
            node = node->right;
            node = GoToLeftMost(node);
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
        if (root == nullptr)
        {
            head->left = head;
            head->is_lthread = true;
            return;
        }

        Node* iter_node = GoToLeftMost(root);

        while (iter_node != head)
        {
            Node* next_node = Successor(iter_node);
            delete iter_node;
            iter_node = next_node;
        }

        root = nullptr;
    }
    
    Node* RotateRight(Node* subroot)
    {
        assert(subroot);

        Node* y = subroot;
        if (y->is_lthread) { return subroot; }
        Node* x = y->left;
        
        if (x->is_rthread)
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
        assert(subroot);

        Node* y = subroot;
        if (y->is_rthread == true) { return subroot; }
        Node* x = y->right;
        
        if (x->is_lthread)
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

    const size_t PARENT = 2;
    const size_t GRAND_PARENT = 3;

    Color IsNodeColor(Node* node) { return node->color; }

    bool HistoryToPathHasFromTop(size_t i) { return (path_history.size() >= i); }
    Node* ElemFromTop(size_t i) { return (path_history[path_history.size() - i].first); }
    Node** UplinkFromTop(size_t i)
    { return (path_history[path_history.size() - i].second); }

    bool UncleExist()
    { return ((!ElemFromTop(GRAND_PARENT)->is_lthread)
              && (!ElemFromTop(GRAND_PARENT)->is_rthread)); }
    bool IsUncleLeft()
    { return (UplinkFromTop(PARENT) == &(ElemFromTop(GRAND_PARENT)->right)); }
    Node* SearchUncle()
    { return IsUncleLeft() ? ElemFromTop(GRAND_PARENT)->left
             : ElemFromTop(GRAND_PARENT)->right; }

    void Balancing()
    {
        for (;;)
        {
            if (!HistoryToPathHasFromTop(GRAND_PARENT))
            {
                if (HistoryToPathHasFromTop(PARENT))
                {
                    Node* parent = ElemFromTop(PARENT);
                    parent->color = Color::black;
                }

                break;
            }

            Node* grand_parent = ElemFromTop(GRAND_PARENT);
            Node* parent = ElemFromTop(PARENT);

            if (UncleExist() && (SearchUncle()->color == Color::red))
            {
                grand_parent->color = Color::red;
                parent->color = Color::black;
                SearchUncle()->color = Color::black;

                path_history.pop_back();
                path_history.pop_back();

                continue;
            }

            if (IsBranchInternalAndLeft())
            {
                grand_parent->left = RotateLeft(parent);
                Node* new_root = RotateRight(grand_parent);
                *UplinkFromTop(GRAND_PARENT) = new_root;

                new_root->color = Color::black;
                if (!new_root->is_rthread) new_root->right->color = Color::red;

                break;
            }

            else if (IsBranchInternalAndRight())
            {
                grand_parent->right = RotateRight(parent);
                Node* new_root = RotateLeft(grand_parent);
                *UplinkFromTop(GRAND_PARENT) = new_root;

                new_root->color = Color::black;
                if (!new_root->is_lthread) new_root->left->color = Color::red;

                break;
            }

            else if (IsBranchExternalAndLeft())
            {
                *UplinkFromTop(GRAND_PARENT) = RotateRight(grand_parent);
                
                parent->color = Color::black;
                grand_parent->color = Color::red;

                break;
            }

            else if (IsBranchExternalAndRight())
            {
                *UplinkFromTop(GRAND_PARENT) = RotateLeft(grand_parent);

                parent->color = Color::black;
                grand_parent->color = Color::red;

                break;
            }
        }
    }

    bool IsBranchInternalAndLeft()
    { return (!IsUncleLeft() && (ElemFromTop(PARENT)->right == ElemFromTop(1))); }
    bool IsBranchInternalAndRight()
    { return (IsUncleLeft()  && (ElemFromTop(PARENT)->left  == ElemFromTop(1))); }
    bool IsBranchExternalAndLeft()
    { return (!IsUncleLeft() && (ElemFromTop(PARENT)->left  == ElemFromTop(1))); }
    bool IsBranchExternalAndRight()
    { return (IsUncleLeft()  && (ElemFromTop(PARENT)->right == ElemFromTop(1))); }

    using HistoryToPath = typename std::vector<std::pair<Node*, Node**>>;
    HistoryToPath path_history;
    
    public:
    
    Tree()
    {
        head = new Node();
        head->left = head;
        head->right = head;
        head->color = Color::black;
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
            root->color = Color::black;
            return;
        }

        path_history.clear();
        path_history.push_back({node, &root});
        bool is_node_left = true;
        for (;;)
        {
            if (value < node->value)
            {
                if (node->is_lthread) { break; }
                path_history.push_back({node->left, &(node->left)});
                node = node->left;
            }
            else if (value > node->value)
            {
                if (node->is_rthread) { is_node_left = false; break; }
                path_history.push_back({node->right, &(node->right)});
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
            new_node->color = Color::red;
            path_history.push_back({new_node, &(node->left)});
        }

        else
        {
            Node* succ = node->right;
            Node* new_node = new Node(value);
            new_node->right = succ;
            new_node->left = node;
            node->is_rthread = false;
            node->right = new_node;
            new_node->color = Color::red;
            path_history.push_back({new_node, &(node->right)});
        }

        Balancing();
        root->color = Color::black;
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