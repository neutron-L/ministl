//
// Created by rda on 2023/9/27.
//

#ifndef MINISTL_MAP_HH
#define MINISTL_MAP_HH

#include "alloc.hh"
#include "construct.hh"
#include "rbtree.hh"

namespace stl
{
    template <typename Key, typename Value,
              typename Compare = std::less<Key>,
              typename Alloc = alloc>
    class map
    {
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = std::pair<const Key, Value>;

        using size_type = std::size_type;
        using difference_type = std::ptrdiff_t;

        using key_compare = Compare;
        using allocator_type = Alloc;

        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

    private:
        using rep_type = Rb_tree<key_type, value_type, std::_Select1st<value_type>, key_compare, Alloc>;

        rep_type tree;

    public:
        using iterator = typename rep_type::iterator;
        using const_iterator = typename rep_type::const_iterator;
        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

        /*
         * Constructors
         * */
        map() : map(Compare()) {}
        explicit map(const Compare &comp) {}
        template <typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
        map(InputIterator first, InputIterator last, const Compare &comp = Compare())
            : tree(comp)
        {
            tree.insert_unique(first, last);
        }

        map(const map &other) : tree(other.tree)
        {
        }
        map(map &&other) : tree(std::move(other.tree))
        {
        }

        map(std::initializer_list<value_type> init,
            const Compare &comp = Compare()) : tree(comp)
        {
            tree.insert_unique(init.begin(), init.end());
        }
        /*
         * Destructor
         * */
        ~map() = default;

        /*
         * assignment operation
         * */
        map &operator=(const map &other)
        {
            tree = other.tree;
            return *this;
        }
        map &operator=(map &&other) noexcept
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

        template <typename InputIt, typename = std::_RequireInputIter<InputIterator>>
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
            tree.emplace(std::forward<Args>(args)...);
        }

        iterator erase(iterator pos)
        {
            tree.erase(pos);
        }
        iterator erase(const_iterator pos)
        {
            tree.erase(pos);
        }
        iterator erase(const_iterator first, const_iterator last)
        {
            tree.erase(first, last);
        }
        size_type erase(const Key &key)
        {
            tree.erase(key);
        }
        void swap(map &other) noexcept
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
        std::map::value_compare value_comp() const
        {
            return tree.key_comp();
        }
    };

} // namespace stl

#endif
