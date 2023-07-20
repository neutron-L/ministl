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
    stl::deque<int> vi1;
    assert(vi1.size() == 0);

    // 2. initializer-deque constructor
    stl::deque<int> vi2{1, 2, 3, 4, 5};
    cout << (vi2.begin() != vi2.end()) << endl;
    cout << (vi2.begin() == vi2.end()) << endl;
    cout << (vi2.begin() < vi2.end()) << endl;
    assert(to_str(vi2) == "1 2 3 4 5 ");

    // 3. range constructor
    stl::deque<int> vi3(vi2.begin(), vi2.end());

    check_equal(vi2, vi3, 1);

    for_each(vi2.begin(), vi2.end(), [](int &i)
             { i <<= 1; });

    // 4. copy constructor
    stl::deque<int> vi4(vi2);
    check_equal(vi4, vi2, 1);
    check_equal(vi4, vi3, 2);

    // 5. <n> constructor
    stl::deque<int> vi5(10);
    n = vi5.size();
    for (auto &i : vi5)
        assert(i == int());

    stl::deque<int> vc1(n);
    stl::deque<String> vs1(n);

    for (auto &i : vc1)
        assert(i == complex());

    for (auto &i : vs1)
        assert(i == String());

    // 6. <n, elem> constructor
    int elem = -12;
    stl::deque<int> vi6(10, elem);

    complex c(1.3, 2.4);
    stl::deque<complex> vc2(n, c);
    String str("what??");
    stl::deque<String> vs2(n, str);
    for (auto &i : vi6)
        assert(i == elem);
    for (auto &i : vc2)
        assert(i == c);
    for (auto &i : vs2)
        assert(i == str);

    // 7. move constructor
    stl::deque<String> vs3(std::move(vs2));
    assert(vs2.empty() && vs2.begin() == vs2.end());
    for (auto &i : vs3)
        assert(i == str);
}

void test_assignment()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::deque<double>::size_type n;
    stl::deque<double> vd1{3.14, 18.90, 10};
    stl::deque<double> vd2;

    // 1. =(const deque &)
    vd2 = vd1;

    check_equal(vd1, vd2, 1);
    for_each(vd2.begin(), vd2.end(), [](double &i)
             { i *= 2; });
    check_equal(vd2, vd1, 2);

    // 2. =(deque &&)
    stl::deque<double> vd3;
    vd3 = std::move(vd1);
    check_equal(vd2, vd3, 2);

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
    stl::deque<complex> vc1(4, c);
    for_each(vc1.begin(), vc1.end(), [&](const complex &i)
             { assert(i == c); });

    // 4&5. = initialized deque & assign(init list)
    const complex c1(1, 2);
    const complex c2(3, 4);
    const complex c3;
    const stl::deque<complex> vc2 = {c, c1, c2, c3};

    auto iter = vc2.begin();
    assert(*iter == c);
    ++iter;
    assert(*iter == c1);
    ++iter;
    assert(*iter == c2);
    assert(*(--vc2.end()) == c3);

    stl::deque<complex> cv;
    cv.assign({c, c1, c2, c3});
    check_equal(vc2, cv, 1);

    // 6. range assign
    stl::deque<String> vs1;
    vs1.assign({String(str[0]), String(str[1]), String(str[2]), String(str[3]), String(str[4])});
    n = 4;
    stl::deque<String> vs2(n);
    for (auto &s : vs2)
        assert(s == String());

    vs2.assign(vs1.begin(), vs1.end());
    std::equal(vs1.begin(), vs1.end(), vs2.begin());
}

void test_capacity()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::deque<double>::size_type n;
    stl::deque<double> vd1{3.14, 18.90, 10};
    stl::deque<double> vd2;

    // 1. empty
    assert(!vd1.empty() && vd2.empty());
    vd2 = vd1;
    assert(!vd2.empty() && vd1.size() == vd2.size());

    // 2. size
    assert(vd1.size() == 3);
    vd1.pop_back();
    assert(vd1.size() == 2);
    vd1.push_front(4);
    assert(vd1.size() == 3);

    vd2.assign({1.2, 3.4, 5.6, 1.0});
    assert(vd2.size() == 4);

    vd2.clear();
    assert(vd2.empty());

    vd2.assign(1233, 1.25);
    for (auto &it : vd2)
        assert(it == 1.25);

    n = 512;
    vd2.resize(n);
    for (auto &it : vd2)
        assert(it == 1.25);
    assert(vd2.size() == n);

    vd2.shrink_to_fit();
    vd2.resize(2 * n, 123);
    assert(vd2.size() == 2 * n);
    for (int i = 0; i < 2 * n; ++i)
    {
        if (i < n)
            assert(vd2[i] == 1.25);
        else
            assert(vd2[i] == 123);
    }

    vd2.shrink_to_fit();
    vd2.resize(n / 2, 123);
    assert(vd2.size() == n / 2);
    for (auto &it : vd2)
        assert(it == 1.25);
    vd1 = std::move(vd2);
    assert(vd1.size() == n / 2);
    for (auto &it : vd1)
        assert(it == 1.25);
    assert(vd2.empty());

    vd2.shrink_to_fit();
    vd2.erase(vd2.begin());
    assert(vd2.empty());

    vd2.shrink_to_fit();
    vd1.erase(vd1.begin());
    assert(vd1.size() == n / 2 - 1);

    size_t len1 = 100, len2 = 321;
    double d1 = 13, d2 = 90;
    vd1.assign(len1, d1);
    vd2.assign(len2, d2);
    assert(vd1.size() == len1 && vd1.size() == stl::distance(vd1.begin(), vd1.end()));
    assert(vd2.size() == len2 && vd1.size() == stl::distance(vd1.begin(), vd1.end()));

    for (auto &i : vd1)
        assert(i == d1);
    for (auto &i : vd2)
        assert(i == d2);

    vd1.swap(vd2);

    assert(vd1.size() == len2 && vd1.size() == stl::distance(vd1.begin(), vd1.end()));
    assert(vd2.size() == len1 && vd1.size() == stl::distance(vd1.begin(), vd1.end()));

    for (auto &i : vd1)
        assert(i == d2);
    for (auto &i : vd2)
        assert(i == d1);
}

void test_modifiers_built_in_types()
{
    printf("=============%s=================\n", __FUNCTION__);

    static int arr[] = {11, 12, 13, 14, 15};
    /* Insert */
    stl::deque<int> vi;
    static stl::deque<int>::size_type len = 5;

    // 1.1 insert lvalue
    stl::deque<int>::iterator iter;
    for (auto &i : arr)
    {
        iter = vi.insert(vi.begin(), i);
        assert(*iter == i);
    }
    // 15 14 13 12 11
    int i = 0;
    for_each(vi.rbegin(), vi.rend(), [&i](auto &item)
             { assert(item == arr[i++]); });
    assert(to_str(vi) == "15 14 13 12 11 ");

    // 1.2 insert rvalue
    vi.shrink_to_fit();
    iter = vi.begin();
    ++iter;
    ++iter;

    iter = vi.insert(iter, 9); // 15 14 9 13 12 11
    assert(*iter == 9);
    assert(to_str(vi) == "15 14 9 13 12 11 ");

    // 1.3 insert n items
    vi.shrink_to_fit();
    vi.insert(++vi.begin(), 3, 88); // 15 88 88 88 14 9 13 12 11
    iter = vi.begin();
    assert(*++iter == 88 && *++iter == 88 && *++iter == 88);
    assert(to_str(vi) == "15 88 88 88 14 9 13 12 11 ");

    // 1.4 insert initializer-deque
    vi.shrink_to_fit();
    iter = vi.end();
    --iter;
    --iter;
    vi.insert(iter, {3, 2, 1}); // 15 88 88 88 14 9 13 3 2 1 12 11
    cout << to_str(vi) << endl;
    assert(to_str(vi) == "15 88 88 88 14 9 13 3 2 1 12 11 ");

    // clear
    vi.clear();
    assert(vi.empty());
    vi.shrink_to_fit();
    assert(vi.empty());
}

void test_modifiers_complex()
{
    printf("=============%s=================\n", __FUNCTION__);
    std::cout << "-----------complex------------\n";

    stl::deque<complex> vc;

    // 为了更方便逐步测试，使用标准库的实现作为参考
    // 但是也意味着很多操作都要分别做两次……，属实难绷
    std::deque<complex> rvc;

    /* Insert */

    // 1.1 insert lvalue
    stl::deque<complex>::iterator ic;
    std::deque<complex>::iterator ric;

    for (auto &i : cs)
    {
        ic = vc.insert(vc.begin(), i);
        ric = rvc.insert(rvc.begin(), i);
        assert(*ic == i);
        assert(*ric == i);
    }
    check_equal(vc, rvc, 1);

    // 1.2 insert rvalue
    ic = vc.begin();
    stl::advance(ic, 2);
    ric = rvc.begin();
    stl::advance(ric, 2);

    ic = vc.insert(ic, complex(12, 13));
    ric = rvc.insert(ric, complex(12, 13));

    assert(*ic == complex(12, 13));
    ic = vc.begin();
    stl::advance(ic, 2);
    assert(*ic == complex(12, 13));
    check_equal(vc, rvc, 1);

    // 1.3 insert n items
    vc.insert(++vc.begin(), 3, complex(11, 43));
    rvc.insert(++rvc.begin(), 3, complex(11, 43));
    ic = vc.begin();
    assert(*++ic == complex(11, 43) && *++ic == complex(11, 43) && *++ic == complex(11, 43));
    check_equal(vc, rvc, 1);

    // 1.4 insert initializer-deque
    std::initializer_list<complex> ilst{complex(3, 2), complex(1, 0), complex(45, 9)};
    ic = vc.end();
    stl::advance(ic, -2);
    vc.insert(ic, ilst);
    ric = rvc.end();
    stl::advance(ric, -2);
    rvc.insert(ric, ilst);
    check_equal(vc, rvc, 1);

    // 1.5 clear
    vc.clear();
    rvc.clear();
    assert(vc.empty());

    // 1.6 emplace
    ic = vc.emplace(vc.begin(), 12, 3.14);
    ric = rvc.emplace(rvc.begin(), 12, 3.14);
    check_equal(vc, rvc, 1);

    ic = vc.emplace(vc.end(), 1.2, 3.14);
    ric = rvc.emplace(rvc.end(), 1.2, 3.14);
    ic = vc.emplace(ic, complex(0, 1));
    ric = rvc.emplace(ric, complex(0, 1));
    check_equal(vc, rvc, 1);

    assert(*ic == *ric);

    // 1.7 push/emplace back
    for (int i = 0; i < 15; ++i)
    {
        vc.push_back(complex(i, i + 1));
        rvc.push_back(complex(i, i + 1));
    }
    check_equal(vc, rvc, 1);

    for (int i = 0; i < 15; ++i)
    {
        vc.emplace_back(i, i + 1);
        rvc.emplace_back(i, i + 1);
    }
    check_equal(vc, rvc, 1);

    // 1.8 pop back
    for (int i = 0; i < 10; ++i)
    {
        vc.pop_back();
        rvc.pop_back();
    }
    check_equal(vc, rvc, 1);

    // 1.9 erase
    for (int i = 0; i < 5; ++i)
    {
        vc.erase(--vc.end());
        rvc.erase(--rvc.end());
    }
    check_equal(vc, rvc, 1);

    ic = vc.end();
    stl::advance(ic, -5);

    ric = rvc.end();
    stl::advance(ric, -5);

    assert(vc.begin() + vc.size() == vc.end());
    vc.erase(ic, vc.end());
    rvc.erase(ric, rvc.end());
    check_equal(vc, rvc, 1);
    assert(vc.begin() + vc.size() == vc.end());

    // 1.10 resize
    auto size = rvc.size();
    vc.insert(vc.end(), 10, complex());
    rvc.insert(rvc.end(), 10, complex());
    check_equal(vc, rvc, 1);

    ic = vc.end();
    stl::advance(ic, -10);
    ric = rvc.end();
    stl::advance(ric, -10);
    vc.erase(ic, vc.end());
    rvc.erase(ric, rvc.end());
    check_equal(vc, rvc, 1);

    vc.clear();
    rvc.clear();
    check_equal(vc, rvc, 1);

    vc.assign(10, complex(1, 9));
    rvc.assign(10, complex(1, 9));
    check_equal(vc, rvc, 1);

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

    stl::deque<String> vs;

    vs.emplace(vs.end(), "Hello");
    vs.emplace_back(String("world"));

    assert(vs.size() == 2);
    assert(connect_strs(vs) == "Hello world ");

    vs.emplace(++(++vs.begin()), "exit"); // Hello world exit

    assert(vs.size() == 3);
    assert(connect_strs(vs) == "Hello world exit ");

    std::cout << "after 2 pop back\n";
    vs.pop_back();
    vs.pop_back();
    assert(connect_strs(vs) == "Hello ");
    assert(vs.size() == 1);

    vs.push_back("what");
    vs.insert(--vs.end(), "how"); // Hello how what
    assert(vs.size() == 3);
    assert(connect_strs(vs) == "Hello how what ");

    vs.erase(vs.begin(), --vs.end()); // what
    assert(connect_strs(vs) == "what ");
    assert(vs.size() == 1);

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
    // test_assignment();
    test_capacity();
    test_modifiers_built_in_types();
    test_modifiers_complex();
    test_modifiers_string();
    test_non_member_func();
    std::cout << "Pass!\n";

    return 0;
}
