// 
// Create by rda on 2023/4/5.
// 

#ifndef MINISTL_TYPE_STRAITS_HH
#define MINISTL_TYPE_STRAITS_HH

namespace stl
{
    struct __true_type {};
    struct __false_type {};

    template <typename T>
    struct type_traits
    {
        using has_trivial_default_constructor = __false_type;
        using has_trivial_copy_constructor = __false_type;
        using has_trivial_assignment_operator = __false_type;
        using has_tirvial_destructor = __false_type;
        using is_POD_type = __false_type;
    };

    template <>
    struct type_traits<char>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<signed char>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<unsigned char>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };


    template <>
    struct type_traits<short>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<unsigned short>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<int>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<unsigned int>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<long>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<unsigned long>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<float>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<double>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <>
    struct type_traits<long double>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };

    template <typename T>
    struct type_traits<T *>
    {
        using has_trivial_default_constructor = __true_type;
        using has_trivial_copy_constructor = __true_type;
        using has_trivial_assignment_operator = __true_type;
        using has_tirvial_destructor = __true_type;
        using is_POD_type = __true_type;
    };
    
} // namespace stl

#endif
