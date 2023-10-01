//
// Created by rda on 2023/5/26.
//

#ifndef MINISTL_ALGOBASE_HH
#define MINISTL_ALGOBASE_HH

#include <utility>
#include <cstring>

#include "construct.hh"
#include "iterator.hh"

namespace stl
{
    /* Minimum/maximum operations */
    template <class T>
    const T &max(const T &a, const T &b)
    {
        return (b < a) ? a : b;
    }
    template <class T>
    const T &min(const T &a, const T &b)
    {
        return (b < a) ? b : a;
    }

    /* Non-modifying sequence operations */
    template <class InputIt1, class InputIt2>
    std::pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1,
             InputIt2 first2)
    {
        while (first1 != last1 && *first1 == *first2)
        {
            ++first1;
            ++first2;
        }

        return {first1, first2};
    }

    /* Modifying sequence operations */
    template <typename InputIt, typename OutputIt>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
    {
        while (first != last)
            *d_first++ = *first++;
        return d_first;
    }

    template <typename InputIt, typename OutputIt, typename UnaryPredictate>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first, UnaryPredictate pred)
    {
        for (; first != last; ++first)
        {
            if (pred(*first))
            {
                *d_first = *first;
                ++d_first;
            }
        }

        return d_first;
    }

    template <typename BidirIt1, typename BidirIt2>
    BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
    {
        while (--last >= first)
        {
            --d_last;
            stl::construct(&*d_last, *last);
        }

        return d_last;
    }

    template <typename InputIt, typename Size, typename OutputIt>
    OutputIt copy_n(InputIt first, Size count, OutputIt result)
    {
        while (count--)
            *result++ = *first++;
        return result;
    }

    template <typename ForwardIt, typename T>
    void fill(ForwardIt first, ForwardIt last, const T &value)
    {
        while (first != last)
            *first++ = value;
    }

    template <typename OutputIt, typename Size, typename T>
    OutputIt fill_n(OutputIt first, Size count, const T &value)
    {
        while (count--)
            *first++ = value;
        return first;
    }

    template <class T>
    void swap(T &a, T &b) noexcept
    {
        T tmp = a;
        a = b;
        b = tmp;
    }

    template <typename T, std::size_t N>
    void swap(T (&a)[N], T (&b)[N])
    {
        for (std::size_t i = 0; i < N; ++i)
            swap(a[i], b[i]);
    }

    template <typename ForwardIt1, typename ForwardIt2>
    void iter_swap(ForwardIt1 a, ForwardIt2 b)
    {
        swap(*a, *b);
    }

    /*
     * Comparison operations
     * */
    template <typename InputIt1, typename InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
    {
        while (first1 != last1 && *first1 == *first2)
        {
            ++first1;
            ++first2;
        }
        return first1 == last1;
    }

    template <class InputIt1, class InputIt2, class BinaryPredicate>
    bool equal(InputIt1 first1, InputIt1 last1,
               InputIt2 first2, BinaryPredicate p)
    {
        while (first1 != last1 && p(*first1, *first2))
        {
            ++first1;
            ++first2;
        }

        return first1 == last1;
    }

    template <typename InputIt1, typename InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
                return true;
            else if (*first1 > *first2)
                return false;
            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 != last2;
    }

    template <typename InputIt1, typename InputIt2, typename Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                 InputIt2 first2, InputIt2 last2, Compare cmp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (cmp(*first1, *first2))
                return true;
            else if (cmp(*first2, *first1))
                return false;
            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 != last2;
    }

    template <>
    bool lexicographical_compare(const char *first1, const char *last1, const char *first2, const char *last2)
    {
        const int len1 = last1 - first1;
        const int len2 = last2 - first2;
        int result = strncmp(first1, first2, stl::min(len1, len2));

        return result != 0 ? result < 0 : len1 < len2;
    }

    /*
     * Search
     * */
    template <typename ForwardIt, typename T, typename Distance>
    ForwardIt lower_bound_aux(ForwardIt first, ForwardIt last, const T &value, Distance *, forward_iterator_tag)
    {
        Distance len = stl::distance(first, last);
        ForwardIt middle;
        Distance half;

        while (len > 0)
        {
            half = len >> 1;
            middle = first;
            stl::advance(middle, half);
            if (*middle < value)
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else
                len = half;
        }

        return first;
    }

    template <typename ForwardIt, typename T, typename Distance>
    ForwardIt lower_bound_aux(ForwardIt first, ForwardIt last, const T &value, Distance *, random_access_iterator_tag)
    {
        Distance len = last - first;
        ForwardIt middle;
        Distance half;

        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if (*middle < value)
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else
                len = half;
        }

        return first;
    }

    template <typename ForwardIt, typename T>
    inline ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T &value)
    {
        return lower_bound_aux(first, last, value, stl::distance_type(first), stl::iterator_category(first));
    }

    template <typename ForwardIt, typename T, typename Distance>
    ForwardIt upper_bound_aux(ForwardIt first, ForwardIt last, const T &value, Distance *, forward_iterator_tag)
    {
        Distance len = stl::distance(first, last);
        ForwardIt middle;
        Distance half;

        while (len > 0)
        {
            half = len >> 1;
            middle = first;
            stl::advance(middle, half);
            if (value < *middle)
                len = half;
            else
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }

        return first;
    }

    template <typename ForwardIt, typename T, typename Distance>
    ForwardIt upper_bound_aux(ForwardIt first, ForwardIt last, const T &value, Distance *, random_access_iterator_tag)
    {
        Distance len = last - first;
        ForwardIt middle;
        Distance half;

        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if (value < *middle)
                len = half;
            else
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }

        return first;
    }

    template <typename ForwardIt, typename T>
    inline ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T &value)
    {
        return upper_bound_aux(first, last, value, stl::distance_type(first), stl::iterator_category(first));
    }

} // namespace stl

#endif