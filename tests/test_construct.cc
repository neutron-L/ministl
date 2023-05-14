#include <cassert>
#include <vector>
#include "array.hh"
#include "construct.hh"
#include "type_traits.hh"

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

    struct Obj
    {
        int i{};
        double d{};
        Obj() = default;
        Obj(int ii, double dd) : i(ii), d(dd) {}
        ~Obj() { std::cout << "Obj destructor\n"; }
    };

    std::cout << "test destroy one obj\n";
    Obj *p = (Obj *)malloc(sizeof(Obj));
    stl::construct(p, 1, 3.14);
    assert(p->i == 1);
    assert(p->d == 3.14);

    stl::destroy(p);
    free(p);

    std::cout << "test destroy range in array\n";
    // destroy stl::array
    stl::array<Obj, 4> ao{Obj(1, 2), Obj(3, 4), Obj(5, 6), Obj(7, 8)};
    stl::destroy<typename stl::array<Obj, 4>::iterator>(ao.begin(), ao.end());

    // destroy heap-allocated array of objs
    std::cout << "test destroy range in array\n";
    int num = 10;
    Obj *po = (Obj *)malloc(sizeof(Obj) * num);
    for (int i = 0; i < num; ++i)
        stl::construct(&po[i]);
    stl::destroy(&po[0], &po[num]);
    free(po);
    std::cout << "==========================\n";
}

int main()
{
    test_built_in_type();
    test_user_defined_type();

    return 0;
}