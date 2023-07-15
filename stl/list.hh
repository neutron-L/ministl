//
// Created by rda on 2023/6/3.
//

#ifndef MINISTL_LIST_HH
#define MINISTL_LIST_HH

#include "alloc.hh"
#include "iterator.hh"
#include "construct.hh"
#include "algobase.hh"

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

        explicit list_iterator(const list_node<T> *n)
            : node(const_cast<Node *>(n)) {}

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

        explicit list_const_iterator(const list_node<T> *n) : node(n) {}
        list_const_iterator(const iterator &iter) : node(iter.node) {}

        reference
        operator*() const
        {
            return const_cast<reference>(node->data);
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
        size_type num_of_nodes{};

        using list_node_allocator = simple_alloc<Node, Alloc>;

        link_type get_node()
        {
            return list_node_allocator::allocate();
        }

        template <class... Args>
        link_type create_node(Args &&...args)
        {
            link_type node = get_node();
            stl::construct(&node->data, std::forward<Args>(args)...);

            return node;
        }

        void put_node(link_type p) { list_node_allocator::deallocate(p); }

        void destroy_node(link_type p)
        {
            stl::destroy(&p->data);
            put_node(p);
        }

        void empty_initialize()
        {
            head = get_node();
            head->prev = head->next = head;
        }

        void transfer(iterator pos, iterator first, iterator last)
        {
            if (pos != last && first != last)
            {
                auto prev_node = (link_type)(pos.node)->prev;
                auto last_prev_node = (link_type)(last.node)->prev;

                // 关联first的prev节点与last节点
                first.node->prev->next = last.node;
                last.node->prev = first.node->prev;

                prev_node->next = first.node;
                first.node->prev = prev_node;
                last_prev_node->next = pos.node;
                pos.node->prev = last_prev_node;
            }
        }

    public:
        /* Member functions */

        /*
         *  constructor
         * */
        list()
        {
            empty_initialize();
        }

        explicit list(size_type count, const T &value = T());

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        list(InputIt first, InputIt last)
        {
            empty_initialize();

            while (first != last)
            {
                link_type node = create_node(*first);

                auto end = head->prev;

                node->prev = end;
                end->next = node;

                node->next = head;
                head->prev = node;

                ++first;
                ++num_of_nodes;
            }
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
        list &operator=(const list &other);
        list &operator=(list &&other) noexcept;
        list &operator=(std::initializer_list<T> ilist);

        void assign(size_type count, const value_type &value);

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        void
        assign(InputIt first, InputIt last)
        {
            erase(cbegin(), cend());
            num_of_nodes = 0;

            while (first != last)
            {
                link_type node = create_node(*first);
                auto end = head->prev;

                node->prev = end;
                end->next = node;

                node->next = head;
                head->prev = node;

                ++first;
                ++num_of_nodes;
            }
        }

        void assign(std::initializer_list<T> ilist);

        allocator_type get_allocator() const noexcept
        {
            return allocator_type();
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

        iterator insert(const_iterator pos, const value_type &value);
        iterator insert(const_iterator pos, value_type &&value);
        iterator insert(const_iterator pos, size_type count, const value_type &value);

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            if (first != last)
            {
                iterator rt = insert(pos, *first++);
                pos = const_iterator(rt.node);
                ++pos;
                auto inserter = stl::insert_iterator(*this, iterator(pos.node));

                while (first != last)
                    *inserter = *first++;

                return rt;
            }
            else
                return iterator(pos.node);
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);

        template <class... Args>
        iterator emplace(const_iterator pos, Args &&...args);

        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        void push_back(const value_type &value);
        void push_back(value_type &&value);

        template <typename... Args>
        reference emplace_back(Args &&...args);

        void pop_back();

        void push_front(const T &value);
        void push_front(T &&value);

        template <class... Args>
        reference emplace_front(Args &&...args);

        void pop_front();

        void resize(size_type count);
        void resize(size_type count, const value_type &value);

        void swap(list &other) noexcept;

        /*
         * Operations
         * */
        void merge(list &other) { merge(std::move(other)); }
        void merge(list &&other);

        template <typename Compare>
        void merge(list &other, Compare comp);
        template <typename Compare>
        void merge(list &&other, Compare comp);

        void splice(const_iterator pos, list &other);
        void splice(const_iterator pos, list &&other);

        void splice(const_iterator pos, list &other, const_iterator it);
        void splice(const_iterator pos, list &&other, const_iterator it);

        void splice(const_iterator pos, list &other,
                    const_iterator first, const_iterator last);
        void splice(const_iterator pos, list &&other,
                    const_iterator first, const_iterator last);

        size_type remove(const T &value);
    
        template <class UnaryPredicate>
        size_type remove_if(UnaryPredicate p);

        void reverse() noexcept;

        size_type unique();

        template <typename BinaryPredicate>
        size_type unique(BinaryPredicate p);

        void sort();
        template <class Compare>
        void sort(Compare comp);
    };

    /*
     *  constructor
     * */
    template <typename T, typename Alloc>
    list<T, Alloc>::list(size_type count, const T &value)
    {
        empty_initialize();

        while (count--)
            insert(cend(), value);
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::list(const list &other)
    {
        empty_initialize();

        // 1.首先逐个节点赋值
        auto bg = begin();
        auto ed = end();
        auto cbg = other.cbegin();
        auto ced = other.cend();

        while (bg != ed && cbg != ced)
        {
            stl::destroy(&*bg);
            stl::construct(&*bg, *cbg);

            ++bg;
            ++cbg;
        }

        // 如果节点数不够，则插入新节点
        if (cbg != ced)
        {
            while (cbg != ced)
                push_back(*cbg++);
        }
        else if (bg != ed) // 如果有多余节点则释放掉
            erase(const_iterator(bg), const_iterator(ed));

        // update number of nodes
        num_of_nodes = other.num_of_nodes;
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::list(list &&other)
    {
        empty_initialize();

        transfer(end(), other.begin(), other.end());

        num_of_nodes = other.num_of_nodes;
        other.num_of_nodes = 0;
    }

    template <typename T, typename Alloc>
    list<T, Alloc>::list(std::initializer_list<T> init) : list(init.begin(), init.end())
    {
    }

    /*
     *  destructor
     * */
    template <typename T, typename Alloc>
    list<T, Alloc>::~list()
    {
        clear();
        put_node(head);
        num_of_nodes = 0;
    }

    /*
     * assignment operation
     * */
    template <typename T, typename Alloc>
    list<T, Alloc> &
    list<T, Alloc>::operator=(const list &other)
    {
        this->assign(other.begin(), other.end());
        return *this;
    }

    template <typename T, typename Alloc>
    list<T, Alloc> &
    list<T, Alloc>::operator=(list &&other) noexcept
    {
        // Free old storage
        clear();

        transfer(end(), other.begin(), other.end());

        num_of_nodes = other.num_of_nodes;
        other.num_of_nodes = 0;

        return *this;
    }

    template <typename T, typename Alloc>
    list<T, Alloc> &
    list<T, Alloc>::operator=(std::initializer_list<T> ilist)
    {
        this->assign(ilist.begin(), ilist.end());
        return *this;
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::assign(size_type count, const value_type &value)
    {
        clear();

        while (count--)
            insert(end(), value);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::assign(std::initializer_list<T> ilist)
    {
        this->assign(ilist.begin(), ilist.end());
    }

    /*
     * Element access
     * */
    template <typename T, typename Alloc>
    typename list<T, Alloc>::reference
    list<T, Alloc>::front()
    {
        return const_cast<reference>(const_cast<const list &>(*this).front());
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_reference
    list<T, Alloc>::front() const
    {
        return *begin();
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::reference
    list<T, Alloc>::back()
    {
        return const_cast<reference>(const_cast<const list &>(*this).back());
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_reference
    list<T, Alloc>::back() const
    {
        return *(--end());
    }

    /*
     * Iterator
     * */
    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::begin() noexcept
    {
        return iterator(head->next);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_iterator
    list<T, Alloc>::begin() const noexcept
    {
        return const_iterator(head->next);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_iterator
    list<T, Alloc>::cbegin() const noexcept
    {
        return const_iterator(head->next);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::end() noexcept
    {
        return iterator(head);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_iterator
    list<T, Alloc>::end() const noexcept
    {
        return const_iterator(head);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_iterator
    list<T, Alloc>::cend() const noexcept
    {
        return const_iterator(head);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::reverse_iterator
    list<T, Alloc>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_reverse_iterator
    list<T, Alloc>::rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_reverse_iterator
    list<T, Alloc>::crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::reverse_iterator
    list<T, Alloc>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_reverse_iterator
    list<T, Alloc>::rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::const_reverse_iterator
    list<T, Alloc>::crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    /*
     * Capacity
     * */
    template <typename T, typename Alloc>
    bool
    list<T, Alloc>::empty() const noexcept
    {
        return num_of_nodes == 0;
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::size_type
    list<T, Alloc>::size() const noexcept
    {
        return num_of_nodes;
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::size_type
    list<T, Alloc>::max_size() const noexcept
    {
        return static_cast<size_type>(-1l);
    }

    /*
     * Modifiers
     * */
    template <typename T, typename Alloc>
    void
    list<T, Alloc>::clear() noexcept
    {
        erase(cbegin(), cend());
        // update number of nodes
        num_of_nodes = 0;
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(const_iterator pos, const value_type &value)
    {
        value_type cv = value;
        return insert(pos, std::move(cv));
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(const_iterator pos, value_type &&value)
    {
        link_type cur = link_type(pos.node);

        auto node = create_node(std::forward<value_type>(value));

        node->prev = cur->prev;
        cur->prev->next = node;

        node->next = cur;
        cur->prev = node;

        // update number of nodes
        ++num_of_nodes;

        return iterator(node);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(const_iterator pos, size_type count, const value_type &value)
    {
        iterator it = iterator(pos.node);
        while (count--)
            pos = const_iterator(insert(pos, value).node);

        return iterator(pos.node);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(const_iterator pos, std::initializer_list<value_type> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template <typename T, typename Alloc>
    template <class... Args>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::emplace(const_iterator pos, Args &&...args)
    {
        return insert(pos, std::move(value_type(std::forward<Args>(args)...)));
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(const_iterator pos)
    {
        auto prev_node = link_type(pos.node)->prev;
        auto next_node = link_type(pos.node)->next;

        prev_node->next = next_node;
        next_node->prev = prev_node;

        destroy_node((link_type)(pos.node));

        // update number of nodes
        --num_of_nodes;

        return iterator(next_node);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(const_iterator first, const_iterator last)
    {
        while (first != last)
            first = const_iterator(erase(first).node);
        return iterator(first.node);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::push_back(const value_type &value)
    {
        insert(cend(), value);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::push_back(value_type &&value)
    {
        insert(end(), std::move(value));
    }

    template <typename T, typename Alloc>
    template <typename... Args>
    typename list<T, Alloc>::reference
    list<T, Alloc>::emplace_back(Args &&...args)
    {
        return *emplace(end(), std::forward<Args>(args)...);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::pop_back()
    {
        erase(--end());
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::push_front(const T &value)
    {
        insert(begin(), value);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::push_front(T &&value)
    {
        insert(begin(), std::move(value));
    }

    template <typename T, typename Alloc>
    template <class... Args>
    typename list<T, Alloc>::reference
    list<T, Alloc>::emplace_front(Args &&...args)
    {
        insert(begin(), std::move(value_type(std::forward<Args>(args)...)));
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::pop_front()
    {
        erase(begin());
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::resize(size_type count)
    {
        resize(count, value_type());
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::resize(size_type count, const value_type &value)
    {
        num_of_nodes = count;
        auto iter = this->begin();

        while (count)
        {
            if (iter != end() && *iter != value)
            {
                stl::destroy(&*iter);
                stl::construct(&*iter, value);
            }
            else
                iter = insert(iter, value);
            ++iter;
            --count;
        }

        erase(iter, end());
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::swap(list &other) noexcept
    {
        stl::swap(other.head, head);
        stl::swap(other.num_of_nodes, num_of_nodes);
    }

    /*
     * Operations
     * */

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::merge(list &&other)
    {
        merge(std::move(other), std::less<T>());
    }

    template <typename T, typename Alloc>
    template <typename Compare>
    void
    list<T, Alloc>::merge(list &other, Compare comp)
    {
        merge(std::move(other), comp);
    }

    template <typename T, typename Alloc>
    template <typename Compare>
    void
    list<T, Alloc>::merge(list &&other, Compare comp)
    {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = other.begin();
        iterator last2 = other.end();

        while (first1 != last1 && first2 != last2)
        {
            if (comp(*first2, *first1))
            {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;
        }

        transfer(last1, first2, last2);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::splice(const_iterator pos, list &other)
    {
        splice(pos, std::move(other));
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::splice(const_iterator pos, list &&other)
    {
        // this is a undefined behavior if other is itself, but we prevent this behavior
        if (&other == this)
            return;

        transfer(pos, other.begin(), other.end());
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::splice(const_iterator pos, list &other, const_iterator it)
    {
        splice(pos, std::move(other), it);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::splice(const_iterator pos, list &&other, const_iterator it)
    {
        // this is a undefined behavior if other is itself, but we prevent this behavior
        if (&other == this)
            return;
        iterator nit = it;
        ++nit;

        if (pos == it || pos == nit)
            return;
        transfer(pos, it, ++it);
    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::splice(const_iterator pos, list &other,
                           const_iterator first, const_iterator last)
    {
        splice(pos, std::move(other), first, last);
    }
    template <typename T, typename Alloc>
    void
    list<T, Alloc>::splice(const_iterator pos, list &&other,
                           const_iterator first, const_iterator last)
    {
        // this is a undefined behavior if other is itself, but we prevent this behavior
        if (&other == this)
            return;
        transfer(pos, first, last);
    }


    template <typename T, typename Alloc>
    typename list<T, Alloc>::size_type 
    list<T, Alloc>::remove(const T &value)
    {
        iterator first = begin();
        iterator last = end();
        size_type rt = 0;

        while (first != last)
        {
            if (*first == value)
            {
                first = erase(first);
                ++rt;
            }
            else
                ++first;
        }
        
        return rt;
    }

    
    template <typename T, typename Alloc>
    template <class UnaryPredicate>
    typename list<T, Alloc>::size_type 
    list<T, Alloc>::remove_if(UnaryPredicate p)
    {

    }

    template <typename T, typename Alloc>
    void
    list<T, Alloc>::reverse() noexcept
    {
        iterator first = begin();
        iterator last = end();
        auto old = first;

        while (++first != last)
        {
            transfer(begin(), old, first);
            old = first;
        }
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::size_type
    list<T, Alloc>::unique()
    {
        iterator first = begin();
        iterator last = end();
        size_type old_num = num_of_nodes;

        auto next = first;
        while (++next != last)
        {
            if (*first == *next)
                erase(next);
            else
                first = next;
            next = first;
        }

        return old_num - num_of_nodes;
    }

} // namespace stl

#endif
