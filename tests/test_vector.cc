#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include "type.hh"
#include "complex.hh"
#include "string.hh"
#include "vector.hh"
using namespace std;

const char *str[]{"one", "two", "three", "four", "five"};
const complex cs[]{complex(1.2, 3.14), complex(0, 0), complex(9.14, 4.5), complex(4.2, 6.18)};

void test_constructors()
{
    printf("=============%s=================\n", __FUNCTION__);

    stl::vector<int>::size_type n;

    // 1. default constructor
    stl::vector<int> vi1;
    assert(vi1.size() == 0 && vi1.capacity() == 0);

    // 2. initializer-list constructor
    stl::vector<int> vi2{1, 2, 3, 4, 5};

    // 3. range constructor
    std::vector<int> vi3(vi2.begin(), vi2.end());

    n = vi2.size();
    assert(vi3.size() == n);
    for (decltype(n) i = 0; i < n; ++i)
        assert(vi2.at(i) == vi3.at(i) && vi2[i] == vi3[i]);

    for_each(vi2.begin(), vi2.end(), [](int &i)
             { i <<= 1; });

    // 4. copy constructor
    stl::vector<int> vi4(vi2);
    for (decltype(n) i = 0; i < n; ++i)
        assert(vi2[i] == vi4[i] && vi4[i] == 2 * vi3[i]);

    // 5. <n> constructor
    stl::vector<int> vi5(10);
    n = vi5.size();
    for (decltype(n) i = 0; i < n; ++i)
        assert(vi5[i] == int());

    stl::vector<int> vc1(n);
    stl::vector<String> vs1(n);

    for (decltype(n) i = 0; i < n; ++i)
    {
        assert(vc1[i] == complex());
        assert(vs1[i] == String());
    }

    // 6. <n, elem> constructor
    int elem = -12;
    stl::vector<int> vi6(10, elem);
    n = vi6.size();

    complex c(1.3, 2.4);
    stl::vector<complex> vc2(n, c);
    String str("what??");
    stl::vector<String> vs2(n, str);
    for (decltype(n) i = 0; i < n; ++i)
    {
        assert(vi6[i] == elem);
        assert(vc2[i] == c);
        assert(vs2[i] == str);
    }

    // 7. move constructor
    stl::vector<String> vs3(std::move(vs2));
    assert(vs2.empty() && vs2.begin() == nullptr);
    for (decltype(n) i = 0; i < n; ++i)
        assert(vs3[i] == str);
}

void test_assignment()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::vector<double>::size_type n;
    stl::vector<double> vd1{3.14, 18.90, 10};
    stl::vector<double> vd2;

    // 1. =(const vector &)
    vd2 = vd1;

    n = vd1.size();
    for (decltype(n) i = 0; i < n; ++i)
    {
        assert(vd1[i] == vd2[i]);
        vd1[i] *= 2;
        assert(vd1[i] == 2 * vd2[i]);
    }

    // 2. =(vector &&)
    stl::vector<double> vd3;
    vd3 = std::move(vd1);
    for (decltype(n) i = 0; i < n; ++i)
        assert(vd3[i] == 2 * vd2[i]);

    String s1("What");
    String s2("How");
    stl::vector<String> ax(n, s1);
    stl::vector<String> ay;
    ay = std::move(ax);
    for (decltype(n) i = 0; i < n; ++i)
        assert(!strcmp(ay[i].get_c_str(), s1.get_c_str()));

    stl::vector<String> az(n, s2);
    ax = az;

    for (decltype(n) i = 0; i < n; ++i)
        assert(ax[i] == s2);

    // 4. assign(n, elem)
    const complex c(3.14, 6.18);
    stl::vector<complex> vc1(4, c);
    n = vd1.size();
    for (decltype(n) i = 0; i < n; ++i)
        assert(vc1[i] == c);

    // 5. initializer list assign
    const complex c1(1, 2);
    const complex c2(3, 4);
    const complex c3;
    const stl::vector<complex> vc2{c, c1, c2, c3};
    assert(*(vc2.begin()) == c);
    assert(vc2[1] == c1);
    assert(vc2.at(2) == c2);
    assert(*(vc2.end() - 1) == c3);

    // 6. range assign
    stl::vector<String> vs1{String(str[0]), String(str[1]), String(str[2]), String(str[3]), String(str[4])};
    n = 4;
    stl::vector<String> vs2(n);
    for (auto &s : vs2)
        assert(s == String());

    vs2.assign(vs1.begin(), vs1.end());
    n = vs1.size();
    for (decltype(n) i = 0; i < n; ++i)
        assert(vs1[i] == vs2[i]);
}

void test_capacity()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::vector<double>::size_type n;
    stl::vector<double> vd1{3.14, 18.90, 10};
    stl::vector<double> vd2;

    // 1. empty
    assert(!vd1.empty() && vd2.empty());
    vd2 = vd1;
    assert(!vd2.empty() && vd1.size() == vd2.size());

    // 2. size
    assert(vd1.size() == 3);

    // 3. capacity & reserve
    stl::vector<String> vs1;
    assert(!vs1.capacity());
    n = 12;
    vs1.reserve(n);
    assert(vs1.capacity() == n && vs1.empty());
    vs1 = {String(str[0]), String(str[1]), String(str[2]), String(str[3]), String(str[4])};
    assert(vs1.capacity() == n && !vs1.empty());

    // 4. shrink_to_fit
    assert(vs1.capacity() != vs1.size());
    vs1.shrink_to_fit();
    assert(vs1.capacity() == vs1.size() && vs1.size() == 5);
}

void test_modifiers_built_in_types()
{
    printf("=============%s=================\n", __FUNCTION__);

    auto to_string = [](const auto &container)
    {
        std::string res;
        for (auto &v : container)
        {
            res += std::to_string(v);
            res += ' ';
        }

        return res;
    };

    static int arr[] = {11, 12, 13, 14, 15};
    /* Insert */
    stl::vector<int> vi;
    static stl::vector<int>::size_type len = 5;

    // 1.1 insert lvalue
    stl::vector<int>::iterator iter;
    for (auto &i : arr)
    {
        iter = vi.insert(vi.begin(), i);
        assert(*iter == i);
    }
    // 15 14 13 12 11

    for (decltype(len) i = 0; i < len; ++i)
        assert(arr[i] == vi[len - 1 - i]);

    // 1.2 insert rvalue

    iter = vi.begin() + 2;
    iter = vi.insert(iter, 9); // 15 14 9 13 12 11
    assert(vi[iter - vi.begin()] == 9);
    assert(vi[2] == 9);
    assert(to_string(vi) == "15 14 9 13 12 11 ");

    // 1.3 insert n items
    vi.insert(vi.begin() + 1, 3, 88); // 15 88 88 88 14 9 13 12 11
    assert(vi[1] == 88 && vi.at(2) == 88 && *(vi.begin() + 3) == 88);

    assert(to_string(vi) == "15 88 88 88 14 9 13 12 11 ");

    // 1.4 insert initializer-list
    vi.insert(vi.end() - 2, {3, 2, 1}); // 15 88 88 88 14 9 13 3 2 1 12 11
    assert(to_string(vi) == "15 88 88 88 14 9 13 3 2 1 12 11 ");

    // clear
    vi.clear();
    assert(vi.empty());
}

void test_modifiers_complex()
{
    printf("=============%s=================\n", __FUNCTION__);
    std::cout << "-----------complex------------\n";
    auto check = [](const auto &c1, const auto &c2)
    {
        assert(c1.size() == c2.size());
        auto len = c1.size();

        for (decltype(len) i = 0; i < len; ++i)
            assert(c1[i] == c2[i]);
    };

    stl::vector<complex> vc;

    // 为了更方便逐步测试，使用标准库的实现作为参考
    // 但是也意味着很多操作都要分别做两次……，属实难绷
    std::vector<complex> rvc;

    /* Insert */

    // 1.1 insert lvalue
    stl::vector<complex>::iterator ic;
    std::vector<complex>::iterator ric;

    for (auto &i : cs)
    {
        ic = vc.insert(vc.begin(), i);
        ric = rvc.insert(rvc.begin(), i);
        assert(*ic == i);
        assert(*ric == i);
    }
    check(vc, rvc);

    // 1.2 insert rvalue
    ic = vc.begin() + 2;
    ric = rvc.begin() + 2;
    ic = vc.insert(ic, complex(12, 13));
    ric = rvc.insert(ric, complex(12, 13));
    assert(vc[ic - vc.begin()] == complex(12, 13));
    assert(vc[2] == complex(12, 13));
    check(vc, rvc);

    // 1.3 insert n items
    vc.insert(vc.begin() + 1, 3, complex(11, 43));
    rvc.insert(rvc.begin() + 1, 3, complex(11, 43));
    assert(vc[1] == complex(11, 43) && vc.at(2) == complex(11, 43) && *(vc.begin() + 3) == complex(11, 43));
    check(vc, rvc);

    // 1.4 insert initializer-list
    std::initializer_list<complex> ilst{complex(3, 2), complex(1, 0), complex(45, 9)};
    vc.insert(vc.end() - 2, ilst);
    rvc.insert(rvc.end() - 2, ilst);
    check(vc, rvc);

    // 1.5 clear
    vc.clear();
    rvc.clear();
    assert(vc.empty());

    // 1.6 emplace
    ic = vc.emplace(vc.begin(), 12, 3.14);
    ric = rvc.emplace(rvc.begin(), 12, 3.14);
    check(vc, rvc);

    ic = vc.emplace(vc.end(), 1.2, 3.14);
    ric = rvc.emplace(rvc.end(), 1.2, 3.14);
    ic = vc.emplace(ic, complex(0, 1));
    ric = rvc.emplace(ric, complex(0, 1));
    check(vc, rvc);
    assert(*ic == *ric);

    // 1.7 push/emplace back
    for (int i = 0; i < 15; ++i)
    {
        vc.push_back(complex(i, i + 1));
        rvc.push_back(complex(i, i + 1));
    }
    check(vc, rvc);

    for (int i = 0; i < 15; ++i)
    {
        vc.emplace_back(i, i + 1);
        rvc.emplace_back(i, i + 1);
    }
    check(vc, rvc);

    // 1.8 pop back
    for (int i = 0; i < 10; ++i)
    {
        vc.pop_back();
        rvc.pop_back();
    }
    check(vc, rvc);

    // 1.9 erase
    for (int i = 0; i < 5; ++i)
    {
        vc.erase(vc.end() - 1);
        rvc.erase(rvc.end() - 1);
    }
    check(vc, rvc);
    ic = vc.end() - 5;
    ric = rvc.end() - 5;
    vc.erase(ic, vc.end());
    rvc.erase(ric, rvc.end());
    check(vc, rvc);

    // 1.10 resize
    auto size = rvc.size();
    vc.resize(size + 10);
    rvc.resize(size + 10);
    check(vc, rvc);
    vc.resize(size - 10);
    rvc.resize(size - 10);
    check(vc, rvc);

    vc.resize(0);
    rvc.resize(0);
    check(vc, rvc);
    vc.resize(10, complex(1, 9));
    rvc.resize(10, complex(1, 9));
    check(vc, rvc);

    std::cout << "-----------end---------------\n";
}

void test_modifiers_string()
{
    printf("=============%s=================\n", __FUNCTION__);

    stl::vector<String> vs;

    vs.emplace(vs.end(), "Hello");
    vs.emplace_back(String("world"));
    vs.resize(4, String("and new"));

    assert(vs.size() == 4);
    vs.emplace(vs.begin() + 2, "exit");
    for (auto &i : vs)
        std::cout << i.get_c_str() << '*';
    std::cout << endl;
    std::cout << "after 2 pop back\n";
    vs.pop_back();
    vs.pop_back();
    for (auto &i : vs)
        std::cout << i.get_c_str() << '*';
    assert(vs.size() == 3);
    std::cout << endl;

    vs.erase(vs.begin(), vs.end() - 1);
    cout << "after erase [0, -1)\n";
    for (auto &i : vs)
        std::cout << i.get_c_str() << '*';
    std::cout << endl;
    assert(vs.size() == 1);

    std::cout << "-----------end---------------\n";
}

void test_non_member_func()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::vector<int> vi1{1, 2, 3};
    stl::vector<int> vi2{1, 2};
    stl::vector<int> vi3{1, 4, 3};
    stl::vector<int> vi4{1, 2, 3, 4};
    assert(vi1 >= vi1 && vi1 <= vi1);
    assert(vi1 < vi4 && vi1 <= vi4);
    assert(vi1 > vi2 && vi1 >= vi2);
    assert(vi1 < vi3 && vi1 <= vi3);
    assert(vi3 >= vi4);
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

    // test pair
    stl::vector<std::pair<int, int>> vp;
    vp.push_back({12, 14});
    std::cout << "Pass!\n";

    return 0;
}
