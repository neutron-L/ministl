#include <iostream>
#include <cassert>
#include "type_traits.hh"

using std::cin;
using std::cout;
using std::endl;

struct Item
{
};

void test_self_define_type_traits()
{
    printf("===========%s===========\n", __FUNCTION__);
    assert(typeid(stl::type_traits<Item>::has_trivial_default_constructor) == typeid(stl::__false_type));
    assert(typeid(stl::type_traits<Item>::has_trivial_copy_constructor) == typeid(stl::__false_type));
    assert(typeid(stl::type_traits<Item>::has_trivial_assignment_operator) == typeid(stl::__false_type));
    assert(typeid(stl::type_traits<Item>::has_trivial_destructor) == typeid(stl::__false_type));
    assert(typeid(stl::type_traits<Item>::is_POD_type) == typeid(stl::__false_type));
}

template <typename T>
void test_basic_type_traits()
{
    printf("===========%s===========\n", __FUNCTION__);
    assert(typeid(typename stl::type_traits<T>::has_trivial_default_constructor) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T>::has_trivial_copy_constructor) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T>::has_trivial_assignment_operator) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T>::has_trivial_destructor) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T>::is_POD_type) == typeid(stl::__true_type));
}

template <typename T>
void test_pointer_type_traits()
{
    printf("===========%s===========\n", __FUNCTION__);
    assert(typeid(typename stl::type_traits<T *>::has_trivial_default_constructor) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T *>::has_trivial_copy_constructor) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T *>::has_trivial_assignment_operator) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T *>::has_trivial_destructor) == typeid(stl::__true_type));
    assert(typeid(typename stl::type_traits<T *>::is_POD_type) == typeid(stl::__true_type));
}

int main()
{
    test_self_define_type_traits();

    test_basic_type_traits<int>();
    test_basic_type_traits<char>();
    // self-define type pointer
    test_pointer_type_traits<Item>();
    // basic type pointer
    test_pointer_type_traits<double>();
    // const pointer
    test_pointer_type_traits<const int>();

    std::cout << "Pass!\n";

    return 0;
}