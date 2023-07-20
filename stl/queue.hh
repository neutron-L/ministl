//
// Created by rda on 2023/7/20.
//

#ifndef MINISTL_QUEUE_HH
#define MINISTL_QUEUE_HH

#include "construct.hh"
#include "deque.hh"

namespace stl
{
    template <typename T, typename Container = stl::deque<T>>
    class queue
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
        queue() : queue(Container()) {}
        explicit queue(const Container &cont) : container(cont) {}
        explicit queue(Container &&cont) : container(std::move(cont))
        {
        }
        queue(queue &&other) : container(other.container)
        {
        }
        queue(queue &&other) : container(std::move(other.container))
        {
        }
        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        queue(InputIt first, InputIt last) : container(first, last)
        {
        }

        /*
         * Destructor
         * */
        ~queue() = default;

        /*
         * assignment operation
         * */
        queue &operator=(const queue &other)
        {
            container = other.container;

            return *this;
        }
        queue &operator=(queue &&other)
        {
            container = std::move(other.container);

            return *this;
        }

        /*
         * Element access
         * */
        reference front()
        {
            return const_cast<reference>(const_cast<const stack &>(*this).front());
        }
        const_reference front() const
        {
            return container.front();
        }
        reference back()
        {
            return const_cast<reference>(const_cast<const stack &>(*this).front());
        }
        const_reference back() const
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
        void swap(queue &other) noexcept
        {
            container.swap(other.container);
        }
    };

} // namespace stl

#endif
