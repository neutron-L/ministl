//
// Created by rda on 2023/9/14.
//

#ifndef MINISTL_SET_HH
#define MINISTL_SET_HH

#include "alloc.hh"
#include "construct.hh"
#include "rbtree.hh"

namespace stl
{
    template <typename Key,
              typename Compare = std::less<Key>,
              typename Alloc = alloc>
    class set
    {
    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = std::size_type;
        using difference_type = std::ptrdiff_t;
        using key_compare = Compare;
        using value_compare = Compare;
        using allocator_type = Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

        // using iterator
    protected:
        Container container;

    public:
        /*
         * Constructors
         * */
        set() : set(Compare()) {}
        explicit set(const Compare &comp) {}
        template <typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
        set(InputIterator first, InputIterator last, const Compare &comp = Compare())
        {
        }

        set(const queue &other)
        {
        }
        set(queue &&other)
        {
        }

        set(std::initializer_list<value_type> init,
            const Compare &comp = Compare())
        {
        }
        /*
         * Destructor
         * */
        ~set() = default;

        /*
         * assignment operation
         * */
        set &operator=(const set &other)
        {
        }
        set &operator=(set &&other) noexcept
        {
        }

        /*
         * Iterator function
         * */
        iterator begin() noexcept
        {
        }

        const_iterator begin() const noexcept
        {
        }

        const_iterator cbegin() const noexcept
        {
        }

        iterator end() noexcept
        {
        }

        const_iterator end() const noexcept
        {
        }

        const_iterator cend() const noexcept
        {
        }

        reverse_iterator rbegin() noexcept
        {
        }
        const_reverse_iterator rbegin() const noexcept
        {
        }
        const_reverse_iterator crbegin() const noexcept
        {
        }

        reverse_iterator rend() noexcept
        {
        }
        const_reverse_iterator rend() const noexcept
        {
        }
        const_reverse_iterator crend() const noexcept
        {
        }
        /*
         * Capacity
         * */
        bool empty() const noexcept
        {
        }
        size_type size() const noexcept
        {
        }

        size_type max_size() const noexcept
        {
        }

        /*
         * Modifiers
         * */
        
    };

} // namespace stl

#endif
