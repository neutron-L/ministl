//
// Created by rda on 2023/9/14.
//

#ifndef MINISTL_RBTREE_HH
#define MINISTL_RBTREE_HH

#include "alloc.hh"
#include "construct.hh"
#include "iterator.hh"

namespace stl
{
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
        Value value_field;
    };

    // RB-Tree iterator base class
    struct Rb_tree_base_iterator
    {
        using base_ptr = Rb_tree_node_base::base_ptr;
        using iterator_category = stl::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;

        base_ptr node;

        void increment()
        {
        }

        void decrement()
        {
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

        reference operator*() const { return link_type(node)->value_field; }

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

        self &operator++()
        {
            decrement();
            return *this;
        }
        self operator++(int)
        {
            auto obj = *this;
            decrement();
            return obj;
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

    protected:
        using base_ptr = Rb_tree_node_base *;
        using rb_tree_node = Rb_tree_node<value_type>;
        using color_type = Rb_tree_color;
        using rb_tree_node_allocator = simple_alloc<rb_tree_node, Alloc>;

        link_type header{};
        size_type node_count{};
        Compare key_compare{};

        link_type get_node() { return rb_tree_node_allocator::allocate(); }
        void put_node(link_type p) { rb_tree_node_allocator::deallcate(p); }

        link_type create_node(const value_type &val)
        {
            link_type node = get_node();
            stl::construct(&node->value_field, val);

            return node;
        }

        link_type clone_node(link_type p)
        {
            link_type node = create_node(p->value_field);
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
        link_type &root() const { return header->parent; }
        link_type &leftmost() const { return header->left; }
        link_type &rightmost() const { return header->right; }

        // get the members of node
        static link_type &left(link_type p)
        {
            try
            {
                return dynamic_cast<link_type &>(p->left);
            }
            catch (std::bad_cast)
            {
                std::cerr << "bad cast " << endl;
                exit(-1);
            }
        }

        // find the max/min node
        static link_type minimum(link_type p)
        {
            return dynamic_cast<link_type>(Rb_tree_node_base::minimum(p));
        }

        static link_type maximum(link_type p)
        {
            return dynamic_cast<link_type>(Rb_tree_node_base::maximum(p));
        }

    public:
        using iterator = Rb_tree_iterator<value_type, reference, pointer>::iterator;
        using const_iterator = Rb_tree_iterator<value_type, reference, pointer>::const_iterator;

    private:
        itrator insert(base_ptr x, base_ptr y, const value_type &v);
        link_type copy(link_type x, link_type y);
        void erase(link_type x);

        void init()
        {
            header = get_node();

            color(header) = color_type::Red;
            root() = nullptr;
            leftmost() = rightmost() = header;
        }

        void rb_tree_rebalance(Rb_tree_node_base * p, Rb_tree_node_base *& root);
        void rb_tree_rotate_left(Rb_tree_node_base * p, Rb_tree_node_base *& root);
        void rb_tree_rotate_right(Rb_tree_node_base * p, Rb_tree_node_base *& root);


    public:
        Rb_tree(const Compare &comp = Compare())
            : key_compare(com)
        {
            init();
        }

        Compare key_comp() const { return key_comp; }
        iterator begin() const { return leftmost(); }
        iterator end() const { return header; }
        bool empty() const { return node_count == 0; }
        size_type size() const { return node_count; }
        size_type max_size() const { return static_cast<size_type>(-1); }

    public:
        pair<iterator, bool> insert_unique();
        iterator insert_equal();


    };

} // namespace stl

#endif
