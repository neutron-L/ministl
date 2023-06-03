//
// Created by rda on 2023/6/3.
//

#ifndef MINISTL_LIST_HH
#define MINISTL_LIST_HH

#include "alloc.hh"
#include "iterator.hh"

namespace stl
{
    /* list node, contain 2 pointer and a data item */
    template <typename T>
    struct list_node
    {
        list_node *prev;
        list_node *next;
        T data;
    };

    /* list iterator, contain a pointer to node */
    template <typename T>
    struct list_iterator
    {
        using Self = list_iterator<T>;
        using Node = list_node<T>;

        using difference_type = std::ptrdiff_t;
        using iterator_category = stl::bidirectional_iterator_tag;

        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;

        list_iterator() = default;

        explicit list_iterator(list_node *n) : node(n) {}

        reference
        operator*() const
        {
            return node->data;
        }

        pointer
        operator->() const
        {
            return &(operator*());
        }

        Self &
        operator++()
        {
            node = node->next;
            return *this;
        }

        Self
        operator++(int)
        {
            Self temp = *this;
            node = node->next;
            return temp;
        }

        Self &
        operator--()
        {
            node = node->prev;
            return *this;
        }

        Self
        operator--(int)
        {
            Self temp = *this;
            node = node->prev;
            return temp;
        }

        friend bool
        operator==(const Self &lhs, const Self &rhs)
        {
            return lhs.node == rhs.node;
        }

        friend bool
        operator!=(const Self &lhs, const Self &rhs)
        {
            return lhs.node != rhs.node;
        }

        Node *node{};
    };

    /* list const iterator, contain a pointer to node */
    template <typename T>
    struct list_const_iterator
    {
        using Self = list_const_iterator<T>;
        using Node = const list_node<T>;
        using iterator = list_iterator<T>;

        using difference_type = std::ptrdiff_t;
        using iterator_category = stl::bidirectional_iterator_tag;

        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;

        list_const_iterator() = default;

        explicit list_const_iterator(const iterator &iter) : node(iter.node) {}

        reference
        operator*() const
        {
            return node->data;
        }

        pointer
        operator->() const
        {
            return &(operator*());
        }

        Self &
        operator++()
        {
            node = node->next;
            return *this;
        }

        Self
        operator++(int)
        {
            Self temp = *this;
            node = node->next;
            return temp;
        }

        Self &
        operator--()
        {
            node = node->prev;
            return *this;
        }

        Self
        operator--(int)
        {
            Self temp = *this;
            node = node->prev;
            return temp;
        }

        friend bool
        operator==(const Self &lhs, const Self &rhs)
        {
            return lhs.node == rhs.node;
        }

        friend bool
        operator!=(const Self &lhs, const Self &rhs)
        {
            return lhs.node != rhs.node;
        }

        const Node *node{};
    };

    template <typename T, typename Alloc = alloc>
    class list
    {
    public:
        /* Member types */
        using value_type = T;
        using allocator_type = Alloc;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

        using iterator = list_iterator<T>;
        using const_iterator = list_const_iterator<T>;
        using reverse_iterator = stl::reverse_iterator<iterator>;
        using const_reverse_iterator = stl::reverse_iterator<const_iterator>;

    protected:
        using Node = list_node<T>;
        using link_type = Node *;
        link_type head;

        using list_node_allocator = simple_alloc<Node, Alloc>;

        link_type get_node() { return list_const_iterator::allocate(); }
        void put_node(link_type p) { list_const_iterator::deallocate(p); }

        link_type create_node(const value_type &value)
        {
            link_type p = get_node();
            stl::construct(&p->data, value);
            return p;
        }

        void destroy_node()
        {
            stl::destroy(&p->data);
            put_node(p);
        }

        void empty_initialize()
        {
            head = get_node();
            head->prev = head->next = head;
        }

    public:
        /* Member functions */

        /*
         *  constructor
         * */
        list() = default;

        explicit list(size_type count, const T &value = T());

        template <typename InputIt>
        list(InputIt first, InputIt last);
        list(const list &other);
        list(list &&other);
        list(std::initializer_list<T> init);

        /*
         *  destructor
         * */
        ~list();

        /*
         * assignment operation
         * */
        list & operator=(const list & other);
        list & operator=(list && other) noexcept;
        list & operator=(std::initializer_list<T> ilist);
    };

} // namespace stl

#endif