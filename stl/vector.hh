//
// Created by rda on 2023/5/14.
//

#ifndef MINISTL_VECTOR_HH
#define MINISTL_VECTOR_HH

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
    template <typename T, typename Alloc = alloc>
    class vector
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

        using iterator = value_type *;
        using const_iterator = const value_type *;
        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

    protected:
        using data_allocator = simple_alloc<value_type, Alloc>;

        iterator start{};
        iterator finish{};
        iterator end_of_storage{};

        iterator allocate_and_fill(size_type n, const T &value);

        void deallocate();

        void fill_initialize(size_type n, const T &value);

        void reallocate(size_type n);

    public:
        /*
         * constructor
         * */
        vector() = default;

        vector(const vector &other);

        vector(vector &&other) noexcept;

        explicit vector(size_type n)
        {
            fill_initialize(n, T());
        }

        vector(size_type n, const T &elem)
        {
            fill_initialize(n, elem);
        }

        vector(int n, const T &elem)
        {
            fill_initialize(n, elem);
        }

        vector(long n, const T &elem)
        {
            fill_initialize(n, elem);
        }

        template <typename InputIterator>
        vector(InputIterator first, InputIterator last);

        vector(std::initializer_list<T>);

        /*
         *  destructor
         * */
        ~vector()
        {
            destroy(start, finish);
            deallocate();
        }

        /*
         * assignment operation
         * */
        vector &operator=(const vector &other);

        vector &operator=(vector &&other) noexcept;

        vector &operator=(std::initializer_list<T> ilist);

        void assign(size_type count, const T &elem);

        template <typename InputIt>
        void assign(InputIt first, InputIt last);

        void assign(std::initializer_list<T>);

        allocator_type get_allocator() const noexcept;

        /*
         * Element access
         * */
        reference at(size_type pos)
        {
            return const_cast<reference>(static_cast<const vector &>(*this).at(pos));
        }

        const_reference at(size_type pos) const
        {
            if (pos >= size())
            {
                error("%d is larger than size %d", pos, size());
                throw new std::out_of_range("");
            }
            return *(begin() + pos);
        }

        reference operator[](size_type pos)
        {
            return const_cast<reference>(static_cast<const vector &>(*this)[pos]);
        }

        const_reference operator[](size_type pos) const
        {
            return *(begin() + pos);
        }

        reference front()
        {
            return const_cast<reference>(static_cast<const vector &>(*this).front());
        }

        const_reference front() const
        {
            return *begin();
        }

        reference back()
        {
            return const_cast<reference>(static_cast<const vector &>(*this).back());
        }

        const_reference back() const
        {
            return *(end() - 1);
        }

        T *data() noexcept
        {
            return const_cast<T *>(static_cast<const vector &>(*this).data());
        }
        const T *data() const noexcept
        {
            return start;
        }

        /*
         * Iterator function
         * */
        iterator begin() noexcept
        {
            return const_cast<iterator>(static_cast<const vector &>(*this).begin());
        }

        const_iterator begin() const noexcept
        {
            return start;
        }

        const_iterator cbegin() const noexcept
        {
            return start;
        }

        iterator end() noexcept
        {
            return const_cast<iterator>(static_cast<const vector &>(*this).end());
        }

        const_iterator end() const noexcept
        {
            return finish;
        }

        const_iterator cend() const noexcept
        {
            return finish;
        }

        reverse_iterator rbegin() noexcept
        {
            return const_cast<reverse_iterator>(static_cast<const vector &>(*this).rbegin());
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() noexcept
        {
            return const_cast<reverse_iterator>(static_cast<const vector &>(*this).rend());
        }

        const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        /*
         * Capacity
         * */
        bool empty() const noexcept
        {
            return begin() == end();
        }

        size_type size() const noexcept
        {
            return stl::distance(begin(), end());
        }

        size_type max_size() const noexcept // copy from cppreference
        {
            return std::numeric_limits<difference_type>::max();
        }

        void reserve(size_type new_cap);

        size_type capacity() const noexcept
        {
            return size_type(end_of_storage - start);
        }

        void shrink_to_fit();

        /*
         * Modifiers
         * */
        void clear() noexcept;

        // insert
        iterator insert(const_iterator pos, const T &elem);
        iterator insert(const_iterator pos, T &&elem);
        iterator insert(const_iterator pos, size_type count, const T &elem);
        template <typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last);
        iterator insert(const_iterator pos, std::initializer_list<T> ilist);

        template <typename... Args>
        iterator emplace(const_iterator pos, Args &&...args);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        void push_back(const T &elem);

        void push_back(T &&elem);

        template <typename... Args>
        reference emplace_back(Args &&...args);

        void pop_back();

        void resize(size_type size);
        void resize(size_type size, const value_type &elem);

        void swap(vector &other) noexcept;
    };

    /*
     * Protected function
     * */
    template <typename T, typename Alloc>
    auto vector<T, Alloc>::allocate_and_fill(size_type n, const T &value) -> iterator
    {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, value);
        return result;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::deallocate()
    {
        if (start)
            data_allocator::deallocate(start, end_of_storage - start);
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::fill_initialize(size_type n, const T &value)
    {
        start = allocate_and_fill(n, value);
        finish = start;
        stl::advance(finish, n);
        end_of_storage = finish;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::reallocate(size_type n)
    {
        T * new_start = nullptr;
        T * new_finish = nullptr;
        if (n)
        {
            new_start = data_allocator::allocate(n);
            new_finish = new_start;
            new_finish = stl::uninitiazed_copy(begin(), end(), new_finish);
        }

        // destroy and release old storage
        stl::destroy(begin(), end());
        deallocate();

        start = new_start;
        finish = new_finish;
        if (start)
            end_of_storage = start + n;
    }

    /*
     * Constructors
     * */
    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(const vector &rhs)
    {
        size_type size = rhs.size();
        start = finish = data_allocator::allocate(size);

        for (auto &item : rhs)
            stl::construct(finish++, item);
        end_of_storage = finish;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(vector &&rhs) noexcept : start(rhs.start), finish(rhs.finish), end_of_storage(rhs.end_of_storage)
    {
        rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
    }

    template <typename T, typename Alloc>
    template <typename InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last)
    {
        start = finish = data_allocator::allocate(stl::distance(first, last));
        while (first != last)
            construct(finish++, *first++);
        end_of_storage = finish;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(std::initializer_list<T> lst) : vector(lst.begin(), lst.end())
    {
    }

    /*
     * Assignment operation
     * */
    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T, Alloc> &rhs)
    {
        assign(rhs.begin(), rhs.end());
        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(vector<T, Alloc> &&rhs) noexcept
    {
        // release old storage
        destroy(begin(), end());
        deallocate();

        start = rhs.start;
        finish = rhs.finish;
        end_of_storage = rhs.end_of_storage;

        rhs.start = rhs.finish = rhs.end_of_storage = nullptr;

        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(std::initializer_list<T> lst)
    {
        assign(lst.begin(), lst.end());

        return *this;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::assign(size_type n, const T &elem)
    {
        destroy(begin(), end());

        if (capacity() < n)
        {
            start = data_allocator::allocate(n);
            end_of_storage = start + n;
        }
        finish = start;
        finish = stl::uninitialized_fill_n(finish, n, elem);
    }

    template <typename T, typename Alloc>
    template <typename InputIt>
    void vector<T, Alloc>::assign(InputIt first, InputIt last)
    {
        destroy(begin(), end());
        size_type n = stl::distance(first, last);

        if (capacity() < n)
        {
            start = data_allocator::allocate(n);
            end_of_storage = start + n;
        }
        finish = start;
        finish = stl::uninitiazed_copy(first, last, finish);
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::assign(std::initializer_list<T> lst)
    {
        assign(lst.begin(), lst.end());
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::swap(vector<T, Alloc> &other) noexcept
    {
        stl::swap(start, other.start);
        stl::swap(finish, other.finish);
        stl::swap(end_of_storage, other.end_of_storage);
    }

    /*
     * Capacity
     * */
    template <typename T, typename Alloc>
    void vector<T, Alloc>::reserve(size_type new_cap)
    {
        if (capacity() >= new_cap)
            return;
        reallocate(new_cap);
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::shrink_to_fit()
    {
        if (finish != end_of_storage)
            reallocate(size());
    }

    /*
     * Modifiers
     * */
    template <typename T, typename Alloc>
    void vector<T, Alloc>::clear() noexcept
    {
        erase(begin(), end());
        finish = start;
    }

    /* Non-member functions */
    template <typename T, typename Alloc>
    bool operator==(const stl::vector<T, Alloc> &lhs,
                    const stl::vector<T, Alloc> &rhs)
    {
        return false;
    }

    template <typename T, typename Alloc>
    bool operator!=(const stl::vector<T, Alloc> &lhs,
                    const stl::vector<T, Alloc> &rhs)
    {
        return false;
    }

    template <typename T, typename Alloc>
    bool operator<(const stl::vector<T, Alloc> &lhs,
                   const stl::vector<T, Alloc> &rhs)
    {
        return false;
    }

    template <typename T, typename Alloc>
    bool operator<=(const stl::vector<T, Alloc> &lhs,
                    const stl::vector<T, Alloc> &rhs)
    {
        return false;
    }

    template <typename T, typename Alloc>
    bool operator>(const stl::vector<T, Alloc> &lhs,
                   const stl::vector<T, Alloc> &rhs)
    {
        return false;
    }

    template <typename T, typename Alloc>
    bool operator>=(const stl::vector<T, Alloc> &lhs,
                    const stl::vector<T, Alloc> &rhs)
    {
        return false;
    }

    template <typename T, typename Alloc>
    void swap(const stl::vector<T, Alloc> &lhs,
              const stl::vector<T, Alloc> &rhs) noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }

} // namespace stl

#endif
