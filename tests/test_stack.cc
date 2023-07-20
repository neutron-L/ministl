//
// Created by rda on 2023/7/20.
//

/*
 * 测试stl::stack
 * 1. 测试所有接口
 * 2. 使用自定义实现的stl::stack解决几个经典栈问题，包括
 *    2.1 括号匹配
 * */

#include <cassert>
#include "complex.hh"
#include "string.hh"
#include "stack.hh"
#include "vector.hh"
#include "list.hh"

const char *str[]{"one", "two", "three", "four", "five"};
const complex cs[]{complex(1.2, 3.14), complex(0, 0), complex(9.14, 4.5), complex(4.2, 6.18)};

void test_constructors()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::deque<int> di{1, 2, 3, 4, 5};
    stl::stack<int>::size_type n;

    // 1. default constructor
    stl::stack<int> stk1;
    assert(stk1.size() == 0 && stk1.empty());

    // 2. constructor(const container &)
    stl::stack<int> stk2(di);
    n = di.size();
    while (!stk2.empty())
    {
        assert(stk2.top() == di[--n]);
        stk2.pop();
    }

    // 3. constructor(container &)
    stl::deque<int> d2{1, 2, 3, 4, 5};
    stl::stack<int> stk3(std::move(d2));
    n = di.size();
    while (!stk3.empty())
    {
        assert(stk3.top() == di[--n]);
        stk3.pop();
    }

    // 4. copy constructor
    stl::stack<int> t1(di);
    stl::stack<int> stk4(t1);
    while (!stk4.empty())
    {
        assert(stk4.top() == t1.top());
        stk4.pop();
        t1.pop();
    }

    // 5. move constructor
    stl::stack<int> t2(di);
    stl::stack<int> stk5(std::move(t2));
    n = di.size();
    while (!stk5.empty())
    {
        assert(stk5.top() == di[--n]);
        stk5.pop();
    }

    // 6. range constructor
    stl::stack<int> stk6(di.begin() + 1, di.end() - 1);
    assert(stk6.size() == stl::distance(di.begin() + 1, di.end() - 1));
    assert(stk6.top() == 4);
    stk6.pop();
    assert(stk6.size() == 2);
    assert(stk6.top() == 3);
    stk6.pop();
    assert(stk6.size() == 1);
    assert(stk6.top() == 2);
    stk6.pop();
    assert(stk6.size() == 0);
}

void test_assignment()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::deque<double>::size_type n;

    stl::deque<double> vd1{3.14, 18.90, 10};

    stl::stack<double> sd1(vd1);
    stl::stack<double> sd2;
    assert(sd1.size() == vd1.size() && sd2.size() == 0);

    sd2 = sd1;
    assert(sd1.size() == sd1.size());
    n = vd1.size();
    while (!sd1.empty())
    {
        assert(sd1.top() == sd2.top() && sd1.top() == vd1[--n]);
        sd1.pop();
        sd2.pop();
    }

    stl::stack<double> sd3;
    stl::stack<double> sd4(vd1);
    sd3 = std::move(sd4);
    n = vd1.size();
    assert(sd3.size() == n);

    while (!sd3.empty())
    {
        assert(sd3.top() == vd1[--n]);
        sd3.pop();
    }
}

void test_modifiers_complex()
{
    printf("=============%s=================\n", __FUNCTION__);
    std::cout << "-----------complex------------\n";

    stl::stack<complex> cx_stk;

    for (auto &c : cs)
        cx_stk.push(c);
    auto n = cx_stk.size();
    for (int i = n - 1; i >= 0; --i)
    {
        assert(cx_stk.top() == cs[i]);
        cx_stk.pop();
    }
    assert(cx_stk.empty());

    std::cout << "-----------end---------------\n";
}


void test_modifiers_string()
{
    printf("=============%s=================\n", __FUNCTION__);
    std::cout << "-----------string------------\n";

    stl::stack<String> str_stk;

    for (auto &s : str)
        str_stk.push(String(s));
    auto n = str_stk.size();
    for (int i = n - 1; i >= 0; --i)
    {
        assert(!strcmp(str_stk.top().get_c_str(), str[i]));
        str_stk.pop();
    }
    assert(str_stk.empty());

    std::cout << "-----------end---------------\n";
}

void test_non_member_func()
{
    printf("=============%s=================\n", __FUNCTION__);
}

static bool is_valid(const string & str)
{
    stl::stack<char> stk;

    for (auto & c : str)
    {
        if (c == '(')
            stk.push(c);
        else
        {
            if (stk.empty())
                return false;
            else
                stk.pop();
        }
    }

    return stk.empty();
}

int main()
{
    test_constructors();
    test_assignment();
    test_modifiers_complex();
    test_modifiers_string();
    test_non_member_func();

    assert(is_valid("(())"));
    assert(is_valid("()()"));
    assert(is_valid("(()())"));
    assert(!is_valid("(()()"));
    std::cout << "Pass!\n";

    return 0;
}
