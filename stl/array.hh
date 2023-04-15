//
// Created by 20550 on 2023/4/6.
//

#ifndef UNTITLED_ARRAY_H
#define UNTITLED_ARRAY_H

#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <cstring>
#include "iterator.hh"

namespace stl
{
    template <typename T, size_t N>
    class array
    {
        friend void swap(array<T, N> & lhs, array<T, N> & rhs)
        {
            T temp[N];
            memmove(temp, lhs.arr, sizeof(lhs.arr));
            memmove(lhs.arr, rhs.arr, sizeof(lhs.arr));
            memmove(rhs.arr, temp, sizeof(lhs.arr));

            // for (size_t i = 0; i < N; ++i)
                // std::swap(lhs[i], rhs[i]);
        }
    private:
        T arr[N];
    public:
        /*
         * Type
         * */
        using value_type = T;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = value_type *;
        using const_iterator = const value_type *;
        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = const stl::reverse_iterator<iterator>;


        /*
         * Class array<> constructors
         * */
        array()=default;
        array(const array &);
        array(array &&) noexcept;
        array(std::initializer_list<T>);

        /*
         * Non-modifying Operation
         * */
        bool empty() { return size() == 0;}
        size_t size() const { return N; }
        size_t max_size() const { return N; }

        bool operator==(const array & rhs) const
        {
            for (int i = 0; i < N; ++i)
                if (arr[i] != rhs.arr[i])
                    return false;
            return true;
        }

        bool operator!=(const array & rhs) const
        {
            return !(this->operator==(rhs));
        }

        bool operator<=(const array & rhs)
        {
            for (int i = 0; i < N; ++i)
                if (arr[i] > rhs.arr[i])
                    return false;
            return true;
        }

        bool operator>=(const array & rhs)
        {
            for (int i = 0; i < N; ++i)
            {
                if (arr[i] < rhs.arr[i])
                    return false;
                else if (arr[i] > rhs.arr[i])
                    return true;
            }
                
            return true;
        }

        bool operator>(const array & rhs)
        {
            return !(this->operator<=(rhs));
        }

        bool operator<(const array & rhs)
        {
            return !(this->operator>=(rhs));
        }

        /*
         * Assignment Operation
         * */
        array & operator=(const array & rhs)
        {
            if (this != &rhs)
            {
                for (int i = 0; i < N; ++i)
                    arr[i] = rhs.arr[i];
            }

            return *this;
        }

        array & operator=(array && rhs) noexcept{
            if (this != &rhs)
            {
                for (int i = 0; i < N; ++i)
                    arr[i] = std::move(rhs.arr[i]);
            }

            return *this;
        }

        void swap(array & rhs)
        {
            using std::swap;
            swap(*this, rhs);
        }


        /*
         * Class array<> Element access
         * */
        value_type & operator[](const size_t idx)
        {
            return arr[idx];
        }

        const value_type & operator[](const size_t idx) const
        {
            return arr[idx];
        }

        value_type & at(const size_t idx)
        {
            if (idx >= N)
                throw std::out_of_range("out of range");
            return arr[idx];
        }

        value_type & front()
        {
            return arr[0];
        }

        value_type & back()
        {
            return arr[N - 1];
        }

        /*
         * Iterator function
         * */
        iterator begin()
        {
            return &arr[0];
        }

        iterator end()
        {
            return &arr[N];
        }

        const_iterator cbegin() const
        {
            return &arr[0];
        }

        const_iterator cend() const
        {
            return &arr[N];
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(begin());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator crend() const
        {
            return reverse_iterator(end());
        }
    };

    template<typename T, size_t N>
    array<T, N>::array(std::initializer_list<T> lst)
    {
        size_t idx = 0;
        for (auto &i: lst)
        {
            if (idx == N)
                break;
            new(std::__addressof(arr[idx++])) T(i);
        }

        while (idx < N)
            new(std::__addressof(arr[idx++])) T();
    }

    template<typename T, size_t N>
    array<T, N>::array(array && rhs) noexcept
    { 
        for (int i = 0; i < N; ++i)
            new(std::__addressof(arr[i])) T(std::move(rhs[i]));
    }

    template<typename T, size_t N>
    array<T, N>::array(const array & rhs)
    {
        for (int i = 0; i < N; ++i)
            new(std::__addressof(arr[i])) T(rhs[i]);
    }

    
} // namespace stl

#endif