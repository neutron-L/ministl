//
// Created by rda on 2023/7/18.
//

#ifndef MINISTL_STACK_HH
#define MINISTL_STACK_HH

#include "alloc.hh"
#include "iterator.hh"
#include "construct.hh"
#include "algobase.hh"
#include "deque.hh"

namespace stl
{
    template <typename T, typename Container = stl::deque<T>>
    class stack
    {
    public:
        using container_type = Container;
        using value_type = Container::value_type;
        using size_type = Container::size_type;
        using reference = Container::reference;
        using const_reference = Container::const_reference;

        /*
         * Constructors
         * */
        explicit stack(const Container &cont);
        explicit stack(Container &&cont);
        stack(const stack &other);
        stack(stack &&other);
        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        stack(InputIt first, InputIt last);

        /*
         * Destructor
         * */
        ~stack();

        /*
         * assignment operation
         * */
        stack &operator=(const stack &other);
        stack &operator=(stack &&other);

        /*
         * Element access
         * */
        reference top();
        const_reference top() const;

        /*
         * Capacity
         * */
        bool empty() const;
        size_type size() const;

        /*
         * Modifiers
         * */
        void push(const value_type &value);
        void push(value_type &&value);

        template <class... Args>
        void emplace(Args &&...args);

        void pop();
        void swap(stack &other) noexcept;
    };

} // namespace stl

#endif
