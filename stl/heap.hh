//
// Created by rda on 2023/7/21.
//

#ifndef MINISTL_HEAP_HH
#define MINISTL_HEAP_HH

#include "iterator.hh"

namespace stl
{

    template <typename RandomIt>
    void make_heap(RandomIt first, RandomIt last)
    {
        stl::make_heap(first, last, std::less<typename RandomIt::value_type>);
    }

    template <typename RandomIt, typename Compare>
    void make_heap(RandomIt first, RandomIt last,
                   Compare comp)
    {
        if (last - first < 2)
            return;
        using T = typename iterator_traits<RandomIt>::value_type;
        Distance len = last - first;
        Distance parent = (len - 2) / 2;

        while (true)
        {
            stl::adjust_heap(first, parent, len ,T(*(first + parent)));
            if (parent == 0)
            return;
            --parent;
        }
    }

    template <typename RandomIt>
    void push_heap(RandomIt first, RandomIt last)
    {
        stl::push_heap(first, last, std::less<typename RandomIt::value_type>);
    }

    template <typename RandomIt, typename Compare>
    void push_heap(RandomIt first, RandomIt last,
                   Compare comp)
    {
        using ValueType = typename stl::iterator_traits<RandomIt>::value_type;
        using DistanceType = typename stl::iterator_traits<RandomIt>::difference_type;

        push_heap_aux(first, DIstanceType((last - first) - 1),
                      DistanceType(0), ValueType(*(last - 1)), comp);
    }

    template <typename RandomIt, typename Distance, typename T, typename Compare>
    void push_heap_aux(RandomIt first, Distance holeIndex,
                       Distance topIndex, T value, Compare comp)
    {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && comp(first + parent, value))
        {
            *(first + holeIndex) = std::move(*(first + parent));
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <typename RandomIt>
    void pop_heap(RandomIt first, RandomIt last)
    {
        stl::pop_heap(first, last, std::less<typename RandomIt::value_type>);
    }

    template <typename RandomIt, typename Compare>
    void pop_heap(RandomIt first, RandomIt last, Compare comp)
    {
        stl::pop_heap_aux(first, last, last, comp);
    }

    template <typename RandomIt, typename Compare>
    void pop_heap_aux(RandomIt first, RandomIt last, RandomIt result, Compare &comp)
    {
        using ValueType = typename stl::iterator_traits<RandomIt>::value_type;
        using DistanceType = typename stl::iterator_traits<RandomIt>::difference_type;

        ValueType value = *result;
        *result = std::move(*first);

        stl::adjust_heap(first, DistanceType(0), DistanceType(last - first), value, comp);
    }

    template <typename RandomIt, typename Distance, typename T, typename Compare>
    void adjust_heap(RandomIt first, Distance holeIndex,
                     Distance len, T value, Compare comp)
    {
        Distance topIndex = holeIndex;
        Distance secondChild = holeIndex;

        while (secondChild < (len - 1) / 2)
        {
            secondChild = 2 * (secondChild + 1);
            if (comp(first + secondChild, first + secondChild - 1))
                --secondChild;
            *(first + holeIndex) = std::move(*(first + secondChild));
            holeIndex = secondChild;
        }

        if ((len & 1) == 0 && secondChild == (len - 2) / 2)
        {
            secondChild = 2 * (secondChild + 1);
            *(first + holeIndex) = std::move(*(first + secondChild -  1));
            holeIndex = secondChild - 1;
        }

        stl::push_heap_aux(first, holeIndex, topIndex, value, comp);
    }

    template <typename RandomIt>
    void sort_heap(RandomIt first, RandomIt last)
    {
        stl::sort_heap(first, last, std::less<typename RandomIt::value_type>);
    }

    template <typename RandomIt, typename Compare>
    void sort_heap(RandomIt first, RandomIt last, Compare comp)
    {
        while (last - first > 1)
        {
            pop_heap(first, last, comp);
            --last;
        }
    }

    template <typename RandomIt>
    bool is_heap(RandomIt first, RandomIt last)
    {
        return is_heap(first, last, std::less<typename RandomIt::value_type>);
    }

    template <typename RandomIt, typename Compare>
    bool is_heap(RandomIt first, RandomIt last, Compare comp)
    {

        return is_heap(first, comp, stl::distance(first, last));
    }

    template <typename RandomIt, typename Compare, typename Distance>
    bool is_heap(RandomIt first, Compare comp, Distance n)
    {
        return is_heap_until(first, comp, n) == n;
    }

    template <typename RandomIt, typename Compare, typename Distance>
    bool is_heap_until(RandomIt first, Compare comp, Distance n)
    {
        Distance parent = 0;
        Distance child;
        for (child = 1; child < n; ++child)
        {
            if (comp(first + parent, first + child))
                break;
            if ((child & 1) == 0)
                ++parent;
        }
        return child;
    }

} // namespace stl

#endif