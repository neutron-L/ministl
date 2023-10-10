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

        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using key_compare = Compare;
        using value_compare = Compare;

        using allocator_type = Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

    private:
        using rep_type = Rb_tree<key_type, value_type, std::_Identity<value_type>, key_compare, Alloc>;

        rep_type tree;

    public:
        using iterator = typename rep_type::const_iterator;
        using const_iterator = typename rep_type::const_iterator;
        using reverse_iterator = typename rep_type::const_reverse_iterator;
        using const_reverse_iterator = typename rep_type::const_reverse_iterator;
        /*
         * Constructors
         * */
        set() : set(Compare()) {}
        explicit set(const Compare &comp) : tree(comp) {}
        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        set(InputIt first, InputIt last, const Compare &comp = Compare())
            : tree(comp)
        {
            tree.insert_unique(first, last);
        }

        set(const set &other) : tree(other.tree)
        {
        }
        set(set &&other) : tree(std::move(other.tree))
        {
        }

        set(std::initializer_list<value_type> init,
            const Compare &comp = Compare()) : tree(comp)
        {
            tree.insert_unique(init.begin(), init.end());
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
            tree = other.tree;
            return *this;
        }
        set &operator=(set &&other) noexcept
        {
            tree = std::move(other.tree);
            return *this;
        }

        /*
         * Iterator function
         * */
        iterator begin() noexcept
        {
            return tree.begin();
        }

        const_iterator begin() const noexcept
        {
            return tree.begin();
        }

        const_iterator cbegin() const noexcept
        {
            return tree.cbegin();
        }

        iterator end() noexcept
        {
            return tree.end();
        }

        const_iterator end() const noexcept
        {
            return tree.end();
        }

        const_iterator cend() const noexcept
        {
            return tree.cend();
        }

        reverse_iterator rbegin() noexcept
        {
            return tree.rbegin();
        }
        const_reverse_iterator rbegin() const noexcept
        {
            return tree.rbegin();
        }
        const_reverse_iterator crbegin() const noexcept
        {
            return tree.crbegin();
        }

        reverse_iterator rend() noexcept
        {
            return tree.rend();
        }
        const_reverse_iterator rend() const noexcept
        {
            return tree.rend();
        }
        const_reverse_iterator crend() const noexcept
        {
            return tree.crend();
        }
        /*
         * Capacity
         * */
        bool empty() const noexcept
        {
            return tree.empty();
        }
        size_type size() const noexcept
        {
            return tree.size();
        }

        size_type max_size() const noexcept
        {
            return tree.max_size();
        }

        /*
         * Modifiers
         * */
        void clear() noexcept
        {
            tree.clear();
        }

        std::pair<iterator, bool> insert(const value_type &value) { return tree.insert_unique(value); }
        std::pair<iterator, bool> insert(value_type &&value) { return tree.insert_unique(std::move(value)); }
        iterator insert(const_iterator pos, const value_type &value) { return tree.insert_unique(pos, value); }
        iterator insert(const_iterator pos, value_type &&value) { return tree.insert_unique(pos, std::move(value)); }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void insert(InputIt first, InputIt last)
        {
            tree.insert_unique(first, last);
        }
        void insert(std::initializer_list<value_type> ilist)
        {
            tree.insert_unique(ilist.begin(), ilist.end());
        }
        template <class... Args>
        std::pair<iterator, bool> emplace(Args &&...args)
        {
            return tree.emplace(std::forward<Args>(args)...);
        }

        // iterator erase(iterator pos)
        // {
        //     return tree.erase(pos);
        // }
        iterator erase(const_iterator pos)
        {
            return tree.erase(pos);
        }
        iterator erase(const_iterator first, const_iterator last)
        {
            return tree.erase(first, last);
        }
        size_type erase(const Key &key)
        {
            return tree.erase(key);
        }
        void swap(set &other) noexcept
        {
            tree.swap(other.tree);
        }

        /*
         * Lookup
         * */

        size_type count(const Key &key) const { return tree.count(key); }
        iterator find(const Key &key) { return tree.find(key); }
        const_iterator find(const Key &key) const { return tree.find(key); }
        std::pair<iterator, iterator> equal_range(const Key &key)
        {
            return tree.equal_range(key);
        }
        std::pair<const_iterator, const_iterator> equal_range(const Key &key) const
        {
            return tree.equal_range(key);
        }
        iterator lower_bound(const Key &key)
        {
            return tree.lower_bound(key);
        }
        const_iterator lower_bound(const Key &key) const
        {
            return tree.lower_bound(key);
        }
        iterator upper_bound(const Key &key)
        {
            return tree.upper_bound(key);
        }
        const_iterator upper_bound(const Key &key) const
        {
            return tree.upper_bound(key);
        }
        /*
         * Observers
         * */
        key_compare key_comp() const
        {
            return tree.key_comp();
        }
        value_compare value_comp() const
        {
            return tree.key_comp();
        }
    };

    template <typename Key,
              typename Compare = std::less<Key>,
              typename Alloc = alloc>
    class multiset
    {
    public:
        using key_type = Key;
        using value_type = Key;

        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using key_compare = Compare;
        using value_compare = Compare;

        using allocator_type = Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

    private:
        using rep_type = Rb_tree<key_type, value_type, std::_Identity<value_type>, key_compare, Alloc>;

        rep_type tree;

    public:
        using iterator = typename rep_type::const_iterator;
        using const_iterator = typename rep_type::const_iterator;
        using reverse_iterator = typename rep_type::const_reverse_iterator;
        using const_reverse_iterator = typename rep_type::const_reverse_iterator;
        /*
         * Constructors
         * */
        multiset() : multiset(Compare()) {}
        explicit multiset(const Compare &comp) : tree(comp) {}
        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        multiset(InputIt first, InputIt last, const Compare &comp = Compare())
            : tree(comp)
        {
            tree.insert_equal(first, last);
        }

        multiset(const multiset &other) : tree(other.tree)
        {
        }
        multiset(multiset &&other) : tree(std::move(other.tree))
        {
        }

        multiset(std::initializer_list<value_type> init,
            const Compare &comp = Compare()) : tree(comp)
        {
            tree.insert_equal(init.begin(), init.end());
        }
        /*
         * Destructor
         * */
        ~multiset() = default;

        /*
         * assignment operation
         * */
        multiset &operator=(const multiset &other)
        {
            tree = other.tree;
            return *this;
        }
        multiset &operator=(multiset &&other) noexcept
        {
            tree = std::move(other.tree);
            return *this;
        }

        /*
         * Iterator function
         * */
        iterator begin() noexcept
        {
            return tree.begin();
        }

        const_iterator begin() const noexcept
        {
            return tree.begin();
        }

        const_iterator cbegin() const noexcept
        {
            return tree.cbegin();
        }

        iterator end() noexcept
        {
            return tree.end();
        }

        const_iterator end() const noexcept
        {
            return tree.end();
        }

        const_iterator cend() const noexcept
        {
            return tree.cend();
        }

        reverse_iterator rbegin() noexcept
        {
            return tree.rbegin();
        }
        const_reverse_iterator rbegin() const noexcept
        {
            return tree.rbegin();
        }
        const_reverse_iterator crbegin() const noexcept
        {
            return tree.crbegin();
        }

        reverse_iterator rend() noexcept
        {
            return tree.rend();
        }
        const_reverse_iterator rend() const noexcept
        {
            return tree.rend();
        }
        const_reverse_iterator crend() const noexcept
        {
            return tree.crend();
        }
        /*
         * Capacity
         * */
        bool empty() const noexcept
        {
            return tree.empty();
        }
        size_type size() const noexcept
        {
            return tree.size();
        }

        size_type max_size() const noexcept
        {
            return tree.max_size();
        }

        /*
         * Modifiers
         * */
        void clear() noexcept
        {
            tree.clear();
        }

        iterator insert(const value_type &value) { return tree.insert_equal(value); }
        iterator insert(value_type &&value) { return tree.insert_equal(std::move(value)); }
        iterator insert(const_iterator pos, const value_type &value) { return tree.insert_equal(pos, value); }
        iterator insert(const_iterator pos, value_type &&value) { return tree.insert_equal(pos, std::move(value)); }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void insert(InputIt first, InputIt last)
        {
            tree.insert_equal(first, last);
        }
        void insert(std::initializer_list<value_type> ilist)
        {
            tree.insert_equal(ilist.begin(), ilist.end());
        }
        template <class... Args>
        std::pair<iterator, bool> emplace(Args &&...args)
        {
            return tree.emplace(std::forward<Args>(args)...);
        }

        // iterator erase(iterator pos)
        // {
        //     return tree.erase(pos);
        // }
        iterator erase(const_iterator pos)
        {
            return tree.erase(pos);
        }
        iterator erase(const_iterator first, const_iterator last)
        {
            return tree.erase(first, last);
        }
        size_type erase(const Key &key)
        {
            return tree.erase(key);
        }
        void swap(multiset &other) noexcept
        {
            tree.swap(other.tree);
        }

        /*
         * Lookup
         * */

        size_type count(const Key &key) const { return tree.count(key); }
        iterator find(const Key &key) { return tree.find(key); }
        const_iterator find(const Key &key) const { return tree.find(key); }
        std::pair<iterator, iterator> equal_range(const Key &key)
        {
            return tree.equal_range(key);
        }
        std::pair<const_iterator, const_iterator> equal_range(const Key &key) const
        {
            return tree.equal_range(key);
        }
        iterator lower_bound(const Key &key)
        {
            return tree.lower_bound(key);
        }
        const_iterator lower_bound(const Key &key) const
        {
            return tree.lower_bound(key);
        }
        iterator upper_bound(const Key &key)
        {
            return tree.upper_bound(key);
        }
        const_iterator upper_bound(const Key &key) const
        {
            return tree.upper_bound(key);
        }
        /*
         * Observers
         * */
        key_compare key_comp() const
        {
            return tree.key_comp();
        }
        value_compare value_comp() const
        {
            return tree.key_comp();
        }
    };
} // namespace stl

#endif
