#include <cassert>
#include <vector>
#include "complex.hh"
#include "string.hh"
#include "array.hh"
#include "construct.hh"
#include "type_traits.hh"
#include "string.hh"
#include "complex.hh"

// built-in type
void test_built_in_type()
{
    printf("===========%s===========\n", __FUNCTION__);
    int x;
    stl::construct(&x, 13);
    assert(x == 13);
    stl::destroy(&x);

    double pie;
    stl::construct(&pie, 3.14);
    assert(pie == 3.14);
    stl::destroy(&pie);

    // destroy stl::array
    stl::array<int, 4> ai{1, 2, 3, 4};
    stl::destroy<typename stl::array<int, 4>::iterator>(ai.begin(), ai.end());
}

// user-defined type
void test_user_defined_type()
{
    printf("===========%s===========\n", __FUNCTION__);

    std::cout << "construct one string (default)\n";
    String *p = (String *)malloc(sizeof(String));
    char *str1 = "This is string\n";
    stl::construct(p, str1);
    assert(!strcmp(p->get_c_str(), str1));

    std::cout << "destroy\n";
    stl::destroy(p);

    char *str2 = "This is second\n";
    String so(str2);
    std::cout << "--------constructo one string (copy)-------\n";
    stl::construct(p, so);
    assert(!strcmp(p->get_c_str(), str2));
    std::cout << "destroy\n";
    stl::destroy(p);
    std::cout << "--------constructo one string (std::move)-------\n";
    stl::construct(p, std::move(so));
    assert(so.get_c_str() == nullptr);
    assert(!strcmp(p->get_c_str(), str2));
    std::cout << "destroy\n";
    stl::destroy(p);

    free(p);

    std::cout << "------destroy a stl::array of complex-------\n";
    // destroy stl::array
    stl::array<complex, 4> ao{complex(1, 2), complex(3, 4), complex(5, 6), complex(7, 8)};
    stl::destroy<typename stl::array<complex, 4>::iterator>(ao.begin(), ao.end());

    // destroy heap-allocated array of objs
    std::cout << "------destroy a c-array of complex-------\n";
    int num = 10;
    complex *po = (complex *)malloc(sizeof(complex) * num);
    for (int i = 0; i < num; ++i)
        stl::construct(&po[i], i, i + 1);
    for (int i = 0; i < num; ++i)
        assert(po[i].real() == i && po[i].imag() == i + 1);
    stl::destroy(&po[0], &po[num]);
    free(po);
    std::cout << "==========================\n";
}

// mix user-define type test
struct Mix
{
    String str{};
    complex cmx{};

    Mix() = default;
    Mix(const String &s, const complex &c) : str(s), cmx(c)
    {
    }
    Mix(String &&s, const complex &c) : str(std::move(s)), cmx(c)
    {
    }
    Mix(const String &s, complex &&c) : str(s), cmx(std::move(c))
    {
    }
    Mix(String &&s, complex &&c) : str(std::move(s)), cmx(std::move(c))
    {
    }
};

void test_mix_user_defined_type()
{
    printf("===========%s===========\n", __FUNCTION__);
    Mix * mix = (Mix *)malloc(sizeof(Mix));

    std::cout << "---test Mix(const String & s, const complex & c)----\n";
    String s1("hello world");
    complex c1(1.1, 3.4);
    stl::construct(mix, s1, c1);
    stl::destroy(mix);
    std::cout << "---test Mix(String && s, const complex & c)----\n";
    String s2("hello world");
    complex c2(1.1, 3.4);
    stl::construct(mix, std::move(s2), c2);
    stl::destroy(mix);
    std::cout << "---test Mix(const String & s, complex && c)----\n";
    String s3("hello world");
    complex c3(1.1, 3.4);
    stl::construct(mix, s3, std::move(c3));
    stl::destroy(mix);
    std::cout << "---test Mix(String && s, complex && c)----\n";
    String s4("hello world");
    complex c4(1.1, 3.4);
    stl::construct(mix, std::move(s4), std::move(c4));
    stl::destroy(mix);
    std::cout << "----------------------------\n";
}

int main()
{
    test_built_in_type();
    test_user_defined_type();
    test_mix_user_defined_type();
    std::cout << "Pass!" << std::endl;

    return 0;
}
