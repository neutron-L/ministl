//
// Created by rda on 2023/4/5.
//

#ifndef MINISTL_ITERATOR_HH
#define MINISTL_ITERATOR_HH

#include <cstddef>

namespace stl
{
    struct input_iterator
    {
    };
    struct output_iterator
    {
    };
    struct forward_iterator : public input_iterator
    {
    };
    struct bidirectional_iterator : public forward_iterator
    {
    };
    struct random_access_iterator : public bidirectional_iterator
    {
    };

    template<typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T *, typename Reference = T &>
    struct iterator
    {
        using iterator_category = Category;
        using value_type = T;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
    };


    template<typename Iterator>
    struct iterator_traits
    {
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    template<typename T>
    struct iterator_traits<T *>
    {
        using iterator_category = random_access_iterator;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T &;
    };

    template<typename T>
    struct iterator_traits<const T *>
    {
        using iterator_category = random_access_iterator;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T &;
    };

    template<typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator &)
    {
        using category = typename iterator_traits<Iterator>::iterator_category;
        return category();
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type *
    distance_type(const Iterator &)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::value_type *
    value_type(const Iterator &)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
    }

    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance_aux(InputIterator first, InputIterator last, input_iterator)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance_aux(InputIterator first, InputIterator last, random_access_iterator)
    {
        return last - first;
    }


    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last)
    {
        using category = typename iterator_traits<InputIterator>::iterator_category;
        return distance_aux(first, last, iterator_category(first));
    }


    template <typename InputIterator, typename Distance>
    inline void
    advance_aux(InputIterator & i, Distance n, input_iterator)
    {
        while (n--)
            ++i;
    }

    template <typename InputIterator, typename Distance>
    inline void
    advance_aux(InputIterator & i, Distance n, bidirectional_iterator)
    {
        if (n >= 0)
            while (n--)
                ++i;
        else
            while (n++)
                --i;
    }

    template <typename InputIterator, typename Distance>
    inline void
    advance_aux(InputIterator & i, Distance n, random_access_iterator)
    {
        i += n;
    }

    template <typename InputIterator, typename Distance>
    inline void
    advance(InputIterator & i, Distance n)
    {
        advance_aux(i, n, iterator_category(i));
    }
} // namespace stl

#endif //UNTITLED_ITERATOR_H
