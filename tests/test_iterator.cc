#include <iostream>
#include <vector>
#include <deque>
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
static void test_reverse_iterator_aux(const std::initializer_list<typename Container::value_type> & lst);
template<typename Container>
static void test_back_iterator_aux(const std::initializer_list<typename Container::value_type> & lst);
template<typename Container>
static void test_front_iterator_aux(const std::initializer_list<typename Container::value_type> & lst);
template<typename Container>
static void test_insert_iterator_aux(const std::initializer_list<typename Container::value_type> & lst);


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
    printf("===========%s===========\n", __FUNCTION__);
    // int
    test_back_iterator_aux<list<int>>({});
    test_back_iterator_aux<list<int>>({1});
    test_back_iterator_aux<list<int>>({1, 2, 3});
    test_back_iterator_aux<list<int>>({1, 3, 5, 7});

    test_back_iterator_aux<deque<int>>({});
    test_back_iterator_aux<deque<int>>({1});
    test_back_iterator_aux<deque<int>>({1, 2, 3});
    test_back_iterator_aux<deque<int>>({1, 3, 5, 7});
    // double
    test_back_iterator_aux<list<double>>({});
    test_back_iterator_aux<list<double>>({1.12});
    test_back_iterator_aux<list<double>>({1.12, 2.34, 3.76});
    test_back_iterator_aux<list<double>>({1.32, 3.54, 5.132, 7.901});

    test_back_iterator_aux<deque<double>>({});
    test_back_iterator_aux<deque<double>>({1.12});
    test_back_iterator_aux<deque<double>>({1.12, 2.34, 3.76});
    test_back_iterator_aux<deque<double>>({1.32, 3.54, 5.132, 7.901});

    // char
    test_back_iterator_aux<list<char>>({});
    test_back_iterator_aux<list<char>>({'a'});
    test_back_iterator_aux<list<char>>({'a', 'b', 'c'});
    test_back_iterator_aux<list<char>>({'A', 'B', 'C', 'D'});

    test_back_iterator_aux<deque<char>>({});
    test_back_iterator_aux<deque<char>>({'a'});
    test_back_iterator_aux<deque<char>>({'a', 'b', 'c'});
    test_back_iterator_aux<deque<char>>({'A', 'B', 'C', 'D'});

    // struct type
    test_back_iterator_aux<list<string>>({});
    test_back_iterator_aux<list<string>>({"hello"});
    test_back_iterator_aux<list<string>>({"apple", "box", "cat"});
    test_back_iterator_aux<list<string>>({"Cpp", "Java", "C-sharp", "Python"});

    test_back_iterator_aux<deque<string>>({});
    test_back_iterator_aux<deque<string>>({"hello"});
    test_back_iterator_aux<deque<string>>({"apple", "box", "cat"});
    test_back_iterator_aux<deque<string>>({"Cpp", "Java", "C-sharp", "Python"});
}


void test_front_iterator()
{
    printf("===========%s===========\n", __FUNCTION__);
    // int
    test_front_iterator_aux<list<int>>({});
    test_front_iterator_aux<list<int>>({1});
    test_front_iterator_aux<list<int>>({1, 2, 3});
    test_front_iterator_aux<list<int>>({1, 3, 5, 7});

    test_front_iterator_aux<deque<int>>({});
    test_front_iterator_aux<deque<int>>({1});
    test_front_iterator_aux<deque<int>>({1, 2, 3});
    test_front_iterator_aux<deque<int>>({1, 3, 5, 7});
    // double
    test_front_iterator_aux<list<double>>({});
    test_front_iterator_aux<list<double>>({1.12});
    test_front_iterator_aux<list<double>>({1.12, 2.34, 3.76});
    test_front_iterator_aux<list<double>>({1.32, 3.54, 5.132, 7.901});

    test_front_iterator_aux<deque<double>>({});
    test_front_iterator_aux<deque<double>>({1.12});
    test_front_iterator_aux<deque<double>>({1.12, 2.34, 3.76});
    test_front_iterator_aux<deque<double>>({1.32, 3.54, 5.132, 7.901});

    // char
    test_front_iterator_aux<list<char>>({});
    test_front_iterator_aux<list<char>>({'a'});
    test_front_iterator_aux<list<char>>({'a', 'b', 'c'});
    test_front_iterator_aux<list<char>>({'A', 'B', 'C', 'D'});

    test_front_iterator_aux<deque<char>>({});
    test_front_iterator_aux<deque<char>>({'a'});
    test_front_iterator_aux<deque<char>>({'a', 'b', 'c'});
    test_front_iterator_aux<deque<char>>({'A', 'B', 'C', 'D'});

    // struct type
    test_front_iterator_aux<list<string>>({});
    test_front_iterator_aux<list<string>>({"hello"});
    test_front_iterator_aux<list<string>>({"apple", "box", "cat"});
    test_front_iterator_aux<list<string>>({"Cpp", "Java", "C-sharp", "Python"});

    test_front_iterator_aux<deque<string>>({});
    test_front_iterator_aux<deque<string>>({"hello"});
    test_front_iterator_aux<deque<string>>({"apple", "box", "cat"});
    test_front_iterator_aux<deque<string>>({"Cpp", "Java", "C-sharp", "Python"});
}

void test_insert_iterator()
{
    printf("===========%s===========\n", __FUNCTION__);
    // int
    test_insert_iterator_aux<list<int>>({});
    test_insert_iterator_aux<list<int>>({1});
    test_insert_iterator_aux<list<int>>({1, 2, 3});
    test_insert_iterator_aux<list<int>>({1, 3, 5, 7});

    test_insert_iterator_aux<deque<int>>({});
    test_insert_iterator_aux<deque<int>>({1});
    test_insert_iterator_aux<deque<int>>({1, 2, 3});
    test_insert_iterator_aux<deque<int>>({1, 3, 5, 7});
    // double
    test_insert_iterator_aux<list<double>>({});
    test_insert_iterator_aux<list<double>>({1.12});
    test_insert_iterator_aux<list<double>>({1.12, 2.34, 3.76});
    test_insert_iterator_aux<list<double>>({1.32, 3.54, 5.132, 7.901});

    test_insert_iterator_aux<deque<double>>({});
    test_insert_iterator_aux<deque<double>>({1.12});
    test_insert_iterator_aux<deque<double>>({1.12, 2.34, 3.76});
    test_insert_iterator_aux<deque<double>>({1.32, 3.54, 5.132, 7.901});

    // char
    test_insert_iterator_aux<list<char>>({});
    test_insert_iterator_aux<list<char>>({'a'});
    test_insert_iterator_aux<list<char>>({'a', 'b', 'c'});
    test_insert_iterator_aux<list<char>>({'A', 'B', 'C', 'D'});

    test_insert_iterator_aux<deque<char>>({});
    test_insert_iterator_aux<deque<char>>({'a'});
    test_insert_iterator_aux<deque<char>>({'a', 'b', 'c'});
    test_insert_iterator_aux<deque<char>>({'A', 'B', 'C', 'D'});

    // struct type
    test_insert_iterator_aux<list<string>>({});
    test_insert_iterator_aux<list<string>>({"hello"});
    test_insert_iterator_aux<list<string>>({"apple", "box", "cat"});
    test_insert_iterator_aux<list<string>>({"Cpp", "Java", "C-sharp", "Python"});

    test_insert_iterator_aux<deque<string>>({});
    test_insert_iterator_aux<deque<string>>({"hello"});
    test_insert_iterator_aux<deque<string>>({"apple", "box", "cat"});
    test_insert_iterator_aux<deque<string>>({"Cpp", "Java", "C-sharp", "Python"});
}


void test_distance()
{

}

void test_advance()
{

}


template<typename Container>
static void test_reverse_iterator_aux(const std::initializer_list<typename Container::value_type> & lst)
{

}


template<typename Container>
static void test_back_iterator_aux(const std::initializer_list<typename Container::value_type> & lst)
{
    using T = typename Container::value_type;
    Container c1 = lst;
    Container c2 = lst;
    vector<T> vi {T(), T(), T()};
    deque<T> di(3);
    list<T> li(di.cbegin(), di.cend());

    auto iter1 = std::back_inserter(c1);
    auto iter2 = stl::back_inserter(c2);
    *iter1 = *vi.begin();
    *iter1 = *(vi.end() - 1);
    *iter1 = *(di.begin() + 1);

    *iter2 = *vi.begin();
    *iter2 = *(vi.end() - 1);
    *iter2 = *(di.begin() + 1);
    
    assert(c1 == c2);
    std::copy(vi.begin(), vi.end(), iter1);
    std::copy(vi.begin(), vi.end(), iter2);
    assert(c1 == c2);
    std::copy(di.begin(), di.end(), iter1);
    std::copy(di.begin(), di.end(), iter2);
    assert(c1 == c2);
    std::copy(li.begin(), li.end(), iter1);
    std::copy(li.begin(), li.end(), iter2);
    assert(c1 == c2);
}

template<typename Container>
static void test_front_iterator_aux(const std::initializer_list<typename Container::value_type> & lst)
{
    using T = typename Container::value_type;
    Container c1 = lst;
    Container c2 = lst;
    vector<T> vi {T(), T(), T()};
    deque<T> di(3);
    list<T> li(di.cbegin(), di.cend());

    auto iter1 = std::front_inserter(c1);
    auto iter2 = stl::front_inserter(c2);
    *iter1 = *vi.begin();
    *iter1 = *(vi.end() - 1);
    *iter1 = *(di.begin() + 1);

    *iter2 = *vi.begin();
    *iter2 = *(vi.end() - 1);
    *iter2 = *(di.begin() + 1);
    
    assert(c1 == c2);
    std::copy(vi.begin(), vi.end(), iter1);
    std::copy(vi.begin(), vi.end(), iter2);
    assert(c1 == c2);
    std::copy(di.begin(), di.end(), iter1);
    std::copy(di.begin(), di.end(), iter2);
    assert(c1 == c2);
    std::copy(li.begin(), li.end(), iter1);
    std::copy(li.begin(), li.end(), iter2);
    assert(c1 == c2);
}


template<typename Container>
static void test_insert_iterator_aux(const std::initializer_list<typename Container::value_type> & lst)
{
    using T = typename Container::value_type;
    Container c1 = lst;
    Container c2 = lst;
    vector<T> vi {T(), T(), T()};
    deque<T> di(3);
    list<T> li(di.cbegin(), di.cend());

    // head insert
    auto iter1 = std::insert_iterator(c1, c1.begin());
    auto iter2 = stl::insert_iterator(c2, c2.begin());
    *iter1 = *vi.begin();
    *iter1 = *(vi.end() - 1);
    *iter1 = *(di.begin() + 1);

    *iter2 = *vi.begin();
    *iter2 = *(vi.end() - 1);
    *iter2 = *(di.begin() + 1);
    
    assert(c1 == c2);
    std::copy(vi.begin(), vi.end(), iter1);
    std::copy(vi.begin(), vi.end(), iter2);
    assert(c1 == c2);
    std::copy(di.begin(), di.end(), iter1);
    std::copy(di.begin(), di.end(), iter2);
    assert(c1 == c2);
    std::copy(li.begin(), li.end(), iter1);
    std::copy(li.begin(), li.end(), iter2);
    assert(c1 == c2);

    // body insert
    auto step = c1.size() / 2;
    auto t1 = c1.begin();
    auto t2 = c2.begin();

    while (step--)
    {
        ++t1;
        ++t2;
    }
    iter1 = std::insert_iterator(c1, t1);
    iter2 = stl::insert_iterator(c2, t2);
    *iter1 = *vi.begin();
    *iter1 = *(vi.end() - 1);
    *iter1 = *(di.begin() + 1);

    *iter2 = *vi.begin();
    *iter2 = *(vi.end() - 1);
    *iter2 = *(di.begin() + 1);
    
    assert(c1 == c2);
    std::copy(vi.begin(), vi.end(), iter1);
    std::copy(vi.begin(), vi.end(), iter2);
    assert(c1 == c2);
    std::copy(di.begin(), di.end(), iter1);
    std::copy(di.begin(), di.end(), iter2);
    assert(c1 == c2);
    std::copy(li.begin(), li.end(), iter1);
    std::copy(li.begin(), li.end(), iter2);
    assert(c1 == c2);

    // tail insert
    iter1 = std::insert_iterator(c1, c1.end());
    iter2 = stl::insert_iterator(c2, c2.end());
    *iter1 = *vi.begin();
    *iter1 = *(vi.end() - 1);
    *iter1 = *(di.begin() + 1);

    *iter2 = *vi.begin();
    *iter2 = *(vi.end() - 1);
    *iter2 = *(di.begin() + 1);
    
    assert(c1 == c2);
    std::copy(vi.begin(), vi.end(), iter1);
    std::copy(vi.begin(), vi.end(), iter2);
    assert(c1 == c2);
    std::copy(di.begin(), di.end(), iter1);
    std::copy(di.begin(), di.end(), iter2);
    assert(c1 == c2);
    std::copy(li.begin(), li.end(), iter1);
    std::copy(li.begin(), li.end(), iter2);
    assert(c1 == c2);
}

