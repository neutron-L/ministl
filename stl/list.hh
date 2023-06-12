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
        list_node *prev{};
        list_node *next{};
        T data{};

        list_node() 
        {
            prev = next = this;
        }
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
        link_type head{};

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
        list() : head(new Node());

        explicit list(size_type count, const T &value = T());

        template <typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
        list(InputIt first, InputIt last) : head(new Node())
        {
            while (first != last)
                insert(end(), *first++);
        }

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
    
        void assign(size_type count, const value_type & value);

        template <typename T, typename Alloc>
        template<typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void 
        list<T, Alloc>::assign(InputIt first, InputIt last)
        {
            erase(bein(), end());
            while (first != last)
                insert(end(), *first++);
        }

        void assign(std::initializer_list<T> ilist);

        allocator_type get_allocator() const noexcept
        {
            return list_node_allocator;
        }

        /* 
         * Element access 
         * */
        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        /* 
         * Iterator 
         * */
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;

        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

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

        iterator insert(const_iterator pos, const value_type & value);

    };

     /*
      *  constructor
      * */
    template <typename T, typename Alloc>
    list<T, Alloc>::list(size_type count, const T &value = T()) : head(new Node())
    {
        while (count--)
        {
            /* code */
            insert(end(), value);
        }
    }



    template <typename T, typename Alloc>
    list<T, Alloc>::list(const list &other) : head(other.head)
    {
        other.head = nullptr;
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::list(list &&other) : head(other.head)
    {
        other.head = nullptr;
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::list(std::initializer_list<T> init) : list(init.begin(), init.end())
    {
    }

    /*
     * assignment operation
     * */
    template <typename T, typename Alloc>
    list<T, Alloc> & 
    list<T, Alloc>::operator=(const list & other)
    {
        this->assign(other.begin(), other.end());
        return *this;
    }

    template <typename T, typename Alloc>
    list<T, Alloc> & 
    list<T, Alloc>::operator=(list && other) noexcept
    {
        // release old storage
        erase(begin(), end());

        head = other.head;
        other.head = nullptr;

        return *this;
    }

    template <typename T, typename Alloc>
    list<T, Alloc> & 
    list<T, Alloc>::operator=(std::initializer_list<T> ilist)
    {

    }

    template <typename T, typename Alloc>
    void 
    list<T, Alloc>::assign(size_type count, const value_type & value)
    {

    }

    
    template <typename T, typename Alloc>
    void 
    list<T, Alloc>::assign(std::initializer_list<T> ilist)
    {
        
    }

    /* 
     * Element access 
     * */
    template <typename T, typename Alloc>
    list<T, Alloc>::reference 
    list<T, Alloc>::front()
    {
        return const_cast<reference>(const_cast<const list&>(*this).front());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_reference 
    list<T, Alloc>::front() const
    {
        return *begin();
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::reference 
    list<T, Alloc>::back()
    {
        return const_cast<reference>(const_cast<const list&>(*this).back());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_reference 
    list<T, Alloc>::back() const
    {
        return *(--end());
    }

    /* 
     * Iterator 
     * */
    template <typename T, typename Alloc>
    list<T, Alloc>::iterator 
    list<T, Alloc>::begin() noexcept
    {
        return const_cast<iterator>(const_cast<const list &>(*this).begin());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_iterator 
    list<T, Alloc>::begin() const noexcept
    {
        return const_iterator(head->next);
    }
    
    
    template <typename T, typename Alloc>
    list<T, Alloc>::const_iterator 
    list<T, Alloc>::cbegin() const noexcept
    {
        return const_iterator(head->next);
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::iterator 
    list<T, Alloc>::end() noexcept
    {
        return const_cast<iterator>(const_cast<const list &>(*this).begin());
    }


    template <typename T, typename Alloc>
    list<T, Alloc>::const_iterator 
    list<T, Alloc>::end() const noexcept
    {
        return const_iterator(head);
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_iterator 
    list<T, Alloc>::cend() const noexcept
    {
        return const_iterator(head);
    }


    template <typename T, typename Alloc>
    list<T, Alloc>::reverse_iterator 
    list<T, Alloc>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_reverse_iterator 
    list<T, Alloc>::rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_reverse_iterator 
    list<T, Alloc>::crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::reverse_iterator 
    list<T, Alloc>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_reverse_iterator 
    list<T, Alloc>::rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::const_reverse_iterator 
    list<T, Alloc>::crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

} // namespace stl

#endif
