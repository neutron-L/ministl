//
// Created by rda on 2023/7/14.
//

#ifndef MINISTL_DEQUE_HH
#define MINISTL_DEQUE_HH

#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <cstring>
#include <limits>

#include "alloc.hh"
#include "iterator.hh"
#include "uninitialized.hh"
#include "algobase.hh"

namespace stl
{
    /* Deque Iterator */
    template <typename T>
    struct deque_iterator
    {
        using iterator = deque_iterator<T>;
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;
        using size_type = std::size_t;
        using difference_type = ptrdiff_t;
        using map_pointer = pointer *;

        using Self = deque_iterator<T>;

        pointer cur;   // 此迭代器所指的缓冲区的现元素
        pointer first; // 此迭代器所指的缓冲区的头
        pointer last;  // 此迭代器所指的缓冲区的尾

        map_pointer node; // 指向中继器中指向缓冲区的节点

        reference operator*() const { return *cur; }
        pointer operator->() const { return &(operator*()); }

        difference_type operator-(const Self &rhs) const {}

        Self &operator++()
        {
            ++cur;

            if (cur == last)
            {
                set_node(node + 1);
                cur = first;
            }

            return *this;
        }


        Self operator++(int)
        {
            Self tmp = *this;
            ++*this;

            return tmp;
        }

        Self &operator--()
        {
            if (cur != first)
                --cur;
            else
            {
                set_node(node - 1);
                cur = last - 1;
            }

            return *this;
        }

        Self operator--(int)
        {
            Self tmp = *this;
            --*this;

            return tmp;
        }

        Self &operator+=(difference_type n)
        {
            difference_type offset = cur - first + n;
            difference_type node_offset;

            if (offset >= 0)
                node_offset = offset / BUFFER_SIZE;
            else
                node_offset = -(-offset + BUFFER_SIZE - 1) / BUFFER_SIZE;
            set_node(node + node_offset);
            cur = first + offset - node_offset * BUFFER_SIZE;

            return *this;
        }

        Self operator+(difference_type n)
        {
            Self tmp = *this;
            tmp += n;

            return tmp;
        }

        Self &operator-=(difference_type n)
        {
            return *this += -n;
        }

        Self operator-(difference_type n)
        {
            Self tmp = *this;
            tmp -= n;

            return tmp;
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }

        bool operator==(const Self &rhs) const { return node == rhs.node && first == rhs.first && last == rhs.last && cur == rhs.cur; }

        bool operator!=(const Self &rhs) const { !this->operator==(rhs);}
        
        bool operator<(const Self &rhs) const 
        {
            if (node == rhs.node) ? cur < rhs.cur : node < rhs.node;
        }

    protected:
        static const int BUFFER_SIZE = 8;

        void set_node(map_pointer new_node)
        {
            node = new_node;
            first = *node;
            last = first + BUFFER_SIZE;
        }
    };



    /* Deque */
    template <typename T, typename Alloc = alloc>
    class deque
    {
    public:
        /* Member types */
        using value_type = T;
        using allocator_type = Alloc;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

        using iterator = deque_iterator<T>;
        using const_iterator = deque_iterator<const T>;
        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

    protected:
        using data_allocator = simple_alloc<value_type, Alloc>;
        using map_pointer = pointer *;

        iterator start;
        iterator finish;

        map_pointer map; //

        size_type map_size; // map内指针数

    public:
        /*
         * Constructors
         * */
        deque();

        deque(size_type count, const T &value);

        explicit deque(size_type count);

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        deque(InputIt first, InputIt last);

        deque(const deque &other);
        deque(deque &&other);
        deque(std::initializer_list<T> init);

        /*
         * Destructor
         * */
        ~deque();

        /*
         * assignment operation
         * */
        deque &operator=(const deque &other);
        deque &operator=(deque &&other) noexcept;
        deque &operator=(std::initializer_list<T> ilist);

        void assign(size_type count, const T &value);
        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>

        void assign(InputIt first, InputIt last);
        void assign(std::initializer_list<T> ilist);
    };

} // namespace stl

#endif
