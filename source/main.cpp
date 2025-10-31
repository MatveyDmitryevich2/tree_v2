#include <iostream>
#include <string>
#include <cstddef>

#include "../include/tree.hpp"

namespace
{
    const char KEY = 'k';
    const char QUERY = 'q';
}

int main()
{
    Tree tree;

    char c;

    while (std::cin >> c)
    {
        switch(c)
        {
            case KEY:
            {
                int key = 0;
                if (!(std::cin >> key)) { return 0; }
                tree.Insert(key);
            } break;

            case QUERY:
            {
                int min = 0;
                int max = 0;
                if (!(std::cin >> min >> max)) { return 0; }
                std::cout << tree.RangeQuery(min, max) << " ";
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