//
// Created by rda on 2023/9/29.
//
/*
 前序遍历和中序遍历可以确定一棵树的唯一构造，
*/
#include <iostream>
#include <vector>
#include <cassert>

#include "rbtree.hh"
#include "vector.hh"

using std::cout;
using std::endl;

void test_constructors()
{
}

void test_insert()
{
    using Rb_tree = stl::Rb_tree<int, int, std::_Identity<int>, std::less<int>>;
    Rb_tree rbtree;
    std::vector<int> elems{10, 7, 8, 15, 5, 6, 11, 13, 12};
    stl::vector<std::pair<int, int>> pre{{8, 1}, {6, 1}, {5, 0}, {7, 0}, {11, 0}, {10, 1}, {13, 1}, {12, 0}, {15, 0}};
    stl::vector<std::pair<int, int>> mid{{5, 0}, {6, 1}, {7, 0}, {8, 1}, {10, 1}, {11, 0}, {12, 0}, {13, 1}, {15, 0}};
    stl::vector<std::pair<int, int>> post{{5, 0}, {7, 0}, {6, 1}, {10, 1}, {12, 0}, {15, 0}, {13, 1}, {11, 0}, {8, 1}};
    for (auto &elem : elems)
    {
        auto res = rbtree.insert_unique(elem);
        assert(*(res.first) == elem && res.second);
    }
    auto pre_tree = rbtree.pre_traverse();
    auto mid_tree = rbtree.mid_traverse();
    auto post_tree = rbtree.post_traverse();

    // for (auto &p : mid_tree)
    //     cout << p.first << ' ' << p.second << endl;
    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
}

void test_erase()
{
}

int main()
{
    test_constructors();
    test_insert();
    test_erase();

    return 0;
}