//
// Created by 20550 on 2023/4/6.
//

#ifndef MINISTL_ARRAY_H
#define MINISTL_ARRAY_H

#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <cstring>
#include "iterator.hh"
#include "uninitialized.hh"

namespace stl
{
    template <typename T, std::size_t N>
    class array
    {
    private:
        T arr[N];

    public:
        /*
         * Type
         * */
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

        using iterator = value_type *;
        using const_iterator = const value_type *;
        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = const stl::reverse_iterator<iterator>;

        /*
         * Constructors
         * */
        array() = default;
        array(const array &);
        array(array &&) noexcept;
        array(std::initializer_list<T>);

        /*
         * Element access
         * */
        reference at(size_type pos)
        {
            return const_cast<reference>(static_cast<const array &>(*this).at(pos));
        }

        const_reference at(size_type pos) const
        {
            if (pos >= N)
                throw std::out_of_range("out of range");
            return arr[pos];
        }

        reference operator[](size_type pos)
        {
            return const_cast<reference>(static_cast<const array &>(*this)[pos]);
        }

        const_reference operator[](const size_t pos) const
        {
            return arr[pos];
        }

        reference front()
        {
            return const_cast<reference>(static_cast<const array &>(*this).front());
        }

        const_reference front() const
        {
            return arr[0];
        }

        reference back()
        {
            return const_cast<reference>(static_cast<const array &>(*this).back());
        }

        const_reference back() const
        {
            return arr[N - 1];
        }

        T *data() noexcept
        {
            return const_cast<T *>(static_cast<const array &>(*this).data());
        }

        const T *data() const noexcept
        {
            return data;
        }

        /*
         * Iterators
         * */
        iterator begin() noexcept
        {
            return const_cast<iterator>(static_cast<const array &>(*this).begin());
        }

        const_iterator begin() const noexcept
        {
            return &arr[0];
        }

        const_iterator cbegin() const noexcept
        {
            return &arr[0];
        }

        iterator end() noexcept
        {
            return const_cast<iterator>(static_cast<const array &>(*this).end());
        }

        const_iterator end() const noexcept
        {
            return &arr[N];
        }

        const_iterator cend() const noexcept
        {
            return &arr[N];
        }

        reverse_iterator rbegin() noexcept
        {
            return const_cast<reverse_iterator>(static_cast<const array &>(*this).rbegin());
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return reverse_iterator(end());
        }

        reverse_iterator rend() noexcept
        {
            return const_cast<reverse_iterator>(static_cast<const array &>(*this).rend());
        }

        const_reverse_iterator rend() const noexcept
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator crend() const
        {
            return reverse_iterator(begin());
        }

        /*
         * Capacity
         */
        bool empty() const noexcept { return size() == 0; }
        constexpr size_type size() const noexcept { return N; }
        constexpr size_type max_size() const noexcept { return N; }

        /*
         *   Operations
         */
        void fill(const T &value)
        {
            stl::uninitialized_fill_n(data, N, value);
        }
        void swap(array &other) noexcept
        {
            stl::swap_ranges(begin(), end(), other.begin());
        }
    };

    /* Constructors */
    template <typename T, size_t N>
    array<T, N>::array(const array &rhs)
    {
        for (int i = 0; i < N; ++i)
            new (std::__addressof(arr[i])) T(rhs[i]);
    }

    template <typename T, size_t N>
    array<T, N>::array(array &&rhs) noexcept
    {
        for (int i = 0; i < N; ++i)
            new (std::__addressof(arr[i])) T(std::move(rhs[i]));
    }

    template <typename T, size_t N>
    array<T, N>::array(std::initializer_list<T> lst)
    {
        size_t pos = 0;
        for (auto &i : lst)
        {
            if (pos == N)
                break;
            new (std::__addressof(arr[pos++])) T(i);
        }

        while (pos < N)
            new (std::__addressof(arr[pos++])) T();
    }

    /* Non-member functions */
    template <typename T, std::size_t N>
    bool operator==(const stl::array<T, N> &lhs, 
                    const stl::array<T, N> &rhs)
    {
    }

    template <typename T, std::size_t N>
    bool operator!=(const stl::array<T, N> &lhs, 
                    const stl::array<T, N> &rhs)
    {
    }

    template <typename T, std::size_t N>
    bool operator<(const stl::array<T, N> &lhs, 
                    const stl::array<T, N> &rhs)
    {
    }

    template <typename T, std::size_t N>
    bool operator<=(const stl::array<T, N> &lhs, 
                    const stl::array<T, N> &rhs)
    {
    }

    template <typename T, std::size_t N>
    bool operator>(const stl::array<T, N> &lhs, 
                    const stl::array<T, N> &rhs)
    {
    }

    template <typename T, std::size_t N>
    bool operator>=(const stl::array<T, N> &lhs, 
                    const stl::array<T, N> &rhs)
    {
    }
} // namespace stl

#endif
