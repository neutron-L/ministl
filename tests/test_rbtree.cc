//
// Created by rda on 2023/9/29.
//
/*
 前序遍历和中序遍历可以确定一棵树的唯一构造，
*/
#include <iostream>
#include <vector>
#include <cassert>
#include <initializer_list>

#include "rbtree.hh"
#include "vector.hh"

using std::cout;
using std::endl;

void test_constructors()
{
}

void test_modifiers()
{
    using Rb_tree = stl::Rb_tree<int, int, std::_Identity<int>, std::less<int>>;
    Rb_tree rbtree;

    // 1. 初始状态下的rb-tree
    std::vector<int> elems{10, 7, 8, 15, 5, 6, 11, 13, 12};
    stl::vector<std::pair<int, int>> pre{{8, 1}, {6, 1}, {5, 0}, {7, 0}, {11, 0}, {10, 1}, {13, 1}, {12, 0}, {15, 0}};
    stl::vector<std::pair<int, int>> mid{{5, 0}, {6, 1}, {7, 0}, {8, 1}, {10, 1}, {11, 0}, {12, 0}, {13, 1}, {15, 0}};
    stl::vector<std::pair<int, int>> post{{5, 0}, {7, 0}, {6, 1}, {10, 1}, {12, 0}, {15, 0}, {13, 1}, {11, 0}, {8, 1}};
    // rbtree.insert_unique(elems.begin(), elems.end());

    /*
     * Test Insert
     * */
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

    stl::vector<std::pair<int, int>> v1;
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    for (auto iter = rbtree.begin(); iter != rbtree.end(); ++iter)
        cout << *iter << ' ';
    cout << endl;
    for (auto iter = rbtree.cbegin(); iter != rbtree.cend(); ++iter)
        cout << *iter << ' ';
    cout << endl;
    for (auto iter = rbtree.rbegin(); iter != rbtree.rend(); ++iter)
        cout << *iter << ' ';
    cout << endl;
    for (auto iter = rbtree.end(); iter != rbtree.begin();)
        cout << *(--iter) << ' ';
    cout << endl;

    // 2. insert unique 12, 失败，因为已经有了一个12
    auto ret = rbtree.insert_unique(12);
    assert(*ret.first == 12 && !ret.second);
    ret = rbtree.insert_unique(11);
    assert(*ret.first == 11 && !ret.second);
    ++ret.first;
    assert(*ret.first == 12);
    // 3. insert unique 所有已存在的元素, 均失败
    for (auto &elem : elems)
    {
        auto res = rbtree.insert_unique(elem);
        assert(*(res.first) == elem && !res.second);
    }

    // 4. insert equal 11， 成功，并验证红黑树的结构是否符合预期
    auto iter = rbtree.insert_equal(11);
    assert(*iter == 11);
    pre = {{11, 1}, {8, 0}, {6, 1}, {5, 0}, {7, 0}, {10, 1}, {13, 0}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 0}, {6, 1}, {7, 0}, {8, 0}, {10, 1}, {11, 1}, {11, 0}, {12, 1}, {13, 0}, {15, 1}};
    post = {{5, 0}, {7, 0}, {6, 1}, {10, 1}, {8, 0}, {11, 0}, {12, 1}, {15, 1}, {13, 0}, {11, 1}};

    pre_tree = rbtree.pre_traverse();
    mid_tree = rbtree.mid_traverse();
    post_tree = rbtree.post_traverse();

    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    // 5. insert equal 9, 10， 成功，并验证红黑树的结构是否符合预期
    iter = rbtree.insert_equal(10);
    assert(*iter == 10);
    iter = rbtree.insert_equal(9);
    assert(*iter == 9);

    pre = {{11, 1}, {8, 0}, {6, 1}, {5, 0}, {7, 0}, {10, 1}, {9, 0}, {10, 0}, {13, 0}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 0}, {6, 1}, {7, 0}, {8, 0}, {9, 0}, {10, 1}, {10, 0}, {11, 1}, {11, 0}, {12, 1}, {13, 0}, {15, 1}};
    post = {{5, 0}, {7, 0}, {6, 1}, {9, 0}, {10, 0}, {10, 1}, {8, 0}, {11, 0}, {12, 1}, {15, 1}, {13, 0}, {11, 1}};

    pre_tree = rbtree.pre_traverse();
    mid_tree = rbtree.mid_traverse();
    post_tree = rbtree.post_traverse();

    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
    assert(rbtree.size() == pre.size());

    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    stl::vector<int> dump(rbtree.begin(), rbtree.end());
    // insert unique已经存在的元素全部失败
    for (auto &elem : dump)
    {
        auto res = rbtree.insert_unique(elem);
        assert(*(res.first) == elem && !res.second);
    }

    // 6. insert equal 6
    iter = rbtree.insert_equal(6);
    assert(*iter == 6);

    pre = {{11, 1}, {8, 1}, {6, 0}, {5, 1}, {6, 1}, {7, 0}, {10, 1}, {9, 0}, {10, 0}, {13, 1}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 1}, {6, 0}, {6, 1}, {7, 0}, {8, 1}, {9, 0}, {10, 1}, {10, 0}, {11, 1}, {11, 0}, {12, 1}, {13, 1}, {15, 1}};
    post = {{5, 1}, {7, 0}, {6, 1}, {6, 0}, {9, 0}, {10, 0}, {10, 1}, {8, 1}, {11, 0}, {12, 1}, {15, 1}, {13, 1}, {11, 1}};

    pre_tree = rbtree.pre_traverse();
    mid_tree = rbtree.mid_traverse();
    post_tree = rbtree.post_traverse();

    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
    assert(rbtree.size() == pre.size());

    // 7. insert equal 8
    iter = rbtree.insert_equal(8);
    assert(*iter == 8);

    pre = {{11, 1}, {8, 1}, {6, 0}, {5, 1}, {6, 1}, {7, 0}, {10, 0}, {9, 1}, {8, 0}, {10, 1}, {13, 1}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 1}, {6, 0}, {6, 1}, {7, 0}, {8, 1}, {8, 0}, {9, 1}, {10, 0}, {10, 1}, {11, 1}, {11, 0}, {12, 1}, {13, 1}, {15, 1}};
    post = {{5, 1}, {7, 0}, {6, 1}, {6, 0}, {8, 0}, {9, 1}, {10, 1}, {10, 0}, {8, 1}, {11, 0}, {12, 1}, {15, 1}, {13, 1}, {11, 1}};

    pre_tree = rbtree.pre_traverse();
    mid_tree = rbtree.mid_traverse();
    post_tree = rbtree.post_traverse();

    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
    assert(rbtree.size() == pre.size());

    /*
     * Test Lookup
     * */
    // 8. find单个元素
    iter = rbtree.find(100);
    assert(iter == rbtree.end());
    iter = rbtree.find(8);
    assert(*iter == 8);
    ++iter;
    assert(*iter == 8); // 树中有两个8，默认应该返回第一个

    iter = rbtree.find(6);
    assert(*iter == 6);

    // 9. find所有元素
    dump.clear();
    dump.insert(dump.end(), rbtree.begin(), rbtree.end());
    for (auto &elem : dump)
    {
        iter = rbtree.find(elem);
        assert(*iter == elem);
    }

    // 10. lower_bound
    iter = rbtree.lower_bound(6);
    assert(*iter++ == 6);
    assert(*iter++ == 6);
    assert(*iter == 7);

    auto min_elem = *rbtree.begin();
    for (int i = -3; i <= min_elem; ++i)
    {
        iter = rbtree.lower_bound(i);
        assert(iter == rbtree.begin() && *iter == min_elem);
    }

    assert(rbtree.lower_bound(*(--rbtree.end())) == --rbtree.end());
    assert(rbtree.lower_bound(100) == rbtree.end());

    // 11. upper_bound
    iter = rbtree.upper_bound(5);
    assert(*iter++ == 6);
    assert(*iter++ == 6);
    iter = rbtree.upper_bound(6);
    assert(*iter == 7);
    iter = rbtree.upper_bound(11);
    assert(*iter == 12);

    iter = rbtree.upper_bound(15);
    assert(iter == rbtree.end());
    assert(rbtree.upper_bound(*(--rbtree.end())) == rbtree.end());

    for (int i = -3; i < min_elem; ++i)
    {
        iter = rbtree.upper_bound(i);
        assert(iter == rbtree.begin() && *iter == min_elem);
    }

    /*
     * Test Erase
     * */
    // 8. erase
}

int main()
{
    test_constructors();
    test_modifiers();

    return 0;
}