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

#include "vector.hh"

namespace stl
{
    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    class Hashtable;

    template <typename Value>
    struct Hashtable_node
    {
        Hashtable_node *next;
        Value val;
    };

    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    struct Hashtable_iterator
    {
        using hashtable = Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>;
        using iterator = Hashtable_iterator<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>;
        using self = iterator;
        using node = Hashtable_node<Value>;

        using iterator_category = stl::forward_iterator_tag;

        using value_type = Value;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using pointer = value_type *;

        node *cur{};
        hashtable *ht;

        Hashtable_iterator() = default;
        Hashtable_iterator(node *n, hashtable *tab)
            : cur(n), ht(tab)
        {
        }

        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        self &operator++()
        {
        }
        self operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const self &rhs) const
        {
            return cur == rhs.cur && ht == rhs.ht;
        }
        bool operator!=(const self &rhs) const

        {
            return !(*this == rhs);
        }
    };

    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    struct Hashtable_const_iterator
    {
        using hashtable = Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>;
        using iterator = Hashtable_iterator<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>;
        using self = Hashtable_const_iterator<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>;

        using node = Hashtable_node<Value>;

        using iterator_category = stl::forward_iterator_tag;

        using value_type = Value;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = const value_type &;
        using pointer = const value_type *;

        node *cur{};
        hashtable *ht;

        iterator ht_const_cast()
        {
            return iterator(cur, ht);
        }

        Hashtable_const_iterator() = default;
        Hashtable_const_iterator(node *n, hashtable *tab)
            : cur(n), ht(tab)
        {
        }

        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        self &operator++()
        {
        }
        self operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const self &rhs) const
        {
            return cur == rhs.cur && ht == rhs.ht;
        }
        bool operator!=(const self &rhs) const

        {
            return !(*this == rhs);
        }
    };

    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual = std::equal_to<Key>,
              typename Alloc = alloc>
    class Hashtable
    {
    public:
        using key_type = Key;
        using value_type = Value;
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
        using const_iterator = Hashtable_const_iterator<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>;
        // using local_iterator = ht_type::local_iterator;
        // using const_local_iterator = ht_type::const_local_iterator;

    private:
        using node = Hashtable_node<Value>;
        using hashtable_node_allocator = simple_alloc<node, Alloc>;

        hasher hash;
        key_equal equals;
        ExtractKey get_key;

        stl::vector<node *> buckets{};
        size_type num_elements{};

        static const int stl_num_primes = 28;
        static constexpr unsigned long stl_prime_list[stl_num_primes] =
            {
                53, 97, 193, 389, 769,
                1543, 3079, 6151, 12289, 24593,
                49157, 98317, 196613, 393241, 786433,
                1572869, 3145739, 6291469, 12582917, 25165843,
                50331653, 100663319, 201326611, 402653189, 805306457,
                1610612741, 3221225473ul, 4294967291ul};

        inline unsigned long stl_next_prime(unsigned long n)
        {
            const unsigned long *pos = stl::lower_bound(stl_prime_list, stl_prime_list + stl_num_primes, n);
            return pos == stl_prime_list + stl_num_primes ? *(--pos) : *pos;
        }

    protected:
        node *new_node(const value_type &obj)
        {
            node *n = hashtable_node_allocator::allocate();
            n->next = nullptr;
            stl::construct(&n->val, obj);

            return n;
        }

        node *new_node(value_type &&obj)
        {
            node *n = hashtable_node_allocator::allocate();
            n->next = nullptr;
            stl::construct(&n->val, obj);

            return n;
        }

        void delete_node(node *n)
        {
            stl::destroy(&n->val);
            hashtable_node_allocator::deallocate(n);
        }

        void initialize_buckets(size_type n)
        {
            const size_type n_buckets = stl_next_prime(n);
            buckets.reserve(n_buckets);
            buckets.insert(buckets.end(), n_buckets, nullptr);
        }

        size_type bkt_num(const value_type &obj, size_t n) const
        {
            return bkt_num_key(get_key(obj), n);
        }

        size_type bkt_num(const value_type &obj) const
        {
            return bkt_num_key(get_key(obj));
        }

        size_type bkt_num_key(const key_type &key) const
        {
            return bkt_num_key(key, buckets.size());
        }

        size_type bkt_num_key(const key_type &key, size_type n) const
        {
            return hash(key) % n;
        }

        void resize(size_type num_elements_hint);

        std::pair<iterator, bool> insert_unique_node(node *n);
        iterator insert_equal_node(node *n);

        void copy_from(const Hashtable &htb);

    public:
        /*
         * Constructors
         * */
        Hashtable(size_type n = 0,
                  const Hash &hf = Hash(),
                  const key_equal &eq = key_equal())
            : hash(hf), equals(eq), get_key(ExtractKey()), num_elements(n)
        {
            initialize_buckets(n);
        }

        Hashtable(const Hashtable &other)
        {
            copy_from(other);
        }
        Hashtable(Hashtable &&other) : buckets(std::move(other.buckets)), num_elements(other.num_elements)
        {
            other.buckets.resize(other.buckets.size(), nullptr);
            other.num_elements = 0;
        }

        /*
         * Destructor
         * */
        ~Hashtable()
        {
            clear();
        }

        /*
         * assignment operation
         * */
        Hashtable &operator=(const Hashtable &other)
        {
            clear();
            copy_from(other);

            return *this;
        }

        Hashtable &operator=(Hashtable &&other) noexcept
        {
            clear();
            buckets = std::move(other.buckets);

            size_type bsize = other.buckets.size();
            for (size_type i = 0; i < bsize; ++i)
            {
                buckets[i] = other.buckets[i];
                other.buckets[i] = nullptr;
            }

            num_elements = other.num_elements;
            other.num_elements = 0;

            return *this;
        }
        Hashtable &operator=(std::initializer_list<value_type> ilist)
        {
            clear();

            insert(ilist.begin(), ilist.end());

            return *this;
        }

        /*
         * Iterator function
         * */
        iterator begin() noexcept
        {
            if (empty())
                return end();
            for (auto &bucket : buckets)
            {
                if (bucket)
                    return iterator(bucket, this);
            }
        }
        const_iterator begin() const noexcept
        {
            return cbegin();
        }
        const_iterator cbegin() const noexcept
        {
            if (empty())
                return cend();
            for (auto &bucket : buckets)
            {
                if (bucket)
                    return const_iterator(bucket, this);
            }
        }

        iterator end() noexcept
        {
            return iterator(nullptr, this);
        }
        const_iterator end() const noexcept
        {
            return const_iterator(nullptr, this);
        }
        const_iterator cend() const noexcept
        {
            return const_iterator(nullptr, this);
        }

        /*
         * Capacity
         * */
        bool empty() const noexcept
        {
            return num_elements == 0;
        }
        size_type size() const noexcept
        {
            return num_elements;
        }
        size_type max_size() const noexcept
        {
            return static_cast<size_type>(-1);
        }

        /*
         * Modifiers
         *  */
        void clear() noexcept
        {
            for (auto &bucket : buckets)
            {
                node *cur = bucket;
                node *next;
                while (cur)
                {
                    next = cur->next;
                    delete_node(cur);
                    cur = next;
                }
                bucket = nullptr;
            }
            num_elements = 0;
        }

        std::pair<iterator, bool> insert_unique(const value_type &value)
        {
            resize(num_elements + 1);
            node *n = new_node(value);
            return insert_unique_node(n);
        }
        std::pair<iterator, bool> insert_unique(value_type &&value)
        {
            resize(num_elements + 1);
            node *n = new_node(std::move(value));
            return insert_unique_node(n);
        }
        iterator insert_unique(const_iterator hint, const value_type &value)
        {
            (void)hint;
            return insert_unique(value);
        }
        iterator insert_unique(const_iterator hint, value_type &&value)
        {
            (void)hint;
            return insert_unique(std::move(value));
        }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void insert_unique(InputIt first, InputIt last)
        {
            while (first != last)
                insert_unique(*first++);
        }
        void insert_unique(std::initializer_list<value_type> ilist)
        {
            insert_unique(ilist.begin(), ilist.end());
        }

        iterator insert_equal(const value_type &value)
        {
            resize(num_elements + 1);
            node *n = new_node(value);
            return insert_equal_node(n);
        }
        iterator insert_equal(value_type &&value)
        {
            resize(num_elements + 1);
            node *n = new_node(std::move(value));
            return insert_equal_node(n);
        }
        iterator insert_equal(const_iterator hint, const value_type &value)
        {
            (void)hint;
            return insert_equal(value);
        }
        iterator insert_equal(const_iterator hint, value_type &&value)
        {
            (void)hint;
            return insert_equal(std::move(value));
        }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void insert_equal(InputIt first, InputIt last)
        {
            while (first != last)
                insert_equal(*first++);
        }
        void insert_equal(std::initializer_list<value_type> ilist)
        {
            insert_equal(ilist.begin(), ilist.end());
        }

        template <typename... Args>
        std::pair<iterator, bool> emplace_unique(Args &&...args)
        {
            resize(num_elements + 1);
            node *n = new_node(std::forward<Args>(args)...);
            return insert_unique_node(n);
        }

        template <typename... Args>
        iterator emplace_unique_hint(const_iterator hint, Args &&...args)
        {
            return emplace_unique(std::forward<Args>(args)...).first;
        }

        template <typename... Args>
        std::pair<iterator, bool> emplace_equal(Args &&...args)
        {
            resize(num_elements + 1);
            node *n = new_node(std::forward<Args>(args)...);
            return insert_equal_node(n);
        }

        template <typename... Args>
        iterator emplace_equal_hint(const_iterator hint, Args &&...args)
        {
            return emplace_equal(std::forward<Args>(args)...).first;
        }
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        size_type erase(const Key &key);

        void swap(Hashtable &other);
        void swap(Hashtable &&other) noexcept;
        // node_type extract(const_iterator position);
        // node_type extract(const Key &k);

        // template <typename H2, typename P2>
        // void merge(std::unordered_set<Key, H2, P2, Alloc> &source);
        // template <typename H2, typename P2>
        // void merge(std::unordered_set<Key, H2, P2, Alloc> &&source);
        // template <typename H2, typename P2>
        // void merge(std::unordered_multiset<Key, H2, P2, Alloc> &source);
        // template <typename H2, typename P2>
        // void merge(std::unordered_multiset<Key, H2, P2, Alloc> &&source);

        size_type max_bucket_count() const
        {
            return stl_prime_list[stl_num_primes - 1];
        }

        /*
         * Lookup
         * */
        size_type count(const key_type &key) const
        {
            size_type times = 0;

            size_type bkt_idx = bkt_num_key(key);
            node *cur = buckets[bkt_idx];

            // by default, the nodes with same value are in group
            while (cur && !equals(get_key(cur->val), key))
                cur = cur->next;
            while (cur && equals(get_key(cur->val), key))
            {
                ++times;
                cur = cur->next;
            }

            return times;
        }
        const_iterator find(const key_type &key) const
        {
            size_type bkt_idx = bkt_num_key(key);
            node *cur = buckets[bkt_idx];

            while (cur && !equals(get_key(cur->val), key))
                cur = cur->next;
            return const_iterator(cur, this);
        }
        bool contains(const key_type &key) const
        {
            return find(key) != cend();
        }
        std::pair<const_iterator, const_iterator>
        equal_range(const key_type &key) const;
    };

    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    void
    Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::resize(size_type num_elements_hint)
    {
        size_type new_bucket_size = stl_next_prime(num_elements_hint);
        if (new_bucket_size <= buckets.size())
            return;
        stl::vector<node *> new_buckets(new_bucket_size, nullptr);

        for (auto &bucket : buckets)
        {
            node *first = bucket;

            while (first)
            {
                bucket = bucket->next;
                first->next = buckets[bkt_num(first->val)];
                buckets[bkt_num(first->val)] = first;
                first = bucket;
            }
        }
        buckets.swap(new_buckets);
    }
    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    std::pair<typename Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::iterator, bool>
    Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::insert_unique_node(node *n)
    {
        size_type bkt_idx = bkt_num(n->val);
        bool nofound = true;
        node **pnode = &buckets[bkt_idx];
        while (*pnode && !equals(n->val, (*pnode)->val))
            pnode = &((*pnode)->val);
        if (!(*pnode))
        {
            *pnode = n;
            ++num_elements;
        }
        else
            nofound = false;
        return {iterator(*pnode, this), nofound};
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    typename Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::iterator
    Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::insert_equal_node(node *n)
    {
        size_type bkt_idx = bkt_num(n->val);
        node **pnode = &buckets[bkt_idx];

        while (*pnode && !equals(n->val, (*pnode)->val))
            pnode = &((*pnode)->val);
        n->next = *pnode;
        *pnode = n;

        ++num_elements;
        return iterator(*pnode, this);
    }
    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    void
    Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::copy_from(const Hashtable &htb)
    {
        // by default, all elements are cleared, that is, clear() was called before

        buckets.reserve(htb.buckets.size());
        buckets.insert(buckets.end(), htb.buckets.size(), nullptr);

        size_type bsize = htb.buckets.size();
        node *cur, *next, *copy;
        for (size_type i = 0; i < bsize; ++i)
        {
            if (cur = htb.buckets[i])
            {
                copy = new_node(cur->val);
                buckets[i] = copy;

                while (cur->next)
                {
                    cur = cur->next;
                    copy->next = new_node(cur->val);
                    copy = copy->next;
                }
            }
        }
        num_elements = htb.num_elements;
    }
    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    typename Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::iterator
    Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::erase(const_iterator pos)
    {
        node *cur = pos.node;
        // find the pre
        size_type bkt_idx = bkt_num(cur->val);
        node *&first = buckets[bkt_idx];

        // 找到cur的pre node
        while (first != cur && first->next != cur->val)
            first = first->next;

        if (first == cur)
            first = cur->next;
        else
            first = nullptr;
        iterator ret(pos.node, this);
        ++ret;

        delete_node(cur);

        return ret;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    typename Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::iterator
    Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::erase(const_iterator first, const_iterator last)
    {
        iterator ret;
        while (first != last)
            ret = erase(first++);
        return ret;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename ExtractKey,
              typename KeyEqual,
              typename Alloc>
    typename Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::size_type
    Hashtable<Key, Value, Hash, ExtractKey, KeyEqual, Alloc>::erase(const Key &key)
    {
        size_type times = 0;
        iterator it = find(key);
        if (it == end())
            return times;

        while (it != end() && equals(get_key(it->val), key))
        {
            ++times;
            it = erase(it);
        }

        return times;
    }

} // namespace stl

#endif