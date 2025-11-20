#include <iostream>
#include <set>
#include <chrono>

#include "../include/tree.hpp"
#include "../include/std_tree.hpp"

namespace
{
    const char KEY = 'k';
    const char QUERY = 'q';
}

int main()
{
    char c;

    #ifdef USE_MY_TREE
        Tree tree;
    #endif

    #ifdef USE_STD_TREE
        std::set<int> tree;
    #endif

    while (std::cin >> c)
    {
        switch(c)
        {
            case KEY:
            {
                int key = 0;
                if (!(std::cin >> key)) return 0;

                auto start = std::chrono::steady_clock::now();
                #ifdef USE_MY_TREE
                    tree.Insert(key);
                #endif

                #ifdef USE_STD_TREE
                    Insert(tree, key);
                #endif
                auto finish = std::chrono::steady_clock::now();

                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
                std::cout << "Insert:     " << elapsed << std::endl;

            } break;

            case QUERY:
            {
                int min = 0;
                int max = 0;
                if (!(std::cin >> min >> max)) return 0;

                auto start = std::chrono::steady_clock::now();
                #ifdef USE_MY_TREE
                    tree.RangeQuery(min, max);
                #endif

                #ifdef USE_STD_TREE
                    RangeQuery(tree, min, max);
                #endif
                auto finish = std::chrono::steady_clock::now();

                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
                std::cout << "RangeQuery: " << elapsed << std::endl;
            } break;

            default:
            {
                return 0;
            }
        }
    }

    std::cout << std::endl;
    return 0;
}
