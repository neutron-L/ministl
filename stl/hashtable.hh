//
// Created by rda on 2023/10/06.
//

#ifndef MINISTL_HASHTABLE_HH
#define MINISTL_HASHTABLE_HH

#include <iostream>
#include <cassert>

#include "alloc.hh"
#include "construct.hh"
#include "iterator.hh"

namespace stl
{
    template <typename Value>
    struct Hashtable_node
    {
        Hashtable_node *next;
        Value val;
    };

    template <typename Key,
              typename Value,
              typename Hash = std::hash<Key>,
              typename ExtractKey,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    struct Hashtable_const_iterator
    {
    };

    template <typename Key,
              typename Value,
              typename Hash = std::hash<Key>,
              typename ExtractKey,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    struct Hashtable_iterator
    {
    };

    template <typename Key,
              typename Value,
              typename Hash = std::hash<Key>,
              typename ExtractKey,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    class Hashtable
    {
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
        using iterator = Hashtable_iterator<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>;
        using const_iterator = ht_type::const_iterator;
        using iterator = ht_type::local_iterator;
        using const_local_iterator = ht_type::const_local_iterator;

    private:
        hasher hash;
        key_equal equals;
        ExtractKey get_key;

    public:
        /*
         * Constructors
         * */
        unordered_set();
        explicit unordered_set(size_type bucket_count,
                               const Hash &hash = Hash(),
                               const key_equal &equal = key_equal());
        template <class InputIt>
        unordered_set(InputIt first, InputIt last,
                      size_type bucket_count = /* implementation-defined */,
                      const Hash &hash = Hash(),
                      const key_equal &equal = key_equal());
        unordered_set(const unordered_set &other);
        unordered_set(unordered_set &&other);
        unordered_set(std::initializer_list<value_type> init,
                      size_type bucket_count = /* implementation-defined */,
                      const Hash &hash = Hash(),
                      const key_equal &equal = key_equal());
        /*
         * Destructor
         * */
        ~unordered_set();

        /*
         * assignment operation
         * */
        unordered_set &operator=(const unordered_set &other);
        unordered_set &operator=(unordered_set &&other) noexcept;
        unordered_set &operator=(std::initializer_list<value_type> ilist);

        /*
         * Iterator function
         * */
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator begin() const noexcept;

        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        /*
         * Capacity
         * */
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;

        /*
         * Modifiers
         * */
        void clear() noexcept;

        std::pair<iterator, bool> insert(const value_type &value);
        std::pair<iterator, bool> insert(value_type &&value);
        iterator insert(const_iterator hint, const value_type &value);
        iterator insert(const_iterator hint, value_type &&value);

        template <class InputIt>
        void insert(InputIt first, InputIt last);
        void insert(std::initializer_list<value_type> ilist);

        template <class... Args>
        std::pair<iterator, bool> emplace(Args &&...args);

        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args &&...args);

        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args &&...args);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        size_type erase(const Key &key);

        void swap(unordered_set &other);
        void swap(unordered_set &other) noexcept;
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
    };
} // namespace stl

#endif