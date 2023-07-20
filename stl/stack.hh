//
// Created by rda on 2023/7/18.
//

#ifndef MINISTL_STACK_HH
#define MINISTL_STACK_HH

#include "construct.hh"
#include "deque.hh"

namespace stl
{
    template <typename T, typename Container = stl::deque<T>>
    class stack
    {
    public:
        using container_type = Container;
        using value_type = typename Container::value_type;
        using size_type = typename Container::size_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;

    protected:
        Container container;

    public:
        /*
         * Constructors
         * */
        stack() : stack(Container()) {}
        explicit stack(const Container &cont) : container(cont) {}
        explicit stack(Container &&cont) : container(std::move(cont))
        {
        }
        stack(const stack &other) : container(other.container)
        {
        }
        stack(stack &&other) : container(std::move(other.container))
        {
        }
        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        stack(InputIt first, InputIt last) : container(first, last)
        {
        }

        /*
         * Destructor
         * */
        ~stack() = default;

        /*
         * assignment operation
         * */
        stack &operator=(const stack &other)
        {
            container = other.container;

            return *this;
        }
        stack &operator=(stack &&other)
        {
            container = std::move(other.container);

            return *this;
        }

        /*
         * Element access
         * */
        reference top()
        {
            return const_cast<reference>(const_cast<const stack &>(*this).top());
        }
        const_reference top() const
        {
            return container.back();
        }

        /*
         * Capacity
         * */
        bool empty() const
        {
            return container.empty();
        }
        size_type size() const
        {
            return container.size();
        }

        /*
         * Modifiers
         * */
        void push(const value_type &value)
        {
            container.push_back(value);
        }
        void push(value_type &&value)
        {
            container.push_back(std::move(value));
        }

        template <class... Args>
        void emplace(Args &&...args)
        {
            container.emplace_back(std::forward<Args>(args)...);
        }

        void pop()
        {
            container.pop_back();
        }

        void swap(stack &other) noexcept
        {
            container.swap(other.container);
        }
    };

} // namespace stl

#endif
