//
// Created by rda on 2023/10/03.
//

#include <iostream>
#include <set>
#include <cassert>

#include "deque.hh"
#include "set.hh"

template <typename Set, typename RefSet>
void test_constructors_assign()
{
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
    assert(/*si2 == si3 && */std::equal(si3.begin(), si3.end(), rsi2.begin(), rsi2.end()));
    assert(si3.size() == rsi2.size());

    // 4. Move constructor
    Set si4(std::move(si3));
    assert(/*si2 == si4 && */std::equal(si4.begin(), si4.end(), rsi2.begin(), rsi2.end()));
    assert(si3.empty());

    // 5. Initializer_list constructor
    Set si5{1, 2, 3, 4, 5, 6, 7, 6, 5, 3, 1, 2, 0};
    assert(/*si2 == si5 && */std::equal(si5.begin(), si5.end(), rsi2.begin(), rsi2.end()));

    // 6. copy assign
    Set si6;
    assert(si6.empty());
    si6 = si5;
    assert(/*si2 == si6 && */std::equal(si6.begin(), si6.end(), rsi2.begin(), rsi2.end()));

    // 7. move assign
    Set si7;
    si7 = std::move(si6);
    assert(/*si2 == si7 && */std::equal(si7.begin(), si7.end(), rsi2.begin(), rsi2.end()));
    assert(si6.empty());
}

int main()
{
    printf("=============%s=================\n", __FUNCTION__);

    test_constructors_assign<stl::set<int>, std::set<int>>();
    test_constructors_assign<stl::multiset<int>, std::multiset<int>>();
    std::cout << "Pass!\n";

    return 0;
}