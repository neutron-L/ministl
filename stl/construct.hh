//
// Created by rda on 2023/4/5.
//

#ifndef MINISTL_CONSTRUCT_HH
#define MINISTL_CONSTRUCT_HH


#include "type_traits.hh"

namespace stl
{
    template<typename T1, typename T2>
    inline void construct(T1 * p, const T2 & val)
    {
        new (p) T1(val);
    }

    template<typename T>
    inline void destroy(T * p)
    {
        p->~T();
    }

    template<typename ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last)
    {
        __destroy(first, last, value_type(first));
    }

    template<typename ForwardIterator, typename T>
    inline void destroy(ForwardIterator first, ForwardIterator last, T*)
    {
        using trivial_destructor = typename type_traits<T>::has_trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
    {
        while (first != last)
        {
            destroy(&*first);
            ++first;
        }
    }

    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
    {
    }
} // namespace stl



#endif