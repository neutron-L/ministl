//
// Created by rda on 2023/5/14.
//

#ifndef MINISTL_THS_ths_vector_HH
#define MINISTL_THS_ths_vector_HH

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>

#include "algobase.hh"
#include "alloc.hh"
#include "iterator.hh"
#include "uninitialized.hh"

namespace stl {
/*
 * thread-safe version ths_vector
 * 使用RCU机制实现线程安全版本的ths_vector
 * 考虑到并发的性能，仅支持只读迭代器
 * */
template <typename T, typename Alloc = alloc> 
class ths_vector {
  public:
    using vector = std::vector<T>;

    /* Member types */
    using value_type      = vector::value_type;
    using allocator_type  = Alloc;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    using reference       = vector::reference;
    using const_reference = vector::const_reference;
    using pointer         = vector::pointer;
    using const_pointer   = vector::const_pointer;

    using iterator               = vector::iterator;
    using const_iterator         = vector::const_iterator;
    using reverse_iterator       = stl::reverse_iterator<iterator>;
    using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

  protected:
    using data_allocator = simple_alloc<value_type, Alloc>;

    // iterator start{};
    // iterator finish{};
    // iterator end_of_storage{};

    // iterator allocate_and_fill(size_type n, const T &value);

    // void deallocate();

    // void fill_initialize(size_type n, const T &value);

    // void reallocate(size_type n);

    // iterator insert_aux(const_iterator pos, T &&elem);
    std::shared_ptr<vector> p_vec{};
    std::mutex                      mtx{};

  public:
    /*
     * constructor
     * */
    ths_vector() : p_vec(std::make_shared<vector>()) {}

    ths_vector(const ths_vector& other);

    ths_vector(ths_vector&& other) noexcept;

    explicit ths_vector(size_type n)
        : p_vec(std::make_shared<vector>(n))
    {
    }

    ths_vector(size_type n, const T& elem)
        : p_vec(std::make_shared<vector>(n, elem))
    {
    }

    template <
        typename InputIterator,
        typename = std::_RequireInputIter<InputIterator>>
    ths_vector(InputIterator first, InputIterator last)
        : p_vec(std::make_shared<vector>(first, last))
    {
    }

    ths_vector(std::initializer_list<T>);

    /*
     *  destructor
     * */
    ~ths_vector()
    {
    }

    /*
     * assignment operation
     * */
    ths_vector& operator=(const ths_vector& other);

    ths_vector& operator=(ths_vector&& other) noexcept;

    ths_vector& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T& elem);

    template <typename InputIt> void assign(InputIt first, InputIt last);

    void assign(std::initializer_list<T>);

    allocator_type get_allocator() const noexcept;

    /*
     * Element access
     * */
    reference at(size_type pos)
    {
        return const_cast<reference>(
            static_cast<const ths_vector&>(*this).at(pos));
    }

    const_reference at(size_type pos) const
    {
        if (pos >= size()) {
            error("%ld is larger than size %ld", pos, size());
            throw new std::out_of_range("");
        }
        return *(begin() + pos);
    }

    reference operator[](size_type pos)
    {
        return const_cast<reference>(const_cast<const ths_vector&>(*this)[pos]);
    }

    const_reference operator[](size_type pos) const
    {
        return *(begin() + pos);
    }

    reference front()
    {
        return const_cast<reference>(
            const_cast<const ths_vector&>(*this).front());
    }

    const_reference front() const
    {
        return *begin();
    }

    reference back()
    {
        return const_cast<reference>(
            const_cast<const ths_vector&>(*this).back());
    }

    const_reference back() const
    {
        return *(end() - 1);
    }

    T* data() noexcept
    {
        return const_cast<T*>(const_cast<const ths_vector&>(*this).data());
    }
    const T* data() const noexcept
    {
        return start;
    }

    /*
     * Iterator function
     * */
    iterator begin() noexcept
    {
        return const_cast<iterator>(
            const_cast<const ths_vector&>(*this).begin());
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
        return const_cast<iterator>(const_cast<const ths_vector&>(*this).end());
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
        return const_cast<reverse_iterator>(
            const_cast<const ths_vector&>(*this).rbegin());
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
        return const_cast<reverse_iterator>(
            const_cast<const ths_vector&>(*this).rend());
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

    size_type max_size() const noexcept  // copy from cppreference
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
    iterator insert(const_iterator pos, T&& elem);
    iterator insert(const_iterator pos, const T& elem);
    iterator insert(const_iterator pos, size_type count, const T& elem);
    template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
    iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
        iterator ipos  = const_cast<iterator>(pos);
        auto     count = stl::distance(first, last);

        if (count) {
            if (stl::distance(finish, end_of_storage) >= count) {
                const size_type elems_after = stl::distance(ipos, end());

                if (elems_after > count) {
                    stl::uninitialized_copy(finish - count, finish, finish);
                    stl::copy_backward(ipos, finish - count, finish);

                    auto iter = ipos;
                    while (first != last)
                        *iter++ = *first++;

                    finish += count;
                }
                else {
                    iterator old_finish = finish;
                    finish += count - elems_after;
                    stl::uninitialized_copy(ipos, old_finish, finish);
                    finish += elems_after;

                    auto iter = ipos;
                    while (iter != old_finish)
                        *iter++ = *first++;

                    while (first != last)
                        stl::construct(&*iter++, *first++);
                }
            }

            else {
                const size_type old_size = size();
                const size_type len =
                    old_size + stl::max(old_size, (size_type)count);

                // allocate new storage
                iterator new_start  = data_allocator::allocate(len);
                iterator new_finish = new_start;

                try {
                    /* code */
                    new_finish =
                        stl::uninitialized_copy(start, ipos, new_start);
                    auto old_ipos = ipos;        // save old ipos
                    ipos          = new_finish;  // update ipos
                    while (first != last)
                        stl::construct(&*new_finish++, *first++);
                    new_finish =
                        stl::uninitialized_copy(old_ipos, finish, new_finish);
                }
                catch (const std::exception& e) {
                    stl::destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, len);
                    throw;
                }
                // release old storage
                stl::destroy(begin(), end());
                deallocate();

                start          = new_start;
                finish         = new_finish;
                end_of_storage = new_start + len;
            }
        }

        return ipos;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    iterator erase(const_iterator pos);

    iterator erase(const_iterator first, const_iterator last);

    void push_back(const T& elem);
    void push_back(T&& elem);

    template <typename... Args> reference emplace_back(Args&&... args);

    void pop_back();

    void resize(size_type size);
    void resize(size_type size, const value_type& elem);

    void swap(ths_vector& other) noexcept;
};



/*
 * Constructors
 * */
template <typename T, typename Alloc>
ths_vector<T, Alloc>::ths_vector(const ths_vector& rhs)
:p_vec(rhs.p_vec)
{
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>::ths_vector(ths_vector&& rhs) noexcept
:p_vec(std::move(rhs.p_vec))
{
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>::ths_vector(std::initializer_list<T> lst)
    : p_vec(std::make_shared<vector>(lst))
{
}

/*
 * Assignment operation
 * */
template <typename T, typename Alloc>
ths_vector<T, Alloc>&
ths_vector<T, Alloc>::operator=(const ths_vector<T, Alloc>& rhs)
{
    if (this != &rhs)
    {
        // 按地址大小顺序获取锁，防止死锁
        if (&mtx < rhs.mtx)
        {
            std::lock_guard<std::mutex> guard1(mtx);
            std::lock_guard<std::mutex> guard1(rhs.mtx);
            p_vec = rhs.p_vec;
        }
        else
        {
            std::lock_guard<std::mutex> guard1(rhs.mtx);
            std::lock_guard<std::mutex> guard1(mtx);
            p_vec = rhs.p_vec;
        }
    }
    return *this;
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>&
ths_vector<T, Alloc>::operator=(ths_vector<T, Alloc>&& rhs) noexcept
{
    if (this != &rhs)
    {
        // 按地址大小顺序获取锁，防止死锁
        if (&mtx < rhs.mtx)
        {
            std::lock_guard<std::mutex> guard1(mtx);
            std::lock_guard<std::mutex> guard2(rhs.mtx);
            p_vec = std::move(rhs.p_vec); // 在这之后rh.p_vec会变为nullptr
        }
        else
        {
            std::lock_guard<std::mutex> guard1(rhs.mtx);
            std::lock_guard<std::mutex> guard2(mtx);
            p_vec = std::move(rhs.p_vec);
        }
    }
    return *this;
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>&
ths_vector<T, Alloc>::operator=(std::initializer_list<T> lst)
{
    assign(lst.begin(), lst.end());

    return *this;
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::assign(size_type n, const T& elem)
{
    destroy(begin(), end());

    if (capacity() < n) {
        start          = data_allocator::allocate(n);
        end_of_storage = start + n;
    }
    finish = start;
    finish = stl::uninitialized_fill_n(finish, n, elem);
}

template <typename T, typename Alloc>
template <typename InputIt>
void ths_vector<T, Alloc>::assign(InputIt first, InputIt last)
{
    stl::destroy(begin(), end());
    size_type n = stl::distance(first, last);

    if (capacity() < n) {
        start          = data_allocator::allocate(n);
        end_of_storage = start + n;
    }
    finish = start;
    finish = stl::uninitialized_copy(first, last, finish);
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::assign(std::initializer_list<T> lst)
{
    assign(lst.begin(), lst.end());
}

/*
 * Capacity
 * */
template <typename T, typename Alloc>
void ths_vector<T, Alloc>::reserve(size_type new_cap)
{
    if (capacity() >= new_cap)
        return;
    reallocate(new_cap);
}

template <typename T, typename Alloc> void ths_vector<T, Alloc>::shrink_to_fit()
{
    if (finish != end_of_storage)
        reallocate(size());
}

/*
 * Modifiers
 * */
template <typename T, typename Alloc>
void ths_vector<T, Alloc>::clear() noexcept
{
    erase(begin(), end());
}

// insert
template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, const T& elem)
{
    T elem_copy = elem;
    return insert_aux(pos, std::move(elem_copy));
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, T&& elem)
{
    return insert_aux(pos, std::move(elem));
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, size_type count, const T& elem)
{
    iterator ipos = const_cast<iterator>(pos);

    if (count) {
        if (finish && stl::distance(finish, end_of_storage) >= count) {
            const size_type elems_after = stl::distance(ipos, end());

            if (elems_after > count) {
                stl::uninitialized_copy(finish - count, finish, finish);
                stl::copy_backward(ipos, finish - count, finish);
                stl::fill_n(ipos, count, elem);
                finish += count;
            }
            else {
                iterator old_finish = finish;
                finish += count;
                stl::uninitialized_copy(ipos, old_finish, finish);
                finish += elems_after;
                stl::fill(ipos, old_finish, elem);
                stl::uninitialized_fill(old_finish, finish - elems_after, elem);
            }
        }

        else {
            const size_type old_size = size();
            const size_type len      = old_size + max(old_size, count);

            // allocate new storage
            iterator new_start  = data_allocator::allocate(len);
            iterator new_finish = new_start;

            try {
                /* code */
                if (finish)
                    new_finish =
                        stl::uninitialized_copy(start, ipos, new_start);
                auto old_ipos = ipos;
                ipos          = new_finish;
                new_finish = stl::uninitialized_fill_n(new_finish, count, elem);
                if (finish)
                    new_finish =
                        stl::uninitialized_copy(old_ipos, finish, new_finish);
            }
            catch (const std::exception& e) {
                stl::destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            // release old storage
            stl::destroy(begin(), end());
            deallocate();

            start          = new_start;
            finish         = new_finish;
            end_of_storage = new_start + len;
        }
    }

    return ipos;
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
    return insert(pos, ilist.begin(), ilist.end());
}

template <typename T, typename Alloc>
template <typename... Args>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::emplace(const_iterator pos, Args&&... args)
{
    T elem(std::forward<Args>(args)...);
    return insert_aux(pos, std::move(elem));
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::erase(const_iterator pos)
{
    iterator ipos = const_cast<iterator>(pos);

    stl::copy(ipos + 1, finish, ipos);
    --finish;
    stl::destroy(finish);

    return ipos;
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    iterator f = const_cast<iterator>(first);
    iterator l = const_cast<iterator>(last);
    auto     i = stl::copy(l, finish, f);
    stl::destroy(i, finish);
    finish = finish - stl::distance(first, last);

    return f;
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::push_back(const T& elem)
{
    if (finish != end_of_storage) {
        stl::construct(finish, elem);
        ++finish;
    }
    else
        (void)insert_aux(end(), elem);
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::push_back(T&& elem)
{
    if (finish != end_of_storage) {
        stl::construct(finish, std::move(elem));
        ++finish;
    }
    else
        (void)insert_aux(end(), std::move(elem));
}

template <typename T, typename Alloc>
template <typename... Args>
typename ths_vector<T, Alloc>::reference
ths_vector<T, Alloc>::emplace_back(Args&&... args)
{
    if (finish != end_of_storage) {
        stl::construct(finish, std::forward<Args>(args)...);
        ++finish;
    }
    else {
        T elem(std::forward<Args>(args)...);
        (void)insert_aux(end(), std::move(elem));
    }
    return *end();
}

template <typename T, typename Alloc> void ths_vector<T, Alloc>::pop_back()
{
    --finish;
    stl::destroy(finish);
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::resize(size_type size)
{
    resize(size, T());
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::resize(size_type size, const value_type& elem)
{
    const size_type s = this->size();

    if (size > s) {
        // whether or not need to allocate more storage space
        if (size > capacity()) {
            auto new_start  = data_allocator::allocate(size);
            auto new_finish = new_start;

            try {
                if (start)
                    new_finish =
                        stl::uninitialized_copy(start, finish, new_start);
            }
            catch (const std::exception& e) {
                stl::destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, size);

                throw;
            }
            start          = new_start;
            finish         = new_finish;
            end_of_storage = start + size;
        }

        stl::uninitialized_fill(finish, start + size, elem);
        finish = start + size;
    }
    else if (size < s) {
        auto old_finish = finish;
        finish          = start + size;
        stl::destroy(finish, old_finish);
    }
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::swap(ths_vector& other) noexcept
{
    stl::swap(start, other.start);
    stl::swap(finish, other.finish);
    stl::swap(end_of_storage, other.end_of_storage);
}

/* Non-member functions */
template <typename T, typename Alloc>
bool operator==(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs)
{
    return (
        lhs.size() == rhs.size() &&
        stl::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename T, typename Alloc>
bool operator!=(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
bool operator<(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs)
{
    return stl::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
bool operator<=(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs)
{
    return !(lhs > rhs);
}

template <typename T, typename Alloc>
bool operator>(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs)
{
    return rhs < lhs;
}

template <typename T, typename Alloc>
bool operator>=(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename T, typename Alloc>
void swap(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

}  // namespace stl

#endif
