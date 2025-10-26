#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

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
            right(nullptr), is_rthread(true), color(red) {}
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

        Node* iter_node = GoToLeftmost(root);

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

    Color IsColorLeftChild(Node* node) { return (node->is_lthread == true) ? black : node->left->color; }
    Color IsColorRightChild(Node* node) { return (node->is_rthread == true) ? black : node->right->color; }
    Color IsNodeColor(Node* node) { return node->color; }

    bool HistoryToPathHasFromTop(size_t i) { return (history_to_path.size() >= i); }
    Node* ElemFromTop(size_t i) { return (history_to_path[history_to_path.size() - i].first); }
    Node** UplinkFromTop(size_t i) { return (history_to_path[history_to_path.size() - i].second); }

    bool UncleExist() { return ((!ElemFromTop(3)->is_lthread) && (!ElemFromTop(3)->is_rthread)); }
    bool IsUncleLeft() { return (UplinkFromTop(2) == &(ElemFromTop(3)->right)); }
    Node* SearchUncle() { return IsUncleLeft() ? ElemFromTop(3)->left : ElemFromTop(3)->right; }

    void Balancing()
    {
        for(;;)
        {
            if(!HistoryToPathHasFromTop(3))
            {
                if(HistoryToPathHasFromTop(2))
                {
                    Node* parent = ElemFromTop(2);
                    parent->color = black;
                }

                break;
            }

            Node* grand_parent = ElemFromTop(3);
            Node* parent = ElemFromTop(2);

            if(UncleExist() && (SearchUncle()->color == red))
            {
                grand_parent->color = red;
                parent->color = black;
                SearchUncle()->color = black;

                history_to_path.pop_back();
                history_to_path.pop_back();

                continue;
            }

            if(IsBranchInternalAndLeft())
            {
                grand_parent->left = RotateLeft(parent);
                Node* new_root = RotateRight(grand_parent);
                *UplinkFromTop(3)= new_root;

                new_root->color = black;
                if (!new_root->is_rthread) new_root->right->color = red;

                break;
            }

            else if(IsBranchInternalAndRight())
            {
                grand_parent->right = RotateRight(parent);
                Node* new_root = RotateLeft(grand_parent);
                *UplinkFromTop(3) = new_root;

                new_root->color = black;
                if (!new_root->is_lthread) new_root->left->color = red;

                break;
            }

            else if(IsBranchExternalAndLeft())
            {
                *UplinkFromTop(3)= RotateRight(grand_parent);
                
                parent->color = black;
                grand_parent->color = red;

                break;
            }

            else if(IsBranchExternalAndRight())
            {
                *UplinkFromTop(3)= RotateLeft(grand_parent);

                parent->color = black;
                grand_parent->color = red;

                break;
            }
        }
    }

    bool IsBranchInternalAndLeft()  { return (!IsUncleLeft() && (ElemFromTop(2)->right == ElemFromTop(1))); }
    bool IsBranchInternalAndRight() { return (IsUncleLeft()  && (ElemFromTop(2)->left  == ElemFromTop(1))); }
    bool IsBranchExternalAndLeft()  { return (!IsUncleLeft() && (ElemFromTop(2)->left  == ElemFromTop(1))); }
    bool IsBranchExternalAndRight() { return (IsUncleLeft()  && (ElemFromTop(2)->right == ElemFromTop(1))); }

    using HistoryToPath = typename std::vector<std::pair<Node*, Node**>>;
    HistoryToPath history_to_path;
    
    public:
    
    Tree()
    {
        head = new Node();
        head->left = head;
        head->right = head;
        head->color = black;
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
            root->color = black;
            return;
        }

        history_to_path.clear();
        history_to_path.push_back({node, &root});
        bool is_node_left = true;
        for (;;)
        {
            if (value < node->value)
            {
                if (node->is_lthread) { break; }
                history_to_path.push_back({node->left, &(node->left)});
                node = node->left;
            }
            else if (value > node->value)
            {
                if (node->is_rthread) { is_node_left = false; break; }
                history_to_path.push_back({node->right, &(node->right)});
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
            new_node->color = red;
            history_to_path.push_back({new_node, &(node->left)});
        }

        else
        {
            Node* succ = node->right;
            Node* new_node = new Node(value);
            new_node->right = succ;
            new_node->left = node;
            node->is_rthread = false;
            node->right = new_node;
            new_node->color = red;
            history_to_path.push_back({new_node, &(node->right)});
        }

        Balancing();
        root->color = black;
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