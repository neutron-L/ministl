#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <deque>
#include "iterator.hh"
using namespace std;


/* Internal aux-functions */
// test container such as vector、list and so on
template<typename Container>
static void test_reverse_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst);
template<typename Container>
static void test_back_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst);
template<typename Container>
static void test_front_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst);
template<typename Container>
static void test_insert_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst);


// test all container with type T
template<typename T>
static void test_reverse_iterator_type_aux();
template<typename T>
static void test_back_iterator_type_aux();
template<typename T>
static void test_front_iterator_type_aux();
template<typename T>
static void test_insert_iterator_type_aux();

/* Test Functions */
void test_iter_traits();
void test_reverse_iterator();
void test_back_iterator();
void test_front_iterator();
void test_insert_iterator();
void test_distance();
void test_advance();

int main()
{
    test_iter_traits();
    test_reverse_iterator();
    test_back_iterator();
    test_front_iterator();
    test_insert_iterator();

    return 0;
}

void test_iter_traits()
{

}

void test_reverse_iterator()
{

}


void test_back_iterator()
{
    
}


void test_front_iterator()
{

}

void test_insert_iterator()
{

}


void test_distance()
{

}

void test_advance()
{

}


template<typename Container>
static void test_reverse_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst)
{

}


template<typename Container>
static void test_back_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst)
{
    
}

template<typename Container>
static void test_front_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst)
{
    printf("===========%s===========n", __FUNCTION__);
    using T = typename Container::value_type;
    Container c1 = lst;
    Container c2 = lst;
    vector<T> vi = lst;
    deque<T> di = lst;
    list<T> li = lst;
    

    auto fi1 = std::front_inserter(c1);
    auto fi2 = stl::front_inserter(c2);
    *fi1 = 2;
    *fi1 = 3;
    *fi1 = 4;

    *fi2 = 2;
    *fi2 = 3;
    *fi2 = 4;
    
    assert(c1 == c2);
    std::copy(li.begin(), li.end(), fi1);
    std::copy(li.begin(), li.end(), fi2);
    assert(c1 == c2);
}


template<typename Container>
static void test_insert_iterator_container_aux(const std::initializer_list<typename Container::value_type> & lst)
{
    printf("===========%s===========n", __FUNCTION__);
    using T = typename Container::value_type;
    Container c1 = lst;
    Container c2 = lst;
    vector<T> vi {100, 101, 102, 103};
    deque<T> di{1, 2, 3, 4};
    list<T> li{11, 12, 13, 14};
    

    auto fi1 = std::insert_inserter(c1);
    auto fi2 = stl::insert_inserter(c2);
    *fi1 = 2;
    *fi1 = 3;
    *fi1 = 4;

    *fi2 = 2;
    *fi2 = 3;
    *fi2 = 4;
    
    assert(c1 == c2);
    std::copy(li.begin(), li.end(), fi1);
    std::copy(li.begin(), li.end(), fi2);
    assert(c1 == c2);
}

