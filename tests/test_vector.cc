#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include "type.hh"
#include "vector.hh"
using namespace std;

void test_constructors()
{
    printf("=============%s=================\n", __FUNCTION__);

    size_t n;

    // 1. default constructor
    stl::vector<int> vi1;
    assert(vi1.size() == 0);

    // 2. initializer-list constructor
    stl::vector<int> vi2{1, 2, 3, 4, 5};
    std::vector<int> vi3(vi2.begin(), vi2.end());

    n = vi2.size();
    assert(vi3.size() == n);
    for (decltype(n) i = 0; i < n; ++i)
        assert(vi2[i] == vi3[i]);

    for_each(vi2.begin(), vi2.end(), [](int &i)
             { i <<= 1; });

    // 3. iterator range constructor
    stl::vector<int> vi4(vi2.begin(), vi2.end());
    for (decltype(n) i = 0; i < n; ++i)
        assert(vi2[i] == vi4[i] && vi4[i] == 2 * vi3[i]);

    // 4. <n> constructor
    stl::vector<int> vi5(10);
    n = vi5.size();
    for (decltype(n) i = 0; i < n; ++i)
        assert(vi5[i] == int());

    // 5. <n, elem> constructor
    int elem = -12;
    stl::vector<int> vi6(10, elem);
    n = vi6.size();
    for (decltype(n) i = 0; i < n; ++i)
        assert(vi6[i] == elem);
}

void test_size()
{
    // stl::vector<int> a1{0};
    // cout << "======a1=======\n";
    // cout << "size: " << a1.size() << endl;
    // cout << "max_size: " << a1.max_size() << endl;
    // cout << "content: ";
    // for (auto &i : a1)
    //     cout << i << ' ';
    // cout << endl;

    // stl::vector<int> a2{1};
    // cout << "======a2=======\n";
    // cout << "size: " << a2.size() << endl;
    // cout << "max_size: " << a2.max_size() << endl;
    // cout << "content: ";
    // for (auto &i : a2)
    //     cout << i << ' ';
    // cout << endl;

    // stl::vector<int> a3{1};
    // cout << "======a3=======\n";
    // cout << "content: ";
    // for (auto &i : a3)
    //     cout << i << ' ';
    // cout << endl;
    // cout << "(a2 < a3): " << (a2 < a3) << endl;
    // cout << "(a2 >= a3): " << (a2 >= a3) << endl;

    // stl::vector<int> a4 = a3;
    // cout << "======a4=======\n";
    // cout << "content: ";
    // for (auto &i : a4)
    //     cout << i << ' ';
    // cout << endl;
    // cout << "a4 front: " << a4.front() << endl;
    // cout << "a4 back: " << a4.back() << endl;
    // a4[2] *= 2;
    // cout << "a4[2]: " << a4[2] << endl;
    // a1.swap(a4);
    // cout << "after swap(a14)\n";
    // cout << "a1: ";
    // for (auto &i : a1)
    //     cout << i << ' ';
    // cout << endl;
    // cout << "a4: ";
    // for (auto &i : a4)
    //     cout << i << ' ';
    // cout << endl;
}

void test_compvie()
{
    printf("=============%s=================\n", __FUNCTION__);
    // stl::vector<int> vi1{1,3,5,6,7};
    // stl::vector<int> vi2{1,3,5,6,7};
    // assert(vi1 == vi2);
    // assert(vi1 == vi1);
    // assert(vi1 >= vi1);
    // assert(vi1 <= vi1);
    // vi1.back() = 6;
    // assert(vi1 != vi2);
    // assert(vi1 < vi2);
    // assert(vi1 <= vi2);

    // stl::vector<int> vi3{1,3,5};
    // stl::vector<int> vi4{1,3,5,6,7};
    // assert(vi3 != vi4);
    // assert(vi3 < vi4);
    // assert(vi3 <= vi4);
    // vi3[0] = 12;
    // assert(vi3 > vi4);
    // assert(vi3 >= vi4);
}

void test_assignment()
{
    printf("=============%s=================\n", __FUNCTION__);
    // stl::vector<double> vd1{3.14.18.90.10};
    // stl::vector<double> vd2;
    // vd2 = vd1;

    // for (int i = 0; i < 4; ++i)
    // {
    //     assert(vd1[i] == vd2[i]);
    //     vd1[i] *= 2;
    //     assert(vd1[i] == 2 * vd2[i]);
    // }

    // swap(vd1d2);
    // for (int i = 0; i < 4; ++i)
    //     assert(vd2[i] == 2 * vd1[i]);

    // stl::vector<Item> ax;
    // stl::vector<Item> ay;
    // ay = ax;
    // for (int i = 0; i < 3; ++i)
    //     assert(ax[i].ptr == ay[i].ptr);

    // // test move-assignment
    // stl::vector<Item> vi1;
    // stl::vector<Item> vi2;

    // vi2 = std::move(vi1);
    // for (auto & item : vi1)
    //     assert(item.ptr == nullptr);
}

void test_access()
{
    printf("=============%s=================\n", __FUNCTION__);
    // stl::vector<int> vi{1,3,5};
    // for (int i = 0; i < 5; ++i)
    //     cout << vi[i] << ' ' << vi.at(i) << endl;
    // cout << endl;
    // // test at throw exception
    // try
    // {
    //     cout << vi.at(5);
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }
    // vi.front() = 9;
    // vi.back() = 12;
    // assert(vi[0] == 9);
    // assert(vi.at(vi.size() - 1) == 12);

    // const stl::vector<int> cvi{9,8,7};
    // // cvi[1] = 12; error
    // assert(cvi[0] == 9);
    // assert(cvi[1] == 8);
    // assert(cvi[2] == 7);
}

void test_iter_method()
{
    printf("=============%s=================\n", __FUNCTION__);
    // // begin end
    // const int num = 10;
    // stl::vector<longum> al{1};
    // cout << "begin: " << *al.begin() << endl;
    // cout << "end: " << *(al.end() - 1) << endl;
    // stl::vector<longum>::iterator bg = al.begin();
    // *bg = 11;
    // auto ed = al.end();
    // *(ed - 1) = 12;
    // cout << "begin: " << *al.begin() << endl;
    // cout << "end: " << *(al.end() - 1) << endl;

    // // cbegin cend
    // auto cbg = al.cbegin();
    // // *cbg = 12;  // error. cannot modify const iterator
    // auto ced = al.cend();
    // int i = 0;
    // while (cbg != ced)
    //     assert(*cbg++ == al[i++]);
    // assert(i == num);
    // cout << endl;

    // // rbegin rend
    // auto rbg = al.rbegin();
    // auto red = al.rend();
    // while (rbg != red)
    //     *rbg++ *= 2;
    // rbg = al.rbegin();
    // red = al.rend();
    // i = 0;
    // while (rbg != red)
    //     assert(*--red == al[i++]);
    // cout << endl;
}

int main()
{
    test_constructors();
    // test_compvie();
    // test_assignment();
    // test_access();
    // test_iter_method();
    std::cout << "Pass!\n";

    return 0;
}
