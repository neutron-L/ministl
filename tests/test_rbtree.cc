//
// Created by rda on 2023/9/29.
//
/*
 * 前序遍历和中序遍历可以确定一棵树的唯一构造，不过当前采用对红黑树的前中后序遍历，判断是否和预期的一致
 * 红黑树的遍历返回的每一个元素为pair{值，颜色}
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <initializer_list>

#include "rbtree.hh"
#include "vector.hh"
#include "list.hh"

using std::cout;
using std::endl;

void test_constructors_assign()
{
    printf("=============%s=================\n", __FUNCTION__);
    using Rb_tree = stl::Rb_tree<int, int, std::_Identity<int>, std::less<int>>;

    stl::list<int> elems{10, 7, 8, 15, 5, 6, 11, 13, 12};

    // 1. default constructor
    Rb_tree t1;
    assert(t1.empty() && t1.begin() == t1.end() && t1.cbegin() == t1.cend());

    t1.insert_equal(elems.begin(), elems.end());
    // 2. copy constructor
    Rb_tree t2(t1);
    assert(stl::equal(t1.begin(), t1.end(), t2.begin()));
    assert(t1.size() == t2.size());

    // 3. move constructor
    Rb_tree t3(std::move(t1));
    assert(stl::equal(t2.begin(), t2.end(), t3.begin()));
    assert(t2.size() == t3.size() && t1.size() == 0);

    cout << "t3: \n";
    for (auto &i : t3)
        cout << i << ' ';
    cout << endl;

    // 4. =
    elems.clear();
    elems = {1, 2, 3, 5, 0, 3};
    t3.clear();
    assert(t3.empty());
    t3.insert_equal(elems.begin(), elems.end());
    t1 = t3;
    assert(stl::equal(t1.begin(), t1.end(), t3.begin()));
    assert(t1.size() == t3.size());
    cout << "t1: \n";
    for (auto &i : t1)
        cout << i << ' ';
    cout << endl;

    // 5. = move
    t2 = std::move(t3);
    assert(t3.empty());
    assert(stl::equal(t1.begin(), t1.end(), t2.begin()));
    assert(t1.size() == t2.size());
    cout << "t2: \n";
    for (auto &i : t2)
        cout << i << ' ';
    cout << endl;

    // more test: erase and iterator
    assert(*t1.begin() == 0);
    t1.erase(t1.begin());
    assert(*t1.begin() == 1);
    t1.erase(5);
    assert(*--t1.end() == 3);
}

void test_modifiers()
{
    printf("=============%s=================\n", __FUNCTION__);

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

    pre = {{11, 1}, {8, 1}, {6, 0}, {5, 1}, {7, 1}, {6, 0}, {10, 1}, {9, 0}, {10, 0}, {13, 1}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 1}, {6, 0}, {6, 0}, {7, 1}, {8, 1}, {9, 0}, {10, 1}, {10, 0}, {11, 1}, {11, 0}, {12, 1}, {13, 1}, {15, 1}};
    post = {{5, 1}, {6, 0}, {7, 1}, {6, 0}, {9, 0}, {10, 0}, {10, 1}, {8, 1}, {11, 0}, {12, 1}, {15, 1}, {13, 1}, {11, 1}};

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

    pre = {{11, 1}, {8, 1}, {6, 0}, {5, 1}, {7, 1}, {6, 0}, {10, 0}, {9, 1}, {8, 0}, {10, 1}, {13, 1}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 1}, {6, 0}, {6, 0}, {7, 1}, {8, 1}, {8, 0}, {9, 1}, {10, 0}, {10, 1}, {11, 1}, {11, 0}, {12, 1}, {13, 1}, {15, 1}};
    post = {{5, 1}, {6, 0}, {7, 1}, {6, 0}, {8, 0}, {9, 1}, {10, 1}, {10, 0}, {8, 1}, {11, 0}, {12, 1}, {15, 1}, {13, 1}, {11, 1}};

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
    Rb_tree shadow = rbtree;
    // 12. erase pos
    iter = rbtree.erase(rbtree.find(8));
    assert(*iter == 8);

    // check
    pre = {{11, 1}, {8, 1}, {6, 0}, {5, 1}, {7, 1}, {6, 0}, {10, 0}, {9, 1}, {10, 1}, {13, 1}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 1}, {6, 0}, {6, 0}, {7, 1}, {8, 1}, {9, 1}, {10, 0}, {10, 1}, {11, 1}, {11, 0}, {12, 1}, {13, 1}, {15, 1}};
    post = {{5, 1}, {6, 0}, {7, 1}, {6, 0}, {9, 1}, {10, 1}, {10, 0}, {8, 1}, {11, 0}, {12, 1}, {15, 1}, {13, 1}, {11, 1}};

    pre_tree = rbtree.pre_traverse();
    mid_tree = rbtree.mid_traverse();
    post_tree = rbtree.post_traverse();

    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
    assert(rbtree.size() == pre.size());
    assert(rbtree.size() == shadow.size() - 1);

    iter = rbtree.find(8);
    iter = rbtree.erase(iter);
    assert(*iter == 9);

    pre = {{11, 1}, {9, 1}, {6, 0}, {5, 1}, {7, 1}, {6, 0}, {10, 1}, {10, 0}, {13, 1}, {12, 1}, {11, 0}, {15, 1}};
    mid = {{5, 1}, {6, 0}, {6, 0}, {7, 1}, {9, 1}, {10, 1}, {10, 0}, {11, 1}, {11, 0}, {12, 1}, {13, 1}, {15, 1}};
    post = {{5, 1}, {6, 0}, {7, 1}, {6, 0}, {10, 0}, {10, 1}, {9, 1}, {11, 0}, {12, 1}, {15, 1}, {13, 1}, {11, 1}};

    pre_tree = rbtree.pre_traverse();
    mid_tree = rbtree.mid_traverse();
    post_tree = rbtree.post_traverse();

    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
    assert(rbtree.size() == pre.size());

    assert(rbtree.size() == shadow.size() - 2);

    iter = rbtree.find(13);
    iter = rbtree.erase(iter);
    assert(*iter == 15);
    assert(iter == --rbtree.end());

    pre = {{11, 1}, {9, 1}, {6, 0}, {5, 1}, {7, 1}, {6, 0}, {10, 1}, {10, 0}, {12, 1}, {11, 1}, {15, 1}};
    mid = {{5, 1}, {6, 0}, {6, 0}, {7, 1}, {9, 1}, {10, 1}, {10, 0}, {11, 1}, {11, 1}, {12, 1}, {15, 1}};
    post = {{5, 1}, {6, 0}, {7, 1}, {6, 0}, {10, 0}, {10, 1}, {9, 1}, {11, 1}, {15, 1}, {12, 1}, {11, 1}};

    pre_tree = rbtree.pre_traverse();
    mid_tree = rbtree.mid_traverse();
    post_tree = rbtree.post_traverse();

    assert(pre_tree == pre);
    assert(mid_tree == mid);
    assert(post_tree == post);
    assert(rbtree.size() == pre.size());

    assert(rbtree.size() == shadow.size() - 3);

    // 12. erase key
    cout << "before erasing 6:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    assert(rbtree.erase(6) == 2);
    cout << "aftererasing 6:\n";

    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    assert(rbtree.erase(11) == 2);
    cout << "aftererasing 11:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    assert(rbtree.erase(7) == 1);
    cout << "aftererasing 7:\n";

    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    assert(rbtree.erase(15) == 1);
    cout << "aftererasing 15:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    assert(rbtree.erase(16) == 0);
    cout << "aftererasing 16:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    assert(rbtree.erase(5) == 1);
    cout << "aftererasing 5:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    assert(rbtree.erase(4) == 0);
    cout << "aftererasing 4:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    // 12. erase range
    rbtree = shadow;
    cout << "before erasing [begin+2, end-1):\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    auto first = rbtree.begin();
    auto last = rbtree.end();
    ++first;
    ++first;
    --last;
    rbtree.erase(first, last);
    cout << "aftererasing [begin+2, end-1):\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    cout << "restore rbtree:\n";
    rbtree = shadow;
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    cout << "before erasing [begin+4, end-5):\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    first = rbtree.begin();
    last = rbtree.end();
    ++first;
    ++first;
    ++first;
    ++first;
    --last;
    --last;
    --last;
    --last;
    --last;
    rbtree.erase(first, last);
    cout << "aftererasing [begin+4, end-5):\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    first = rbtree.begin();
    last = rbtree.end();
    ++first;
    ++first;
    --last;
    --last;
    --last;
    rbtree.erase(first, last);
    cout << "aftererasing [begin+2, end-3):\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
}

void test_lookup()
{
    printf("=============%s=================\n", __FUNCTION__);

    const int N = 300;
    stl::vector<char> vc;
    using Rb_tree = stl::Rb_tree<char, char, std::_Identity<char>, std::less<char>>;
    Rb_tree rbtree;

    // init vc
    // The initialization array contains N random lowercase letters from 'a' to 'k',
    cout << "init vc with 30 random lowercase letters from 'a' to 'k'\n";
    vc.resize(N);
    for (auto &c : vc)
    {
        c = ((int)rand() % 10) + 'a';
        cout << c << ' ';
    }

    // insert all elems in vc
    for (auto &c : vc)
        rbtree.insert_equal(c);
    cout << "init tree:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    assert(rbtree.size() == vc.size());

    // 1. test find
    auto iter = rbtree.find(*vc.begin());
    assert(*iter == *vc.begin());

    for (char c = 'a'; c <= 'z'; ++c)
    {
        iter = rbtree.find(c);
        if (iter != rbtree.end())
            assert(*iter == c);
    }

    std::sort(vc.begin(), vc.end());
    // 2. test count and equal_range
    for (char c = 'a'; c <= 'z'; ++c)
    {
        auto dis = (std::upper_bound(vc.begin(), vc.end(), c)) - std::lower_bound(vc.begin(), vc.end(), c);
        assert(rbtree.count(c) == dis);
        auto ret = rbtree.equal_range(c);
        assert(stl::distance(ret.first, ret.second) == dis);
    }
}

void test_insert_pos()
{
    printf("=============%s=================\n", __FUNCTION__);

    const int N = 100;

    using Rb_tree = stl::Rb_tree<int, int, std::_Identity<int>, std::less<int>>;
    Rb_tree rbtree;
    stl::vector<int> vc;

    // 1. init vc
    // The initialization array contains N random numbers from 0 to 9,
    cout << "init vc with "<< N << " random lowercase letters from 'a' to 'k'\n";
    vc.resize(N);
    for (auto &c : vc)
    {
        c = ((int)rand() % 10);
        cout << c << ' ';
    }
    cout << endl;

    // 2. init rbtree
    rbtree.insert_equal(vc.begin(), vc.end());
    cout << "init tree:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    assert(rbtree.size() == vc.size());

    // 3. test insert unique (noexist)
    auto iter = rbtree.insert_unique(rbtree.end(), 11);
    assert(*iter == 11);
    iter = rbtree.begin();
    stl::advance(iter, rbtree.size() / 2);

    iter = rbtree.insert_unique(iter, -1);
    assert(*iter == -1);

    // 4. insert unique (exist)
    iter = rbtree.begin();
    stl::advance(iter, rbtree.size() / 2);
    auto elem = *iter;
    auto old_size = rbtree.size();
    assert(*rbtree.insert_unique(rbtree.begin(), *iter) == elem);
    assert(*rbtree.insert_unique(rbtree.end(), *iter) == elem);
    assert(*rbtree.insert_unique(iter, *iter) == elem);
    assert(rbtree.size() == old_size);

    rbtree.clear();
    assert(rbtree.empty());
    vc = {1, 1, 3, 3, 3, 4, 4};
    rbtree.insert_equal(vc.begin(), vc.end());
    assert(*rbtree.begin() == 1 && *(--rbtree.end()) == 4);

    // 5. insert equal (noexist)
    iter = rbtree.insert_equal(rbtree.begin(), 2);
    assert(*iter == 2 && stl::distance(rbtree.begin(), iter) == 2);
    iter = rbtree.insert_equal(iter, 2);
    assert(*iter == 2 && stl::distance(rbtree.begin(), iter) == 2 && *++iter == 2);
    iter = rbtree.insert_equal(rbtree.end(), 2);
    assert(*iter == 2 && stl::distance(rbtree.begin(), iter) == 4 && *--iter == 2 && *--iter == 2);
    assert(*rbtree.begin() == 1 && *(--rbtree.end()) == 4);

    cout << "tree:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    assert(rbtree.size() == vc.size() + 3);
    assert(rbtree.erase(2) == 3);
    assert(*rbtree.begin() == 1 && *(--rbtree.end()) == 4);

    // 6. insert equal (exist)
    iter = rbtree.insert_equal(rbtree.begin(), 3);
    assert(*iter == 3 && stl::distance(rbtree.begin(), iter) == 2);
    iter = rbtree.insert_equal(iter, 3);
    assert(*iter == 3 && stl::distance(rbtree.begin(), iter) == 2);
    ++iter;
    iter = rbtree.insert_equal(iter, 3);
    assert(*iter == 3 && stl::distance(rbtree.begin(), iter) == 3);
    assert(*rbtree.begin() == 1 && *(--rbtree.end()) == 4);

    cout << "tree:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;
    iter = rbtree.end();
    iter = rbtree.insert_equal(iter, 3);
    assert(*iter == 3 && stl::distance(iter, rbtree.end()) == 3);
    cout << "tree:\n";
    for (auto &i : rbtree)
        cout << i << ' ';
    cout << endl;

    // 7. big test!!
    rbtree.clear();

    int t = N;
    while (t--)
    {
        for (int k = 0; k < 2; ++k)
            for (int i = k; i <= 1000; i += 2)
            {
                int p = i % 5;
                switch (i)
                {
                case 0:
                    iter = rbtree.begin();
                    break;
                case 1:
                    iter = rbtree.lower_bound(i);
                    break;
                case 2:
                    iter = rbtree.lower_bound(i);
                    stl::advance(iter, rbtree.count(i) / 2);
                    break;
                case 3:
                    iter = rbtree.upper_bound(i);
                    break;
                case 4:
                    iter = rbtree.end();
                    break;
                default:
                    break;
                }
                assert(*rbtree.insert_equal(iter, i) == i);
            }
    }

    vc.clear();
    vc = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    std::random_shuffle(vc.begin(), vc.end());
    for (auto &j : vc)
    {
        cout << j << ' ';
        for (int i = j * 100; i < (j + 1) * 100; ++i)
            assert(rbtree.erase(i) == N);
    }
    cout << endl;
    assert(rbtree.count(1000) == N);
    assert(rbtree.erase(1000) == N);
    assert(rbtree.empty());
}

int main()
{
    test_constructors_assign();
    test_modifiers();
    test_lookup();
    test_insert_pos();
    std::cout << "Pass!\n";

    return 0;
}