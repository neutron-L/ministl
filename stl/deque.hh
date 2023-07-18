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

        using iterator_category = bidirectional_iterator_tag;
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

        bool operator==(const Self &rhs) const { return node == rhs.node && cur == rhs.cur; }

        bool operator!=(const Self &rhs) const { return !this->operator==(rhs); }

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
            node = nullptr;
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
            /* 如果刚好整除，会多分配一个节点，此时finish指向最后一个
               空缓冲区的起始处
               如果count是0，也会分配一个空的缓冲区
              这意味着需要添加缓冲区时直接从finish.node+1开始
              而不用考虑finish.node == nullptr的清空
              */
            size_type num_of_nodes = count / BUFFER_SIZE + 1;

            map_size = std::max(BUFFER_SIZE, num_of_nodes + 2);

            map = map_allocator::allocate(map_size);
            memset(map, 0, map_size * sizeof(*map)); // 初始时清空map

            map_pointer nstart = map + (map_size - num_of_nodes) / 2;
            map_pointer nfinish = nstart + num_of_nodes - 1;

            map_pointer cur;

            try
            {
                /* code */
                for (cur = nstart; cur <= nfinish; ++cur)
                    allocate_node(cur);
            }
            catch (const std::exception &e)
            {
                // 释放已经分配的节点
                while (--cur >= nstart)
                    deallocate_node(cur);

                // 释放中继器
                map_allocator::deallocate(map);
                map_size = 0;
                map = nullptr;

                std::cerr << e.what() << '\n';
            }
            start.set_node(nstart);
            finish.set_node(nfinish);
            start.cur = start.first;
            finish.cur = finish.first + count % BUFFER_SIZE;
            length = count;
        }

        // 分配一块新的缓冲区，将其位置保存在map中的指定位置
        // 除非shrink fit，否则一般弹出元素也不会释放其占有的内存，而是以备添加元素的需求
        // 因此有可能mp处本来就有一块缓冲区，此时不会分配新的缓冲区
        void allocate_node(map_pointer mp)
        {
            if (!*mp)
                *mp = data_allocator::allocate(BUFFER_SIZE);
        }

        // 归还一个node
        void deallocate_node(map_pointer mp)
        {
            data_allocator::deallocate(*mp);
            *mp = nullptr; // 标注该指针为空
        }

        // 负责初始化一个deque并且填充count个value元素
        void fill_initialize(size_type count, const value_type &value)
        {
            create_map_and_nodes(count);

            map_pointer cur;

            try
            {
                // 应该可以uninitialized_fill(start, finish + 1, value)
                // 但是可能效率低一些
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
            // 如果map为空，比如该对象被std::move过，则直接返回
            if (!map)
                return;
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

                // 置空迭代器
                start.set_null();
                finish.set_null();
            }
            else
                finish = start; // finish和start指向同一个位置
        }

        // 确保当前deque有cap个元素空间，并且调整finish的位置使得length为cap
        // 缓冲区不够则需要重新申请，甚至重新分配更大的中继器空间
        void reserve(size_type cap)
        {
            // map为空可能是因为该deque被move过
            // 创建map并预留cap个元素即可
            if (!map)
            {
                create_map_and_nodes(cap);
                return;
            }

            if (length < cap)
            {

                size_type node_num = finish.node - start.node + 1;

                size_type reserve_node_num = (cap + BUFFER_SIZE - 1) / BUFFER_SIZE;
                auto diff_nums = reserve_node_num - node_num;

                // 1. map size够用
                if (reserve_node_num <= map_size)
                {
                    // 如果map尾部不足以存放新的缓冲区指针
                    // 则前移map中的缓冲区指针
                    if (map_size - (finish.node - map + 1) <
                        diff_nums)
                    {
                        map_pointer nstart = map + (map_size - reserve_node_num) / 2;
                        map_pointer nfinish = stl::copy(start.node, finish.node + 1, nstart) - 1;

                        start.set_node(nstart);
                        finish.set_node(nfinish);
                    }
                }
                else // 重新分配一个更大的map
                    reallocate_map(diff_nums, false);

                map_pointer mp = finish.node + 1;

                while (diff_nums--)
                {
                    allocate_node(mp);
                    ++mp;
                }
            }

            // reset finish
            finish = start + cap;
            length = cap;
        }

        void reallocate_map(size_type nodes_to_add, bool at_front)
        {
            size_type old_num_nodes = finish.node - start.node + 1;
            size_type new_num_nodes = old_num_nodes + nodes_to_add;

            map_pointer new_start;

            if (map_size > 2 * new_num_nodes)
            {
                new_start = map + (map_size - new_num_nodes) / 2 +
                            (at_front ? nodes_to_add : 0);

                if (new_start < start.node)
                    stl::copy(start.node, finish.node + 1, new_start);
                else if (new_start > start.node)
                    stl::copy_backward(start.node, finish.node + 1, new_start);
            }
            else
            {
                size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;

                map_pointer new_map = map_allocator::allocate(new_map_size);
                memset(new_map, 0, new_map_size * sizeof(*new_map)); // 清空！！！

                new_start = new_map + (new_map_size - new_num_nodes) / 2 +
                            (at_front ? nodes_to_add : 0);

                stl::copy(start.node, finish.node + 1, new_start);

                map = new_map;
                map_size = new_map_size;
            }

            start.set_node(new_start);
            finish.set_node(new_start + old_num_nodes - 1);
        }

        void reserve_map_at_back(size_type nodes_to_add = 1)
        {
            if (map_size - (finish.node - map + 1) < nodes_to_add)
                reallocate_map(nodes_to_add, false);
        }

        void reserve_map_at_front(size_type nodes_to_add = 1)
        {
            if (start.node - map < nodes_to_add)
                reallocate_map(nodes_to_add, true);
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
            create_map_and_nodes(stl::distance(first, last));

            stl::uninitialized_copy(first, last, begin());
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
        void assign(InputIt first, InputIt last)
        {
            clear();
            // 预留足够大的缓冲区空间
            reserve(stl::distance(first, last));

            stl::uninitialized_copy(first, last, begin());
        }

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

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist)
        {
            insert(pos, ilist.begin(), ilist.end());
        }

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

        stl::uninitialized_copy(other.begin(), other.end(), begin());
    }

    template <typename T, typename Alloc>
    deque<T, Alloc>::deque(deque &&other)
    {
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
        release_storage(true);
    }

    /*
     * assignment operation
     * */
    template <typename T, typename Alloc>
    deque<T, Alloc> &
    deque<T, Alloc>::operator=(const deque &other)
    {
        assign(other.begin(), other.end());

        return *this;
    }

    template <typename T, typename Alloc>
    deque<T, Alloc> &
    deque<T, Alloc>::operator=(deque &&other) noexcept
    {
        // 释放原有的控制数据结构和元素
        release_storage(true);

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

        return *this;
    }

    template <typename T, typename Alloc>
    deque<T, Alloc> &
    deque<T, Alloc>::operator=(std::initializer_list<T> ilist)
    {
        assign(ilist.begin(), ilist.end());

        return *this;
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::assign(size_type count, const T &value)
    {
        clear();
        // 预留足够大的缓冲区空间
        reserve(count);

        stl::uninitialized_fill_n(begin(), count, value);
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::assign(std::initializer_list<T> ilist)
    {
        assign(ilist.begin(), ilist.end());
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

    /*
     * Modifiers
     * */
    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::clear() noexcept
    {
        release_storage(false);
    }

    /*
     * Modifiers
     * */
    template <typename T, typename Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(const_iterator pos, const T &value)
    {
        value_type val = value;
        insert(pos, std::move(val));
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(const_iterator pos, T &&value)
    {
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(const_iterator pos, size_type count, const T &value)
    {
    }

    template <typename T, typename Alloc>
    template <class... Args>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::emplace(const_iterator pos, Args &&...args)
    {
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::erase(const_iterator pos)
    {
    }

    template <typename T, typename Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::erase(const_iterator first, const_iterator last)
    {
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::push_back(const T &value)
    {
        T v = value;
        push_back(std::move(v));
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::push_back(T &&value)
    {
        stl::construct(finish.cur++, std::move(value));

        if (finish.cur == finish.last)
        {
            reserve_map_at_back();
            allocate_node(finish.node + 1);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }

        ++length;
    }

    template <typename T, typename Alloc>
    template <class... Args>
    typename deque<T, Alloc>::reference
    deque<T, Alloc>::emplace_back(Args &&...args)
    {
        value_type val(std::forward<Args>(args)...);
        push_back(std::move(val));

        return *(end() - 1);
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::pop_back()
    {
        if (!length)
            return;
        if (finish.cur == finish.first)
        {
            finish.set_node(finish.node - 1);
            finish.cur = finish.last;
        }
        stl::destroy(--finish.cur);

        --length;
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::push_front(const T &value)
    {
        T v = value;
        push_back(std::move(v));
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::push_front(T &&value)
    {
        if (start.cur == start.first)
        {
            reserve_map_at_front();
            allocate_node(start.node - 1);

            start.set_node(start.node - 1);
            start.cur = start.last;
        }
        stl::construct(--start.cur, std::move(value));

        ++length;
    }

    template <typename T, typename Alloc>
    template <class... Args>
    typename deque<T, Alloc>::reference
    deque<T, Alloc>::emplace_front(Args &&...args)
    {
        value_type val(std::forward<Args>(args)...);
        push_front(std::move(val));

        return front();
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::pop_front()
    {
        if (!length)
            return;

        stl::destroy(start.cur++);

        if (start.cur == start.last)
        {
            start.set_node(start.node + 1);
            start.cur = start.first;
        }
        --length;
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::resize(size_type count)
    {
        resize(count, value_type());
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::resize(size_type count, const value_type &value)
    {
        auto old_finish = finish;
        auto old_length = length;

        reserve(count);

        if (count < old_length)
            stl::destroy(begin() + count, old_finish);
        else if (count > old_length) // 不够的元素按照value构造
            stl::uninitialized_fill_n(old_finish, count - old_length, value);
    }

    template <typename T, typename Alloc>
    void
    deque<T, Alloc>::swap(deque &other) noexcept
    {
        stl::swap(map, other.map);
        stl::swap(start, other.start);
        stl::swap(finish, other.finish);
        stl::swap(map_size, other.map_size);
        stl::swap(length, other.length);
    }

} // namespace stl

#endif
