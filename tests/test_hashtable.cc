//
// Created by rda on 2023/10/10.
//

#include <iostream>

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

    // 2. 

}
void test_modifiers()
{
}
void test_lookup()
{
}
void test_insert_pos()
{
}

int main()
{
    test_constructors_assign();
    return 0;
}