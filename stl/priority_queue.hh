//
// Created by rda on 2023/7/21.
//

#ifndef MINISTL_PRIORITY_QUEUE_HH
#define MINISTL_PRIORITY_QUEUE_HH

#include "construct.hh"
#include "heap.hh"
#include "vector.hh"

namespace stl
{
    template <typename T, typename Container = stl::vector<T>, typename Compare = std::less<typename Container::value_type>>
    class priority_queue
    {
    public:
        using container_type = Container;
        using value_compare = Compare;
        using value_type = typename Container::value_type;
        using size_type = typename Container::size_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;

    protected:
        Container container; // 内部存储容器
        value_compare comp;  // 比较对象成员

    public:
        /*
         * Constructors
         * */
        priority_queue() : priority_queue(Compare(), Container()) {}

        explicit priority_queue(const Compare &compare)
            : priority_queue(compare, Container()) {}

        priority_queue(const Compare &compare, const Container &cont)
            : comp(compare), container(cont)
        {
            stl::make_heap(cont.begin(), cont.end(), comp);
        }

        priority_queue(const Compare &compare, Container &&cont)
            : comp(compare), container(std::move(cont))
        {
            stl::make_heap(cont.begin(), cont.end(), comp);
        }

        priority_queue(const priority_queue &other) = default;

        priority_queue(priority_queue &&other)
            : comp(std::move(other.compare)), container(std::move(other.cont))
        {
            stl::make_heap(cont.begin(), cont.end(), comp);
        }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        priority_queue(InputIt first, InputIt last,
                       const Compare &compare = Compare())
            : priority_queue(compare)
        {
            container.insert(container.end(), first, last);
            stl::make_heap(cont.begin(), cont.end(), comp);
        }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        priority_queue(InputIt first, InputIt last,
                       const Compare &compare, const Container &cont)
            : priority_queue(compare, cont)
        {
            container.insert(container.end(), first, last);
            stl::make_heap(cont.begin(), cont.end(), comp);
        }

        template <typename InputIt, typename = std::_RequireInputIter<InputIt>>
        priority_queue(InputIt first, InputIt last,
                       const Compare &compare, Container &&cont)
            : priority_queue(compare, std::move(cont))
        {
            container.insert(container.end(), first, last);
            stl::make_heap(cont.begin(), cont.end(), comp);
        }

        /*
         * Destructor
         * */
        ~priority_queue() = default;

        /*
         * assignment operation
         * */
        priority_queue &operator=(const priority_queue &other) = default;
        priority_queue &operator=(priority_queue &&other) = default;

        /*
         * Element access
         * */
        const_reference top() const
        {
            return container.front();
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
            value_type val = value;
            push(std::move(value));
        }
        void push(value_type &&value)
        {
            container.push_back(std::move(value));
            push_heap(container.begin(), container.end(), comp);
        }

        template <class... Args>
        void emplace(Args &&...args)
        {
            container.emplace(std::forward<Args>(args)...);
            push_heap(container.begin(), container.end(), comp);
        }

        void pop()
        {
            pop_heap(container.begin(), container.end(), comp);
            container.pop_back();
        }

        void swap(priority_queue &other) noexcept
        {
            container.swap(other.container);
        }
    };

} // namespace stl

#endif
