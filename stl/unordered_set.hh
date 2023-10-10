//
// Created by rda on 2023/10/05.
//

#ifndef MINISTL_UNORDERED_SET_HH
#define MINISTL_UNORDERED_SET_HH

#include <iostream>
#include <cassert>

#include "alloc.hh"
#include "construct.hh"
#include "iterator.hh"

namespace stl
{
    template <typename Key,
              typename Hash = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    class unordered_set
    {
    private:
        using ht_type = Hashtable<Key, Key, Hash, std::_Identity<Key>, KeyEqual, Alloc>;
        ht_type hashtable;

    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using hasher = Hash;
        using key_equal = KeyEqual;
        using allocator_type = Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = ht_type::iterator;
        using const_iterator = ht_type::const_iterator;
        using local_iterator = ht_type::local_iterator;
        using const_local_iterator = ht_type::const_local_iterator;

        /*
         * Constructors
         * */
        unordered_set()
            : hashtable(0, Hash(), KeyEqual())
        {
        }

        explicit unordered_set(size_type bucket_count,
                               const Hash &hash = Hash(),
                               const key_equal &equal = key_equal())
            : hashtable(bucket_count, hash, equal)
        {
        }
        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        unordered_set(InputIt first, InputIt last,
                      size_type bucket_count = /* implementation-defined */,
                      const Hash &hash = Hash(),
                      const key_equal &equal = key_equal())
            : hashtable(bucket_count, hash, equal)
        {
            insert(first, last);
        }
        unordered_set(const unordered_set &other) : hashtable(other.hashtable)
        {
        }

        unordered_set(unordered_set &&other) : hashtable(std::move(other.hashtable))
        {
        }

        unordered_set(std::initializer_list<value_type> init,
                      size_type bucket_count = /* implementation-defined */,
                      const Hash &hash = Hash(),
                      const key_equal &equal = key_equal())
            : hashtable(bucket_count, hash, equal)
        {
            insert(init.begin(), init.end());
        }
        /*
         * Destructor
         * */
        ~unordered_set();

        /*
         * assignment operation
         * */
        unordered_set &operator=(const unordered_set &other)
        {
            hashtable = other.hashtable;
            return *this;
        }
        unordered_set &operator=(unordered_set &&other) noexcept
        {
            hashtable = std::move(other.hashtable);
            return *this;
        }
        unordered_set &operator=(std::initializer_list<value_type> ilist)
        {
            return *this;
        }

        /*
         * Iterator function
         * */
        iterator begin() noexcept
        {
            return hashtable.begin();
        }
        const_iterator begin() const noexcept
        {
            return hashtable.begin();
        }
        const_iterator cbegin() const noexcept
        {
            return hashtable.cbegin();
        }

        iterator end() noexcept
        {
            return hashtable.end();
        }
        const_iterator end() const noexcept
        {
            return hashtable.end();
        }
        const_iterator cend() const noexcept
        {
            return hashtable.cend();
        }

        /*
         * Capacity
         * */
        bool empty() const noexcept
        {
            return hashtable.empty();
        }

        size_type size() const noexcept
        {
            return hashtable.size();
        }

        size_type max_size() const noexcept
        {
            return hashtable.max_size();
        }

        /*
         * Modifiers
         * */
        void clear() noexcept
        {
            hashtable.clear();
        }

        std::pair<iterator, bool> insert(const value_type &value);
        std::pair<iterator, bool> insert(value_type &&value);
        iterator insert(const_iterator hint, const value_type &value);
        iterator insert(const_iterator hint, value_type &&value);

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void insert(InputIt first, InputIt last)
        {
            hashtable.insert(first, last);
        }
        void insert(std::initializer_list<value_type> ilist)
        {
            hashtable.insert(ilist);
        }

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args &&...args);

        template <typename... Args>
        iterator emplace_hint(const_iterator hint, Args &&...args);

        template <typename... Args>
        iterator emplace_hint(const_iterator hint, Args &&...args);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        size_type erase(const Key &key);

        
        void swap(unordered_set &other) noexcept
        {
            hashtable.swap(other.hashtable);
        }

        node_type extract(const_iterator position);
        node_type extract(const Key &k);

        template <class H2, class P2>
        void merge(std::unordered_set<Key, H2, P2, Alloc> &source);
        template <class H2, class P2>
        void merge(std::unordered_set<Key, H2, P2, Alloc> &&source);
        template <class H2, class P2>
        void merge(std::unordered_multiset<Key, H2, P2, Alloc> &source);
        template <class H2, class P2>
        void merge(std::unordered_multiset<Key, H2, P2, Alloc> &&source);

        /*
         * Lookup
         * */
        size_type count(const Key &key) const
        {
            return hashtable.count(key);
        }
        const_iterator find(const Key &key) const
        {
            return hashtable.find(key);
        }
        bool contains(const Key &key) const
        {
            return hashtable.contains(key);
        }
        std::pair<const_iterator, const_iterator>
        equal_range(const Key &key) const
        {
            return hashtable.equal_range(key);
        }

        /*
         * Bucket interface
         * */
        local_iterator begin(size_type n);
        const_local_iterator begin(size_type n) const;
        const_local_iterator cbegin(size_type n) const;
        local_iterator end(size_type n);
        const_local_iterator end(size_type n) const;
        const_local_iterator cend(size_type n) const;
        size_type bucket_count() const
        {
            return hashtable.bucket_count();
        }
        size_type max_bucket_count() const
        {
            return hashtable.max_bucket_count();
        }
        size_type bucket_size(size_type n) const
        {
            return hashtable.bucket_size(n);
        }
        size_type bucket(const Key &key) const
        {
            return hashtable.bucket(key);
        }

        /*
         * Observers
         * */
        hasher hash_function() const
        {
            return hashtable.hash_function();
        }
        key_equal key_eq() const
        {
            return hashtable.key_eq();
        }
    };
} // namespace stl

#endif