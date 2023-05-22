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
#include "alloc.hh"
#include "iterator.hh"
#include "uninitialized.hh"

namespace stl
{
    template <typename T, typename Alloc = alloc>
    class vector
    {
        bool operator==(const vector &rhs) const
        {
            auto lstart = begin();
            auto lend = end();
            auto rstart = rhs.begin();
            auto rend = rhs.end();
            while (lstart != lend && rstart != rend)
                if (*lstart != *rstart)
                    return false;
            return lstart == lend && rstart == rend;
        }

        bool operator!=(const vector &rhs) const
        {
            return !(this->operator==(rhs));
        }

        bool operator<(const vector &rhs) const
        {
            auto lstart = begin();
            auto lend = end();
            auto rstart = rhs.begin();
            auto rend = rhs.end();
            while (lstart != lend && rstart != rend)
                if (*lstart >= *rstart)
                    return false;
            return lstart == lend;
        }

        bool operator>(const vector &rhs) const
        {
            auto lstart = begin();
            auto lend = end();
            auto rstart = rhs.begin();
            auto rend = rhs.end();
            while (lstart != lend && rstart != rend)
                if (*lstart <= *rstart)
                    return false;
            return rstart == rend;
        }

        bool operator<=(const vector &rhs) const
        {
            return !(this->operator>(rhs));
        }

        bool operator>=(const vector &rhs) const
        {
            return !(this->operator<(rhs));
        }

        

        void swap(vector &rhs) noexcept;
    public:
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
        using const_reverse_iterator = const stl::reverse_iterator<iterator>;

    protected:
        using data_allocator = simple_alloc<value_type, Alloc>;
        iterator start{};
        iterator finish{};
        iterator end_of_storage{};

        iterator allocate_and_fill(size_type n, const T &value);

        void deallocate();

        void fill_initialize(size_type n, const T &value);

    public:
        /*
         * constructor
         * */
        vector() = default;

        vector(const vector &rhs);

        vector(vector &&rhs) noexcept;

        explicit vector(size_type n)
        {
            fill_initialize(n, T());
        }

        vector(size_t n, const T &elem)
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
        vector &operator=(const vector &rhs);

        vector &operator=(vector &&rhs) noexcept;

        vector &operator=(std::initializer_list<T>);

        vector &assign(size_type n, const T &elem);

        template <typename InputIterator>
        vector &assign(InputIterator first, InputIterator last);

        vector &assign(std::initializer_list<T>);

        allocator_type get_allocator() const noexcept;

        
        /*
         * Capacity
         * */
        bool empty() const
        {
            return begin() == end();
        }

        size_type size() const
        {
            return size_type(finish - start);
        }

        size_type max_size() const // copy from std
        {
            return 0x2000000000000000;
        }

        void reserve(size_type newsize) const;


        size_type capacity() const
        {
            return size_type(end_of_storage - start);
        }


        void shrink_to_fit();

        

        /*
         * Element access
         * */
         reference at(size_type idx)
        {
            if (idx >= size())
                throw new std::out_of_range("");
            return *(begin() + idx);
        }


        const_reference at(size_type idx) const
        {
            if (idx >= size())
                throw new std::out_of_range("");
            return *(begin() + idx);
        }

        reference operator[](size_type idx)
        {
            return *(begin() + idx);
        }

        const_reference operator[](size_type idx) const
        {
            return *(begin() + idx);
        }

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *begin();
        }

        reference back()
        {
            return *(end() - 1);
        }

        const_reference back() const
        {
            return *(end() - 1);
        }


        /*
         * Iterator function
         * */
        iterator begin()
        {
            return start;
        }

        iterator end()
        {
            return finish;
        }

        const_iterator cbegin() const
        {
            return start;
        }

        const_iterator cend() const
        {
            return finish;
        }

        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        /*
         * inserting and removing
         * */
        void push_back(const T &elem);

        void push_back(T &&elem);

        void insert(iterator pos, const T &elem);

        void insert(iterator pos, size_type n, const T &elem);

        template <typename InputIterator>
        void insert(iterator pos, InputIterator first, InputIterator last);

        void insert(iterator pos, std::initializer_list<T>);

        iterator erase(iterator pos);

        iterator erase(iterator first, iterator last);

        void resize(size_type size);

        void resize(size_type size, const T &elem);

        void resize(size_type size, T &&elem);

        void clear();
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
        finish = end_of_storage = start + n;
    }

    /*
     * Definition: constructor
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
    vector<T, Alloc>::vector(vector &&rhs)
    {
        start = rhs.start;
        finish = rhs.finish;
        end_of_storage = rhs.end_of_storage;

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
    vector<T, Alloc>::vector(std::initializer_list<T> lst)
    {
        start = finish = data_allocator::allocate(lst.size());
        for (auto &elem : lst)
            construct(finish++, elem);
        end_of_storage = finish;
    }

    /*
     * assignment operation
     * */
    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T, Alloc> &rhs)
    {
        start = finish = data_allocator::allocate(rhs.size());
        for (auto &elem : rhs)
            construct(finish++, elem);
        end_of_storage = finish;

        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(vector<T, Alloc> &&rhs)
    {
        start = rhs.start;
        finish = rhs.finish;
        end_of_storage = rhs.end_of_storage;

        rhs.start = rhs.finish = rhs.end_of_storage = nullptr;

        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(std::initializer_list<T> lst)
    {
        start = finish = data_allocator::allocate(lst.size());
        for (auto &elem : lst)
            construct(finish++, elem);
        end_of_storage = finish;

        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::assign(size_type n, const T &elem)
    {
        start = finish = data_allocator::allocate(n);
        finish = stl::uninitialized_fill_n(finish, n, elem);
        end_of_storage = finish;

        return *this;
    }

    template <typename T, typename Alloc>
    template <typename InputIterator>
    vector<T, Alloc> &vector<T, Alloc>::assign(InputIterator first, InputIterator last)
    {
        start = finish = data_allocator::allocate(n);
        finish = stl::uninitiazed_copy(first, last, finish);
        end_of_storage = finish;

        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::assign(std::initializer_list<T> lst)
    {
        return assign(lst.begin(), lst.end());
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::swap(vector<T, Alloc> &rhs)
    {
        std::swap(start, rhs.start);
        std::swap(finsih, rhs.finish);
        std::swap(end_of_storage, rhs.end_of_storage);
    }

    /*
     * inserting and removing
     * */
    template <typename T, typename Alloc>
    void vector<T, Alloc>::push_back(const T &elem)
    {
        if (finish != end_of_storage)
            construct(finish++, elem);
        else
            insert_aux(end(), elem);
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::push_back(T &&elem)
    {
        if (finish != end_of_storage)
            construct(finish++, std::move(elem));
        else
            insert_aux(end(), std::move(elem));
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert(iterator pos, const T &elem)
    {
        if (pos == end())
            push_back(elem);
        else
        {
            // re-allocate and copy
            if (finish == end_of_storage)
            {
                const size_type old_size = size();
                const size_type new_size = old_size ? old_size << 1 : 1;
                iterator new_start = data_allocator ::allocate(new_size);
                iterator new_finish;
                try
                {
                    new_finish = uninitiazed_copy(start, pos, new_start);
                    construct(new_finish, elem);
                    ++new_finish;
                    new_finish = uninitiazed_copy(pos, finish, new_finish);
                }
                catch (std::exception e)
                {
                }
            }
        }
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert(iterator pos, size_type n, const T &elem)
    {
        if (n == 0)
            return;

        // remaining space is enough
        if (finish - pos >= n)
        {
            auto num_after_pos = finish - pos;

            if (num_after_pos >= n)
            {
                auto old_finish = finish;
                finish = stl::uninitiazed_copy(finish - n, finish, finish);
                stl::uninitialized_copy(pos, old_finish - n, old_finish - (num_after_pos - n));
                stl::uninitialized_fill_n(pos, n, elem);
            }
            else
            {
                auto old_finish = finish;
                finish = stl::uninitialized_fill_n(finish, n - num_after_pos, elem);
                finish += n - num_after_pos;
                finish = stl::uninitiazed_copy(pos, old_finish, finish);
                stl::uninitialized_fill(pos, old_finish, elem);
            }
            assert(finish == old_finish + n);
        }
        else // re-allocate
        {
            const size_type old_size = std::max(size(), n);
            const size_type new_size = old_size ? old_size << 1 : 1;
            iterator new_start = data_allocator ::allocate(new_size);
            iterator new_finish;
            try
            {
                new_finish = uninitiazed_copy(start, pos, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, elem);
                new_finish = uninitiazed_copy(pos, finish, new_finish);

                // release old space
                stl::destroy(start, finish);
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = start + new_size;
            }
            catch (std::exception e)
            {
            }
        }
    }

    template <typename T, typename Alloc>
    template <typename InputIterator>
    void vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last)
    {
        auto size = stl::distance(first, last);
        /* reallocate storage */
        if (capacity() < size)
        {
            iterator new_start, new_finish;
            auto new_size = 2 * size() + size;
            new_start = new_finish = data_allocator::allocate(new_size);
            new_finish = stl::uninitialized_copy(start, pos, new_finish);
            new_finish = stl::uninitialized_copy(first, last, new_finish);
            new_finish = stl::uninitialized_copy(pos, finish, new_finish);

            // free old-storage
            stl::destroy(start, finish);
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = start + new_size;
        }
        else
        {
            auto old_finish = finish;
            finish += size;
            std::copy_backward(pos, old_finish, finish);
            stl::uninitiazed_copy(first, last, pos);
        }
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert(iterator pos, std::initializer_list<T> lst)
    {
        insert(pos, lst.begin(), lst.end());
    }

    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos)
    {
        if (pos + 1 != end())
            std::copy(pos + 1, end(), pos);
        --finish;
        destroy(finish);
        return pos;
    }

    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
    {
        iterator new_finish = std::copy(last, finish, first);
        destroy(new_finish, finish);
        finish = new_finish;
        return last;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type size)
    {
        resize(size, T());
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type size, const T &elem)
    {
        if (size < finish)
        {
            while (finish != start + size)
                destroy(--finish);
        }
        else if (size > finish)
        {
            while (finish != start + size)
                construct(finish++, elem);
        }
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type size, T &&elem)
    {
        if (size < finish)
        {
            while (finish != start + size)
                destroy(--finish);
        }
        else if (size > finish)
        {
            while (finish != start + size)
            {
                T temp = elem;
                construct(finish++, std::move(elem));
            }
        }
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::clear()
    {
        erase(begin(), end());
    }

} // namespace stl

#endif