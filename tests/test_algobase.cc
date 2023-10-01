//
// Created by rda on 2023/10/01.
//

#include <iostream>
#include <cassert>
#include <list>
#include <vector>
#include <deque>
#include <algorithm>

#include "list.hh"
#include "vector.hh"
#include "deque.hh"
#include "algobase.hh"

using std::cout;
using std::endl;

template <typename Container1, typename Container2>
void test_search()
{
    std::vector<int> elems{1, 3, 5, 5, 7, 7, 7, 9, 9, 10};
    Container1 c1(elems.begin(), elems.end());
    Container2 ref(elems.begin(), elems.end());

    for (int i = 0; i < 11; ++i)
    {
        auto iter1 = stl::lower_bound(c1.begin(), c1.end(), i);
        auto iter2 = stl::lower_bound(ref.begin(), ref.end(), i);
        assert((iter1 == c1.end() && iter2 == ref.end()) || (*iter1 == *iter2));

        auto iter3 = stl::upper_bound(c1.begin(), c1.end(), i);
        auto iter4 = stl::upper_bound(ref.begin(), ref.end(), i);

        assert((iter3 == c1.end() && iter4 == ref.end()) || (*iter3 == *iter4));

        auto iter5 = stl::lower_bound(ref.begin(), ref.end(), i);
        auto iter6 = std::lower_bound(ref.begin(), ref.end(), i);

        assert((iter5 == ref.end() && iter6 == ref.end()) || (*iter5 == *iter6));

        auto iter7 = stl::upper_bound(ref.begin(), ref.end(), i);
        auto iter8 = std::upper_bound(ref.begin(), ref.end(), i);
        assert((iter7 == ref.end() && iter8 == ref.end()) || (*iter7 == *iter8));
    }
}

int main()
{
    test_search<stl::vector<int>, std::vector<int>>();
    test_search<stl::list<int>, std::list<int>>();
    test_search<stl::deque<int>, std::deque<int>>();

    return 0;
}