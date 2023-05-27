//
// Created by rda on 2023/5/26.
//

#ifndef MINISTL_NUMERIC_HH
#define MINISTL_NUMERIC_HH

#include "iterator.hh"

namespace stl
{
    // accumulate
    template <typename InputIt, typename T>
    T accumulate(InputIt first, InputIt last, T init)
    {
        while (first != last)
            init += *first++;
        return init;
    }

    template <typename InputIt, typename T, typename BinaryOp>
    T accumulate(InputIt first, InputIt last, T init, BinaryOp op)
    {
        while (first != last)
            init = op(init, *first++);
        return init;
    }

    // adjacent_difference
    template <typename InputIt, typename OutputIt>
    OutputIt adjacent_difference(InputIt first, InputIt last, OutputIt result)
    {
        if (first == last)
            return result;
        
        using value_t = typename stl::iterator_traits<InputIt>::value_type;
        value_t acc = *first;
        *result = acc;

        while (++first != last)
        {
            value_t val = *first;
            *++result = val - std::move(acc);
            acc = std::move(val);
        }

        return ++result;
    }

    template <typename InputIt, typename OutputIt, typename BinaryOp>
    OutputIt adjacent_difference(InputIt first, InputIt last, OutputIt result, BinaryOp op)
    {
        if (first == last)
            return result;
        
        using value_t = typename stl::iterator_traits<InputIt>::value_type;
        value_t acc = *first;
        *result = acc;

        while (++first != last)
        {
            value_t val = *first;
            *++result = op(val, std::move(acc));
            acc = std::move(val);
        }

        return ++result;
    }

    // inner_product
    template <typename InputIt1, typename InputIt2, typename T>
    T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init)
    {
        while (first1 != last1)
        {
            init = std::move(init) + *first1 * *first2;
            ++first1;
            ++first2;
        }

        return init;
    }

    template <typename InputIt1, typename InputIt2, typename T, typename BinaryOp1, typename BinaryOp2>
    T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init, BinaryOp1 op1, BinaryOp2 op2)
    {
        while (first1 != last1)
        {
            init = op1(std::move(init), op2(*first1, *first2));
            ++first1;
            ++first2;
        }

        return init;
    }

    // partial_sum
    template <typename InputIt, typename OutputIt>
    OutputIt partial_sum(InputIt first, InputIt last, OutputIt result)
    {
        if (first == last)
            return result;

        typename stl::iterator_traits<InputIt>::value_type sum = *first;
        *result = sum;

        while (++first != last)
        {
            sum = std::move(sum) + *first;
            *++result = sum;
        }

        return ++result;
    }

    template <typename InputIt, typename OutputIt, typename BinaryOp>
    OutputIt partial_sum(InputIt first, InputIt last, OutputIt result, BinaryOp op)
    {
        if (first == last)
            return result;

        typename stl::iterator_traits<InputIt>::value_type acc = *first;
        *result = acc;

        while (++first != last)
        {
            acc = op(std::move(acc), *first);
            *++result = acc;
        }

        return ++result;
    }

    // power
    template <typename T, typename Integer>
    inline T power(T x, Integer n)
    {
        return x;
    }

    template <typename T, typename Integer, typename MonoidOp>
    inline T power(T x, Integer n, MonoidOp op)
    {
        return x;
    }

    // iota
    template <typename ForwardIt, typename T>
    void iota(ForwardIt first, ForwardIt last, T value)
    {
        while (first != last)
        {
            *first++ = value;
            ++value;
        }
    }

} // namespace stl

#endif