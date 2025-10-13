#pragma once

#include <cassert>
#include <cstddef>

class Tree
{
    struct Node
    {
        int key;
        Node* left;
        Node* right;
        bool lthread;
        bool rthread;

        Node(int k = 0)
            : key(k), left(nullptr), right(nullptr),
            lthread(true), rthread(true) {}
    };

    Node* root = nullptr;
    Node* head = nullptr;

    Node* Successor(Node* node)
    {
        if (node->rthread == true) { return node->right; }
        else 
        {
            node = node->right;
            while (node->lthread != true) { node = node->left; }
            return node;
        }
    }

    Node* LowerBound(int min)
    {
        Node* node = root;
        for (;;)
        {
            if (min == node->key) { return node; }
            if (min < node->key)
            {
                if (node->lthread == false) { node = node->left; continue; }
                else { return node; }
            }
            if (min > node->key)
            {
                if (node->rthread == false) { node = node->right; continue; }
                else { node = Successor(node); return node; }
            }
        }
    }

    public:

    Tree()
    {
        head = new Node();
        head->left = head;
        head->right = head;
    }

    void Insert(int key)
    {
        Node* node = root;
        bool is_node_left = true;

        if (root == nullptr)
        {
            root = new Node(key);
            root->left = head;
            root->right = head;
            head->left = root;
            head->lthread = false;
            return;
        }

        else
        {
            for (;;)
            {
                if (key < node->key)
                {
                    if (node->lthread == false) { node = node->left; continue; }
                    else { break; }
                }
                if (key > node->key)
                {
                    if (node->rthread == false) { node = node->right; continue; }
                    else { is_node_left = false; break; }
                }
            }
        }

        if (is_node_left)
        {
            Node* pred = node->left;
            Node* new_node = new Node(key);
            new_node->left = pred;
            new_node->right = node;
            node->lthread = false;
            node->left = new_node;
        }

        else
        {
            Node* succ = node->right;
            Node* new_node = new Node(key);
            new_node->right = succ;
            new_node->left = node;
            node->rthread = false;
            node->right = new_node;
        }
    }

    std::size_t RangeQuery(int min, int max)
    {
        if (root == nullptr) { return 0; }

        std::size_t count = 0;
        for (Node* iter_node = LowerBound(min);
             iter_node != head && iter_node->key <= max;
             count++)
        {
            iter_node = Successor(iter_node);
        }

        return count;
    }

};