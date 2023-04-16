#include <iostream>
#include <algorithm>
#include <cassert>
#include "array.hh"
using namespace std;

struct Item
{
    void * ptr;

    Item()=default;
    Item(const Item & rhs)
    {
        ptr = rhs.ptr;
        cout << "copy-constructor" << endl;
    }

    Item(Item && rhs)
    {
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
        cout << "move-constructor" << endl;
    }

    Item & operator=(const Item & rhs)
    {
        if (this != &rhs)
            ptr = rhs.ptr;
        cout << "copy-assignment" << endl;
        return *this;
    }

    Item & operator=(Item && rhs)
    {
        if (this != &rhs)
        {
            ptr = rhs.ptr;
            rhs.ptr = nullptr;
            cout << "move-assginment" << endl;
        }
        
        return *this;
    }
};

template <typename Array>
void test_array()
{
    Array a1{0};
    cout << "======a1=======\n";
    cout << "size: " << a1.size() << endl;
    cout << "max_size: " << a1.max_size() << endl;
    cout << "content: ";
    for (auto &i : a1)
        cout << i << ' ';
    cout << endl;

    Array a2{1, 2, 3, 4};
    cout << "======a2=======\n";
    cout << "size: " << a2.size() << endl;
    cout << "max_size: " << a2.max_size() << endl;
    cout << "content: ";
    for (auto &i : a2)
        cout << i << ' ';
    cout << endl;

    Array a3{1, 2, 3, 3};
    cout << "======a3=======\n";
    cout << "content: ";
    for (auto &i : a3)
        cout << i << ' ';
    cout << endl;
    cout << "(a2 < a3): " << (a2 < a3) << endl;
    cout << "(a2 >= a3): " << (a2 >= a3) << endl;

    Array a4 = a3;
    cout << "======a4=======\n";
    cout << "content: ";
    for (auto &i : a4)
        cout << i << ' ';
    cout << endl;
    cout << "a4 front: " << a4.front() << endl;
    cout << "a4 back: " << a4.back() << endl;
    a4[2] *= 2;
    cout << "a4[2]: " << a4[2] << endl;
    a1.swap(a4);
    cout << "after swap(a1, a4)\n";
    cout << "a1: ";
    for (auto &i : a1)
        cout << i << ' ';
    cout << endl;
    cout << "a4: ";
    for (auto &i : a4)
        cout << i << ' ';
    cout << endl;
}

void test_constructors()
{
    printf("=============%s=================\n", __FUNCTION__);
    // default constructor
    stl::array<int, 5> ar1;
    for (auto &i : ar1)
        cout << i << ' ';
    cout << endl;

    // initializer-list constructor
    stl::array<double, 5> ar2{1.2, 3.4, 5.6, 9.0};
    for (auto &i : ar2)
        cout << i << ' ';
    cout << endl;

    // copy-constructor
    stl::array<double, 5> ar3(ar2);
    for (auto &i : ar2)
        i *= 2;
    for (auto &i : ar2)
        cout << i << ' ';
    cout << endl;
    // move-constructor
    stl::array<double, 5> ar4(std::move(ar3));
    for (auto &i : ar3)
        cout << i << ' ';
    cout << endl;
}

void test_compare()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::array<int, 5> ar1{1,3,5,6,7};
    stl::array<int, 5> ar2{1,3,5,6,7};
    assert(ar1 == ar2);
    assert(ar1 == ar1);
    assert(ar1 >= ar1);
    assert(ar1 <= ar1);
    ar1.back() = 6;
    assert(ar1 != ar2);
    assert(ar1 < ar2);
    assert(ar1 <= ar2);

    stl::array<int, 5> ar3{1,3,5};
    stl::array<int, 5> ar4{1,3,5,6,7};
    assert(ar3 != ar4);
    assert(ar3 < ar4);
    assert(ar3 <= ar4);
    ar3[0] = 12;
    assert(ar3 > ar4);
    assert(ar3 >= ar4);
}

void test_assignment()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::array<double, 4> ad1{3.14, 6.18, 9.9, 10.10};
    stl::array<double, 4> ad2;
    ad2 = ad1;

    for (int i = 0; i < 4; ++i)
    {
        assert(ad1[i] == ad2[i]);
        ad1[i] *= 2;
        assert(ad1[i] == 2 * ad2[i]);
    }
    
    swap(ad1, ad2);
    for (int i = 0; i < 4; ++i)
        assert(ad2[i] == 2 * ad1[i]);

    stl::array<Item, 3> ax;
    stl::array<Item, 3> ay;
    ay = ax;
    for (int i = 0; i < 3; ++i)
        assert(ax[i].ptr == ay[i].ptr);

    // test move-assignment
    stl::array<Item, 5> ai1;
    stl::array<Item, 5> ai2;

    ai2 = std::move(ai1);
    for (auto & item : ai1)
        assert(item.ptr == nullptr);

}

void test_access()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::array<int, 5> ar{1,3,5};
    for (int i = 0; i < 5; ++i)
        cout << ar[i] << ' ' << ar.at(i) << endl;
    cout << endl;
    // test at throw exception
    try
    {
        cout << ar.at(5);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    ar.front() = 9;
    ar.back() = 12;
    assert(ar[0] == 9);
    assert(ar.at(ar.size() - 1) == 12);

    const stl::array<int, 3> car{9,8,7};
    // car[1] = 12; error
    assert(car[0] == 9);
    assert(car[1] == 8);
    assert(car[2] == 7);
}

void test_iter_method()
{
    printf("=============%s=================\n", __FUNCTION__);
    // begin end
    stl::array<long, 10> al{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    cout << "begin: " << *al.begin() << endl;
    cout << "end: " << *(al.end() - 1) << endl;
    array<long, 10>::iterator bg = al.begin();
    *bg = 11;
    auto ed = al.end();
    *(ed - 1) = 12;
    cout << "begin: " << *al.begin() << endl;
    cout << "end: " << *(al.end() - 1) << endl;

    // cbegin cend
    auto cbg = al.cbegin();
    // *cbg = 12;  // error. cannot modify const iterator
    auto ced = al.cend();
    while (cbg != ced)
        cout << *cbg++ << ' ';
    cout << endl;

    // // rbegin rend
    // auto rbg = al.rbegin();
    // auto red = al.rend();
    // while (rbg != red)
    //     *rbg++ *= 2;
    // rbg = al.rbegin();
    // red = al.rend();
    // while (rbg != red)
    //     cout << *rbg-- << ' ';
    // cout << endl;
}

int main()
{
    test_constructors();
    test_compare();
    test_assignment();
    test_access();
    test_iter_method();

    return 0;
}
