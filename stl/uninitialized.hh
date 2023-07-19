//
// Create by rda on 2023/4/5.
//

#ifndef MINISTL_UNINITIALIZED_HH
#define MINISTL_UNINITIALIZED_HH

#include "algobase.hh"

#include "type_traits.hh"
#include "construct.hh"

namespace stl
{
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last,
                                         ForwardIterator result, __true_type)
    {
        // 暂时不能调用
        // return std::copy(first, last, result); // TODO: replace by our own copy function, as stl:copy
        return uninitialized_copy_aux(first, last, result, __false_type());
    }

    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last,
                                         ForwardIterator result, __false_type)
    {
        while (first != last)
        {
            stl::construct(&*result, *first);
            ++first;
            ++result;
        }
        return result;
    }

    template <typename InputIterator, typename ForwardIterator, typename T>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                     ForwardIterator result, T *)
    {
        using is_POD = typename type_traits<T>::is_POD_type;
        return uninitialized_copy_aux(first, last, result, is_POD());
    }
    /**
     * copy elements in [first, last) to [result, result + (last - first)), but it does not initialize the destination elements.
     * @param   first	the beginning iterator of source range.
     * @param   fs      the end iterator of source range(not including).
     * @param   result  the beginning iterator of destinatioin range.
     * @return  Iterator points the next position of the last copied element.
     **/
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                     ForwardIterator result)
    {
        return uninitialized_copy(first, last, result, value_type(first));
    }

    
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator
    uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type)
    {
        return stl::fill_n(first, n, x); 
    }

    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator
    uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type)
    {
        while (n--)
        {
            stl::construct(&*first, x);
            ++first;
        }
        return first;
    }

    template <typename ForwardIterator, typename Size, typename T, typename T1>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x, T1 *)
    {
        using is_POD = typename type_traits<T1>::is_POD_type;
        return uninitialized_fill_n_aux(first, n, x, is_POD());
    }


    /**
     * fill from first with n value x
     * @param   first	the beginning iterator of source range.
     * @param   n       the number of place to fill
     * @param   x       the value to fill.
     * @return  Iterator points the next position of the last copied element.
     **/
    template <typename ForwardIterator, typename Size, typename T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
    {
        return uninitialized_fill_n(first, n, x, value_type(first));
    }

    template <typename ForwardIterator, typename T>
    void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __true_type)
    {
        std::fill(first, last, x); // TODO: replace by our own copy function, as stl:fill
    }

    template <typename ForwardIterator, typename T>
    void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __false_type)
    {
        while (first != last)
        {
            construct(&*first, x);
            ++first;
        }
    }

    template <typename ForwardIterator, typename T, typename T1>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x, T1 *)
    {
        using is_POD = typename type_traits<T1>::is_POD_type;
        uninitialized_fill_aux(first, last, x, is_POD());
    }

    /**
     * fill [first, last) with value x
     * @param   first	the beginning iterator of source range.
     * @param   last    the end iterator of source range(not including).
     * @param   x       the value to fill.
     * @return  Iterator points the next position of the last copied element.
     **/
    template <typename ForwardIterator, typename T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x)
    {
        uninitialized_fill(first, last, x, value_type(first));
    }

} // namespace stl

#endif