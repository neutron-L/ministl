#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>
#include <deque>
#include "iterator.hh"
using namespace std;

void test_back_iterator()
{
    vector<int> vi{1, 2, 3, 4};
    list<int> li{11, 12, 13, 14};
    auto iter = vi.begin() + 2;
    insert_iterator<vector<int>> ist(vi, iter);
    *ist = 31;
    *ist = 15;
    for (auto &i: vi)
        cout << i << ' ';
    cout << endl;

    auto bi = back_inserter(vi);
    std::copy(li.begin(), li.end(), bi);
    for (auto &i: vi)
        cout << i << ' ';
    cout << endl;
    auto fi = front_inserter(li);
    std::copy(vi.begin(), vi.end(), fi);
    for (auto &i: li)
        cout << i << ' ';
}


void test_front_iterator()
{

}


void test_insert_iterator()
{

}



int main()
{
    test_back_iterator();
    test_front_iterator();
    test_insert_iterator();

    return 0;
}
