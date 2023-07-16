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
    static const std::size_t BUFFER_SIZE = 8;

    /* Deque Iterator */
    template <typename Tp, typename Ref, typename Ptr>
    struct deque_iterator
    {
        using iterator = deque_iterator<Tp, Tp &, Tp *>;
        using const_iterator = deque_iterator<Tp, const Tp &, const Tp *>;

        using iterator_category = random_access_iterator_tag;
        using value_type = Tp;
        using pointer = Ptr;
        using reference = Ref;
        using size_type = std::size_t;
        using difference_type = ptrdiff_t;
        using map_pointer = Tp **;

        using Self = deque_iterator;

        pointer cur{};   // 此迭代器所指的缓冲区的现元素
        pointer first{}; // 此迭代器所指的缓冲区的头
        pointer last{};  // 此迭代器所指的缓冲区的尾

        map_pointer node{}; // 指向中继器中指向缓冲区的节点

        deque_iterator() = default;

        deque_iterator(map_pointer n, pointer f, pointer l, pointer c)
            : node(n), first(f), last(l), cur(c)
        {
        }

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

        Self operator+(difference_type n) const
        {
            Self tmp = *this;
            tmp += n;

            return tmp;
        }

        Self &operator-=(difference_type n)
        {
            return *this += -n;
        }

        Self operator-(difference_type n) const
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

        bool operator!=(const Self &rhs) const { !this->operator==(rhs); }

        bool operator<(const Self &rhs) const
        {
            return (node == rhs.node)
                       ? cur < rhs.cur
                       : node < rhs.node;
        }

        void set_node(map_pointer new_node)
        {
            node = new_node;
            first = *node;
            last = first + BUFFER_SIZE;
        }

        // 置空一个迭代器
        void set_null()
        {
            first = last = cur = nullptr;
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

        using iterator = deque_iterator<T, T &, T *>;
        using const_iterator = deque_iterator<T, const T &, const T *>;

        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

    protected:
        using data_allocator = simple_alloc<value_type, Alloc>;
        using map_allocator = simple_alloc<pointer, Alloc>;

        using map_pointer = pointer *;

        iterator start{};  // 指向第一个元素
        iterator finish{}; // 指向最后一个元素的下一个位置

        // 指向中控器，是一个指针数组，其中每个指针指向一块缓冲区，
        // 缓冲区存放元素，每个缓冲区称为一个“节点”
        map_pointer map{};

        size_type map_size{}; // map内指针数
        size_type length{};   // 元素数

        // 创建map结构，并分配相应的节点
        void create_map_and_nodes(size_type count) noexcept
        {
            size_type num_of_nodes = count / BUFFER_SIZE + 1;

            map_size = std::max(BUFFER_SIZE, num_of_nodes + 2);

            map = map_allocator::allocate(map_size);

            map_pointer nstart = map + (map_size - num_of_nodes) / 2;
            map_pointer nfinish = nstart + num_of_nodes - 1;

            map_pointer cur;

            try
            {
                /* code */
                for (cur = nstart; cur <= nfinish; ++cur)
                    *cur = allocate_node();
            }
            catch (const std::exception &e)
            {
                // 释放已经分配的节点
                while (--cur >= nstart)
                    deallocate_node(*cur);

                // 释放中继器
                map_allocator::deallocate(map);
                map_size = 0;
                map = nullptr;

                std::cerr << e.what() << '\n';
            }
            start.set_node(nstart);
            finish.set_node(nfinish);
            start.cur = start.first;

            length = count;

            finish.cur = finish.first + count % BUFFER_SIZE;
        }

        // 分配一块新的缓冲区
        pointer allocate_node()
        {
            return data_allocator::allocate(BUFFER_SIZE);
        }

        // 归还一个node
        void deallocate_node(pointer node)
        {
            data_allocator::deallocate(node);
        }

        // 负责初始化一个deque并且填充count个value元素
        void fill_initialize(size_type count, const value_type &value)
        {
            create_map_and_nodes(count);

            map_pointer cur;

            try
            {
                for (cur = start.node; cur < finish.node; ++cur)
                    stl::uninitialized_fill(*cur, *cur + BUFFER_SIZE, value);
                stl::uninitialized_fill(finish.first, finish.cur, value);
            }
            catch (const std::exception &e)
            {
                std::cerr << "do not know what to do...\n";

                std::cerr << e.what() << '\n';
            }
        }

        // 销毁所有的元素
        // 根据参数指示是否销毁控制结构，即中控器和所有node
        void release_storage(bool release_all)
        {
            map_pointer mp;

            for (mp = start.node; mp <= finish.node; ++mp)
            {
                stl::destroy(*mp, *mp + (mp == finish.node ? length % BUFFER_SIZE : BUFFER_SIZE));
                if (release_all)
                    data_allocator::deallocate(*mp);
            }
            length = 0;

            if (release_all)
            {
                // 释放中控器
                map_allocator::deallocate(map);
                map_size = 0;
                map = nullptr;
            }
        }

    public:
        /*
         * Constructors
         * */
        deque();

        explicit deque(size_type count, const T &value = T());

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        deque(InputIt first, InputIt last)
        {
            // 分配中控器和默认大小的相应的缓冲区
            create_map_and_nodes(0);

            while (first != last)
                push_back(*first++);
        }

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

        /*
         * Element access
         * */
        reference at(size_type pos);
        const_reference at(size_type pos) const;

        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        /*
         * Iterator
         * */
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;

        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

        /*
         * Capacity
         * */
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;
        void shrink_to_fit();

        /*
         * Modifiers
         * */
        void clear() noexcept;
        iterator insert(const_iterator pos, const T &value);
        iterator insert(const_iterator pos, T &&value);
        iterator insert(const_iterator pos, size_type count, const T &value);
        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last);
        iterator insert(const_iterator pos, std::initializer_list<T> ilist);

        template <class... Args>
        iterator emplace(const_iterator pos, Args &&...args);

        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        void push_back(const T &value);
        void push_back(T &&value);
        template <class... Args>
        reference emplace_back(Args &&...args);
        void pop_back();

        void push_front(const T &value);
        void push_front(T &&value);

        template <class... Args>
        void emplace_front(Args &&...args);
        template <class... Args>
        reference emplace_front(Args &&...args);

        void pop_front();

        void resize(size_type count);
        void resize(size_type count, const value_type &value);

        void swap(deque &other) noexcept;
    };

    /*
     * Constructors
     * */
    template <typename T, typename Alloc>
    deque<T, Alloc>::deque()
    {
        fill_initialize(0, value_type()); // 仅仅创建中继器
    }

    template <typename T, typename Alloc>
    deque<T, Alloc>::deque(size_type count, const T &value)
    {
        fill_initialize(count, value);
    }

    template <typename T, typename Alloc>
    deque<T, Alloc>::deque(const deque &other)
    {
        // 创建足够的map结构和缓冲区
        create_map_and_nodes(other.size());

        for (size_type i = 0; i < length; ++i)
            stl::construct(&(*this)[i], other[i]);
    }

    template <typename T, typename Alloc>
    deque<T, Alloc>::deque(deque &&other)
    {
        // 释放已有空间和元素
        release_storage();

        // 接管other的元素
        map = other.map;
        start = other.start;
        finish = other.finish;
        map_size = other.map_size;
        length = other.length;

        other.map = nullptr;
        other.start.set_null();
        other.finish.set_null();
        other.length = other.map_size = 0;
    }

    template <typename T, typename Alloc>
    deque<T, Alloc>::deque(std::initializer_list<T> init)
        : deque(init.begin(), init.end()) {}

    /*
     * Destructor
     * */
    template <typename T, typename Alloc>
    deque<T, Alloc>::~deque()
    {
        clear();
    }

    /*
     * Element access
     * */

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::reference
    deque<T, Alloc>::at(size_type pos)
    {
        return const_cast<reference>(static_cast<const deque &>(*this)[pos]);
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reference
    deque<T, Alloc>::at(size_type pos) const
    {
        if (pos >= length)
        {
            error("%ld is larger than size %ld", pos, size());
            throw new std::out_of_range("");
        }

        return start[pos];
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::reference
    deque<T, Alloc>::operator[](size_type pos)
    {
        return const_cast<reference>(const_cast<const deque &>(*this)[pos]);
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reference
    deque<T, Alloc>::operator[](size_type pos) const
    {
        return start[pos];
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::reference
    deque<T, Alloc>::front()
    {
        return const_cast<reference>(const_cast<const deque &>(*this).front());
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reference
    deque<T, Alloc>::front() const
    {
        return *start;
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::reference
    deque<T, Alloc>::back()
    {
        return const_cast<reference>(const_cast<const deque &>(*this).front());
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reference
    deque<T, Alloc>::back() const
    {
        return *(finish - 1);
    }

    /*
     * Iterator
     * */
    template <typename T, typename Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::begin() noexcept
    {
        return start;
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_iterator
    deque<T, Alloc>::begin() const noexcept
    {
        return const_iterator(start.node, start.first, start.last, start.cur);
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_iterator
    deque<T, Alloc>::cbegin() const noexcept
    {
        return const_iterator(start.node, start.first, start.last, start.cur);
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::end() noexcept
    {
        return finish;
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_iterator
    deque<T, Alloc>::end() const noexcept
    {
        return const_iterator(finish.node, finish.first, finish.last, finish.cur);
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_iterator
    deque<T, Alloc>::cend() const noexcept
    {
        return const_iterator(finish.node, finish.first, finish.last, finish.cur);
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::reverse_iterator
    deque<T, Alloc>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reverse_iterator
    deque<T, Alloc>::rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reverse_iterator
    deque<T, Alloc>::crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::reverse_iterator
    deque<T, Alloc>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reverse_iterator
    deque<T, Alloc>::rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::const_reverse_iterator
    deque<T, Alloc>::crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    /*
     * Capacity
     * */
    template <typename T, typename Alloc>
    bool
    deque<T, Alloc>::empty() const noexcept
    {
        return length == 0;
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::size_type
    deque<T, Alloc>::size() const noexcept
    {
        return length;
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::size_type
    deque<T, Alloc>::max_size() const noexcept
    {
        return static_cast<size_type>(-1l);
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::shrink_to_fit()
    {
    }

} // namespace stl

#endif
