//
// Created by rda on 2023/10/03.
//

#include <iostream>
#include <set>
#include <cassert>

#include "deque.hh"
#include "set.hh"

using std::cout;
using std::endl;

template <typename Set, typename RefSet>
void test_constructors_assign()
{
    printf("=============%s=================\n", __FUNCTION__);

    // 1. Default constructor
    Set si1;
    assert(si1.empty() && si1.begin() == si1.end());

    // 2. Range constructor
    stl::deque<int> di{1, 2, 3, 4, 5, 6, 7, 6, 5, 3, 1, 2, 0};
    Set si2(di.begin(), di.end());
    RefSet rsi2(di.begin(), di.end());

    assert(std::equal(si2.begin(), si2.end(), rsi2.begin(), rsi2.end()));
    assert(si2.size() == rsi2.size());

    // 3. Copy constructor
    Set si3(si2);
    assert(/*si2 == si3 && */ std::equal(si3.begin(), si3.end(), rsi2.begin(), rsi2.end()));
    assert(si3.size() == rsi2.size());

    // 4. Move constructor
    Set si4(std::move(si3));
    assert(/*si2 == si4 && */ std::equal(si4.begin(), si4.end(), rsi2.begin(), rsi2.end()));
    assert(si3.empty());

    // 5. Initializer_list constructor
    Set si5{1, 2, 3, 4, 5, 6, 7, 6, 5, 3, 1, 2, 0};
    assert(/*si2 == si5 && */ std::equal(si5.begin(), si5.end(), rsi2.begin(), rsi2.end()));

    // 6. copy assign
    Set si6;
    assert(si6.empty());
    si6 = si5;
    assert(/*si2 == si6 && */ std::equal(si6.begin(), si6.end(), rsi2.begin(), rsi2.end()));

    // 7. move assign
    Set si7;
    si7 = std::move(si6);
    assert(/*si2 == si7 && */ std::equal(si7.begin(), si7.end(), rsi2.begin(), rsi2.end()));
    assert(si6.empty());
}

void test_insert_unique()
{
    printf("=============%s=================\n", __FUNCTION__);
    const int N = 1000;
    stl::set<int> si;
    std::set<int> rsi;

    /*
     * Insert
     * */
    // 1. Random Insert
    auto tmp = N;
    while (tmp--)
    {
        int elem = rand() % 100;
        auto r1 = si.insert(elem);
        auto r2 = rsi.insert(elem);
        assert(*r1.first == *r2.first && r1.second == r2.second);
    }
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());
}

void test_insert_multi()
{
    printf("=============%s=================\n", __FUNCTION__);
    const int N = 1000;
    stl::multiset<int> si;
    std::multiset<int> rsi;

    /*
     * Insert
     * */
    // 1. Insert
    auto tmp = N;
    while (tmp--)
    {
        int elem = rand() % 100;
        auto r1 = si.insert(elem);
        auto r2 = rsi.insert(elem);
        assert(*r1 == *r2);
    }
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());
}

template <typename Set, typename RefSet>
void test_modifiers()
{
    printf("=============%s=================\n", __FUNCTION__);

    Set si;
    RefSet rsi;

    /*
     * Insert
     * */
    // 1. Insert Range
    stl::vector<int> vi{1, 3, 2, 4, 5, 76, 7, 567, 45, 3, 2, 2};
    si.insert(vi.begin(), vi.end());
    rsi.insert(vi.begin(), vi.end());
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());

    // 2. Insert ilist
    si.insert({1, 2, 3, 4, 5, 6, 7, 6, 5, 3, 1, 2, 0});
    rsi.insert({1, 2, 3, 4, 5, 6, 7, 6, 5, 3, 1, 2, 0});
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());

    // 3. clear
    auto tsi = si;
    auto trsi = rsi;
    si.clear();
    rsi.clear();
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());

    si = tsi;
    rsi = trsi;

    // 4. insert pos
    auto iter = si.begin();
    auto riter = rsi.begin();

    int n = si.size();
    for (int i = 0; i < n; ++i)
    {
        // insert当前值
        assert(*si.insert(iter, *iter) == *rsi.insert(riter, *riter));

        // insert前驱值
        if (iter != si.begin())
        {
            assert(riter != rsi.begin());
            auto ix = iter;
            --ix;
            auto iy = riter;
            --iy;
            assert(*si.insert(iter, *ix) == *rsi.insert(riter, *iy));
        }
        // insert后继值
        if (iter != --si.end())
        {
            assert(riter != --rsi.end());
            auto ix = iter;
            ++ix;
            auto iy = riter;
            ++iy;
            assert(*si.insert(iter, *ix) == *rsi.insert(riter, *iy));
        }
        assert(stl::distance(si.begin(), iter) == stl::distance(rsi.begin(), riter));

        iter = si.begin();
        stl::advance(iter, i);
        riter = rsi.begin();
        stl::advance(riter, i);
    }
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());

    /*
     * Erase
     * */
    // 1. erase range
    auto f1 = si.begin();
    auto l1 = f1;
    stl::advance(l1, si.size() / 2);
    auto f2 = rsi.begin();
    auto l2 = f2;
    stl::advance(l2, rsi.size() / 2);

    assert(*si.erase(f1, l1) == *rsi.erase(f2, l2));
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());

    // 2. erase pos
    n = si.size();
    iter = si.begin();
    riter = rsi.begin();
    for (int i = 0; i < n; ++i)
    {
        iter = si.erase(iter);
        riter = rsi.erase(riter);
        if (iter != si.end())
        {
            assert(riter != rsi.end());
            assert(*iter == *riter);
        }
    }
    assert(si.empty() && rsi.empty());

    // 3. erase key
    si.swap(tsi);
    rsi = trsi;
    assert(tsi.empty());
    std::set<int> s(si.begin(), si.end());
    for (auto & elem : s)
        assert(si.erase(elem) == rsi.erase(elem));
    assert(std::equal(si.begin(), si.end(), rsi.begin()) && si.size() == rsi.size());
}

template <typename Set, typename RefSet>
void test_lookup()
{
    printf("=============%s=================\n", __FUNCTION__);
}

template <typename Set, typename RefSet>
void test_all()
{
    test_constructors_assign<Set, RefSet>();
    test_modifiers<Set, RefSet>();
    test_lookup<Set, RefSet>();
}
int main()
{
    test_insert_unique();
    test_insert_multi();
    test_all<stl::set<int>, std::set<int>>();
    test_all<stl::multiset<int>, std::multiset<int>>();
    std::cout << "Pass!\n";

    return 0;
}