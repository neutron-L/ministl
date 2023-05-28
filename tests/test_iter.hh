//
// Created by rda on 2023/5/28.
//

#include <iostream>
#include <initializer_list>
#include <vector>

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
