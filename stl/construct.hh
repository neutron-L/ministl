//
// Created by rda on 2023/4/5.
//

#ifndef MINISTL_CONSTRUCT_HH
#define MINISTL_CONSTRUCT_HH

#include <iostream>

#include "type_traits.hh"
#include "iterator.hh"

namespace stl
{
    template<typename T1, typename... Args>
    inline void construct(T1 * p, Args&&... val) noexcept
    {
        new (p) T1(std::forward<Args>(val)...);
    }

    template<typename T>
    inline void destroy(T * p)
    {
        p->~T();
    }

    template<typename ForwardIterator>
    static inline void destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
    {
        #ifdef DEBUG
        std::cout << "call destroy (has non-trivial destroctor)\n";
        #endif
        while (first != last)
        {
            destroy(&*first);
            ++first;
        }
    }

    template<typename ForwardIterator>
    static inline void destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
    {
        #ifdef DEBUG
        std::cout << "call destroy (has trivial destroctor)\n";
        #endif
    }

    
    template<typename ForwardIterator, typename T>
    static inline void destroy(ForwardIterator first, ForwardIterator last, T*)
    {
        using trivial_destructor = typename type_traits<T>::has_trivial_destructor;
        destroy_aux(first, last, trivial_destructor());
    }

    template<typename ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last)
    {
        destroy(first, last, value_type(first));
    }

} // namespace stl



#endif