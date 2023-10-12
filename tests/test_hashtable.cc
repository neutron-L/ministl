//
// Created by rda on 2023/10/10.
//

#include <iostream>
#include <cassert>

#include "deque.hh"
#include "hashtable.hh"


using std::cout;
using std::endl;

void test_constructors_assign()
{
    printf("=============%s=================\n", __FUNCTION__);
    using Hashtable = stl::Hashtable<int, int, std::hash<int>, std::_Identity<int>>;

    stl::deque<int> elems{10, 7, 8, 15, 5, 6, 11, 13, 12};

    // 1. Default constructor
    Hashtable ht1;
    assert(ht1.size() == 0);

    // 2. init list insert
    ht1.insert_unique(elems.begin(), elems.end());
    for (auto & elem : elems)
        assert(*ht1.find(elem) == elem);

    // 3. Copy constructor
    Hashtable ht2 = ht1;
    assert(ht2 == ht1);

     // 4. Move constructor
    Hashtable ht3 = std::move(ht2);
    assert(ht3 == ht1 && ht2.empty());
    
    ht2.insert_unique(elems.begin(), elems.end());
    assert(ht3 == ht1 && ht2 == ht1);

    // 5. copy assign
    Hashtable ht4;
    assert(ht4.empty() && ht4.begin() ==ht4.end());
    ht4 = ht2;
    assert(ht4 == ht3);

    // 6. move assign
    Hashtable ht5;
    ht5 = std::move(ht2);
    assert(ht5 == ht3 && ht2.empty());
}
void test_modifiers()
{
    printf("=============%s=================\n", __FUNCTION__);
    using Hashtable = stl::Hashtable<int, int, std::hash<int>, std::_Identity<int>>;

    stl::deque<int> elems{10, 7, 8, 15, 5, 6, 11, 13, 12};

    Hashtable ht1;
    ht1.insert_equal(elems.begin(), elems.end());

    // 1. Default constructor
   
}
void test_lookup()
{
    printf("=============%s=================\n", __FUNCTION__);
    using Hashtable = stl::Hashtable<int, int, std::hash<int>, std::_Identity<int>>;

    stl::deque<int> elems{9,8,7};

    Hashtable ht1;
    ht1.insert_equal(elems.begin(), elems.end());

    // 1. Default constructor
}

int main()
{
    test_constructors_assign();
    test_modifiers();
    test_lookup();

    return 0;
}