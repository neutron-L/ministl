#include <iostream>
#include <algorithm>
#include <cassert>

#include "complex.hh"
#include "string.hh"
#include "array.hh"
#include "complex.hh"

#include "test_iter.hh"
using namespace std;

stl::array<int, 3> glob_ar;

void test_constructors()
{
    printf("=============%s=================\n", __FUNCTION__);
    const size_t N = 5;
    std::cout << "----test default constructor----\n";
    // default constructor
    for (auto &i : glob_ar)
        assert(!i);
    stl::array<int, 3> ar1;
    for (auto &i : ar1)
        cout << i << ' ';
    cout << endl;

    // initializer-list constructor
    std::cout << "----test initializer-list constructor----\n";
    stl::array<double, N> ar2{1.2, 3.4, 5.6, 9.0};
    for (auto &i : ar2)
        cout << i << ' ';
    cout << endl;
    stl::array<double, N> ar{};
    for (auto &i : ar)
        assert(!i);

    // copy-constructor
    std::cout << "----test copy constructor----\n";
    stl::array<double, N> ar3(ar2);
    for (auto &i : ar2)
        i *= 2;
    for (int i = 0; i < N; ++i)
        assert(ar2[i] == 2 * ar3[i]);

    // move-constructor
    std::cout << "----test move constructor----\n";
    stl::array<complex, N> ac1{complex(1.1, 2.2), complex(3.1, 4.2), complex(11, 22)};
    stl::array<complex, N> ac2(std::move(ac1));
    for (int i = 0; i < N; ++i)
        assert(ac1[i] == ac2[i]);

    const char *str[]{"one", "two", "three", "four", "five"};
    stl::array<String, N> as1{String(str[0]), String(str[1]), String(str[2]), String(str[3]), String(str[4])};
    stl::array<String, N> as2(std::move(as1));
    for (int i = 0; i < N; ++i)
        assert(!strcmp(as2[i].get_c_str(), str[i]) && as1[i].get_c_str() == nullptr);
}

void test_access()
{
    printf("=============%s=================\n", __FUNCTION__);
    const int N = 5;
    stl::array<int, N> ar{1, 3, 5};
    const stl::array<int, N> car{1, 3, 5};
    for (int i = 0; i < 5; ++i)
    {
        assert(ar[i] == ar.at(i));
        assert(car[i] == car.at(i));
    }
    // test at throw exception
    try
    {
        cout << ar.at(5);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        cout << car.at(-1);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    ar.front() = 9;
    ar.back() = 12;
    assert(ar[0] == 9 && ar[0] == ar.front());
    assert(ar.at(ar.size() - 1) == 12 && ar[ar.size() - 1] == ar.back());

    assert(car[0] == 1);
    assert(car[1] == 3);
    assert(car[2] == 5);
    // car.front() = 9; // error
    // car.at(1) = 12; // error
    // car.data()[2] = 13; // error

    int *d = ar.data();
    const int *dc = car.data();
    for (int i = 0; i < N; ++i)
    {
        assert(d[i] == ar.at(i));
        assert(dc[i] == car.at(i));
    }
}

// void test_iterators_by_obj()
// {
//     printf("=============%s=================\n", __FUNCTION__);
//     const int num = 10;
//     stl::array<long, num> al{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
//     // begin end access
//     std::cout << "----test begin/end----\n";
//     assert(*al.begin() == al[0]);
//     assert(*(al.end() - 1) == al.back());

//     // begin end modify
//     stl::array<long, num>::iterator bg = al.begin();
//     *bg = 11;
//     auto ed = al.end();
//     *(ed - 1) = 12;
//     assert(*al.begin() == 11 && al[0] == 11);
//     assert(*(al.end() - 1) == 12 && al.back() == 12);

//     // cbegin cend
//     std::cout << "----test cbegin/cend----\n";

//     auto cbg = al.cbegin();
//     // *cbg = 12;  // error. cannot modify const iterator
//     auto ced = al.cend();
//     // *ced = 11; // error
//     int i = 0;
//     while (cbg != ced)
//         assert(*cbg++ == al[i++]);
//     assert(i == num);

//     cbg = al.cbegin();
//     ced = al.cend();
//     i = num;
//     while (ced != cbg)
//         assert(*--ced == al[--i]);
//     assert(!i);

//     // rbegin rend
//     std::cout << "----test rbegin/rend----\n";
//     auto rbg = al.rbegin();
//     auto red = al.rend();
//     stl::array<long, num> mirror = al;

//     while (rbg != red)
//         *rbg++ *= 2;
//     rbg = al.rbegin();
//     red = al.rend();
//     i = num;
//     while (rbg != red)
//         assert(*rbg++ == 2 * mirror[--i]);
//     assert(!i);

//     rbg = al.rbegin();
//     red = al.rend();
//     i = 0;
//     while (rbg != red)
//         assert(*--red == al[i++]);
//     assert(i == num);

//     // crbegin crend
//     std::cout << "----test crbegin/crend----\n";

//     auto crbg = al.crbegin();
//     // *crbg = 12;  // error. cannot modify const iterator
//     auto cred = al.crend();
//     // *cred = 11; // error
//     i = num;
//     while (crbg != cred)
//         assert(*crbg++ == al[--i]);
//     assert(!i);

//     crbg = al.crbegin();
//     cred = al.crend();
//     i = 0;
//     while (crbg != cred)
//         assert(*--cred == al[i++]);
//     assert(i == num);
// }

// void test_iterators_by_const_obj()
// {
//     printf("=============%s=================\n", __FUNCTION__);
//     const int num = 10;
//     const stl::array<long, num> al{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
//     // begin end
//     std::cout << "----test begin/end----\n";
//     assert(*al.begin() == al[0]);
//     assert(*(al.end() - 1) == al.back());

//     stl::array<long, num>::const_iterator bg = al.begin();
//     // *bg = 11; // error
//     auto ed = al.end();
//     // *(ed - 1) = 12; // error

//     // cbegin cend
//     std::cout << "----test cbegin/cend----\n";

//     auto cbg = al.cbegin();
//     // *cbg = 12;  // error. cannot modify const iterator
//     auto ced = al.cend();
//     // *ced = 11; // error
//     int i = 0;
//     while (cbg != ced)
//         assert(*cbg++ == al[i++]);
//     assert(i == num);

//     cbg = al.cbegin();
//     ced = al.cend();
//     i = num;
//     while (ced != cbg)
//         assert(*--ced == al[--i]);
//     assert(!i);

//     // rbegin rend
//     std::cout << "----test rbegin/rend----\n";
//     auto rbg = al.rbegin();
//     auto red = al.rend();
//     // *rbg += 14; // error
//     // *red += 14; // error

//     i = num;
//     while (rbg != red)
//         assert(*rbg++ == al[--i]);
//     assert(!i);

//     rbg = al.rbegin();
//     red = al.rend();
//     i = 0;
//     while (rbg != red)
//         assert(*--red == al[i++]);
//     assert(i == num);

//     // crbegin crend
//     std::cout << "----test crbegin/crend----\n";

//     auto crbg = al.crbegin();
//     // *crbg = 12;  // error. cannot modify const iterator
//     auto cred = al.crend();
//     // *cred = 11; // error
//     i = num;
//     while (crbg != cred)
//         assert(*crbg++ == al[--i]);
//     assert(!i);

//     crbg = al.crbegin();
//     cred = al.crend();
//     i = 0;
//     while (crbg != cred)
//         assert(*--cred == al[i++]);
//     assert(i == num);
// }

void test_capacity()
{
    printf("=============%s=================\n", __FUNCTION__);
    const int num = 10;
    const stl::array<long, num> cal{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    stl::array<int, num> ai;

    assert(!cal.empty());
    assert(!ai.empty());
    assert(cal.size() == num && ai.size() == num);
    assert(cal.max_size() == num && ai.max_size() == num);
}

void test_operations_built_in_type()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::array<double, 4> ad1{3.14, 6.18, 9.9, 10.10};
    stl::array<double, 4> ad2;

    std::cout << "----test operator=----\n";
    ad2 = ad1;

    for (int i = 0; i < 4; ++i)
    {
        assert(ad1[i] == ad2[i]);
        ad1[i] *= 2;
        assert(ad1[i] == 2 * ad2[i]);
    }

    std::cout << "----test swap----\n";
    // stl::swap(ad1, ad2);
    // for (int i = 0; i < 4; ++i)
    //     assert(ad2[i] == 2 * ad1[i]);

    // test fill
    std::cout << "----test fill----\n";
    ad1.fill(1.2);
    for (auto &i : ad1)
        assert(i == 1.2);
}

void test_operations_user_defined_type()
{
    printf("=============%s=================\n", __FUNCTION__);
    const int N = 3;
    stl::array<complex, N> ad1{complex(1, 2), complex(3, 4), complex(5, 6)};
    stl::array<complex, N> ad2;

    std::cout << "----test operator=----\n";
    ad2 = ad1;

    for (int i = 0; i < N; ++i)
        assert(ad1[i] == ad2[i]);

    std::cout << "----test swap----\n";
    // stl::swap(ad1, ad2);
    // for (int i = 0; i < 4; ++i)
    //     assert(ad2[i] == ad1[i]);

    // test fill
    std::cout << "----test fill----\n";
    ad1.fill(1.2);
    for (auto &i : ad1)
        assert(i == 1.2);

        // test move
    std::cout << "----test move----\n";
    const char *str[]{"one", "two", "three"};
    stl::array<String, N> as1{String(str[0]), String(str[1]), String(str[2])};
    stl::array<String, N> as2;

    as2 = std::move(as1);
    for (int i = 0; i < N; ++i)
        assert(!strcmp(as2[i].get_c_str(), str[i]) && as1[i].get_c_str() == nullptr);
}

void test_compare()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::array<int, 5> ar1{1, 3, 5, 6, 7};
    stl::array<int, 5> ar2{1, 3, 5, 6, 7};
    assert(ar1 == ar2);
    assert(ar1 == ar1);
    assert(ar1 >= ar1);
    assert(ar1 <= ar1);
    ar1.back() = 6;
    assert(ar1 != ar2);
    assert(ar1 < ar2);
    assert(ar1 <= ar2);

    stl::array<int, 5> ar3{1, 3, 5};
    stl::array<int, 5> ar4{1, 3, 5, 6, 7};
    assert(ar3 != ar4);
    assert(ar3 < ar4);
    assert(ar3 <= ar4);
    ar3[0] = 12;
    assert(ar3 > ar4);
    assert(ar3 >= ar4);
}

int main()
{
    test_constructors();
    test_access();
    test_iterators_by_obj<stl::array<int, 10>>({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
    test_iterators_by_const_obj<stl::array<int, 10>>({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
    test_iterators_by_obj<stl::array<complex, 5>>({complex(1, 2), complex(3, 4), complex(5, 6), complex(7, 8), complex(9, 0)});
    test_iterators_by_const_obj<stl::array<complex, 5>>({complex(1, 2), complex(3, 4), complex(5, 6), complex(7, 8), complex(9, 0)});
    test_capacity();
    test_operations_built_in_type();
    test_operations_user_defined_type();
    
    std::cout << "Pass!\n";

    return 0;
}
