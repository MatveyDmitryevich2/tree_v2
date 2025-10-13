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

    for(;;)
    {
        char c;
        std::cin >> c;

        if (!std::cin) { break; }

        if (c == KEY)
        {
            int key = 0;
            std::cin >> key;
            tree.Insert(key);
        }

        if (c == QUERY)
        {
            int min = 0;
            int max = 0;
            std::cin >> min >> max;
            std::cout << tree.RangeQuery(min, max) << " ";
        }
    }

    std::cout << std::endl;
}