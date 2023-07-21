//
// Created by rda on 2023/7/21.
//

/*
 * 测试stl::queue
 * 1. 测试所有接口
 * 2. 使用自定义实现的stl::queue解决几个经典队列问题
 * */

#include <cassert>
#include "complex.hh"
#include "string.hh"
#include "queue.hh"
#include "vector.hh"
#include "list.hh"

const char *str[]{"one", "two", "three", "four", "five"};
const complex cs[]{complex(1.2, 3.14), complex(0, 0), complex(9.14, 4.5), complex(4.2, 6.18)};

template <typename Container>
void test_constructors()
{
    printf("=============%s=================\n", __FUNCTION__);
    Container di{1, 2, 3, 4, 5};
    stl::deque<int> rdi(di.begin(), di.end());
    typename stl::queue<int, Container>::size_type n;

    // 1. default constructor
    stl::queue<int, Container> stk1;
    assert(stk1.size() == 0 && stk1.empty());

    // 2. constructor(const container &)
    stl::queue<int, Container> stk2(di);
    n = 0;
    while (!stk2.empty())
    {
        assert(stk2.front() == rdi[n++]);
        stk2.pop();
        assert(stk2.size() == di.size() - n);
    }

    // 3. constructor(container &)
    Container d2{1, 2, 3, 4, 5};
    stl::queue<int, Container> stk3(std::move(d2));
    n = 0;

    while (!stk3.empty())
    {
        assert(stk3.front() == rdi[n++]);
        stk3.pop();
        assert(stk3.size() == di.size() - n);
    }

    // 4. copy constructor
    stl::queue<int, Container> t1(di);
    stl::queue<int, Container> stk4(t1);
    while (!stk4.empty())
    {
        assert(stk4.front() == t1.front());
        stk4.pop();
        t1.pop();
        assert(stk4.size() == t1.size());
    }

    // 5. move constructor
    stl::queue<int, Container> t2(di);
    stl::queue<int, Container> stk5(std::move(t2));
    n = 0;
    while (!stk5.empty())
    {
        assert(stk5.front() == rdi[n++]);
        stk5.pop();
        assert(stk5.size() == rdi.size() - n);
    }

    // 6. range constructor
    auto iter1 = di.begin();
    auto iter2 = di.end();
    ++iter1;
    --iter2;
    stl::queue<int, Container> stk6(iter1, iter2);
    assert(stk6.size() == stl::distance(iter1, iter2));
    assert(stk6.front() == 2);
    stk6.pop();
    assert(stk6.size() == 2);
    assert(stk6.front() == 3);
    stk6.pop();
    assert(stk6.size() == 1);
    assert(stk6.front() == 4);
    stk6.pop();
    assert(stk6.size() == 0);
}

void test_assignment()
{
    printf("=============%s=================\n", __FUNCTION__);
    stl::deque<double>::size_type n;

    stl::deque<double> vd1{3.14, 18.90, 10};

    stl::queue<double> sd1(vd1);
    stl::queue<double> sd2;
    assert(sd1.size() == vd1.size() && sd2.size() == 0);

    sd2 = sd1;
    assert(sd1.size() == sd1.size());
    n = 0;
    while (!sd1.empty())
    {
        assert(sd1.front() == sd2.front() && sd1.front() == vd1[n++]);
        sd1.pop();
        sd2.pop();
    }

    stl::queue<double> sd3;
    stl::queue<double> sd4(vd1);
    sd3 = std::move(sd4);
    n = 0;
    assert(sd3.size() == vd1.size());

    while (!sd3.empty())
    {
        assert(sd3.front() == vd1[n++]);
        sd3.pop();
    }
}

void test_modifiers_complex()
{
    printf("=============%s=================\n", __FUNCTION__);
    std::cout << "-----------complex------------\n";

    stl::queue<complex> cx_stk;

    for (auto &c : cs)
        cx_stk.push(c);
    auto n = cx_stk.size();
    for (size_t i = 0; i < n; ++i)
    {
        assert(cx_stk.front() == cs[i]);
        cx_stk.pop();
    }
    assert(cx_stk.empty());

    cx_stk.push(cs[0]);
    assert(cx_stk.front() == cx_stk.back() && cx_stk.size() == 1);
    cx_stk.push(cs[1]);
    assert(cx_stk.front() == cs[0] && cx_stk.back() == cs[1] && cx_stk.size() == 2);

    std::cout << "-----------end---------------\n";
}

void test_modifiers_string()
{
    printf("=============%s=================\n", __FUNCTION__);
    std::cout << "-----------string------------\n";

    stl::queue<String> str_stk;

    for (auto &s : str)
        str_stk.push(String(s));
    auto n = str_stk.size();
    for (size_t i = 0; i < n; ++i)

    {
        assert(!strcmp(str_stk.front().get_c_str(), str[i]));
        std::cout << str_stk.front().get_c_str() << std::endl;
        str_stk.pop();
    }
    assert(str_stk.empty());

    std::cout << "-----------end---------------\n";
}

void test_non_member_func()
{
    printf("=============%s=================\n", __FUNCTION__);
}

int main()
{
    test_constructors<stl::list<int>>();
    test_constructors<stl::deque<int>>();
    test_assignment();
    test_modifiers_complex();
    test_modifiers_string();
    test_non_member_func();

    std::cout << "Pass!\n";

    return 0;
}
