//
// Created by rda on 2023/6/3.
//

#ifndef MINISTL_LIST_HH
#define MINISTL_LIST_HH

#include "alloc.hh"
#include "iterator.hh"

namespace stl
{
    template <typename T>
    struct list_node
    {
        list_node * prev;
        list_node * next;
        T data;
    };

    template<typename T>
    struct list_iterator
    {
        using Self = list_iterator<T>;
        using Node = list_node<T>;

        using difference_type = std::ptrdiff_t;
        using iterator_category = stl::bidirectional_iterator_tag;
        
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;

        Node * node;
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

        /* Member functions */
        list()=default;

        explicit list(size_type count, const T & value=T());

        template<typename InputIt>
        list(InputIt first, InputIt last);
        list(const list & other);
        list(list && other);
        list(std::initializer_list<T> init);

        /*
         *  constructor
         * */



        /*
         *  destructor
         * */

        /*
         * assignment operation
         * */
    };

} // namespace stl

#endif