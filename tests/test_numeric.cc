// 
// 参考《STL源码剖析》P298 测试程序
// 

#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <cassert>

#include "numeric.hh"

using namespace std;

int main()
{
    int ia[5] = {1,2,3,4,5};
    vector<int> iv{ia, ia+5};

    // test accumulate 
    assert(stl::accumulate(iv.begin(),iv.end(), 0) == 15);
    assert(stl::accumulate(iv.begin(), iv.end(), 0, minus<int>()) == -15);

    // test inner_product 
    assert(stl::inner_product(iv.begin(), iv.end(), iv.begin(), 10) == 65);
    assert(stl::inner_product(iv.begin(), iv.end(), iv.begin(), 10, minus<int>(), plus<int>()) == -20);

    // test partial_sum 
    ostream_iterator<int> oite(cout, " ");
    stl::partial_sum(iv.begin(), iv.end(), oite); // 1 3 6 10 15
    stl::partial_sum(iv.begin(), iv.end(), oite, minus<int>()); // 1 -1 -4 -8 -13

    // test adjacent_difference 
    stl::adjacent_difference(iv.begin(), iv.end(), oite); // 1 1 1 1 1
    stl::adjacent_difference(iv.begin(), iv.end(), oite, plus<int>()); // 1 3 5 7 9

    // test power 
    assert(stl::power(10, 3) == 1000);
    assert(stl::power(10, 3, plus<int>()) == 30);

    // test iota 
    int n = 3;
    stl::iota(iv.begin(), iv.end(), n);
    for (int i = 0; i < n; ++i)
        assert(iv[i] == n + i);

    return 0;
}