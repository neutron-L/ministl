#include <iostream>
#include <algorithm>
#include <cassert>
#include <deque>
#include <cassert>
#include "type.hh"
#include "iterator.hh"
#include "complex.hh"
#include "string.hh"
#include "deque.hh"
using namespace std;

const char *str[]{"one", "two", "three", "four", "five"};
const complex cs[]{complex(1.2, 3.14), complex(0, 0), complex(9.14, 4.5), complex(4.2, 6.18)};

auto check_equal = [](auto &l, auto &r, size_t times)
{
    assert(l.size() == r.size());
    auto len = l.size();

    auto li = l.begin();
    auto ri = r.begin();

    while (len--)
    {
        assert(*li == times * *ri);
        ++li;
        ++ri;
    }
};

auto to_str = [](const auto &container)
{
    std::string res;
    for (auto &v : container)
    {
        res += std::to_string(v);
        res += ' ';
    }

    return res;
};

void test_constructors()
{
    printf("=============%s=================\n", __FUNCTION__);

    stl::deque<int>::size_type n;

    // 1. default constructor
    stl::deque<int> di1;
    assert(di1.size() == 0);

    // 2. initializer-deque constructor
    stl::deque<int> di2{1, 2, 3, 4, 5};
    cout << (di2.begin() != di2.end()) << endl;
    cout << (di2.begin() == di2.end()) << endl;
    cout << (di2.begin() < di2.end()) << endl;
    assert(to_str(di2) == "1 2 3 4 5 ");

    // 3. range constructor
    stl::deque<int> di3(di2.begin(), di2.end());

    check_equal(di2, di3, 1);

    for_each(di2.begin(), di2.end(), [](int &i)
             { i <<= 1; });

    // 4. copy constructor
    stl::deque<int> di4(di2);
    check_equal(di4, di2, 1);
    check_equal(di4, di3, 2);

    // 5. <n> constructor
    stl::deque<int> di5(10);
    n = di5.size();
    for (auto &i : di5)
        assert(i == int());

    stl::deque<int> dc1(n);
    stl::deque<String> ds1(n);

    for (auto &i : dc1)
        assert(i == complex());

    for (auto &i : ds1)
        assert(i == String());

    // 6. <n, elem> constructor
    int elem = -12;
    stl::deque<int> di6(10, elem);

    complex c(1.3, 2.4);
    stl::deque<complex> dc2(n, c);
    String str("what??");
    stl::deque<String> ds2(n, str);
    for (auto &i : di6)
        assert(i == elem);
    for (auto &i : dc2)
        assert(i == c);
    for (auto &i : ds2)
        assert(i == str);

    // 7. move constructor
    stl::deque<String> ds3(std::move(ds2));
    assert(ds2.empty() && ds2.begin() == ds2.end());
    for (auto &i : ds3)
        assert(i == str);
}

void test_assignment()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::deque<double>::size_type n;
    stl::deque<double> dd1{3.14, 18.90, 10};
    stl::deque<double> dd2;

    // 1. =(const deque &)
    dd2 = dd1;

    check_equal(dd1, dd2, 1);
    for_each(dd2.begin(), dd2.end(), [](double &i)
             { i *= 2; });
    check_equal(dd2, dd1, 2);

    // 2. =(deque &&)
    stl::deque<double> dd3;
    dd3 = std::move(dd1);
    check_equal(dd2, dd3, 2);

    String s1("What");
    String s2("How");
    n = 256;
    stl::deque<String> ax(n, s1);
    assert(ax.size() == n && ax.front() == s1);
    stl::deque<String> ay;
    assert(ay.empty());
    ay = std::move(ax);
    for_each(ay.begin(), ay.end(), [&](String &i)
             { assert(i == s1); });

    stl::deque<String> az(n, s2);
    ax = az;

    for_each(ax.begin(), ax.end(), [&](String &i)
             { assert(i == s2); });

    // 3. assign(n, elem)
    const complex c(3.14, 6.18);
    stl::deque<complex> dc1(4, c);
    for_each(dc1.begin(), dc1.end(), [&](const complex &i)
             { assert(i == c); });

    // 4&5. = initialized deque & assign(init list)
    const complex c1(1, 2);
    const complex c2(3, 4);
    const complex c3;
    const stl::deque<complex> dc2 = {c, c1, c2, c3};

    auto iter = dc2.begin();
    assert(*iter == c);
    ++iter;
    assert(*iter == c1);
    ++iter;
    assert(*iter == c2);
    assert(*(--dc2.end()) == c3);

    stl::deque<complex> cv;
    cv.assign({c, c1, c2, c3});
    check_equal(dc2, cv, 1);

    // 6. range assign
    stl::deque<String> ds1;
    ds1.assign({String(str[0]), String(str[1]), String(str[2]), String(str[3]), String(str[4])});
    n = 4;
    stl::deque<String> ds2(n);
    for (auto &s : ds2)
        assert(s == String());

    ds2.assign(ds1.begin(), ds1.end());
    std::equal(ds1.begin(), ds1.end(), ds2.begin());
}

void test_capacity()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::deque<double>::size_type n;
    stl::deque<double> dd1{3.14, 18.90, 10};
    stl::deque<double> dd2;

    // 1. empty
    assert(!dd1.empty() && dd2.empty());
    dd2 = dd1;
    assert(!dd2.empty() && dd1.size() == dd2.size());

    // 2. size
    assert(dd1.size() == 3);
    dd1.pop_back();
    assert(dd1.size() == 2);
    dd1.push_front(4);
    assert(dd1.size() == 3);

    dd2.assign({1.2, 3.4, 5.6, 1.0});
    assert(dd2.size() == 4);

    dd2.clear();
    assert(dd2.empty());

    dd2.assign(1233, 1.25);
    for (auto &it : dd2)
        assert(it == 1.25);

    n = 512;
    dd2.resize(n);
    for (auto &it : dd2)
        assert(it == 1.25);
    assert(dd2.size() == n);

    dd2.shrink_to_fit();
    dd2.resize(2 * n, 123);
    assert(dd2.size() == 2 * n);
    for (int i = 0; i < 2 * n; ++i)
    {
        if (i < n)
            assert(dd2[i] == 1.25);
        else
            assert(dd2[i] == 123);
    }

    dd2.shrink_to_fit();
    dd2.resize(n / 2, 123);
    assert(dd2.size() == n / 2);
    for (auto &it : dd2)
        assert(it == 1.25);
    dd1 = std::move(dd2);
    assert(dd1.size() == n / 2);
    for (auto &it : dd1)
        assert(it == 1.25);
    assert(dd2.empty());

    dd2.shrink_to_fit();
    dd2.erase(dd2.begin());
    assert(dd2.empty());

    dd2.shrink_to_fit();
    dd1.erase(dd1.begin());
    assert(dd1.size() == n / 2 - 1);

    size_t len1 = 100, len2 = 321;
    double d1 = 13, d2 = 90;
    dd1.assign(len1, d1);
    dd2.assign(len2, d2);
    assert(dd1.size() == len1 && dd1.size() == stl::distance(dd1.begin(), dd1.end()));
    assert(dd2.size() == len2 && dd1.size() == stl::distance(dd1.begin(), dd1.end()));

    for (auto &i : dd1)
        assert(i == d1);
    for (auto &i : dd2)
        assert(i == d2);

    dd1.swap(dd2);

    assert(dd1.size() == len2 && dd1.size() == stl::distance(dd1.begin(), dd1.end()));
    assert(dd2.size() == len1 && dd1.size() == stl::distance(dd1.begin(), dd1.end()));

    for (auto &i : dd1)
        assert(i == d2);
    for (auto &i : dd2)
        assert(i == d1);
}

void test_modifiers_built_in_types()
{
    printf("=============%s=================\n", __FUNCTION__);

    static int arr[] = {11, 12, 13, 14, 15};
    /* Insert */
    stl::deque<int> di;
    static stl::deque<int>::size_type len = 5;

    // 1.1 insert lvalue
    stl::deque<int>::iterator iter;
    for (auto &i : arr)
    {
        iter = di.insert(di.begin(), i);
        assert(*iter == i);
    }
    // 15 14 13 12 11
    int i = 0;
    for_each(di.rbegin(), di.rend(), [&i](auto &item)
             { assert(item == arr[i++]); });
    assert(to_str(di) == "15 14 13 12 11 ");

    // 1.2 insert rvalue
    di.shrink_to_fit();
    iter = di.begin();
    ++iter;
    ++iter;

    iter = di.insert(iter, 9); // 15 14 9 13 12 11
    assert(*iter == 9);
    assert(to_str(di) == "15 14 9 13 12 11 ");

    // 1.3 insert n items
    di.shrink_to_fit();
    di.insert(++di.begin(), 3, 88); // 15 88 88 88 14 9 13 12 11
    iter = di.begin();
    assert(*++iter == 88 && *++iter == 88 && *++iter == 88);
    assert(to_str(di) == "15 88 88 88 14 9 13 12 11 ");

    // 1.4 insert initializer-deque
    di.shrink_to_fit();
    iter = di.end();
    --iter;
    --iter;
    di.insert(iter, {3, 2, 1}); // 15 88 88 88 14 9 13 3 2 1 12 11
    cout << to_str(di) << endl;
    assert(to_str(di) == "15 88 88 88 14 9 13 3 2 1 12 11 ");

    // clear
    di.clear();
    assert(di.empty());
    di.shrink_to_fit();
    assert(di.empty());
}

void test_modifiers_complex()
{
    printf("=============%s=================\n", __FUNCTION__);
    std::cout << "-----------complex------------\n";

    stl::deque<complex> dc;

    // 为了更方便逐步测试，使用标准库的实现作为参考
    // 但是也意味着很多操作都要分别做两次……，属实难绷
    std::deque<complex> rdc;

    /* Insert */

    // 1.1 insert lvalue
    stl::deque<complex>::iterator ic;
    std::deque<complex>::iterator ric;

    for (auto &i : cs)
    {
        ic = dc.insert(dc.begin(), i);
        ric = rdc.insert(rdc.begin(), i);
        assert(*ic == i);
        assert(*ric == i);
    }
    check_equal(dc, rdc, 1);

    // 1.2 insert rvalue
    ic = dc.begin();
    stl::advance(ic, 2);
    ric = rdc.begin();
    stl::advance(ric, 2);

    ic = dc.insert(ic, complex(12, 13));
    ric = rdc.insert(ric, complex(12, 13));

    assert(*ic == complex(12, 13));
    ic = dc.begin();
    stl::advance(ic, 2);
    assert(*ic == complex(12, 13));
    check_equal(dc, rdc, 1);

    // 1.3 insert n items
    dc.insert(++dc.begin(), 3, complex(11, 43));
    rdc.insert(++rdc.begin(), 3, complex(11, 43));
    ic = dc.begin();
    assert(*++ic == complex(11, 43) && *++ic == complex(11, 43) && *++ic == complex(11, 43));
    check_equal(dc, rdc, 1);

    // 1.4 insert initializer-deque
    std::initializer_list<complex> ilst{complex(3, 2), complex(1, 0), complex(45, 9)};
    ic = dc.end();
    stl::advance(ic, -2);
    dc.insert(ic, ilst);
    ric = rdc.end();
    stl::advance(ric, -2);
    rdc.insert(ric, ilst);
    check_equal(dc, rdc, 1);

    // 1.5 clear
    dc.clear();
    rdc.clear();
    assert(dc.empty());

    // 1.6 emplace
    ic = dc.emplace(dc.begin(), 12, 3.14);
    ric = rdc.emplace(rdc.begin(), 12, 3.14);
    check_equal(dc, rdc, 1);

    ic = dc.emplace(dc.end(), 1.2, 3.14);
    ric = rdc.emplace(rdc.end(), 1.2, 3.14);
    ic = dc.emplace(ic, complex(0, 1));
    ric = rdc.emplace(ric, complex(0, 1));
    check_equal(dc, rdc, 1);

    assert(*ic == *ric);

    // 1.7 push/emplace back
    for (int i = 0; i < 15; ++i)
    {
        dc.push_back(complex(i, i + 1));
        rdc.push_back(complex(i, i + 1));
    }
    check_equal(dc, rdc, 1);

    for (int i = 0; i < 15; ++i)
    {
        dc.emplace_back(i, i + 1);
        rdc.emplace_back(i, i + 1);
    }
    check_equal(dc, rdc, 1);

    // 1.8 pop back
    for (int i = 0; i < 10; ++i)
    {
        dc.pop_back();
        rdc.pop_back();
    }
    check_equal(dc, rdc, 1);

    // 1.9 erase
    for (int i = 0; i < 5; ++i)
    {
        dc.erase(--dc.end());
        rdc.erase(--rdc.end());
    }
    check_equal(dc, rdc, 1);

    ic = dc.end();
    stl::advance(ic, -5);

    ric = rdc.end();
    stl::advance(ric, -5);

    assert(dc.begin() + dc.size() == dc.end());
    dc.erase(ic, dc.end());
    rdc.erase(ric, rdc.end());
    check_equal(dc, rdc, 1);
    assert(dc.begin() + dc.size() == dc.end());

    // 1.10 resize
    auto size = rdc.size();
    dc.insert(dc.end(), 10, complex());
    rdc.insert(rdc.end(), 10, complex());
    check_equal(dc, rdc, 1);

    ic = dc.end();
    stl::advance(ic, -10);
    ric = rdc.end();
    stl::advance(ric, -10);
    dc.erase(ic, dc.end());
    rdc.erase(ric, rdc.end());
    check_equal(dc, rdc, 1);

    dc.clear();
    rdc.clear();
    check_equal(dc, rdc, 1);

    dc.assign(10, complex(1, 9));
    rdc.assign(10, complex(1, 9));
    check_equal(dc, rdc, 1);

    std::cout << "-----------end---------------\n";
}

void test_modifiers_string()
{
    printf("=============%s=================\n", __FUNCTION__);

    auto connect_strs = [](const stl::deque<String> &container)
    {
        std::string res;
        for (const String &v : container)
        {
            res += string(v.get_c_str());
            res += ' ';
        }

        return res;
    };

    stl::deque<String> ds;

    ds.emplace(ds.end(), "Hello");
    ds.emplace_back(String("world"));

    assert(ds.size() == 2);
    assert(connect_strs(ds) == "Hello world ");

    ds.emplace(++(++ds.begin()), "exit"); // Hello world exit

    assert(ds.size() == 3);
    assert(connect_strs(ds) == "Hello world exit ");

    std::cout << "after 2 pop back\n";
    ds.pop_back();
    ds.pop_back();
    assert(connect_strs(ds) == "Hello ");
    assert(ds.size() == 1);

    ds.push_back("what");
    ds.insert(--ds.end(), "how"); // Hello how what
    assert(ds.size() == 3);
    assert(connect_strs(ds) == "Hello how what ");

    ds.erase(ds.begin(), --ds.end()); // what
    assert(connect_strs(ds) == "what ");
    assert(ds.size() == 1);

    std::cout << "-----------end---------------\n";
}

void test_deque_operations()
{
    printf("=============%s=================\n", __FUNCTION__);
}

void test_non_member_func()
{
    printf("=============%s=================\n", __FUNCTION__);
}

int main()
{
    test_constructors();
    test_assignment();
    test_capacity();
    test_modifiers_built_in_types();
    test_modifiers_complex();
    test_modifiers_string();
    test_non_member_func();
    std::cout << "Pass!\n";

    return 0;
}
