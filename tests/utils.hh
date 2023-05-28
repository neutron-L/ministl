//
// Created by rda on 2023/5/28.
//

/* Logging */
#include <stdio.h>

#include <iostream>
#include <initializer_list>
#include <vector>

#ifdef DEBUG
#define debug(M, ...) \
    fprintf(stderr, "DEBUG %s:%d:%s: " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

#define info(M, ...) \
    fprintf(stderr, "INFO  " M "\n", ##__VA_ARGS__)

#define error(M, ...) \
    fprintf(stderr, "ERROR " M "\n", ##__VA_ARGS__)



/**
 * @brief  begin end cbegin cend rbegin rend crbegin crend 
 * use non-const built-in type and user-defined type
 * @param   lst	initializer-list including origin content
 **/
template <typename Container>
void test_iterators_by_obj(const std::initializer_list<typename Container::value_type> &lst)
{
    printf("=============%s=================\n", __FUNCTION__);
    using type_t = typename Container::value_type;
    typename Container::size_type num = lst.size();
    Container c{lst};
    std::vector<type_t> vc{lst};
    // begin end access
    std::cout << "----test begin/end----\n";
    typename std::initializer_list<typename Container::value_type>::size_type i = 0;


    for (auto & item : c)
        assert(item == vc[i++]);
    assert(i == num);

    // begin end modify
    type_t x, y;

    typename Container::iterator bg = c.begin();
    auto ed = c.end();
    x = *bg;
    y = *(ed - 1);
    *bg = y;
    *(ed - 1) = x;
    assert(*c.begin() == y);
    assert(*(c.end() - 1) == x);
    *bg = x;
    *(ed - 1) = y;

    // cbegin cend
    std::cout << "----test cbegin/cend----\n";

    auto cbg = c.cbegin();
    // *cbg = 12;  // error. cannot modify const iterator
    auto ced = c.cend();
    // *ced = 11; // error
    i = 0;
    while (cbg != ced)
        assert(*cbg++ == vc[i++]);
    assert(i == num);

    cbg = c.cbegin();
    ced = c.cend();
    i = num;
    while (ced != cbg)
        assert(*--ced == vc[--i]);
    assert(!i);

    // rbegin rend
    std::cout << "----test rbegin/rend----\n";
    auto rbg = c.rbegin();
    auto red = c.rend();

    while (rbg != red)
        *rbg++ *= 2;
    rbg = c.rbegin();
    red = c.rend();
    i = num;
    while (rbg != red)
        assert(*rbg++ == 2 * vc[--i]);
    assert(!i);

    rbg = c.rbegin();
    red = c.rend();
    i = 0;
    while (rbg != red)
    {
        assert(*--red == c[i++]);
        *red = *red / 2;
    }
    assert(i == num);

    // crbegin crend
    std::cout << "----test crbegin/crend----\n";

    auto crbg = c.crbegin();
    // *crbg = 12;  // error. cannot modify const iterator
    auto cred = c.crend();
    // *cred = 11; // error
    i = num;
    while (crbg != cred)
        assert(*crbg++ == vc[--i]);
    assert(!i);

    crbg = c.crbegin();
    cred = c.crend();
    i = 0;
    while (crbg != cred)
        assert(*--cred == vc[i++]);
    assert(i == num);
}


/**
 * @brief  begin end cbegin cend rbegin rend crbegin crend 
 * use const built-in type and user-defined type
 * @param   lst	initializer-list including origin content
 **/
template <typename Container>
void test_iterators_by_const_obj(const std::initializer_list<typename Container::value_type> &lst)
{
    printf("=============%s=================\n", __FUNCTION__);
    using type_t = typename Container::value_type;
    typename Container::size_type num = lst.size();
    Container c{lst};
    std::vector<type_t> vc{lst};
    
    // begin end
    std::cout << "----test begin/end----\n";

    typename std::initializer_list<typename Container::value_type>::size_type i = 0;

    for (auto & item : c)
        assert(item == vc[i++]);
    assert(i == num);

    typename Container::const_iterator bg = c.begin();
    // *bg = 11; // error
    auto ed = c.end();
    // *(ed - 1) = 12; // error

    // cbegin cend
    std::cout << "----test cbegin/cend----\n";

    auto cbg = c.cbegin();
    // *cbg = 12;  // error. cannot modify const iterator
    auto ced = c.cend();
    // *ced = 11; // error
    i = 0;
    while (cbg != ced)
        assert(*cbg++ == vc[i++]);
    assert(i == num);

    cbg = c.cbegin();
    ced = c.cend();
    i = num;
    while (ced != cbg)
        assert(*--ced == vc[--i]);
    assert(!i);

    // rbegin rend
    std::cout << "----test rbegin/rend----\n";
    auto rbg = c.rbegin();
    auto red = c.rend();
    // *rbg += 14; // error
    // *red += 14; // error

    i = num;
    while (rbg != red)
        assert(*rbg++ == vc[--i]);
    assert(!i);

    rbg = c.rbegin();
    red = c.rend();
    i = 0;
    while (rbg != red)
        assert(*--red == vc[i++]);
    assert(i == num);

    // crbegin crend
    std::cout << "----test crbegin/crend----\n";

    auto crbg = c.crbegin();
    // *crbg = 12;  // error. cannot modify const iterator
    auto cred = c.crend();
    // *cred = 11; // error
    i = num;
    while (crbg != cred)
        assert(*crbg++ == vc[--i]);
    assert(!i);

    crbg = c.crbegin();
    cred = c.crend();
    i = 0;
    while (crbg != cred)
        assert(*--cred == vc[i++]);
    assert(i == num);
}


/**
 * @brief  test front back [] at
 * use built-in type and user-defined type(const and non-const)
 * @param   lst	initializer-list including origin content
 **/
template <typename Container>
void test_access(const std::initializer_list<typename Container::value_type> &lst)
{
    printf("=============%s=================\n", __FUNCTION__);
    using type_t = typename Container::value_type;
    Container c{lst};
    typename Container::size_type num = lst.size();

    auto i = num - num;
    for (auto & item : lst)
        assert(c[i] == c.at(i) && c[i++] == item);
    assert(i == num);
    // test at throw exception
    try
    {
        std::cout << c.at(i);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    const type_t x = c.front();
    const type_t y = c.back();
    c.front() = y;
    c.back() = x;
    
    assert(c[0] == y);
    assert(c.at(c.size() - 1) == x);

    const Container cc{x, x, y, y};
    // cvi[1] = 12; error
    assert(cc.front() == x);
    assert(cc[0] == x);
    assert(cc[1] == x);
    assert(cc[2] == y);
    assert(cc[3] == y);
    assert(cc.back() == y);
}



/**
 * @brief  基于两个初始化列表的值，测试指定容器类型的比较函数，
 * 通过将每种compare函数的结果与任一stl库容器的对应compare函数的结果对比
 * @param   lst1	initializer-list including origin content
 * @param   lst2	initializer-list including origin content
 **/
template <typename Container>
void test_compare_aux(const std::initializer_list<typename Container::value_type> &lst1, 
                 const std::initializer_list<typename Container::value_type> &lst2)
{
    printf("=============%s=================\n", __FUNCTION__);
    Container c1{lst1}, c2(lst2);
    std::vector<typename Container::value_type> v1(lst1), v2(lst2);

    assert((c1 < c2) == (v1 < v2));
    assert((c1 <= c2) == (v1 <= v2));
    assert((c1 >= c2) == (v1 >= v2));
    assert((c1 > c2) == (v1 > v2));
    assert((c1 != c2) == (v1 != v2));
}




/**
 * @brief  通过指定容器类型的compare函数，测试对比几对不同的初值列的相对大小
 **/
template <typename Container>
void test_compare()
{
    printf("=============%s=================\n", __FUNCTION__);
    test_compare_aux<Container>({}, {});
    test_compare_aux<Container>({}, {1, 2, 3});
    test_compare_aux<Container>({1, 2}, {1, 2, 3});
    test_compare_aux<Container>({1, 2, 3}, {1, 2, 3});
    test_compare_aux<Container>({1, 2, 4}, {1, 2, 3});
    test_compare_aux<Container>({2, 4}, {1, 2, 3});
}
