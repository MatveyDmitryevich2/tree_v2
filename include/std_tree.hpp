#pragma once
#include <set>

using Tree_std = typename std::set<int>;
using Iter_std = typename std::set<int>::iterator;

void Insert(Tree_std& tree, int key)
{
    tree.insert(key);
}

size_t RangeQuery(const Tree_std& tree, int min, int max)
{
    if (max <= min) return 0;
    Iter_std it_min = tree.lower_bound(min);
    Iter_std it_max = tree.upper_bound(max);
    return (size_t)distance(it_min, it_max);
}