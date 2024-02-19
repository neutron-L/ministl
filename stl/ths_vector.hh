//
// Created by rda on 2024/2/19.
//

#ifndef MINISTL_THS_VECTOR_HH
#define MINISTL_THS_VECTOR_HH

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>
#include <vector>

#include "algobase.hh"
#include "alloc.hh"
#include "iterator.hh"
#include "uninitialized.hh"

namespace stl {
/*
 * thread-safe version ths_vector
 * 使用RCU机制实现线程安全版本的ths_vector，但是不完全支持vector的所有功能
 * 1. 考虑到并发的性能，仅支持只读迭代器
 * 2. 不返回non-const饮用和non-const指针，防止外泄内部数据的引用和指针
 *     使得其他使用者绕过锁修改内部数据
 * */

template <typename Vector> class ths_vector_iterator {
  public:
    using iterator = typename Vector::const_iterator;
    using self     = ths_vector_iterator;

    using value_type        = typename Vector::value_type;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    using reference = typename Vector::const_reference;
    using pointer   = typename Vector::const_pointer;

  private:
    std::shared_ptr<const Vector> ptr{};
    iterator                      iter{};
    bool                          is_end{};

  public:
    ths_vector_iterator() = default;

    ths_vector_iterator(const std::shared_ptr<Vector>& p, int index) : ptr(p)
    {
        iter = ptr->begin();
        std::advance(iter, index);
        if (index == p->size())
            is_end = true;
    }

    ~ths_vector_iterator() = default;

    self& operator++()
    {
        ++iter;
        if (iter == ptr->end())
            is_end = true;
        return *this;
    }

    self operator++(int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    self& operator--()
    {
        --iter;
        if (iter != ptr->end())
            is_end = false;
        return *this;
    }

    self operator--(int)
    {
        auto temp = *this;
        --*this;
        return temp;
    }

    reference operator*() const
    {
        return *iter;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    bool operator==(const self& rhs) const
    {
        if (is_end != rhs.is_end)
            return false;
        if (is_end)
            return true;
        return ptr == rhs.ptr && iter == rhs.iter;
    }

    bool operator!=(const self& rhs) const
    {
        return !(*this == rhs);
    }
};

template <typename T, typename Alloc = alloc> class ths_vector {
  public:
    using Vector = std::vector<T>;

    /* Member types */
    using value_type      = typename Vector::value_type;
    using allocator_type  = Alloc;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    using reference       = typename Vector::const_reference;
    using const_reference = typename Vector::const_reference;
    using pointer         = typename Vector::pointer;
    using const_pointer   = typename Vector::const_pointer;

    using iterator               = ths_vector_iterator<Vector>;
    using const_iterator         = ths_vector_iterator<Vector>;
    using reverse_iterator       = stl::reverse_iterator<iterator>;
    using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

  protected:
    using data_allocator = simple_alloc<value_type, Alloc>;

    // 两个数据成员
    // 1. 指向non-thread-safe的vector的共享指针
    // 2. 互斥锁
    std::shared_ptr<Vector> p_vec{};
    mutable std::mutex              mtx{};

  public:
    /*
     * constructor
     * */
    ths_vector() : p_vec(std::make_shared<Vector>()) {}

    ths_vector(const ths_vector& other);

    ths_vector(ths_vector&& other) noexcept;

    explicit ths_vector(size_type n) : p_vec(std::make_shared<Vector>(n)) {}

    ths_vector(size_type n, const T& elem)
        : p_vec(std::make_shared<Vector>(n, elem))
    {
    }

    template <
        typename InputIterator,
        typename = std::_RequireInputIter<InputIterator>>
    ths_vector(InputIterator first, InputIterator last)
        : p_vec(std::make_shared<Vector>(first, last))
    {
    }

    ths_vector(std::initializer_list<T>);

    /*
     *  destructor
     * */
    ~ths_vector() {}

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
     * Read-only
     * */
    const_reference at(size_type pos) const
    {
        std::lock_guard<std::mutex> guard(mtx);

        if (pos >= size()) {
            error("%ld is larger than size %ld", pos, size());
            throw new std::out_of_range("");
        }
        return *(p_vec->begin() + pos);
    }

    const_reference operator[](size_type pos) const
    {
        std::lock_guard<std::mutex> guard(mtx);
        return *(p_vec->begin() + pos);
    }

    const_reference front() const
    {
        std::lock_guard<std::mutex> guard(mtx);

        return *p_vec->begin();
    }

    const_reference back() const
    {
        std::lock_guard<std::mutex> guard(mtx);
        return *(p_vec->end() - 1);
    }

    const_pointer data() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return p_vec->data();
    }

    /*
     * Iterator function
     * */
    const_iterator begin() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return iterator(p_vec, 0);
    }

    const_iterator cbegin() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return iterator(p_vec, 0);
    }

    const_iterator end() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return iterator(p_vec, p_vec->size());
    }

    const_iterator cend() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return iterator(p_vec, p_vec->size());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return reverse_iterator(iterator(p_vec, 0));
    }

    const_reverse_iterator crbegin() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return const_reverse_iterator(iterator(p_vec, 0));
    }

    const_reverse_iterator rend() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return reverse_iterator(iterator(p_vec, p_vec->size()));
    }

    const_reverse_iterator crend() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);
        return const_reverse_iterator(iterator(p_vec, p_vec->size()));
    }

    /*
     * Capacity
     * */
    bool empty() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);

        return p_vec->empty();
    }

    size_type size() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);

        return p_vec->size();
    }

    size_type max_size() const noexcept  // copy from cppreference
    {
        return std::numeric_limits<difference_type>::max();
    }

    void reserve(size_type new_cap);

    size_type capacity() const noexcept
    {
        std::lock_guard<std::mutex> guard(mtx);

        return p_vec->capacity();
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
        std::lock_guard<std::mutex> guard(mtx);

        if (p_vec.use_count() > 1) {
            p_vec.reset(new Vector(*p_vec));
        }
        assert(p_vec.use_count() == 1);
        // 在新的副本上修改
        return p_vec->insert(pos, first, last);
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
ths_vector<T, Alloc>::ths_vector(const ths_vector& rhs) : p_vec(rhs.p_vec)
{
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>::ths_vector(ths_vector&& rhs) noexcept
    : p_vec(std::move(rhs.p_vec))
{
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>::ths_vector(std::initializer_list<T> lst)
    : p_vec(std::make_shared<Vector>(lst))
{
}

/*
 * Assignment operation
 * */
template <typename T, typename Alloc>
ths_vector<T, Alloc>&
ths_vector<T, Alloc>::operator=(const ths_vector<T, Alloc>& rhs)
{
    if (this != &rhs) {
        // 按地址大小顺序获取锁，防止死锁
        if (&mtx < &rhs.mtx) {
            std::lock_guard<std::mutex> guard1(mtx);
            std::lock_guard<std::mutex> guard2(rhs.mtx);

            // RCU，如果当前vector存在其他读者，则复制一份再修改
            if (p_vec.use_count() > 1) {
                p_vec.reset(new Vector(*p_vec));
            }
            assert(p_vec.use_count() == 1);
            p_vec = rhs.p_vec;
        }
        else {
            std::lock_guard<std::mutex> guard1(rhs.mtx);
            std::lock_guard<std::mutex> guard2(mtx);
            // RCU，如果当前vector存在其他读者，则复制一份再修改
            if (p_vec.use_count() > 1) {
                p_vec.reset(new Vector(*p_vec));
            }
            assert(p_vec.use_count() == 1);

            p_vec = rhs.p_vec;
        }
    }
    return *this;
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>&
ths_vector<T, Alloc>::operator=(ths_vector<T, Alloc>&& rhs) noexcept
{
    if (this != &rhs) {
        // 按地址大小顺序获取锁，防止死锁
        if (&mtx < &rhs.mtx) {
            std::lock_guard<std::mutex> guard1(mtx);
            std::lock_guard<std::mutex> guard2(rhs.mtx);

            // RCU，如果当前vector存在其他读者，则复制一份再修改
            if (p_vec.use_count() > 1) {
                p_vec.reset(new Vector(*p_vec));
            }
            assert(p_vec.use_count() == 1);

            p_vec = std::move(rhs.p_vec);  // 在这之后rh.p_vec会变为nullptr
        }
        else {
            std::lock_guard<std::mutex> guard1(rhs.mtx);
            std::lock_guard<std::mutex> guard2(mtx);

            // RCU，如果当前vector存在其他读者，则复制一份再修改
            if (p_vec.use_count() > 1) {
                p_vec.reset(new Vector(*p_vec));
            }
            assert(p_vec.use_count() == 1);

            p_vec = std::move(rhs.p_vec);
        }
    }
    return *this;
}

template <typename T, typename Alloc>
ths_vector<T, Alloc>&
ths_vector<T, Alloc>::operator=(std::initializer_list<T> lst)
{
    std::lock_guard<std::mutex> guard(mtx);
    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    *p_vec = lst;

    return *this;
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::assign(size_type n, const T& elem)
{
    std::lock_guard<std::mutex> guard(mtx);
    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    p_vec->assign(n, elem);
}

template <typename T, typename Alloc>
template <typename InputIt>
void ths_vector<T, Alloc>::assign(InputIt first, InputIt last)
{
    std::lock_guard<std::mutex> guard(mtx);
    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    p_vec->assign(first, last);
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::assign(std::initializer_list<T> lst)
{
    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    p_vec->assign(lst);
}

/*
 * Capacity
 * */
template <typename T, typename Alloc>
void ths_vector<T, Alloc>::reserve(size_type new_cap)
{
    std::lock_guard<std::mutex> guard(mtx);
    if (capacity() >= new_cap)
        return;

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改容量
    p_vec->reserve(new_cap);
}

template <typename T, typename Alloc> void ths_vector<T, Alloc>::shrink_to_fit()
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    p_vec->shrink_to_fit();
}

/*
 * Modifiers
 * */
template <typename T, typename Alloc>
void ths_vector<T, Alloc>::clear() noexcept
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    p_vec->clear();
}

// insert
template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, const T& elem)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->insert(pos, elem);
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, T&& elem)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->insert(pos, std::move(elem));
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, size_type count, const T& elem)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->insert(pos, count, elem);
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->insert(pos, ilist);
}

template <typename T, typename Alloc>
template <typename... Args>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::emplace(const_iterator pos, Args&&... args)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->emplace(pos, std::forward<Args>(args)...);
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::erase(const_iterator pos)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->erase(pos);
}

template <typename T, typename Alloc>
typename ths_vector<T, Alloc>::iterator
ths_vector<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->erase(first, last);
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::push_back(const T& elem)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    p_vec->push_back(elem);
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::push_back(T&& elem)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    p_vec->push_back(std::move(elem));
}

template <typename T, typename Alloc>
template <typename... Args>
typename ths_vector<T, Alloc>::reference
ths_vector<T, Alloc>::emplace_back(Args&&... args)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    return p_vec->emplace_back(std::forward<Args>(args)...);
}

template <typename T, typename Alloc> void ths_vector<T, Alloc>::pop_back()
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    p_vec->pop_back();
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::resize(size_type size)
{
    resize(size, T());
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::resize(size_type size, const value_type& elem)
{
    std::lock_guard<std::mutex> guard(mtx);

    if (p_vec.use_count() > 1) {
        p_vec.reset(new Vector(*p_vec));
    }
    assert(p_vec.use_count() == 1);
    // 在新的副本上修改
    p_vec->resize(size, elem);
}

template <typename T, typename Alloc>
void ths_vector<T, Alloc>::swap(ths_vector& other) noexcept
{
    if (this == &other)
        return;

    // 按地址大小顺序获取锁，防止死锁
    if (&mtx < &other.mtx) {
        std::lock_guard<std::mutex> guard1(mtx);
        std::lock_guard<std::mutex> guard2(other.mtx);

        // RCU，如果当前vector存在其他读者，则复制一份再修改
        if (p_vec.use_count() > 1) {
            p_vec.reset(new Vector(*p_vec));
        }
        assert(p_vec.use_count() == 1);

        p_vec->swap(*other.p_vec);
    }
    else {
        std::lock_guard<std::mutex> guard1(other.mtx);
        std::lock_guard<std::mutex> guard2(mtx);

        // RCU，如果当前vector存在其他读者，则复制一份再修改
        if (p_vec.use_count() > 1) {
            p_vec.reset(new Vector(*p_vec));
        }
        assert(p_vec.use_count() == 1);

        p_vec->swap(*other.p_vec);
    }
}

/* Non-member functions */
template <typename T, typename Alloc>
bool operator==(
    const stl::ths_vector<T, Alloc>& lhs,
    const stl::ths_vector<T, Alloc>& rhs)
{
    // 按地址大小顺序获取锁，防止死锁
    if (&lhs.mtx < &rhs.mtx) {
        std::lock_guard<std::mutex> guard1(lhs.mtx);
        std::lock_guard<std::mutex> guard2(rhs.mtx);

        return (
            lhs.size() == rhs.size() &&
            stl::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
    else {
        std::lock_guard<std::mutex> guard1(rhs.mtx);
        std::lock_guard<std::mutex> guard2(lhs.mtx);

        return (
            lhs.size() == rhs.size() &&
            stl::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
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
    // 按地址大小顺序获取锁，防止死锁
    if (&lhs.mtx < &rhs.mtx) {
        std::lock_guard<std::mutex> guard1(lhs.mtx);
        std::lock_guard<std::mutex> guard2(rhs.mtx);

        return stl::lexicographical_compare(
            lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    else {
        std::lock_guard<std::mutex> guard1(rhs.mtx);
        std::lock_guard<std::mutex> guard2(lhs.mtx);

        return stl::lexicographical_compare(
            lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
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
