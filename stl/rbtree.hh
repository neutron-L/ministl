//
// Created by rda on 2023/9/14.
//

#ifndef MINISTL_RBTREE_HH
#define MINISTL_RBTREE_HH

#include <iostream>
#include <cassert>

#include "alloc.hh"
#include "construct.hh"
#include "iterator.hh"
#include "vector.hh"
#include "stack.hh"

namespace stl
{
    using std::cerr;
    using std::endl;

    // RB-Tree Node Color, include red and black
    enum class Rb_tree_color
    {
        Red,
        Black
    };

    // Base struct of RB-Tree Node
    struct Rb_tree_node_base
    {
        using color_type = Rb_tree_color;
        using base_ptr = Rb_tree_node_base *;

        color_type color{};
        base_ptr parent{};
        base_ptr left{};
        base_ptr right{};

        base_ptr minimum()
        {
            base_ptr cur = this;
            while (cur->left != nullptr)
                cur = cur->left;
            return cur;
        }

        base_ptr maximum()
        {
            base_ptr cur = this;
            while (cur->right != nullptr)
                cur = cur->right;
            return cur;
        }
    };

    // struct of RB-Tree Node
    template <typename Value>
    struct Rb_tree_node : public Rb_tree_node_base
    {
        using link_type = Rb_tree_node<Value> *;
        Value value_field{};
    };

    // RB-Tree iterator base class
    struct Rb_tree_base_iterator
    {
        using base_ptr = Rb_tree_node_base::base_ptr;
        using iterator_category = stl::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;

        base_ptr node{};

        void increment()
        {
            if (node->right) // CASE 1: 进入右子树，寻找右子树的最小节点
            {
                node = node->right;
                while (node->left)
                    node = node->left;
            }
            else
            {
                base_ptr p = node->parent;
                while (node == p->right) // CASE 2: 已经是某树的左子树最大节点
                {
                    node = p;
                    p = p->parent;
                }
                if (node->right != p) // node可能为header
                    node = p;
            }
        }

        void decrement()
        {
            if (node->color == Rb_tree_color::Red && node->parent->parent == node)
                node = node->right;
            else if (node->left)
            {
                node = node->left;
                while (node->right)
                    node = node->right;
            }
            else
            {
                base_ptr p = node->parent;
                while (node == p->left)
                {
                    node = p;
                    p = p->parent;
                }
                node = p;
            }
        }
    };

    // RB-Tree iterator
    template <typename Value, typename Ref, typename Ptr>
    struct Rb_tree_iterator : public Rb_tree_base_iterator
    {
        using value_type = Value;
        using reference = Ref;
        using pointer = Ptr;

        using iterator = Rb_tree_iterator<Value, Value &, Value *>;
        using const_iterator = Rb_tree_iterator<Value, const Value &, const Value *>;
        using self = Rb_tree_iterator<Value, Ref, Ptr>;
        using link_type = Rb_tree_node<Value> *;

        Rb_tree_iterator() {}
        Rb_tree_iterator(link_type lt) { node = lt; }
        Rb_tree_iterator(const iterator &it) { node = it.node; }

        reference operator*() const { return ((link_type)node)->value_field; }

        pointer operator->() const { return &operator*(); }

        self &operator++()
        {
            increment();
            return *this;
        }
        self operator++(int)
        {
            auto obj = *this;
            increment();
            return obj;
        }

        self &operator--()
        {
            decrement();
            return *this;
        }
        self operator--(int)
        {
            auto obj = *this;
            decrement();
            return obj;
        }

        friend bool
        operator==(const self &lhs, const self &rhs)
        {
            return lhs.node == rhs.node;
        }

        friend bool
        operator!=(const self &lhs, const self &rhs)
        {
            return lhs.node != rhs.node;
        }
    };

    // RB-Tree
    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc = alloc>
    class Rb_tree
    {
    public:
        /* Member types */
        using key_type = Key;
        using value_type = Value;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using link_type = Rb_tree_node<Value> *;
        using const_link_type = const Rb_tree_node<Value> *;

        using iterator = typename Rb_tree_iterator<value_type, reference, pointer>::iterator;
        using const_iterator = typename Rb_tree_iterator<value_type, reference, pointer>::const_iterator;
        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

    protected:
        using base_ptr = Rb_tree_node_base *;
        using rb_tree_node = Rb_tree_node<value_type>;
        using color_type = Rb_tree_color;
        using rb_tree_node_allocator = simple_alloc<rb_tree_node, Alloc>;

        link_type header{};
        // link_type nil{};           // 作为rb-tree中的空指针（空节点），所有叶子节点的左右指针指向它，为黑色
        size_type node_count{};
        Compare key_compare{};

        static link_type get_node() { return rb_tree_node_allocator::allocate(); }
        static void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

        link_type create_node(const value_type &val)
        {
            link_type node = get_node();
            stl::construct(&node->value_field, val);

            return node;
        }

        link_type create_node(value_type &&val)
        {
            link_type node = get_node();
            stl::construct(&node->value_field, std::move(val));

            return node;
        }

        link_type clone_node(link_type p)
        {
            auto v{p->value_field};
            link_type node = create_node(std::move(v));
            node->color = p->color;
            node->left = node->right = node->parent = nullptr;

            return node;
        }

        void destroy_node(link_type p)
        {
            stl::destroy(&p->value_field);
            put_node(p);
        }

        // get the members of header
        base_ptr &root() const { return header->parent; }
        base_ptr &leftmost() const { return header->left; }
        base_ptr &rightmost() const { return header->right; }

        // get the members of node
        static base_ptr &left(base_ptr p)
        {
            return p->left;
        }

        static base_ptr &right(base_ptr p)
        {
            return p->right;
        }

        static base_ptr &parent(base_ptr p)
        {
            return p->parent;
        }

        static reference value(base_ptr p)
        {
            return static_cast<link_type>(p)->value_field;
        }

        static const Key &key(base_ptr p)
        {
            return KeyOfValue()(value(p));
        }

        static color_type &color(base_ptr p)
        {
            return p->color;
        }

        // link type
        // static link_type &left(link_type p)
        // {
        //     return static_cast<link_type>(p->left);
        // }

        // static link_type &right(link_type p)
        // {
        //     return static_cast<link_type>(p->right);
        // }

        // static link_type &parent(link_type p)
        // {
        //     return static_cast<link_type>(p->parent);
        // }

        static reference value(link_type p)
        {
            return p->value_field;
        }

        static const Key &key(link_type p)
        {
            return KeyOfValue()(value(p));
        }

        // static color_type &color(link_type p)
        // {
        //     return p->color;
        // }

        // find the max/min node
        static link_type minimum(link_type p)
        {
            return static_cast<link_type>(Rb_tree_node_base::minimum(p));
        }

        static link_type maximum(link_type p)
        {
            return static_cast<link_type>(Rb_tree_node_base::maximum(p));
        }

        // Auxiliary functions for inserting methods
        std::pair<base_ptr, base_ptr> get_insert_unique_pos(const key_type &);
        std::pair<base_ptr, base_ptr> get_insert_hint_unique_pos(const_iterator, const key_type &);
        std::pair<base_ptr, base_ptr> get_insert_equal_pos(const key_type &);
        std::pair<base_ptr, base_ptr> get_insert_hint_equal_pos(const_iterator, const key_type &);

    private:
        // 通过判断black height，验证红黑树是否合法
        // 返回一个pair{balck height, is valid}
        static std::pair<int, bool> isValid(base_ptr);
        iterator insert(link_type, link_type, value_type &&);
        void transplant(base_ptr x, base_ptr y);
        /*
         * 创建一个伪节点nil，颜色为black，默认是p（不为header）的右子节点
         * */
        link_type create_nil(base_ptr p)
        {
            link_type nil = get_node();
            left(nil) = right(nil) = nullptr;
            parent(nil) = p;
            color(nil) = Rb_tree_color::Black;

            p->right = nil;

            return nil;
        }
        iterator erase(link_type cur);

        void init()
        {
            header = get_node();
            // nil = get_node();

            color(header) = color_type::Red;
            root() = nullptr;
            leftmost() = rightmost() = header;

            node_count = 0;
        }

        void rb_tree_insert_rebalance(Rb_tree_node_base *p, Rb_tree_node_base *&root);
        void rb_tree_erase_rebalance(Rb_tree_node_base *p, Rb_tree_node_base *&root);
        void rb_tree_rotate_left(Rb_tree_node_base *p, Rb_tree_node_base *&root);
        void rb_tree_rotate_right(Rb_tree_node_base *p, Rb_tree_node_base *&root);

    public:
        Rb_tree(const Compare &comp = Compare())
            : key_compare(comp)
        {
            init();
        }

        Rb_tree(const Rb_tree &other)
        {
            init();
            clear();

            // insert all elements
            insert_equal(other.begin(), other.end());

            assert(node_count == other.node_count);
        }
        Rb_tree(Rb_tree &&other)
            : header(std::move(other.header)), node_count(std::move(other.node_count))
        {
            other.init(); // other还原到初始状态
        }

        /*
         * Destructor
         * */
        ~Rb_tree()
        {
            clear();
            put_node(header);
            // put_node(nil);
        }

        /*
         * assignment operation
         * */
        Rb_tree &operator=(const Rb_tree &other)
        {
            clear();

            insert_equal(other.begin(), other.end());

            return *this;
        }

        Rb_tree &operator=(Rb_tree &&other) noexcept
        {
            clear();
            put_node(header);

            header = std::move(other.header);
            // nil = std::move(other.nil);
            node_count = other.node_count;

            other.init(); // other还原到初始状态

            return *this;
        }

        /*
         * Iterator function
         * */
        iterator begin() noexcept
        {
            return iterator(static_cast<link_type>(leftmost()));
        }

        const_iterator begin() const noexcept
        {
            return const_iterator(static_cast<link_type>(leftmost()));
        }

        const_iterator cbegin() const noexcept
        {
            return const_iterator(static_cast<link_type>(leftmost()));
        }

        iterator end() noexcept
        {
            return iterator(header);
        }

        const_iterator end() const noexcept
        {
            return const_iterator(header);
        }

        const_iterator cend() const noexcept
        {
            return const_iterator(header);
        }

        reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
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
            return reverse_iterator(begin());
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
        bool empty() const noexcept { return node_count == 0; }
        size_type size() const noexcept { return node_count; }
        size_type max_size() const noexcept { return static_cast<size_type>(-1); }

        /*
         * Modifiers
         * */
        void clear() noexcept
        {
            erase(begin(), end());
        }

        std::pair<iterator, bool> insert_unique(const value_type &value)
        {
            // value_type v(std::move(value));
            value_type v(value);
            return insert_unique(std::move(v));
        }

        std::pair<iterator, bool> insert_unique(value_type &&value)
        {
            auto res = get_insert_unique_pos(KeyOfValue()(value));
            if (res.second)
                return {insert(static_cast<link_type>(res.first), static_cast<link_type>(res.second), std::move(value)), true};
            return {iterator(static_cast<link_type>(res.first)), false};
        }

        iterator insert_unique(const_iterator pos, const value_type &value)
        {
            value_type v(value);
            return insert_unique(pos, std::move(v));
        }
        iterator insert_unique(const_iterator pos, value_type &&value)
        {
            auto res = get_insert_hint_unique_pos(pos, KeyOfValue()(value));
            if (res.second)
                return insert(static_cast<link_type>(res.first), static_cast<link_type>(res.second), std::move(value));
            return res.first;
        }

        iterator insert_equal(const value_type &value)
        {
            value_type v(value);
            return insert_equal(std::move(v));
        }

        iterator insert_equal(value_type &&value)
        {
            auto res = get_insert_equal_pos(KeyOfValue()(value));
            return insert(static_cast<link_type>(res.first), static_cast<link_type>(res.second), std::move(value));
        }

        iterator insert_equal(const_iterator pos, const value_type &value)
        {
            value_type v(std::move(value));
            return insert_equal(pos, std::move(v));
        }
        iterator insert_equal(const_iterator pos, value_type &&value)
        {
        }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void insert_unique(InputIt first, InputIt last)
        {
            while (first != last)
            {
                insert_unique(*first);
                ++first;
            }
        }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void insert_equal(InputIt first, InputIt last)
        {
            while (first != last)
            {
                insert_equal(*first);
                ++first;
            }
        }

        iterator erase(iterator pos)
        {
            return erase(static_cast<link_type>(pos.node));
        }
        iterator erase(const_iterator pos)
        {
            iterator i = iterator(static_cast<link_type>(pos.node));
            return erase(static_cast<link_type>(i.node));
        }
        iterator erase(const_iterator first, const_iterator last)
        {
            iterator ret;

            while (first != last)
            {
                ret = erase(first++);
                assert(*ret == *first);
            }
            return ret;
        }
        size_type erase(const Key &key)
        {
            auto first = lower_bound(key);
            auto last = upper_bound(key);
            size_type ret = stl::distance(first, last);
            erase(first, last);

            return ret;
        }
        void swap(Rb_tree &other) noexcept
        {
            swap(header, other.header);
            swap(node_count, other.node_count);
        }

        /*
         * Lookup
         * */
        size_type count(const key_type &key) const
        {
            return stl::distance(lower_bound(key), upper_bound(key));
        }
        iterator find(const key_type &key)
        {
            base_ptr node = const_cast<const Rb_tree *>(this)->find(key).node;
            return iterator(static_cast<link_type>(node));
        }
        const_iterator find(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key)
        {
            auto ret = const_cast<const Rb_tree *>(this)->equal_range(key);
            return {iterator(static_cast<link_type>(ret.first.node)), iterator(static_cast<link_type>(ret.second.node))};
        }
        std::pair<const_iterator, const_iterator> equal_range(const Key &k) const;
        iterator lower_bound(const Key &k)
        {
            base_ptr node = const_cast<const Rb_tree *>(this)->lower_bound(k).node;
            return iterator(static_cast<link_type>(node));
        }
        const_iterator lower_bound(const Key &) const;
        iterator upper_bound(const Key &k)
        {
            base_ptr node = const_cast<const Rb_tree *>(this)->upper_bound(k).node;
            return iterator(static_cast<link_type>(node));
        }
        const_iterator upper_bound(const Key &) const;

        /*
         * Observers
         * */
        Compare key_comp() const { return key_compare; }

        // Traverse Rb-tree
        // first: value field; second: color(0 for red, 1 for black)
        stl::vector<std::pair<Value, int>> pre_traverse();
        stl::vector<std::pair<Value, int>> mid_traverse();
        stl::vector<std::pair<Value, int>> post_traverse();
    };

    /*
     * Rb-Tree insert and erase balance
     * */
    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_insert_rebalance(Rb_tree_node_base *x, Rb_tree_node_base *&root)
    {
        x->color = Rb_tree_color::Red;
        while (x != root && x->parent->color == Rb_tree_color::Red)
        {
            auto p = x->parent;
            if (p == p->parent->left) // 父亲节点是祖父节点的左孩子
            {
                auto y = p->parent->right;               // 伯父节点
                if (y && y->color == Rb_tree_color::Red) // 伯父节点为Red，则祖父节点为Black
                {
                    p->color = y->color = Rb_tree_color::Black;
                    y->color = Rb_tree_color::Black;
                    p->parent->color = Rb_tree_color::Red;
                    x = p->parent;
                }
                else // 伯父节点不存在或为Black
                {
                    if (x == p->right) // 把x调整为父亲节点的左孩子
                    {
                        x = p;
                        rb_tree_rotate_left(x, root);
                        p = x->parent;
                    }
                    p->color = Rb_tree_color::Black;

                    p->parent->color = Rb_tree_color::Red;
                    // x = p;
                    rb_tree_rotate_right(p->parent, root);
                }
            }
            else
            {
                auto y = p->parent->left;                // 伯父节点
                if (y && y->color == Rb_tree_color::Red) // 伯父节点为Red，则祖父节点为Black
                {
                    p->color = y->color = Rb_tree_color::Black;
                    y->color = Rb_tree_color::Black;
                    p->parent->color = Rb_tree_color::Red;
                    x = p->parent;
                }
                else // 伯父节点不存在或为Black
                {
                    if (x == p->left) // 把x调整为父亲节点的左孩子
                    {
                        x = p;
                        rb_tree_rotate_right(x, root);
                        p = x->parent;
                    }
                    p->color = Rb_tree_color::Black;

                    p->parent->color = Rb_tree_color::Red;
                    // x = p;
                    rb_tree_rotate_left(p->parent, root);
                }
            }
        }

        root->color = Rb_tree_color::Black;
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    void
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_erase_rebalance(Rb_tree_node_base *x, Rb_tree_node_base *&root)
    {
        while (x != root && x->color == Rb_tree_color::Black)
        {
            if (x == x->parent->left)
            {
                auto w = x->parent->right;
                assert(w);

                if (w->color == Rb_tree_color::Red)
                {
                    w->color = Rb_tree_color::Black;
                    w->parent->color = Rb_tree_color::Red;
                    rb_tree_rotate_left(w->parent, root);
                    w = x->parent->right;
                    assert(w);
                }
                assert(w->color == Rb_tree_color::Black);
                if (!((w->left && w->left->color == Rb_tree_color::Red) || (w->right && w->right->color == Rb_tree_color::Red)))
                {
                    w->color = Rb_tree_color::Red;
                    x = x->parent;
                }
                else
                {
                    if (!(w->right && w->right->color == Rb_tree_color::Red))
                    {
                        w->color = Rb_tree_color::Red;
                        w->left->color = Rb_tree_color::Black;
                        rb_tree_rotate_right(w, root);
                        w = x->parent->right;
                        assert(w->right->color == Rb_tree_color::Red);
                    }
                    assert(w->color == Rb_tree_color::Black);

                    w->color = w->parent->color;
                    w->parent->color = Rb_tree_color::Black;
                    w->right->color = Rb_tree_color::Black;
                    rb_tree_rotate_left(w->parent, root);
                    x = root;
                }
            }
            else
            {
                auto w = x->parent->left;
                assert(w);

                if (w->color == Rb_tree_color::Red)
                {
                    w->color = Rb_tree_color::Black;
                    w->parent->color = Rb_tree_color::Red;
                    rb_tree_rotate_right(w->parent, root);
                    w = x->parent->left;
                    assert(w);
                }
                assert(w->color == Rb_tree_color::Black);
                if (!((w->left && w->left->color == Rb_tree_color::Red) || (w->right && w->right->color == Rb_tree_color::Red)))
                {
                    w->color = Rb_tree_color::Red;
                    x = x->parent;
                }
                else
                {
                    if (!(w->left && w->left->color == Rb_tree_color::Red))
                    {
                        w->color = Rb_tree_color::Red;
                        w->right->color = Rb_tree_color::Black;
                        rb_tree_rotate_left(w, root);
                        w = x->parent->left;
                        assert(w->left->color == Rb_tree_color::Red);
                    }
                    assert(w->color == Rb_tree_color::Black);

                    w->color = w->parent->color;
                    w->parent->color = Rb_tree_color::Black;
                    w->left->color = Rb_tree_color::Black;
                    rb_tree_rotate_right(w->parent, root);
                    x = root;
                }
            }
        }

        x->color = Rb_tree_color::Black;
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(Rb_tree_node_base *x, Rb_tree_node_base *&root)
    {
        Rb_tree_node_base *y = x->right;

        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if (root == x)
            root = y;
        else if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }
    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(Rb_tree_node_base *x, Rb_tree_node_base *&root)
    {
        Rb_tree_node_base *y = x->left;

        x->left = y->right;
        if (y->right)
            y->right->parent = x;
        y->parent = x->parent;

        if (root == x)
            root = y;
        else if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->right = x;
        x->parent = y;
    }

    /*
     * Auxiliary methods
     * */
    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    std::pair<typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr,
              typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::get_insert_unique_pos(const key_type &k)
    {
        base_ptr y = header;
        base_ptr x = root();
        bool comp = true;

        while (x)
        {
            y = x;
            comp = key_compare(k, key(x));
            x = comp ? left(x) : right(x);
        }

        iterator j = iterator(static_cast<link_type>(y));
        if (comp)
        {
            if (j == begin())
                return {x, y};
            else
                --j;
        }
        if (key_compare(key(j.node), k))
            return {x, y};
        return {j.node, 0};
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    std::pair<typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr,
              typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::get_insert_hint_unique_pos(const_iterator pos, const key_type &k)
    {
        iterator p = iterator(static_cast<link_type>(pos.node));

        if (p == end())
        {
            if (size() > 0 && key_compare(key(rightmost()), k))
                return {0, rightmost()};
            else
                return get_insert_unique_pos(k);
        }
        else if (key_compare(k, key(p.node)))
        {
            iterator before = p;
            if (p == begin() && key_compare(k, key(leftmost())))
                return {leftmost(), leftmost()};
            else if (key_compare(key((--before).node), k))
            {
                if (right(before.node) == nullptr)
                    return {0, before.node};
                else
                    return {p.node, p.node};
            }
            else
                return get_insert_unique_pos(k);
        }
        else if (key_compare(k, key(p.node)))
        {
            iterator after = p;
            if (p.node == rightmost() && key_compare(key(rightmost()), k))
                return {0, rightmost()};
            else if (key_compare(k, key((++after).node)))
            {
                if (left(after.node) == nullptr)
                    return {after.node, after.node};
                else
                    return {0, p.node};
            }
            else
                return get_insert_unique_pos(k);
        }
        else
            return {pos.node, 0};
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    std::pair<typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr,
              typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::get_insert_equal_pos(const key_type &k)
    {
        base_ptr y = header;
        base_ptr x = root();

        while (x)
        {
            y = x;
            if (key_compare(k, key(x)))
                x = left(x);
            else
                x = right(x);
        }

        return {x, y};
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    std::pair<int, bool>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::isValid(base_ptr p)
    {
        static bool first = true;
        if (!p)
            return {0, true};
        else
        {
            auto l = isValid(p->left);
            auto r = isValid(p->right);
            if (!l.second || !r.second)
            {
                if (first)
                {
                    if (!l.second)
                        cerr << "subtree " << static_cast<link_type>(p->left)->value_field << " is invalid" << endl;
                    if (!r.second)
                        cerr << "subtree " << static_cast<link_type>(p->right)->value_field << " is invalid" << endl;
                }
                first = false;
                return {-1, false};
            }

            bool eq = l.first == r.first;
            return {eq ? l.first + (p->color == Rb_tree_color::Black) : -1, eq};
        }
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert(link_type x, link_type y, value_type &&value)
    {
        link_type node = create_node(std::move(value));

        if (y == header || x != nullptr || key_compare(KeyOfValue()(value), key(y))) // CASE 1: left
        {
            left(y) = node;
            if (y == header)
            {
                root() = node;
                rightmost() = node;
            }
            else if (y == static_cast<link_type>(leftmost()))
                leftmost() = node;
        }
        else
        {
            right(y) = node;
            if (y == rightmost())
                rightmost() = node;
        }

        parent(node) = y;
        left(node) = right(node) = nullptr;

        rb_tree_insert_rebalance(node, header->parent);
        ++node_count;

        assert(isValid(root()).second);
        return iterator(node);
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    void
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::transplant(base_ptr x, base_ptr y)
    {
        assert(x && y);
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->parent = x->parent;
        if (header->parent == x)
            header->parent = y;
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(link_type cur)
    {
        static base_ptr nil = nullptr;

        Rb_tree_color origin_color = cur->color;
        base_ptr x, y;
        y = cur;
        iterator ret = iterator(cur);
        ++ret;

        // step 1. 调整leftmost和rightmost
        if (y == leftmost())
            leftmost() = ret.node;
        if (y == rightmost())
        {
            auto i = iterator(static_cast<link_type>(y));
            --i;
            rightmost() = i.node;
        }

        // step 2. 寻找“替代”，若无则为nil
        if (cur->left && cur->right)
        {
            y = ret.node;
            x = y->right;
            origin_color = y->color;

            if (!x)
            {
                nil = create_nil(y);

                // x为伪节点nil
                x = nil;
            }

            if (y->parent != cur)
            {
                transplant(y, x);
                y->right = cur->right;
                y->right->parent = y;
            }
            transplant(cur, y);
            y->left = cur->left;
            y->left->parent = y;
            y->color = cur->color;
        }
        else
        {
            if (!cur->left && !cur->right)
            {
                nil = create_nil(cur);
                x = nil;
            }
            else if (!cur->left)
            {
                x = cur->right;
            }
            else
                x = cur->left;
            transplant(cur, x);
            if (cur == root())
                left(header) = right(header) = x;
        }

        // step 3. rebalance
        if (origin_color == Rb_tree_color::Black)
            rb_tree_erase_rebalance(x, header->parent);
        put_node(cur);
        --node_count;

        // step 4. if nil, delete it
        if (nil)
        {
            if (nil == root())
            {
                left(header) = right(header) = header;
                parent(header) = nullptr;
            }
            else
            {
                if (nil->parent->left == nil)
                    nil->parent->left = nullptr;
                else
                    nil->parent->right = nullptr;
            }

            put_node(static_cast<link_type>(nil));
            nil = nullptr;
        }

        assert(isValid(root()).second);

        return ret;
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) const
    {
        base_ptr y = header;
        base_ptr x = root();

        while (x)
        {
            if (!key_compare(key(x), k))
            {
                y = x;
                x = left(x);
            }
            else
                x = right(x);
        }

        iterator j = iterator(static_cast<link_type>(y));
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    std::pair<typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
              typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key &k) const
    {
        return {lower_bound(k), upper_bound(k)};
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key &k) const
    {
        base_ptr pre = header;
        base_ptr cur = root();

        while (cur)
        {
            if (!key_compare(key(cur), KeyOfValue()(k)))
            {
                pre = cur;
                cur = left(cur);
            }
            else
                cur = right(cur);
        }

        return const_iterator(static_cast<link_type>(pre));
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key &k) const
    {
        base_ptr pre = header;
        base_ptr cur = root();

        while (cur)
        {
            if (key_compare(KeyOfValue()(k), key(cur)))
            {
                pre = cur;
                cur = left(cur);
            }
            else
                cur = right(cur);
        }

        return const_iterator(static_cast<link_type>(pre));
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    stl::vector<std::pair<Value, int>>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::pre_traverse()
    {
        stl::vector<std::pair<Value, int>> info;
        stl::stack<link_type> stk;
        link_type p = static_cast<link_type>(root());

        while (p || !stk.empty())
        {
            if (p)
            {
                stk.push(p);
                info.push_back({key(p), !(color(p) == Rb_tree_color::Red)});
                p = static_cast<link_type>(p->left);
            }
            else
            {
                p = stk.top();
                stk.pop();
                p = static_cast<link_type>(p->right);
            }
        }

        return info;
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    stl::vector<std::pair<Value, int>>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::mid_traverse()
    {
        stl::vector<std::pair<Value, int>> info;
        stl::stack<link_type> stk;
        link_type p = static_cast<link_type>(root());

        while (p || !stk.empty())
        {
            if (p)
            {
                stk.push(p);
                p = static_cast<link_type>(p->left);
            }
            else
            {
                p = stk.top();
                stk.pop();
                info.push_back({key(p), !(color(p) == Rb_tree_color::Red)});

                p = static_cast<link_type>(p->right);
            }
        }

        return info;
    }

    template <typename Key, typename Value, typename KeyOfValue,
              typename Compare, typename Alloc>
    stl::vector<std::pair<Value, int>>
    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::post_traverse()
    {
        stl::vector<std::pair<Value, int>> info;
        stl::stack<link_type> stk;

        link_type p = static_cast<link_type>(root());
        link_type r = nullptr;

        while (p || !stk.empty())
        {
            if (p)
            {
                stk.push(p);
                p = static_cast<link_type>(p->left);
            }
            else
            {
                p = stk.top();
                if (p->right && p->right != r)
                    p = static_cast<link_type>(p->right);
                else
                {
                    info.push_back({key(p), !(color(p) == Rb_tree_color::Red)});

                    stk.pop();
                    r = p;
                    p = nullptr;
                }
            }
        }

        return info;
    }

} // namespace stl

#endif
