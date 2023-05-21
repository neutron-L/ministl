#ifndef MINISTL_LIST_HH
#define MINISTL_LIST_HH

#include "alloc.hh"

namespace stl
{
    template<typename T, typename Alloc = alloc>
    class list
    {
    private:
        template<F>
        struct list_node
        {
            using void_pointer = void *;
            void_pointer prev;
            void_pointer next;
            F data;
        };

        template<typename F, typename Ref, typename Ptr>
        struct list_iterator
        {
            using iterator = list_iterator<F,F&, F*>;
            using self = list_iterator<F, Ref, Ptr>;
            using iterator_category = stl::bidirectional_iterator_tag;
            using value_type = F;
            using pointer = Ptr;
            using reference =  Ref;
            using link_type = list_node<F> *;
            using size_type = size_t;
            using difference_type = ptrdiff_t;

            link_type node; // point to normal node

            /* constructor */
            list_iterator(link_type x) : node(x) {}
            list_iterator() {} 
            list_iterator(const iterator& x) : node(x.node) {}

            /* non-modifying operation */
            bool operator==(const self & x) const { return node == x.node; }
            bool operator!=(const self & x) const { return node != x.node; }
        
            reference operator*() const { return (*node).data; }
            pointer operator->() const { return & (operator*()); }


            /* modifying operation */
            self & operator++() 
            {
                node = (link_type)((*node).next);
                return *this;
            }

            self operator++(int) 
            {
                self tmp = *this;
                node = (link_type)((*node).next);
                return tmp;
            }

            self & operator--() 
            {
                node = (link_type)((*node).prev);
                return *this;
            }

            self operator--(int) 
            {
                self tmp = *this;
                node = (link_type)((*node).prev);
                return tmp;
            }

        };
    public:

    protected:
    public:
    };

} // namespace stl


#endif