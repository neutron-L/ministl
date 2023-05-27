//
// Created by rda on 2023/5/27.
//

#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <cassert>

#include "numeric.hh"

using namespace std;

void test_accumulate();
void test_inner_product();
void test_adjacent_difference();
void test_partial_sum();
void test();

int main()
{
    test_accumulate();
    test_inner_product();
    test_adjacent_difference();
    test_partial_sum();
    test();
    cout << "Pass!\n";

    return 0;
}


//
// From https://en.cppreference.com/w/cpp/algorithm/accumulate example
//
/* output
sum: 55
product: 3628800
dash-separated string: 1-2-3-4-5-6-7-8-9-10
dash-separated string (right-folded): 10-9-8-7-6-5-4-3-2-1
*/
void test_accumulate()
{
    printf("===========%s===========\n", __FUNCTION__);
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int sum = stl::accumulate(v.begin(), v.end(), 0);
    int product = stl::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

    auto dash_fold = [](std::string a, int b)
    {
        return std::move(a) + '-' + std::to_string(b);
    };

    std::string s = stl::accumulate(std::next(v.begin()), v.end(),
                                    std::to_string(v[0]), // start with first element
                                    dash_fold);

    // Right fold using reverse iterators
    std::string rs = stl::accumulate(std::next(v.rbegin()), v.rend(),
                                     std::to_string(v.back()), // start with last element
                                     dash_fold);

    std::cout << "sum: " << sum << '\n'
              << "product: " << product << '\n'
              << "dash-separated string: " << s << '\n'
              << "dash-separated string (right-folded): " << rs << '\n';
}

//
// From https://en.cppreference.com/w/cpp/algorithm/inner_product example
//
/* output
Inner product of a and b: 21
Number of pairwise matches between a and b: 2
*/
void test_inner_product()
{
    printf("===========%s===========\n", __FUNCTION__);
    vector<int> a{0, 1, 2, 3, 4};
    vector<int> b{5, 4, 2, 3, 1};

    int r1 = stl::inner_product(a.begin(), a.end(), b.begin(), 0);
std::cout << "Inner product of a and b: " << r1 << '\n';
    int r2 = stl::inner_product(a.begin(), a.end(), b.begin(), 0,
                                std::plus<>(), std::equal_to<>());
std::cout << "Number of pairwise matches between a and b: " <<  r2 << '\n';
}

//
// From https://en.cppreference.com/w/cpp/algorithm/adjacent_difference example
//
/* output
Initially, v = 4 6 9 13 18 19 19 15 10 
Modified v = 4 2 3 4 5 1 0 -4 -5 
Fibonacci, a = 1 1 2 3 5 8 13 21 34 55
*/
void test_adjacent_difference()
{
    printf("===========%s===========\n", __FUNCTION__);
    auto print = [](auto comment, const auto &sequence)
    {
        std::cout << comment;
        for (const auto &n : sequence)
            std::cout << n << ' ';
        std::cout << endl;
    };

    vector<int> v{4, 6, 9, 13, 18, 19, 19, 15, 10};
    print("Initially, v = ", v);

    stl::adjacent_difference(v.begin(), v.end(), v.begin());
    print("Modified v = ", v);

    // Fibonacci
    std::array<int, 10> a{1};
    stl::adjacent_difference(a.begin(), std::prev(std::end(a)),
                             std::next(std::begin(a)), std::plus<>{});
    print("Fibonacci, a = ", a);
}

//
// From https://en.cppreference.com/w/cpp/algorithm/partial_sum example
//
/* output
The first 10 even numbers are: 2 4 6 8 10 12 14 16 18 20 
The first 10 powers of 2 are: 2 4 8 16 32 64 128 256 512 1024
*/
void test_partial_sum()
{
     std::vector<int> v(10, 2); // v = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
 
    std::cout << "The first " << v.size() << " even numbers are: ";
    // write the result to the cout stream
    stl::partial_sum(v.cbegin(), v.cend(), 
                     std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
 
    // write the result back to the vector v
    stl::partial_sum(v.cbegin(), v.cend(),
                     v.begin(), std::multiplies<int>());
 
    std::cout << "The first " << v.size() << " powers of 2 are: ";
    for (int n : v)
        std::cout << n << ' ';
    std::cout << '\n';
}

//
// 参考《STL源码剖析》P298 测试程序(不知道定义个什么名字...,就叫test好了)
//
void test()
{
    printf("===========%s===========\n", __FUNCTION__);
    int ia[5] = {1, 2, 3, 4, 5};
    vector<int> iv{ia, ia + 5};

    // test accumulate
    assert(stl::accumulate(iv.begin(), iv.end(), 0) == 15);
    assert(stl::accumulate(iv.begin(), iv.end(), 0, minus<int>()) == -15);

    // test inner_product
    assert(stl::inner_product(iv.begin(), iv.end(), iv.begin(), 10) == 65);
    assert(stl::inner_product(iv.begin(), iv.end(), iv.begin(), 10, minus<int>(), plus<int>()) == -20);

    // test partial_sum
    ostream_iterator<int> oite(cout, " ");
    stl::partial_sum(iv.begin(), iv.end(), oite); // 1 3 6 10 15
    cout << endl;              
    stl::partial_sum(iv.begin(), iv.end(), oite, minus<int>()); // 1 -1 -4 -8 -13
    cout << endl;

    // test adjacent_difference
    stl::adjacent_difference(iv.begin(), iv.end(), oite);              // 1 1 1 1 1
    cout << endl;
    stl::adjacent_difference(iv.begin(), iv.end(), oite, plus<int>()); // 1 3 5 7 9

    // test power
    // assert(stl::power(10, 3) == 1000);
    // assert(stl::power(10, 3, plus<int>()) == 30);

    // test iota
    int n = 3;
    stl::iota(iv.begin(), iv.end(), n);
    for (int i = 0; i < n; ++i)
        assert(iv[i] == n + i);
    cout << endl;
}
